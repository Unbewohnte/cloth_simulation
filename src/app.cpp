#include "app.hpp"
#include "cloth.hpp"
#include "mouse.hpp"
#include "window.hpp"
#include "vec.hpp"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>

App* app_init(App_config conf) {
    App* app = new App;

    if (conf.window_dimensions.x <= 0) {
        conf.window_dimensions.x = 500;
    }
    if (conf.window_dimensions.y <= 0) {
        conf.window_dimensions.y = 500;
    }
    if (conf.efficiency_factor < 0 || conf.efficiency_factor > 1) {
        conf.efficiency_factor = 0.6;
    }
    app->conf = conf;
    
    app->mouse = new_mouse(Vec2{0, 0}, conf.cursor_radius);
    app->window = new_window(conf.win_name, conf.window_dimensions);
    app->cloth = new_cloth(
        conf.cloth_startpos,
        conf.cloth_dimensions,
        conf.cloth_spacing,
        conf.efficiency_factor, 
        conf.friction_factor,
        Vec2{0, 0}, Vec2{app->window->dimensions.x-5, app->window->dimensions.y-5}
    );

    if (app->window == NULL || app->cloth == NULL) {
        return NULL;
    }

    return app;  
}

void app_handle_input(App* app) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {

        switch (event.type) {
        case SDL_QUIT:
            app->is_running = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    app->is_running = false;
                    break;
                
                case SDLK_q:
                    app->is_running = false;
                    break;

                case SDLK_SPACE:
                    toggle_pause(app);
                    break; 
                
                case SDLK_LEFT:
                    if (app->conf.timestep >= 0.15f) {
                        app->conf.timestep -= 0.15f;
                    }
                    break;
                
                case SDLK_RIGHT:
                    app->conf.timestep += 0.15f;
                    break;
            }

        case SDL_MOUSEMOTION:
            app->mouse->last_pos = app->mouse->pos;
            app->mouse->pos = Vec2{
                static_cast<unsigned int>(event.motion.x),
                static_cast<unsigned int>(event.motion.y)
            };
            break;        
        
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT) {
                app->mouse->left_button_clicked = true;
            }
            break;

        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT) {
                app->mouse->left_button_clicked = false;
            }
            break;
        }

    }
}

void app_render(App* app) {
    SDL_RenderClear(app->window->renderer);    
    fill_screen(app->window, app->conf.background_color);
    draw_cloth(
        app->window, app->cloth,
        app->conf.point_color, app->conf.connection_color, app->conf.point_selected_color);
    SDL_RenderPresent(app->window->renderer);
}

void app_update(App* app) {
    if (app->paused) {
        return;
    }

    int updated_window_w;
    int updated_window_h;
    SDL_GetWindowSize(app->window->sdl_win, &updated_window_w, &updated_window_h);
    app->window->dimensions = Vec2{(unsigned int) updated_window_w, (unsigned int) updated_window_h};
    if (app->cloth->constraint_bottom_right.x != (unsigned int) updated_window_w || app->cloth->constraint_bottom_right.y != (unsigned int) updated_window_h) {
        app->cloth->constraint_bottom_right.x = app->window->dimensions.x;
        app->cloth->constraint_bottom_right.y = app->window->dimensions.y-5;
    }

    cloth_step(app->cloth, app->mouse, app->conf.gravity, app->conf.timestep);
}

void destroy_app(App* app) {
    destroy_mouse(app->mouse);
    destroy_cloth(app->cloth);
    destroy_window(app->window);
    app->mouse = NULL;
    app->cloth = NULL;
    app->window = NULL;
    app->is_running = false;
}

void toggle_pause(App* app) {
    if (app->paused) {
        app->paused = false;
    } else {
        app->paused = true;
    }
}