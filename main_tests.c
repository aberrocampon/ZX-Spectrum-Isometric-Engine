
/*
 * 
 * To build:
 * 	zcc +zx -m -create-app .\isometricos_tests.c .\sprites_mask_tests.c .\precalculated_shift_tables.asm
 * 
 */

#pragma output CRT_ORG_CODE          = 0x7700

#include "keyboard.h"
#include "game_isometric_objects.h"
#include "game_isometric_objects_behavior.h"

/*******************************************************************************************************/

t_b_vec3d pos_block_0 = { 70, 30, 54 };
t_b_vec3d pos_block_1 = { 50, 50, 35 };
t_b_vec3d pos_block_2 = { 40, 50, 20 };
t_b_vec3d pos_block_3 = { 70, 75, 40 };
t_b_vec3d pos_ghost   = { 70, 30, 5 };
t_b_vec3d pos_guardian_box3d = { 70, 30, 23 };
t_b_vec3d pos_guardian_trunk = { 70, 30, 26 };
t_b_vec3d pos_human_feet   = { 70, 30, 13 };

/*******************************************************************************************************/

extern t_isometric_obj isometric_objects_table[];

void main()
{
	byte i;

	sprite_init();
	sprite_transfer_vdisplay();

	isometric_reset_table();
	isometric_add_object_to_table(isometric_create_object(&game_isometric_objects_table[GAME_ISOMETRIC_OBJ_TYPE_BLOCK], &pos_block_2, 1, PHYS_BOX3D_FLAG_CINEMATIC, behavior_cinematic_updown, 0));
	isometric_add_object_to_table(isometric_create_object(&game_isometric_objects_table[GAME_ISOMETRIC_OBJ_TYPE_BLOCK], &pos_block_3, 1, PHYS_BOX3D_FLAG_CINEMATIC, behavior_cinematic_updown, 0));
	isometric_add_object_to_table(isometric_create_object(&game_isometric_objects_table[GAME_ISOMETRIC_OBJ_TYPE_BLOCK], &pos_block_0, 1, 0, NULL, 0));
	isometric_add_object_to_table(isometric_create_object(&game_isometric_objects_table[GAME_ISOMETRIC_OBJ_TYPE_BLOCK], &pos_block_1, 1, 0, NULL, 0));

	isometric_add_object_to_table(isometric_create_object(&game_isometric_objects_table[GAME_ISOMETRIC_OBJ_TYPE_GUARDIAN_BOX3D], &pos_guardian_box3d, 1, 0, behavior_controller_player, 0));
	isometric_add_object_to_table(isometric_create_object(&game_isometric_objects_table[GAME_ISOMETRIC_OBJ_TYPE_GUARDIAN_TRUNK], &pos_guardian_trunk, 0, 0, NULL, 0));
	isometric_add_object_to_table(isometric_create_object(&game_isometric_objects_table[GAME_ISOMETRIC_OBJ_TYPE_HUMAN_FEET], &pos_human_feet, 0, 0, NULL, 0));

	// En estos momentos (18-09-2023) se observa aprox con 10 sprites en el bucle while(1)
	// 256 refrescos en 22 segundos =>
	// cada refresco de 10 sprites completos 86 ms, 8.6ms por sprite
	// 11.6 cuadros por segundo

	while(1)//for(nframes = 256; nframes>=0; nframes--)
	{

		keyboard_readrow_54321();
		keyboard_readrow_trewq();

		if(keyboard_is_key_pressed_t()) break;

		isometric_step();
	}

}

