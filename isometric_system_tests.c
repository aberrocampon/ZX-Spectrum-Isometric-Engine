
#include "isometric_system_tests.h"

byte isometric_origen_proj_x = ISOMETRIC_ORIGEN_PROJ_X_DEFFAULT;
byte isometric_origen_proj_y = ISOMETRIC_ORIGEN_PROJ_Y_DEFFAULT;

t_isometric_obj *isometric_objects_table[N_MAX_ISOMETRIC_OBJECTS];
byte n_isometric_objects = 0;
byte n_bakckground_isometric_objects = 0;

t_isometric_obj created_isometric_objects_table[N_MAX_CREATED_ISOMETRIC_OBJECTS];
byte n_created_isometric_objects = 0;

int height_over_proj_plane[N_MAX_ORDERED_ISOMETRIC_OBJECTS];
t_isometric_obj *ordered_isometric_objects_table[N_MAX_ORDERED_ISOMETRIC_OBJECTS];
byte n_ordered_isometric_objects = 0;

t_isometric_obj *isogbl_p_isometric_obj;
t_sprite *isogbl_psprite1, *isogbl_psprite2;
t_sprite *isogbl_psprite3, *isogbl_psprite4;

void isometric_add_object_to_table(t_isometric_obj *p_isometric_obj)
{
	if(n_isometric_objects >= N_MAX_ISOMETRIC_OBJECTS) return;

    // Añade a la tabla de objetos fisicos
	isogbl_p_isometric_obj = p_isometric_obj;
    isometric_objects_table[n_isometric_objects++] = isogbl_p_isometric_obj;
	isometric_proj_obj();
	if(n_isometric_objects <= n_bakckground_isometric_objects)
	{
		isogbl_psprite1 = &(isogbl_p_isometric_obj->sprite);
		isogbl_psprite1->not_moved_rect_max_x = isogbl_psprite1->pos_x + isogbl_psprite1->width_px - 1;
		isogbl_psprite1->not_moved_rect_max_y = isogbl_psprite1->pos_y + isogbl_psprite1->height - 1;
		isometric_add_object_to_order();
	}
	else
	{
		n_phys_box3d_objects++;
	}
}

t_isometric_obj *isometric_create_object(byte graphic_type_index, t_isometric_obj_graphic_def *p_isometric_obj_def, byte initial_required_graphic_state,
						t_b_vec3d *p_init_pos, byte physics_enabled, byte init_flags, void (*behavior)(t_isometric_obj **), byte behavior_parameter)
{
	byte i;
	byte *p;
	t_isometric_obj *p_isometric_obj;

	if(n_created_isometric_objects >= N_MAX_CREATED_ISOMETRIC_OBJECTS) return NULL;

	p_isometric_obj = &created_isometric_objects_table[n_created_isometric_objects++];

	p_isometric_obj->graphic_type_index = graphic_type_index;

	p_isometric_obj->physics.box3d.pos_x = p_isometric_obj->physics.last_pos_x = p_init_pos->x;
	p_isometric_obj->physics.box3d.pos_y = p_isometric_obj->physics.last_pos_y = p_init_pos->y;
	p_isometric_obj->physics.box3d.pos_z = p_isometric_obj->physics.last_pos_z = p_init_pos->z;
	p_isometric_obj->physics.box3d.width_x = p_isometric_obj_def->size.x;
	p_isometric_obj->physics.box3d.width_y = p_isometric_obj_def->size.y;
	p_isometric_obj->physics.box3d.height = p_isometric_obj_def->size.z;
	p_isometric_obj->physics.speed_x = 0;
	p_isometric_obj->physics.speed_y = 0;
	p_isometric_obj->physics.speed_z = 0;
	p_isometric_obj->physics.enabled = physics_enabled;
	p_isometric_obj->physics.gravity_count = 1;
	p_isometric_obj->physics.flags = init_flags; // Flags de propiedades dinamicas del objeto aplicado al motor de fisicas

	p = (byte *)&(p_isometric_obj->behavior_variables);
	for(i = 0; i < sizeof(p_isometric_obj->behavior_variables); i++)
	{
		*p++ = 0;
	}
	p_isometric_obj->bahavior_parameter = behavior_parameter;
	p_isometric_obj->behavior = behavior;

	p_isometric_obj->sprite.last_y = 255; // Aun no ha sido dibujado, no es necesario borrarlo antes de dibujarlo en el buffer virtual o tranferir la zona que ocupaba al frame buffer visible para borrarlo
	sprite_set_graphic_def(&p_isometric_obj->sprite, initial_required_graphic_state, p_isometric_obj_def->p_sprite_def);

	return p_isometric_obj;

}

