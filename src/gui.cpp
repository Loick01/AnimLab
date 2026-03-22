#include "gui.hpp"

ImGuiLayer::ImGuiLayer(sf::RenderWindow& renderWindow):
    m_renderWindow(renderWindow), m_selectedChain(1) // m_selectedChain should be initialized according to the default chain type in Application constructor
{
    if (!ImGui::SFML::Init(m_renderWindow))
        throw std::runtime_error("Failed to initialize ImGui SFML\n");
}

ImGuiLayer::~ImGuiLayer()
{
    ImGui::SFML::Shutdown();
}

void ImGuiLayer::SetChain(Chain* chain){
    m_chain = m_chain;
}

void ImGuiLayer::SetFrame(const sf::Time deltaTime)
{
    ImGui::SFML::Update(m_renderWindow, deltaTime);
    ImGui::Begin("AnimLab");

    const char* chainList[] = {"Forward Kinematics", "Inverse Kinematics (FABRIK)"}; // Must be the same order than ChainType

    if (ImGui::BeginCombo("Chain Type", chainList[m_selectedChain])){
        for (int i = 0; i < IM_ARRAYSIZE(chainList); i++){
            if (ImGui::Selectable(chainList[i])){
                m_selectedChain = i;
                Notify(static_cast<ChainType>(m_selectedChain));
            }
        }
        ImGui::EndCombo();
    }

    ImGui::End();
}

void ImGuiLayer::Draw()
{
    ImGui::SFML::Render(m_renderWindow);
}

void ImGuiLayer::HandleEvents(std::vector<sf::Event> events)
{
    for (const sf::Event& event : events)
        ImGui::SFML::ProcessEvent(m_renderWindow, event);
}