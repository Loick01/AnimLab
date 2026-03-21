#include "event.hpp"

bool EventController::HandleWindowEvents()
{
    for (const sf::Event event : m_events) {
        if (event.type == sf::Event::Closed)
            return false;
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            return false;
    }
    return true;
}

void EventController::GetEvents(sf::RenderWindow& renderWindow)
{   
    m_events.clear();
    sf::Event event;
    while (renderWindow.pollEvent(event))
        m_events.push_back(event);
}

void EventController::HandleEvents()
{
    for (const sf::Event event : m_events) {
        switch(event.type) {
            case sf::Event::MouseButtonPressed : {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    int x = event.mouseButton.x;
                    int y = event.mouseButton.y;
                }
                break;
            }
            default:
                break;
        }
    }
}