#include "application.hpp"

Application::Application(const std::string& title, const sf::Color backgroundColor):
    m_window(title, backgroundColor), m_isRunning(true), m_eventController(m_window.GetRender()), 
    m_gui(m_window.GetRender(), m_window.GetBackgroundColor())
{
    SwitchScene(SceneType::BODY_2D); // Default scene type
    m_gui.AddCallback([this](SceneType e){SwitchScene(e);});
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
        m_window.Draw(*m_element);
        m_element->Update(m_time);
        m_gui.SetFrame(m_time.GetDeltaTime());
        m_gui.Draw();
        m_window.Display();
    }
    
    m_window.Close();
}

void Application::SwitchScene(const SceneType e)
{
    switch(e){
        case SceneType::FK: {
            m_element = std::make_unique<FKChain>(sf::Vector2f{m_window.GetWidth()/2, m_window.GetHeight()}, 5, 40);
            break;
        } 
        case SceneType::IK_FABRIK: {
            m_element = std::make_unique<IKChain>(TargetMode::Rotating, 5, 40, &m_eventController, sf::Vector2f{m_window.GetWidth()/2, m_window.GetHeight()});
            break;
        }
        case SceneType::BODY_2D: {
            m_element = std::make_unique<Body>(sf::Vector2f{150.f, 150.f}, sf::Vector2f{m_window.GetWidth()/2, 0.f}, m_window.GetHeight(), 3, 40);
            break;
        }
        default:
            throw std::invalid_argument("Unknown scene");
    }
    m_gui.SetElement(m_element.get());
}