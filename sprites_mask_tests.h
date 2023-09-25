
#ifndef SPRITES_MASKS_TESTS_H
#define SPRITES_MASKS_TESTS_H

#include "value_types.h"

typedef struct 
{
	byte pos_x, pos_y;
	byte last_x, last_y;
	byte to_update_x, to_update_y;
	byte to_update_width, to_update_height;
	byte width, height; // width in bytes, not pixels
	int frame_size; // size of a frame ( width * height * 2 in bytes, is *2 because of the mask acoompaning the graphic)
	byte *first_frame;
	byte *last_frame;
	byte *actual_frame;
} t_sprite;

// Imagen binaria y mascara en bytes alternos
typedef struct
{
	byte width, height; // width in bytes, not pixels
	int frame_size; // size of a frame ( width * height * 2 in bytes, is *2 because of the mask acoompaning the graphic)
	int total_frames_size; // size of all frames (n_frames * wodth * height * 2 in bytes, is *2 because of the mask acoompaning the graphic)
	byte *graphic_bin_def; // Imagen binaria y mascara en bytes alternos de todos los frames
} t_sprite_graphic_def;

void sprite_init(void);

void sprite_set_graphic_def(t_sprite *psprite, t_sprite_graphic_def *psprite_graphdef, int first_frane_offset, int last_frane_offset);
void sprite_set_frames_subset(t_sprite *psprite, t_sprite_graphic_def *psprite_graphdef, int first_frane_offset, int last_frane_offset);
void sprite_next_frame(t_sprite *psprite);

void sprite_draw(t_sprite *psprite);
void sprite_update_display(t_sprite *psprite);
void sprite_restore_vdisplay(t_sprite *psprite);
void sprite_transfer_vdisplay(void);
void sprite_transfer_and_restore_vdisplay(void);


#endif