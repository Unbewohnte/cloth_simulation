#pragma once

#include "point.hpp"

struct Connection {
    Point& p0;
    Point& p1;
    float rigidity;
    float initial_length;
    float length;
};