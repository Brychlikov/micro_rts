#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <stdbool.h>
#include <stdio.h>
#include "keyboard.h"
#include "coord_utils.h"
#include "gamestate.h"
#include "unit.h"
#include "selection_ui.h"
#include "mouse.h"
#include "sprite.h"
#include "collider.h"
#include "building.h"
#include "laser_turret.h"
#include "enemy.h"

#define FUNARR_LEN(arr) (sizeof(arr)/sizeof(size_t))

typedef void (*PURE_SYSTEM)(GameState*);
typedef void (*EVENT_SYSTEM)(GameState*, union ALLEGRO_EVENT);


void misc_allegro_init(GameState* gs) {
    al_init();
    al_init_primitives_addon();
    al_init_image_addon();
    al_hold_bitmap_drawing(true);

    gs->timer = al_create_timer(1.0 / 60.0);
    gs->queue = al_create_event_queue();
    gs->display = al_create_display(1280, 720);
    gs->font = al_create_builtin_font();

    al_register_event_source(gs->queue, al_get_display_event_source(gs->display));
    al_register_event_source(gs->queue, al_get_timer_event_source(gs->timer));

    al_start_timer(gs->timer);
}

void misc_allegro_deinit(GameState* gs) {
    al_destroy_font(gs->font);
    al_destroy_display(gs->display);
    al_destroy_timer(gs->timer);
    al_destroy_event_queue(gs->queue);
}

void should_exit(GameState* gs, ALLEGRO_EVENT event) {
    if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        gs->should_exit = true;
    }
}

void should_redraw(GameState* gs, ALLEGRO_EVENT event) {
    if(event.type == ALLEGRO_EVENT_TIMER) {
        gs->redraw = true;
    }
}

void prep_redraw(GameState* gs) {
    gs->redraw = false;
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC_RAW, &now);

    long usecs_elapsed = (now.tv_sec - gs->resources.last_frame_timestamp.tv_sec) * 1000000 + \
        (now.tv_nsec - gs->resources.last_frame_timestamp.tv_nsec) / 1000;

    if(gs->resources.last_frame_timestamp.tv_sec == 0) {  // first frame of the game
        gs->resources.time_delta = 1.0/60;
    }
    else {
        gs->resources.time_delta = (float)usecs_elapsed / 1000000;
    }
    gs->resources.last_frame_timestamp = now;
    al_clear_to_color(al_map_rgba(0, 0, 0, 255));
}

void print_collisions(GameState* gs) {
    for (int i = 0; i < VEC_LEN(gs->resources.collisions.vec); ++i) {
        CollisionData* cd = vec_CollisionData_get_ptr(gs->resources.collisions.vec, i);
        if(cd->entity == -1) continue;
        printf("Collisions of entity %d:\n", i);
        while(cd != NULL && cd->entity != -1) {
            printf("\tentity %d\n", cd->entity);
            cd = cd->next;
        }
        printf("\n");
    }
}

PURE_SYSTEM init_fns[] = {
        misc_allegro_init,

        init_keyboard,
        init_mouse,

        init_components,
        init_selection_system,
        init_colliders,
        init_health,
        init_sprites,
        init_bullets,
        init_laser_turrets,
        init_units,
        init_enemy,
        init_game,
};

EVENT_SYSTEM event_fns[] = {
        update_mouse,
        update_keys,
        should_exit,
        should_redraw,
};

PURE_SYSTEM redraw_fns[] = {
        prep_redraw,
        selection_system,
        draw_selection_area,
        check_collisions,
//        print_collisions,
        advance_units,
        process_laser_turrets,
        process_bullets,
        health_system,
        process_enemy,
        process_income,
        buy_units,
        process_overdrive,

#ifdef DEBUG_COLLIDERS
        draw_colliders,
#endif
//        draw_units,
        draw_sprites,
        command_units,
        render_balance,

        reset_keys, // these should always be last
        reset_mouse_buttons,
};

PURE_SYSTEM deinit_fns[] = {
        misc_allegro_deinit
};



int main()
{

    GameState gs;
    memset(&gs, 0, sizeof (GameState));  // zero the gamestate to make debugging uninitialized things easier
    gs.resources.selection.in_progress = false;
//    GameState gs = {.during_selection=false, .selection_area={.tl={.x=0, .y=0}, .br={.x=100, .y=100}}};

    for (int i = 0; i < FUNARR_LEN(init_fns); ++i) {
        init_fns[i](&gs);
    }

    gs.redraw = true;

    ALLEGRO_EVENT event;

    while(!gs.should_exit)
    {
        al_wait_for_event(gs.queue, &event);

        for (int i = 0; i < FUNARR_LEN(event_fns); ++i) {
            event_fns[i](&gs, event);
        }

        if(gs.redraw && al_is_event_queue_empty(gs.queue))
        {

            for (int i = 0; i < FUNARR_LEN(redraw_fns); ++i) {
                redraw_fns[i](&gs);
            }


            al_flip_display();

        }
    }

    for (int i = 0; i < FUNARR_LEN(deinit_fns); ++i) {
        deinit_fns[i](&gs);
    }

    return 0;
}