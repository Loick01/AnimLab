#include "application.hpp"

Application::Application(const std::string& title, const sf::Color backgroundColor):
    m_window(title, backgroundColor), m_isRunning(true), m_eventController(m_window.GetRender()), m_gui(m_window.GetRender())
{
    SwitchChain(ChainType::IK_FABRIK); // Default chain type
    m_gui.AddCallback([this](ChainType e){SwitchChain(e);});
}

void Application::Run()
{   
    while (m_isRunning) {
        m_time.Update();
        m_eventController.PollEvents(m_window.GetRender());
        m_isRunning = m_eventController.HandleWindowEvents();
        m_eventController.HandleEvents();
        m_gui.HandleEvents(m_eventController.GetEvents());
        
        m_window.ClearBackground();
        m_window.Draw(*m_chain);
        m_chain->Update(m_time);
        m_gui.SetFrame(m_time.GetDeltaTime());
        m_gui.Draw();
        m_window.Display();
    }
    
    m_window.Close();
}

void Application::SwitchChain(const ChainType e)
{
    switch(e){
        case ChainType::FK: {
            m_chain = std::make_unique<FKChain>(sf::Vector2f{m_window.GetWidth()/2, m_window.GetHeight()}, 20, 40);
            break;
        } 
        case ChainType::IK_FABRIK: {
            m_chain = std::make_unique<IKChain>(m_eventController, sf::Vector2f{m_window.GetWidth()/2, m_window.GetHeight()}, 20, 40);
            break;
        }
        default:
            throw std::invalid_argument("Unknown chain type");
    }
    m_gui.SetChain(m_chain.get());
}