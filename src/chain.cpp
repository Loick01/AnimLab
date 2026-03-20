#include "chain.hpp"

Chain::Chain(const Vec2 origin, const unsigned int nrJoint, const unsigned int initialLength):
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

unsigned int Chain::GetNrLink() const
{
    return m_links.size();
}

void Chain::SetAngleAt(const unsigned int index, const float newLocalAngle)
{
    if (index >= m_links.size()) throw std::invalid_argument("Index is greater than m_links\n");
    m_links[index].localAngle = newLocalAngle;
    m_links[index].ComputeEnd();
    unsigned int currentIndex = index;
    float worldAngle = m_links[index].worldAngle + m_links[index].localAngle; 
    for (unsigned int i = index+1 ; i < m_links.size() ; i++){
        m_links[i].start = m_links[currentIndex].end;
        m_links[i].worldAngle = worldAngle;
        m_links[i].ComputeEnd();
        worldAngle += m_links[i].localAngle;
        currentIndex = i;
    }
}

void Chain::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (const Link& l : m_links) {
        target.draw(l.start.circle, states);
        target.draw(l.end.circle, states);
    }
}