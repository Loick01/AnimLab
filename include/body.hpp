#pragma once

#include <memory> // Try to remove ?

#include <SFML/Graphics/RectangleShape.hpp>

#include "chain.hpp"
#include "element.hpp"

class Body : public Element
{
    private:
        sf::RectangleShape m_chest;
        std::unique_ptr<Chain> m_leftLeg;
        std::unique_ptr<Chain> m_rightLeg;
        sf::Vector2f m_bodyPosition;
        sf::Vector2f m_chestSize;
        
    public:
        Body(const sf::Vector2f chestSize, const sf::Vector2f bodyPosition, const float floorHeight,
            const unsigned int nrLegJoint, const unsigned int linkLength);

        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        void Update(const Time& time) override;
        void SetElementGUI() override;
};