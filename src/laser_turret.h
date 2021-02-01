//
// Created by brych on 31/01/2021.
//

#ifndef _RTS_LASER_TURRET_H
#define _RTS_LASER_TURRET_H

#include "vector.h"
#include "gamestate_forward.h"
#include "coord_utils.h"

typedef struct {
    bool exists;
    float shot_cooldown;
    Vec2 target_position;
    int entity;
} LaserTurretComponent;

GENERATE_VECTOR_DECLARATION(LaserTurretComponent)

void init_laser_turrets(GameState* gs);

int create_laser_turret(GameState* gs, Vec2 position, int team);

void process_laser_turrets(GameState* gs);

void deinit_laser_turrets(GameState* gs);


#endif //_RTS_LASER_TURRET_H
