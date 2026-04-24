#include "body.hpp"

Body::Body(const EventController& eventController, const sf::Vector2f chestSize, const sf::Vector2f bodyPosition, const float floorHeight, 
const unsigned int nrLegJoint, const unsigned int linkLength) :
    m_leftLeg(TargetMode::Walking, nrLegJoint, linkLength, State::Rest),
    m_rightLeg(TargetMode::Walking, nrLegJoint, linkLength, State::Rest),
    m_eventController(eventController),
    m_bodyPosition(bodyPosition), m_chestSize(chestSize), m_speed(200.f)
{  
    m_bodyPosition.y = floorHeight - m_chestSize.y - ((nrLegJoint-1)*linkLength)/1.1f; // Remove
    m_chest.setSize(m_chestSize);
    m_chest.setPosition(m_bodyPosition);
    
    m_leftLeg.SetOrigin(m_bodyPosition + sf::Vector2f{m_chestSize.x*0.2f, m_chestSize.y});
    m_rightLeg.SetOrigin(m_bodyPosition + sf::Vector2f{m_chestSize.x*0.8f, m_chestSize.y});

    m_leftLeg.SetNextTarget(sf::Vector2f{m_bodyPosition.x + m_chestSize.x*0.2f, floorHeight});
    m_rightLeg.SetNextTarget(sf::Vector2f{m_bodyPosition.x + m_chestSize.x*0.8f, floorHeight});

    m_rightLeg.SetCurrentTarget(m_rightLeg.GetNextTarget());
    const float distanceThreshold = m_leftLeg.GetDistanceThreshold();
    m_leftLeg.SetCurrentTarget(m_leftLeg.GetNextTarget() - sf::Vector2f{distanceThreshold/2.f, 0.f});
}

void Body::Move(const Direction eventDirection, const float deltaTime)
{
    if (eventDirection == Direction::None) return;

    const float movement = (eventDirection == Direction::Right ? 1 : -1) * m_speed * deltaTime;
    m_bodyPosition.x += movement;
    
    m_chest.setPosition(m_bodyPosition);

    m_leftLeg.SetOrigin(m_bodyPosition + sf::Vector2f{m_chestSize.x*0.2f, m_chestSize.y});
    m_rightLeg.SetOrigin(m_bodyPosition + sf::Vector2f{m_chestSize.x*0.8f, m_chestSize.y});
    m_leftLeg.MoveNextTarget(movement);
    m_rightLeg.MoveNextTarget(movement);
}

void Body::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_chest, states);
    m_rightLeg.draw(target, states);
    m_leftLeg.draw(target, states);
}

void Body::Update(const Time& time)
{
    Move(m_eventController.GetEventDirection(), time.GetDeltaTime().asSeconds());
    
    m_rightLeg.Update(time);
    m_leftLeg.Update(time);
}

void Body::SetElementGUI() 
{
    ImGui::SliderFloat("Speed", &m_speed, 10.f, 300.f);
    
    ImGui::Text("Left leg");
    ImGui::PushID("Left");
    m_leftLeg.SetElementGUI();
    ImGui::PopID();

    ImGui::Text("Right leg");
    ImGui::PushID("Right");
    m_rightLeg.SetElementGUI();
    ImGui::PopID();
}

void Body::SetAngleGUI() 
{   
    ImGui::Text("Left leg");
    ImGui::PushID("Left");
    m_leftLeg.SetAngleGUI();
    ImGui::PopID();

    ImGui::Text("Right leg");
    ImGui::PushID("Right");
    m_rightLeg.SetAngleGUI();
    ImGui::PopID();
}