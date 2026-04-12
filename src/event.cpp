#include "event.hpp"

EventController::EventController(sf::RenderWindow& renderWindow):
    m_mousePosition(sf::Mouse::getPosition(renderWindow)), m_eventDirection(Direction::None)
{}

std::vector<sf::Event>& EventController::GetEvents()
{
    return m_events;
}

Direction EventController::GetEventDirection() const
{
    return m_eventDirection;
}

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

void EventController::PollEvents(sf::RenderWindow& renderWindow)
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
            case sf::Event::KeyPressed : {
                if (event.key.code == sf::Keyboard::Left)
                    m_eventDirection = Direction::Left;
                else if (event.key.code == sf::Keyboard::Right)
                    m_eventDirection = Direction::Right;
                break;
            }
            case sf::Event::KeyReleased :
                m_eventDirection = Direction::None;
                break;
            default:
                break;
        }
    }
}