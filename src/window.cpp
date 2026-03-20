#include "window.hpp"

Window::Window(const std::string& title, const sf::Color backgroundColor):
    m_renderWindow(sf::VideoMode::getDesktopMode(), title, sf::Style::Fullscreen),
    m_backgroungColor(backgroundColor), m_size(m_renderWindow.getSize())
{}

sf::RenderWindow& Window::GetRender()
{
    return m_renderWindow;
}

int Window::GetWidth() const {
    return m_size.x;
}

int Window::GetHeight() const {
    return m_size.y;
}

void Window::ClearBackground()
{
    m_renderWindow.clear(m_backgroungColor);
}

void Window::Close()
{
    m_renderWindow.close();
}

void Window::Draw(const sf::Drawable& drawable)
{
    m_renderWindow.draw(drawable);
}

void Window::Display()
{
    m_renderWindow.display();
}