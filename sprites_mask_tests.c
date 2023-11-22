#include "sprites_mask_tests.h"

//unsigned char background_vdisplay_bin_buff[192*32]; // buffer imagen binaria
unsigned char vdisplay_bin_buff[192*32]; // buffer imagen binaria

extern byte precalculated_flip_byte_table[]; // en direccion 0xEF00, tabla de 256 bytes para invertir el orden de los bits de un byte
extern byte precalculated_shift_tables[]; // en direccion 0xF000, las tables ocupan 4Kb al final de la memoria

void precalculate_flip_byte_table(void)
{
	byte* p;
	byte valor, valor_bits_invertidos;
	byte i;

	p = precalculated_flip_byte_table;

	for(valor = 0; valor < 255; valor++)
	{
		for(i=0, valor_bits_invertidos=0; i<8; i++)
		{
			if(valor & (1<<i))
			{
				valor_bits_invertidos |= (1<<(7-i));
			}
		}
		*p++ = valor_bits_invertidos;
	}

	*p = 255;
}

void precalculate_shift_tables(void)
{
	byte* p;
	int r, d, aux;
	
	for(r=0; r<8; r++)
	{
		p = precalculated_shift_tables + ( r<<9 );
		for(d=0; d<=0xff; d++)
		{
			aux = d<<(8-r);
			*(p + 0x100+d) = (byte)aux;
			aux = (aux >> 8);
			*(p + d) = (byte)aux;
		}
	}
}

void sprite_clear_display(void)
{
	#asm
		ld hl, 0x4000
		ld d, h
		ld e, l
		inc e
		ld bc, 6143
		ld (hl), 0
		ldir
	#endasm
}

void sprite_clear_vdisplay(void)
{
	#asm
		ld hl, _vdisplay_bin_buff
		ld d, h
		ld e, l
		inc e
		ld bc, 6143
		ld (hl), 0
		ldir
	#endasm
}

byte att;
void sprite_set_attrib(byte attrib)
{
	att = attrib;
	#asm
		ld hl, 0x5800
		ld d, h
		ld e, l
		inc e
		ld bc, 767
		ld a, (_att)
		ld (hl), a
		ldir
	#endasm
}

void sprite_set_border(byte border)
{
	att = border;
	#asm
		ld a, (_att)
		out (0xfe), a
	#endasm
}

// void sprite_init_background(void)
// {
// 	int i;
// 	byte *p_attribs_buffer = (byte *)0x5800;

// 	for(i=0; i<0x1800; i++)
// 	{
// 		if(i & 0x20)
// 		{
// 			background_vdisplay_bin_buff[i] = 0x55;
// 	  		vdisplay_bin_buff[i] = 0x55;
// 		}
// 		else
// 		{
// 			background_vdisplay_bin_buff[i] = 0xaa;
// 	  		vdisplay_bin_buff[i] = 0xaa;
// 		}
// 	}

// 	for(; p_attribs_buffer < ((byte *)0x5b00); p_attribs_buffer++)
// 	{
// 		*p_attribs_buffer = 7;
// 	}
// }

void sprite_init(void)
{
	precalculate_flip_byte_table();
	precalculate_shift_tables();
}

void sprite_set_graphic_def(t_sprite *psprite, byte initial_required_graphic_state, t_sprite_graphic_def *psprite_graphdef)
{
	if(psprite_graphdef)
	{
		psprite->required_graphic_state = initial_required_graphic_state;
		psprite->width = psprite_graphdef->width;
		psprite->height = psprite_graphdef->height;
		psprite->width_px = precalculated_shift_tables[0xb00 + psprite->width]; // x 8
		psprite->last_y = 255;
		psprite->moved_or_changed = 1;
		psprite->redraw_not_moved = 0;
		psprite->frame_size = psprite_graphdef->frame_size;
		sprite_set_frames_subset(psprite, psprite_graphdef->graphic_bin_def, psprite_graphdef->graphic_bin_def + psprite_graphdef->total_frames_size);
	}
	else
	{
		psprite->first_frame = psprite->actual_frame = psprite->last_frame = NULL;
	}
}

void sprite_set_frames_subset(t_sprite *psprite, byte *first_frane_address, byte *last_frane_address)
{
	psprite->first_frame = psprite->actual_frame = first_frane_address;
	psprite->last_frame = last_frane_address;
	psprite->moved_or_changed = 1;
}

void sprite_next_frame(t_sprite *psprite)
{
	psprite->actual_frame += psprite->frame_size;
	if((psprite->actual_frame) >= (psprite->last_frame))
	{
		psprite->actual_frame = psprite->first_frame;
	}
	psprite->moved_or_changed = 1;
}

void sprite_set_required_graphic_state(t_sprite *psprite, byte required_graphic_state)
{
	psprite->required_graphic_state = required_graphic_state;
	psprite->moved_or_changed = 1;
}

int sprgbl_guarda_sp;
byte sprgbl_width;
int sprgbl_iWidth;
byte sprgbl_half_width;
byte sprgbl_height;
byte *sprgbl_p_gbd;
byte *sprgbl_p_vdisp;
byte *sprgbl_p_disp;
byte sprgbl_p_shift_table_high;
int sprgbl_incr_v_ptr_disp;
byte sprgbl_mask_start;
byte sprgbl_mask_end;
byte mask_start_array[] = {0, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe};
byte mask_end_array[] = {0xff, 0x7f, 0x3f, 0x1f, 0x0f, 0x7, 0x03, 0x01};
byte sprgbl_x0, sprgbl_x1, sprgbl_aux;
t_sprite *sprgbl_psprite;

