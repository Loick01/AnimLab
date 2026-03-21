#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class EventController
{
    private:
        std::vector<sf::Event> m_events;
        sf::Vector2i m_mousePosition;

    public:
        EventController(sf::RenderWindow& renderWindow);

        sf::Vector2i GetMousePosition() const;
        bool HandleWindowEvents();
        void GetEvents(sf::RenderWindow& renderWindow); // Should be in Window class ?
        void HandleEvents();
};