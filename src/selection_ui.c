//
// Created by brych on 15/01/2021.
//

#include "gamestate.h"
#include "coord_utils.h"
#include "keyboard.h"
#include <stdio.h>
#include <stdbool.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro5.h>
#include "selection_ui.h"
#include "unit.h"
#include "mouse.h"


void draw_selection_area(GameState* gs) {
    Rect sa = gs->resources.selection.area;
    if(gs->resources.selection.in_progress)
        al_draw_filled_rectangle(RECT_COORDINATES(sa), al_map_rgba_f(0, 1, 0.4, 0.2));
}

void selection_system(GameState* gs) {
    if(gs->resources.mouse_buttons[LEFT_MOUSE_BUTTON] & MOUSE_KEY_UNPROCESSED) {
        gs->resources.selection.in_progress = true;
        printf("beginning selection\n");
        gs->resources.selection.area.tl = gs->resources.mouse_position;
    }

    // if selection in progress, and LMB no longer being held
    if(gs->resources.selection.in_progress && !(gs->resources.mouse_buttons[LEFT_MOUSE_BUTTON] & MOUSE_KEY_HELD)) {
        gs->resources.selection.in_progress = false;
        gs->resources.selection.area.br = gs->resources.mouse_position;

        vec_int_clear(gs->units_selected_indices);

        printf("querying for collisions\n");
        for (size_t i = 0; i < VEC_LEN(gs->unit_entries); ++i) {
            UnitEntry* unit_entry = vec_UnitEntry_get_ptr(gs->unit_entries, i);
            if(unit_entry->exists) {
                Collision collision_result = rect_collide(unit_entry->unit.collider, gs->resources.selection.area);
                switch (collision_result) {
                    case COLLISION:
                        vec_int_push(gs->units_selected_indices, i);
                        break;
                    default:
                        break;
                }
            }

        }
        for(int i=0; i < VEC_LEN(gs->units_selected_indices); ++i) {
            size_t index = vec_int_get(gs->units_selected_indices, i);
            printf("Unit selected: %ld\n", index);
        }
    }

    if(gs->resources.selection.in_progress) gs->resources.selection.area.br = gs->resources.mouse_position;
}