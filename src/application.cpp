#include "application.hpp"

Application::Application(const std::string& title, const sf::Color backgroundColor):
    m_window(title, backgroundColor), m_isRunning(true)
{
    //m_chain = std::make_unique<FKChain>(sf::Vector2f{m_window.GetWidth()/2, m_window.GetHeight()}, 20, 40);
    m_chain = std::make_unique<IKChain>(sf::Vector2f{m_window.GetWidth()/2, m_window.GetHeight()}, 20, 40);
}

void Application::Run()
{   
    while (m_isRunning) {
        m_time.Update();
        m_eventController.GetEvents(m_window.GetRender());
        m_isRunning = m_eventController.HandleWindowEvents();
        m_eventController.HandleEvents();
        
        m_window.ClearBackground();
        m_window.Draw(*m_chain);
        m_chain->Update(m_time);
        m_window.Display();
    }
    
    m_window.Close();
}