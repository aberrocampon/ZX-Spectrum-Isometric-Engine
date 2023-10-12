
/*
 * 
 * To build:
 * 	zcc +zx -m -create-app .\isometricos_tests.c .\sprites_mask_tests.c .\precalculated_shift_tables.asm
 * 
 */

#pragma output CRT_ORG_CODE          = 0x8000

#include "keyboard.h"
#include "isometric_system_tests.h"

/*******************************************************************************************************/

// imagen binaria y mascara en bytes alternos
byte graph_bin_def_block[] =
	{
		SPRITE_GRAPHIC_STATE_FLIPPED_RIGHT, // graphic state mask
		0x00, 0xFF, 0x00, 0xFE, 0x00, 0x7F, 0x00, 0xFF,
		0x00, 0xFF, 0x01, 0xF8, 0x80, 0x1F, 0x00, 0xFF,
		0x00, 0xFF, 0x07, 0xE0, 0xE0, 0x07, 0x00, 0xFF,
		0x00, 0xFF, 0x1F, 0x80, 0xF8, 0x01, 0x00, 0xFF,
		0x00, 0xFE, 0x7F, 0x00, 0xFE, 0x00, 0x00, 0x7F,
		0x01, 0xF8, 0xFF, 0x00, 0xFF, 0x00, 0x80, 0x1F,
		0x07, 0xE0, 0xFF, 0x00, 0xFF, 0x00, 0xE0, 0x07,
		0x1F, 0x80, 0xFF, 0x00, 0xFF, 0x00, 0xF8, 0x01,
		0x7F, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFE, 0x00,
		0x5F, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFA, 0x00,
		0x67, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xE4, 0x00,
		0x79, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x8A, 0x00,
		0x7E, 0x00, 0x7F, 0x00, 0xFE, 0x00, 0x54, 0x00,
		0x7F, 0x00, 0x9F, 0x00, 0xF8, 0x00, 0xAA, 0x00,
		0x7F, 0x00, 0xE7, 0x00, 0xE5, 0x00, 0x54, 0x00,
		0x7F, 0x00, 0xF9, 0x00, 0x8A, 0x00, 0xAA, 0x00,
		0x7F, 0x00, 0xFE, 0x00, 0x55, 0x00, 0x54, 0x00,
		0x7F, 0x00, 0xFE, 0x00, 0xAA, 0x00, 0xAA, 0x00,
		0x7F, 0x00, 0xFE, 0x00, 0x55, 0x00, 0x54, 0x00,
		0x7F, 0x00, 0xFE, 0x00, 0xAA, 0x00, 0xAA, 0x00,
		0x1F, 0x80, 0xFE, 0x00, 0x55, 0x00, 0x50, 0x01,
		0x07, 0xE0, 0xFE, 0x00, 0xAA, 0x00, 0x80, 0x07,
		0x01, 0xF8, 0xFE, 0x00, 0x55, 0x00, 0x00, 0x1F,
		0x00, 0xFE, 0x7E, 0x00, 0xAA, 0x00, 0x00, 0x7F,
		0x00, 0xFF, 0x1E, 0x80, 0x50, 0x01, 0x00, 0xFF,
		0x00, 0xFF, 0x06, 0xE0, 0xA0, 0x07, 0x00, 0xFF,
		0x00, 0xFF, 0x01, 0xF8, 0x80, 0x1F, 0x00, 0xFF,
		0x00, 0xFF, 0x00, 0xFE, 0x00, 0x7F, 0x00, 0xFF
	};

