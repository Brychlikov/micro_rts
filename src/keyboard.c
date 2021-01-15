//
// Created by brych on 14/01/2021.
//

#include "keyboard.h"
#include <allegro5/allegro5.h>


void init_keyboard(GameState* gs) {

    al_install_keyboard();
    memset(gs->keys, 0, sizeof (gs->keys));
    al_register_event_source(gs->queue, al_get_keyboard_event_source());

}

void reset_keys(GameState* gs) {
    for (int i = 0; i < sizeof(gs->keys); ++i) {
        gs->keys[i] &= KEY_SEEN;
    }
}

void update_keys(GameState* gs, ALLEGRO_EVENT event) {
    switch (event.type) {
        case ALLEGRO_EVENT_KEY_DOWN:
            gs->keys[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
            break;
        case ALLEGRO_EVENT_KEY_UP:
            gs->keys[event.keyboard.keycode] &= KEY_RELEASED;
            break;
    }
}

