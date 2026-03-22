#pragma once

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

struct Time
{
    sf::Clock clock;
    float elapsedTime;
    sf::Time currentTime;
    sf::Time previousTime;
    sf::Time deltaTime;

    Time() : previousTime(clock.getElapsedTime()), deltaTime(sf::Time::Zero), elapsedTime(0.f)
    {}

    sf::Clock& GetClock() {
        return clock;
    }

    void Update()
    {
        currentTime = clock.getElapsedTime();
        deltaTime = currentTime-previousTime;
        previousTime = currentTime;
    }

    sf::Time GetDeltaTime() const
    {
        return deltaTime;
    }

    float GetElapsedTime() const
    {
        return clock.getElapsedTime().asSeconds();
    }
};