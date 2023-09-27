
/*
 * 
 * To build:
 * 	zcc +zx -m -create-app .\isometricos_tests.c .\sprites_mask_tests.c .\precalculated_shift_tables.asm
 * 
 */

#pragma output CRT_ORG_CODE          = 0x8000

#include <stdlib.h>

#include "sprites_mask_tests.h"
#include "value_types.h"

typedef struct
{
	// posicion 3d del vertice inferior-izquierdo de la caja que contiene la forma
	byte box_pos_x;
	byte box_pos_y;
	byte box_pos_z;
	// dimensiones de la caja de contencion
	byte box_width_x;
	byte box_width_y;
	byte box_height;
	// distancia en 2d entre el vertice inf-izq de la caja de contencion 
	// y la esquina sup-izq del sprite 2d 
	byte delta_sprite_x;
	byte delta_sprite_y; 
	// sprite grafico
	t_sprite sprite;
} t_isometric_obj;

#define ISOMETRIC_ORIGEN_PROJ_X 128
#define ISOMETRIC_ORIGEN_PROJ_Y 64

#define ISOMETRIC_MAX_X_3D 125
#define ISOMETRIC_MAX_Y_3D 125
#define ISOMETRIC_MAX_Z_3D 60

byte isometric_origen_proj_x = ISOMETRIC_ORIGEN_PROJ_X;
byte isometric_origen_proj_y = ISOMETRIC_ORIGEN_PROJ_Y;

byte isometric_max_x_3d = ISOMETRIC_MAX_X_3D;
byte isometric_max_y_3d = ISOMETRIC_MAX_Y_3D;
byte isometric_max_z_3d = ISOMETRIC_MAX_Z_3D;

#define N_MAX_ORDERED_ISOMETRIC_OBJECTS 16

int height_over_proj_plane[N_MAX_ORDERED_ISOMETRIC_OBJECTS];
t_isometric_obj *ordered_isometric_objects_table[N_MAX_ORDERED_ISOMETRIC_OBJECTS];
byte n_ordered_isometric_objects = 0;

void isometric_proj_obj(t_isometric_obj *p_isometric_obj)
{
	p_isometric_obj->sprite.pos_x = 
				isometric_origen_proj_x -
				p_isometric_obj->box_pos_x + 
				p_isometric_obj->box_pos_y +
				p_isometric_obj->delta_sprite_x;
	p_isometric_obj->sprite.pos_y = 
				isometric_origen_proj_y +
				((p_isometric_obj->box_pos_x)>>1) + 
				((p_isometric_obj->box_pos_y)>>1) -
				p_isometric_obj->box_pos_z +
				p_isometric_obj->delta_sprite_y;
}

#define reset_isometric_objects_ordering() {n_ordered_isometric_objects = 0;}

void isometric_add_object_to_order(t_isometric_obj *p_isometric_obj)
{
	int i;
	int ordered_index;
	int height;

	if(n_ordered_isometric_objects >= N_MAX_ORDERED_ISOMETRIC_OBJECTS) return;

	height = p_isometric_obj->box_pos_x + p_isometric_obj->box_pos_y + p_isometric_obj->box_pos_z;

	for(i = (n_ordered_isometric_objects - 1); i >= 0; i--)
	{
		if(height_over_proj_plane[i] < height) break;
	}

	ordered_index = i + 1;

	i = 0; // compiler bug !!!
	for(i = n_ordered_isometric_objects; i > ordered_index; i--)
	{
		height_over_proj_plane[i] = height_over_proj_plane[i - 1];
		ordered_isometric_objects_table[i] = ordered_isometric_objects_table[i - 1];
	}

	ordered_isometric_objects_table[ordered_index] = p_isometric_obj;
	height_over_proj_plane[ordered_index] = height;

	n_ordered_isometric_objects++;
}

/*******************************************************************************************************/

