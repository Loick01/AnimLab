#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <vector>

class EventController
{
    private:
        std::vector<sf::Event> m_events;
        sf::Vector2i m_mousePosition;

    public:
        EventController(sf::RenderWindow& renderWindow);

        std::vector<sf::Event>& GetEvents();
        sf::Vector2i GetMousePosition() const;
        bool HandleWindowEvents();
        void PollEvents(sf::RenderWindow& renderWindow); // Should be in Window class ?
        void HandleEvents();
};