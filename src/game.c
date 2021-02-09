//
// Created by brych on 09/02/2021.
//

#include "building.h"
#include "gamestate.h"
#include "game.h"
#include "tints.h"

#define PLAYER_BASE_INCOME 10
#define INCOME_PER_BASE 5
#define AUX_BASE_HP 200
#define MAIN_BASE_MAX_HP 1000
#define OVERDRIVE_COST 5

void init_game(GameState* gs) {

    gs->resources.game.player_auxiliary_bases = vec_int_new();
    gs->resources.game.enemy_auxiliary_bases = vec_int_new();

    char fname[128];
    sprintf(fname, "assets/map%d.txt", gs->resources.config.level);
    printf("Loading map from %s\n", fname);

    FILE* map = fopen(fname, "r");
    if(map == NULL) {
        fprintf(stderr, "Could not read map file. Is the ./assets folder accessible?\n");
        exit(1);
    }
    int width, height;
    float multiplier;
    if(fscanf(map, "%d %d %f", &width, &height, &multiplier) != 3)  {
        fprintf(stderr, "Malformed map file. Are the 3 config values in the first line?\n");
        exit(1);
    }

    gs->resources.config.enemy_multiplier = multiplier;

    float margin = 100;
    float usable_width = (float)al_get_display_width(gs->display) - 2 * margin;
    float usable_height = (float)al_get_display_height(gs->display) - 2 * margin;
    printf("w: %f h: %f\n", usable_width, usable_height + 2 * margin + 2 * margin);

    for (int map_y = 0; map_y < height; ++map_y) {
        for (int map_x = 0; map_x < width; ++map_x) {
            float x = margin + (float)map_x / (float)width * usable_width;
            float y = margin + (float)map_y / (float)height * usable_height;
            char c;
            fscanf(map, " %c", &c);

            switch(c) {
                case '#':  // player main base
                    ;
                    int player_base = create_building(gs, vec2_make(x, y), 1, MAIN_BASE_MAX_HP,  PLAYER_TEAM);
                    gs->resources.game.player_base = player_base;
                    break;
                case '$':  // enemy main base
                    ;
                    int enemy_base = create_building(gs, vec2_make(x, y), 1, MAIN_BASE_MAX_HP,  ENEMY_TEAM);
                    gs->resources.game.enemy_base = enemy_base;
                    break;
                case '*': // player auxiliary base
                    ;
                    int aux_player_base = create_building(gs, vec2_make(x, y), 0.5, AUX_BASE_HP, PLAYER_TEAM);
                    vec_int_push(gs->resources.game.player_auxiliary_bases, aux_player_base);
                    break;
                case '@': // enemy auxiliary base
                    ;
                    int aux_enemy_base = create_building(gs, vec2_make(x, y), 0.5, AUX_BASE_HP, ENEMY_TEAM);
                    vec_int_push(gs->resources.game.enemy_auxiliary_bases, aux_enemy_base);
                    break;
                case 'x':  // enemy laser turret
                    ;
                    create_laser_turret(gs, vec2_make(x, y), ENEMY_TEAM);
                    break;
                case '+':  // player laser turret
                    ;
                    create_laser_turret(gs, vec2_make(x, y), PLAYER_TEAM);
                    break;
                case '.':
                    break;
                default:
                    fprintf(stderr, "WARNING: Unknown character in map\n");
            }
        }
    }


    gs->resources.game.player_income = PLAYER_BASE_INCOME;
    gs->resources.game.enemy_income = PLAYER_BASE_INCOME * gs->resources.config.enemy_multiplier;

    gs->resources.game.player_balance = 600;
    gs->resources.game.enemy_balance = 600;

}

void deinit_game(GameState *gs) {
    vec_int_destroy(gs->resources.game.player_auxiliary_bases);
    vec_int_destroy(gs->resources.game.enemy_auxiliary_bases);
}

void update_income_values(GameState *gs) {
    if(!gs->resources.game_begun) return;
    float player_income = PLAYER_BASE_INCOME;
    for (unsigned int i = 0; i < VEC_LEN(gs->resources.game.player_auxiliary_bases); ++i) {
        int entity = vec_int_get(gs->resources.game.player_auxiliary_bases, i);
        bool alive = vec_bool_get(gs->entities, entity);
        if(alive) {
            player_income += INCOME_PER_BASE;
        }
    }

    // add overdrive cost
    int count = 0;
    for (unsigned int i = 0; i < VEC_LEN(gs->resources.overdrive.units); ++i) {
        int entity = vec_int_get(gs->resources.overdrive.units, i);
        count += vec_bool_get(gs->entities, entity) ? 1 : 0;
    }

    player_income -= count * OVERDRIVE_COST;

    float enemy_income = PLAYER_BASE_INCOME;
    for (unsigned int i = 0; i < VEC_LEN(gs->resources.game.enemy_auxiliary_bases); ++i) {
        int entity = vec_int_get(gs->resources.game.enemy_auxiliary_bases, i);
        bool alive = vec_bool_get(gs->entities, entity);
        if(alive) {
            enemy_income += INCOME_PER_BASE;
        }
    }
    enemy_income *= gs->resources.config.enemy_multiplier;

    gs->resources.game.player_income = player_income;
    gs->resources.game.enemy_income = enemy_income;
}

void process_income(GameState* gs) {
    gs->resources.game.player_balance += gs->resources.time_delta * gs->resources.game.player_income;
    gs->resources.game.enemy_balance += gs->resources.time_delta * gs->resources.game.enemy_income;
}

void render_balance(GameState *gs) {
    int balance = (int)gs->resources.game.player_balance;
    float income = gs->resources.game.player_income;
    al_draw_textf(gs->font, color_code_to_allegro("492C3F"), 25, 25, 0, "Balance: %d", balance);
    al_draw_textf(gs->font, color_code_to_allegro("492C3F"), 25, 50, 0, "%+.1f", income);
}