#include "window.hpp"

Window::Window(const std::string& title, const sf::Color backgroundColor):
    m_renderWindow(sf::VideoMode::getDesktopMode(), title, sf::Style::Fullscreen),
    m_backgroungColor(backgroundColor)
{}

sf::RenderWindow& Window::GetRender()
{
    return m_renderWindow;
}

void Window::ClearBackground()
{
    m_renderWindow.clear(m_backgroungColor);
}

void Window::Close()
{
    m_renderWindow.close();
}

void Window::Draw()
{
    m_renderWindow.display();
}