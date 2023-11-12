
/*
 * 
 * To build:
 * 	zcc +zx -m -create-app .\isometricos_tests.c .\sprites_mask_tests.c .\precalculated_shift_tables.asm
 * 
 */

#pragma output CRT_ORG_CODE          = 0x7000

#include "keyboard.h"
#include "game_isometric_objects.h"
#include "game_isometric_objects_behavior.h"
#include "game_map.h"

void main()
{
	byte i;

	sprite_init();
	game_map_init();

	while(1)
	{

		keyboard_readrow_54321();
		keyboard_readrow_trewq();

		if(keyboard_is_key_pressed_t()) break;

		isometric_step();
		game_map_step();
	}

}

