#pragma once

#include "point.hpp"
#include "connection.hpp"
#include "vec.hpp"
#include <vector>

struct Cloth {
    Vec2 startpos;
    Vec2 dimensions;
    std::vector<Point*> points;
    std::vector<Connection*> connections;
};

Cloth* new_cloth(Vec2 startpos, Vec2 dimensions, unsigned int spacing);
void destroy_cloth(Cloth* cloth);
void compute_cloth_forces(Cloth* cloth, Vec2f gravity_vec);