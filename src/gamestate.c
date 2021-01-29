//
// Created by brych on 19/01/2021.
//

#include "gamestate.h"
#include "building.h"

#define PLAYER_BASE_INCOME 10
#define ENEMY_BASE_INCOME 45

void init_components(GameState *gs) {
    gs->entities = vec_bool_new();

    gs->transform_components = vec_Transform_new();
    gs->collider_components = vec_Collider_new();
    gs->unit_components = vec_UnitComponent_new();
}

void init_game(GameState* gs) {

    int enemy_building = create_building(gs, vec2_make(1180, 100), ENEMY_TEAM);
    int player_building = create_building(gs, vec2_make(100, 620), PLAYER_TEAM);

    gs->resources.game.enemy_base = enemy_building;
    gs->resources.game.player_base = player_building;

    gs->resources.game.player_income = PLAYER_BASE_INCOME;
    gs->resources.game.enemy_income = ENEMY_BASE_INCOME;

    gs->resources.game.player_balance = 0;
    gs->resources.game.enemy_balance = 0;
}

void process_income(GameState *gs) {
    gs->resources.game.player_balance += gs->resources.time_delta * gs->resources.game.player_income;
    gs->resources.game.enemy_balance += gs->resources.time_delta * gs->resources.game.enemy_income;
}

void render_balance(GameState *gs) {
    int balance = (int)gs->resources.game.player_balance;
    float income = gs->resources.game.player_income;
    al_draw_textf(gs->font, al_map_rgb_f(1, 1, 1), 25, 25, 0, "Balance: %d", balance);
    al_draw_textf(gs->font, al_map_rgb_f(1, 1, 1), 25, 50, 0, "+%.1f", income);
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

