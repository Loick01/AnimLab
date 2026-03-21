#include "chain.hpp"

Chain::Chain(const sf::Vector2f origin, const unsigned int nrJoint, const unsigned int initialLength):
    m_origin(origin)
{
    if (nrJoint < 2) throw std::invalid_argument("A chain must have at least 2 joints\n");
    Joint currentJoint(m_origin);
    float localAngle = -90.f; // First link only
    float worldAngle = 0.f; 
    for (unsigned int i = 0 ; i < nrJoint-1 ; i++) {
        Link l(currentJoint, radians(localAngle), radians(worldAngle), initialLength);
        m_links.push_back(l);
        currentJoint = l.end;
        worldAngle += localAngle;
        localAngle = 0.f;
    }
}

sf::Vector2f Chain::Normalize(const sf::Vector2f v)
{
    const float norm = std::sqrt(v.x*v.x+v.y*v.y);
    if (norm == 0) throw std::runtime_error("Vector length should not be null\n");
    return v/norm;
}

unsigned int Chain::GetNrLink() const
{
    return m_links.size();
}

void Chain::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (const Link& l : m_links) {
        target.draw(l.start.circle, states);
        target.draw(l.end.circle, states);
        target.draw(l.line, 2, sf::Lines);
    }
}

FKChain::FKChain(const sf::Vector2f origin, const unsigned int nrJoint, const unsigned int initialLength):
    Chain(origin, nrJoint, initialLength)
{}

void FKChain::Update(const Time& time)
{
    const float mappedValue = sin(time.GetElapsedTime())*8; // [-8; 8]
    for (unsigned int i = 1 ; i < GetNrLink() ; i++)
        SetAngleAt(i, radians(mappedValue));
}

void FKChain::SetAngleAt(const unsigned int index, const float newLocalAngle)
{
    if (index >= m_links.size()) throw std::invalid_argument("Index is greater than m_links\n");
    m_links[index].localAngle = newLocalAngle;
    m_links[index].ComputeEndWithAngle();
    unsigned int currentIndex = index;
    float worldAngle = m_links[index].worldAngle + m_links[index].localAngle; 
    for (unsigned int i = index+1 ; i < m_links.size() ; i++){
        m_links[i].start = m_links[currentIndex].end;
        m_links[i].worldAngle = worldAngle;
        m_links[i].ComputeEndWithAngle();
        worldAngle += m_links[i].localAngle;
        currentIndex = i;
    }
}

IKChain::IKChain(const EventController& eventController, const sf::Vector2f origin, const unsigned int nrJoint, const unsigned int initialLength):
    Chain(origin, nrJoint, initialLength), m_eventController(eventController)
{}

void IKChain::Update(const Time& time)
{
    const float elapsedTime = time.GetElapsedTime();
    // sf::Vector2f targetPosition = {960 + 500 * (float)cos(elapsedTime), 700 + 200 * (float)sin(elapsedTime*2.)};
    sf::Vector2f targetPosition = sf::Vector2f(m_eventController.GetMousePosition());
    
    // Forward pass
    const unsigned int lastIndex = m_links.size()-1; // Last link index of the chain
    m_links[lastIndex].SetEndPosition(targetPosition);

    for (int i = lastIndex ; i >= 0 ; i--) {
        const sf::Vector2f endPosition = m_links[i].end.position;
        const sf::Vector2f startPosition = m_links[i].start.position;
        const sf::Vector2f endToStart = Normalize(startPosition - endPosition);
        const sf::Vector2f newPos = endPosition + endToStart*m_links[i].length;
        m_links[i].SetStartPosition(newPos);
        if (i != 0) m_links[i-1].end = m_links[i].start;
    }

    // Backward pass
    m_links[0].SetStartPosition(m_origin);
    for (int i = 0 ; i < m_links.size() ; i++) {
        const sf::Vector2f endPosition = m_links[i].end.position;
        const sf::Vector2f startPosition = m_links[i].start.position;
        const sf::Vector2f startToEnd = Normalize(endPosition - startPosition);
        const sf::Vector2f newPos = startPosition + startToEnd*m_links[i].length;
        m_links[i].SetEndPosition(newPos);
        if (i != m_links.size()-1) m_links[i+1].start = m_links[i].end;
    }

}