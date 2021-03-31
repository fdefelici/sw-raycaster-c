#ifndef COLOR_H
#define COLOR_H

typedef unsigned char uint8;

typedef struct {
    uint8 r;
    uint8 g;
    uint8 b;
} color;

static color color_red() {
    color c;
    c.r = 255;
    c.g = 0;
    c.b = 0;
    return c;
}

static color color_blue() {
    color c;
    c.r = 0;
    c.g = 0;
    c.b = 255;
    return c;
}

static color color_yellow() {
    color c;
    c.r = 255;
    c.g = 255;
    c.b = 0;
    return c;
}

static color color_green() {
    color c;
    c.r = 0;
    c.g = 255;
    c.b = 0;
    return c;
}

static color color_purple() {
    color c;
    c.r = 252;
    c.g = 15;
    c.b = 192;
    return c;
}



static color color_black() {
    color c;
    c.r = 0;
    c.g = 0;
    c.b = 0;
    return c;
}

static color color_mul(color* c, float s) {
    color res;
    res.r = c->r * s;
    res.g = c->g * s;
    res.b = c->b * s;
    return res;
}
#endif