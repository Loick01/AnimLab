#pragma once

#include <cmath>

inline float radians(const float degrees) { // Clockwise
    return degrees * M_PI/180;
}

inline float degrees(const float radians) { // Clockwise
    return radians * 180/M_PI;
}

inline float Length(const sf::Vector2f v) {
    return sqrt(v.x*v.x + v.y*v.y);
}