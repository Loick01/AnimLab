#include "gui.hpp"

ImGuiLayer::ImGuiLayer(sf::RenderWindow& renderWindow):
    m_renderWindow(renderWindow)
{
    if (!ImGui::SFML::Init(m_renderWindow))
        throw std::runtime_error("Failed to initialize ImGui SFML\n");
}

ImGuiLayer::~ImGuiLayer()
{
    ImGui::SFML::Shutdown();
}

void ImGuiLayer::SetFrame(const sf::Time deltaTime)
{
    ImGui::SFML::Update(m_renderWindow, deltaTime);
    ImGui::Begin("AnimLab");
    ImGui::Button("This is a button");
    ImGui::End();
}

void ImGuiLayer::Draw()
{
    ImGui::SFML::Render(m_renderWindow);
}