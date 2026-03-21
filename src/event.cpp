#include "event.hpp"

EventController::EventController(sf::RenderWindow& renderWindow):
    m_mousePosition(sf::Mouse::getPosition(renderWindow))
{}

sf::Vector2i EventController::GetMousePosition() const
{
    return m_mousePosition;
}

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
            case sf::Event::MouseMoved : {
                m_mousePosition = {event.mouseMove.x, event.mouseMove.y};
                break;
            }
            default:
                break;
        }
    }
}