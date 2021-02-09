//
// Created by brych on 06/02/2021.
//

#include "health_bars.h"
#include "health.h"
#include "gamestate.h"
#include "coord_utils.h"
#include "vector.h"
#include "allegro5/allegro_primitives.h"
#include "tints.h"
#include "colors.h"

void draw_healthbars(GameState *gs) {
    for (int entity = 0; entity < VEC_LEN(gs->entities); ++entity) {
        bool alive = vec_bool_get(gs->entities, entity);
        Transform t = vec_Transform_get(gs->transform_components, entity);
        Health h = vec_Health_get(gs->health_components, entity);
        Sprite s = vec_Sprite_get(gs->sprite_components, entity);
        if(alive && t.exists && h.exists && s.exists) {
            float bar_width = (float)al_get_bitmap_width(s.bitmap) * t.scale;
            float bar_height = 10 * t.scale;
            float filled = bar_width * (float)h.points / (float)h.max_points;
            float x = t.position.x - 50 * t.scale;
            float y = t.position.y + 50 * t.scale  ;
            al_draw_filled_rectangle(x, y, x + bar_width, y + bar_height, color_code_to_allegro(HP_BAR_BACKGROUND));
            al_draw_filled_rectangle(x, y, x + filled, y + bar_height, color_code_to_allegro(HP_BAR_FOREGROUND));
        }
    }
}
