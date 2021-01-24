//
// Created by brych on 20/01/2021.
//

#ifndef _RTS_SPRITE_H
#define _RTS_SPRITE_H

#include <allegro5/allegro5.h>
#include "vector.h"
#include "coord_utils.h"
#include "gamestate_forward.h"

typedef struct {
    bool exists;  // we want all components to have this as first field
    ALLEGRO_BITMAP* bitmap;
    Vec2 offset; // coordinates of center point in local space
    float rotation;
    ALLEGRO_COLOR tint;
    int entity;
} Sprite;

GENERATE_VECTOR_DECLARATION(Sprite)

void draw_sprites(GameState* gs) ;

void init_sprites(GameState* gs) ;

#endif //_RTS_SPRITE_H