byte graph_bin_def_ghost[] =
	{
		// frame 0-0. Direccion sur-oeste
		SPRITE_GRAPHIC_STATE_FLIPPED_LEFT, // graphic state mask
		0x00, 0xFF, 0x00, 0xC3, 0x00, 0xFF,
		0x00, 0xFF, 0x3C, 0x00, 0x00, 0xFF,
		0x00, 0xFE, 0xFF, 0x00, 0x00, 0x7F,
		0x01, 0xFC, 0xFF, 0x00, 0x80, 0x3F,
		0x01, 0xFC, 0xFF, 0x00, 0xC0, 0x1F,
		0x03, 0xF8, 0x3F, 0x00, 0xE0, 0x0F,
		0x02, 0xF8, 0xC7, 0x00, 0xE0, 0x0F,
		0x06, 0xF0, 0x9B, 0x00, 0xF0, 0x07,
		0x07, 0xF0, 0x0B, 0x00, 0xF0, 0x07,
		0x0F, 0xE0, 0xE7, 0x00, 0xF8, 0x03,
		0x1F, 0xC0, 0xFF, 0x00, 0xFC, 0x01,
		0x3F, 0x80, 0xFF, 0x00, 0xFC, 0x01,
		0x7F, 0x00, 0xFF, 0x00, 0xFE, 0x00,
		0x7F, 0x00, 0xFF, 0x00, 0xFE, 0x00,
		0x03, 0x80, 0xFF, 0x00, 0x66, 0x00,
		0x01, 0xFC, 0xFE, 0x00, 0x00, 0x19,
		0x01, 0xFC, 0xFF, 0x00, 0x00, 0x7F,
		0x00, 0xFE, 0x3F, 0x00, 0x00, 0x7F,
		0x00, 0xFF, 0x1E, 0xC0, 0x00, 0xFF,
		0x00, 0xFF, 0x00, 0xE1, 0x00, 0xFF,
		// frame 0-1. Direccion sur-oeste
		SPRITE_GRAPHIC_STATE_FLIPPED_LEFT, // graphic state mask
		0x00, 0xFF, 0x00, 0xC3, 0x00, 0xFF,
		0x00, 0xFE, 0x3C, 0x00, 0x00, 0x7F,
		0x01, 0xFC, 0xFF, 0x00, 0x80, 0x1F,
		0x03, 0xF8, 0xFF, 0x00, 0xE0, 0x0F,
		0x07, 0xF0, 0x3F, 0x00, 0xF0, 0x07,
		0x06, 0xF0, 0xC7, 0x00, 0xF0, 0x07,
		0x06, 0xF0, 0x9B, 0x00, 0xF0, 0x07,
		0x0F, 0xE0, 0x0B, 0x00, 0xF8, 0x03,
		0x1F, 0xC0, 0xE7, 0x00, 0xF8, 0x03,
		0x3F, 0x80, 0xFF, 0x00, 0xFC, 0x01,
		0x7F, 0x00, 0xFF, 0x00, 0xFE, 0x00,
		0x7F, 0x00, 0xFF, 0x00, 0xFE, 0x00,
		0x07, 0x80, 0xFF, 0x00, 0xFE, 0x00,
		0x07, 0xF0, 0xFF, 0x00, 0xF8, 0x01,
		0x0F, 0xE0, 0xFF, 0x00, 0xF8, 0x03,
		0x07, 0xF0, 0x3F, 0x00, 0x38, 0x03,
		0x00, 0xF8, 0x1E, 0xC0, 0x00, 0xC7,
		0x00, 0xFF, 0x1C, 0xC1, 0x00, 0xFF,
		0x00, 0xFF, 0x00, 0xE3, 0x00, 0xFF,
		0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
		// frame 1-0. Direccion nor-este
		SPRITE_GRAPHIC_STATE_FLIPPED_RIGHT, // graphic state mask
		0x00, 0xFF, 0x00, 0xC3, 0x00, 0xFF,
		0x00, 0xFF, 0x3C, 0x00, 0x00, 0xFF,
		0x00, 0xFE, 0xFF, 0x00, 0x00, 0x7F,
		0x01, 0xFC, 0xFF, 0x00, 0x80, 0x3F,
		0x01, 0xFC, 0xFF, 0x00, 0xC0, 0x1F,
		0x03, 0xF8, 0xFF, 0x00, 0x20, 0x0F,
		0x03, 0xF8, 0xFF, 0x00, 0x20, 0x0F,
		0x07, 0xF0, 0xFF, 0x00, 0x50, 0x07,
		0x07, 0xF0, 0xFF, 0x00, 0x98, 0x03,
		0x0F, 0xE0, 0xFF, 0x00, 0xF8, 0x03,
		0x1F, 0xC0, 0xFF, 0x00, 0xFC, 0x01,
		0x3F, 0x80, 0xFF, 0x00, 0xFE, 0x00,
		0x7F, 0x00, 0xFF, 0x00, 0xFE, 0x00,
		0x7F, 0x00, 0xFF, 0x00, 0xFC, 0x01,
		0x3F, 0x80, 0xFF, 0x00, 0xF0, 0x03,
		0x0F, 0xC0, 0xFF, 0x00, 0xF0, 0x07,
		0x07, 0xF0, 0xFF, 0x00, 0xF0, 0x07,
		0x03, 0xF8, 0x1F, 0x00, 0x00, 0x0F,
		0x00, 0xFC, 0x06, 0xE0, 0x00, 0xFF,
		0x00, 0xFF, 0x00, 0xF9, 0x00, 0xFF,
		// frame 1-1. Direccion nor-este
		SPRITE_GRAPHIC_STATE_FLIPPED_RIGHT, // graphic state mask
		0x00, 0xFF, 0x00, 0xC3, 0x00, 0xFF,
		0x00, 0xFF, 0x3C, 0x00, 0x00, 0xFF,
		0x00, 0xFE, 0xFF, 0x00, 0x00, 0x7F,
		0x01, 0xFC, 0xFF, 0x00, 0x80, 0x3F,
		0x01, 0xFC, 0xFF, 0x00, 0xC0, 0x1F,
		0x03, 0xF8, 0xFF, 0x00, 0x20, 0x0F,
		0x03, 0xF8, 0xFF, 0x00, 0x20, 0x0F,
		0x07, 0xF0, 0xFF, 0x00, 0x50, 0x07,
		0x07, 0xF0, 0xFF, 0x00, 0x98, 0x03,
		0x0F, 0xE0, 0xFF, 0x00, 0xF8, 0x03,
		0x1F, 0xC0, 0xFF, 0x00, 0xFC, 0x01,
		0x3F, 0x80, 0xFF, 0x00, 0xFE, 0x00,
		0x7F, 0x00, 0xFF, 0x00, 0xFE, 0x00,
		0x3F, 0x80, 0xFF, 0x00, 0xFC, 0x01,
		0x0F, 0xC0, 0xFF, 0x00, 0xF8, 0x03,
		0x0F, 0xE0, 0x3F, 0x00, 0xF8, 0x03,
		0x1E, 0xC0, 0x3F, 0x80, 0x30, 0x07,
		0x00, 0xE1, 0x3F, 0x80, 0x00, 0x0F,
		0x00, 0xFF, 0x1E, 0xC0, 0x00, 0xFF,
		0x00, 0xFF, 0x00, 0xE1, 0x00, 0xFF
	};

