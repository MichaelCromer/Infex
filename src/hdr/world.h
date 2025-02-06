#ifndef INFEX_WORLD_H
#define INFEX_WORLD_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>


/* GRID */
size_t grid_face_index(size_t r, size_t c);
size_t grid_vert_index(size_t r, size_t c);
size_t grid_num_faces(void);
size_t grid_num_vertices(void);
size_t grid_num_rows(void);
size_t grid_num_cols(void);
Vector2 *grid_faces(void);
Vector2 grid_face(size_t i);
Vector2 *grid_vertices(void);
Vector2 grid_vert(size_t i);

/* MAP */
Color *map_colours(void);
uint8_t *map_slopes(void);

/* ENEMY */
float *enemy_state(void);
float enemy_score(void);

/* WORLD */
void world_initialise(size_t rows, size_t cols);
void world_generate(void);
void world_update(float dt);

Vector2 world_bounds(void);
Vector2 world_bounds_upper(void);
Vector2 world_bounds_lower(void);
Vector2 world_centre(void);

#endif
