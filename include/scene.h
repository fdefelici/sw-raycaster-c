#ifndef SCENE_H
#define SCENE_H
#include "vec2.h"
#include "SDL.h"
#include "color.h"

typedef struct {
    int width;
    int height;
} screen;

typedef struct {
    vec2 position;
    vec2 direction;
    vec2 plane;
    float fov;
    float fov_tan;
} camera;



enum map_element_enum { EMPTY=0, BLUE_WALL, RED_WALL, GREEN_WALL, YELLOW_WALL };
typedef unsigned char cell_type;

enum cell_side_enum { CELL_SIDE_NORTH_SOUTH=0, CELL_SIDE_EAST_WEST=1 };
typedef unsigned char cell_side;



typedef struct {
    cell_type* cells;
    int rows;
    int cols;
} map2d;

typedef struct {
    int width;
    int height;
    int pixel_size;
    unsigned char* data;

} texture;

typedef struct {
    screen* screen;
    camera* camera;
    map2d* map;
    texture** textures;
    int texture_count;
    SDL_Texture* buffer_texture;
    uint8* buffer_surface;
    int buffer_size;
    int buffer_depth;
} scene;

scene* scene_create(int w, int h, SDL_Renderer* r);
void scene_update(scene*, SDL_Renderer* renderer, float delta_time);
void scene_destroy(scene*);


texture* load_texture(const char* path, SDL_Renderer* r);

#endif //SCENE_H