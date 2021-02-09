//
// Created by brych on 20/01/2021.
//

#include "sprite.h"
#include "tints.h"
#include "colors.h"
#include "gamestate.h"

GENERATE_VECTOR_DEFINITION(Sprite)

ALLEGRO_COLOR pure_tint;

void init_sprites(GameState* gs) {
    pure_tint = color_code_to_allegro(PLAYER_COLOR);
//    pure_tint = al_map_rgba(255, 255, 255, 255);
    gs->sprite_components = vec_Sprite_new();
    gs->resources.sprites.unit = al_load_bitmap("assets/unit.png");
    if (gs->resources.sprites.unit == NULL) {
        fprintf(stderr, "Could not load unit sprite\n");
    }
    gs->resources.sprites.building = al_load_bitmap("assets/building.png");
    if(gs->resources.sprites.building == NULL) {
        fprintf(stderr, "Could not load building sprite\n");
    }

    gs->resources.sprites.bullet = al_load_bitmap("assets/bullet.png");
    if(gs->resources.sprites.bullet == NULL) {
        fprintf(stderr, "Could not load bullet sprite\n");
    }

    gs->resources.sprites.bullet_turret = al_load_bitmap("assets/bullet_turret.png");
    if(gs->resources.sprites.bullet_turret == NULL) {
        fprintf(stderr, "Could not load bullet turret sprite\n");
    }

    gs->resources.sprites.laser_turret = al_load_bitmap("assets/laser_turret.png");
    if(gs->resources.sprites.laser_turret == NULL) {
        fprintf(stderr, "Could not load laser turret sprite\n");
    }

}

void deinit_sprites(GameState *gs) {
    vec_Sprite_destroy(gs->sprite_components);
    al_destroy_bitmap(gs->resources.sprites.unit);
    al_destroy_bitmap(gs->resources.sprites.building);
    al_destroy_bitmap(gs->resources.sprites.bullet);
    al_destroy_bitmap(gs->resources.sprites.bullet_turret);
    al_destroy_bitmap(gs->resources.sprites.laser_turret);
}

void draw_sprites(GameState* gs) {
    for (unsigned int i = 0; i < VEC_LEN(gs->sprite_components); ++i) {
        // for now assert that every entity with sprite has a valid transform
        Sprite s = vec_Sprite_get(gs->sprite_components, i);
        if(vec_bool_get(gs->entities, i)) {  // if entity still alive
            Transform t = vec_Transform_get(gs->transform_components, i);
            al_draw_tinted_scaled_rotated_bitmap(s.bitmap, s.tint, s.offset.x, s.offset.y, t.position.x, t.position.y, t.scale, t.scale, t.rotation + s.rotation, 0);    // disregard scale for now
        }
    }
}