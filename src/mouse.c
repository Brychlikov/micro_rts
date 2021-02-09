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
    switch(event.type) {
        case ALLEGRO_EVENT_MOUSE_AXES:
            gs->resources.mouse_position.x = (float)event.mouse.x;
            gs->resources.mouse_position.y = (float)event.mouse.y;
            break;
        case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
            gs->resources.mouse_buttons[event.mouse.button] = MOUSE_KEY_UNPROCESSED | MOUSE_KEY_HELD;
            break;
        case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
            gs->resources.mouse_buttons[event.mouse.button] &= ~MOUSE_KEY_HELD;
    }
}
void reset_mouse_buttons(GameState* gs) {
    for (unsigned int i = 0; i < sizeof(gs->resources.mouse_buttons); ++i) {
        gs->resources.mouse_buttons[i] &= ~MOUSE_KEY_UNPROCESSED;
    }
}
