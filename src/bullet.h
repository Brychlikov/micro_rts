//
// Created by brych on 26/01/2021.
//

#ifndef _RTS_BULLET_H
#define _RTS_BULLET_H

#include "gamestate_forward.h"
#include "coord_utils.h"
#include "collider.h"
#include "vector.h"
#include "health.h"
#include "sprite.h"

void init_bullets(GameState* gs);

void deinit_bullets(GameState* gs);

void create_bullet(GameState* gs, Transform t, int team);

void process_bullets(GameState* gs);

typedef struct {
    bool exists;
    int team;
    int damage;
    float time_remaining;
    int entity;
} BulletComponent;

GENERATE_VECTOR_DECLARATION(BulletComponent)

#endif //_RTS_BULLET_H
