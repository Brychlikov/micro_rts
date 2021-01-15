cmake_minimum_required(VERSION 3.17)
project(_rts C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)

pkg_check_modules(ALLEGRO REQUIRED allegro-5 allegro_font-5 allegro_primitives-5)
include_directories(${ALLEGRO_INCLUDE_DIRS})
link_directories(${ALLEGRO_LIBRARY_DIRS})

add_definitions(${ALLEGRO_CFLAGS_OTHER})

add_executable(_rts src/main.c src/keyboard.c src/keyboard.h src/gamestate.h src/coord_utils.h src/selection_ui.c src/selection_ui.h)

target_link_libraries(_rts ${ALLEGRO_LIBRARIES})
