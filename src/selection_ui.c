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

void draw_selection_area(GameState* gs) {
    Rect sa = gs->selection_area;
    al_draw_filled_rectangle(RECT_COORDINATES(sa), al_map_rgba_f(0, 1, 0.4, 0.2));
}

void selection_system(GameState* gs, ALLEGRO_EVENT event) {
    if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
        gs->during_selection = true;
        gs->selection_area.tl = gs->mouse_position;
    }

    if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
        gs->during_selection = false;
        gs->selection_area.br = gs->mouse_position;
    }

    if(gs->during_selection) gs->selection_area.br = gs->mouse_position;
}