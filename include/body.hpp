#pragma once

#include "chain.hpp"
#include "element.hpp"

class Body : public Element
{
    //private:
        
    public:
        Body();

        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        void Update(const Time& time) override;
        void SetElementGUI() override;
};