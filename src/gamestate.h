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
#include "sprite.h"
#include "collider.h"
#include "health.h"
#include "bullet.h"
#include <allegro5/allegro_font.h>

typedef int Entity;


typedef struct GameState{

    bool should_exit;

    ALLEGRO_TIMER* timer;
    ALLEGRO_EVENT_QUEUE* queue;
    ALLEGRO_DISPLAY* display;
    ALLEGRO_FONT* font;

    bool redraw;

    // resources
    struct {
        unsigned char keys[ALLEGRO_KEY_MAX];
        unsigned char mouse_buttons[MOUSE_KEY_MAX];

        Vec2 mouse_position;

        float time_delta;
        struct timespec last_frame_timestamp;

        struct {
            bool in_progress;
            Rect area;
            Vector_int entities_selected;
        } selection;

        struct {
            Vector_CollisionData vec;
            int masks[32];
        } collisions;

        struct {
            ALLEGRO_BITMAP* unit;
            ALLEGRO_BITMAP* building;
            ALLEGRO_BITMAP* bullet;
        } sprites;

        struct {
            struct timespec last_unit_spawn_timestamp;
        } enemy;
    } resources;

    // todo
    // split sprites and resources from gamestate
    Vector_bool entities;  // true - alive false - dead

    // Component land
    // invariant -- all the vectors have the same length as entities
    Vector_Transform transform_components;
    Vector_Collider collider_components;
    Vector_Sprite sprite_components;
    Vector_UnitComponent unit_components;
    Vector_Health health_components;
    Vector_BulletComponent bullet_components;

} GameState;

void init_components(GameState* gs);

int create_entity(GameState* gs);

void destroy_entity(GameState* gs, Entity entity);

#endif //_RTS_GAMESTATE_H
