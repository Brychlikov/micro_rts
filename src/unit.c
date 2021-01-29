//
// Created by brych on 16/01/2021.
//

#include "unit.h"
#include "gamestate.h"
#include "sprite.h"
#include "collider.h"
#include <math.h>

#define UNIT_SPEED 150
#define PI 3.14159265
#define GROUPING_ITERATIONS 10
#define GROUPING_STEP 5
#define REPEL_MULTIPLIER 50000.0
#define ATTRACT_MULTIPLIER 50.0

#define UNIT_ATTACK_RANGE 400
#define UNIT_ATTACK_FOLLOW_RANGE 500
#define UNIT_HP 100

GENERATE_VECTOR_DEFINITION(UnitComponent)


ALLEGRO_COLOR pure_tint;
ALLEGRO_COLOR selected_tint;

int create_unit(GameState *gs, Vec2 position, int team) {

    int new = create_entity(gs);
    Transform t = {
            .exists=true,
            .position=position,
            .rotation=1,
            .scale=0.4,
            .entity=new
    };
    *vec_Transform_get_ptr(gs->transform_components, new) = t;
    Sprite s = {
            .exists=true,
            .offset={.x=40, .y=40},
            .rotation=PI/2,
            .bitmap=gs->resources.sprites.unit,
            .entity=new,
            .tint=pure_tint,
    };
    *vec_Sprite_get_ptr(gs->sprite_components, new) = s;

    int mask = team == PLAYER_TEAM ? FRIENDLY_UNIT : ENEMY_UNIT;

    Collider c = {
            .exists=true,
            .rect={
                    .tl={.x=-40, .y=-40},
                    .br={.x=40, .y=40},
            },
            .mask=mask,
            .entity=new
    };

    *vec_Collider_get_ptr(gs->collider_components, new) = c;

    UnitComponent uc = {
            .exists=true,
            .sm={
                    .state=IDLE,
            },
            .entity=new,
    };
    *vec_UnitComponent_get_ptr(gs->unit_components, new) = uc;

    Health h = {
            .exists=true,
            .team=team,
            .points=UNIT_HP,
            .entity=new,
    };
    *vec_Health_get_ptr(gs->health_components, new) = h;

    return new;
}

Vector_int units_selected;

void init_units(GameState* gs) {

    pure_tint = al_map_rgba_f(1, 1, 1, 1);
    selected_tint = al_map_rgba_f(0.3, 1, 0.3, 1);

    units_selected = vec_int_with_capacity(64);

    // ECS land below
    create_unit(gs, vec2_make(100, 100), 0);
    create_unit(gs, vec2_make(200, 100), 0);
    create_unit(gs, vec2_make(300, 100), 0);
    create_unit(gs, vec2_make(100, 200), 0);
    create_unit(gs, vec2_make(200, 200), 0);
    create_unit(gs, vec2_make(300, 200), 0);
    create_unit(gs, vec2_make(100, 300), 0);
    create_unit(gs, vec2_make(200, 300), 0);
    create_unit(gs, vec2_make(300, 300), 0);
}

//void draw_units(GameState *gs) {
//    for (int i = 0; i < gs->unit_entries.inner->length; ++i) {
//        struct UnitEntry u_e = vec_UnitEntry_get(gs->unit_entries, i);
//        if(u_e.exists) {
//            al_draw_bitmap(unit_sprite, u_e.unit.position.x, u_e.unit.position.y, 0);
//        }
//    }
//
//    //draw over selected units with tinted bitmap
//    for (int i = 0; i < gs->units_selected_indices.inner->length; ++i) {
//        int index = vec_int_get(gs->units_selected_indices, i);
//        struct UnitEntry u_e = vec_UnitEntry_get(gs->unit_entries, index);
//        if(u_e.exists) {
//            al_draw_tinted_bitmap(unit_sprite, al_map_rgb(0, 100, 0), u_e.unit.position.x, u_e.unit.position.y, 0);
//        }
//    }
//}

