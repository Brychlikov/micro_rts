//
// Created by brych on 16/01/2021.
//

#include "unit.h"
#include "gamestate.h"
#define UNIT_SPEED 5

GENERATE_VECTOR_DEFINITION(UnitEntry)
GENERATE_VECTOR_DEFINITION(Sprite)

struct ALLEGRO_BITMAP* unit_sprite;


void init_units(GameState* gs) {
    printf("Size of UnitEntry: %ld\n", sizeof (struct UnitEntry));
    gs->unit_entries = vec_UnitEntry_new();
    gs->units_selected_indices = vec_int_new();
    unit_sprite = al_load_bitmap("assets/unit.png");
    if (unit_sprite == NULL) {
        fprintf(stderr, "Could not load unit sprite");
    }
    Unit new_unit = {
            .position= {.x=200, .y=400},
            .collider= {
                .tl= {
                    .x=200, .y=400,
                },
                .br={
                    .x=280, .y=480
            }},
            .destination={.x=900, .y=100}
    };
    struct UnitEntry new_entry = {
            .exists=true,
            .unit=new_unit
    };
    vec_UnitEntry_push(gs->unit_entries, new_entry);


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
            .bitmap=unit_sprite,
            .entity=new,
    };
    *vec_Sprite_get_ptr(gs->sprite_components, new) = s;
}

void draw_units(GameState *gs) {
    for (int i = 0; i < gs->unit_entries.inner->length; ++i) {
        struct UnitEntry u_e = vec_UnitEntry_get(gs->unit_entries, i);
        if(u_e.exists) {
            al_draw_bitmap(unit_sprite, u_e.unit.position.x, u_e.unit.position.y, 0);
        }
    }

    //draw over selected units with tinted bitmap
    for (int i = 0; i < gs->units_selected_indices.inner->length; ++i) {
        int index = vec_int_get(gs->units_selected_indices, i);
        struct UnitEntry u_e = vec_UnitEntry_get(gs->unit_entries, index);
        if(u_e.exists) {
            al_draw_tinted_bitmap(unit_sprite, al_map_rgb(0, 100, 0), u_e.unit.position.x, u_e.unit.position.y, 0);
        }
    }
}


void advance_units(GameState *gs) {
    for (int i = 0; i < gs->unit_entries.inner->length; ++i) {

        struct UnitEntry* u_e = vec_UnitEntry_get_ptr(gs->unit_entries, i);
        if(u_e->exists) {

            Vec2 diff = vec2_sub(u_e->unit.destination, u_e->unit.position);
            if(vec2_length(diff) <= UNIT_SPEED) {
                u_e->unit.position = u_e->unit.destination;
            }
            else {
                Vec2 dir = vec2_norm(diff);
                Vec2 delta = vec2_scale(dir, UNIT_SPEED);
//                printf("len: %f %f\n", vec2_length(dir), vec2_length(delta));
//                printf("Delta: x=%f  y=%f\n", delta.x, delta.y);
                u_e->unit.position = vec2_add(u_e->unit.position, delta);
                u_e->unit.collider.tl = vec2_add(u_e->unit.collider.tl, delta);
                u_e->unit.collider.br = vec2_add(u_e->unit.collider.br, delta);
//                printf("New pos: x=%f  y=%f\n\n", u_e->unit.position.x, u_e->unit.position.y);
            }

        }

    }
}

void command_units(GameState *gs, ALLEGRO_EVENT event) {
    if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP && event.mouse.button == 2) {
        printf("giving commands\n");
        for (int i = 0; i < VEC_LEN(gs->units_selected_indices); ++i) {
            printf("command issued to unit %d\n", i);
            int index = vec_int_get(gs->units_selected_indices, i);
            UnitEntry* unit_entry = vec_UnitEntry_get_ptr(gs->unit_entries, index);
            if(unit_entry->exists) {
                unit_entry->unit.destination = gs->resources.mouse_position;
            }
        }
    }
}

void draw_sprites(GameState* gs) {
    for (int i = 0; i < VEC_LEN(gs->sprite_components); ++i) {
        // for now assert that every entity with sprite has a valid transform
        Sprite s = vec_Sprite_get(gs->sprite_components, i);
        if(vec_bool_get(gs->entities, i)) {  // if entity still alive
            Transform t = vec_Transform_get(gs->transform_components, i);
            al_draw_rotated_bitmap(s.bitmap, s.offset.x, s.offset.y, t.position.x, t.position.y, t.rotation, 0);    // disregard scale for now
        }
    }
}
