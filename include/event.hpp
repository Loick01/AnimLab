#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <vector>

enum class Direction
{
    Right, Left, None
};

class EventController
{
    private:
        std::vector<sf::Event> m_events;
        sf::Vector2i m_mousePosition;
        Direction m_eventDirection;

    public:
        EventController(sf::RenderWindow& renderWindow);

        std::vector<sf::Event>& GetEvents();
        Direction GetEventDirection() const;
        sf::Vector2i GetMousePosition() const;
        bool HandleWindowEvents();
        void PollEvents(sf::RenderWindow& renderWindow); // Should be in Window class ?
        void HandleEvents();
};