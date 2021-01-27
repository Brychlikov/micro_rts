//
// Created by brych on 20/01/2021.
//

#include <math.h>
#include "sprite.h"
#include "gamestate.h"
#include "unit.h"
#include "collider.h"
#include "allegro5/allegro_primitives.h"

GENERATE_VECTOR_DEFINITION(Collider)
GENERATE_VECTOR_DEFINITION(CollisionData)

void init_colliders(GameState* gs) {
    gs->resources.collisions.vec = vec_CollisionData_new();
    gs->collider_components = vec_Collider_new();
    gs->resources.collisions.masks[FRIENDLY_UNIT_BIT] = ENEMY_BULLET | ENEMY_BUILDING;
    gs->resources.collisions.masks[ENEMY_BUILDING_BIT] = FRIENDLY_UNIT | FRIENDLY_BULLET;
}

bool check_mask(GameState* gs, int mask1, int mask2) {
    for (int i = 0; i < 32; ++i) {
        if(!(mask1 & (1 << i))) continue;
        // if bit i of mask1 is set

        //
        if(gs->resources.collisions.masks[i] & mask2) {
            return true;
        }
    }
    return false;
}

void free_collision_data_children(CollisionData* cd) {
    if(cd->next != NULL) {
        free_collision_data_children(cd->next);
    }
    free(cd->next);
}

void push_collision(CollisionData* cd, int entity) {
    if(cd->entity == -1) {
        cd->entity = entity;
        assert(cd->next == NULL);
        return;
    }
    if(cd->next == NULL) {
        CollisionData* new = malloc(sizeof(CollisionData));
        new->next = NULL;
        new->entity = entity;
        cd->next = new;
    }
    else {
        push_collision(cd->next, entity);
    }
}

void draw_colliders(GameState *gs) {
    for (int i = 0; i < VEC_LEN(gs->entities); ++i) {
        bool alive = vec_bool_get(gs->entities, i);
        Collider c = vec_Collider_get(gs->collider_components, i);
        Transform t = vec_Transform_get(gs->transform_components, i);
        if(!alive || !c.exists || !t.exists) continue;

        Rect r = rect_local_to_global(c.rect, t);

        al_draw_filled_rectangle(RECT_COORDINATES(r), al_map_rgba_f(0, 0.1, 0.5, 0.1));
    }
}

void check_collisions(GameState* gs) {
    // begin by freeing previous collisions
    for (int i = 0; i < VEC_LEN(gs->resources.collisions.vec); ++i) {
        CollisionData* c_ptr = vec_CollisionData_get_ptr(gs->resources.collisions.vec, i);
        free_collision_data_children(c_ptr);
        c_ptr->next = NULL;
        c_ptr->entity = -1;
    }

    Vector_int collidables = vec_int_with_capacity(128);
    for (int i = 0; i < VEC_LEN(gs->entities); ++i) {
        Collider c = vec_Collider_get(gs->collider_components, i);
        if(c.exists) {
            vec_int_push(collidables, i);
        }
    }

    for (int i = 0; i < VEC_LEN(collidables); ++i) {
        int entity1 = vec_int_get(collidables, i);
        Collider c1 = vec_Collider_get(gs->collider_components, entity1);
        Rect rect1 = c1.rect;
        Transform t = vec_Transform_get(gs->transform_components, entity1);
        if(t.exists) rect1 = rect_local_to_global(rect1, t);

        for (int j = 0; j < VEC_LEN(collidables); ++j) {
            if(i >= j) continue;
            int entity2 = vec_int_get(collidables, j);

            Collider c2 = vec_Collider_get(gs->collider_components, entity2);
            Rect rect2 = c2.rect;
            Transform t2 = vec_Transform_get(gs->transform_components, entity2);
            if(t.exists) rect2 = rect_local_to_global(rect2, t2);

            if(check_mask(gs, c1.mask, c2.mask)) {
                Collision collision_result = rect_collide(rect1, rect2);

                if(collision_result == COLLISION) {
                    push_collision(vec_CollisionData_get_ptr(gs->resources.collisions.vec, entity1), entity2);
                    push_collision(vec_CollisionData_get_ptr(gs->resources.collisions.vec, entity2), entity1);
                }
            }


        }
    }

    vec_int_destroy(collidables);
}