//
// Created by brych on 24/01/2021.
//

#ifndef _RTS_HEALTH_H
#define _RTS_HEALTH_H

#include "vector.h"
#include "gamestate_forward.h"

#define PLAYER_TEAM 0
#define ENEMY_TEAM 1

typedef struct {
    bool exists;
    int points;
    int team;
    int entity;
} Health;

GENERATE_VECTOR_DECLARATION(Health)

void init_health(GameState* gs);

void deinit_health(GameState* gs);

void health_system(GameState* gs);

#endif //_RTS_HEALTH_H
