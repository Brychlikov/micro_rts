//
// Created by brych on 16/01/2021.
//

#include "unit.h"
#include "gamestate.h"
#include "sprite.h"
#include "collider.h"
#include <math.h>
#include "keyboard.h"

#define UNIT_SPEED 50
#define UNIT_OVERDRIVE_SPEED 150
#define UNIT_OVERDRIVE_ROTATION_SPEED 4

#define UNIT_DAMAGE 10
#define UNIT_OVERDRIVE_MULTIPLIER 3.0f
#define UNIT_SHOT_COOLDOWN 1.0
#define PI 3.14159265
#define GROUPING_ITERATIONS 10
#define GROUPING_STEP 5
#define REPEL_MULTIPLIER 50000.0
#define ATTRACT_MULTIPLIER 50.0

#define UNIT_ATTACK_RANGE 400
#define UNIT_ATTACK_FOLLOW_RANGE 500
#define UNIT_HP 100

GENERATE_VECTOR_DEFINITION(UnitComponent)


//ALLEGRO_COLOR pure_tint;
//ALLEGRO_COLOR selected_tint;

int create_unit(GameState *gs, Vec2 position, int team) {

    int new = create_entity(gs);
    Transform t = {
            .exists=true,
            .position=position,
            .rotation=1,
            .scale=0.3,
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
                    .tl={.x=-35, .y=-40},
                    .br={.x=30, .y=40},
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
            .overdrive=false,
            .entity=new,
    };
    *vec_UnitComponent_get_ptr(gs->unit_components, new) = uc;

    Health h = {
            .exists=true,
            .team=team,
            .points=UNIT_HP,
            .max_points=UNIT_HP,
            .entity=new,
    };
    *vec_Health_get_ptr(gs->health_components, new) = h;

    return new;
}

Vector_int units_selected;

void init_units(GameState* gs) {
    gs->unit_components = vec_UnitComponent_new();
    units_selected = vec_int_with_capacity(64);

    gs->resources.overdrive.units = vec_int_new();
}

