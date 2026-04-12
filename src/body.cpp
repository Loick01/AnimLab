#include "body.hpp"

Body::Body(const sf::Vector2f chestSize, const sf::Vector2f bodyPosition, const float floorHeight, 
const unsigned int nrLegJoint, const unsigned int linkLength) :
    m_leftLeg(TargetMode::Walking, nrLegJoint, linkLength),
    m_rightLeg(TargetMode::Walking, nrLegJoint, linkLength),
    m_bodyPosition(bodyPosition), m_chestSize(chestSize), m_thresholdLeg(200.f)
{  
    m_bodyPosition.y = floorHeight - m_chestSize.y - (nrLegJoint-1)*linkLength; // Remove
    m_chest.setSize(m_chestSize);
    m_chest.setPosition(m_bodyPosition);
    m_leftLeg.SetOrigin(m_bodyPosition + sf::Vector2f{m_chestSize.x*0.2f, m_chestSize.y});
    m_rightLeg.SetOrigin(m_bodyPosition + sf::Vector2f{m_chestSize.x*0.8f, m_chestSize.y});
    m_rightLeg.SetCurrentTarget(sf::Vector2f{m_bodyPosition.x, floorHeight});
    m_leftLeg.SetCurrentTarget(sf::Vector2f{m_bodyPosition.x, floorHeight});
    m_nextTargetL = sf::Vector2f{m_bodyPosition.x, floorHeight};
    m_nextTargetR = sf::Vector2f{m_bodyPosition.x, floorHeight};
}

void Body::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_chest, states);
    m_rightLeg.draw(target, states);
    m_leftLeg.draw(target, states);
}

void Body::Update(const Time& time)
{
    m_rightLeg.Update(time);
    m_leftLeg.Update(time);

    if (Length(m_rightLeg.GetCurrentTarget()-m_nextTargetR) > m_thresholdLeg) {
        m_rightLeg.SetCurrentTarget(m_nextTargetR);
    }
    if (Length(m_leftLeg.GetCurrentTarget()-m_nextTargetL) > m_thresholdLeg) {
        m_leftLeg.SetCurrentTarget(m_nextTargetL);
    }
}

void Body::SetElementGUI() 
{
    ImGui::SliderFloat("Distance threshold for legs", &m_thresholdLeg, 0.f, 1000.f);
}