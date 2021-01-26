//
// Created by brych on 19/01/2021.
//

#include "gamestate.h"

void init_components(GameState *gs) {
    gs->entities = vec_bool_new();

    gs->transform_components = vec_Transform_new();
    gs->collider_components = vec_Collider_new();
    gs->unit_components = vec_UnitComponent_new();
}

int create_entity(GameState* gs) {
    int res = gs->entities.inner->length;
    vec_bool_push(gs->entities, true);

    // for each component type, push zero
    vec_Sprite_push_zero(gs->sprite_components);
    vec_Transform_push_zero(gs->transform_components);
    vec_UnitComponent_push_zero(gs->unit_components);
    vec_Collider_push_zero(gs->collider_components);
    vec_Health_push_zero(gs->health_components);

    // add empty CollisionData element too
    CollisionData cd = {.entity=-1, .next=NULL};
    vec_CollisionData_push(gs->resources.collisions.vec, cd);

    return res;
}

void destroy_entity(GameState* gs, Entity entity) {
    *vec_bool_get_ptr(gs->entities, entity) = false;

    // for each component, set exists=false

    vec_Sprite_get_ptr(gs->sprite_components, entity)->exists = false;
    vec_Transform_get_ptr(gs->transform_components, entity)->exists = false;
    vec_UnitComponent_get_ptr(gs->unit_components, entity)->exists = false;
    vec_Collider_get_ptr(gs->collider_components, entity)->exists = false;
    vec_Health_get_ptr(gs->health_components, entity)->exists = false;
}

