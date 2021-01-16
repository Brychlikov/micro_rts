//
// Created by brych on 16/01/2021.
//

#include "unit.h"
#define UNIT_SPEED 5

struct ALLEGRO_BITMAP* unit_sprite;

void init_units(GameState* gs) {
    printf("Size of UnitEntry: %ld\n", sizeof (struct UnitEntry));
    gs->units = vec_new(sizeof(struct UnitEntry));
    unit_sprite = al_load_bitmap("assets/unit.png");
    if (unit_sprite == NULL) {
        fprintf(stderr, "Could not load unit sprite");
    }
    Unit new_unit = {
            .position= {.x=200, .y=400},
            .collider= {},
            .destination={.x=900, .y=100}
    };
    struct UnitEntry new_entry = {
            .exists=true,
            .unit=new_unit
    };
    vec_push(&gs->units, &new_entry);
}

void draw_units(GameState *gs) {
    for (int i = 0; i < gs->units.length; ++i) {
        struct UnitEntry u_e = *(struct UnitEntry*) vec_get(&gs->units, i);
        if(u_e.exists) {
            al_draw_bitmap(unit_sprite, u_e.unit.position.x, u_e.unit.position.y, 0);
        }
    }
}


void advance_units(GameState *gs) {
    for (int i = 0; i < gs->units.length; ++i) {

        struct UnitEntry* u_e = (struct UnitEntry*) vec_get(&gs->units, i);
        if(u_e->exists) {

            Vec2 diff = vec2_sub(vec2_from_point(u_e->unit.destination), vec2_from_point(u_e->unit.position));
            if(vec2_length(diff) <= UNIT_SPEED) {
                u_e->unit.position = u_e->unit.destination;
            }
            else {
                Vec2 dir = vec2_norm(diff);
                Vec2 delta = vec2_scale(dir, UNIT_SPEED);
                printf("len: %f %f\n", vec2_length(dir), vec2_length(delta));
                printf("Delta: x=%f  y=%f\n", delta.x, delta.y);
                u_e->unit.position = point_from_vec2(
                        vec2_add(vec2_from_point(u_e->unit.position), delta)
                );
                printf("New pos: x=%f  y=%f\n\n", u_e->unit.position.x, u_e->unit.position.y);
            }

        }

    }
}
