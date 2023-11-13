
#include "isometric_system_tests.h"

byte isometric_origen_proj_x = ISOMETRIC_ORIGEN_PROJ_X_DEFFAULT;
byte isometric_origen_proj_y = ISOMETRIC_ORIGEN_PROJ_Y_DEFFAULT;

t_isometric_obj *isometric_objects_table[N_MAX_ISOMETRIC_OBJECTS];
byte n_isometric_objects = 0;

t_isometric_obj created_isometric_objects_table[N_MAX_CREATED_ISOMETRIC_OBJECTS];
byte n_created_isometric_objects = 0;

int height_over_proj_plane[N_MAX_ORDERED_ISOMETRIC_OBJECTS];
t_isometric_obj *ordered_isometric_objects_table[N_MAX_ORDERED_ISOMETRIC_OBJECTS];
byte n_ordered_isometric_objects = 0;

void isometric_add_object_to_table(t_isometric_obj *p_isometric_obj)
{
	if(n_isometric_objects >= N_MAX_ISOMETRIC_OBJECTS) return;

    // Añade a la tabla de objetos fisicos
    isometric_objects_table[n_isometric_objects++] = p_isometric_obj;
}

t_isometric_obj *isometric_create_object(byte graphic_type_index, t_isometric_obj_graphic_def *p_isometric_obj_def, t_b_vec3d *p_init_pos, byte physics_enabled, byte init_flags, void (*behavior)(t_isometric_obj **), byte behavior_parameter)
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
	sprite_set_graphic_def(&p_isometric_obj->sprite , p_isometric_obj_def->p_sprite_def);

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
																&(p_isometric_object_def->init_pos), 
																p_isometric_object_def->physics_enabled,
																p_isometric_object_def->init_flags,
																p_isometric_object_def->behavior,
																p_isometric_object_def->behavior_parameter
																)
										);
	}
}

void isometric_proj_obj(t_isometric_obj *p_isometric_obj)
{
	p_isometric_obj->sprite.pos_x_ref = 
				isometric_origen_proj_x -
				p_isometric_obj->physics.box3d.pos_x + 
				p_isometric_obj->physics.box3d.pos_y;
	p_isometric_obj->sprite.pos_y_ref = 
				isometric_origen_proj_y +
				((p_isometric_obj->physics.box3d.pos_x)>>1) + 
				((p_isometric_obj->physics.box3d.pos_y)>>1) -
				p_isometric_obj->physics.box3d.pos_z;
}

