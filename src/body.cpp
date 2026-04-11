#include "body.hpp"

Body::Body(const sf::Vector2f bodyPosition):
    m_bodyPosition(bodyPosition)
{  
    m_rightLeg = std::make_unique<IKChain>(m_bodyPosition, 17, 40);
    m_leftLeg = std::make_unique<IKChain>(m_bodyPosition + sf::Vector2f{100.f, 0.f}, 5, 40);
}

void Body::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
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