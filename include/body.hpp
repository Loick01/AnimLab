#pragma once

#include <memory> // Try to remove ?

#include "chain.hpp"
#include "element.hpp"

class Body : public Element
{
    private:
        // sf::IntRect m_chest;
        std::unique_ptr<Chain> m_leftLeg;
        std::unique_ptr<Chain> m_rightLeg;
        sf::Vector2f m_bodyPosition;
        
    public:
        Body(const sf::Vector2f bodyPosition);

        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        void Update(const Time& time) override;
        void SetElementGUI() override;
};