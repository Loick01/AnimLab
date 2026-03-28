#include "gui.hpp"

ImGuiLayer::ImGuiLayer(sf::RenderWindow& renderWindow, sf::Color& backgroundColor):
    m_renderWindow(renderWindow), m_backgroundColor(backgroundColor), 
    m_selectedScene(1) // m_selectedScene should be initialized according to the default scene type in Application constructor
{
    if (!ImGui::SFML::Init(m_renderWindow))
        throw std::runtime_error("Failed to initialize ImGui SFML\n");
}

ImGuiLayer::~ImGuiLayer()
{
    ImGui::SFML::Shutdown();
}

void ImGuiLayer::SetElement(Element* element){
    m_element = element;
}

void ImGuiLayer::SetFrame(const sf::Time deltaTime)
{
    ImGui::SFML::Update(m_renderWindow, deltaTime);
    ImGui::Begin("AnimLab");

    const char* sceneList[] = {"Forward Kinematics", "Inverse Kinematics (FABRIK)", "Body 2D"}; // Must be the same order than SceneType

    if (ImGui::BeginCombo("Scene", sceneList[m_selectedScene])){
        for (int i = 0; i < IM_ARRAYSIZE(sceneList); i++){
            if (ImGui::Selectable(sceneList[i])){
                m_selectedScene = i;
                Notify(static_cast<SceneType>(m_selectedScene));
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

    m_element->SetElementGUI();

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