void sprite_set_pos_from_posref(t_sprite *psprite)
{
	sprgbl_psprite = psprite;
	sprgbl_p_gbd = sprgbl_psprite->actual_frame;
	if(sprgbl_psprite->required_graphic_state != *sprgbl_p_gbd++)
	{
		sprgbl_psprite->pos_x = sprgbl_psprite->pos_x_ref + 1 - sprgbl_psprite->width_px - *sprgbl_p_gbd++;
	}
	else
	{
		sprgbl_psprite->pos_x = sprgbl_psprite->pos_x_ref + *sprgbl_p_gbd++;
	}
	sprgbl_psprite->pos_y = sprgbl_psprite->pos_y_ref + *sprgbl_p_gbd;
}

void sprite_flip_h(void)
{
	// Tamaño en byte: 160 bytes
	#asm
		di
		ld (_sprgbl_guarda_sp), sp

		ld hl, (_sprgbl_p_gbd)
		ld a, (_sprgbl_width)
		ld c, a
		dec c
		ld b, 0
		add hl, bc
		add hl, bc
		ld d, 0xef ; tabla de inversion de orden de bit de un byte en direccion 0xe000
		exx
		ld hl, (_sprgbl_p_gbd)
		ld d, 0xef ; tabla de inversion de orden de bit de un byte en direccion 0xe000
		ld a, (_sprgbl_width)
		rra
		ld (_sprgbl_half_width), a
		jr nc, l_sprite_flip_h_1
		; en cada linea del bitmap una cantidad impar de parejas byte / mascara
		rla
		inc a
		ld ixl, a ; incremento al puntero de la parte izquierda en cada salto de linea
		dec a
		ld ixh, a
		add a, ixh
		add a, ixh
		dec a
		ld ixh, a		
		jr l_sprite_flip_h_2

	l_sprite_flip_h_1:
		; en cada linea del bitmap una cantidad par de parejas byte / mascara
		rla
		ld ixl, a ; incremento al puntero de la parte izquierda en cada salto de linea
		add a, ixl
		add a, ixl
		ld ixh, a
	l_sprite_flip_h_2:
		ld a, (_sprgbl_half_width)
		ex af, af
		ld a, (_sprgbl_height)

	; En el bucle, por linea del bitmap:
	; hl  : byte izquierdo de la linea actual del bitmap (el siguiente byte es su mascara)
	; hl_alt : byte derecho de la linea actual del bitmap (el siguiente byte es su mascara)
	; d y d_alt : Byte alto de la direccion de la tabla de flipado de byte precalculada (invierte orden de bits)
	; a : numero de parejas bytes + mascara de una linea del bitmap / 2. Si es impar hay que flipar luego el byte central y mascara
	; a_alt numero de lineas de bitmap
	l_sprite_flip_h_3:
		ex af, af
	l_sprite_flip_h_4:
		ld sp, hl                  ; 6
		pop bc                     ; 10
		inc hl                     ; 6
		inc hl                     ; 6
		ex de, hl                  ; 4
		ld l, c                    ; 4
		ld c, (hl)                 ; 7
		ld l, b                    ; 4
		ld b, (hl)                 ; 7
		ex de, hl                  ; 4

		exx                        ; 4
		ld sp, hl                  ; 6
		pop bc                     ; 10
		dec hl                     ; 6
		dec hl                     ; 6
		ex de, hl                  ; 4
		ld l, c                    ; 4
		ld c, (hl)                 ; 7
		ld l, b                    ; 4
		ld b, (hl)                 ; 7
		ex de, hl                  ; 4

		exx                        ; 4
		push bc                    ; 11
		ld sp, hl                  ; 6

		exx                        ; 4
		push bc                    ; 11

		exx                        ; 4

		dec a                      ; 4
		jr nz, l_sprite_flip_h_4   ; 12 / 7  ; total =  176 t-states en bucle que se repite

		ld b, 0
		ld c, ixl
        add hl, bc
		exx
		ld b, 0
		ld c, ixh
		add hl, bc
		exx
		ld a, (_sprgbl_half_width)
		ex af, af
		dec a
		jr nz, l_sprite_flip_h_3
		
		ld a, (_sprgbl_width)
		rra
		jr nc, l_sprite_flip_h_6
		; el numero de parejas byte / mascara en cada linea es impar, luego hay una colunna central que no ha sido flipada aun
		ld hl, (_sprgbl_p_gbd)
		rla
		ld c, a
		dec c
		add hl, bc
		ld sp, hl
		ex de, hl
		add a, a
		exx
		ld e, a
		ld d, 0
		ld a, (_sprgbl_height)
		ld b, a
	l_sprite_flip_h_5:
		exx
		pop bc
		ld l, c
		ld c, (hl)
		ld l, b
		ld b, (hl)
		push bc
		exx
		ld h, d
		ld l, e
		add hl, sp
		ld sp, hl
		djnz l_sprite_flip_h_5

	l_sprite_flip_h_6:
		ld sp, (_sprgbl_guarda_sp)
		ei

	#endasm

/*
	// alternativa
	#asm
	; bucle flipando bytes no buts
	l_sprite_flip_h_2:
		ld sp, hl                       ; 6
		pop bc                          ; 10
		inc hl                          ; 6
		inc hl                          ; 6
		exx                             ; 4
		ldi                             ; 16
		ldi                             ; 16
		ld sp, hl                       ; 6
		dec hl                          ; 6
		dec hl                          ; 6
		dec hl                          ; 6
		dec hl                          ; 6
		exx                             ; 4
		push bc                         ; 11
		dec a                           ; 4
		jr nz, l_sprite_flip_h_2        ; 12 / 7   Total = 125 con repeticion de bucle

	; siguiente bucle flipando los bits de cada byte
	l_sprite_flip_h_3:
		exx                            ; 4
		pop bc                         ; 10
		ld h, a                        ; 4
		ld l, c                        ; 4
		ld c, 1                        ; 7
		ldi                            ; 16
		ld h, a                        ; 4
		ld l, b                        ; 4
		ldi                            ; 16
		exx                            ; 4
		djnz l_sprite_flip_h_3         13 / 8  Total = 86 con repeticion

	#endasm
*/

}

