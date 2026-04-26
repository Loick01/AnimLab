#include "application.hpp"

int main() {
    Application app("AnimLab", sf::Color(33,33,33), SceneType::IK_FABRIK);
    app.Run();
    return 0;
}