void isometric_create_and_add_objects_to_table(byte n_isometric_objects_def, t_isometric_object_def *isometric_objects_def_table, t_isometric_obj_graphic_def *isometric_obj_graphic_def_table)
{
	byte i;
	t_isometric_object_def *p_isometric_object_def;

	for(i = 0; i < n_isometric_objects_def; i++)
	{
		p_isometric_object_def = &isometric_objects_def_table[i];
		isometric_add_object_to_table(
										isometric_create_object(
																p_isometric_object_def->graphic_type_index,
																&isometric_obj_graphic_def_table[p_isometric_object_def->graphic_type_index],
																p_isometric_object_def->initial_required_graphic_state,
																&(p_isometric_object_def->init_pos), 
																p_isometric_object_def->physics_enabled,
																p_isometric_object_def->init_flags,
																p_isometric_object_def->behavior,
																p_isometric_object_def->behavior_parameter
																)
										);
	}
}

t_isometric_obj **isogbl_pp_isometric_obj, **isogbl_pp_isometric_obj2;
int8 isogbl_i, isogbl_j;
int8 isogbl_i_ordering;
byte ordered_index;
int height;

void isometric_proj_obj(void)
{
	isogbl_p_isometric_obj->sprite.pos_x_ref = 
				isometric_origen_proj_x -
				isogbl_p_isometric_obj->physics.box3d.pos_x + 
				isogbl_p_isometric_obj->physics.box3d.pos_y;
	isogbl_p_isometric_obj->sprite.pos_y_ref = 
				isometric_origen_proj_y +
				((isogbl_p_isometric_obj->physics.box3d.pos_x)>>1) + 
				((isogbl_p_isometric_obj->physics.box3d.pos_y)>>1) -
				isogbl_p_isometric_obj->physics.box3d.pos_z;

	sprite_set_pos_from_posref(&(isogbl_p_isometric_obj->sprite));
}