int target_unit(GameState* gs, int targeting, Vector_int targetables) {
    // targetables - vector of entities having Health and Transform components

    // returns the closest targetable unit in range, -1 otherwise
    int result = -1;
    float closest_distance = INFINITY;
    Transform targeting_transform = vec_Transform_get(gs->transform_components, targeting);
    Health targeting_health = vec_Health_get(gs->health_components, targeting);
    assert(targeting_transform.exists);

    for (int i = 0; i < VEC_LEN(targetables); ++i) {
        int entity = vec_int_get(targetables, i);
        Transform t = vec_Transform_get(gs->transform_components, entity);
        assert(t.exists);
        Health h = vec_Health_get(gs->health_components, entity);
        assert(h.exists);

        float dist = vec2_length(vec2_sub(t.position, targeting_transform.position));
        if(dist <= UNIT_ATTACK_RANGE && dist < closest_distance && h.team != targeting_health.team) {
            result = entity;
            closest_distance = dist;
        }
    }

    return result;
}

void advance_units(GameState *gs) {
    // prepare vector of targetable entities:
    Vector_int targetables = vec_int_with_capacity(128);
    for (int i = 0; i < VEC_LEN(gs->entities); ++i) {
        Transform t = vec_Transform_get(gs->transform_components, i);
        if(!t.exists) continue;

        Health h = vec_Health_get(gs->health_components, i);
        if(!h.exists) continue;

        vec_int_push(targetables, i);
    }

    for (int i = 0; i < VEC_LEN(gs->entities); ++i) {
        if(!vec_bool_get(gs->entities, i)) continue;

        UnitComponent* unit = vec_UnitComponent_get_ptr(gs->unit_components, i);
        if(!unit->exists) continue;

        Transform* my_transform = vec_Transform_get_ptr(gs->transform_components, i);
        Health my_health = vec_Health_get(gs->health_components, i);

        switch(unit->sm.state) {
            case A_MOVE:
                ;
                int target = target_unit(gs, i, targetables);
                if(target != -1) {
                    unit->sm.state = AGGRESSIVE;
                    Vec2 prev_dest = unit->sm.a_move.dest;
                    unit->sm.aggressive.target = target;
                    unit->sm.aggressive.next = A_MOVE;
                    unit->sm.aggressive.next_dest = prev_dest;
                    break;
                }
                // here we depend on move and a_move structs inside UnitStateMachine having the same layout
                // if no target found, proceed to MOVE
            case MOVE:
                ;     // I hate this language sooooo much
                Vec2 diff = vec2_sub(unit->sm.move.dest, my_transform->position);
                Vec2 delta;
                float distance = gs->resources.time_delta * UNIT_SPEED;
                //TODO make it adhere to time deltas
                if(vec2_length(diff) <= distance) {
                    delta = vec2_sub(unit->sm.move.dest, my_transform->position);
                }
                else {
                    Vec2 dir = vec2_norm(diff);
                    delta = vec2_scale(dir, distance);
                }
                float angle = atan2f(delta.y, delta.x);
                my_transform->position = vec2_add(my_transform->position, delta);
                my_transform->rotation = angle;
                break;
            case AGGRESSIVE:
                ;
                // turn unit to face the enemy:
                Transform target_transform = vec_Transform_get(gs->transform_components, unit->sm.aggressive.target);
                Vec2 to_enemy = vec2_sub(target_transform.position, my_transform->position);

                bool target_dead = !vec_bool_get(gs->entities, unit->sm.aggressive.target);
                bool target_too_far = vec2_length(to_enemy) > UNIT_ATTACK_FOLLOW_RANGE;

                if(target_dead || target_too_far) { // if target is dead
                    unit->sm.state = unit->sm.aggressive.next;

                    // once again depending on same layout of move and a-move
                    unit->sm.move.dest = unit->sm.aggressive.next_dest;
                    break;
                }

                assert(target_transform.exists);
                angle = atan2f(to_enemy.y, to_enemy.x);
                my_transform->rotation = angle;

                struct timespec now;
                clock_gettime(CLOCK_MONOTONIC_RAW, &now);
                if(now.tv_sec - 1 >= unit->sm.aggressive.last_shot_timestamp.tv_sec) {
                    create_bullet(gs, *my_transform, my_health.team);
                    unit->sm.aggressive.last_shot_timestamp = now;
                }
                break;
            case IDLE:
                break;
            case WATCH:
                target = target_unit(gs, i, targetables);
                if(target != -1) {
                    unit->sm.state = AGGRESSIVE;
                    unit->sm.aggressive.target = target;
                    unit->sm.aggressive.next = WATCH;
                    break;
                }
        }


    }
}

