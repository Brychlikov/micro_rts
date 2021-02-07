//
// Created by brych on 07/02/2021.
//

#include "tints.h"
#include "gamestate.h"
#include "sprite.h"
#include "health.h"

ALLEGRO_COLOR selected_tint;
ALLEGRO_COLOR enemy_tint;
ALLEGRO_COLOR overdrive_tint;
ALLEGRO_COLOR selected_overdrive_tint;

void init_tints(GameState* gs) {
    selected_tint = al_map_rgba_f(0.5, 1, 0.5, 1);
    enemy_tint = al_map_rgba_f(1, 0.5, 0.5, 1);
    overdrive_tint = al_map_rgba_f(1, 1, 0.3, 1);
    selected_overdrive_tint = al_map_rgba_f(0.7, 1, 0.3, 1);
}

void tint_sprites(GameState *gs) {
    for (int entity = 0; entity < VEC_LEN(gs->entities); ++entity) {
        Sprite* s = vec_Sprite_get_ptr(gs->sprite_components, entity);
        if(!s->exists) continue;
        Health h = vec_Health_get(gs->health_components, entity);
        UnitComponent uc = vec_UnitComponent_get(gs->unit_components, entity);

        s->tint = pure_tint;

        if(h.exists && h.team == ENEMY_TEAM) {
            s->tint = enemy_tint;
        }

        if(uc.exists) {
            bool selected = vec_int_contains(gs->resources.selection.entities_selected, entity);
            bool overdriven = uc.overdrive;
            if(selected && overdriven) {
                s->tint = selected_overdrive_tint;
            }
            else if(selected) {
                s->tint = selected_tint;
            } else if(overdriven) {
                s->tint = overdrive_tint;
            }
        }
    }
}
