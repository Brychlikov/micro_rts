//
// Created by brych on 19/01/2021.
//

#include "gamestate.h"

void init_components(GameState *gs) {
    gs->entities = vec_bool_new();
    printf("Entities init'ed at   %lx, buffer at %x with size %d\n", gs->entities.inner, gs->entities.inner->buf, gs->entities.inner->type_size);

    gs->sprite_components = vec_Sprite_new();
    gs->transform_components = vec_Transform_new();
    gs->collider_components = vec_Collider_new();
    gs->unit_components = vec_UnitComponent_new();
    printf("Spites init'ed at     %lx, buffer at %x with size %d\n", gs->sprite_components.inner, gs->sprite_components.inner->buf, gs->sprite_components.inner->type_size);
    printf("Transforms init'ed at %lx, buffer at %x with size %d\n", gs->transform_components.inner, gs->transform_components.inner->buf, gs->transform_components.inner->type_size);
    printf("UnitsComps init'ed at %lx, buffer at %x with size %d\n", gs->unit_components.inner, gs->unit_components.inner->buf, gs->unit_components.inner->type_size);
    printf("Colliders init'ed at  %lx, buffer at %x with size %d\n", gs->collider_components.inner, gs->collider_components.inner->buf, gs->collider_components.inner->type_size);
}

int create_entity(GameState* gs) {
    int res = gs->entities.inner->length;
    vec_bool_push(gs->entities, true);

    // for each component type, push zero
    vec_Sprite_push_zero(gs->sprite_components);
    vec_Transform_push_zero(gs->transform_components);
    vec_UnitComponent_push_zero(gs->unit_components);
    vec_Collider_push_zero(gs->collider_components);

    return res;
}

