#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

struct Time
{
    sf::Clock clock;
    float elapsedTime;
    sf::Time currentTime;
    sf::Time previousTime;
    sf::Time deltaTime;
    sf::Time count;

    Time() : deltaTime(sf::Time::Zero), count(sf::Time::Zero), elapsedTime(0.f)
    {
        Reset();
    }

    sf::Clock& GetClock() {
        return clock;
    }

    void Reset()
    {
        previousTime = clock.getElapsedTime();
    }

    void Update()
    {
        currentTime = clock.getElapsedTime();
        deltaTime = currentTime-previousTime;
        previousTime = currentTime;
        count += deltaTime;
    }

    sf::Time GetDeltaTime() const
    {
        return deltaTime;
    }

    float GetElapsedTime() const
    {
        return clock.getElapsedTime().asSeconds();
    }

    float GetCountTime() const
    {
        return count.asSeconds();
    }
};