#pragma once

#include <memory>

#include "element.hpp"
#include "event.hpp"
#include "gui.hpp"
#include "time.hpp"
#include "type.hpp"
#include "window.hpp"

class Application
{
    private:
        Window m_window;
        EventController m_eventController;
        ImGuiLayer m_gui;
        std::unique_ptr<Element> m_element;
        Time m_globalTime;
        Time m_elementTime;
        bool m_isRunning;
        bool m_isPaused;

    public:
        Application(const std::string& title, const sf::Color backgroundColor, const SceneType defaultScene);

        void SwitchScene(const SceneType e);
        void Run();
};