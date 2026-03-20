#pragma once

#include <SFML/Graphics.hpp>

#include <cmath>
#include <stdexcept>
#include <vector>

#include "type.hpp"

struct Joint
{
    Vec2 position;
    float radius;
    sf::CircleShape circle;

    Joint() : Joint(Vec2{0,0})
    {}

    Joint(const Vec2 p) : position(p), radius(30.f)
    {
        circle.setRadius(radius);
        circle.setOrigin(radius, radius);
        circle.setFillColor(sf::Color::White);
        Update();
    }

    void Update() {
        circle.setPosition(position.x, position.y);
    }
};

struct Link
{
    Joint start;
    Joint end;
    float angle;
    float length;

    Link(const Joint j, const float a, const float l):
        start(j), angle(a), length(l) 
    {
        ComputeEnd();
    }

    void ComputeEnd() {
        end.position.x = start.position.x + cos(angle) * length; 
        end.position.y = start.position.y + sin(angle) * length;
        end.Update();
    }
};

class Chain : public sf::Drawable
{
    private:
        Vec2 m_origin;
        std::vector<Link> m_links;
        
    public:
        Chain() = default;
        Chain(const Vec2 origin, const unsigned int nrJoint, const unsigned int initialLength);

        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};