void sprite_draw(t_sprite *psprite)
{	
	sprgbl_psprite = psprite;

	if(!(sprgbl_psprite->moved_or_changed || sprgbl_psprite->redraw_not_moved))
	{
		return;
	}
	sprgbl_psprite->redraw_not_moved = 0;

	sprgbl_width = sprgbl_psprite->width;
	sprgbl_height = sprgbl_psprite->height;
	sprgbl_p_gbd = sprgbl_psprite->actual_frame;

	// actualizar al fipado horizontal requerido /////////////
	sprgbl_aux = (sprgbl_psprite->required_graphic_state) & SPRITE_GRAPHIC_STATE_MASK_FLIPPED_H;
	if( sprgbl_aux != (*sprgbl_p_gbd & SPRITE_GRAPHIC_STATE_MASK_FLIPPED_H) )
	{
		*sprgbl_p_gbd = (*sprgbl_p_gbd & ~SPRITE_GRAPHIC_STATE_MASK_FLIPPED_H) | sprgbl_aux;
		sprgbl_p_gbd += 3;
		sprite_flip_h();
		// ajusta delta_x (desplazamiento del punto de referencia del sprite respecto a esquina superior izquierda)
		sprgbl_p_gbd -= 2;
		*sprgbl_p_gbd = 1 - sprgbl_psprite->width_px - *sprgbl_p_gbd;
		sprgbl_p_gbd += 2;
	}
	else
	{
		sprgbl_p_gbd += 3;
	}
	// desplazar coordenadas de referencia en la pantalla a coordenadas de la esquina superior izquierda en donde se dibujara el sprite
	//sprgbl_psprite->pos_x = sprgbl_psprite->pos_x_ref + *sprgbl_p_gbd++;
	//sprgbl_psprite->pos_y = sprgbl_psprite->pos_y_ref + *sprgbl_p_gbd++;
	/////////////////////////////////////////////////////////

	sprgbl_p_vdisp = vdisplay_bin_buff + (((unsigned int)(sprgbl_psprite->pos_y))<<5) + (((unsigned int)(sprgbl_psprite->pos_x))>>3);
	sprgbl_p_shift_table_high = 0xf0 | (((sprgbl_psprite->pos_x)&7)<<1);
	sprgbl_mask_start = mask_start_array[(sprgbl_psprite->pos_x)&7];
	sprgbl_mask_end = mask_end_array[(sprgbl_psprite->pos_x)&7];
	sprgbl_incr_v_ptr_disp = 32 - sprgbl_width;

	#asm
		di
		ld (_sprgbl_guarda_sp), sp
		ld sp,(_sprgbl_p_gbd)

        ld a ,(_sprgbl_p_shift_table_high)
		ld h, a
		ld de, (_sprgbl_p_vdisp)

		exx
		ld a, (_sprgbl_height)
		ld c, a
		ld a, (_sprgbl_width)
		ld d, a
		ld b, a

		ld a, (_sprgbl_mask_start)
		ex af, af
		xor a

;// En el bucle:
;// * sp, direccion de definicion grafica binaria
;// * h, 0xf0 | (x & 7). Puntero a tablas de desplazamiento precalculadas
;// * de, direccion del primer byte a escribir en el buffer de imagen virtual. Superior, izquierda en la imagen.
;// * a  , byte abterior de la imagen del sprite desplazado a la derecha segun x&7. Inicialmente 0
;// * a' , byte anterior de la mascara el sprite desplazado a la derecha segun x&7. Inicialmente 0xff << (8 - x&7)
;// * b' , ancho del sprite en bytes
;// * c', alto del sprite en scans
	l_draw_sprite_1:
		exx                       ;// 4

    	pop bc                    ;// 10
		ld l, c                   ;// 4
		or (hl)                   ;// 7
		
		ld l, b                   ;// 4
		ld b, a                   ;// 4
		ex af, af                 ;// 4
		or (hl)                   ;// 7
				
		ex de, hl                 ;// 4
		and (hl)                  ;// 7
		or b                      ;// 4
		ld (hl), a                ;// 7
		inc hl                    ;// 6
		ex de, hl                 ;// 4
	
		inc h                     ;// 4
		ld a, (hl)                ;// 7
		ex af, af                 ;// 4
		ld l, c                   ;// 4
		ld a, (hl)                ;// 7
		dec h                     ;// 4
		
		exx                       ;// 4
		djnz l_draw_sprite_1      ;// 13 (B /= 0), 8 (B = 0)
		
	;// Ultimo byte ///////////////////////////////////////////////////////
		
		exx                         ; 4
		ld b, a                     ; 4
		ld a, (_sprgbl_mask_end)           ; 13
		ld c, a                     ; 4
		ex af, af                   ; 4
		or c                        ; 4
		ex de,hl                    ; 4
		and (hl)                    ; 7
		or b                        ; 4
		ld (hl),a                   ; 7
		ld bc, (_sprgbl_incr_v_ptr_disp)   ; 20
		add hl, bc                  ; 11
		ex de, hl                   ; 4

		ld a, (_sprgbl_mask_start)         ; 13
		ex af, af                   ; 4
		xor a                       ; 4
		
		exx                         ; 4
		ld b, d                     ; 4
		dec c                       ; 4
		jr nz, l_draw_sprite_1      ; 12 / 7

		ld sp, (_sprgbl_guarda_sp)
		ei

	#endasm

	if(sprgbl_psprite->last_y != 255)
	{
		sprgbl_x0 = sprgbl_psprite->last_x;
		sprgbl_x1 = sprgbl_psprite->pos_x;
		if(sprgbl_x0 > sprgbl_x1)
		{
			sprgbl_aux = sprgbl_x1;
			sprgbl_x1 = sprgbl_x0;
			sprgbl_x0 = sprgbl_aux;
		}
		sprgbl_psprite->to_update_x = sprgbl_x0;
		sprgbl_psprite->to_update_width = sprgbl_psprite->width;
		if(sprgbl_x1 & 7) sprgbl_psprite->to_update_width++;
		sprgbl_aux = (sprgbl_x1 & 0xf8) - (sprgbl_x0 & 0xf8);
		sprgbl_psprite->to_update_width += precalculated_shift_tables[0x600 + sprgbl_aux];

		sprgbl_x0 = sprgbl_psprite->last_y;
		sprgbl_x1 = sprgbl_psprite->pos_y;
		if(sprgbl_x0 > sprgbl_x1)
		{
			sprgbl_aux = sprgbl_x1;
			sprgbl_x1 = sprgbl_x0;
			sprgbl_x0 = sprgbl_aux;
		}
		sprgbl_psprite->to_update_y = sprgbl_x0;
		sprgbl_psprite->to_update_height = sprgbl_psprite->height;
		sprgbl_psprite->to_update_height += (sprgbl_x1 - sprgbl_x0 );
	}
	else
	{
		sprgbl_psprite->to_update_x = sprgbl_psprite->pos_x;
		sprgbl_psprite->to_update_y = sprgbl_psprite->pos_y;
		sprgbl_psprite->to_update_width = sprgbl_psprite->width;
		if((sprgbl_psprite->to_update_x) & 7) sprgbl_psprite->to_update_width++;
		sprgbl_psprite->to_update_height = sprgbl_psprite->height;
	}

	sprgbl_psprite->last_x = sprgbl_psprite->pos_x;
	sprgbl_psprite->last_y = sprgbl_psprite->pos_y;


}

