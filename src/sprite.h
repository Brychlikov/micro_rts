//
// Created by brych on 20/01/2021.
//

// Sprite component. This module loads sprites and takes care of rendering them on entities having a Sprite component

#ifndef _RTS_SPRITE_H
#define _RTS_SPRITE_H

#include <allegro5/allegro5.h>
#include "vector.h"
#include "coord_utils.h"
#include "gamestate_forward.h"

extern ALLEGRO_COLOR pure_tint;

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

void deinit_sprites(GameState* gs);


#endif //_RTS_SPRITE_H
