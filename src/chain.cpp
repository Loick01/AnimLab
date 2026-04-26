#include "chain.hpp"

Chain::Chain(const sf::Vector2f origin, const unsigned int nrJoint, const unsigned int initialLength):
    m_origin(origin), m_jointColor(sf::Color::White), m_nrJoint(nrJoint), m_initialLength(initialLength) // Default joint color and initial link length will not stay here
{
    if (nrJoint < 2) throw std::invalid_argument("A chain must have at least 2 joints\n");
    Joint j(m_origin, m_jointColor);
    float angle = 90.f;
    for (unsigned int i = 0 ; i < nrJoint-1 ; i++) {
        Link l(j, m_jointColor, radians(angle), m_initialLength);
        m_links.push_back(l);
        j = l.end;
    }
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
    Link l(lastJoint, m_jointColor, radians(lastLink.angle), m_initialLength);
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
        ImGui::Text("Link %d : Angle = %f", i, degrees(l.angle));
    }
}

FKChain::FKChain(const sf::Vector2f origin, const unsigned int nrJoint, const unsigned int initialLength):
    Chain(origin, nrJoint, initialLength), m_amplitude(20.f)
{}

void FKChain::Update(const Time& time)
{
    const float value = sin(time.GetElapsedTime()+M_PI/2.)*m_amplitude;
    for (unsigned int i = 1 ; i < GetNrLink() ; i++)
        AddAngleAt(i, radians(value)*time.GetDeltaTime().asSeconds());
}


void FKChain::SetAngleAt(const unsigned int index, const float angle)
{
    if (index >= m_links.size()) throw std::invalid_argument("Index is greater than m_links\n");
    const float deltaAngle = angle - m_links[index].angle;
    unsigned int currentIndex = index;
    m_links[index].angle = angle;
    m_links[index].ComputeEnd();
    for (unsigned int i = index+1 ; i < m_links.size() ; i++){
        m_links[i].start = m_links[currentIndex].end;
        m_links[i].angle += deltaAngle;
        m_links[i].ComputeEnd();
        currentIndex = i;
    }
}

void FKChain::AddAngleAt(const unsigned int index, const float angle)
{
    if (index >= m_links.size()) throw std::invalid_argument("Index is greater than m_links\n");
    unsigned int currentIndex = index;
    m_links[index].angle += angle;
    m_links[index].ComputeEnd();
    for (unsigned int i = index+1 ; i < m_links.size() ; i++){
        m_links[i].start = m_links[currentIndex].end;
        m_links[i].angle += angle;
        m_links[i].ComputeEnd();
        currentIndex = i;
    }
}

void FKChain::SetElementGUI()
{
    Chain::SetElementGUI();
    
    ImGui::SliderFloat("Amplitude", &m_amplitude, 1.f, 50.f);
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

    // Each link will have its own constraints
    // const float constraintMin = radians(15.f);
    // const float constraintMax = radians(300.f);
    // const float epsilon = 0.01f;

    for (int i = lastIndex ; i >= 0 ; i--) {
        const sf::Vector2f linkEnd = m_links[i].end.position;
        const sf::Vector2f direction = m_links[i].GetDirection();
        const sf::Vector2f newPos = linkEnd - direction * m_links[i].length;
        m_links[i].SetStartPosition(newPos);
        if (i != 0) m_links[i-1].end = m_links[i].start;

        m_links[i].angle = ComputeAngle(direction);
        float a = 0.f;
        if (i == 0) {
            a = m_links[i].angle;
        } else {
            const sf::Vector2f pld = -m_links[i-1].GetDirection(); // Previous link reversed direction
            const float cross = pld.x * direction.y - pld.y * direction.x;
            const float dot = pld.x * direction.x + pld.y * direction.y;
            a = -atan2(cross, dot); // Angle CCW ([-PI, PI]) from previous link (i-1) to current link (i)
        }

        if (a < 0.) { a += 2 * M_PI; } // [0, 2PI]
        m_links[i].angle = a; // Will be removed

        // const float angle = M_PI + m_links[i].localAngle; // Angle from m_links[i-1] to m_links[i]
        // const float clamped = std::clamp(angle, constraintMin, constraintMax);
        // const float delta = clamped - angle;
        // if (abs(delta) > epsilon) { // If the angle have been clamped
        //     const float cosA = cos(delta);
        //     const float sinA = sin(delta);
        //     const sf::Vector2f newV = {startToEnd.x * cosA - startToEnd.y * sinA, startToEnd.x * sinA + startToEnd.y * cosA};
        //     m_links[i].SetEndPosition(newPos + newV * m_links[i].length);
        // }
    }

    // Backward pass
    if (m_doBackwardPass) {
        m_links[0].SetStartPosition(m_origin);
        for (int i = 0 ; i < m_links.size() ; i++) {
            const sf::Vector2f linkStart = m_links[i].start.position;
            const sf::Vector2f direction = m_links[i].GetDirection();
            const sf::Vector2f newPos = linkStart + direction * m_links[i].length;
            m_links[i].SetEndPosition(newPos);
            if (i != m_links.size()-1) m_links[i+1].start = m_links[i].end;

            // m_links[i].angle = ComputeAngle(direction);
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