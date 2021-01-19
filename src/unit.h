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
    int entity;
} Sprite;

GENERATE_VECTOR_DECLARATION(Sprite)

struct Unit {
    Vec2  position;
    Rect collider;
    Vec2 destination;
} ;

typedef struct GameState GameState;

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

void command_units(GameState* gs, ALLEGRO_EVENT event);

void advance_units(GameState* gs);


#endif //_RTS_UNIT_H
