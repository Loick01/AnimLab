#include "window.hpp"

Window::Window(const std::string& title, const sf::Color backgroundColor):
    m_renderWindow(sf::VideoMode::getDesktopMode(), title, sf::Style::Fullscreen),
    m_backgroundColor(backgroundColor), m_size(m_renderWindow.getSize())
{}

sf::RenderWindow& Window::GetRender()
{
    return m_renderWindow;
}

sf::Color& Window::GetBackgroundColor()
{
    return m_backgroundColor;
}

float Window::GetWidth() const {
    return m_size.x;
}

float Window::GetHeight() const {
    return m_size.y;
}

void Window::ClearBackground()
{
    m_renderWindow.clear(m_backgroundColor);
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