void deinit_units(GameState* gs) {
    vec_UnitComponent_destroy(gs->unit_components);
    vec_int_destroy(gs->resources.overdrive.units);
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

int target_unit(GameState *gs, int targeting, Vector_int targetables, float max_range) {
    // targetables - vector of entities having Health and Transform components

    // returns the closest targetable unit in range, -1 otherwise
    int result = -1;
    float closest_distance = INFINITY;
    Transform targeting_transform = vec_Transform_get(gs->transform_components, targeting);
    Health targeting_health = vec_Health_get(gs->health_components, targeting);
    assert(targeting_transform.exists);

    for (unsigned int i = 0; i < VEC_LEN(targetables); ++i) {
        int entity = vec_int_get(targetables, i);
        Transform t = vec_Transform_get(gs->transform_components, entity);
        assert(t.exists);
        Health h = vec_Health_get(gs->health_components, entity);
        assert(h.exists);

        float dist = vec2_length(vec2_sub(t.position, targeting_transform.position));
        if(dist <= max_range && dist < closest_distance && h.team != targeting_health.team) {
            result = entity;
            closest_distance = dist;
        }
    }

    return result;
}

void advance_units(GameState *gs) {
    // prepare vector of targetable entities:
    Vector_int targetables = vec_int_with_capacity(128);
    for (unsigned int i = 0; i < VEC_LEN(gs->entities); ++i) {
        Transform t = vec_Transform_get(gs->transform_components, i);
        if(!t.exists) continue;

        Health h = vec_Health_get(gs->health_components, i);
        if(!h.exists) continue;

        vec_int_push(targetables, i);
    }

    for (unsigned int i = 0; i < VEC_LEN(gs->entities); ++i) {
        if(!vec_bool_get(gs->entities, i)) continue;

        UnitComponent* unit = vec_UnitComponent_get_ptr(gs->unit_components, i);
        if(!unit->exists) continue;

        Transform* my_transform = vec_Transform_get_ptr(gs->transform_components, i);
        Health my_health = vec_Health_get(gs->health_components, i);

        float overdrive_multiplier = unit->overdrive ? UNIT_OVERDRIVE_MULTIPLIER : 1.0f;
        unit->sm.shot_cooldown -= gs->resources.time_delta * overdrive_multiplier;

        switch(unit->sm.state) {
            case A_MOVE:
                ;
                int target = target_unit(gs, i, targetables, UNIT_ATTACK_RANGE);
                if(target != -1) {
                    assert(vec_Health_get(gs->health_components, target).team != my_health.team);
//                    printf("Unit %d of team %d targeting entity %d of team %d\n", i, my_health.team, target, vec_Health_get(gs->health_components, target).team);
                    unit->sm.state = AGGRESSIVE;
                    Vec2 prev_dest = unit->sm.a_move.dest;
                    unit->sm.aggressive.target = target;
                    unit->sm.aggressive.next = A_MOVE;
                    unit->sm.aggressive.next_dest = prev_dest;
                    break;
                }
                if(my_transform->position.x == unit->sm.a_move.dest.x && my_transform->position.y == unit->sm.a_move.dest.y) {
                    unit->sm.state = WATCH;
                }
                // here we depend on move and a_move structs inside UnitStateMachine having the same layout
                // if no target found, proceed to MOVE
                __attribute__((fallthrough));
            case MOVE:
                ;     // I hate this language sooooo much
                Vec2 diff = vec2_sub(unit->sm.move.dest, my_transform->position);
                Vec2 delta;
                float distance = gs->resources.time_delta * UNIT_SPEED * overdrive_multiplier;
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

                if(my_transform->position.x == unit->sm.a_move.dest.x && my_transform->position.y == unit->sm.a_move.dest.y) {
                    unit->sm.state = IDLE;
                }
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

                if(unit->sm.shot_cooldown < 0) unit->sm.shot_cooldown = 0;
                if(unit->sm.shot_cooldown == 0) {
                    create_bullet(gs, *my_transform, my_health.team, UNIT_DAMAGE);
                    // create_bullet can cause a realloc, invalidating unit pointer
                    unit = vec_UnitComponent_get_ptr(gs->unit_components, i);
                    unit->sm.shot_cooldown += UNIT_SHOT_COOLDOWN;
                }
                break;
            case IDLE:
                break;
            case WATCH:
                target = target_unit(gs, i, targetables, UNIT_ATTACK_RANGE);
                if(target != -1) {
                    unit->sm.state = AGGRESSIVE;
                    unit->sm.aggressive.target = target;
                    unit->sm.shot_cooldown = 0;
                    unit->sm.aggressive.next = WATCH;
                    break;
                }
        }


    }
    vec_int_destroy(targetables);
}

Vector_Vec2 calculate_group_offsets(GameState *gs, Vector_int units) {
    Vector_Vec2 result = vec_Vec2_with_capacity(units.inner->length);
    Vec2 mean_position = vec2_make(0, 0);

    for (unsigned int i = 0; i < VEC_LEN(units); ++i) {
        int entity = vec_int_get(units, i);
        Transform t = vec_Transform_get(gs->transform_components, entity);
        mean_position = vec2_add(mean_position, t.position);
    }

    mean_position = vec2_scale(mean_position, 1.0f / VEC_LEN(units));

    // calculate the center unit
    int center_unit;
    float closest_dist = INFINITY;
    for (unsigned int i = 0; i < VEC_LEN(units); ++i) {
        int entity = vec_int_get(units, i);
        Transform t = vec_Transform_get(gs->transform_components, entity);
        if(vec2_length(vec2_sub(mean_position, t.position)) < closest_dist) {
            closest_dist = vec2_length(vec2_sub(mean_position, t.position));
            center_unit = entity;
        }
    }

    Transform center_transform = vec_Transform_get(gs->transform_components, center_unit);

    for (unsigned int i = 0; i < VEC_LEN(units); ++i) {
        int entity = vec_int_get(units, i);
        Transform t = vec_Transform_get(gs->transform_components, entity);

        Vec2 offset = vec2_sub(center_transform.position, t.position);
        vec_Vec2_push(result, offset);
    }

    for (int i = 0; i < GROUPING_ITERATIONS; ++i) {
        for (unsigned int j = 0; j < VEC_LEN(units); ++j) {
            Vec2* offset = vec_Vec2_get_ptr(result, j);
            if(offset->x == 0 && offset->y == 0) {
                continue;
            }

            //calculate repel vector
            Vec2 repel = vec2_make(0, 0);
            for (unsigned int k = 0; k < VEC_LEN(units); ++k) {
                Vec2* other = vec_Vec2_get_ptr(result, k);
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

    return result;
}

void move_units(GameState *gs, Vector_int units, Vec2 dest) {
    Vector_Vec2 offsets = calculate_group_offsets(gs, units);

    for (unsigned int i = 0; i < VEC_LEN(units); ++i) {
        int entity = vec_int_get(units, i);
        Vec2 offset = vec_Vec2_get(offsets, i);

        UnitComponent* ue = vec_UnitComponent_get_ptr(gs->unit_components, entity);
        assert(ue->exists);

        ue->sm.state = MOVE;
        ue->sm.move.dest = vec2_sub(dest, offset);
    }
    vec_Vec2_destroy(offsets);
}

void a_move_units(GameState *gs, Vector_int units, Vec2 dest) {
    Vector_Vec2 offsets = calculate_group_offsets(gs, units);

    for (unsigned int i = 0; i < VEC_LEN(units); ++i) {
        int entity = vec_int_get(units, i);
        Vec2 offset = vec_Vec2_get(offsets, i);

        UnitComponent* ue = vec_UnitComponent_get_ptr(gs->unit_components, entity);
        assert(ue->exists);

        ue->sm.state = A_MOVE;
        ue->sm.a_move.dest = vec2_sub(dest, offset);
    }
    vec_Vec2_destroy(offsets);
}

void command_units(GameState *gs) {
    if(gs->resources.mouse_buttons[RIGHT_MOUSE_BUTTON] || gs->resources.keys[ALLEGRO_KEY_A]) {
        vec_int_clear(units_selected);


        for (unsigned int i = 0; i < VEC_LEN(gs->resources.selection.entities_selected); ++i) {
            int entity = vec_int_get(gs->resources.selection.entities_selected, i);
            UnitComponent ue = vec_UnitComponent_get(gs->unit_components, entity);
            Transform t = vec_Transform_get(gs->transform_components, entity);
            if(t.exists && ue.exists) {
                vec_int_push(units_selected, entity);
            }
        }

        if(VEC_LEN(units_selected) == 0) return;

        if(gs->resources.keys[ALLEGRO_KEY_A]) {
            a_move_units(gs, units_selected, gs->resources.mouse_position);
        }
        else {
            move_units(gs, units_selected, gs->resources.mouse_position);
        }
    }
}

void process_overdrive(GameState *gs) {
    if(gs->resources.game.player_balance < 0) {  // player ran out of money
        // disable all overdrives and punish units
        for (unsigned int i = 0; i < VEC_LEN(gs->resources.overdrive.units); ++i) {
            int entity = vec_int_get(gs->resources.overdrive.units, i);
            UnitComponent* uc = vec_UnitComponent_get_ptr(gs->unit_components, entity);
            Health* h = vec_Health_get_ptr(gs->health_components, entity);
            uc->overdrive = false;
            h->points = 10;  // probably enough to just set it to 10 hp
            // it is *theoretically* possible that it would heal a unit
            // ... but if a player pulls that off, they should be rewarded for their ingenuity

            // overdrive vector will be cleaned up at the end of the function
        }
    }

    if(gs->resources.keys[ALLEGRO_KEY_E] & KEY_UNPROCESSED) {

        bool all_overdriven = true;
        printf("in overdrive\n");
        for (unsigned int i = 0; i < VEC_LEN(gs->resources.selection.entities_selected); ++i) {
            int entity = vec_int_get(gs->resources.selection.entities_selected, i);
            UnitComponent* uc = vec_UnitComponent_get_ptr(gs->unit_components, entity);
            if(uc->exists && uc->overdrive) {
            }
            else if(uc->exists){
                all_overdriven = false;
            }
        }
        printf("all_overdriven: %d\n", all_overdriven);

        if(all_overdriven) { // disable overdrives
            for (unsigned int i = 0; i < VEC_LEN(gs->resources.selection.entities_selected); ++i) {
                int entity = vec_int_get(gs->resources.selection.entities_selected, i);

                UnitComponent* uc = vec_UnitComponent_get_ptr(gs->unit_components, entity);
                if(!uc->exists) continue;

                uc->overdrive = false;
            }
        }
        else {  // add new units to overdrive
            for (unsigned int i = 0; i < VEC_LEN(gs->resources.selection.entities_selected); ++i) {
                int entity = vec_int_get(gs->resources.selection.entities_selected, i);

                UnitComponent* uc = vec_UnitComponent_get_ptr(gs->unit_components, entity);
                if(!uc->exists) continue;

                if(!uc->overdrive) {
                    uc->overdrive = true;
                    vec_int_push(gs->resources.overdrive.units, entity);
                }

            }
        }

    }
    // clean up the overdrive vector:
    // remove dead and non-overdriven units
    Vector_int new_overdrives =  vec_int_new();
    for (unsigned int i = 0; i < VEC_LEN(gs->resources.overdrive.units); ++i) {
        int entity = vec_int_get(gs->resources.overdrive.units, i);
        UnitComponent* uc = vec_UnitComponent_get_ptr(gs->unit_components, entity);
        bool alive =  vec_bool_get(gs->entities, entity);
        if(alive && uc->overdrive) {
            vec_int_push(new_overdrives, entity);
        }
    }
    vec_int_destroy(gs->resources.overdrive.units);
    gs->resources.overdrive.units = new_overdrives;
}

// copy pasted from enemy.c :/
Vec2 random_offset2() {
    int x = (rand() % 150) - 75;
    int y = (rand() % 150) - 75;
    return vec2_make((float)x, (float)y);
}

void buy_units(GameState* gs) {
    if(gs->resources.keys[ALLEGRO_KEY_Q] & KEY_UNPROCESSED) {
        if(gs->resources.game.player_balance < UNIT_COST) return;
        Transform t = vec_Transform_get(gs->transform_components, gs->resources.game.player_base);
        Vec2 spawn_point = vec2_add(t.position, vec2_make(gs->resources.config.level == 1 ? 350 : 180, 0));
        spawn_point = vec2_add(spawn_point, random_offset2());

        gs->resources.game.player_balance -= UNIT_COST;
        printf("player balance: %f\n", gs->resources.game.player_balance);
        create_unit(gs, spawn_point, PLAYER_TEAM);
    }
}

