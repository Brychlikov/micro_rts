//
// Created by brych on 15/01/2021.
//

#ifndef _RTS_COORD_UTILS_H
#define _RTS_COORD_UTILS_H

#define RECT_COORDINATES(rect) rect.tl.x, rect.tl.y, rect.br.x, rect.br.y

typedef enum {
    APART,
    COLLISION
} Collision;

typedef struct {
    float x; float y;
} Point;

typedef struct {
    Point tl;
    Point br;
} Rect;

typedef struct {
    float x;
    float y;
} Vec2;

Point point_from_vec2(Vec2 v);

Vec2 vec2_from_point(Point p);

Vec2 vec2_add(Vec2 a, Vec2 b);

Vec2 vec2_flip(Vec2 v);

Vec2 vec2_sub(Vec2 a, Vec2 b);

Vec2 vec2_scale(Vec2 v, float scalar);

float vec2_length(Vec2 v);

Vec2 vec2_norm(Vec2 v);

Collision rect_collide(Rect r1, Rect r2);

#endif //_RTS_COORD_UTILS_H
