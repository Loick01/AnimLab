#include "leg.hpp"

Leg::Leg(const TargetMode targetMode, const unsigned int nrJoint, const unsigned int initialLength, State initialState):
    IKChain(targetMode, nrJoint, initialLength), m_state(initialState), m_distanceThreshold(75.f), m_anim(20.f)
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
    switch (m_state) {
        case State::Rest : {
            if (Length(GetCurrentTarget()-m_nextTarget) > m_distanceThreshold) {
                m_state = State::Lift;
                m_anim.startX = GetCurrentTarget().x;
                m_anim.endX = m_nextTarget.x;
            }
            break;
        }
        case State::Lift : {
            if (m_anim.t >= 1.0) {
                SetCurrentTarget(sf::Vector2f{m_anim.endX, m_nextTarget.y});
                m_state = State::Rest;
                m_anim.t = 0.f;
            } else {
                m_anim.t += time.GetDeltaTime().asSeconds() * m_anim.speed;
                const sf::Vector2f p = {m_anim.startX + m_anim.t * (m_anim.endX-m_anim.startX), 1080.f - (float)sin(m_anim.t*M_PI) * m_anim.height};
                SetCurrentTarget(p);
            }
            break;
        }
        default :
            break;
    }

    IKChain::Update(time);
}

void Leg::SetElementGUI() 
{
    ImGui::SliderFloat("Distance threshold", &m_distanceThreshold, 10.f, 200.f);
    ImGui::SliderFloat("Height when lifting", &m_anim.height, 0.f, 100.f);
    ImGui::SliderFloat("Lifting speed", &m_anim.speed, 0.01f, 10.f);
}