//
// Created by brych on 16/01/2021.
//

#include "gamestate.h"
#include <allegro5/allegro5.h>
#include "mouse.h"

void init_mouse(GameState* gs) {
    al_install_mouse();
    al_register_event_source(gs->queue, al_get_mouse_event_source());
}

void update_mouse (GameState * gs, ALLEGRO_EVENT event) {
    if(event.type == ALLEGRO_EVENT_MOUSE_AXES) {
        gs->mouse_position.x = (float)event.mouse.x;
        gs->mouse_position.y = (float)event.mouse.y;
    }
}