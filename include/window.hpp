#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <string>

class Window
{
    private:
        void CreateWindow();

        sf::RenderWindow m_renderWindow;
        sf::Color m_backgroundColor;
        std::string m_title;
        sf::Vector2f m_size;

    public:
        Window(const std::string& title, const sf::Color backgroundColor);

        sf::RenderWindow& GetRender();
        sf::Color& GetBackgroundColor();
        float GetWidth() const;
        float GetHeight() const;
        void ClearBackground(); // Must be called before every Draw()
        void Close();
        void Draw(const sf::Drawable& drawable);
        void Display(); // Must be called after every Draw()
};