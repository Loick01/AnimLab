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

inline sf::Vector2f GetNormalized(const sf::Vector2f v) {
    const float norm = std::sqrt(v.x*v.x+v.y*v.y);
    if (norm == 0) throw std::runtime_error("Vector length should not be null\n");
    return v/norm;
}

inline float ComputeAngle(const sf::Vector2f v) { // CCW angle from the X axis to v ([-PI, PI])
    return -atan2(v.y, v.x);
}