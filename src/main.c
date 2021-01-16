#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <stdbool.h>
#include <stdio.h>
#include "keyboard.h"
#include "coord_utils.h"
#include "gamestate.h"
#include "selection_ui.h"
#include "mouse.h"

#define FUNARR_LEN(arr) (sizeof(arr)/sizeof(size_t))

typedef void (*PURE_SYSTEM)(GameState*);
typedef void (*EVENT_SYSTEM)(GameState*, union ALLEGRO_EVENT);


void misc_allegro_init(GameState* gs) {
    al_init();
    al_init_primitives_addon();

    gs->timer = al_create_timer(1.0 / 30.0);
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
    al_clear_to_color(al_map_rgb(50, 50, 80));
}


PURE_SYSTEM init_fns[] = {
        misc_allegro_init,
        init_keyboard,
        init_mouse,
};

EVENT_SYSTEM event_fns[] = {
        update_mouse,
        update_keys,
        should_exit,
        should_redraw,
        selection_system,

};

PURE_SYSTEM redraw_fns[] = {
        prep_redraw,
        draw_selection_area,

        reset_keys, // this should be last
};

PURE_SYSTEM deinit_fns[] = {
        misc_allegro_deinit
};



int main()
{

    GameState gs = {.during_selection=false, .selection_area={.tl={.x=0, .y=0}, .br={.x=100, .y=100}}};

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