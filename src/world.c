#include <string.h>

#include "include/world.h"

#define MAX_U (64)
#define MAX_V (64)
#define MAX_TILES ((MAX_U) * (MAX_V))
#define UV_SCALE (128)
#define ROOT_3 (1.7320508f)
#define ROOT_3_INV (0.5773502f)
#define DELTA_U (UV_SCALE * ROOT_3)
#define DELTA_V (UV_SCALE / 2.0f)

/* scalar function map data */
Vector2 faces[MAX_TILES] = { 0 }; /* geometric locations on-screen of tile faces */
uint8_t heights[MAX_TILES] = { 0 }; /* heightmap of tiles */
float enemy[MAX_TILES] = { 0 };     /* heightmap of enemy growth */

/* current state */
size_t num_tiles = 0;
size_t num_rows = 0,
       num_cols = 0;

Vector2 centre = { 0 };
Vector2 bounds = { 0 }; /* lower bounds always 0, upper bounds given here */

void world_initialise(size_t rows, size_t cols)
{
    num_rows = rows, num_cols = cols;
    num_tiles = rows * cols;

    float u = 0, v = 0;
    Vector2 *ptr = faces;
    for (size_t r = 0; r < rows; r++) {
        u = (r % 2) ? (DELTA_U / 2) : 0.0f;
        for (size_t c = 0; c < cols; c++) {
            *ptr = (Vector2) {u, v};
            u += DELTA_U;
            ptr++;
        }
        v += DELTA_V;
    }

    bounds = (Vector2) { cols * DELTA_U, rows * DELTA_V };
    centre = (Vector2) { bounds.x / 2.0f, bounds.y / 2.0f };
}

size_t world_num_tiles(void)
{
    return num_tiles;
}

Vector2 *world_faces(void)
{
    return faces;
}

uint8_t *world_heights(void)
{
    return heights;
}

Vector2 world_centre(void)
{
    return centre;
}

Vector2 world_bounds(void)
{
    return bounds;
}
