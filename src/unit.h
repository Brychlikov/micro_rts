//
// Created by brych on 16/01/2021.
//


#ifndef _RTS_UNIT_H
#define _RTS_UNIT_H

#include "coord_utils.h"
//#include "gamestate.h"
#include <allegro5/allegro5.h>
#include "vector.h"

extern ALLEGRO_BITMAP* unit_sprite;

typedef struct {
    bool exists;  // we want all components to have this as first field
    ALLEGRO_BITMAP* bitmap;
    Vec2 offset; // coordinates of center point in local space
    float rotation;
    ALLEGRO_COLOR tint;
    int entity;
} Sprite;

GENERATE_VECTOR_DECLARATION(Sprite)

struct Unit {
    Vec2  position;
    Rect collider;
    Vec2 destination;
} ;

typedef enum UnitState {
    IDLE,
    MOVE,
    A_MOVE,
    AGGRESSIVE,
    WATCH
} UnitState;

typedef struct UnitStateMachine {
    UnitState state;
    union {
        struct {
            Vec2 dest;
        } move;
        struct {
            Vec2 dest;
        } a_move;
        struct {
            int target;
            UnitState next;
        } aggressive;
    };
} UnitStateMachine;

typedef struct UnitComponent {
    bool exists;
    UnitStateMachine sm;
    int entity;
} UnitComponent;

GENERATE_VECTOR_DECLARATION(UnitComponent)

typedef struct {
    bool exists;
    Rect rect;      // this should be in local space??
    int mask;      // ignored for now
    int entity;
} Collider;

GENERATE_VECTOR_DECLARATION(Collider)

typedef struct {
    int entity1;
    int entity2;
} CollisionData;

GENERATE_VECTOR_DECLARATION(CollisionData)

typedef struct GameState GameState;


void init_colliders(GameState* gs);

void draw_sprites(GameState* gs) ;

typedef struct Unit Unit;


struct UnitEntry {
    bool exists;
    union {
        struct {
            int test;
            int test2;
        } tbd;
        Unit unit;
    };
};

typedef struct UnitEntry UnitEntry;

GENERATE_VECTOR_DECLARATION(UnitEntry)

void init_units(GameState* gs);

void draw_units(GameState* gs);

void command_units(GameState* gs);

void advance_units(GameState* gs);


#endif //_RTS_UNIT_H
