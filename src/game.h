//
// Created by brych on 09/02/2021.
//

// Collection of miscellaneous systems connected to gameplay. Contains board initialization and income processing.

#ifndef MICRO_RTS_GAME_H
#define MICRO_RTS_GAME_H
#include "gamestate_forward.h"

void init_game(GameState* gs);

void deinit_game(GameState* gs);

void update_income_values(GameState* gs);

void process_income(GameState* gs);

void render_balance(GameState* gs);

#include "gamestate.h"

#endif //MICRO_RTS_GAME_H
