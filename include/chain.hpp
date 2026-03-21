#pragma once

#include <SFML/Graphics.hpp>

#include <cmath>
#include <stdexcept>
#include <vector>

#include "type.hpp"

struct Joint
{
    sf::Vector2f position;
    float radius;
    sf::CircleShape circle;

    Joint() : Joint(sf::Vector2f{0.,0.})
    {}

    Joint(const sf::Vector2f p) : position(p), radius(10.f)
    {
        circle.setRadius(radius);
        circle.setOrigin(radius, radius);
        circle.setFillColor(sf::Color::White);
        UpdatePosition();
    }

    void UpdatePosition() {
        circle.setPosition(position.x, position.y);
    }
};

struct Link
{
    Joint start;
    Joint end;
    sf::Vertex line[2];
    float worldAngle; // Sum of the angles of the parent links
    float localAngle; // Angle of the link with the previous link of the chain
    float length;

    Link(const Joint j, const float local, const float world, const float l):
        start(j), localAngle(local), worldAngle(world), length(l) 
    {
        ComputeEnd();
    }

    void ComputeEnd() {
        end.position.x = start.position.x + cos(worldAngle + localAngle) * length; 
        end.position.y = start.position.y + sin(worldAngle + localAngle) * length;
        end.UpdatePosition();
        line[0] = sf::Vertex(start.position, sf::Color::White);
        line[1] = sf::Vertex(end.position, sf::Color::White);
    }
};

class Chain : public sf::Drawable
{
    private:
        sf::Vector2f m_origin;
        std::vector<Link> m_links;
        
    public:
        Chain() = default;
        Chain(const sf::Vector2f origin, const unsigned int nrJoint, const unsigned int initialLength);

        unsigned int GetNrLink() const;
        void SetAngleAt(const unsigned int index, const float newLocalAngle);
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};