void sprite_update_display(t_sprite *psprite)
{
/*
	sprgbl_psprite = psprite;
	if(!(sprgbl_psprite->moved_or_changed)) return;

	sprgbl_psprite->moved_or_changed = 0;

	sprgbl_p_vdisp = vdisplay_bin_buff + (((unsigned int)(psprite->to_update_y))<<5) + (((unsigned int)(psprite->to_update_x))>>3);
	sprgbl_p_disp = (byte *) (0x4000 + 
			((((unsigned int)(psprite->to_update_y)) & 0xc0) << 5) + ((((unsigned int)(psprite->to_update_y))&0x7)<<8) +
			((((unsigned int)(psprite->to_update_y))&0x38)<<2) + (((unsigned int)(psprite->to_update_x))>>3) );
	sprgbl_iWidth = psprite->to_update_width;
	sprgbl_height = psprite->to_update_height;
	sprgbl_incr_v_ptr_disp = 32 - sprgbl_iWidth;

	#asm
		di

		ld hl, (_sprgbl_p_vdisp)
		ld de, (_sprgbl_p_disp)
		ld a, (_sprgbl_height)
	l_update_display_sprite_1:
		ex af, af								; 5
		ld bc, (_sprgbl_iWidth)					; 22
		ldir									; n x 23 - 5
		ld bc, (_sprgbl_incr_v_ptr_disp)		; 22
		add hl, bc								; 12
		ld de, (_sprgbl_p_disp)					; 22
		inc d									; 5
		ld a, d									; 5
		and 7									; 8
		jr nz, l_update_display_sprite_2		; 13
		ld a, e
		add a, 0x20
		ld e, a
		jr c, l_update_display_sprite_2
		ld a, d
		sub a, 8
		ld d, a
	l_update_display_sprite_2:
	    ld (_sprgbl_p_disp), de					; 22
		ex af, af								; 5
		dec a									; 5
		jr nz, l_update_display_sprite_1		; 13

		ei

	#endasm
*/
	sprgbl_psprite = psprite;

	if(!(sprgbl_psprite->moved_or_changed)) return;

	sprgbl_psprite->moved_or_changed = 0;
	sprgbl_p_vdisp = vdisplay_bin_buff + (((unsigned int)(sprgbl_psprite->to_update_y))<<5) + (((unsigned int)(sprgbl_psprite->to_update_x))>>3);
	sprgbl_p_disp = (byte *) (0x4000 + 
			((((unsigned int)(sprgbl_psprite->to_update_y)) & 0xc0) << 5) + ((((unsigned int)(sprgbl_psprite->to_update_y))&0x7)<<8) +
			((((unsigned int)(sprgbl_psprite->to_update_y))&0x38)<<2) + (((unsigned int)(sprgbl_psprite->to_update_x))>>3) );
	sprgbl_iWidth = sprgbl_psprite->to_update_width;
	sprgbl_height = sprgbl_psprite->to_update_height;

	#asm
		di
		ld (_sprgbl_guarda_sp), sp

		ld a, (_sprgbl_height)
		ld b, a
		ld a, (_sprgbl_iWidth)
		cp 7
		jp nc, l_update_display_sprite_more_than_6_bytes
		cp 1
		jp z, l_update_display_sprite_more_than_6_bytes

		exx
		ld hl, (_sprgbl_p_disp)	
		ld e, a
		dec e
		ld d, 0
		add hl, de
		exx
		ld hl, (_sprgbl_p_vdisp)
		ld de, 32
		cp 3
		jr z, l_sprite_update_display_3_bytes_loop
		cp 4
		jr z, l_sprite_update_display_4_bytes_loop
		cp 5
		jr z, l_sprite_update_display_5_bytes_loop
		cp 6
		jp z, l_sprite_update_display_6_bytes_loop

	l_sprite_update_display_2_bytes_loop:
		ld sp, hl
		exx
		pop de
		ld sp, hl
		inc sp
		push de
		inc h
		ld a, h
		and 7
		jr z, l_sprite_update_display_inc_display_ptr_row_2_bytes
	l_sprite_update_display_inc_display_ptr_row_2_bytes_continue:
		exx
		add hl, de
		djnz l_sprite_update_display_2_bytes_loop
		jp l_sprite_update_display_end
	l_sprite_update_display_inc_display_ptr_row_2_bytes:
		ld a, l
		add a, 32
		ld l, a
		jr c, l_sprite_update_display_inc_display_ptr_row_2_bytes_continue
		ld a, h
		sub 8
		ld h, a
		jr l_sprite_update_display_inc_display_ptr_row_2_bytes_continue

	l_sprite_update_display_3_bytes_loop:
		ld sp, hl
		exx
		pop bc
		dec sp
		pop de
		ld sp, hl
		inc sp
		push de
		inc sp
		push bc
		inc h
		ld a, h
		and 7
		jr z, l_sprite_update_display_inc_display_ptr_row_3_bytes
	l_sprite_update_display_inc_display_ptr_row_3_bytes_continue:
		exx
		add hl, de
		djnz l_sprite_update_display_3_bytes_loop
		jp l_sprite_update_display_end
	l_sprite_update_display_inc_display_ptr_row_3_bytes:
		ld a, l
		add a, 32
		ld l, a
		jr c, l_sprite_update_display_inc_display_ptr_row_3_bytes_continue
		ld a, h
		sub 8
		ld h, a
		jr l_sprite_update_display_inc_display_ptr_row_3_bytes_continue

	l_sprite_update_display_4_bytes_loop:
		ld sp, hl
		exx
		pop bc
		pop de
		ld sp, hl
		inc sp
		push de
		push bc
		inc h
		ld a, h
		and 7
		jr z, l_sprite_update_display_inc_display_ptr_row_4_bytes
	l_sprite_update_display_inc_display_ptr_row_4_bytes_continue:
		exx
		add hl, de
		djnz l_sprite_update_display_4_bytes_loop
		jp l_sprite_update_display_end
	l_sprite_update_display_inc_display_ptr_row_4_bytes:
		ld a, l
		add a, 32
		ld l, a
		jr c, l_sprite_update_display_inc_display_ptr_row_4_bytes_continue
		ld a, h
		sub 8
		ld h, a
		jr l_sprite_update_display_inc_display_ptr_row_4_bytes_continue

	l_sprite_update_display_5_bytes_loop:
		ld sp, hl				; 7
		exx						; 5
		pop af					; 11
		dec sp					; 7
		pop bc					; 11
		pop de					; 11
		ld sp, hl 				; 7
		inc sp                  ; 7
		push de					; 12
		push bc					; 12
		inc sp					; 7
		push af					; 12
		inc h					; 5
		ld a, h					; 5
		and 7					; 8
		jr z, l_sprite_update_display_inc_display_ptr_row_5_bytes 		; 8
	l_sprite_update_display_inc_display_ptr_row_5_bytes_continue:
		exx						; 5
		add hl, de				; 12
		djnz l_sprite_update_display_5_bytes_loop 						; 14
		jr l_sprite_update_display_end
	l_sprite_update_display_inc_display_ptr_row_5_bytes:
		ld a, l
		add a, 32
		ld l, a
		jr c, l_sprite_update_display_inc_display_ptr_row_5_bytes_continue
		ld a, h
		sub 8
		ld h, a
		jr l_sprite_update_display_inc_display_ptr_row_5_bytes_continue

	l_sprite_update_display_6_bytes_loop:
		ld sp, hl
		exx
		pop af
		pop bc
		pop de
		ld sp, hl
		inc sp
		push de
		push bc
		push af
		inc h
		ld a, h
		and 7
		jr z, l_sprite_update_display_inc_display_ptr_row_6_bytes
	l_sprite_update_display_inc_display_ptr_row_6_bytes_continue:
		exx
		add hl, de
		djnz l_sprite_update_display_6_bytes_loop
		jr l_sprite_update_display_end
	l_sprite_update_display_inc_display_ptr_row_6_bytes:
		ld a, l
		add a, 32
		ld l, a
		jr c, l_sprite_update_display_inc_display_ptr_row_6_bytes_continue
		ld a, h
		sub 8
		ld h, a
		jr l_sprite_update_display_inc_display_ptr_row_6_bytes_continue

	l_update_display_sprite_more_than_6_bytes:
		ld a, 32
		ld de, (_sprgbl_iWidth)
		sub e
		ld e, a
		ld (_sprgbl_incr_v_ptr_disp), de
		ld hl, (_sprgbl_p_vdisp)
		ld de, (_sprgbl_p_disp)
		ld a, b
	l_update_display_sprite_more_than_6_bytes_loop:
		ex af, af
		ld bc, (_sprgbl_iWidth)
		ldir
		ld bc, (_sprgbl_incr_v_ptr_disp)
		add hl, bc
		ld de, (_sprgbl_p_disp)
		inc d
		ld a, d
		and 7
		jr z, l_sprite_update_display_inc_display_ptr_row_more_than_6_bytes
	l_sprite_update_display_inc_display_ptr_row_more_than_6_bytes_continue:
	    ld (_sprgbl_p_disp), de
		ex af, af
		dec a
		jr nz, l_update_display_sprite_more_than_6_bytes_loop
	l_sprite_update_display_inc_display_ptr_row_more_than_6_bytes:
		ld a, e
		add a, 32
		ld e, a
		jr c, l_sprite_update_display_inc_display_ptr_row_more_than_6_bytes_continue
		ld a, d
		sub 8
		ld d, a
		jr l_sprite_update_display_inc_display_ptr_row_more_than_6_bytes_continue

	l_sprite_update_display_end:
		ld sp, (_sprgbl_guarda_sp)
		ei

	#endasm

}

