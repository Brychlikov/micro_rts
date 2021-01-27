//
// Created by brych on 26/01/2021.
//

#include "bullet.h"
#include "gamestate.h"

GENERATE_VECTOR_DEFINITION(BulletComponent)

#define BULLET_DAMAGE 10

void init_bullets(GameState* gs) {
    gs->bullet_components = vec_BulletComponent_new();
}

void create_bullet(GameState* gs, Transform t, int team) {
    int new = create_entity(gs);
    t.entity = new;

    // override scale of creator
    t.scale = 0.4;

    *vec_Transform_get_ptr(gs->transform_components, new) = t;

    BulletComponent bc = {
            .exists=true,
            .team=team,
            .damage=BULLET_DAMAGE,
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
    if(gs->resources.mouse_buttons[MIDDLE_MOUSE_BUTTON] & MOUSE_KEY_UNPROCESSED) {
        Transform new_t = {
                .exists=true,
                .position=gs->resources.mouse_position,
                .rotation=0,
                .scale=1,
        };
//        printf("making a bullet\n");
        create_bullet(gs, new_t, PLAYER_TEAM);
    }
    for (int i = 0; i < VEC_LEN(gs->entities); ++i) {
        BulletComponent bc = vec_BulletComponent_get(gs->bullet_components, i);
        bool alive = vec_bool_get(gs->entities, i);
        if(!alive || !bc.exists) continue;

        //movement
        Transform* t = vec_Transform_get_ptr(gs->transform_components, i);
        Vec2 delta = vec2_scale(vec2_unit_from_radius(t->rotation), BULLET_SPEED);
        t->position = vec2_add(t->position, delta);

        // delete bullet if outside of playable zone
        if(t->position.x < -100  || t->position.y < -100 || t->position.x > 2000 || t->position.y > 2000) {
            destroy_entity(gs, i);
            return;
        }

        CollisionData* cd = vec_CollisionData_get_ptr(gs->resources.collisions.vec, i);
        while(cd != NULL && cd->entity != -1) {
            Health* h = vec_Health_get_ptr(gs->health_components, cd->entity);

            if(h->exists && h->team != bc.team) {
                // we have a valid target
                h->points -= bc.damage;
//                printf("damage dealt. hp remaining: %d", h->points);
                destroy_entity(gs, i);
                return;
            }
            if(h->team == bc.team) {
                printf("Something went wrong: recorded collision between entities of the same team\n");
            }
            cd = cd->next;
        }
    }
}
