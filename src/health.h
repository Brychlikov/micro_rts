//
// Created by brych on 24/01/2021.
//

#ifndef _RTS_HEALTH_H
#define _RTS_HEALTH_H

#include "vector.h"
#include "gamestate_forward.h"

typedef struct {
    bool exists;
    int points;
    int team;
    int entity;
} Health;

GENERATE_VECTOR_DECLARATION(Health)

void init_health(GameState* gs);

void health_system(GameState* gs);

#endif //_RTS_HEALTH_H