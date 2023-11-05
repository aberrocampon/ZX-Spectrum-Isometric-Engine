#ifndef GAME_ISOMETRIC_OBJECTS_H
#define GAME_ISOMETRIC_OBJECTS_H

#include "isometric_system_tests.h"

extern byte graph_bin_def_ghost_moving_s_w[];
extern byte graph_bin_def_ghost_moving_n_e[];

#define GAME_SPRITE_FIRST_FRAME_GHOST_MOVING_S_W (graph_bin_def_ghost_moving_s_w)
#define GAME_SPRITE_LAST_FRAME_GHOST_MOVING_S_W (graph_bin_def_ghost_moving_s_w + 2*(120 + 3))

#define GAME_SPRITE_FIRST_FRAME_GHOST_MOVING_N_E (graph_bin_def_ghost_moving_n_e)
#define GAME_SPRITE_LAST_FRAME_GHOST_MOVING_N_E (graph_bin_def_ghost_moving_n_e + 2*(120 + 3))

extern byte graph_bin_def_human_feet_moving_s_w[];
extern byte graph_bin_def_human_feet_moving_n_e[];

#define GAME_SPRITE_FIRST_FRAME_HUMAN_FEET_MOVING_S_W (graph_bin_def_human_feet_moving_s_w)
#define GAME_SPRITE_LAST_FRAME_HUMAN_FEET_MOVING_S_W (graph_bin_def_human_feet_moving_s_w + 6*(96 + 3))

#define GAME_SPRITE_FIRST_FRAME_HUMAN_FEET_MOVING_N_E (graph_bin_def_human_feet_moving_n_e)
#define GAME_SPRITE_LAST_FRAME_HUMAN_FEET_MOVING_N_E (graph_bin_def_human_feet_moving_n_e + 6*(96 + 3))

extern byte graph_bin_def_guardian_trunk_moving_s_w[];
extern byte graph_bin_def_guardian_trunk_moving_n_e[];

#define GAME_SPRITE_FIRST_FRAME_GUARDIAN_TRUNK_MOVING_S_W (graph_bin_def_guardian_trunk_moving_s_w)
#define GAME_SPRITE_LAST_FRAME_GUARDIAN_TRUNK_MOVING_S_W (graph_bin_def_guardian_trunk_moving_s_w + 1*(6*23 + 3))

#define GAME_SPRITE_FIRST_FRAME_GUARDIAN_TRUNK_MOVING_N_E (graph_bin_def_guardian_trunk_moving_n_e)
#define GAME_SPRITE_LAST_FRAME_GUARDIAN_TRUNK_MOVING_N_E (graph_bin_def_guardian_trunk_moving_n_e + 1*(6*23 + 3))

/***********************************************************************/
// Tabla de objetos del juego

extern t_isometric_obj_graphic_def game_isometric_objects_table[];

#define GAME_ISOMETRIC_OBJ_TYPE_BLOCK (0)
#define GAME_ISOMETRIC_OBJ_TYPE_GHOST (1)
#define GAME_ISOMETRIC_OBJ_TYPE_GUARDIAN_BOX3D (2)
#define GAME_ISOMETRIC_OBJ_TYPE_HUMAN_FEET (3)
#define GAME_ISOMETRIC_OBJ_TYPE_GUARDIAN_TRUNK (4)
#define GAME_ISOMETRIC_OBJ_TYPE_GATE_DINTEL_N_S (5)
#define GAME_ISOMETRIC_OBJ_TYPE_STONE_GATE_LEFT_N_S (6)
#define GAME_ISOMETRIC_OBJ_TYPE_STONE_GATE_RIGHT_N_S (7)
#define GAME_ISOMETRIC_OBJ_TYPE_GATE_DINTEL_E_W (8)
#define GAME_ISOMETRIC_OBJ_TYPE_STONE_GATE_LEFT_E_W (9)
#define GAME_ISOMETRIC_OBJ_TYPE_STONE_GATE_RIGHT_E_W (10)

// Sprites para el fondo, es decir paredes de las habitaciones
extern t_sprite_graphic_def game_spr_graph_def_stone_corner_nw;
extern t_sprite_graphic_def game_spr_graph_def_stone_corner_ne_1;
extern t_sprite_graphic_def game_spr_graph_def_stone_corner_ne_2;

#endif