//
// Created by brych on 24/01/2021.
//

#include "building.h"
#include "gamestate.h"
#include "sprite.h"
#include "collider.h"
#include "health.h"
#include "coord_utils.h"



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
    return new;
}