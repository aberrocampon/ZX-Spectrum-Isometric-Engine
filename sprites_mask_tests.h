
#ifndef SPRITES_MASKS_TESTS_H
#define SPRITES_MASKS_TESTS_H

#include "value_types.h"

#define	SPRITE_GRAPHIC_STATE_MASK_FLIPPED_H (1)
#define	SPRITE_GRAPHIC_STATE_FLIPPED_LEFT (1)
#define	SPRITE_GRAPHIC_STATE_FLIPPED_RIGHT (0)

typedef struct 
{
	byte required_graphic_state; // flipping state of the bitmap for example
	byte pos_x_ref, pos_y_ref;
	byte pos_x, pos_y;
	// distancia en 2d entre el vertice origen 3d (vertice de mayor x, y, z en caja de contencion)
	// y la esquina sup-izq del sprite 2d 
	byte last_x, last_y;
	byte to_update_x, to_update_y;
	byte to_update_width, to_update_height;
	byte width, height; // width in bytes, not pixels
	byte width_px; // width in pixels
	byte moved_or_changed; // moved so need to be erased and redrawn, also the same in case changed frame or flip state
	byte redraw_not_moved; // not moved or chabged state but redraw in virtual buffer because another sprite is erased over it or redrawn below it
	int frame_size; // size of a frame ( width * height * 2 in bytes, is *2 because of the mask acoompaning the graphic)
	byte *first_frame;
	byte *last_frame;
	byte *actual_frame;
	byte erase_rect_min_x;
	byte erase_rect_max_x;
	byte erase_rect_max_y;
	byte not_moved_rect_max_x;
	byte not_moved_rect_max_y;
} t_sprite;

// Offsets a miembros de la estructura para usar dentro del codigo ensamblador inline
#define T_SPRITE_OFFSET_POS_X 3
#define T_SPRITE_OFFSET_POS_Y 4
#define T_SPRITE_OFFSET_LAST_X 5
#define T_SPRITE_OFFSET_LAST_Y 6
#define T_SPRITE_OFFSET_HEIGHT 12
#define T_SPRITE_OFFSET_WIDTH_PX 13
#define T_SPRITE_OFFSET_MOVED_OR_CHANGED 14
#define T_SPRITE_OFFSET_REDRAW_NOT_MOVED 15
#define T_SPRITE_OFFSET_ERASE_RECT_MIN_X 24
#define T_SPRITE_OFFSET_ERASE_RECT_MAX_X 25
#define T_SPRITE_OFFSET_ERASE_RECT_MAX_Y 26
#define T_SPRITE_OFFSET_NOT_MOVED_RECT_MAX_X 27
#define T_SPRITE_OFFSET_NOT_MOVED_RECT_MAX_Y 28

// Imagen binaria y mascara en bytes alternos
typedef struct
{
	byte width, height; // width in bytes, not pixels
	// distancia en 2d entre el vertice origen 3d
	// y la esquina sup-izq del sprite 2d 
	int frame_size; // size of a frame ( width * height * 2 in bytes, is *2 because of the mask acoompaning the graphic)
	int total_frames_size; // size of all frames (n_frames * wodth * height * 2 in bytes, is *2 because of the mask acoompaning the graphic)
	byte *graphic_bin_def; // Imagen binaria y mascara en bytes alternos de todos los frames. El primero es un byte flags que representan el estado del bitmap. Por ejemplo flipado izquierda / derecha
} t_sprite_graphic_def;

void sprite_init(void);
//void sprite_init_background(void);
void sprite_set_border(byte border);
void sprite_clear_display(void);
void sprite_clear_vdisplay(void);
void sprite_set_attrib(byte attrib);

void sprite_set_graphic_def(t_sprite *psprite, byte initial_required_graphic_state, t_sprite_graphic_def *psprite_graphdef);
void sprite_set_frames_subset(t_sprite *psprite, byte *first_frane_address, byte *last_frane_address);
void sprite_next_frame(t_sprite *psprite);
void sprite_set_required_graphic_state(t_sprite *psprite, byte required_graphic_state);

void sprite_set_pos_from_posref(t_sprite *psprite);
void sprite_draw(t_sprite *psprite);
void sprite_update_display(t_sprite *psprite);
/*
void sprite_restore_vdisplay(t_sprite *psprite);
void sprite_transfer_vdisplay_2_phys_display(void);
void sprite_transfer_vdisplay_2_background_vdisplay(void);
void sprite_transfer_and_restore_vdisplay(void);
*/
void sprite_erase_with_zeros(t_sprite *psprite) __z88dk_fastcall;

#endif