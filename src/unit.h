//
// Created by brych on 16/01/2021.
//


#ifndef _RTS_UNIT_H
#define _RTS_UNIT_H

#include "coord_utils.h"
#include "gamestate.h"
#include <allegro5/allegro5.h>

extern ALLEGRO_BITMAP* unit_sprite;

struct Unit {
    Vec2  position;
    Rect collider;
    Vec2 destination;
} ;

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

void init_units(GameState* gs);

void draw_units(GameState* gs);

void command_units(GameState* gs, ALLEGRO_EVENT event);

void advance_units(GameState* gs);


#endif //_RTS_UNIT_H
