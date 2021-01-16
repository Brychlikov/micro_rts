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

#define LEFT_MOUSE_BUTTON 1
#define RIGHT_MOUSE_BUTTON 2

void draw_selection_area(GameState* gs) {
    Rect sa = gs->selection_area;
    if(gs->during_selection)
        al_draw_filled_rectangle(RECT_COORDINATES(sa), al_map_rgba_f(0, 1, 0.4, 0.2));
}

void selection_system(GameState* gs, ALLEGRO_EVENT event) {
    if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && event.mouse.button == LEFT_MOUSE_BUTTON) {
        gs->during_selection = true;
        gs->selection_area.tl = gs->mouse_position;
    }

    if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP && event.mouse.button == LEFT_MOUSE_BUTTON) {
        gs->during_selection = false;
        gs->selection_area.br = gs->mouse_position;

        vec_clear(&gs->units_selected_indices);

        printf("querying for collisions\n");
        for (size_t i = 0; i < gs->unit_entries.length; ++i) {
            UnitEntry* unit_entry = vec_get(&gs->unit_entries, i);
            if(unit_entry->exists) {
                Collision collision_result = rect_collide(unit_entry->unit.collider, gs->selection_area);
                switch (collision_result) {
                    case COLLISION:
                        vec_push(&gs->units_selected_indices, &i);
                        break;
                    default:
                        break;
                }
            }

        }
        for(int i=0; i < gs->units_selected_indices.length; ++i) {
            size_t index = *(size_t*)vec_get(&gs->units_selected_indices, i);
            printf("Unit selected: %ld\n", index);
        }
    }

    if(gs->during_selection) gs->selection_area.br = gs->mouse_position;
}