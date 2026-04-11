#include "body.hpp"

Body::Body(const sf::Vector2f chestSize, const sf::Vector2f bodyPosition, const float floorHeight, 
const unsigned int nrLegJoint, const unsigned int linkLength) :
    m_bodyPosition(bodyPosition), m_chestSize(chestSize)
{  
    m_bodyPosition.y = floorHeight - m_chestSize.y - (nrLegJoint-1)*linkLength;
    m_chest.setSize(m_chestSize);
    m_chest.setPosition(m_bodyPosition);
    m_leftLeg = std::make_unique<IKChain>(TargetMode::Walking, m_bodyPosition + sf::Vector2f{m_chestSize.x*0.2f, m_chestSize.y}, nrLegJoint, linkLength);
    m_rightLeg = std::make_unique<IKChain>(TargetMode::Walking, m_bodyPosition + sf::Vector2f{m_chestSize.x*0.8f, m_chestSize.y}, nrLegJoint, linkLength);
}

void Body::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_chest, states);
    m_rightLeg->draw(target, states);
    m_leftLeg->draw(target, states);
}

void Body::Update(const Time& time)
{
    m_rightLeg->Update(time);
    m_leftLeg->Update(time);
}

void Body::SetElementGUI() 
{
    
}