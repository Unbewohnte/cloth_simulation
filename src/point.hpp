#pragma once


#include "vec.hpp"

struct Point {
    Vec2f pos;
    float mass;
    bool frozen;
    bool selected;
    float radius;
    Vec2f acceleration;
    Vec2f velocity;
};