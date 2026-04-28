#include "application.hpp"

int main() {
    Application app("AnimLab", sf::Color(33,33,33), SceneType::BODY_2D);
    app.Run();
    return 0;
}