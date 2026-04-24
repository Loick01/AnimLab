#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <stdexcept>

#include "imgui.h"
#include "imgui-SFML.h"

#include "element.hpp"
#include "notifier.hpp"

enum class SceneType
{
    FK, IK_FABRIK, BODY_2D //, IK_CCD, BODY_3D
};

class ImGuiLayer : public Notifier<SceneType>
{
    private:
        sf::RenderWindow& m_renderWindow;
        Element* m_element;
        sf::Color& m_backgroundColor;
        unsigned int m_selectedScene;

    public:
        ImGuiLayer(sf::RenderWindow& renderWindow, sf::Color& backgroundColor, const SceneType defaultScene);
        ~ImGuiLayer();

        void SetElement(Element* element);
        void HandleEvents(std::vector<sf::Event> events);
        void SetFrame(const sf::Time deltaTime);
        void Draw();
};