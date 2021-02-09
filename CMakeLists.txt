cmake_minimum_required(VERSION 3.16)
project(micro_rts C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)

# add_compile_definitions(DEBUG_COLLIDERS)

pkg_check_modules(ALLEGRO REQUIRED allegro-5 allegro_font-5 allegro_primitives-5 allegro_image-5)
include_directories(${ALLEGRO_INCLUDE_DIRS})
link_directories(${ALLEGRO_LIBRARY_DIRS})

add_definitions(${ALLEGRO_CFLAGS_OTHER})

add_executable(micro_rts
        src/main.c
        src/keyboard.c src/keyboard.h
        src/gamestate.h src/gamestate.c
        src/coord_utils.h src/coord_utils.c
        src/selection_ui.c src/selection_ui.h
        src/mouse.c src/mouse.h
        src/unit.c src/unit.h
        src/vector.c src/vector.h
        src/sprite.c src/sprite.h
        src/collider.h src/collider.c
        src/gamestate_forward.h src/health.c src/health.h src/building.c src/building.h src/bullet.c src/bullet.h src/enemy.c src/enemy.h src/laser_turret.c src/laser_turret.h src/health_bars.c src/health_bars.h src/tints.c src/tints.h src/colors.h)

target_link_libraries(micro_rts ${ALLEGRO_LIBRARIES} m)
