
#include "isometric_system_tests.h"

byte isometric_origen_proj_x = ISOMETRIC_ORIGEN_PROJ_X_DEFFAULT;
byte isometric_origen_proj_y = ISOMETRIC_ORIGEN_PROJ_Y_DEFFAULT;

int height_over_proj_plane[N_MAX_ORDERED_ISOMETRIC_OBJECTS];
t_isometric_obj *ordered_isometric_objects_table[N_MAX_ORDERED_ISOMETRIC_OBJECTS];
byte n_ordered_isometric_objects = 0;

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