//
// Created by brych on 26/01/2021.
//

// Logic of bullets. Bullets are entities that move in a given direction at a constant speed.
// If they collide with an entity having a HealthComponent from the opposing team,
// bullet is destroyed and entity is damaged.
// To limit the number of entities on the screen, bullets have limited lifetime.

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

void create_bullet(GameState* gs, Transform t, int team, int damage);

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
