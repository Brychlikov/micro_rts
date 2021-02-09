//
// Created by brych on 16/01/2021.
//

// This module contains an event system updating mouse state in gs->resources.mouse
// Mouse buttons are handled analogously to keyboard.h

#ifndef _RTS_MOUSE_H
#define _RTS_MOUSE_H

// 8 possible buttons in allegro, no idea what numbers >3 do
#define MOUSE_KEY_MAX 8

#define LEFT_MOUSE_BUTTON 1
#define RIGHT_MOUSE_BUTTON 2
#define MIDDLE_MOUSE_BUTTON 3

#define MOUSE_KEY_UNPROCESSED 1
#define MOUSE_KEY_HELD 2

void init_mouse(GameState* );
void update_mouse (GameState * , ALLEGRO_EVENT );
void reset_mouse_buttons(GameState*);

#endif //_RTS_MOUSE_H
