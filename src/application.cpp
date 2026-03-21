#include "application.hpp"

Application::Application(const std::string& title, const sf::Color backgroundColor):
    m_window(title, backgroundColor), m_isRunning(true)
{
    m_chain = Chain(sf::Vector2f{m_window.GetWidth()/2, m_window.GetHeight()}, 20, 40);
}

void Application::Run()
{   
    while (m_isRunning) {
        m_time.Update();
        m_eventController.GetEvents(m_window.GetRender());
        m_isRunning = m_eventController.HandleWindowEvents();
        m_eventController.HandleEvents();
        
        m_window.ClearBackground();
        m_window.Draw(m_chain);
        // const float mappedValue = ((sin(m_time.GetElapsedTime())-1)/2)*M_PI; // [-PI; 0]
        const float mappedValue = sin(m_time.GetElapsedTime())*8; // [-8; 8]
        for (unsigned int i = 1 ; i < m_chain.GetNrLink() ; i++)
            m_chain.SetAngleAt(i, radians(mappedValue));
        m_window.Display();
    }
    
    m_window.Close();
}