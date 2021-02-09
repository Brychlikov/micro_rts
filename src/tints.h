//
// Created by brych on 07/02/2021.
//

// This module changes the tint of sprites to distinguish enemy units from player units and overdriven units from those
// without overdrives

#ifndef _RTS_TINTS_H
#define _RTS_TINTS_H
#include "gamestate_forward.h"
#include "allegro5/allegro5.h"

extern ALLEGRO_COLOR selected_tint;
extern ALLEGRO_COLOR enemy_tint;
extern ALLEGRO_COLOR overdrive_tint;
extern ALLEGRO_COLOR selected_overdrive_tint;

void tint_sprites(GameState* gs);

void init_tints(GameState* gs);

ALLEGRO_COLOR color_code_to_allegro(const char* s);


#endif //_RTS_TINTS_H
