//
// Created by brych on 31/01/2021.
//

#include "laser_turret.h"
#include "gamestate.h"
#include "coord_utils.h"
#include "collider.h"
#include "health.h"
#include "sprite.h"

#include "allegro5/allegro_primitives.h"

// kinda ugly to include this, but i need the unit targeting logic
#include "unit.h"

GENERATE_VECTOR_DEFINITION(LaserTurretComponent)

#define LASER_TURRET_HP 500
#define LASER_TURRET_ATTACK_RANGE 500
#define LASER_TURRET_ATTACK_COOLDOWN 2.0
#define LASER_TURRET_DAMAGE 50

void init_laser_turrets(GameState* gs) {
    gs->laser_turret_components = vec_LaserTurretComponent_new();
}

int create_laser_turret(GameState* gs, Vec2 position, int team) {
    int new = create_entity(gs);

    Transform t = {
            .exists=true,
            .position=position,
            .rotation=0,
            .scale=1,
            .entity=new,
    };

    *vec_Transform_get_ptr(gs->transform_components, new) = t;

    Sprite s = {
            .exists=true,
            .bitmap=gs->resources.sprites.laser_turret,
            .offset={.x=50, .y=50},
            .rotation=0,
            .tint=pure_tint,
            .entity=new,
    };

    *vec_Sprite_get_ptr(gs->sprite_components, new) = s;

    Health h = {
            .exists=true,
            .points=LASER_TURRET_HP,
            .max_points=LASER_TURRET_HP,
            .team=team,
            .entity=new,
    };

    *vec_Health_get_ptr(gs->health_components, new) = h;

    int mask = team == PLAYER_TEAM ? FRIENDLY_BUILDING : ENEMY_BUILDING;
    Collider c = {
            .exists=true,
            .rect={
                    .tl={.x=-50, .y=-50},
                    .br={.x=50, .y=50},
            },
            .mask=mask,
            .entity=new,
    };

    *vec_Collider_get_ptr(gs->collider_components, new) = c;

    LaserTurretComponent ltc = {
        .exists=true,
        .shot_cooldown=0,
        .entity=new,
    };

    *vec_LaserTurretComponent_get_ptr(gs->laser_turret_components, new) = ltc;

    return new;
}

void process_laser_turrets(GameState *gs) {
    Vector_int targetables = vec_int_with_capacity(128);
    for (int i = 0; i < VEC_LEN(gs->entities); ++i) {
        Transform t = vec_Transform_get(gs->transform_components, i);
        if(!t.exists) continue;

        Health h = vec_Health_get(gs->health_components, i);
        if(!h.exists) continue;

        vec_int_push(targetables, i);
    }

    for (int entity = 0; entity < VEC_LEN(gs->entities); entity++) {
        if(!vec_bool_get(gs->entities, entity)) continue;

        LaserTurretComponent* ltc = vec_LaserTurretComponent_get_ptr(gs->laser_turret_components, entity);
        if(!ltc->exists) continue;
        Health turret_health = vec_Health_get(gs->health_components, entity);
        Transform turret_transform = vec_Transform_get(gs->transform_components, entity);

        ltc->shot_cooldown -= gs->resources.time_delta;
        ltc->shot_cooldown = ltc->shot_cooldown < 0 ? 0 : ltc->shot_cooldown;

        int target = target_unit(gs, entity, targetables, LASER_TURRET_ATTACK_RANGE);

        if(target != -1 && ltc->shot_cooldown == 0) {
            Transform target_transform = vec_Transform_get(gs->transform_components, target);
            ltc->target_position = target_transform.position;

            Health* target_health = vec_Health_get_ptr(gs->health_components, target);
            target_health->points -= LASER_TURRET_DAMAGE;
            ltc->shot_cooldown = LASER_TURRET_ATTACK_COOLDOWN;
        }

        if(ltc->shot_cooldown > LASER_TURRET_ATTACK_COOLDOWN / 2) {
            float opacity = ltc->shot_cooldown - LASER_TURRET_ATTACK_COOLDOWN / 2;
//            printf("drawing opacity: %f\n", opacity);
            al_draw_line(turret_transform.position.x,
                         turret_transform.position.y,
                         ltc->target_position.x,
                         ltc->target_position.y,
                         al_map_rgba_f(0.8, 0.2, 0.2, opacity),
                         10);

        }
    }
    vec_int_destroy(targetables);
}

void deinit_laser_turrets(GameState *gs) {
    vec_LaserTurretComponent_destroy(gs->laser_turret_components);
}
