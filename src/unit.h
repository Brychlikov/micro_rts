//
// Created by brych on 16/01/2021.
//


#ifndef _RTS_UNIT_H
#define _RTS_UNIT_H

#include "coord_utils.h"
//#include "gamestate.h"
#include <allegro5/allegro5.h>
#include "vector.h"
#include <time.h>
#include "gamestate_forward.h"

#define UNIT_COST 100
extern ALLEGRO_COLOR pure_tint;
extern ALLEGRO_COLOR selected_tint;

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
            Vec2 next_dest;
            float shot_cooldown;
        } aggressive;
    };
} UnitStateMachine;

typedef struct UnitComponent {
    bool exists;
    UnitStateMachine sm;
    bool overdrive;
    int entity;
} UnitComponent;

GENERATE_VECTOR_DECLARATION(UnitComponent)

//typedef struct GameState GameState;



void init_units(GameState* gs);

void deinit_units(GameState* gs);

void draw_units(GameState* gs);

int create_unit(GameState* gs, Vec2 position, int team);

void command_units(GameState* gs);

void advance_units(GameState* gs);

void buy_units(GameState* gs);

void process_overdrive(GameState* gs);

int target_unit(GameState* gs, int targeting, Vector_int targetables, float max_range);


#endif //_RTS_UNIT_H
