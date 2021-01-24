//
// Created by brych on 20/01/2021.
//

#include <math.h>
#include "sprite.h"
#include "gamestate.h"
#include "unit.h"
#include "collider.h"

GENERATE_VECTOR_DEFINITION(Collider)
GENERATE_VECTOR_DEFINITION(CollisionData)

void init_colliders(GameState* gs) {
    gs->resources.collisions.vec = vec_CollisionData_new();
    gs->collider_components = vec_Collider_new();
}