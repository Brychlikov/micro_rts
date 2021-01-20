//
// Created by brych on 20/01/2021.
//

#ifndef _RTS_COLLIDER_H
#define _RTS_COLLIDER_H

#include "coord_utils.h"
#include "vector.h"

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

#endif //_RTS_COLLIDER_H
