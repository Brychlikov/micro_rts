//
// Created by brych on 24/01/2021.
//

#include "building.h"
#include "gamestate.h"
#include "sprite.h"
#include "collider.h"
#include "health.h"
#include "coord_utils.h"

GENERATE_VECTOR_DEFINITION(BuildingComponent)



int create_building(GameState* gs, Vec2 position, float scale, int max_hp, int team) {
    int new = create_entity(gs);

    Transform t = {
            .exists=true,
            .position=position,
            .rotation=0,
            .scale=scale,
            .entity=new,
    };

    *vec_Transform_get_ptr(gs->transform_components, new) = t;

    Sprite s = {
            .exists=true,
            .bitmap=gs->resources.sprites.building,
            .offset={.x=50, .y=50},
            .rotation=0,
            .tint=pure_tint,
            .entity=new,
    };

    *vec_Sprite_get_ptr(gs->sprite_components, new) = s;

    Health h = {
            .exists=true,
            .points=max_hp,
            .max_points=max_hp,
            .team=team,
            .entity=new,
    };

    *vec_Health_get_ptr(gs->health_components, new) = h;

    int mask = team == PLAYER_TEAM ? FRIENDLY_BUILDING : ENEMY_BUILDING;
    Collider c = {
            .exists=true,
            .rect={
                    .tl={.x=-45, .y=-45},
                    .br={.x=45, .y=45},
            },
            .mask=mask,
            .entity=new,
    };

    *vec_Collider_get_ptr(gs->collider_components, new) = c;

    BuildingComponent bc = {
            .exists=true,
            .under_attack=false,
            .time_from_attack=0,
            .last_attack_hp=h.points,
            .entity=new,
    };
    *vec_BuildingComponent_get_ptr(gs->building_components, new) = bc;

    return new;
}

void deinit_buildings(GameState *gs) {
    vec_BuildingComponent_destroy(gs->building_components);
}

void init_buildings(GameState *gs) {
    gs->building_components = vec_BuildingComponent_new();
}

void process_buildings(GameState *gs) {
    for (unsigned int entity = 0; entity < VEC_LEN(gs->entities); ++entity) {
        bool alive = vec_bool_get(gs->entities, entity);
        BuildingComponent* bc = vec_BuildingComponent_get_ptr(gs->building_components, entity);

        if(alive && bc->exists) {
            if(bc->under_attack)
                bc->time_from_attack += gs->resources.time_delta;

            Health h = vec_Health_get(gs->health_components, entity);
            if (h.points < bc->last_attack_hp) {
                bc->under_attack = true;
                bc->time_from_attack = 0;
                bc->last_attack_hp = h.points;
            }

            if(bc->time_from_attack > 5) {
                bc->under_attack = false;
            }
        }
    }
}
