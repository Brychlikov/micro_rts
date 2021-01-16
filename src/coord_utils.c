#include "coord_utils.h"
#include "math.h"


Vec2 vec2_from_point(Point p) {
    return *(Vec2*)&p;
}

Point point_from_vec2(Vec2 v) {
    return *(Point *) &v;
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

Vec2 vec2_norm(Vec2 v) {
    return vec2_scale(v, 1/vec2_length(v));
}

Collision rect_collide(Rect r1, Rect r2) {
    // (0, 0) is the top left corner !!
    if(r1.tl.x > r2.br.x) return APART;
    if(r1.tl.y > r2.br.y) return APART;
    if(r1.br.y < r2.tl.y) return APART;
    if(r1.br.x < r2.tl.x) return APART;
    return COLLISION;
}
