#include "scene.h"
#include <stdlib.h>
//#define _USE_MATH_DEFINES //Per "vedere" le define come M_PI
#include "math.h"
#include "raycast.h"
#include "SDL_image.h"
#include <string.h>

#define mapWidth 24
#define mapHeight 24

//Stored in Code Segment because 'const'
const cell_type worldMap[mapWidth][mapHeight] =
{
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,2,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};


texture* load_texture(const char* path, SDL_Renderer* renderer) { 
    SDL_Surface* png_surface = IMG_Load(path);
    if( png_surface == NULL ) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not load SDL_Image PNG: %s\n", SDL_GetError());
        return NULL;
    }
   
    texture* t = malloc(sizeof(texture));
    t->width = png_surface->w;
    t->height = png_surface->h;
    t->pixel_size = png_surface->format->BytesPerPixel;
    int data_size = t->width * t->height * t->pixel_size;
    t->data = malloc(data_size);
    memcpy(t->data, png_surface->pixels, data_size);
   
    SDL_FreeSurface(png_surface);
    return t;
}


scene* scene_create(int screen_width, int screen_height, SDL_Renderer* renderer) {
    scene* s = malloc(sizeof(scene));

    s->screen = malloc(sizeof(screen));
    s->screen->width = screen_width;
    s->screen->height = screen_height;

    s->camera = malloc(sizeof(camera));
    s->camera->position = vec2_new(22, 12);
    s->camera->direction = vec2_new(-1.f, 0.f);
    s->camera->plane = vec2_new(0.f, 1.f); //plane vector must be orthogonal to camera direction
    s->camera->fov = 60.f;
    float half_fov = s->camera->fov * 0.5f;
    float half_fov_rads = half_fov * M_PI / 180.f;
    s->camera->fov_tan = tanf( half_fov_rads ); // tan(a) * |camera_direction| where magnitude(camera_direction) = 1;
    s->map = malloc(sizeof(map2d));
    s->map->rows = mapHeight;
    s->map->cols = mapWidth;
    s->map->cells = calloc(sizeof(cell_type), s->map->rows * s->map->cols);
    for (size_t r = 0; r < s->map->rows; r++) {
         for (size_t c = 0; c < s->map->cols; c++) {
            s->map->cells[c + r * s->map->cols] = (cell_type)worldMap[r][c];
        }
    }

    s->texture_count = 5;
    s->textures = malloc(sizeof(texture) * s->texture_count);
    s->textures[0] = load_texture("resources/bluestone.png", renderer);
    s->textures[1] = load_texture("resources/greystone.png", renderer);
    s->textures[2] = load_texture("resources/redbrick.png", renderer);
    s->textures[3] = load_texture("resources/wood.png", renderer);
    s->textures[4] = load_texture("resources/eagle.png", renderer);
    
    // Ref: https://wiki.libsdl.org/SDL_PixelFormatEnum 
    // SDL_PIXELFORMAT_RGBA32 alias resolved dependng on the platform as follow:
    // - SDL_PIXELFORMAT_RGBA8888 su sistemi BIG Endian
    // - SDL_PIXELFORMAT_ABGR8888 su sistemi LITTLE Endian (e.g. Windows)
    s->buffer_texture = SDL_CreateTexture(renderer,
                           SDL_PIXELFORMAT_RGBA32,
                           SDL_TEXTUREACCESS_STATIC, 
                           screen_width,
                           screen_height);
    s->buffer_depth = 4;
    s->buffer_size = sizeof(uint8) * screen_width * screen_height * s->buffer_depth;
    s->buffer_surface = malloc(s->buffer_size);
    return s;
}

void draw_column_textured(SDL_Renderer* r, int column, scene* scene, ray* ray, ray_hit* hit_info) {
    float height = scene->screen->height;
    float distance = hit_info->distance;

    //Calculate the height of line to draw
    int line_height = (int)(height / distance);

    //calculate where to draw the line
    int draw_start_y = (-line_height + height) / 2;
    if(draw_start_y < 0) draw_start_y = 0;
    int draw_end_y = (line_height + height) / 2;
    if(draw_end_y >= height) draw_end_y = height - 1;

    #pragma region Get Texture Sample and draw per pixel
    cell_side side = hit_info->side;
    int row = hit_info->cell_position.y;
    int col = hit_info->cell_position.x;
    int map_cols = scene->map->cols;
    int tex_index = scene->map->cells[col + row * map_cols] - 1; //cell value to texture index
    texture* tex = scene->textures[tex_index];

    float tex_percentage = hit_info->side_point_normalized01;
    //x coordinate on the texture
    int tex_x = (int)(tex_percentage * (float)tex->width);
    //Flipping texture x coordinate in the following cases.
    //NOTE: look at eagle wall texture. All the eagle have beak oriented to right of the viewer.
    if(side == CELL_SIDE_NORTH_SOUTH && ray->direction.x > 0) tex_x = tex->width - tex_x - 1;
    else if(side == CELL_SIDE_EAST_WEST && ray->direction.y < 0) tex_x = tex->width - tex_x - 1;

    // How much to increase the texture coordinate per screen pixel
    float tex_y_increment = (float)tex->height / (float)line_height;
    // Calculate starting y texture coordinate
    float tex_y_accumulator = (draw_start_y - height / 2 + line_height / 2) * tex_y_increment;
    for(int draw_y = draw_start_y; draw_y < draw_end_y; draw_y++) {
        // Taking integer tex y and mask with texture height(-1) in case of overflow
        // this to avoid artifacts on top or down edges of the wall (mainly from long range view)
        int tex_y = (int)tex_y_accumulator & (tex->height - 1); // In this scenario 64-1 = 63 => 0011 1111
        tex_y_accumulator += tex_y_increment;

        int texel_index = (tex_y * tex->width + tex_x) * tex->pixel_size;
        color* c = tex->data + texel_index;
        
        int pixel_index = (column + draw_y * scene->screen->width) * scene->buffer_depth;
        Uint8* pixels = (Uint8*)scene->buffer_surface;
        pixels[ pixel_index + 0] = c->r;
        pixels[ pixel_index + 1] = c->g;
        pixels[ pixel_index + 2] = c->b;
        pixels[ pixel_index + 3] = 255;
    }
    #pragma endregion
}

