#pragma once

#include <cmath>
#include <stdexcept>
#include <vector>

#include <SFML/Graphics/CircleShape.hpp>

#include "element.hpp"
#include "event.hpp"
#include "imgui.h"
#include "imgui-SFML.h"
#include "type.hpp"

enum class TargetMode
{
    AimingMouse, Rotating, Walking 
};

struct Joint
{
    sf::Vector2f position;
    float radius;
    sf::CircleShape circle;

    Joint(const sf::Color c) : Joint(sf::Vector2f{0.,0.}, c)
    {}

    Joint(const sf::Vector2f p, const sf::Color c) : position(p), radius(10.f)
    {
        SetRadius(radius);
        circle.setOrigin(radius, radius);
        SetColor(c);
        UpdateCirclePosition();
    }

    void SetColor(const sf::Color c) {
        circle.setFillColor(c);
    }

    void SetRadius(const float r) {
        circle.setRadius(r);
    }

    void UpdateCirclePosition() {
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

    Link(const Joint j, const sf::Color c, const float local, const float world, const float l):
        start(j), end(c), localAngle(local), worldAngle(world), length(l) 
    {
        ComputeEndWithAngle();
    }

    void SetStartPosition(const sf::Vector2f position)  {
        start.position = position;
        line[0] = sf::Vertex(start.position, sf::Color::White);
        line[1] = sf::Vertex(end.position, sf::Color::White);
        start.UpdateCirclePosition();
    }

    void SetEndPosition(const sf::Vector2f position)  {
        end.position = position;
        line[0] = sf::Vertex(start.position, sf::Color::White);
        line[1] = sf::Vertex(end.position, sf::Color::White);
        end.UpdateCirclePosition();
    }

    void ComputeEndWithAngle() {
        const float newX = start.position.x + cos(worldAngle + localAngle) * length; 
        const float newY = start.position.y + sin(worldAngle + localAngle) * length;
        SetEndPosition(sf::Vector2f{newX, newY});
    }
};

class Chain : public Element
{
    protected:
        sf::Vector2f m_origin;
        std::vector<Link> m_links;
        sf::Color m_jointColor; // Will be removed
        unsigned int m_initialLength; // Will be removed
        int m_nrJoint;

    public:
        Chain() = default;
        virtual ~Chain() = default;
        Chain(const sf::Vector2f origin, const unsigned int nrJoint, const unsigned int initialLength);

        static sf::Vector2f Normalize(const sf::Vector2f v);

        sf::Color GetColor() const;
        unsigned int GetNrLink() const;
        void SetOrigin(const sf::Vector2f origin);
        void SetElementGUI() override;
        void SetAngleGUI() override;
        void AddJoint(); // Add/Remove at the end of m_links
        void RemoveJoint();
        void UpdateJointColor();
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        void ComputeLinkAngle(const sf::Vector2f v1, const sf::Vector2f v2, const unsigned int linkIndex);
};

class FKChain : public Chain
{
    private:
        void SetAngleAt(const unsigned int index, const float newLocalAngle);
        
    public:
        FKChain() = default;
        FKChain(const sf::Vector2f origin, const unsigned int nrJoint, const unsigned int initialLength);

        void Update(const Time& time) override;
        void SetElementGUI() override;
};

class IKChain : public Chain
{
    private:
        const EventController* m_eventController; // Not here ?
        
        TargetMode m_targetMode;
        sf::Vector2f m_currentTarget; // Currently only used for Walking mode

        // Will be removed (use a struct) ?
        bool m_isAimingMouse;
        bool m_doBackwardPass;
    
    public:
        IKChain() = default;
        IKChain(const TargetMode targetMode, const unsigned int nrJoint, const unsigned int initialLength,
            const EventController* eventController = nullptr, const sf::Vector2f origin = {0.f, 0.f});

        sf::Vector2f GetTargetPosition(const float elapsedTime) const;
        sf::Vector2f GetCurrentTarget() const;
        void SetCurrentTarget(const sf::Vector2f target);
        void Update(const Time& time) override;
        void SetElementGUI() override;
};