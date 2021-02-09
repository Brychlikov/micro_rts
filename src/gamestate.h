//
// Created by brych on 15/01/2021.
//

// Engine. Defines a GameState type, a global state used by all systems. Contains functions for creating empty entities
// and destroying them. All functionality - all components are later added by corresponding modules.

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
#include "building.h"
#include "laser_turret.h"
#include <allegro5/allegro_font.h>

typedef int Entity;


typedef struct GameState{

    bool should_exit;

    ALLEGRO_TIMER* timer;
    ALLEGRO_EVENT_QUEUE* queue;
    ALLEGRO_DISPLAY* display;
    ALLEGRO_FONT* font;
    ALLEGRO_FONT* big_font;

    bool redraw;

    // resources
    struct {
        unsigned char keys[ALLEGRO_KEY_MAX];
        unsigned char mouse_buttons[MOUSE_KEY_MAX];

        Vec2 mouse_position;

        bool game_begun;
        bool game_ended;
        bool victory;

        bool paused;
        float time_delta;
        struct timespec last_frame_timestamp;

        struct {
            int level;
            float enemy_multiplier;
        } config;

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
            ALLEGRO_BITMAP* bullet_turret;
            ALLEGRO_BITMAP* laser_turret;
        } sprites;

        struct {
            struct timespec last_unit_spawn_timestamp;
            Vector_int units;
        } enemy;

        struct {
            bool active;
            Vector_int units;
        } overdrive;

        struct {
            int player_base;
            int enemy_base;
            int victory_state;
            
            Vector_int player_auxiliary_bases;
            Vector_int enemy_auxiliary_bases;

            float player_income;
            float enemy_income;

            float player_balance;
            float enemy_balance;
        } game;
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
    Vector_LaserTurretComponent laser_turret_components;
    Vector_BuildingComponent building_components;

} GameState;

void init_components(GameState* gs);

void deinit_components(GameState* gs);

int create_entity(GameState* gs);

void destroy_entity(GameState* gs, Entity entity);

#endif //_RTS_GAMESTATE_H