// void sprite_restore_vdisplay(t_sprite *psprite)
// {
// 	if((psprite->last_y) == 255) return; // el sprite no fue dibujado en el buffer virtual, no debe ser restaurado el buffer

// 	sprgbl_p_disp = background_vdisplay_bin_buff + (((unsigned int)(psprite->last_y))<<5) + (((unsigned int)(psprite->last_x))>>3);
// 	sprgbl_p_vdisp = vdisplay_bin_buff + (((unsigned int)(psprite->last_y))<<5) + (((unsigned int)(psprite->last_x))>>3);
// 	sprgbl_iWidth = psprite->width;
// 	if((psprite->last_x) & 7) sprgbl_iWidth++;
// 	sprgbl_height = psprite->height;
// 	sprgbl_incr_v_ptr_disp = 32 - sprgbl_iWidth;

// 	#asm
// 		di

// 		ld hl, (_sprgbl_p_disp)                               ; 16
// 		ld de, (_sprgbl_p_vdisp)                              ; 20
// 		ld a, (_sprgbl_height)                                ; 13
// 	l_restore_vdisplay_sprite_1:
// 		ld bc, (_sprgbl_iWidth)                               ; 20
// 		ldir                                           ; 21 / 16   (bc * 21 - 5)
// 		ld bc, (_sprgbl_incr_v_ptr_disp)                      ; 20
// 		add hl, bc                                     ; 11
// 		ex de,hl                                       ; 4
// 		add hl, bc                                     ; 11
// 		ex de,hl                                       ; 4
// 		dec a                                          ; 4
// 		jr nz, l_restore_vdisplay_sprite_1             ; 12/7

