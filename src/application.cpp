#include "application.hpp"

Application::Application(const std::string& title, const sf::Color backgroundColor):
    m_window(title, backgroundColor), m_isRunning(true)
{}

void Application::Run()
{   
    while (m_isRunning) {
        m_eventController.GetEvents(m_window.GetRender());
        m_isRunning = m_eventController.HandleWindowEvents();
        m_eventController.HandleEvents();
        
        m_window.ClearBackground();
        m_window.Draw();
    }
    
    m_window.Close();
}