t_sprite_graphic_def spr_graph_def_block = 
	{	
		4, 28, // 4x character cells in width, 28 scan lines in height
		-15, -13,
		224 + 1,
		224 + 1,
		graph_bin_def_block
	};

t_sprite_graphic_def spr_graph_def_ghost = 
	{	
		3, 20, // 4x character cells in width, 28 scan lines in height
		-11, -10,
		120 + 1,
		4*(120 + 1),
		graph_bin_def_ghost
	};

t_isometric_obj isometric_block_0 =
	{
		{
			{
				30, 30, 50,
				8, 8, 6
			},
			0, 0, 0
		},
		{
			0, // graphic state mask
			0, 0,
			0, 0, // deltax, deltay
			0, 255, // ultima posicion en que fue dibujado
			0, 0, // posicion para actualizar buffer de display visible
			0, 0, // ancho y alto para actualizar buffer de display visible
			0, 0, // 4x character cells in width, 28 scan lines in height
			0, // frame size
			NULL,
			NULL,
			NULL
		}
	};

t_isometric_obj isometric_block_1 =
	{
		{
			{
				30, 30, 20,
				6, 6, 5
			},
			0, 0, 0
		},
		{
			0, // graphic state mask
			0, 0,
			0, 0, // deltax, deltay
			0, 255, // ultima posicion en que fue dibujado
			0, 0, // posicion para actualizar buffer de display visible
			0, 0, // ancho y alto para actualizar buffer de display visible
			0, 0, // 4x character cells in width, 28 scan lines in height
			0, // frame size
			NULL,
			NULL,
			NULL
		}
	};

