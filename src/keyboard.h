//
// Created by brych on 14/01/2021.
//

// Module containing an event system updating key states in gs->resources.keys
// Allows users to check if a key is processed for the first time and if it's still being held

#ifndef _RTS_KEYBOARD_H
#define _RTS_KEYBOARD_H

#include <allegro5/allegro5.h>
#include "gamestate.h"

#define KEY_UNPROCESSED 1
#define KEY_HELD 2


void init_keyboard(GameState*);

void update_keys(GameState*, union ALLEGRO_EVENT);

void reset_keys(GameState*);

#endif //_RTS_KEYBOARD_H
