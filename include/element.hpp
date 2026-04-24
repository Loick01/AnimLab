#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "time.hpp"

class Element : public sf::Drawable
{
    public:
        virtual ~Element() = default;
        
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
        virtual void Update(const Time& time) = 0;
        virtual void SetElementGUI() = 0;
        virtual void SetAngleGUI() = 0;
};