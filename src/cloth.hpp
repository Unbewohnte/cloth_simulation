#pragma once

#include "mouse.hpp"
#include "point.hpp"
#include "connection.hpp"
#include "vec.hpp"
#include <vector>

struct Cloth {
    float efficiency_factor;
    float friction_factor;
    float rigidity;
    Vec2 startpos;
    Vec2 dimensions;
    Vec2 constraint_top_left;
    Vec2 constraint_bottom_right;
    std::vector<Point*> points;
    std::vector<Connection*> connections;
};

Cloth* new_cloth(
    Vec2 startpos,
    Vec2 dimensions,
    unsigned int spacing,
    float efficiency_factor,
    float friction_factor,
    Vec2 constraint_top_left, Vec2 constraint_bottom_right
);
void destroy_cloth(Cloth* cloth);
void cloth_step(Cloth* cloth, Mouse* mouse, Vec2f gravity_vec, float timedelta);