void isometric_add_object_to_order(void)
{
	if(n_ordered_isometric_objects >= N_MAX_ORDERED_ISOMETRIC_OBJECTS) return;

	// height = isogbl_p_isometric_obj->physics.box3d.pos_x + isogbl_p_isometric_obj->physics.box3d.pos_y;
	// height -= ((height>>2) + (height>>3));
	// height += isogbl_p_isometric_obj->physics.box3d.pos_z;
	#asm
		ld hl, (_isogbl_p_isometric_obj)
		inc hl
		ld a, (hl) ; Offset a pos_x es 1. Ademas pos_y y pos_z estan a continuacion consecutivos.
		inc hl
		add (hl)
		ld e, a
		ld a, 0
		adc a, 0
		ld d, a
		rra
		ld a, e
		rra
		srl a
		push hl
		ld l, a
		srl a
		add l
		ld l, a
		ld h, 0
		ex de, hl
		sbc hl, de
		ex de, hl
		pop hl
		inc hl
		ld l, (hl)
		ld h, 0
		add hl, de
		ld (_height), hl
	#endasm

	// for(isogbl_i_ordering = (n_ordered_isometric_objects - 1); isogbl_i_ordering >= n_bakckground_isometric_objects; isogbl_i_ordering--)
	// {
	// 	if(height_over_proj_plane[isogbl_i_ordering] < height) break;
	// }

	// ordered_index = isogbl_i_ordering + 1;
	#asm
		ld a, (_n_ordered_isometric_objects)
		ld l, a
		ld h, 0
		dec l
		add hl, hl
		ld bc, _height_over_proj_plane
		add hl, bc
		inc hl
		ex de, hl
		ld hl, _n_bakckground_isometric_objects
		sub (hl)
		ld b, a
		jr c, l_isometric_add_object_to_order_result_index
		jr z, l_isometric_add_object_to_order_result_index
		ld hl, (_height)
		ex de, hl
	l_isometric_add_object_to_order_loop:
		ld a, d
	l_isometric_add_object_to_order_loop2:
		cp (hl)
		jr z, l_isometric_add_object_to_order_compare_height_low_byte
		jr nc, l_isometric_add_object_to_order_result_index
		dec hl
		dec hl
		djnz l_isometric_add_object_to_order_loop2
		jr l_isometric_add_object_to_order_result_index
	l_isometric_add_object_to_order_loop_continue:
		dec hl
		djnz l_isometric_add_object_to_order_loop
		jr l_isometric_add_object_to_order_result_index
	l_isometric_add_object_to_order_compare_height_low_byte:
		dec hl
		ld a, e
		cp (hl)
		jr c, l_isometric_add_object_to_order_loop_continue
		jr z, l_isometric_add_object_to_order_loop_continue
	l_isometric_add_object_to_order_result_index:
		ld a, (_n_bakckground_isometric_objects)
		add b
		ld (_ordered_index), a
	#endasm

	// isogbl_i_ordering = 0; // compiler bug !!!
	// for(isogbl_i_ordering = n_ordered_isometric_objects; isogbl_i_ordering > ordered_index; isogbl_i_ordering--)
	// {
	// 	height_over_proj_plane[isogbl_i_ordering] = height_over_proj_plane[isogbl_i_ordering - 1];
	// 	ordered_isometric_objects_table[isogbl_i_ordering] = ordered_isometric_objects_table[isogbl_i_ordering - 1];
	// }
	#asm
		ld hl, (_n_ordered_isometric_objects)
		ld h, 0
		add hl, hl
		ld d, h
		ld e, l
		;;ld a, (_ordered_index)
		ld c, a
		ld b, 0
		sbc hl, bc
		sbc hl, bc
		jr z, l_isometric_add_object_to_order_not_to_move
		ld b, h
		ld c, l
		push bc
		inc de
		push de
		ld hl, _height_over_proj_plane
		add hl, de
		ld d, h
		ld e, l
		dec hl
		dec hl
		lddr
		pop hl
		ld de, _ordered_isometric_objects_table
		add hl, de
		ld d, h
		ld e, l
		dec hl
		dec hl
		pop bc
		lddr
	l_isometric_add_object_to_order_not_to_move:
	#endasm

	ordered_isometric_objects_table[ordered_index] = isogbl_p_isometric_obj;
	height_over_proj_plane[ordered_index] = height;

	n_ordered_isometric_objects++;
}

