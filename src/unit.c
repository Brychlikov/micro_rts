//
// Created by brych on 16/01/2021.
//

#include "unit.h"
#define UNIT_SPEED 5

struct ALLEGRO_BITMAP* unit_sprite;

void init_units(GameState* gs) {
    printf("Size of UnitEntry: %ld\n", sizeof (struct UnitEntry));
    gs->unit_entries = vec_new(sizeof(struct UnitEntry));
    gs->units_selected_indices = vec_new(sizeof(size_t));
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
    vec_push(gs->unit_entries, &new_entry);
}

void draw_units(GameState *gs) {
    for (int i = 0; i < gs->unit_entries->length; ++i) {
        struct UnitEntry u_e = *(struct UnitEntry*) vec_get(gs->unit_entries, i);
        if(u_e.exists) {
            al_draw_bitmap(unit_sprite, u_e.unit.position.x, u_e.unit.position.y, 0);
        }
    }

    //draw over selected units with tinted bitmap
    for (int i = 0; i < gs->units_selected_indices->length; ++i) {
        size_t index = *(size_t*) vec_get(gs->units_selected_indices, i);
        struct UnitEntry u_e = *(struct UnitEntry*) vec_get(gs->unit_entries, index);
        if(u_e.exists) {
            al_draw_tinted_bitmap(unit_sprite, al_map_rgb(0, 100, 0), u_e.unit.position.x, u_e.unit.position.y, 0);
        }
    }
}


void advance_units(GameState *gs) {
    for (int i = 0; i < gs->unit_entries->length; ++i) {

        struct UnitEntry* u_e = (struct UnitEntry*) vec_get(gs->unit_entries, i);
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
        for (int i = 0; i < gs->units_selected_indices->length; ++i) {
            printf("command issued to unit %d\n", i);
            size_t index = *(size_t*)vec_get(gs->units_selected_indices, i);
            UnitEntry* unit_entry = vec_get(gs->unit_entries, index);
            if(unit_entry->exists) {
                unit_entry->unit.destination = gs->mouse_position;
            }
        }
    }
}
