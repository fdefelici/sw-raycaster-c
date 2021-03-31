#include "vec2.h"
#define _USE_MATH_DEFINES //Per "vedere" le define come M_PI
#include "math.h"

vec2 vec2_new(float x, float y) {
    vec2 v;
    v.x = x;
    v.y = y;
    return v;
}

vec2 vec2_zero() {
    return vec2_new(0.f, 0.f);
}

float vec2_magn(vec2* v) {
    return sqrtf(vec2_magn2(v));
}

float vec2_magn2(vec2* v) {
    return vec2_dot(v, v);
}

vec2 vec2_norm(vec2* v) {
    vec2 n;
    float magn = vec2_magn(v);
    n.x = v->x / magn;
    n.y = v->y / magn;
    return n;
}

float vec2_dot(vec2* v1, vec2* v2) {
    return v1->x * v2->x + v1->y * v2->y;
}

vec2 vec2_sub(vec2* v1, vec2* v2) {
    vec2 r;
    r.x = v1->x - v2->x;
    r.y = v1->y - v2->y;
    return r;
}


vec2 vec2_sum(vec2* v1, vec2* v2) {
    vec2 r;
    r.x = v1->x + v2->x;
    r.y = v1->y + v2->y;
    return r;
}

vec2 vec2_mul_scal(vec2* v, float s) {
    vec2 r;
    r.x = v->x * s;
    r.y = v->y * s;
    return r;
}

vec2 vec2_rotate_degrees(vec2* v, float angle_degrees) {
    float angle_radians = angle_degrees * M_PI / 180.f;
    vec2 res;
    res.x = v->x * cos(angle_radians) - v->y * sin(angle_radians);
    res.y = v->x * sin(angle_radians) + v->y * cos(angle_radians);
    return res;
}
      
