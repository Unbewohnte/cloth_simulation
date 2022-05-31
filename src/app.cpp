#include "app.hpp"
#include "cloth.hpp"
#include "window.hpp"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
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
    app->conf = conf;
    
    app->window = new_window(conf.win_name, conf.window_dimensions);
    app->cloth = new_cloth(conf.cloth_startpos, conf.cloth_dimensions, conf.cloth_spacing);

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
            }        
        }
    }
}

void app_render(App* app) {    
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

    compute_cloth_forces(app->cloth, app->conf.gravity);
    satisfy_cloth_constraints(app->cloth, Vec2{0, 0}, Vec2{app->window->dimensions.x-5, app->window->dimensions.y-5});
}

void destroy_app(App* app) {
    destroy_cloth(app->cloth);
    destroy_window(app->window);
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