//
// Created by brych on 07/02/2021.
//

#include "tints.h"
#include "gamestate.h"
#include "sprite.h"
#include "health.h"
#include "colors.h"

ALLEGRO_COLOR selected_tint;
ALLEGRO_COLOR enemy_tint;
ALLEGRO_COLOR overdrive_tint;
ALLEGRO_COLOR selected_overdrive_tint;

ALLEGRO_COLOR color_code_to_allegro(const char *s) {
    int combined_color;
    combined_color = (int)strtol(s, NULL, 16);
    int last_byte_mask = 255;
    int blue = combined_color & last_byte_mask;
    combined_color >>= 8;
    int green = combined_color & last_byte_mask;
    combined_color >>= 8;
    int red = combined_color;
    return al_map_rgb(red, green, blue);
}

void init_tints(__attribute__((unused)) GameState* gs) {
    selected_tint = color_code_to_allegro(SELECTED_COLOR);
    enemy_tint = color_code_to_allegro(ENEMY_COLOR);
    overdrive_tint = color_code_to_allegro(OVERDRIVE_COLOR);
    selected_overdrive_tint = color_code_to_allegro(SELECTED_OVERDRIVE_COLOR);
}

void tint_sprites(GameState *gs) {
    for (unsigned int entity = 0; entity < VEC_LEN(gs->entities); ++entity) {
        Sprite* s = vec_Sprite_get_ptr(gs->sprite_components, entity);
        if(!s->exists) continue;
        Health h = vec_Health_get(gs->health_components, entity);
        BulletComponent bc = vec_BulletComponent_get(gs->bullet_components, entity);
        UnitComponent uc = vec_UnitComponent_get(gs->unit_components, entity);

        s->tint = pure_tint;

        if((h.exists && h.team == ENEMY_TEAM)|| (bc.exists && bc.team == ENEMY_TEAM)) {
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
