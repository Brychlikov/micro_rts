//
// Created by brych on 16/01/2021.
//


#ifndef _RTS_UNIT_H
#define _RTS_UNIT_H

#include "coord_utils.h"
//#include "gamestate.h"
#include <allegro5/allegro5.h>
#include "vector.h"
#include "gamestate_forward.h"

extern ALLEGRO_BITMAP* unit_sprite;

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

//typedef struct GameState GameState;


void init_colliders(GameState* gs);



void init_units(GameState* gs);

void draw_units(GameState* gs);

void command_units(GameState* gs);

void advance_units(GameState* gs);


#endif //_RTS_UNIT_H
