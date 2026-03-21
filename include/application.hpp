#pragma once

#include <memory>
#include "chain.hpp"
#include "event.hpp"
#include "time.hpp"
#include "type.hpp"
#include "window.hpp"

class Application
{
    private:
        Window m_window;
        EventController m_eventController;
        std::unique_ptr<Chain> m_chain;
        Time m_time;
        bool m_isRunning;

    public:
        Application(const std::string& title, const sf::Color backgroundColor);

        void Run();
};