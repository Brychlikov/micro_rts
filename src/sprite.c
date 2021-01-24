//
// Created by brych on 20/01/2021.
//

#include "sprite.h"
#include "gamestate.h"

GENERATE_VECTOR_DEFINITION(Sprite)

void init_sprites(GameState* gs) {
    gs->sprite_components = vec_Sprite_new();
    gs->resources.sprites.unit = al_load_bitmap("assets/unit.png");
    if (gs->resources.sprites.unit == NULL) {
        fprintf(stderr, "Could not load unit sprite\n");
    }
    gs->resources.sprites.building = al_load_bitmap("assets/building.png");
    if(gs->resources.sprites.building == NULL) {
        fprintf(stderr, "Could not load building sprite\n");
    }
}

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