#pragma once

#include "vec.hpp"

struct Mouse {
    bool left_button_clicked;
    float cursor_radius_size;
    Vec2 pos;
    Vec2 last_pos;
};

Mouse* new_mouse(Vec2 position, float cursor_size);
void destroy_mouse(Mouse* mouse);