/*******************************************************************************************************/

byte ghost_last_orientation = ISOMETRIC_ORIENTATION_S | ISOMETRIC_ORIENTATION_W;

void main()
{
	byte i, nframes;

	sprite_init();
	sprite_transfer_vdisplay();

	sprite_set_graphic_def(&isometric_block_0.sprite, &spr_graph_def_block, 0, spr_graph_def_block.total_frames_size);
	sprite_set_graphic_def(&isometric_block_1.sprite, &spr_graph_def_ghost, 0, spr_graph_def_ghost.frame_size + spr_graph_def_ghost.frame_size); // direccion sur-oeste

	init_phys_box3d_step();
	phys_box3d_add_object(&isometric_block_0.physics);
	phys_box3d_add_object(&isometric_block_1.physics);

	// En estos momentos (18-09-2023) se observa aprox con 10 sprites en el bucle while(1)
	// 256 refrescos en 22 segundos =>
	// cada refresco de 10 sprites completos 86 ms, 8.6ms por sprite
	// 11.6 cuadros por segundo

	nframes = 0;
	while(1)//for(nframes = 256; nframes>=0; nframes--)
	{
		nframes++;

		keyboard_readrow_54321();
		keyboard_readrow_trewq();

		if(keyboard_is_key_pressed_t()) break;

		if(keyboard_is_key_pressed_1()) 
		{
			if(isometric_block_1.physics.touch_flags & PHYS_BOX3D_TOUCH_FLAG_D)
			{
				isometric_block_1.physics.speed_y = isometric_block_1.physics.p_phys_obj_touching_d->speed_y - 1;
			}

			if(ghost_last_orientation != (ISOMETRIC_ORIENTATION_N | ISOMETRIC_ORIENTATION_W))
			{
				ghost_last_orientation = ISOMETRIC_ORIENTATION_N | ISOMETRIC_ORIENTATION_W;
				sprite_set_frames_subset(&(isometric_block_1.sprite), &spr_graph_def_ghost, spr_graph_def_ghost.frame_size + spr_graph_def_ghost.frame_size, spr_graph_def_ghost.total_frames_size);
				isometric_block_1.sprite.required_graphic_state = (isometric_block_1.sprite.required_graphic_state & (~SPRITE_GRAPHIC_STATE_MASK_FLIPPED_H)) | SPRITE_GRAPHIC_STATE_FLIPPED_LEFT;
			}
			else
			{
				if(!(nframes & 7)) sprite_next_frame(&(isometric_block_1.sprite));
			}
		}

		if(keyboard_is_key_pressed_q())
		{
			if(isometric_block_1.physics.touch_flags & PHYS_BOX3D_TOUCH_FLAG_D)
			{
				isometric_block_1.physics.speed_y = isometric_block_1.physics.p_phys_obj_touching_d->speed_y + 1;
			}

			if(ghost_last_orientation != (ISOMETRIC_ORIENTATION_S | ISOMETRIC_ORIENTATION_E))
			{
				ghost_last_orientation = ISOMETRIC_ORIENTATION_S | ISOMETRIC_ORIENTATION_E;
				sprite_set_frames_subset(&(isometric_block_1.sprite), &spr_graph_def_ghost, 0, spr_graph_def_ghost.frame_size + spr_graph_def_ghost.frame_size);
				isometric_block_1.sprite.required_graphic_state = (isometric_block_1.sprite.required_graphic_state & (~SPRITE_GRAPHIC_STATE_MASK_FLIPPED_H)) | SPRITE_GRAPHIC_STATE_FLIPPED_RIGHT;
			}
			else
			{
				if(!(nframes & 7)) sprite_next_frame(&(isometric_block_1.sprite));
			}
		}

		if(keyboard_is_key_pressed_3())
		{
			if(isometric_block_1.physics.touch_flags & PHYS_BOX3D_TOUCH_FLAG_D)
			{
				isometric_block_1.physics.speed_x = isometric_block_1.physics.p_phys_obj_touching_d->speed_x - 1;
			}

			if(ghost_last_orientation != (ISOMETRIC_ORIENTATION_N | ISOMETRIC_ORIENTATION_E))
			{
				ghost_last_orientation = ISOMETRIC_ORIENTATION_N | ISOMETRIC_ORIENTATION_E;
				sprite_set_frames_subset(&(isometric_block_1.sprite), &spr_graph_def_ghost, spr_graph_def_ghost.frame_size + spr_graph_def_ghost.frame_size, spr_graph_def_ghost.total_frames_size);
				isometric_block_1.sprite.required_graphic_state = (isometric_block_1.sprite.required_graphic_state & (~SPRITE_GRAPHIC_STATE_MASK_FLIPPED_H)) | SPRITE_GRAPHIC_STATE_FLIPPED_RIGHT;
			}
			else
			{
				if(!(nframes & 7)) sprite_next_frame(&(isometric_block_1.sprite));
			}
		}

		if(keyboard_is_key_pressed_2()) 
		{
			if(isometric_block_1.physics.touch_flags & PHYS_BOX3D_TOUCH_FLAG_D)
			{
				isometric_block_1.physics.speed_x = isometric_block_1.physics.p_phys_obj_touching_d->speed_x + 1;
			}

			if(ghost_last_orientation != (ISOMETRIC_ORIENTATION_S | ISOMETRIC_ORIENTATION_W))
			{
				ghost_last_orientation = ISOMETRIC_ORIENTATION_S | ISOMETRIC_ORIENTATION_W;
				sprite_set_frames_subset(&(isometric_block_1.sprite), &spr_graph_def_ghost, 0, spr_graph_def_ghost.frame_size + spr_graph_def_ghost.frame_size);
				isometric_block_1.sprite.required_graphic_state = (isometric_block_1.sprite.required_graphic_state & (~SPRITE_GRAPHIC_STATE_MASK_FLIPPED_H)) | SPRITE_GRAPHIC_STATE_FLIPPED_LEFT;
			}
			else
			{
				if(!(nframes & 7)) sprite_next_frame(&(isometric_block_1.sprite));
			}
		}

		if(keyboard_is_key_pressed_4()) 
		{
			if(isometric_block_1.physics.touch_flags & PHYS_BOX3D_TOUCH_FLAG_D)
			{
				isometric_block_1.physics.speed_z = 4;
			}

			if(!(nframes & 7)) sprite_next_frame(&(isometric_block_1.sprite));
		}

		if(keyboard_is_key_pressed_5()) 
		{
			if(!(nframes & 7)) sprite_next_frame(&(isometric_block_1.sprite));
		}

		phys_box3d_step();

		sprite_restore_vdisplay(&(isometric_block_0.sprite));
		sprite_restore_vdisplay(&(isometric_block_1.sprite));

		isometric_proj_obj(&isometric_block_0);
		isometric_proj_obj(&isometric_block_1);

		reset_isometric_objects_ordering();
		isometric_add_object_to_order(&isometric_block_0);
		isometric_add_object_to_order(&isometric_block_1);

		for(i=0; i < n_ordered_isometric_objects; i++)
		{
			sprite_draw(&(ordered_isometric_objects_table[i]->sprite));
		}

		sprite_update_display(&(isometric_block_0.sprite));
		sprite_update_display(&(isometric_block_1.sprite));
	}

}

