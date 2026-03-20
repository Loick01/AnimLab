#include "chain.hpp"

Chain::Chain(const Vec2 origin, const unsigned int nrJoint, const unsigned int initialLength):
    m_origin(origin)
{
    if (nrJoint < 2) throw std::invalid_argument("A chain must have at least 2 joints\n");
    Joint currentJoint(m_origin);
    for (unsigned int i = 0 ; i < nrJoint-1 ; i++) {
        Link l(currentJoint, radians(-90), initialLength);
        m_links.push_back(l);
        currentJoint = l.end;
    }
}

void Chain::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (const Link& l : m_links) {
        target.draw(l.start.circle, states);
        target.draw(l.end.circle, states);
    }
}