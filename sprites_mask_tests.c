#include "sprites_mask_tests.h"

unsigned char background_vdisplay_bin_buff[192*32]; // buffer imagen binaria
unsigned char vdisplay_bin_buff[192*32 + 1]; // buffer imagen binaria

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

void sprite_init_background(void)
{
	int i;
	byte *p_attribs_buffer = (byte *)0x5800;

	for(i=0; i<0x1800; i++)
	{
		if(i & 0x20)
		{
			background_vdisplay_bin_buff[i] = 0x55;
	  		vdisplay_bin_buff[i] = 0x55;
		}
		else
		{
			background_vdisplay_bin_buff[i] = 0xaa;
	  		vdisplay_bin_buff[i] = 0xaa;
		}
	}

	for(; p_attribs_buffer < ((byte *)0x5b00); p_attribs_buffer++)
	{
		*p_attribs_buffer = 7;
	}
}

void sprite_init(void)
{
	precalculate_flip_byte_table();
	precalculate_shift_tables();
}

void sprite_set_graphic_def(t_sprite *psprite, t_sprite_graphic_def *psprite_graphdef)
{
	if(psprite_graphdef)
	{
		psprite->required_graphic_state = SPRITE_GRAPHIC_STATE_FLIPPED_RIGHT;
		psprite->width = psprite_graphdef->width;
		psprite->height = psprite_graphdef->height;
		psprite->delta_sprite_x = psprite_graphdef->delta_sprite_x;
		psprite->delta_sprite_y = psprite_graphdef->delta_sprite_y;
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
}

void sprite_next_frame(t_sprite *psprite)
{
	psprite->actual_frame += psprite->frame_size;
	if((psprite->actual_frame) >= (psprite->last_frame))
	{
		psprite->actual_frame = psprite->first_frame;
	}
}

int guarda_sp;
byte width;
byte height;
byte *p_gbd;
byte *p_vdisp;
byte *p_disp;
byte p_shift_table_high;
byte mask_start;
byte mask_end;
int incr_v_ptr_disp;
int iWidth;
byte mask_start_array[] = {0, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe};
byte mask_end_array[] = {0xff, 0x7f, 0x3f, 0x1f, 0x0f, 0x7, 0x03, 0x01};
byte half_width;

void sprite_flip_h(void)
{
	// Tamaño en byte: 160 bytes
	#asm
		di
		ld (_guarda_sp), sp

		ld hl, (_p_gbd)
		ld a, (_width)
		ld c, a
		dec c
		ld b, 0
		add hl, bc
		add hl, bc
		ld d, 0xef ; tabla de inversion de orden de bit de un byte en direccion 0xe000
		exx
		ld hl, (_p_gbd)
		ld d, 0xef ; tabla de inversion de orden de bit de un byte en direccion 0xe000
		ld a, (_width)
		rra
		ld (_half_width), a
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
		ld a, (_half_width)
		ex af, af
		ld a, (_height)

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
		ld a, (_half_width)
		ex af, af
		dec a
		jr nz, l_sprite_flip_h_3
		
		ld a, (_width)
		rra
		jr nc, l_sprite_flip_h_6
		; el numero de parejas byte / mascara en cada linea es impar, luego hay una colunna central que no ha sido flipada aun
		ld hl, (_p_gbd)
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
		ld a, (_height)
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
		ld sp, (_guarda_sp)
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
	byte x0, x1, aux;

	width = psprite->width;
	height = psprite->height;
	p_gbd = psprite->actual_frame;

	// actualizar al fipado horizontal requerido /////////////
	aux = (psprite->required_graphic_state) & SPRITE_GRAPHIC_STATE_MASK_FLIPPED_H;
	if( aux != (*p_gbd & SPRITE_GRAPHIC_STATE_MASK_FLIPPED_H) )
	{
		*p_gbd = (*p_gbd & ~SPRITE_GRAPHIC_STATE_MASK_FLIPPED_H) | aux;
		p_gbd++;
		sprite_flip_h();
	}
	else
	{
		p_gbd++;
	}
	/////////////////////////////////////////////////////////

	p_vdisp = vdisplay_bin_buff + (((unsigned int)(psprite->pos_y))<<5) + (((unsigned int)(psprite->pos_x))>>3);
	p_shift_table_high = 0xf0 | (((psprite->pos_x)&7)<<1);
	mask_start = mask_start_array[(psprite->pos_x)&7];
	mask_end = mask_end_array[(psprite->pos_x)&7];
	incr_v_ptr_disp = 32 - width;

	#asm
		di
		ld (_guarda_sp), sp
		ld sp,(_p_gbd)

        ld a ,(_p_shift_table_high)
		ld h, a
		ld de, (_p_vdisp)

		exx
		ld a, (_height)
		ld c, a
		ld a, (_width)
		ld d, a
		ld b, a

		ld a, (_mask_start)
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
		ld a, (_mask_end)           ; 13
		ld c, a                     ; 4
		ex af, af                   ; 4
		or c                        ; 4
		ex de,hl                    ; 4
		and (hl)                    ; 7
		or b                        ; 4
		ld (hl),a                   ; 7
		ld bc, (_incr_v_ptr_disp)   ; 20
		add hl, bc                  ; 11
		ex de, hl                   ; 4

		ld a, (_mask_start)         ; 13
		ex af, af                   ; 4
		xor a                       ; 4
		
		exx                         ; 4
		ld b, d                     ; 4
		dec c                       ; 4
		jr nz, l_draw_sprite_1      ; 12 / 7

		ld sp, (_guarda_sp)
		ei

	#endasm

	if(psprite->last_y != 255)
	{
		x0 = psprite->last_x;
		x1 = psprite->pos_x;
		if(x0 > x1)
		{
			aux = x1;
			x1 = x0;
			x0 = aux;
		}
		psprite->to_update_x = x0;
		psprite->to_update_width = psprite->width;
		if(x1 & 7) psprite->to_update_width++;
		if( (x1-x0) > (x1 & 7) )
		{
			psprite->to_update_width += (((x1 & 0xf8) - x0 )>>3);
			if(x0 & 7) psprite->to_update_width++;
		}

		x0 = psprite->last_y;
		x1 = psprite->pos_y;
		if(x0 > x1)
		{
			aux = x1;
			x1 = x0;
			x0 = aux;
		}
		psprite->to_update_y = x0;
		psprite->to_update_height = psprite->height;
		psprite->to_update_height += (x1 - x0 );
	}
	else
	{
		psprite->to_update_x = psprite->pos_x;
		psprite->to_update_y = psprite->pos_y;
		psprite->to_update_width = psprite->width;
		psprite->to_update_height = psprite->height;
	}

	psprite->last_x = psprite->pos_x;
	psprite->last_y = psprite->pos_y;


}

void sprite_update_display(t_sprite *psprite)
{
	p_vdisp = vdisplay_bin_buff + (((unsigned int)(psprite->to_update_y))<<5) + (((unsigned int)(psprite->to_update_x))>>3);
	p_disp = (byte *) (0x4000 + 
			((((unsigned int)(psprite->to_update_y)) & 0xc0) << 5) + ((((unsigned int)(psprite->to_update_y))&0x7)<<8) +
			((((unsigned int)(psprite->to_update_y))&0x38)<<2) + (((unsigned int)(psprite->to_update_x))>>3) );
	iWidth = psprite->to_update_width;
	height = psprite->to_update_height;
	incr_v_ptr_disp = 32 - iWidth;

	#asm
		di

		ld hl, (_p_vdisp)
		ld de, (_p_disp)
		ld a, (_height)
	l_update_display_sprite_1:
		ex af, af
		ld bc, (_iWidth)
		ldir
		ld bc, (_incr_v_ptr_disp)
		add hl, bc
		ld de, (_p_disp)
		inc d
		ld a, d
		and 7
		jr nz, l_update_display_sprite_2
		ld a, e
		add a, 0x20
		ld e, a
		jr c, l_update_display_sprite_2
		ld a, d
		sub a, 8
		ld d, a
	l_update_display_sprite_2:
	    ld (_p_disp), de
		ex af, af
		dec a
		jr nz, l_update_display_sprite_1

		ei

	#endasm
}

void sprite_restore_vdisplay(t_sprite *psprite)
{
	if((psprite->last_y) == 255) return; // el sprite no fue dibujado en el buffer virtual, no debe ser restaurado el buffer

	p_disp = background_vdisplay_bin_buff + (((unsigned int)(psprite->last_y))<<5) + (((unsigned int)(psprite->last_x))>>3);
	p_vdisp = vdisplay_bin_buff + (((unsigned int)(psprite->last_y))<<5) + (((unsigned int)(psprite->last_x))>>3);
	iWidth = psprite->width;
	if((psprite->last_x) & 7) iWidth++;
	height = psprite->height;
	incr_v_ptr_disp = 32 - iWidth;

	#asm
		di

		ld hl, (_p_disp)                               ; 16
		ld de, (_p_vdisp)                              ; 20
		ld a, (_height)                                ; 13
	l_restore_vdisplay_sprite_1:
		ld bc, (_iWidth)                               ; 20
		ldir                                           ; 21 / 16   (bc * 21 - 5)
		ld bc, (_incr_v_ptr_disp)                      ; 20
		add hl, bc                                     ; 11
		ex de,hl                                       ; 4
		add hl, bc                                     ; 11
		ex de,hl                                       ; 4
		dec a                                          ; 4
		jr nz, l_restore_vdisplay_sprite_1             ; 12/7

		ei

	#endasm
}

void sprite_transfer_vdisplay(void)
{

	#asm
		ld hl, _vdisplay_bin_buff
		ld de, 0x4000
		ld b, 192
	l_transfer_vdisplay_1:
		push bc
		push de
		;ld bc, 32
		;ldir
		ldi
		ldi
		ldi
		ldi
		ldi
		ldi
		ldi
		ldi
		ldi
		ldi
		ldi
		ldi
		ldi
		ldi
		ldi
		ldi
		ldi
		ldi
		ldi
		ldi
		ldi
		ldi
		ldi
		ldi
		ldi
		ldi
		ldi
		ldi
		ldi
		ldi
		ldi
		ldi
		pop de
		inc d
		ld a, d
		and 7
		jr nz, l_transfer_vdisplay_2
		ld a, e
		add a, 32
		ld e, a
		jr c, l_transfer_vdisplay_2
		ld  a, d
		sub 8
		ld d, a
	l_transfer_vdisplay_2:
		pop bc
		djnz l_transfer_vdisplay_1
	#endasm
}

void sprite_transfer_and_restore_vdisplay(void)
{

	#asm
		di
		ld (_guarda_sp), sp
		ld sp, _background_vdisplay_bin_buff
		ld hl, _vdisplay_bin_buff
		ld de, 0x4000
		ld a, 192
	l_transfer_and_restore_vdisplay_1:
		ex af, af
		;; 2 bytes ;;;;;
		pop bc                     ; 10
		ld a, (hl)                 ; 7
		ld (de), a                 ; 7
		ld (hl), c                 ; 7
		inc hl                     ; 6
		inc e                      ; 4
		ld a, (hl)                 
		ld (de), a
		ld (hl), b
		inc hl
		inc e
		;;;;;;;;;;;;;;;;
		;; 2 bytes ;;;;;
		pop bc
		ld a, (hl)
		ld (de), a
		ld (hl), c
		inc hl
		inc e
		ld a, (hl)
		ld (de), a
		ld (hl), b
		inc hl
		inc e
		;;;;;;;;;;;;;;;;
		;; 2 bytes ;;;;;
		pop bc
		ld a, (hl)
		ld (de), a
		ld (hl), c
		inc hl
		inc e
		ld a, (hl)
		ld (de), a
		ld (hl), b
		inc hl
		inc e
		;;;;;;;;;;;;;;;;
		;; 2 bytes ;;;;;
		pop bc
		ld a, (hl)
		ld (de), a
		ld (hl), c
		inc hl
		inc e
		ld a, (hl)
		ld (de), a
		ld (hl), b
		inc hl
		inc e
		;;;;;;;;;;;;;;;;
		;; 2 bytes ;;;;;
		pop bc
		ld a, (hl)
		ld (de), a
		ld (hl), c
		inc hl
		inc e
		ld a, (hl)
		ld (de), a
		ld (hl), b
		inc hl
		inc e
		;;;;;;;;;;;;;;;;
		;; 2 bytes ;;;;;
		pop bc
		ld a, (hl)
		ld (de), a
		ld (hl), c
		inc hl
		inc e
		ld a, (hl)
		ld (de), a
		ld (hl), b
		inc hl
		inc e
		;;;;;;;;;;;;;;;;
		;; 2 bytes ;;;;;
		pop bc
		ld a, (hl)
		ld (de), a
		ld (hl), c
		inc hl
		inc e
		ld a, (hl)
		ld (de), a
		ld (hl), b
		inc hl
		inc e
		;;;;;;;;;;;;;;;;
		;; 2 bytes ;;;;;
		pop bc
		ld a, (hl)
		ld (de), a
		ld (hl), c
		inc hl
		inc e
		ld a, (hl)
		ld (de), a
		ld (hl), b
		inc hl
		inc e
		;;;;;;;;;;;;;;;;
		;; 2 bytes ;;;;;
		pop bc
		ld a, (hl)
		ld (de), a
		ld (hl), c
		inc hl
		inc e
		ld a, (hl)
		ld (de), a
		ld (hl), b
		inc hl
		inc e
		;;;;;;;;;;;;;;;;
		;; 2 bytes ;;;;;
		pop bc
		ld a, (hl)
		ld (de), a
		ld (hl), c
		inc hl
		inc e
		ld a, (hl)
		ld (de), a
		ld (hl), b
		inc hl
		inc e
		;;;;;;;;;;;;;;;;
		;; 2 bytes ;;;;;
		pop bc
		ld a, (hl)
		ld (de), a
		ld (hl), c
		inc hl
		inc e
		ld a, (hl)
		ld (de), a
		ld (hl), b
		inc hl
		inc e
		;;;;;;;;;;;;;;;;
		;; 2 bytes ;;;;;
		pop bc
		ld a, (hl)
		ld (de), a
		ld (hl), c
		inc hl
		inc e
		ld a, (hl)
		ld (de), a
		ld (hl), b
		inc hl
		inc e
		;;;;;;;;;;;;;;;;
		;; 2 bytes ;;;;;
		pop bc
		ld a, (hl)
		ld (de), a
		ld (hl), c
		inc hl
		inc e
		ld a, (hl)
		ld (de), a
		ld (hl), b
		inc hl
		inc e
		;;;;;;;;;;;;;;;;
		;; 2 bytes ;;;;;
		pop bc
		ld a, (hl)
		ld (de), a
		ld (hl), c
		inc hl
		inc e
		ld a, (hl)
		ld (de), a
		ld (hl), b
		inc hl
		inc e
		;;;;;;;;;;;;;;;;
		;; 2 bytes ;;;;;
		pop bc
		ld a, (hl)
		ld (de), a
		ld (hl), c
		inc hl
		inc e
		ld a, (hl)
		ld (de), a
		ld (hl), b
		inc hl
		inc e
		;;;;;;;;;;;;;;;;
		;; 2 bytes ;;;;;
		pop bc
		ld a, (hl)
		ld (de), a
		ld (hl), c
		inc hl
		inc e
		ld a, (hl)
		ld (de), a
		ld (hl), b
		inc hl
		inc e
		;;;;;;;;;;;;;;;;
		ld a, e
		sub 32
		ld e, a
		inc d
		ld a, d
		and 7
		jr nz, l_transfer_and_restore_vdisplay_2
		ld a, e
		add a, 32
		ld e, a
		jr c, l_transfer_and_restore_vdisplay_2
		ld  a, d
		sub 8
		ld d, a
	l_transfer_and_restore_vdisplay_2:
		ex af, af
		dec a
		jp nz, l_transfer_and_restore_vdisplay_1

		ld sp, (_guarda_sp)
		ei
	#endasm
}