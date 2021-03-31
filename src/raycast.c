#include "raycast.h"
#include "float.h"

void ray_cast(ray* ray, map2d* map, ray_hit* hit_out) {
    float pos_x = ray->origin.x;
    float pos_y = ray->origin.y;

    //Current cell on the 2d map. Just taking the integer part of player position
    int cell_x = (int)pos_x;
    int cell_y = (int)pos_y;

    float ray_dir_x = ray->direction.x;
    float ray_dir_y = ray->direction.y;

    //Distance side-to-side on x and y axis
    // Note: Protect against division by 0, even if in C doesn't generate error and handled properly with "inf"
    float next_dist_x;
    if (ray_dir_x == 0) next_dist_x = 1;
    else next_dist_x = sqrt(1 + (ray_dir_y * ray_dir_y) / (ray_dir_x * ray_dir_x));

    float next_dist_y;
    if (ray_dir_y == 0) next_dist_y = 1;
    next_dist_y = sqrt(1 + (ray_dir_x * ray_dir_x) / (ray_dir_y * ray_dir_y)); 

    //Cell increment valued -1 or +1 based on the ray direction
    int cell_increment_x;
    int cell_increment_y;
    cell_side wall_side_hit; //which side of the wall cell has been hit

    //length of ray from current position to the cell side with a wall hit
    float ray_length_x;
    float ray_length_y;

    //Setup cell increment and initial ray length (from current position to the next closest cell)
    if(ray_dir_x < 0) {
        cell_increment_x = -1;
        ray_length_x = (pos_x - cell_x) * next_dist_x;
    } else {
        cell_increment_x = 1;
        ray_length_x = (cell_x + 1.0 - pos_x) * next_dist_x;
    }

    if(ray_dir_y < 0) {
        cell_increment_y = -1;
        ray_length_y = (pos_y - cell_y) * next_dist_y;
    } else {
        cell_increment_y = 1;
        ray_length_y = (cell_y + 1.0 - pos_y) * next_dist_y;
    }
    
    //DDA: Look for the shortest distance in straight line among two points in 2d
    cell_type type;
    while ( (type = map->cells[cell_x + cell_y * map->cols]) == EMPTY ) {
        //Move to the next closest cell depending on the shortest movement on x or y
        if(ray_length_x < ray_length_y) {
            ray_length_x += next_dist_x;
            cell_x += cell_increment_x;
            wall_side_hit = CELL_SIDE_NORTH_SOUTH;
        } else {
            ray_length_y += next_dist_y;
            cell_y += cell_increment_y;
            wall_side_hit = CELL_SIDE_EAST_WEST;
        }
    }

    //Calculate distance projected on camera direction 
    //NOTE: Euclidean distance will give fisheye effect, because middle rays are shorter than side rays
    //      and we calculate the line_height = screen_height / ray_length 
    //float pos_to_wall_distance = sqrt(ray_length_x * ray_length_x + ray_length_y * ray_length_y);
    float pos_to_wall_distance;
    float side_hit_point;
    //Avoid fisheye calculating the ray distance as it were perpendicular to the plane (like view direction)
    if(wall_side_hit == CELL_SIDE_NORTH_SOUTH) {
        pos_to_wall_distance = (cell_x - pos_x + (1 - cell_increment_x) / 2) / ray_dir_x;
        side_hit_point = pos_y + pos_to_wall_distance * ray_dir_y;
    }
    else {
        pos_to_wall_distance = (cell_y - pos_y + (1 - cell_increment_y) / 2) / ray_dir_y;
        side_hit_point = pos_x + pos_to_wall_distance * ray_dir_x;
    }

    hit_out->distance = pos_to_wall_distance;
    hit_out->cell_position = vec2_new(cell_x, cell_y);
    hit_out->cell_type = type;
    hit_out->side = wall_side_hit;  //eventually useful to implement shading on wall sides
 
    hit_out->side_point = side_hit_point;
    hit_out->side_point_normalized01 = side_hit_point - floorf(side_hit_point);
}