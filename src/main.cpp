#include "app.hpp"
#include <SDL2/SDL_error.h>
#include <cstdlib>
#include <ostream>
#include <string>
#include <thread>
#include <SDL2/SDL_timer.h>

const char* HELP_ARG = "--help";
const char* HELP_ARG_LITTLE = "-h";
const char* WINDOW_NAME_ARG = "--window_name";
const char* WINDOW_NAME_ARG_LITTLE = "-wn";
const char* FRAMERATE_ARG = "--framerate";
const char* FRAMERATE_ARG_LITTLE = "-fr";

void print_help() {
    printf("cloth_sim\n");
    
    printf("\nARGS\n");
    printf("%s or %s --> print this message and exit\n", HELP_ARG, HELP_ARG_LITTLE);
    printf("%s (string) or %s (string) --> set a custom window name\n", WINDOW_NAME_ARG, WINDOW_NAME_ARG_LITTLE);
    printf("%s (unsigned integer) or %s (unsigned integer) --> set framerate\n", FRAMERATE_ARG, FRAMERATE_ARG_LITTLE);

    printf("\nKEYS\n");
    printf("ESC or q --> exit\n");
    printf("SPACE --> pause\n");
}

int main(const int argc, char** argv) {
    std::string window_name = "cloth simulation";
    unsigned int FRAMERATE = 60;
    const unsigned int FRAME_TIME = 1000.0f / FRAMERATE;

    unsigned int i = 0;
    while (i < (unsigned int) argc) {
        if (strcmp(argv[i], HELP_ARG) == 0 || strcmp(argv[i], HELP_ARG_LITTLE) == 0) {
            print_help();
            return 0;
        }

        if (strcmp(argv[i], WINDOW_NAME_ARG) == 0 || strcmp(argv[i], WINDOW_NAME_ARG_LITTLE) == 0) {
            if (i == (unsigned int) (argc-1)) {
                break;
            }

            i++;
            window_name = std::string(argv[i]);
        }

        if (strcmp(argv[i], FRAMERATE_ARG) == 0 || strcmp(argv[i], FRAMERATE_ARG_LITTLE) == 0) {
            if (i == (unsigned int) (argc-1)) {
                break;
            }

            i++;
            FRAMERATE =  std::stoi(argv[i]);
            if (FRAMERATE == 0) {
                FRAMERATE = 60;
            }
        }

        i++;
    }

    App_config config = {
        .win_name = window_name.c_str(),
        .window_dimensions = Vec2{750, 700},
        .cloth_startpos = Vec2{50, 50},
        .cloth_dimensions = Vec2{400, 400},
        .cloth_spacing = 20,
        .gravity = Vec2f{0.65f, 0.85f},
        .efficiency_factor = 0.25f,
        .timestep = 1.0f,
        .background_color = RGB{205, 222, 242},
        .point_color = RGB{2, 10, 1},
        .point_selected_color = RGB{200, 40, 12},
        .connection_color = RGB{48, 142, 31},
    };
    App* app = app_init(config);

    app->is_running = true;
    unsigned int frame_start_time, total_frame_time;
    while (app->is_running) {
        frame_start_time = SDL_GetTicks();

        app_handle_input(app);
        app_update(app);
        app_render(app);

        total_frame_time = SDL_GetTicks() - frame_start_time;
        if (total_frame_time < FRAME_TIME) {
            SDL_Delay(FRAME_TIME - total_frame_time);
        }
    }

    destroy_app(app);

    return 0;
}