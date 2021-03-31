#ifndef RAYCAST_H
#define RAYCAST_H
#include "vec2.h"
#include "scene.h"
#include "common.h"

typedef struct {
    vec2 origin;
    vec2 direction;
} ray;

typedef struct {
    float distance;
    vec2 cell_position;
    cell_type cell_type;
    cell_side side;
    float side_point;
    float side_point_normalized01;
} ray_hit;

void ray_cast(ray* ray, map2d* map, ray_hit* hit_out);

#endif 