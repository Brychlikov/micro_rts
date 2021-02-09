//
// Created by brych on 27/01/2021.
//

// Enemy AI. Enemy gathers as much units as possible and:
//   - if one of their buildings is being attacked, sends idle units to help
//   - otherwise occasionally send units to attack a random player base

#ifndef _RTS_ENEMY_H
#define _RTS_ENEMY_H
#include "gamestate_forward.h"

void init_enemy(GameState* gs);

void deinit_enemy(GameState* gs);

void process_enemy(GameState* gs);

#endif //_RTS_ENEMY_H
