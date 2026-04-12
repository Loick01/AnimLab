#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

#include <iostream>

#include "chain.hpp"
#include "element.hpp"
#include "event.hpp"

class Body : public Element
{
    private:
        const EventController& m_eventController;
         
        sf::RectangleShape m_chest;
        IKChain m_leftLeg;
        IKChain m_rightLeg;
        sf::Vector2f m_bodyPosition;
        sf::Vector2f m_chestSize;
        sf::Vector2f m_nextTargetR; // Next target for the right leg
        sf::Vector2f m_nextTargetL; // Next target for the left leg 
        float m_thresholdLeg;
        float m_speed;

        void Move(const Direction eventDirection, const float deltaTime);
        
    public:
        Body(const EventController& eventController, const sf::Vector2f chestSize, const sf::Vector2f bodyPosition, const float floorHeight,
            const unsigned int nrLegJoint, const unsigned int linkLength);

        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        void Update(const Time& time) override;
        void SetElementGUI() override;
};