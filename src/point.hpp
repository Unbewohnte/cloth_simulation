#pragma once


#include "vec.hpp"

struct Point {
    float x;
    float y;
    float mass;
    bool frozen;
    bool selected;
    Vec2f velocity;
};