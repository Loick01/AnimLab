#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class EventController
{
    private:
        std::vector<sf::Event> m_events;

    public:
        EventController() = default;

        bool HandleWindowEvents();
        void GetEvents(sf::RenderWindow& renderWindow); // Should be in Window class ?
        void HandleEvents();
};