#pragma once

#include "chain.hpp"
#include "event.hpp"
#include "type.hpp"
#include "window.hpp"

class Application
{
    private:
        Window m_window;
        EventController m_eventController;
        Chain m_chain;
        bool m_isRunning;

    public:
        Application(const std::string& title, const sf::Color backgroundColor);

        void Run();
};