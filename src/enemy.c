//
// Created by brych on 27/01/2021.
//

#include "enemy.h"
#include <time.h>
#include "unit.h"
#include "gamestate.h"

#define UNIT_COOLDOWN 2
#define SIEGE_CAP 10

void init_enemy(GameState* gs) {
    gs->resources.enemy.units = vec_int_new();
}

Vec2 random_offset() {
    int x = (rand() % 100) - 50;
    int y = (rand() % 100) - 50;
    return vec2_make((float)x, (float)y);
}

void process_enemy(GameState* gs) {
    Vec2 spawn_point = vec2_add(vec2_make(1100, 100), random_offset());
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC_RAW, &now);

    if(gs->resources.game.enemy_balance >= UNIT_COST) {
        gs->resources.game.enemy_balance -= UNIT_COST;

        int new_unit = create_unit(gs, spawn_point, ENEMY_TEAM);

        UnitComponent* uc = vec_UnitComponent_get_ptr(gs->unit_components, new_unit);
        uc->sm.state = WATCH;
        vec_int_push(gs->resources.enemy.units, new_unit);

    }

    if(VEC_LEN(gs->resources.enemy.units) >= SIEGE_CAP){
        for (int i = 0; i < VEC_LEN(gs->resources.enemy.units); ++i) {
            int entity = vec_int_get(gs->resources.enemy.units, i);
            UnitComponent* uc = vec_UnitComponent_get_ptr(gs->unit_components, entity);
            uc->sm.state = A_MOVE;
            uc->sm.a_move.dest = vec2_make(100, 600);
        }

        vec_int_clear(gs->resources.enemy.units);
    }
}

