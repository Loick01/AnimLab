#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <stdexcept>

#include "imgui.h"
#include "imgui-SFML.h"

class ImGuiLayer
{
    private:
        sf::RenderWindow& m_renderWindow;

    public:
        ImGuiLayer(sf::RenderWindow& renderWindow);
        ~ImGuiLayer();

        void SetFrame(const sf::Time deltaTime);
        void Draw();
};