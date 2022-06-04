#include "window.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <cstdio>
#include <stdexcept>

Window* new_window(const char* window_name, Vec2 dimensions) {
    Window* new_win = new Window;
    new_win->dimensions = dimensions;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("[ERROR] Window initialization error: %s\n", SDL_GetError());
        return NULL;
    }

    new_win->sdl_win = SDL_CreateWindow(
        window_name,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        dimensions.x, dimensions.y,
        SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
    
    if (new_win->sdl_win == NULL) {
        printf("[ERROR] Could not create SDL window: %s\n", SDL_GetError());
        return NULL;
    }

    new_win->renderer = SDL_CreateRenderer(new_win->sdl_win, -1, SDL_RENDERER_PRESENTVSYNC);

    return new_win;
}

void destroy_window(Window* window) {
    if (window == NULL) {
        return;
    }

    if (window->sdl_win != NULL) {
        SDL_DestroyWindow(window->sdl_win);
        window->sdl_win = NULL;
        SDL_Quit();
    }

    if (window->renderer != NULL) {
        SDL_DestroyRenderer(window->renderer);
        window->renderer = NULL;
    }

    delete window;
    window = NULL;
}

void fill_screen(Window* window, RGB color) {
    if (window->renderer != NULL) {
        SDL_SetRenderDrawColor(window->renderer, color.r, color.g, color.b, 255);
        SDL_RenderClear(window->renderer);
    }
}

void draw_point(Window* window, Point point, RGB color) {
    if (window->renderer != NULL) {
        SDL_SetRenderDrawColor(window->renderer, color.r, color.g, color.b, 255);
        for (float angle = 0.0f; angle <= 6.283f; angle+=1.0f/point.radius ) {
            SDL_RenderDrawPoint(
            window->renderer,
            point.pos.x+point.radius*cos(angle),
            point.pos.y+point.radius*sin(angle) 
            );
        }
    }
}

void draw_connection(Window* window, Connection connection, RGB color) {
    if (window->renderer != NULL) {
        SDL_SetRenderDrawColor(window->renderer, color.r, color.g, color.b, 255);
        SDL_RenderDrawLine(
            window->renderer,
            (int) connection.p0.pos.x,
            (int) connection.p0.pos.y,
            (int) connection.p1.pos.x,
            (int) connection.p1.pos.y
        );
    }
}

void draw_cloth(Window* window, Cloth* cloth, RGB point_color, RGB connection_color, RGB selected_p_color) {
    for (const Connection* conn : cloth->connections) {
        draw_connection(window, *conn, connection_color);
    }

    for (const Point* point : cloth->points) {
        if (point->selected) {
            draw_point(window, *point, selected_p_color);
        } else {
            draw_point(window, *point, point_color);
        }
    }
}