#pragma once

#include "event.hpp"
#include "window.hpp"

class Application
{
    private:
        Window m_window;
        EventController m_eventController;
        bool m_isRunning;

    public:
        Application(const std::string& title, const sf::Color backgroundColor);

        void Run();
};