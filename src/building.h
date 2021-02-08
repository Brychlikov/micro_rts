//
// Created by brych on 24/01/2021.
//

#ifndef _RTS_BUILDING_H
#define _RTS_BUILDING_H

#include "gamestate_forward.h"
#include "coord_utils.h"
#include "vector.h"

typedef struct {
    bool exists;
    bool under_attack;
    float time_from_attack;
    int last_attack_hp;
    int entity;
} BuildingComponent;

GENERATE_VECTOR_DECLARATION(BuildingComponent)

int create_building(GameState*, Vec2 position, float scale, int max_hp, int team);

void init_buildings(GameState* gs);

void process_buildings(GameState* gs);

void deinit_buildings(GameState* gs);

#endif //_RTS_BUILDING_H
