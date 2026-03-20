#pragma once

#include <cmath>

inline float radians(const float degrees) { // Clockwise
    return degrees * M_PI/180;
}

struct Vec2
{
    int x, y;

    Vec2() : x(0), y(0)
    {}

    Vec2(int a, int b) : x(a), y(b)
    {}
};