// imagen binaria y mascara en bytes alternos
byte graph_bin_def_block[] =
	{
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

byte graph_bin_def_ghost_ld[] =
	{
		// frame 0
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
		// frame 1
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
		0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF
	};

t_sprite_graphic_def spr_graph_def_block = 
	{	
		4, 28, // 4x character cells in width, 28 scan lines in height
		224,
		224,
		graph_bin_def_block
	};

t_sprite_graphic_def spr_graph_def_ghost_d = 
	{	
		3, 20, // 4x character cells in width, 28 scan lines in height
		120,
		240,
		graph_bin_def_ghost_ld
	};

t_isometric_obj isometric_block_0 =
	{
		30,
		30,
		20,
		14,
		15,
		11,
		-15,
		-11,
		{
			0, 0,
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
		15,
		80,
		0,
		10,
		11,
		9,
		-11,
		-9,
		{
			0, 0,
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

byte k_1, k_2, k_3, k_4, k_5, k_q, k_t;

void get_keys(void)
{
	byte d;

	 k_1 = k_2 = k_3 = k_4 = k_5 = k_q = k_t = 0;

	d = inp(0xf7fe);
	d ^= 0xff;

	if(d & 0x1)
	{
		k_1 = 1;
	}

	if(d & 0x2)
	{
		k_3 = 1;
	}

	if(d & 0x4)
	{
		k_2 = 1;
	}

	if(d & 0x8)
	{
		k_4 = 1;
	}

	if(d & 0x10)
	{
		k_5 = 1;
	}

	d = inp(0xfbfe);
	d ^= 0xff;

	if(d & 0x1)
	{
		k_q = 1;
	}

	if(d & 0x10)
	{
		k_t = 1;
	}
}

int x, y, z;

void main()
{
	byte i, nframes;

	sprite_init();
	sprite_transfer_vdisplay();

	sprite_set_graphic_def(&isometric_block_0.sprite, &spr_graph_def_block, 0, spr_graph_def_block.total_frames_size);
	sprite_set_graphic_def(&isometric_block_1.sprite, &spr_graph_def_ghost_d, 0, spr_graph_def_ghost_d.total_frames_size);

	// En estos momentos (18-09-2023) se observa aprox con 10 sprites en el bucle while(1)
	// 256 refrescos en 22 segundos =>
	// cada refresco de 10 sprites completos 86 ms, 8.6ms por sprite
	// 11.6 cuadros por segundo

	nframes = 0;
	while(1)//for(nframes = 256; nframes>=0; nframes--)
	{
		nframes++;

		sprite_restore_vdisplay(&(isometric_block_0.sprite));
		sprite_restore_vdisplay(&(isometric_block_1.sprite));

		get_keys();

		if(k_t) break;

		x = isometric_block_1.box_pos_x;
		y = isometric_block_1.box_pos_y;
		z= isometric_block_1.box_pos_z;

		if(k_1) 
		{
			--y;
			if(!(nframes & 7)) sprite_next_frame(&(isometric_block_1.sprite));
		}

		if(k_q)
		{
			++y;
			if(!(nframes & 7)) sprite_next_frame(&(isometric_block_1.sprite));
		}

		if(k_2)
		{
			--x;
			if(!(nframes & 7)) sprite_next_frame(&(isometric_block_1.sprite));
		}

		if(k_3) 
		{
			++x;
			if(!(nframes & 7)) sprite_next_frame(&(isometric_block_1.sprite));
		}

		if(k_4) 
		{
			++z;
			if(!(nframes & 7)) sprite_next_frame(&(isometric_block_1.sprite));
		}

		if(k_5) 
		{
			--z;
			if(!(nframes & 7)) sprite_next_frame(&(isometric_block_1.sprite));
		}

		if(x < 0)
		{
			x = 0;
		}
		if(x > isometric_max_x_3d - isometric_block_1.box_width_x)
		{
			x = isometric_max_x_3d - isometric_block_1.box_width_x;
		}

		
		if(y < 0)
		{
			y = 0;
		}
		if(y > (int)(isometric_max_y_3d - isometric_block_1.box_width_y))
		{
			y = isometric_max_y_3d - isometric_block_1.box_width_y;
		}

		if(z < 0)
		{
			z = 0;
		}
		if(z > (int)(isometric_max_z_3d - isometric_block_1.box_height))
		{
			z = isometric_max_z_3d - isometric_block_1.box_height;
		}

		isometric_block_1.box_pos_x = x;
		isometric_block_1.box_pos_y = y;
		isometric_block_1.box_pos_z = z;


		#asm
			;halt
		#endasm

		//transfer_and_restore_vdisplay();

		//continue;

		isometric_proj_obj(&isometric_block_0);
		isometric_proj_obj(&isometric_block_1);

		reset_isometric_objects_ordering();
		isometric_add_object_to_order(&isometric_block_0);
		isometric_add_object_to_order(&isometric_block_1);

		for(i=0; i < n_ordered_isometric_objects; i++)
		{
			sprite_draw(&(ordered_isometric_objects_table[i]->sprite));
		}
		//draw_sprite(isometric_block_0.psprite);
		//draw_sprite(isometric_block_1.psprite);

		//transfer_vdisplay();
		//continue;

		sprite_update_display(&(isometric_block_0.sprite));
		sprite_update_display(&(isometric_block_1.sprite));
	}

}

