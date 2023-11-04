#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "isometric_system_tests.h"

typedef struct
{
    byte exits;
    byte room_to_n;
    byte room_to_s;
    byte room_to_e;
    byte room_to_w;
    byte n_isometric_objects;
    t_isometric_object_def *p_game_map_room_isometric_objects;
} t_game_map_room;

#define GAME_MAP_ROOM_FLAG_EXIT_N (1)
#define GAME_MAP_ROOM_FLAG_EXIT_S (2)
#define GAME_MAP_ROOM_FLAG_EXIT_E (4)
#define GAME_MAP_ROOM_FLAG_EXIT_W (8)

extern t_isometric_object_def player_isometric_objects[];
extern t_game_map_room game_map_rooms_table[];
extern byte game_map_next_room;
extern byte game_map_actual_room;

#define game_map_move_player_2_room(room_idx, init_x, init_y, init_z)  \
        { \
            game_map_next_room = room_idx; \
            player_isometric_objects[0].init_pos.x = init_x; \
            player_isometric_objects[0].init_pos.y = init_y; \
            player_isometric_objects[0].init_pos.z = init_z; \
        }

void game_map_enter_room(void);
void game_map_step(void);
void game_map_init(void);

#endif