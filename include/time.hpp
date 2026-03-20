#pragma once

#include <SFML/Graphics.hpp>

struct Time
{
    sf::Clock clock;
    float elapsedTime;
    sf::Time currentTime;
    sf::Time previousTime;
    float deltaTime;

    Time() : previousTime(clock.getElapsedTime()), deltaTime(0.f), elapsedTime(0.f)
    {}

    void Update()
    {
        currentTime = clock.getElapsedTime();
        deltaTime = (currentTime-previousTime).asSeconds();
        previousTime = currentTime;
    }

    float GetDeltaTime() const
    {
        return deltaTime;
    }

    float GetElapsedTime() const
    {
        return clock.getElapsedTime().asSeconds();
    }
};