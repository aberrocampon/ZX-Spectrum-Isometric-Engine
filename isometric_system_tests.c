
#include "isometric_system_tests.h"

byte isometric_origen_proj_x = ISOMETRIC_ORIGEN_PROJ_X_DEFFAULT;
byte isometric_origen_proj_y = ISOMETRIC_ORIGEN_PROJ_Y_DEFFAULT;

t_isometric_obj isometric_objects_table[N_MAX_ISOMETRIC_OBJECTS];
byte n_isometric_objects = 0;

int height_over_proj_plane[N_MAX_ORDERED_ISOMETRIC_OBJECTS];
t_isometric_obj *ordered_isometric_objects_table[N_MAX_ORDERED_ISOMETRIC_OBJECTS];
byte n_ordered_isometric_objects = 0;

void isometric_add_object_to_table(t_isometric_obj_def *p_isometric_obj_def, t_b_vec3d *p_init_pos, void (*behavior)(t_isometric_obj *))
{
	t_isometric_obj *p_isometric_obj;

	if(n_isometric_objects >= N_MAX_ISOMETRIC_OBJECTS) return;

	p_isometric_obj = &isometric_objects_table[n_isometric_objects++];

	p_isometric_obj->physics.box3d.pos_x = p_init_pos->x;
	p_isometric_obj->physics.box3d.pos_y = p_init_pos->y;
	p_isometric_obj->physics.box3d.pos_z = p_init_pos->z;
	p_isometric_obj->physics.box3d.width_x = p_isometric_obj_def->size.x;
	p_isometric_obj->physics.box3d.width_y = p_isometric_obj_def->size.y;
	p_isometric_obj->physics.box3d.height = p_isometric_obj_def->size.z;
	p_isometric_obj->physics.speed_x = 0;
	p_isometric_obj->physics.speed_y = 0;
	p_isometric_obj->physics.speed_z = 0;
	p_isometric_obj->physics.touch_flags = 0;

	p_isometric_obj->behavior = behavior;

	p_isometric_obj->sprite.last_y = 255; // Aun no ha sido dibujado, no es necesario borrarlo antes de dibujarlo en el buffer virtual o tranferir la zona que ocupaba al frame buffer visible para borrarlo
	sprite_set_graphic_def(&p_isometric_obj->sprite , p_isometric_obj_def->p_sprite_def, 0, p_isometric_obj_def->p_sprite_def->total_frames_size);

	phys_box3d_add_object_to_table(&p_isometric_obj->physics);

}

void isometric_proj_obj(t_isometric_obj *p_isometric_obj)
{
	p_isometric_obj->sprite.pos_x = 
				isometric_origen_proj_x -
				p_isometric_obj->physics.box3d.pos_x + 
				p_isometric_obj->physics.box3d.pos_y +
				p_isometric_obj->sprite.delta_sprite_x;
	p_isometric_obj->sprite.pos_y = 
				isometric_origen_proj_y +
				((p_isometric_obj->physics.box3d.pos_x)>>1) + 
				((p_isometric_obj->physics.box3d.pos_y)>>1) -
				p_isometric_obj->physics.box3d.pos_z +
				p_isometric_obj->sprite.delta_sprite_y;
}

void isometric_add_object_to_order(t_isometric_obj *p_isometric_obj)
{
	int i;
	int ordered_index;
	int height;

	if(n_ordered_isometric_objects >= N_MAX_ORDERED_ISOMETRIC_OBJECTS) return;

	height = p_isometric_obj->physics.box3d.pos_x + p_isometric_obj->physics.box3d.pos_y + p_isometric_obj->physics.box3d.pos_z + p_isometric_obj->physics.box3d.pos_z;

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

void isometric_step(void)
{
	byte i;
	t_isometric_obj *p_isometric_obj;

	phys_box3d_step();

	for(i = 0, p_isometric_obj = isometric_objects_table; i < n_isometric_objects; i++, p_isometric_obj++)
	{
		if(p_isometric_obj->behavior)
		{
			(p_isometric_obj->behavior)(p_isometric_obj);
		}
	}

	for(i = 0, p_isometric_obj = isometric_objects_table; i < n_isometric_objects; i++, p_isometric_obj++)
	{
		sprite_restore_vdisplay(&(p_isometric_obj->sprite));
		isometric_proj_obj(p_isometric_obj);
	}

	isometric_reset_objects_ordering();
	for(i = 0, p_isometric_obj = isometric_objects_table; i < n_isometric_objects; i++, p_isometric_obj++)
	{
		isometric_add_object_to_order(p_isometric_obj);
	}

	for(i=0; i < n_ordered_isometric_objects; i++)
	{
		sprite_draw(&(ordered_isometric_objects_table[i]->sprite));
	}

	for(i = 0, p_isometric_obj = isometric_objects_table; i < n_isometric_objects; i++, p_isometric_obj++)
	{
		sprite_update_display(&(p_isometric_obj->sprite));
	}
}