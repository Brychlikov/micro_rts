//
// Created by brych on 15/01/2021.
//

// Logic of selection UI. Draws a green rectangle and stores selected entities in gs->resources.selection.area

#ifndef _RTS_SELECTION_UI_H
#include <allegro5/allegro5.h>

void draw_selection_area(GameState* );
void selection_system(GameState*);
void init_selection_system(GameState*);
void deinit_selection_system(GameState* gs);

#define _RTS_SELECTION_UI_H

#endif //_RTS_SELECTION_UI_H
