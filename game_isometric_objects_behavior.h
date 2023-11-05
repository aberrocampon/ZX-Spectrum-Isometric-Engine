#ifndef GAME_ISOMETRIC_OBJECTS_BEHAVIOR_H
#define GAME_ISOMETRIC_OBJECTS_BEHAVIOR_H

#include "isometric_system_tests.h"

void behavior_player_init(t_isometric_obj **pp_isometric_obj_player);
void behavior_controller_player(t_isometric_obj **pp_isometric_obj_player);
void behavior_cinematic_updown(t_isometric_obj **pp_isometric_obj);
void behavior_gate_init(t_isometric_obj **pp_isometric_obj);

#endif