void isometric_step(void)
{
	phys_box3d_step();

	for(isogbl_i = 0, isogbl_pp_isometric_obj = isometric_objects_table; isogbl_i < n_isometric_objects; isogbl_i++, isogbl_pp_isometric_obj++)
	{
		isogbl_p_isometric_obj = *isogbl_pp_isometric_obj;

		if(isogbl_p_isometric_obj->behavior)
		{
			(isogbl_p_isometric_obj->behavior)(isogbl_pp_isometric_obj);
		}
	}

	isometric_reset_objects_ordering(n_bakckground_isometric_objects);
	for(isogbl_i = n_bakckground_isometric_objects, isogbl_pp_isometric_obj = isometric_objects_table + n_bakckground_isometric_objects; isogbl_i < n_isometric_objects; isogbl_i++, isogbl_pp_isometric_obj++)
	{
		isogbl_p_isometric_obj = *isogbl_pp_isometric_obj;
		isogbl_psprite1 = &(isogbl_p_isometric_obj->sprite);

		if(isogbl_psprite1->actual_frame)
		{
			
			if((isogbl_p_isometric_obj->physics.box3d.pos_x != isogbl_p_isometric_obj->physics.last_pos_x) || 
				(isogbl_p_isometric_obj->physics.box3d.pos_y != isogbl_p_isometric_obj->physics.last_pos_y) || (isogbl_p_isometric_obj->physics.box3d.pos_z != isogbl_p_isometric_obj->physics.last_pos_z))
			{
				isogbl_psprite1->moved_or_changed = 1;
				isometric_proj_obj();
			}

			// if(isogbl_psprite1->moved_or_changed)
			// {
			// 	sprite_erase_with_zeros(isogbl_psprite1);

			// 	// calcular y guardar coordenadas de rectangulos de borrado para el chequeo de sprites 
			// 	// no movidos pero que deben redibujarse porque solapan con uno que se borra y mueve
			// 	isogbl_psprite1->erase_rect_min_x = (isogbl_psprite1->last_x) & 0xf8;
			// 	isogbl_psprite1->erase_rect_max_x = (isogbl_psprite1->last_x + isogbl_psprite1->width_px - 1) | 7;
			// 	isogbl_psprite1->erase_rect_max_y = isogbl_psprite1->last_y + isogbl_psprite1->height - 1;
			// }
			// else
			// {
			// 	isogbl_psprite1->not_moved_rect_max_x = isogbl_psprite1->pos_x + isogbl_psprite1->width_px - 1;
			// 	isogbl_psprite1->not_moved_rect_max_y = isogbl_psprite1->pos_y + isogbl_psprite1->height - 1;
			// }
			#asm
				ld hl, (_isogbl_psprite1)
				ld bc, T_SPRITE_OFFSET_MOVED_OR_CHANGED
				add hl, bc
				ld a, (hl)
				or a
				jr z, l_isomectric_step_sprite_not_moved_calculations
				ld hl, (_isogbl_psprite1)
				call _sprite_erase_with_zeros
				ld hl, (_isogbl_psprite1)
				ld bc, T_SPRITE_OFFSET_LAST_X
				add hl, bc
				ld a, (hl)
				and 0xf8
				ld d, h
				ld e, l
				ld bc, T_SPRITE_OFFSET_ERASE_RECT_MIN_X - T_SPRITE_OFFSET_LAST_X
				add hl, bc
				ld (hl), a
				ex de, hl
				ld a, (hl)
				ld bc, T_SPRITE_OFFSET_WIDTH_PX - T_SPRITE_OFFSET_LAST_X
				add hl, bc
				add (hl)
				dec a
				or 7
				ex de, hl
				inc hl
				ld (hl), a
				ex de, hl
				ld bc, T_SPRITE_OFFSET_LAST_Y - T_SPRITE_OFFSET_WIDTH_PX
				add hl, bc
				ld a, (hl)
				ld bc, T_SPRITE_OFFSET_HEIGHT - T_SPRITE_OFFSET_LAST_Y
				add hl, bc
				add (hl)
				dec a
				ex de, hl
				inc hl
				ld (hl), a
				jr l_isomectric_step_sprite_end_calculations
			l_isomectric_step_sprite_not_moved_calculations:
				ld bc, T_SPRITE_OFFSET_POS_X - T_SPRITE_OFFSET_MOVED_OR_CHANGED
				add hl, bc
				ld a, (hl)
				ld bc, T_SPRITE_OFFSET_WIDTH_PX - T_SPRITE_OFFSET_POS_X
				add hl, bc
				add (hl)
				dec a
				ld d, h
				ld e, l
				ld bc, T_SPRITE_OFFSET_NOT_MOVED_RECT_MAX_X - T_SPRITE_OFFSET_WIDTH_PX
				add hl, bc
				ld (hl), a
				ex de, hl
				ld bc, T_SPRITE_OFFSET_POS_Y - T_SPRITE_OFFSET_WIDTH_PX
				add hl, bc
				ld a, (hl)
				ld bc, T_SPRITE_OFFSET_HEIGHT - T_SPRITE_OFFSET_POS_Y
				add hl, bc
				add (hl)
				dec a
				ex de, hl
				inc hl
				ld (hl), a
			l_isomectric_step_sprite_end_calculations:
			#endasm
			
			isometric_add_object_to_order();
		}
	}

	// lo proximo parece que ocupa del orden de 80000 ciclos (room 1)
	// for(isogbl_i = 0, isogbl_pp_isometric_obj = ordered_isometric_objects_table; isogbl_i < n_ordered_isometric_objects - 1; isogbl_i++, isogbl_pp_isometric_obj++)
	// {
	// 	isogbl_psprite1 = &((*isogbl_pp_isometric_obj)->sprite);
	// 	for(isogbl_j = isogbl_i + 1, isogbl_pp_isometric_obj2 = ordered_isometric_objects_table + isogbl_i + 1; isogbl_j < n_ordered_isometric_objects; isogbl_j++, isogbl_pp_isometric_obj2++)
	// 	{
	// 		isogbl_psprite2 = &((*isogbl_pp_isometric_obj2)->sprite);
	// 		if(isogbl_psprite1->moved_or_changed)
	// 		{
	// 			if(isogbl_psprite2->moved_or_changed)
	// 			{
	// 				continue;
	// 			}
	// 			else
	// 			{
	// 				isogbl_psprite3 = isogbl_psprite1;
	// 				isogbl_psprite4 = isogbl_psprite2;
	// 				goto comprueba_sprite_borrado_sobre_sprite_inmovil;
	// 			}
	// 		}
	// 		else
	// 		{
	// 			if(isogbl_psprite2->moved_or_changed)
	// 			{
	// 				isogbl_psprite3 = isogbl_psprite2;
	// 				isogbl_psprite4 = isogbl_psprite1;
	// 				goto comprueba_sprite_borrado_sobre_sprite_inmovil;
	// 			}
	// 			else
	// 			{
	// 				if(!(isogbl_psprite1->redraw_not_moved)	|| (isogbl_psprite2->redraw_not_moved)) continue;
	// 				if( !((isogbl_psprite1->pos_x > isogbl_psprite2->not_moved_rect_max_x) ||
	// 					(isogbl_psprite1->not_moved_rect_max_x < isogbl_psprite2->pos_x) ||
	// 					(isogbl_psprite1->pos_y > isogbl_psprite2->not_moved_rect_max_y) ||
	// 					(isogbl_psprite1->not_moved_rect_max_y < isogbl_psprite2->pos_y)) )
	// 				{
	// 					isogbl_psprite2->redraw_not_moved = 1;
	// 				}
	// 			}
	// 		}
	// 		continue;
	// 		comprueba_sprite_borrado_sobre_sprite_inmovil:
	// 		if(isogbl_psprite4->redraw_not_moved) continue;
	// 		if( !((isogbl_psprite3->erase_rect_min_x > isogbl_psprite4->not_moved_rect_max_x) ||
	// 				(isogbl_psprite3->erase_rect_max_x < isogbl_psprite4->pos_x) ||
	// 				(isogbl_psprite3->last_y > isogbl_psprite4->not_moved_rect_max_y) ||
	// 				(isogbl_psprite3->erase_rect_max_y < isogbl_psprite4->pos_y)) )
	// 		{
	// 			isogbl_psprite4->redraw_not_moved = 1;
	// 		}
	// 	}
	// }
	#asm
		ld a, (_n_ordered_isometric_objects)
		cp 2
		jp c, l_isomectric_step_sprite_mark_redraw_end_loops
		ld a, 1
		ld (_isogbl_i), a
		ld hl, _ordered_isometric_objects_table
	l_isomectric_step_sprite_mark_redraw_extern_loop:
		ld e, (hl)
		inc hl
		ld d, (hl)
		inc hl
		ld (_isogbl_pp_isometric_obj), hl
		ld bc, T_ISOMETRIC_OBJ_OFFSET_SPRITE
		ex de, hl
		add hl, bc
		ld (_isogbl_psprite1), hl

	l_isomectric_step_sprite_mark_redraw_start_intern_loop:
		ld e, a
		ld a, (_n_ordered_isometric_objects)
		sub e
		ld b, a
		ld hl, _ordered_isometric_objects_table
		ld d, 0
		add hl, de
		add hl, de

	l_isomectric_step_sprite_mark_redraw_intern_loop:
		ld e, (hl)
		inc hl
		ld d, (hl)
		inc hl
		ld a, T_ISOMETRIC_OBJ_OFFSET_SPRITE
		add a, e
		ld e, a
		ld a, 0
		adc a, d
		ld d, a
		ld (_isogbl_psprite2), de
		exx

		ld hl, (_isogbl_psprite1)
		ld bc, T_SPRITE_OFFSET_MOVED_OR_CHANGED
		add hl, bc
		ld a, (hl)
		or a
		jr z, l_isomectric_step_sprite_mark_redraw_spr1_not_moved
		ex de, hl
		ld hl, (_isogbl_psprite2)
		add hl, bc
		ld a, (hl)
		or a
		jr z, l_isomectric_step_sprite_mark_redraw_check_moved_spr_and_unmoved_spr
	l_isomectric_step_sprite_mark_redraw_continue_intern_loop:
		exx
		djnz l_isomectric_step_sprite_mark_redraw_intern_loop

		ld a, (_n_ordered_isometric_objects)
		ld b, a
		ld a, (_isogbl_i)
		inc a
		ld (_isogbl_i), a
		cp b
		ld hl, (_isogbl_pp_isometric_obj)
		jp c, l_isomectric_step_sprite_mark_redraw_extern_loop

		jp l_isomectric_step_sprite_mark_redraw_end_loops

	l_isomectric_step_sprite_mark_redraw_spr1_not_moved:
		ex de, hl
		ld hl, (_isogbl_psprite2)
		add hl, bc
		ld a, (hl)
		or a
		jr z, l_isomectric_step_sprite_mark_redraw_check_unmoved_spr_under_unmoved_spr
		ex de, hl
	l_isomectric_step_sprite_mark_redraw_check_moved_spr_and_unmoved_spr:
		ld bc, T_SPRITE_OFFSET_REDRAW_NOT_MOVED - T_SPRITE_OFFSET_MOVED_OR_CHANGED
		add hl, bc
		ld a, (hl)
		or a
		jr nz, l_isomectric_step_sprite_mark_redraw_continue_intern_loop
		ld bc, T_SPRITE_OFFSET_NOT_MOVED_RECT_MAX_X - T_SPRITE_OFFSET_REDRAW_NOT_MOVED
		add hl, bc
		ld a, (hl)
		ex de, hl
		ld bc, T_SPRITE_OFFSET_ERASE_RECT_MIN_X - T_SPRITE_OFFSET_MOVED_OR_CHANGED
		add hl, bc
		cp (hl)
		jr c, l_isomectric_step_sprite_mark_redraw_continue_intern_loop
		ld bc, T_SPRITE_OFFSET_ERASE_RECT_MAX_X - T_SPRITE_OFFSET_ERASE_RECT_MIN_X
		add hl, bc
		ld a, (hl)
		ex de, hl
		ld bc, T_SPRITE_OFFSET_POS_X - T_SPRITE_OFFSET_NOT_MOVED_RECT_MAX_X
		add hl, bc
		cp (hl)
		jr c, l_isomectric_step_sprite_mark_redraw_continue_intern_loop
		ld bc, T_SPRITE_OFFSET_NOT_MOVED_RECT_MAX_Y - T_SPRITE_OFFSET_POS_X
		add hl, bc
		ld a, (hl)
		ex de, hl
		ld bc, T_SPRITE_OFFSET_LAST_Y - T_SPRITE_OFFSET_ERASE_RECT_MAX_X
		add hl, bc
		cp (hl)
		jr c, l_isomectric_step_sprite_mark_redraw_continue_intern_loop
		ld bc, T_SPRITE_OFFSET_ERASE_RECT_MAX_Y - T_SPRITE_OFFSET_LAST_Y
		add hl, bc
		ld a, (hl)
		ex de, hl
		ld bc, T_SPRITE_OFFSET_POS_Y - T_SPRITE_OFFSET_NOT_MOVED_RECT_MAX_Y
		add hl, bc
		cp (hl)
		jr c, l_isomectric_step_sprite_mark_redraw_continue_intern_loop
		ld bc, T_SPRITE_OFFSET_REDRAW_NOT_MOVED - T_SPRITE_OFFSET_POS_Y
		add hl, bc
		ld (hl), 1
		jr l_isomectric_step_sprite_mark_redraw_continue_intern_loop

	l_isomectric_step_sprite_mark_redraw_check_unmoved_spr_under_unmoved_spr:
		ex de, hl
		ld bc, T_SPRITE_OFFSET_REDRAW_NOT_MOVED - T_SPRITE_OFFSET_MOVED_OR_CHANGED
		add hl, bc
		ld a, (hl)
		or a
		jr z, l_isomectric_step_sprite_mark_redraw_continue_intern_loop
		ex de, hl
		add hl, bc
		ld a, (hl)
		or a
		jr nz, l_isomectric_step_sprite_mark_redraw_continue_intern_loop
		ld bc, T_SPRITE_OFFSET_NOT_MOVED_RECT_MAX_X - T_SPRITE_OFFSET_REDRAW_NOT_MOVED
		add hl, bc
		ld a,(hl)
		ex de, hl
		ld bc, T_SPRITE_OFFSET_POS_X - T_SPRITE_OFFSET_REDRAW_NOT_MOVED
		add hl, bc
		cp (hl)
		jp c, l_isomectric_step_sprite_mark_redraw_continue_intern_loop
		ld bc, T_SPRITE_OFFSET_NOT_MOVED_RECT_MAX_X - T_SPRITE_OFFSET_POS_X
		add hl, bc
		ld a, (hl)
		ex de, hl
		ld bc, T_SPRITE_OFFSET_POS_X - T_SPRITE_OFFSET_NOT_MOVED_RECT_MAX_X
		add hl, bc
		cp (hl)
		jp c, l_isomectric_step_sprite_mark_redraw_continue_intern_loop
		ld bc, T_SPRITE_OFFSET_NOT_MOVED_RECT_MAX_Y - T_SPRITE_OFFSET_POS_X
		add hl, bc
		ld a, (hl)
		ex de, hl
		ld bc, T_SPRITE_OFFSET_POS_Y - T_SPRITE_OFFSET_NOT_MOVED_RECT_MAX_X
		add hl, bc
		cp (hl)
		jp c, l_isomectric_step_sprite_mark_redraw_continue_intern_loop
		ld bc, T_SPRITE_OFFSET_NOT_MOVED_RECT_MAX_Y - T_SPRITE_OFFSET_POS_Y
		add hl, bc
		ld a, (hl)
		ex de, hl
		ld bc, T_SPRITE_OFFSET_POS_Y - T_SPRITE_OFFSET_NOT_MOVED_RECT_MAX_Y
		add hl, bc
		cp (hl)
		jp c, l_isomectric_step_sprite_mark_redraw_continue_intern_loop
		ld bc, T_SPRITE_OFFSET_REDRAW_NOT_MOVED - T_SPRITE_OFFSET_POS_Y
		add hl, bc
		ld (hl), 1
		jp l_isomectric_step_sprite_mark_redraw_continue_intern_loop
	l_isomectric_step_sprite_mark_redraw_end_loops:
	#endasm

	for(isogbl_i = 0; isogbl_i < n_ordered_isometric_objects; isogbl_i++)
	{
		if(ordered_isometric_objects_table[isogbl_i]->sprite.actual_frame)
		{
			sprite_draw(&(ordered_isometric_objects_table[isogbl_i]->sprite));
		}
	}

	for(isogbl_i = 0, isogbl_pp_isometric_obj = isometric_objects_table; isogbl_i < n_isometric_objects; isogbl_i++, isogbl_pp_isometric_obj++)
	{
		isogbl_p_isometric_obj = *isogbl_pp_isometric_obj;

		if(isogbl_p_isometric_obj->sprite.actual_frame)
		{
			sprite_update_display(&(isogbl_p_isometric_obj->sprite));
		}
	}
}