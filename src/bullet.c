//
// Created by brych on 26/01/2021.
//

#include "bullet.h"
#include "gamestate.h"

GENERATE_VECTOR_DEFINITION(BulletComponent)

#define BULLET_DAMAGE 10
#define BULLET_SPEED 200
#define BULLET_LIFE 2.5

void init_bullets(GameState* gs) {
    gs->bullet_components = vec_BulletComponent_new();
}

void deinit_bullets(GameState *gs) {
    vec_BulletComponent_destroy(gs->bullet_components);
}

void create_bullet(GameState* gs, Transform t, int team, int damage) {
    int new = create_entity(gs);
    t.entity = new;

    *vec_Transform_get_ptr(gs->transform_components, new) = t;

    BulletComponent bc = {
            .exists=true,
            .team=team,
            .damage=damage,
            .time_remaining=BULLET_LIFE,
            .entity=new,
    };

    *vec_BulletComponent_get_ptr(gs->bullet_components, new) = bc;

    int collision_mask = team == PLAYER_TEAM ? FRIENDLY_BULLET : ENEMY_BULLET;

    Collider c = {
            .exists=true,
            .mask=collision_mask,
            .rect={
                    .tl={.x=-30, .y=-15},
                    .br={.x=30, .y=15}
            },
            .entity=new,
    };

    *vec_Collider_get_ptr(gs->collider_components, new) = c;

    Sprite s = {
            .exists=true,
            .bitmap=gs->resources.sprites.bullet,
            .offset={.x=30, .y=15},
            .tint=pure_tint,
            .entity=new,
    };

    *vec_Sprite_get_ptr(gs->sprite_components, new) = s;
}

void process_bullets(GameState* gs) {
    for (int i = 0; i < VEC_LEN(gs->entities); ++i) {
        BulletComponent bc = vec_BulletComponent_get(gs->bullet_components, i);
        bool alive = vec_bool_get(gs->entities, i);
        if(!alive || !bc.exists) continue;

        //movement
        Transform* t = vec_Transform_get_ptr(gs->transform_components, i);
        Vec2 delta = vec2_scale(vec2_unit_from_radius(t->rotation), BULLET_SPEED * gs->resources.time_delta);
        t->position = vec2_add(t->position, delta);

        //update bullet lifetime
        BulletComponent *bc_ptr = vec_BulletComponent_get_ptr(gs->bullet_components, i);
        bc_ptr->time_remaining -= gs->resources.time_delta;
        if(bc_ptr->time_remaining < 0) {
            destroy_entity(gs, i);
            continue;
        }

//        // delete bullet if outside of playable zone
//        if(t->position.x < -100  || t->position.y < -100 || t->position.x > 2000 || t->position.y > 2000) {
//            destroy_entity(gs, i);
//            continue;
//        }

        CollisionData* cd = vec_CollisionData_get_ptr(gs->resources.collisions.vec, i);
        while(cd != NULL && cd->entity != -1) {
            Health* h = vec_Health_get_ptr(gs->health_components, cd->entity);

            if(h->exists && h->team != bc.team) {
                // we have a valid target
                h->points -= bc.damage;
//                printf("damage dealt. hp remaining: %d", h->points);
                destroy_entity(gs, i);
                break;
            }
            if(h->team == bc.team) {
                printf("Something went wrong: recorded collision between entities of the same team\n");
            }
            cd = cd->next;
        }
    }
}
