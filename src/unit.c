//
// Created by brych on 16/01/2021.
//

#include "unit.h"
#include "gamestate.h"
#include "sprite.h"
#include "collider.h"
#include <math.h>
#define UNIT_SPEED 5
#define PI 3.14159265

GENERATE_VECTOR_DEFINITION(UnitComponent)

struct ALLEGRO_BITMAP* unit_sprite;
ALLEGRO_COLOR pure_tint;
ALLEGRO_COLOR selected_tint;

void init_colliders(GameState* gs) {
    gs->resources.collisions.vec = vec_CollisionData_new();
    gs->collider_components = vec_Collider_new();
}

void init_units(GameState* gs) {
    unit_sprite = al_load_bitmap("assets/unit.png");
    if (unit_sprite == NULL) {
        fprintf(stderr, "Could not load unit sprite");
    }

    pure_tint = al_map_rgba_f(1, 1, 1, 1);
    selected_tint = al_map_rgba_f(0.5, 0.5, 1, 1);

    // ECS land below
    int new = create_entity(gs);
    Transform t = {
            .exists=true,
            .position={.x=100, .y=100},
            .rotation=1,
            .entity=new
    };
    *vec_Transform_get_ptr(gs->transform_components, new) = t;
    Sprite s = {
            .exists=true,
            .offset={.x=40, .y=40},
            .rotation=PI/2,
            .bitmap=unit_sprite,
            .entity=new,
            .tint=pure_tint,
    };
    *vec_Sprite_get_ptr(gs->sprite_components, new) = s;

    Collider c = {
            .exists=true,
            .rect={
                    .tl={.x=-40, .y=-40},
                    .br={.x=40, .y=40},
            },
            .entity=new
    };

    *vec_Collider_get_ptr(gs->collider_components, new) = c;

    UnitComponent uc = {
            .exists=true,
            .sm={
                    .state=MOVE,
                    .move= {
                            .dest={.x=800, .y=600}
                    }
            },
            .entity=new,
    };

    *vec_UnitComponent_get_ptr(gs->unit_components, new) = uc;
}

//void draw_units(GameState *gs) {
//    for (int i = 0; i < gs->unit_entries.inner->length; ++i) {
//        struct UnitEntry u_e = vec_UnitEntry_get(gs->unit_entries, i);
//        if(u_e.exists) {
//            al_draw_bitmap(unit_sprite, u_e.unit.position.x, u_e.unit.position.y, 0);
//        }
//    }
//
//    //draw over selected units with tinted bitmap
//    for (int i = 0; i < gs->units_selected_indices.inner->length; ++i) {
//        int index = vec_int_get(gs->units_selected_indices, i);
//        struct UnitEntry u_e = vec_UnitEntry_get(gs->unit_entries, index);
//        if(u_e.exists) {
//            al_draw_tinted_bitmap(unit_sprite, al_map_rgb(0, 100, 0), u_e.unit.position.x, u_e.unit.position.y, 0);
//        }
//    }
//}


void advance_units(GameState *gs) {
    for (int i = 0; i < VEC_LEN(gs->entities); ++i) {
        if(!vec_bool_get(gs->entities, i)) continue;

        UnitComponent unit = vec_UnitComponent_get(gs->unit_components, i);
        if(!unit.exists) continue;

        Transform* t = vec_Transform_get_ptr(gs->transform_components, i);

        switch(unit.sm.state) {
            case MOVE:
                ;     // I hate this language sooooo much
                Vec2 diff = vec2_sub(unit.sm.move.dest, t->position);
                Vec2 delta;
                if(vec2_length(diff) <= UNIT_SPEED) {
                    delta = vec2_sub(unit.sm.move.dest, t->position);
                }
                else {
                    Vec2 dir = vec2_norm(diff);
                    delta = vec2_scale(dir, UNIT_SPEED);
                }
                float angle = atan2f(delta.y, delta.x);
                t->position = vec2_add(t->position, delta);
                t->rotation = angle;
                break;
            case IDLE:
                break;
            default:
                break;
        }


    }
}

void command_units(GameState *gs) {
    if(gs->resources.mouse_buttons[RIGHT_MOUSE_BUTTON]) {
//        printf("giving commands\n");
        for (int i = 0; i < VEC_LEN(gs->resources.selection.entities_selected); ++i) {
//            printf("command issued to unit %d\n", i);
            int entity = vec_int_get(gs->resources.selection.entities_selected, i);

            UnitComponent* ue = vec_UnitComponent_get_ptr(gs->unit_components, entity);
            if(!ue->exists) continue;

            ue->sm.state = MOVE;
            ue->sm.move.dest = gs->resources.mouse_position;
        }
    }
}

