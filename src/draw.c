#include "hdr/camera.h"
#include "hdr/draw.h"
#include "hdr/infex.h"
#include "hdr/interface.h"
#include "hdr/mouse.h"
#include "hdr/world.h"

typedef struct InfexDrawAsset {
    Texture2D texture;
    Rectangle bounds;
    Vector2 centre;
} InfexDrawAsset;

void asset_load(InfexDrawAsset *asset, const char *file_name)
{
    asset->texture = LoadTexture(file_name);
    asset->bounds = (Rectangle) { 0, 0, asset->texture.width, asset->texture.height };
    asset->centre = (Vector2) {
        asset->texture.width / 2.0f,
        asset->texture.height / 2.0f
    };
}

InfexDrawAsset hex_tile = { 0 };

void draw_initialise(void)
{
    asset_load(&hex_tile, "res/img/hex_test4.png");
}

void draw_enemy(void)
{
    float *enemy = enemy_state();
    float scale = grid_scale();
    for (size_t i = 0; i < grid_num_faces(); i++) {
        if (FloatEquals(enemy[i], 0.0f)) {
            continue;
        }
        Color colour = RED;
        DrawPoly(grid_face(i), 6, scale - 6, 30.0f, colour);
    }
}

void draw_edge(size_t i, enum GRID_DIR d)
{
    uint8_t weight = 0;
    switch (d) {
        case DIR_NE:
        case DIR_NW:
            weight = 1;
            break;
        case DIR_EE:
        case DIR_WW:
            weight = 2;
            break;
        case DIR_SE:
        case DIR_SW:
            weight = 5;
            break;
        default:
            break;
    }

    Vector2 start = grid_vertex_clockwise_from(i, d);
    Vector2 end = grid_vertex_anticlockwise_from(i, d);

    DrawLineEx(start, end, weight, BLACK);
}

void draw_slopes(size_t i)
{
    for (enum GRID_DIR d = 0; d < NUM_GRID_DIRS; d++) {
        if (map_slope(i, d)) draw_edge(i, d);
    }
}

void draw_tile(Vector2 pos, float scale, Color colour)
{
    (void)scale;
    DrawTexturePro(
        hex_tile.texture,
        hex_tile.bounds,
        (Rectangle) { pos.x, pos.y, hex_tile.texture.width, hex_tile.texture.height },
        hex_tile.centre,
        0,
        colour
    );
//    DrawPoly(pos, 6, scale, 30.0f, colour);
}

void draw_map_debug(void)
{
    Vector2 *faces = grid_faces();
    Vector2 *vertices = grid_vertices();

    for (size_t i = 0; i < grid_num_faces(); i++) {
        DrawCircleV(faces[i], 2, RED);
    }

    for (size_t i = 0; i < grid_num_vertices(); i++) {
        DrawCircleV(vertices[i], 2, YELLOW);
    }
}

void draw_map(void)
{
    Vector2 *faces = grid_faces();
    Color *colours = map_colours();
    float scale = grid_scale();

    for (size_t i = 0; i < grid_num_faces(); i++) {
        draw_tile(faces[i], scale, colours[i]);
        draw_slopes(i);
    }

    if (is_debug()) draw_map_debug();
}

void draw_world(void)
{
    draw_map();
    draw_enemy();
}

void draw_mouse(void)
{
    if (mouse_is_track_face()) {
        Vector2 *faces = grid_faces();
        float scale = grid_scale();
        DrawPoly(faces[mouse_face()], 6, scale/2, 30.0f, YELLOW);
    }
}

void draw_interface(void)
{
    draw_mouse();
    interface_render();
}

void draw_screen_title(void)
{
    BeginMode2D(*camera_state());
    draw_world();
    draw_interface();
    EndMode2D();
}

void draw_screen_game(void)
{
    DrawRectangle(0, 0, screen_width(), screen_height(), PURPLE);
    DrawText("In-Game", 20, 20, 40, MAROON);

    BeginMode2D(*camera_state());
    draw_world();
    draw_interface();
    EndMode2D();
}

void draw_screen_none(void)
{
    DrawRectangle(0, 0, screen_width(), screen_height(), LIGHTGRAY);
    DrawText("NULL Screen", 20, 20, 40, GRAY);
}

void draw_screen(void)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    switch (screen_curr()) {
        case INFEX_SCREEN_MAINMENU:
            draw_screen_title();
            break;
        case INFEX_SCREEN_GAME:
            draw_screen_game();
            break;
        default:
            draw_screen_none();
            break;
    }

    EndDrawing();
}
