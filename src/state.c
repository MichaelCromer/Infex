#include "include/infex.h"
#include "include/state.h"

void state_update(struct State *state)
{
    float dt = GetFrameTime();

    switch (state->screen_curr) {
        case INFEX_SCREEN_TITLE:
            if (IsKeyPressed(KEY_ENTER)) {
                world_initialise(32, 32);
                world_generate();
                camera_set_offset((Vector2) {
                              state->screen_width_px / 2.0f,
                              state->screen_height_px / 2.0f}
                );
                state->screen_curr = INFEX_SCREEN_GAME;
            }
            break;
        case INFEX_SCREEN_GAME:
            if (IsKeyPressed(KEY_ENTER)) {
                state->screen_curr = INFEX_SCREEN_TITLE;
            }

            if (IsKeyDown(KEY_W)) {
                camera_pan((Vector2) { 0.0f, -1.0f});
            }

            if (IsKeyDown(KEY_A)) {
                camera_pan((Vector2) { -1.0f, 0.0f});
            }

            if (IsKeyDown(KEY_S)) {
                camera_pan((Vector2) { 0.0f, 1.0f});
            }

            if (IsKeyDown(KEY_D)) {
                camera_pan((Vector2) { 1.0f, 0.0f});
            }

            break;
        default:
            break;
    }

    camera_update(dt);
    mouse_update(dt);
}
