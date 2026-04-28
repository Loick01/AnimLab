#pragma once

#include "chain.hpp"

enum class State
{
    Rest, Lift
};

struct LegAnimation
{
    float t; // [0., 1.]
    float speed; // ]0., ]
    float height;
    float startX;
    float endX;

    LegAnimation(const float h) {
        t = 0.f;
        speed = 5.f;
        height = h; 
    }
};

class Leg : public IKChain
{
    private:
        LegAnimation m_anim;
        State m_state; 
        sf::Vector2f m_nextTarget; // Current target is in IKChain
        float m_distanceThreshold;
    
    public:
        Leg(const TargetMode targetMode, const unsigned int nrJoint, const unsigned int initialLength, State initialState);

        sf::Vector2f GetNextTarget() const;
        float GetDistanceThreshold() const;
        void MoveNextTarget(const float movement);
        void SetNextTarget(const sf::Vector2f nextTarget);
        void Update(const Time& time) override;
        void SetElementGUI() override;
};