// 		ei

// 	#endasm
// }

// void sprite_transfer_vdisplay_2_phys_display(void)
// {

// 	#asm
// 		ld hl, _vdisplay_bin_buff
// 		ld de, 0x4000
// 		ld b, 192
// 	l_transfer_vdisplay_1:
// 		push bc
// 		push de
// 		;ld bc, 32
// 		;ldir
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		pop de
// 		inc d
// 		ld a, d
// 		and 7
// 		jr nz, l_transfer_vdisplay_2
// 		ld a, e
// 		add a, 32
// 		ld e, a
// 		jr c, l_transfer_vdisplay_2
// 		ld  a, d
// 		sub 8
// 		ld d, a
// 	l_transfer_vdisplay_2:
// 		pop bc
// 		djnz l_transfer_vdisplay_1
// 	#endasm
// }


// void sprite_transfer_vdisplay_2_background_vdisplay(void)
// {
// 	#asm
// 		ld hl, _vdisplay_bin_buff
// 		ld de, _background_vdisplay_bin_buff
// 		ld b, 192
// 	l_transfer_vdisplay_2_background_vdisp_1:
// 		push bc
// 		;ld bc, 32
// 		;ldir
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		ldi
// 		pop bc
// 		djnz l_transfer_vdisplay_2_background_vdisp_1
// 	#endasm
// }

// void sprite_transfer_and_restore_vdisplay(void)
// {

