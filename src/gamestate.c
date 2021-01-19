//
// Created by brych on 19/01/2021.
//

#include "gamestate.h"

void init_components(GameState *gs) {
    gs->entities = vec_bool_new();
    gs->sprite_components = vec_Sprite_new();
    gs->transform_components = vec_Transform_new();
}

int create_entity(GameState* gs) {
    int res = gs->entities.inner->length;
    vec_bool_push(gs->entities, true);

    // for each component type, push zero
    vec_Sprite_push_zero(gs->sprite_components);
    vec_Transform_push_zero(gs->transform_components);

    return res;
}

