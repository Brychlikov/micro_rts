//
// Created by brych on 20/01/2021.
//

#include "sprite.h"
#include "gamestate.h"

GENERATE_VECTOR_DEFINITION(Sprite)

void draw_sprites(GameState* gs) {
    for (int i = 0; i < VEC_LEN(gs->sprite_components); ++i) {
        // for now assert that every entity with sprite has a valid transform
        Sprite s = vec_Sprite_get(gs->sprite_components, i);
        if(vec_bool_get(gs->entities, i)) {  // if entity still alive
            Transform t = vec_Transform_get(gs->transform_components, i);
            al_draw_tinted_rotated_bitmap(s.bitmap, s.tint, s.offset.x, s.offset.y, t.position.x, t.position.y, t.rotation + s.rotation, 0);    // disregard scale for now
        }
    }
}