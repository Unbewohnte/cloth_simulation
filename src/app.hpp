#pragma once

#include "window.hpp"
#include "cloth.hpp"
#include <SDL2/SDL_render.h>

struct App_config {
    const char* win_name;
    Vec2 window_dimensions;
    Vec2 cloth_startpos;
    Vec2 cloth_dimensions;
    unsigned int cloth_spacing;
    Vec2f gravity;
    float efficiency_factor;
    float friction_factor;
    RGB background_color;
    RGB point_color;
    RGB point_selected_color;
    RGB connection_color;
};

struct App {
    bool is_running;
    bool paused;
    App_config conf;
    Window* window;
    Cloth* cloth;
};

App* app_init(App_config conf);

void app_update(App* app);
void app_handle_input(App* app);
void app_render(App* app);
void destroy_app(App* app);
void toggle_pause(App* app);