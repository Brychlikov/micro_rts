//
// Created by brych on 15/01/2021.
//

#ifndef _RTS_GAMESTATE_H
#define _RTS_GAMESTATE_H

#include "coord_utils.h"
#include <allegro5/allegro5.h>
#include "unit.h"
#include "vector.h"
#include "mouse.h"
#include <allegro5/allegro_font.h>

typedef int Entity;


typedef struct GameState{

    bool should_exit;

    ALLEGRO_TIMER* timer;
    ALLEGRO_EVENT_QUEUE* queue;
    ALLEGRO_DISPLAY* display;
    ALLEGRO_FONT* font;

    bool redraw;

    struct {
        unsigned char keys[ALLEGRO_KEY_MAX];
        unsigned char mouse_buttons[MOUSE_KEY_MAX];

        Vec2 mouse_position;

        struct {
            bool in_progress;
            Rect area;
        } selection;
    } resources;

    Vector_bool entities;  // true - alive false - dead

    // Component land
    // invariant -- all the vectors have the same length as entities
    Vector_Transform transform_components;
    Vector_Sprite sprite_components;
    Vector_UnitEntry unit_entries;
    Vector_int units_selected_indices;
} GameState;

void init_components(GameState* gs);

int create_entity(GameState* gs);

#endif //_RTS_GAMESTATE_H