// 	#asm
// 		di
// 		ld (_sprgbl_guarda_sp), sp
// 		ld sp, _background_vdisplay_bin_buff
// 		ld hl, _vdisplay_bin_buff
// 		ld de, 0x4000
// 		ld a, 192
// 	l_transfer_and_restore_vdisplay_1:
// 		ex af, af
// 		;; 2 bytes ;;;;;
// 		pop bc                     ; 10
// 		ld a, (hl)                 ; 7
// 		ld (de), a                 ; 7
// 		ld (hl), c                 ; 7
// 		inc hl                     ; 6
// 		inc e                      ; 4
// 		ld a, (hl)                 
// 		ld (de), a
// 		ld (hl), b
// 		inc hl
// 		inc e
// 		;;;;;;;;;;;;;;;;
// 		;; 2 bytes ;;;;;
// 		pop bc
// 		ld a, (hl)
// 		ld (de), a
// 		ld (hl), c
// 		inc hl
// 		inc e
// 		ld a, (hl)
// 		ld (de), a
// 		ld (hl), b
// 		inc hl
// 		inc e
// 		;;;;;;;;;;;;;;;;
// 		;; 2 bytes ;;;;;
// 		pop bc
// 		ld a, (hl)
// 		ld (de), a
// 		ld (hl), c
// 		inc hl
// 		inc e
// 		ld a, (hl)
// 		ld (de), a
// 		ld (hl), b
// 		inc hl
// 		inc e
// 		;;;;;;;;;;;;;;;;
// 		;; 2 bytes ;;;;;
// 		pop bc
// 		ld a, (hl)
// 		ld (de), a
// 		ld (hl), c
// 		inc hl
// 		inc e
// 		ld a, (hl)
// 		ld (de), a
// 		ld (hl), b
// 		inc hl
// 		inc e
// 		;;;;;;;;;;;;;;;;
// 		;; 2 bytes ;;;;;
// 		pop bc
// 		ld a, (hl)
// 		ld (de), a
// 		ld (hl), c
// 		inc hl
// 		inc e
// 		ld a, (hl)
// 		ld (de), a
// 		ld (hl), b
// 		inc hl
// 		inc e
// 		;;;;;;;;;;;;;;;;
// 		;; 2 bytes ;;;;;
// 		pop bc
// 		ld a, (hl)
// 		ld (de), a
// 		ld (hl), c
// 		inc hl
// 		inc e
// 		ld a, (hl)
// 		ld (de), a
// 		ld (hl), b
// 		inc hl
// 		inc e
// 		;;;;;;;;;;;;;;;;
// 		;; 2 bytes ;;;;;
// 		pop bc
// 		ld a, (hl)
// 		ld (de), a
// 		ld (hl), c
// 		inc hl
// 		inc e
// 		ld a, (hl)
// 		ld (de), a
// 		ld (hl), b
// 		inc hl
// 		inc e
// 		;;;;;;;;;;;;;;;;
// 		;; 2 bytes ;;;;;
// 		pop bc
// 		ld a, (hl)
// 		ld (de), a
// 		ld (hl), c
// 		inc hl
// 		inc e
// 		ld a, (hl)
// 		ld (de), a
// 		ld (hl), b
// 		inc hl
// 		inc e
// 		;;;;;;;;;;;;;;;;
// 		;; 2 bytes ;;;;;
// 		pop bc
// 		ld a, (hl)
// 		ld (de), a
// 		ld (hl), c
// 		inc hl
// 		inc e
// 		ld a, (hl)
// 		ld (de), a
// 		ld (hl), b
// 		inc hl
// 		inc e
// 		;;;;;;;;;;;;;;;;
// 		;; 2 bytes ;;;;;
// 		pop bc
// 		ld a, (hl)
// 		ld (de), a
// 		ld (hl), c
// 		inc hl
// 		inc e
// 		ld a, (hl)
// 		ld (de), a
// 		ld (hl), b
// 		inc hl
// 		inc e
// 		;;;;;;;;;;;;;;;;
// 		;; 2 bytes ;;;;;
// 		pop bc
// 		ld a, (hl)
// 		ld (de), a
// 		ld (hl), c
// 		inc hl
// 		inc e
// 		ld a, (hl)
// 		ld (de), a
// 		ld (hl), b
// 		inc hl
// 		inc e
// 		;;;;;;;;;;;;;;;;
// 		;; 2 bytes ;;;;;
// 		pop bc
// 		ld a, (hl)
// 		ld (de), a
// 		ld (hl), c
// 		inc hl
// 		inc e
// 		ld a, (hl)
// 		ld (de), a
// 		ld (hl), b
// 		inc hl
// 		inc e
// 		;;;;;;;;;;;;;;;;
// 		;; 2 bytes ;;;;;
// 		pop bc
// 		ld a, (hl)
// 		ld (de), a
// 		ld (hl), c
// 		inc hl
// 		inc e
// 		ld a, (hl)
// 		ld (de), a
// 		ld (hl), b
// 		inc hl
// 		inc e
// 		;;;;;;;;;;;;;;;;
// 		;; 2 bytes ;;;;;
// 		pop bc
// 		ld a, (hl)
// 		ld (de), a
// 		ld (hl), c
// 		inc hl
// 		inc e
// 		ld a, (hl)
// 		ld (de), a
// 		ld (hl), b
// 		inc hl
// 		inc e
// 		;;;;;;;;;;;;;;;;
// 		;; 2 bytes ;;;;;
// 		pop bc
// 		ld a, (hl)
// 		ld (de), a
// 		ld (hl), c
// 		inc hl
// 		inc e
// 		ld a, (hl)
// 		ld (de), a
// 		ld (hl), b
// 		inc hl
// 		inc e
// 		;;;;;;;;;;;;;;;;
// 		;; 2 bytes ;;;;;
// 		pop bc
// 		ld a, (hl)
// 		ld (de), a
// 		ld (hl), c
// 		inc hl
// 		inc e
// 		ld a, (hl)
// 		ld (de), a
// 		ld (hl), b
// 		inc hl
// 		inc e
// 		;;;;;;;;;;;;;;;;
// 		ld a, e
// 		sub 32
// 		ld e, a
// 		inc d
// 		ld a, d
// 		and 7
// 		jr nz, l_transfer_and_restore_vdisplay_2
// 		ld a, e
// 		add a, 32
// 		ld e, a
// 		jr c, l_transfer_and_restore_vdisplay_2
// 		ld  a, d
// 		sub 8
// 		ld d, a
// 	l_transfer_and_restore_vdisplay_2:
// 		ex af, af
// 		dec a
// 		jp nz, l_transfer_and_restore_vdisplay_1

