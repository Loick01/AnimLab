#include "leg.hpp"

Leg::Leg(const TargetMode targetMode, const unsigned int nrJoint, const unsigned int initialLength, State initialState):
    IKChain(targetMode, nrJoint, initialLength), m_state(initialState), m_distanceThreshold(75.f)
{
    
}

sf::Vector2f Leg::GetNextTarget() const
{
    return m_nextTarget;
}

float Leg::GetDistanceThreshold() const
{
    return m_distanceThreshold;
}

void Leg::MoveNextTarget(const float movement)
{
    m_nextTarget.x += movement;
}

void Leg::SetNextTarget(const sf::Vector2f nextTarget)
{
    m_nextTarget = nextTarget;
}

void Leg::Update(const Time& time) 
{
    IKChain::Update(time);

    if (Length(GetCurrentTarget()-m_nextTarget) > m_distanceThreshold) {
        SetCurrentTarget(m_nextTarget);
    }
}

void Leg::SetElementGUI() 
{
    ImGui::SliderFloat("Distance threshold", &m_distanceThreshold, 0.f, 100.f);
}