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
#include "collider.h"

void init_selection_system(GameState* gs) {
    gs->resources.selection.entities_selected = vec_int_new();
}

void draw_selection_area(GameState* gs) {
    Rect sa = gs->resources.selection.area;
    if(gs->resources.selection.in_progress)
        al_draw_filled_rectangle(RECT_COORDINATES(sa), al_map_rgba_f(0, 1, 0.4, 0.2));
}

void selection_system(GameState* gs) {
    if(gs->resources.overdrive.active) return;  // disable in overdrive mode
    if(gs->resources.mouse_buttons[LEFT_MOUSE_BUTTON] & MOUSE_KEY_UNPROCESSED) {
        gs->resources.selection.in_progress = true;
//        printf("beginning selection\n");
        gs->resources.selection.area.tl = gs->resources.mouse_position;
    }

    // if selection in progress, and LMB no longer being held
    if(gs->resources.selection.in_progress && !(gs->resources.mouse_buttons[LEFT_MOUSE_BUTTON] & MOUSE_KEY_HELD)) {
        gs->resources.selection.in_progress = false;
        gs->resources.selection.area.br = gs->resources.mouse_position;

        // clear tint of selected units
        for (int i = 0; i < VEC_LEN(gs->resources.selection.entities_selected); ++i) {
            int entity = vec_int_get(gs->resources.selection.entities_selected, i);
            Sprite* s = vec_Sprite_get_ptr(gs->sprite_components, entity);
            // assigning is safe, even to non-existent sprite components
            s->tint = pure_tint;
        }

        vec_int_clear(gs->resources.selection.entities_selected);

//        printf("querying for collisions\n");
        for (size_t i = 0; i < VEC_LEN(gs->entities); ++i) {
            if(!vec_bool_get(gs->entities, i)) continue;

            Collider c = vec_Collider_get(gs->collider_components, i);
            if(!c.exists) continue;

            // only entities having health and controlled by player should be selectable
            Health h = vec_Health_get(gs->health_components, i);
            if(!h.exists || h.team != PLAYER_TEAM) continue;

            // if entity has a transform, rect is in local coord space
            // and needs to be transformed to global space
            Rect rect = c.rect;
            Transform t = vec_Transform_get(gs->transform_components, i);
            if(t.exists) rect = rect_local_to_global(rect, t);


            // TODO check collision masks
            Collision collision_result = rect_collide(rect, gs->resources.selection.area);

            if(collision_result == COLLISION) {
                vec_int_push(gs->resources.selection.entities_selected, i);
                Sprite* s = vec_Sprite_get_ptr(gs->sprite_components, i);
                s->tint = selected_tint;
            }

        }
        for(int i=0; i < VEC_LEN(gs->resources.selection.entities_selected); ++i) {
            size_t index = vec_int_get(gs->resources.selection.entities_selected, i);
//            printf("Entity selected: %ld\n", index);
        }
    }

    if(gs->resources.selection.in_progress) gs->resources.selection.area.br = gs->resources.mouse_position;
}