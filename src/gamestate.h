//
// Created by brych on 15/01/2021.
//

#ifndef _RTS_GAMESTATE_H
#define _RTS_GAMESTATE_H

#include "coord_utils.h"
#include <allegro5/allegro5.h>
//#include "unit.h"
#include "vector.h"
#include <allegro5/allegro_font.h>

//struct Unit;
//struct UnitEntry;

typedef struct {
    bool during_selection;
    Rect selection_area;
    Point mouse_position;

    bool should_exit;

    ALLEGRO_EVENT_QUEUE* event_queue;
    ALLEGRO_TIMER* timer;
    ALLEGRO_EVENT_QUEUE* queue;
    ALLEGRO_DISPLAY* display;
    ALLEGRO_FONT* font;

    bool redraw;

    unsigned char keys[ALLEGRO_KEY_MAX];

    Vector unit_entries;
    Vector units_selected_indices;
} GameState;


#endif //_RTS_GAMESTATE_H
