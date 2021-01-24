#include "coord_utils.h"
#include "math.h"
#include "vector.h"

GENERATE_VECTOR_DEFINITION(Transform)
GENERATE_VECTOR_DEFINITION(Vec2)

Vec2 vec2_make(float x, float y) {
    Vec2 res = {.x=x, .y=y};
    return res;
}

Vec2 vec2_add(Vec2 a, Vec2 b) {
    Vec2 res = {
            .x=a.x + b.x,
            .y=a.y + b.y
    };
    return res;
}

Vec2 vec2_flip(Vec2 v) {
    Vec2 res = {
            .x = -v.x,
            .y = -v.y
    };
    return res;
}

Vec2 vec2_scale(Vec2 v, float scalar) {
    Vec2 res = {
            .x = v.x * scalar,
            .y = v.y * scalar
    };
    return res;
}

Vec2 vec2_sub(Vec2 a, Vec2 b) {
    return vec2_add(a, vec2_flip(b));
}

float vec2_length(Vec2 v) {
    return sqrtf(v.x * v.x + v.y * v.y);
}

float vec2_length_sq(Vec2 v) {
    return v.x * v.x + v.y * v.y;
}

Vec2 vec2_norm(Vec2 v) {
    return vec2_scale(v, 1/vec2_length(v));
}

Rect rect_local_to_global(Rect r, Vec2 global_offset) {
    Rect res = {
            .tl= {
                    .x = r.tl.x + global_offset.x,
                    .y = r.tl.y + global_offset.y
            },

            .br= {
                    .x = r.br.x + global_offset.x,
                    .y = r.br.y + global_offset.y
            }
    };
    return res;
}

Collision rect_collide(Rect r1, Rect r2) {
    // (0, 0) is the top left corner !!
    if(r1.tl.x > r2.br.x) return APART;
    if(r1.tl.y > r2.br.y) return APART;
    if(r1.br.y < r2.tl.y) return APART;
    if(r1.br.x < r2.tl.x) return APART;
    return COLLISION;
}

