#include "chain.hpp"

Chain::Chain(const sf::Vector2f origin, const unsigned int nrJoint, const unsigned int initialLength):
    m_origin(origin), m_jointColor(sf::Color::White), m_nrJoint(nrJoint), m_initialLength(initialLength) // Default joint color and initial link length will not stay here
{
    if (nrJoint < 2) throw std::invalid_argument("A chain must have at least 2 joints\n");
    Joint currentJoint(m_origin, m_jointColor);
    float localAngle = -90.f; // First link only
    float worldAngle = 0.f; 
    for (unsigned int i = 0 ; i < nrJoint-1 ; i++) {
        Link l(currentJoint, m_jointColor, radians(localAngle), radians(worldAngle), m_initialLength);
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

sf::Color Chain::GetColor() const
{
    return m_jointColor;
}

unsigned int Chain::GetNrLink() const
{
    return m_links.size();
}

void Chain::UpdateJointColor()
{
    for (Link& l : m_links) { // Update joints with the same color (later every joint will have his own color)
        l.start.SetColor(m_jointColor);
        l.end.SetColor(m_jointColor);
    }
}

void Chain::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (const Link& l : m_links) {
        target.draw(l.start.circle, states);
        target.draw(l.end.circle, states);
        target.draw(l.line, 2, sf::Lines);
    }
}

void Chain::AddJoint()
{
    const Link lastLink = m_links.back();
    const Joint lastJoint = lastLink.end;
    const float currentAngle = lastLink.worldAngle + lastLink.localAngle;
    Link l(lastJoint, m_jointColor, radians(0.f), radians(currentAngle), m_initialLength);
    m_links.push_back(l);
}

void Chain::RemoveJoint()
{
    m_links.pop_back();
}

void Chain::SetOrigin(const sf::Vector2f origin)
{
    m_origin = origin;
}

void Chain::SetElementGUI()
{
    if (ImGui::SliderInt("Number of joint", &m_nrJoint, 2, 100)){
        const unsigned int nrLink = m_links.size();
        if (m_nrJoint > nrLink+1){
            for (unsigned int i = 0 ; i < m_nrJoint-nrLink-1 ; i++)
                AddJoint();
        }else if (m_nrJoint <= nrLink){
            for (unsigned int i = 0 ; i < nrLink+1-m_nrJoint ; i++)
                RemoveJoint();
        }
    }

    float c[3] = {m_jointColor.r/255.f, m_jointColor.g/255.f, m_jointColor.b/255.f};
    if (ImGui::ColorEdit3("Joint color", c)) {
        m_jointColor = {
            static_cast<std::uint8_t>(c[0]*255), 
            static_cast<std::uint8_t>(c[1]*255),
            static_cast<std::uint8_t>(c[2]*255)
        };
        UpdateJointColor();
    }
}

void Chain::SetAngleGUI() 
{   
    for (unsigned int i = 0 ; i < m_links.size() ; i++) {
        const Link l = m_links[i];
        ImGui::Text("Link %d : World Angle = %f / Local Angle = %f", i, degrees(l.worldAngle), degrees(l.localAngle));
    }
}

void Chain::ComputeLinkAngle(const sf::Vector2f v1, const sf::Vector2f v2, const unsigned int linkIndex)
{
    m_links[linkIndex].worldAngle = -atan2(v1.y, v1.x); // Angle (CCW) from X axis to the vector v1
    if (linkIndex==0) {
        m_links[linkIndex].localAngle = m_links[linkIndex].worldAngle;
    } else {
        const float cross = v1.x * v2.y - v1.y * v2.x;
        const float dot = v1.x * v2.x + v1.y * v2.y;
        m_links[linkIndex].localAngle = atan2(cross, dot); // [-PI, PI]
    }
    // Angle (CCW) from v2 to v1 should be PI + m_links[linkIndex].localAngle
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

void FKChain::SetElementGUI()
{
    Chain::SetElementGUI();
    // Nothing yet
}

IKChain::IKChain(const TargetMode targetMode, const unsigned int nrJoint, const unsigned int initialLength, 
const EventController* eventController, const sf::Vector2f origin) :
    Chain(origin, nrJoint, initialLength), m_eventController(eventController),
    m_targetMode(targetMode), m_isAimingMouse(m_targetMode == TargetMode::AimingMouse), m_doBackwardPass(true)
{}

sf::Vector2f IKChain::GetTargetPosition(const float elapsedTime) const
{
    switch(m_targetMode) {
        case TargetMode::AimingMouse: {
            if (m_eventController == nullptr) {
                throw std::runtime_error("EventController should not be nullptr with AimingMouse mode\n"); // Because Body need IKChain without EventController
            }
            return sf::Vector2f(m_eventController->GetMousePosition());
        }
        case TargetMode::Rotating:
            return sf::Vector2f{960 + 500 * (float)cos(elapsedTime), 700 + 200 * (float)sin(elapsedTime*2.)};
        case TargetMode::Walking:
            return m_currentTarget;
        default:
            throw std::invalid_argument("Unknown target mode");
    }
}

sf::Vector2f IKChain::GetCurrentTarget() const
{
    return m_currentTarget;
}

void IKChain::SetCurrentTarget(const sf::Vector2f target)
{
    m_currentTarget = target;
}

void IKChain::Update(const Time& time)
{
    sf::Vector2f targetPosition = GetTargetPosition(time.GetElapsedTime());
    
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

        const sf::Vector2f previousLinkStartToEnd = m_links[i-1].end.position - m_links[i-1].start.position; // Can't normalize 
        ComputeLinkAngle(-endToStart, previousLinkStartToEnd, i);
    }

    // Backward pass
    if (m_doBackwardPass) {
        m_links[0].SetStartPosition(m_origin);
        for (int i = 0 ; i < m_links.size() ; i++) {
            const sf::Vector2f endPosition = m_links[i].end.position;
            const sf::Vector2f startPosition = m_links[i].start.position;
            const sf::Vector2f startToEnd = Normalize(endPosition - startPosition);
            const sf::Vector2f newPos = startPosition + startToEnd*m_links[i].length;
            m_links[i].SetEndPosition(newPos);
            if (i != m_links.size()-1) m_links[i+1].start = m_links[i].end;

            const sf::Vector2f previousLinkStartToEnd = m_links[i-1].end.position - m_links[i-1].start.position; // Can't normalize 
            ComputeLinkAngle(startToEnd, previousLinkStartToEnd, i);
        }
    }
}

void IKChain::SetElementGUI()
{
    Chain::SetElementGUI();

    // Use a ImGui::BeginCombo for more than 2 modes
    if (ImGui::RadioButton("Aim mouse cursor", m_isAimingMouse)) {
        m_isAimingMouse = true;
        m_targetMode = TargetMode::AimingMouse;
    }
    if (ImGui::RadioButton("Compute target with elapsed time", !m_isAimingMouse)) {
        m_isAimingMouse = false;
        m_targetMode = TargetMode::Rotating;
    }

    ImGui::Checkbox("Perform backward pass", &m_doBackwardPass);
}