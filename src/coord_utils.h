//
// Created by brych on 15/01/2021.
//

// A number of useful constructs for dealing with coordinates. This module defines Vec2 type with algebraic functions
// and a Rect type with simple overlapping check.

#ifndef _RTS_COORD_UTILS_H
#define _RTS_COORD_UTILS_H

#include "vector.h"

#define RECT_COORDINATES(rect) rect.tl.x, rect.tl.y, rect.br.x, rect.br.y

typedef enum {
    APART,
    COLLISION
} Collision;

typedef struct {
    float x;
    float y;
} Vec2;

typedef struct {
    Vec2 tl;
    Vec2 br;
} Rect;

typedef struct {
    bool exists;
    Vec2 position;
    float rotation;
    float scale;
    int entity;
} Transform;

GENERATE_VECTOR_DECLARATION(Transform)
GENERATE_VECTOR_DECLARATION(Vec2)

Vec2 vec2_make(float x, float y);

Vec2 vec2_unit_from_radius(float radius);

Vec2 vec2_add(Vec2 a, Vec2 b);

Vec2 vec2_flip(Vec2 v);

Vec2 vec2_sub(Vec2 a, Vec2 b);

Vec2 vec2_scale(Vec2 v, float scalar);

float vec2_length(Vec2 v);

float vec2_length_sq(Vec2 v);

Vec2 vec2_norm(Vec2 v);

Rect rect_norm(Rect r);

Rect rect_local_to_global(Rect r, Transform entity_transform);

Collision rect_collide(Rect r1, Rect r2);

#endif //_RTS_COORD_UTILS_H