// 		ld sp, (_sprgbl_guarda_sp)
// 		ei
// 	#endasm
// }

void sprite_erase_with_zeros(t_sprite *psprite)
{
	sprgbl_psprite = psprite;

	if(((sprgbl_psprite->last_y) == 255) || !(sprgbl_psprite->moved_or_changed)) return; // el sprite no fue dibujado en el buffer virtual, no debe ser restaurado el buffer

	sprgbl_p_vdisp = vdisplay_bin_buff + (((unsigned int)(sprgbl_psprite->last_y))<<5) + (((unsigned int)(sprgbl_psprite->last_x))>>3);
	sprgbl_iWidth = sprgbl_psprite->width;
	if((sprgbl_psprite->last_x) & 7) sprgbl_iWidth++;
	sprgbl_height = sprgbl_psprite->height;
	sprgbl_incr_v_ptr_disp = 32 + sprgbl_iWidth;

	
	#asm
		di
		ld (_sprgbl_guarda_sp), sp

		ld a, (_sprgbl_height)
		ld b, a
		ld hl, (_sprgbl_p_vdisp)

		ld a, (_sprgbl_iWidth)
		cp 1
		jr z, l_sprite_erase_w_zeros_1_byte

		ld de, (_sprgbl_iWidth)
		add hl, de
		ld sp, hl

		cp 7
		jr c, l_sprite_erase_w_zeros_between_2_and_6_bytes

		exx
		ld de, (_sprgbl_incr_v_ptr_disp)
		exx
		sra a
		jr l_sprite_erase_w_zeros_7_or_more_bytes

	l_sprite_erase_w_zeros_1_byte:
		ld de, 32
		xor a

	l_sprite_erase_w_zeros_1_byte_loop:
		ld (hl), a
		add hl, de
		djnz l_sprite_erase_w_zeros_1_byte_loop

		jr l_sprite_erase_w_zeros_end

	l_sprite_erase_w_zeros_between_2_and_6_bytes:
		ld hl, (_sprgbl_incr_v_ptr_disp)
		ld c, l
		ld de, 0
		cp 2
		jr z, l_sprite_erase_w_zeros_2_bytes
		cp 3
		jr z, l_sprite_erase_w_zeros_3_bytes
		cp 4
		jr z, l_sprite_erase_w_zeros_4_bytes
		cp 5
		jr z, l_sprite_erase_w_zeros_5_bytes
		jr l_sprite_erase_w_zeros_6_bytes
		
	l_sprite_erase_w_zeros_2_bytes:
		ld a, h
	l_sprite_erase_w_zeros_2_bytes_loop:
		push de
		ld h, a
		ld l, c
		add hl, sp
		ld sp, hl
		djnz l_sprite_erase_w_zeros_2_bytes_loop

		jr l_sprite_erase_w_zeros_end

	l_sprite_erase_w_zeros_3_bytes:
		ld a, h
	l_sprite_erase_w_zeros_3_bytes_loop:
		push de
		inc sp
		push de
		ld h, a
		ld l, c
		add hl, sp
		ld sp, hl
		djnz l_sprite_erase_w_zeros_3_bytes_loop

		jr l_sprite_erase_w_zeros_end

	l_sprite_erase_w_zeros_4_bytes:
		ld a, h
	l_sprite_erase_w_zeros_4_bytes_loop:
		push de
		push de
		ld h, a
		ld l, c
		add hl, sp
		ld sp, hl
		djnz l_sprite_erase_w_zeros_4_bytes_loop

		jr l_sprite_erase_w_zeros_end
		
	l_sprite_erase_w_zeros_5_bytes:
		ld a, h
	l_sprite_erase_w_zeros_5_bytes_loop:
		push de
		push de
		inc sp
		push de
		ld h, a
		ld l, c
		add hl, sp
		ld sp, hl
		djnz l_sprite_erase_w_zeros_5_bytes_loop

		jr l_sprite_erase_w_zeros_end

	l_sprite_erase_w_zeros_6_bytes:
		ld a, h
	l_sprite_erase_w_zeros_6_bytes_loop:
		push de
		push de
		push de
		ld h, a
		ld l, c
		add hl, sp
		ld sp, hl
		djnz l_sprite_erase_w_zeros_6_bytes_loop

		jr l_sprite_erase_w_zeros_end

	l_sprite_erase_w_zeros_7_or_more_bytes:
		ld hl, l_address_pushes_erase_w_zeros_7_or_more_bytes + 16
		ld e, a
		ld d, 0
		ex af, af
		and a
		sbc hl, de
		ld e, 0

	l_sprite_erase_w_zeros_7_or_more_bytes_loop:
		jp (hl)
	l_address_pushes_erase_w_zeros_7_or_more_bytes:
		push de
		push de
		push de
		push de
		push de
		push de
		push de
		push de
		push de
		push de
		push de
		push de
		push de
		push de
		push de
		push de
		ex af, af
		jr nc, l_address_pushes_erase_w_zeros_7_or_more_bytes_even
		inc sp
		push de
	l_address_pushes_erase_w_zeros_7_or_more_bytes_even:
		ex af,af
		exx
		ld h, d
		ld l, e
		add hl, sp
		ld sp, hl
		exx
		djnz l_sprite_erase_w_zeros_7_or_more_bytes_loop

	l_sprite_erase_w_zeros_end:

		ld sp, (_sprgbl_guarda_sp)
		ei
	#endasm
}