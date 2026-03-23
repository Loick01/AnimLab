#include "gui.hpp"

ImGuiLayer::ImGuiLayer(sf::RenderWindow& renderWindow, sf::Color& backgroundColor):
    m_renderWindow(renderWindow), m_backgroundColor(backgroundColor), 
    m_selectedChain(1) // m_selectedChain should be initialized according to the default chain type in Application constructor
{
    if (!ImGui::SFML::Init(m_renderWindow))
        throw std::runtime_error("Failed to initialize ImGui SFML\n");
}

ImGuiLayer::~ImGuiLayer()
{
    ImGui::SFML::Shutdown();
}

void ImGuiLayer::SetChain(Chain* chain){
    m_chain = chain;
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

    float bgColor[3] = {m_backgroundColor.r/255.f, m_backgroundColor.g/255.f, m_backgroundColor.b/255.f};
    if (ImGui::ColorEdit3("Background color", bgColor)) {
        m_backgroundColor = {
            static_cast<std::uint8_t>(bgColor[0]*255), 
            static_cast<std::uint8_t>(bgColor[1]*255),
            static_cast<std::uint8_t>(bgColor[2]*255)
        };
    }

    sf::Color chainColor = m_chain->GetColor();
    float jointColor[3] = {chainColor.r/255.f, chainColor.g/255.f, chainColor.b/255.f};
    if (ImGui::ColorEdit3("Joint color", jointColor)) {
        chainColor = {
            static_cast<std::uint8_t>(jointColor[0]*255), 
            static_cast<std::uint8_t>(jointColor[1]*255),
            static_cast<std::uint8_t>(jointColor[2]*255)
        };

        m_chain->SetColor(chainColor);
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