void isometric_add_object_to_order(t_isometric_obj *p_isometric_obj)
{
	int i;
	int ordered_index;
	int height;

	if(n_ordered_isometric_objects >= N_MAX_ORDERED_ISOMETRIC_OBJECTS) return;

	height = p_isometric_obj->physics.box3d.pos_x + p_isometric_obj->physics.box3d.pos_y + ((p_isometric_obj->physics.box3d.pos_z)>>1) ;//+ p_isometric_obj->physics.box3d.pos_z;

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

t_isometric_obj **isogbl_pp_isometric_obj, *isogbl_p_isometric_obj;
int8 sprgbl_i, sprgbl_j;
t_sprite *sprgbl_psprite1, *sprgbl_psprite2;
byte sprgbl_x1, sprgbl_x2, sprgbl_x3, sprgbl_x4;
byte sprgbl_y1, sprgbl_y2, sprgbl_y3, sprgbl_y4;

void isometric_step(void)
{	
	phys_box3d_step();

	for(sprgbl_i = 0, isogbl_pp_isometric_obj = isometric_objects_table; sprgbl_i < n_isometric_objects; sprgbl_i++, isogbl_pp_isometric_obj++)
	{
		isogbl_p_isometric_obj = *isogbl_pp_isometric_obj;

		if(isogbl_p_isometric_obj->behavior)
		{
			(isogbl_p_isometric_obj->behavior)(isogbl_pp_isometric_obj);
		}
	}

	isometric_reset_objects_ordering();
	for(sprgbl_i = 0, isogbl_pp_isometric_obj = isometric_objects_table; sprgbl_i < n_isometric_objects; sprgbl_i++, isogbl_pp_isometric_obj++)
	{
		isogbl_p_isometric_obj = *isogbl_pp_isometric_obj;

		if(isogbl_p_isometric_obj->sprite.actual_frame)
		{
			if((isogbl_p_isometric_obj->physics.box3d.pos_x != isogbl_p_isometric_obj->physics.last_pos_x) || 
				(isogbl_p_isometric_obj->physics.box3d.pos_y != isogbl_p_isometric_obj->physics.last_pos_y) || (isogbl_p_isometric_obj->physics.box3d.pos_z != isogbl_p_isometric_obj->physics.last_pos_z))
			{
				isogbl_p_isometric_obj->sprite.moved_or_changed = 1;
				isogbl_p_isometric_obj->physics.last_pos_x = isogbl_p_isometric_obj->physics.box3d.pos_x;
				isogbl_p_isometric_obj->physics.last_pos_y = isogbl_p_isometric_obj->physics.box3d.pos_y;
				isogbl_p_isometric_obj->physics.last_pos_z = isogbl_p_isometric_obj->physics.box3d.pos_z;
			}
			////sprite_restore_vdisplay(&(isogbl_p_isometric_obj->sprite));
			sprite_erase_with_zeros(&(isogbl_p_isometric_obj->sprite));
			isometric_add_object_to_order(isogbl_p_isometric_obj);
			isometric_proj_obj(isogbl_p_isometric_obj);
		}
	}

	for(sprgbl_i = 0; sprgbl_i < n_ordered_isometric_objects; sprgbl_i++)
	{
		sprgbl_psprite1 = &(ordered_isometric_objects_table[sprgbl_i]->sprite);
		if((sprgbl_psprite1->actual_frame) && !(sprgbl_psprite1->moved_or_changed))
		{
			sprgbl_x1 = sprgbl_psprite1->pos_x;
			sprgbl_x2 = sprgbl_psprite1->pos_x + sprgbl_psprite1->width_px - 1;
			sprgbl_y1 = sprgbl_psprite1->pos_y;
			sprgbl_y2 = sprgbl_psprite1->pos_y + sprgbl_psprite1->height - 1;
			for(sprgbl_j = 0; sprgbl_j < n_ordered_isometric_objects; sprgbl_j++)
			{
				if(sprgbl_i == sprgbl_j) continue;				
				sprgbl_psprite2 = &(ordered_isometric_objects_table[sprgbl_j]->sprite);
				if(!(sprgbl_psprite2->actual_frame)) continue;
				if((sprgbl_i > sprgbl_j) && (sprgbl_psprite2->redraw_not_moved))
				{
					sprgbl_x3 = sprgbl_psprite2->pos_x;
					sprgbl_x4 = sprgbl_psprite2->pos_x + sprgbl_psprite2->width_px - 1;
					sprgbl_y3 = sprgbl_psprite2->pos_y;
					sprgbl_y4 = sprgbl_psprite2->pos_y + sprgbl_psprite2->height - 1;
					if(!((sprgbl_x4 < sprgbl_x1) || (sprgbl_x3 > sprgbl_x2) || (sprgbl_y4 < sprgbl_y1) || (sprgbl_y3 > sprgbl_y2)))
					{
						sprgbl_psprite1->redraw_not_moved = 1;
						break;
					}
				}
				if(sprgbl_psprite2->moved_or_changed)
				{
					sprgbl_x3 = (sprgbl_psprite2->last_x) & 0xf8;
					sprgbl_x4 = (sprgbl_psprite2->last_x + sprgbl_psprite2->width_px - 1) | 7;
					sprgbl_y3 = sprgbl_psprite2->last_y;
					sprgbl_y4 = sprgbl_psprite2->last_y + sprgbl_psprite2->height - 1;
					if(!((sprgbl_x4 < sprgbl_x1) || (sprgbl_x3 > sprgbl_x2) || (sprgbl_y4 < sprgbl_y1) || (sprgbl_y3 > sprgbl_y2)))
					{
						sprgbl_psprite1->redraw_not_moved = 1;
						break;
					}
				}
			}
		}
	}

	for(sprgbl_i = 0; sprgbl_i < n_ordered_isometric_objects; sprgbl_i++)
	{
		if(ordered_isometric_objects_table[sprgbl_i]->sprite.actual_frame)
		{
			sprite_draw(&(ordered_isometric_objects_table[sprgbl_i]->sprite));
		}
	}

	for(sprgbl_i = 0, isogbl_pp_isometric_obj = isometric_objects_table; sprgbl_i < n_isometric_objects; sprgbl_i++, isogbl_pp_isometric_obj++)
	{
		isogbl_p_isometric_obj = *isogbl_pp_isometric_obj;

		if(isogbl_p_isometric_obj->sprite.actual_frame)
		{
			sprite_update_display(&(isogbl_p_isometric_obj->sprite));
		}
	}
}