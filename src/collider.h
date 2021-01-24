//
// Created by brych on 20/01/2021.
//

#ifndef _RTS_COLLIDER_H
#define _RTS_COLLIDER_H

#include <allegro5/allegro5.h>
#include "coord_utils.h"
#include "vector.h"
#include "gamestate_forward.h"

typedef struct {
    bool exists;
    Rect rect;      // this should be in local space??
    int mask;      // ignored for now
    int entity;
} Collider;
typedef struct {
    int entity1;
    int entity2;
} CollisionData;

GENERATE_VECTOR_DECLARATION(Collider)
GENERATE_VECTOR_DECLARATION(CollisionData)

void init_colliders(GameState* gs);

#endif //_RTS_COLLIDER_H
