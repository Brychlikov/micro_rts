//
// Created by brych on 24/01/2021.
//

#include "health.h"
#include "gamestate.h"

GENERATE_VECTOR_DEFINITION(Health)

void init_health(GameState* gs) {
    gs->health_components = vec_Health_new();
}

void deinit_health(GameState *gs) {
    vec_Health_destroy(gs->health_components);
}

void health_system(GameState* gs) {
    for (unsigned int i = 0; i < VEC_LEN(gs->entities); ++i) {
        if(!vec_bool_get(gs->entities, i)) continue;

        Health h = vec_Health_get(gs->health_components, i);
        if(!h.exists) continue;
        if(h.points <= 0) {
            destroy_entity(gs, i);
        }
    }
}
