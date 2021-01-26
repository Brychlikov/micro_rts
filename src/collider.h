//
// Created by brych on 20/01/2021.
//

#ifndef _RTS_COLLIDER_H
#define _RTS_COLLIDER_H

//#include <allegro5/allegro5.h>
#include "coord_utils.h"
#include "vector.h"
#include "gamestate_forward.h"

#define FRIENDLY_UNIT 1
#define ENEMY_UNIT 2
#define FRIENDLY_BUILDING 4
#define ENEMY_BUILDING 8
#define FRIENDLY_BULLET 16
#define ENEMY_BULLET 32

#define FRIENDLY_UNIT_BIT 0
#define ENEMY_UNIT_BIT 1
#define FRIENDLY_BUILDING_BIT 2
#define ENEMY_BUILDING_BIT 3
#define FRIENDLY_BULLET_BIT 4
#define ENEMY_BULLET_BIT 5

typedef struct {
    bool exists;
    Rect rect;      // this should be in local space??
    int mask;
    int entity;
} Collider;

typedef struct CollisionData {
    int entity;
    struct CollisionData* next;
} CollisionData;

GENERATE_VECTOR_DECLARATION(Collider)
GENERATE_VECTOR_DECLARATION(CollisionData)

void init_colliders(GameState* gs);

void check_collisions(GameState* gs);

bool check_mask(GameState* gs, int mask1, int mask2);

#endif //_RTS_COLLIDER_H
