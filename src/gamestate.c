//
// Created by brych on 19/01/2021.
//

#include "gamestate.h"
#include "building.h"

#define PLAYER_BASE_INCOME 10
#define ENEMY_INCOME_MULTIPLIER 2.0

#define INCOME_PER_BASE 5
#define AUX_BASE_HP 200
#define MAIN_BASE_MAX_HP 1000

#define OVERDRIVE_COST 5

void init_components(GameState *gs) {
    gs->entities = vec_bool_new();

    gs->transform_components = vec_Transform_new();
}

void deinit_components(GameState *gs) {
    vec_bool_destroy(gs->entities);

    vec_Transform_destroy(gs->transform_components);
}

void init_game(GameState* gs) {

    int enemy_building = create_building(gs, vec2_make(1180, 100), 1, MAIN_BASE_MAX_HP, ENEMY_TEAM);
    int player_building = create_building(gs, vec2_make(100, 620), 1, MAIN_BASE_MAX_HP,  PLAYER_TEAM);

    gs->resources.game.enemy_base = enemy_building;
    gs->resources.game.player_base = player_building;

    gs->resources.game.player_income = PLAYER_BASE_INCOME;
    gs->resources.game.enemy_income = PLAYER_BASE_INCOME * ENEMY_INCOME_MULTIPLIER;

    gs->resources.game.player_balance = 0;
    gs->resources.game.enemy_balance = 0;

    gs->resources.game.player_auxiliary_bases = vec_int_new();
    gs->resources.game.enemy_auxiliary_bases = vec_int_new();

    int base1 = create_building(gs, vec2_make(100, 520), 0.5, AUX_BASE_HP, PLAYER_TEAM);
    int base2 = create_building(gs, vec2_make(100, 420), 0.5, AUX_BASE_HP, PLAYER_TEAM);
    int base3 = create_building(gs, vec2_make(100, 320), 0.5, AUX_BASE_HP, PLAYER_TEAM);
    int base4 = create_building(gs, vec2_make(100, 220), 0.5, AUX_BASE_HP, PLAYER_TEAM);

    vec_int_push(gs->resources.game.player_auxiliary_bases, base1);
    vec_int_push(gs->resources.game.player_auxiliary_bases, base2);
    vec_int_push(gs->resources.game.player_auxiliary_bases, base3);
    vec_int_push(gs->resources.game.player_auxiliary_bases, base4);

    int enemy_base1 = create_building(gs, vec2_make(1100, 520), 0.5, AUX_BASE_HP, ENEMY_TEAM);
    int enemy_base2 = create_building(gs, vec2_make(1100, 420), 0.5, AUX_BASE_HP, ENEMY_TEAM);
    int enemy_base3 = create_building(gs, vec2_make(1100, 320), 0.5, AUX_BASE_HP, ENEMY_TEAM);
    int enemy_base4 = create_building(gs, vec2_make(1100, 220), 0.5, AUX_BASE_HP, ENEMY_TEAM);

    vec_int_push(gs->resources.game.enemy_auxiliary_bases, enemy_base1);
    vec_int_push(gs->resources.game.enemy_auxiliary_bases, enemy_base2);
    vec_int_push(gs->resources.game.enemy_auxiliary_bases, enemy_base3);
    vec_int_push(gs->resources.game.enemy_auxiliary_bases, enemy_base4);

    create_laser_turret(gs, vec2_make(1100, 300), ENEMY_TEAM);
}

void deinit_game(GameState *gs) {
    vec_int_destroy(gs->resources.game.player_auxiliary_bases);
    vec_int_destroy(gs->resources.game.enemy_auxiliary_bases);
}

void update_income_values(GameState *gs) {
    float player_income = PLAYER_BASE_INCOME;
    for (int i = 0; i < VEC_LEN(gs->resources.game.player_auxiliary_bases); ++i) {
        int entity = vec_int_get(gs->resources.game.player_auxiliary_bases, i);
        bool alive = vec_bool_get(gs->entities, entity);
        if(alive) {
            player_income += INCOME_PER_BASE;
        }
    }

    // add overdrive cost
    int count = 0;
    for (int i = 0; i < VEC_LEN(gs->resources.overdrive.units); ++i) {
        int entity = vec_int_get(gs->resources.overdrive.units, i);
        count += vec_bool_get(gs->entities, entity) ? 1 : 0;
    }

    player_income -= count * OVERDRIVE_COST;

    float enemy_income = PLAYER_BASE_INCOME;
    for (int i = 0; i < VEC_LEN(gs->resources.game.enemy_auxiliary_bases); ++i) {
        int entity = vec_int_get(gs->resources.game.enemy_auxiliary_bases, i);
        bool alive = vec_bool_get(gs->entities, entity);
        if(alive) {
            enemy_income += INCOME_PER_BASE;
        }
    }
    enemy_income *= ENEMY_INCOME_MULTIPLIER;

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
    al_draw_textf(gs->font, al_map_rgb_f(1, 1, 1), 25, 25, 0, "Balance: %d", balance);
    al_draw_textf(gs->font, al_map_rgb_f(1, 1, 1), 25, 50, 0, "%+.1f", income);
}

int create_entity(GameState* gs) {
    int res = gs->entities.inner->length;
    vec_bool_push(gs->entities, true);

    // for each component type, push zero
    vec_Sprite_push_zero(gs->sprite_components);
    vec_Transform_push_zero(gs->transform_components);
    vec_UnitComponent_push_zero(gs->unit_components);
    vec_Collider_push_zero(gs->collider_components);
    vec_Health_push_zero(gs->health_components);
    vec_BulletComponent_push_zero(gs->bullet_components);
    vec_LaserTurretComponent_push_zero(gs->laser_turret_components);

    // add empty CollisionData element too
    CollisionData cd = {.entity=-1, .next=NULL};
    vec_CollisionData_push(gs->resources.collisions.vec, cd);

    return res;
}

void destroy_entity(GameState* gs, Entity entity) {
    *vec_bool_get_ptr(gs->entities, entity) = false;

    // for each component, set exists=false

    vec_Sprite_get_ptr(gs->sprite_components, entity)->exists = false;
    vec_Transform_get_ptr(gs->transform_components, entity)->exists = false;
    vec_UnitComponent_get_ptr(gs->unit_components, entity)->exists = false;
    vec_Collider_get_ptr(gs->collider_components, entity)->exists = false;
    vec_Health_get_ptr(gs->health_components, entity)->exists = false;
    vec_BulletComponent_get_ptr(gs->bullet_components, entity)->exists = false;
}

