//
// Created by brych on 27/01/2021.
//

#include "enemy.h"
#include <time.h>
#include "unit.h"
#include "gamestate.h"

#define UNIT_COOLDOWN 2

void init_enemy(GameState* gs) {

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

    if(now.tv_sec - UNIT_COOLDOWN >= gs->resources.enemy.last_unit_spawn_timestamp.tv_sec) {
        gs->resources.enemy.last_unit_spawn_timestamp = now;

        int new_unit = create_unit(gs, spawn_point, ENEMY_TEAM);

        UnitComponent* uc = vec_UnitComponent_get_ptr(gs->unit_components, new_unit);
        uc->sm.state = A_MOVE;
        uc->sm.a_move.dest = vec2_make(100, 900);
    }
}