static boolean can_walk(map2d* map, vec2* position) {
    int x = (int)position->x;
    int y = (int)position->y;
    int w = map->cols;

    if (map->cells[x + y * w] == EMPTY) return true;
    return false;
}

static void handle_input(scene* s, float delta_time) {
    float speed = 10.f;
    float rotation_speed = 60.f; //degrees per seconds

    const Uint8* keys =  SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_DOWN]) {
        vec2 move = vec2_mul_scal(&s->camera->direction, speed * delta_time);
        vec2 new_pos = vec2_sub(&s->camera->position, &move);
        if (can_walk(s->map, &new_pos)) s->camera->position = new_pos;
    } else if (keys[SDL_SCANCODE_UP]) {
        vec2 move = vec2_mul_scal(&s->camera->direction, speed * delta_time);
        vec2 new_pos = vec2_sum(&s->camera->position, &move);
        if (can_walk(s->map, &new_pos)) s->camera->position = new_pos;
    }

    if (keys[SDL_SCANCODE_LEFT]) {
        s->camera->direction = vec2_rotate_degrees(&s->camera->direction, rotation_speed * delta_time);
        s->camera->plane = vec2_rotate_degrees(&s->camera->plane, rotation_speed * delta_time);
    } else if (keys[SDL_SCANCODE_RIGHT]) {
        s->camera->direction = vec2_rotate_degrees(&s->camera->direction, -rotation_speed * delta_time);
        s->camera->plane = vec2_rotate_degrees(&s->camera->plane, -rotation_speed * delta_time);
    }
}

void scene_update(scene* s, SDL_Renderer* r, float delta_time) {
    handle_input(s, delta_time);

    int width = s->screen->width;
    vec2 camera_pos = s->camera->position;
    vec2 camera_dir = s->camera->direction;
    vec2 plane_dir = s->camera->plane;
    float camera_fov_tan = s->camera->fov_tan;
    

    //Draw Sky and Terrain has half screen each
    for(int i=0; i < width; ++i) {
        //SKY
        for(int j=0; j <  s->screen->height / 2; ++j) {
            int index = (i + j * width) * s->buffer_depth;
            s->buffer_surface[index + 0] = 56;
            s->buffer_surface[index + 1] = 56;
            s->buffer_surface[index + 2] = 56;
            s->buffer_surface[index + 3] = 255;
        }

        //TERRAIN
        for(int j= s->screen->height / 2; j <  s->screen->height; ++j) {
            int index = (i + j * width) * s->buffer_depth;
            s->buffer_surface[index + 0] = 113;
            s->buffer_surface[index + 1] = 113;
            s->buffer_surface[index + 2] = 113;
            s->buffer_surface[index + 3] = 255;
        }
    }

    //Fire Rays and Drawing columns
    for(int col = 0; col < width; col++) {
      //plane step in range [-1, +1]
      float plane_step = 2.f * (float)col / (float)width - 1.f;
      //Multiply by the same amount 'plane_step' change the vector length
      //but keep the direction unchanged.
      float ray_dir_x = camera_dir.x + plane_dir.x * camera_fov_tan * plane_step;
      float ray_dir_y = camera_dir.y + plane_dir.y * camera_fov_tan * plane_step;

      ray ray;
      ray.origin = camera_pos;
      ray.direction = vec2_new(ray_dir_x, ray_dir_y);

      ray_hit hit;
      ray_cast(&ray, s->map, &hit);
      draw_column_textured(r, col, s, &ray, &hit);
    }

    SDL_UpdateTexture(s->buffer_texture, NULL, s->buffer_surface, s->screen->width * s->buffer_depth);
    SDL_RenderCopy(r, s->buffer_texture, NULL, NULL);
}

void scene_destroy(scene* s) {
    free(s->buffer_surface);
    SDL_DestroyTexture(s->buffer_texture);
    
    for (size_t i = 0; i < s->texture_count; i++)
    {   
        texture* t = s->textures[i];
        free(t->data);
        free(t);
    }
    free(s->textures);
    free(s->screen);
    free(s->camera);
    free(s->map->cells);
    free(s->map);
    free(s);
}