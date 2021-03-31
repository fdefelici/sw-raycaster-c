#ifndef vec2_H
#define vec2_H


typedef struct {
    float x;
    float y;
} vec2;

vec2 vec2_new(float x, float y);
vec2 vec2_zero();

float vec2_magn(vec2* v);
float vec2_magn2(vec2* v);
vec2 vec2_norm(vec2* v);
vec2 vec2_sub(vec2* v1, vec2* v2);
float vec2_dot(vec2* v1, vec2* v2);

vec2 vec2_sum(vec2* v1, vec2* v2);
vec2 vec2_mul_scal(vec2* v, float s);

vec2 vec2_rotate_degrees(vec2* v1, float angle_degrees);

#endif //vec2_H