void command_units(GameState *gs) {
    if(gs->resources.mouse_buttons[RIGHT_MOUSE_BUTTON] || gs->resources.keys[ALLEGRO_KEY_A]) {
        Vec2 mean_position = vec2_make(0, 0);
        int unit_count = 0;

        vec_int_clear(units_selected);


        for (int i = 0; i < VEC_LEN(gs->resources.selection.entities_selected); ++i) {
            int entity = vec_int_get(gs->resources.selection.entities_selected, i);
            UnitComponent ue = vec_UnitComponent_get(gs->unit_components, entity);
            Transform t = vec_Transform_get(gs->transform_components, entity);
            if(t.exists && ue.exists) {
                vec_int_push(units_selected, entity);
                unit_count++;
                mean_position = vec2_add(mean_position, t.position);
            }
        }

        if(unit_count == 0) return;

        mean_position = vec2_scale(mean_position, 1.0 / unit_count);

        // calculate the center unit
        int center_unit;
        float closest_dist = INFINITY;
        for (int i = 0; i < VEC_LEN(units_selected); ++i) {
            Transform t = vec_Transform_get(gs->transform_components, i);
            if(vec2_length(vec2_sub(mean_position, t.position)) < closest_dist) {
                closest_dist = vec2_length(vec2_sub(mean_position, t.position));
                center_unit = i;
            }
        }

        Transform center_transform = vec_Transform_get(gs->transform_components, center_unit);
        Vector_Vec2 offsets = vec_Vec2_with_capacity(VEC_LEN(units_selected));

        for (int i = 0; i < VEC_LEN(units_selected); ++i) {
            int entity = vec_int_get(units_selected, i);
            Transform t = vec_Transform_get(gs->transform_components, entity);

            Vec2 offset = vec2_sub(center_transform.position, t.position);
            vec_Vec2_push(offsets, offset);
        }

        for (int i = 0; i < GROUPING_ITERATIONS; ++i) {
            for (int j = 0; j < VEC_LEN(units_selected); ++j) {
                Vec2* offset = vec_Vec2_get_ptr(offsets, j);
                if(offset->x == 0 && offset->y == 0) {
                    continue;
                }
                Vec2 dir_to_center = vec2_norm(vec2_flip(*offset));

                //calculate repel vector
                Vec2 repel = vec2_make(0, 0);
                for (int k = 0; k < VEC_LEN(units_selected); ++k) {
                    Vec2* other = vec_Vec2_get_ptr(offsets, k);
                    if(other == offset) continue;

                    Vec2 diff = vec2_sub(*offset, *other);
                    float diff_len_sq = vec2_length_sq(diff);
                    if(diff_len_sq == 0) continue;

                    Vec2 direction = vec2_norm(diff);
                    repel = vec2_add(repel, vec2_scale(direction, REPEL_MULTIPLIER / diff_len_sq));
                }
                Vec2 attract_direction = vec2_flip(vec2_norm(*offset));
                Vec2 attract = vec2_scale(attract_direction, ATTRACT_MULTIPLIER);
//                printf("Attract vec: %f, %f\n", attract.x, attract.y);
//                printf("Repel vec: %f, %f\n\n", repel.x, repel.y);

                *offset = vec2_add(*offset, repel);
                *offset = vec2_add(*offset, attract);
            }
        }




        for (int i = 0; i < VEC_LEN(units_selected); ++i) {
//            printf("command issued to unit %d\n", i);
            int entity = vec_int_get(units_selected, i);
            Vec2 offset = vec_Vec2_get(offsets, i);

            UnitComponent* ue = vec_UnitComponent_get_ptr(gs->unit_components, entity);
            assert(ue->exists);

            if(gs->resources.keys[ALLEGRO_KEY_A]) {  // A_MOVE
                ue->sm.state = A_MOVE;
                ue->sm.a_move.dest = vec2_sub(gs->resources.mouse_position, offset);
            }
            else {  // MOVE
                ue->sm.state = MOVE;
                ue->sm.move.dest = vec2_sub(gs->resources.mouse_position, offset);
            }
        }
        vec_Vec2_destroy(offsets);
    }
}

