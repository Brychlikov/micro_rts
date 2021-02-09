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

void deinit_enemy(GameState *gs) {
    vec_int_destroy(gs->resources.enemy.units);
}

Vec2 random_offset() {
    int x = (rand() % 100) - 50;
    int y = (rand() % 100) - 50;
    return vec2_make((float)x, (float)y);
}

int check_if_building_needs_help(GameState* gs) {
    BuildingComponent base_bc = vec_BuildingComponent_get(gs->building_components, gs->resources.game.enemy_base);
    if(base_bc.under_attack) {
        return gs->resources.game.enemy_base;
    }

    for (unsigned int i = 0; i < VEC_LEN(gs->resources.game.enemy_auxiliary_bases); ++i) {
        int entity = vec_int_get(gs->resources.game.enemy_auxiliary_bases, i);
        bool alive = vec_bool_get(gs->entities, entity);
        BuildingComponent aux_bc = vec_BuildingComponent_get(gs->building_components, entity);
        if(alive && aux_bc.under_attack) {
            return entity;
        }
    }
    return -1;
}

void process_enemy(GameState* gs) {
    Vec2 base_position = vec_Transform_get(gs->transform_components, gs->resources.game.enemy_base).position;
    Vec2 spawn_point = vec2_add(vec2_add(base_position, vec2_make(-300, 0)), random_offset());

    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC_RAW, &now);

    if(gs->resources.game.enemy_balance >= UNIT_COST) {
        gs->resources.game.enemy_balance -= UNIT_COST;

        int new_unit = create_unit(gs, spawn_point, ENEMY_TEAM);

        UnitComponent* uc = vec_UnitComponent_get_ptr(gs->unit_components, new_unit);
        uc->sm.state = WATCH;
        vec_int_push(gs->resources.enemy.units, new_unit);

    }

    int building_to_help = check_if_building_needs_help(gs);
    
    Vector_int idle_units = vec_int_new();
    Vector_int alive_units = vec_int_new();
    for (unsigned int i = 0; i < VEC_LEN(gs->resources.enemy.units); ++i) {
        int entity = vec_int_get(gs->resources.enemy.units, i);
        bool alive = vec_bool_get(gs->entities, entity);
        UnitComponent uc = vec_UnitComponent_get(gs->unit_components, entity);
        if(alive) {
            vec_int_push(alive_units, entity);
            if(uc.sm.state == IDLE || uc.sm.state == WATCH) {
                vec_int_push(idle_units, entity);
            }
        }
    }

    if(building_to_help != -1) {  // building needs help
        Vec2 building_position = vec_Transform_get(gs->transform_components, building_to_help).position;
        Vec2 rally_point = vec2_add(building_position, vec2_make(-150, 0));
        if(VEC_LEN(idle_units) > 0)
            a_move_units(gs, idle_units, rally_point);
    }

    if(VEC_LEN(idle_units) >= SIEGE_CAP && building_to_help == -1){
        int player_aux_bases_count = 0;
        for (unsigned int i = 0; i < VEC_LEN(gs->resources.game.player_auxiliary_bases); ++i) {
            int entity = vec_int_get(gs->resources.game.player_auxiliary_bases, i);
            player_aux_bases_count += vec_bool_get(gs->entities, entity) ? 1 : 0;
        }

        int max_choice = player_aux_bases_count + 3;
        int choice = rand() % max_choice;

        if(choice < 3) {  // attack main base
            Vec2 player_base_position = vec_Transform_get(gs->transform_components, gs->resources.game.player_base).position;
            Vec2 rally_point = vec2_add(player_base_position, vec2_make(150, 0));
            a_move_units(gs, idle_units, rally_point);
        }
        else {
            int chosen = choice - 3;
            Vec2 target_base_position;
            for (unsigned int i = 0; i < VEC_LEN(gs->resources.game.player_auxiliary_bases); ++i) {
                int entity = vec_int_get(gs->resources.game.player_auxiliary_bases, i);
                bool alive=  vec_bool_get(gs->entities, entity);
                if(alive) {
                    if(chosen == 0) {
                        target_base_position = vec_Transform_get(gs->transform_components, entity).position;
                    }
                    else {
                        chosen--;
                    }
                }
            }

            Vec2 rally_point = vec2_add(target_base_position, vec2_make(150, 0));
            a_move_units(gs, idle_units, rally_point);
        }
    }
    vec_int_destroy(idle_units);
    vec_int_destroy(gs->resources.enemy.units);
    gs->resources.enemy.units = alive_units;
}

