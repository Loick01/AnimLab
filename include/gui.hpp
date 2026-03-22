#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <stdexcept>

#include "imgui.h"
#include "imgui-SFML.h"

#include "chain.hpp"
#include "notifier.hpp"

enum class ChainType
{
    FK, IK_FABRIK //, IK_CCD
};

class ImGuiLayer : public Notifier<ChainType>
{
    private:
        sf::RenderWindow& m_renderWindow;
        Chain* m_chain;
        unsigned int m_selectedChain;

    public:
        ImGuiLayer(sf::RenderWindow& renderWindow);
        ~ImGuiLayer();

        void SetChain(Chain* chain);
        void HandleEvents(std::vector<sf::Event> events);
        void SetFrame(const sf::Time deltaTime);
        void Draw();
};