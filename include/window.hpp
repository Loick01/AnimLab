#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class Window
{
    private:
        void CreateWindow();

        sf::RenderWindow m_renderWindow;
        sf::Color m_backgroungColor;
        std::string m_title;
        unsigned int m_width;
        unsigned int m_height;

    public:
        Window(const std::string& title, const sf::Color backgroundColor);

        sf::RenderWindow& GetRender();
        void ClearBackground();
        void Close();
        void Draw();
};