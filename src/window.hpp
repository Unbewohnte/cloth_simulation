#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include "cloth.hpp"
#include "color.hpp"

struct Window {
    Vec2 dimensions;
    SDL_Window* sdl_win;
    SDL_Renderer* renderer;
};

Window* new_window(const char* window_name, Vec2 dimensions);
void destroy_window(Window* window);
void render_cloth(Window* window, Cloth* cloth);
void fill_screen(Window* window, RGB color);
void draw_point(Window* window, Point point);
void draw_connection(Window* window, Connection connection);
void draw_cloth(Window* window, Cloth* cloth, RGB point_color, RGB connection_color, RGB selected_p_color);