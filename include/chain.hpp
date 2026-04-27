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
    sf::CircleShape circle;
    sf::Color color;
    float radius;

    Joint(const sf::Color c) : Joint(sf::Vector2f{0.,0.}, c)
    {}

    Joint(const sf::Vector2f p, const sf::Color c) : position(p), radius(10.f)
    {
        SetRadius(radius);
        circle.setOrigin(radius, radius); // Define the center point of the shape, relative to the top-left corner (default is 0,0)
        SetColor(c);
        UpdateCirclePosition();
    }

    void SetColor(const sf::Color c) {
        color = c;
        circle.setFillColor(color);
    }

    void SetRadius(const float r) {
        circle.setRadius(r);
    }

    void UpdateCirclePosition() {
        circle.setPosition(position);
    }
};

struct Link
{
    Joint start;
    Joint end;
    sf::Vertex line[2];
    float angle; // Angle (CCW) from X axis to the vector of the link (start to end)
    float length;

    Link(const Joint j, const sf::Color c, const float initialAngle, const float l):
        start(j), end(c), angle(initialAngle), length(l) 
    {
        ComputeEnd();
    }

    sf::Vector2f GetDirection() const {
        return GetNormalized(end.position - start.position);
    }

    void SetStartPosition(const sf::Vector2f position)  {
        start.position = position;
        line[0] = sf::Vertex(start.position, start.color);
        line[1] = sf::Vertex(end.position, end.color);
        start.UpdateCirclePosition();
    }

    void SetEndPosition(const sf::Vector2f position)  {
        end.position = position;
        line[0] = sf::Vertex(start.position, start.color);
        line[1] = sf::Vertex(end.position, end.color);
        end.UpdateCirclePosition();
    }

    void ComputeEnd() {
        // I use -sin because sfml window y axis points downwards. Thus I can use CCW as positive angle
        const float endX = start.position.x + cos(angle) * length;
        const float endY = start.position.y - sin(angle) * length;
        SetEndPosition(sf::Vector2f{endX, endY});
    }

    void ComputeAngle() {
        // angle = atan2();
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

        sf::Color GetColor() const;
        unsigned int GetNrLink() const;
        void SetOrigin(const sf::Vector2f origin);
        void SetElementGUI() override;
        void SetAngleGUI() override;
        void AddJoint(); // Add/Remove at the end of m_links
        void RemoveJoint();
        void UpdateJointColor();
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

class FKChain : public Chain
{
    private:
        float m_amplitude;

        // Should be in Chain ?
        void SetAngleAt(const unsigned int index, const float angle);
        void AddAngleAt(const unsigned int index, const float angle);
        
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