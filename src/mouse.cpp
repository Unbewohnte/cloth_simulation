#include "mouse.hpp"

Mouse* new_mouse(Vec2 position, float cursor_size) {
    return new Mouse{
        .left_button_clicked = false,
        .cursor_radius_size = cursor_size,
        .pos = position,
        .last_pos = position,
    };
}

void destroy_mouse(Mouse* mouse) {
    delete mouse;
}