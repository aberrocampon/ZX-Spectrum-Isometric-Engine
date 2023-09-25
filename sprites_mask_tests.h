
#ifndef SPRITES_MASKS_TESTS_H
#define SPRITES_MASKS_TESTS_H

typedef unsigned char byte;

typedef struct 
{
	byte pos_x, pos_y;
	byte last_x, last_y;
	byte to_update_x, to_update_y;
	byte to_update_width, to_update_height;
	byte width, height; // width in bytes, not pixels
	byte *graphic_bin_def; // imagen binaria y mascara en bytes alternos
} t_sprite;

void draw_sprite(t_sprite *psprite);
void update_display_sprite(t_sprite *psprite);
void restore_vdisplay_sprite(t_sprite *psprite);
void transfer_vdisplay(void);
void transfer_and_restore_vdisplay(void);
void init_sprites(void);

#endif