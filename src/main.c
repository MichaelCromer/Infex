#include "include/draw.h"
#include "include/state.h"

int main(void)
{
    struct State state = {
        .screen_curr = INFEX_SCREEN_TITLE,
        .screen_width_px = 800,
        .screen_height_px = 450,
    };

    InitWindow(state.screen_width_px, state.screen_height_px, "Infex");

    while (!WindowShouldClose()) {
        draw_screen(&state);
        state_update(&state);
    }

    CloseWindow();
    return 0;
}
