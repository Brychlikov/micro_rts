//
// Created by brych on 24/01/2021.
//

#include "building.h"
#include "gamestate.h"
#include "sprite.h"
#include "collider.h"
#include "health.h"
#include "coord_utils.h"

#define BUILDING_MAX_HP 1000


int create_building(GameState* gs, Vec2 position, int team) {
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
            .bitmap=gs->resources.sprites.building,
            .offset={.x=50, .y=50},
            .rotation=0,
            .tint=pure_tint,
            .entity=new,
    };

    *vec_Sprite_get_ptr(gs->sprite_components, new) = s;

    Health h = {
            .exists=true,
            .points=BUILDING_MAX_HP,
            .team=team,
            .entity=new,
    };

    *vec_Health_get_ptr(gs->health_components, new) = h;

    Collider c = {
            .exists=true,
            .rect={
                    .tl={.x=-45, .y=-45},
                    .br={.x=45, .y=45},
            },
            .mask=ENEMY_BUILDING,
            .entity=new,
    };

    *vec_Collider_get_ptr(gs->collider_components, new) = c;
    return new;
}