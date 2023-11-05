#include "game_isometric_objects.h"
#include "game_map.h"
#include "keyboard.h"

t_isometric_obj *p_isometric_obj;
t_isometric_obj *p_isometric_subobj_1, *p_isometric_subobj_2;

byte nframes = 0;
byte player_last_orientation = ISOMETRIC_ORIENTATION_W;

void orient_guardian(byte new_orientation)
{
	// auxiliar function to set graphic sprites frames subsets according to the orientation of the guardian body
	// p_isometric_subobj_1 and p_isometric_subobj_2 must be previuosly set to point to trunk and feet isometric objects repectively
	if((new_orientation == ISOMETRIC_ORIENTATION_N) || (new_orientation == ISOMETRIC_ORIENTATION_E))
	{
		sprite_set_frames_subset(&(p_isometric_subobj_1->sprite), GAME_SPRITE_FIRST_FRAME_GUARDIAN_TRUNK_MOVING_N_E, GAME_SPRITE_LAST_FRAME_GUARDIAN_TRUNK_MOVING_N_E);
		sprite_set_frames_subset(&(p_isometric_subobj_2->sprite), GAME_SPRITE_FIRST_FRAME_HUMAN_FEET_MOVING_N_E, GAME_SPRITE_LAST_FRAME_HUMAN_FEET_MOVING_N_E);
	}
	else
	{
		sprite_set_frames_subset(&(p_isometric_subobj_1->sprite), GAME_SPRITE_FIRST_FRAME_GUARDIAN_TRUNK_MOVING_S_W, GAME_SPRITE_LAST_FRAME_GUARDIAN_TRUNK_MOVING_S_W);
		sprite_set_frames_subset(&(p_isometric_subobj_2->sprite), GAME_SPRITE_FIRST_FRAME_HUMAN_FEET_MOVING_S_W, GAME_SPRITE_LAST_FRAME_HUMAN_FEET_MOVING_S_W);
	}

	if((new_orientation == ISOMETRIC_ORIENTATION_N) || (new_orientation == ISOMETRIC_ORIENTATION_W))
	{
		p_isometric_subobj_1->sprite.required_graphic_state = (p_isometric_subobj_1->sprite.required_graphic_state & (~SPRITE_GRAPHIC_STATE_MASK_FLIPPED_H)) | SPRITE_GRAPHIC_STATE_FLIPPED_LEFT;
		p_isometric_subobj_2->sprite.required_graphic_state = (p_isometric_subobj_2->sprite.required_graphic_state & (~SPRITE_GRAPHIC_STATE_MASK_FLIPPED_H)) | SPRITE_GRAPHIC_STATE_FLIPPED_LEFT;
	}
	else
	{
		p_isometric_subobj_1->sprite.required_graphic_state = (p_isometric_subobj_1->sprite.required_graphic_state & (~SPRITE_GRAPHIC_STATE_MASK_FLIPPED_H)) | SPRITE_GRAPHIC_STATE_FLIPPED_RIGHT;
		p_isometric_subobj_2->sprite.required_graphic_state = (p_isometric_subobj_2->sprite.required_graphic_state & (~SPRITE_GRAPHIC_STATE_MASK_FLIPPED_H)) | SPRITE_GRAPHIC_STATE_FLIPPED_RIGHT;
	}
}

void behavior_player_set_pointers(t_isometric_obj **pp_isometric_obj_player)
{
	p_isometric_obj = *pp_isometric_obj_player;
	p_isometric_subobj_1 = pp_isometric_obj_player[1];
	p_isometric_subobj_2 = pp_isometric_obj_player[2];
}

void behavior_controller_player(t_isometric_obj **pp_isometric_obj_player)
{
	behavior_player_set_pointers(pp_isometric_obj_player);

	// asignar posicion de pies y tronco
	p_isometric_subobj_1->physics.box3d.pos_x = pp_isometric_obj_player[0]->physics.box3d.pos_x;
	p_isometric_subobj_1->physics.box3d.pos_y = pp_isometric_obj_player[0]->physics.box3d.pos_y;
	p_isometric_subobj_1->physics.box3d.pos_z = pp_isometric_obj_player[0]->physics.box3d.pos_z + (13 - 10);

	p_isometric_subobj_2->physics.box3d.pos_x = pp_isometric_obj_player[0]->physics.box3d.pos_x;
	p_isometric_subobj_2->physics.box3d.pos_y = pp_isometric_obj_player[0]->physics.box3d.pos_y;
	p_isometric_subobj_2->physics.box3d.pos_z = pp_isometric_obj_player[0]->physics.box3d.pos_z - (13 - 3);

    nframes++;

	// Comprobacion de salidas
	if((game_map_rooms_table[game_map_actual_room].exits & GAME_MAP_ROOM_FLAG_EXIT_N) && 
				(p_isometric_obj->physics.flags & PHYS_BOX3D_FLAG_TOUCH_N) && (p_isometric_obj->physics.p_phys_obj_touching_n == &phys_box3d_wall_n) &&
				(p_isometric_obj->physics.box3d.pos_x > (half_isometric_max_x_3d - 5)) && (p_isometric_obj->physics.box3d.pos_x < (half_isometric_max_x_3d + 5)))
	{
		game_map_move_player_2_room(game_map_rooms_table[game_map_actual_room].room_to_n, 
							p_isometric_obj->physics.box3d.pos_x, isometric_max_y_3d - p_isometric_obj->physics.box3d.width_y, p_isometric_obj->physics.box3d.pos_z);
	}
	if((game_map_rooms_table[game_map_actual_room].exits & GAME_MAP_ROOM_FLAG_EXIT_S) && 
				(p_isometric_obj->physics.flags & PHYS_BOX3D_FLAG_TOUCH_S) && (p_isometric_obj->physics.p_phys_obj_touching_s == &phys_box3d_wall_s) &&
				(p_isometric_obj->physics.box3d.pos_x > (half_isometric_max_x_3d - 5)) && (p_isometric_obj->physics.box3d.pos_x < (half_isometric_max_x_3d + 5)))
	{
		game_map_move_player_2_room(game_map_rooms_table[game_map_actual_room].room_to_s, 
							p_isometric_obj->physics.box3d.pos_x, p_isometric_obj->physics.box3d.width_y, p_isometric_obj->physics.box3d.pos_z);
	}

	if(keyboard_is_key_pressed_1()) 
	{
		if(p_isometric_obj->physics.flags & PHYS_BOX3D_FLAG_TOUCH_D)
		{
			p_isometric_obj->physics.speed_y = p_isometric_obj->physics.p_phys_obj_touching_d->speed_y - 1;
		}

		if(player_last_orientation != ISOMETRIC_ORIENTATION_N)
		{
			player_last_orientation = ISOMETRIC_ORIENTATION_N;
			orient_guardian(player_last_orientation);
		}
		else
		{
			if(!(nframes & 1)) sprite_next_frame(&(p_isometric_subobj_2->sprite));
		}
	}

	if(keyboard_is_key_pressed_q())
	{
		if(p_isometric_obj->physics.flags & PHYS_BOX3D_FLAG_TOUCH_D)
		{
			p_isometric_obj->physics.speed_y = p_isometric_obj->physics.p_phys_obj_touching_d->speed_y + 1;
		}

		if(player_last_orientation != ISOMETRIC_ORIENTATION_S)
		{
			player_last_orientation = ISOMETRIC_ORIENTATION_S;
			orient_guardian(player_last_orientation);
		}
		else
		{
			if(!(nframes & 1)) sprite_next_frame(&(p_isometric_subobj_2->sprite));
		}
	}

	if(keyboard_is_key_pressed_3())
	{
		if(p_isometric_obj->physics.flags & PHYS_BOX3D_FLAG_TOUCH_D)
		{
			p_isometric_obj->physics.speed_x = p_isometric_obj->physics.p_phys_obj_touching_d->speed_x - 1;
		}

		if(player_last_orientation != ISOMETRIC_ORIENTATION_E)
		{
			player_last_orientation = ISOMETRIC_ORIENTATION_E;
			orient_guardian(player_last_orientation);
		}
		else
		{
			if(!(nframes & 1)) sprite_next_frame(&(p_isometric_subobj_2->sprite));
		}
	}

	if(keyboard_is_key_pressed_2()) 
	{
		if(p_isometric_obj->physics.flags & PHYS_BOX3D_FLAG_TOUCH_D)
		{
			p_isometric_obj->physics.speed_x = p_isometric_obj->physics.p_phys_obj_touching_d->speed_x + 1;
		}

		if(player_last_orientation != ISOMETRIC_ORIENTATION_W)
		{
			player_last_orientation = ISOMETRIC_ORIENTATION_W;
			orient_guardian(player_last_orientation);
		}
		else
		{
			if(!(nframes & 1)) sprite_next_frame(&(p_isometric_subobj_2->sprite));
		}
	}

	if(keyboard_is_key_pressed_4()) 
	{
		if(p_isometric_obj->physics.flags & PHYS_BOX3D_FLAG_TOUCH_D)
		{
			p_isometric_obj->physics.speed_z = 3;
		}

		if(!(nframes & 1)) sprite_next_frame(&(p_isometric_subobj_2->sprite));
	}

	if(keyboard_is_key_pressed_5()) 
	{
		if(!(nframes & 1)) sprite_next_frame(&(p_isometric_subobj_2->sprite));
	}
}

void behavior_player_init(t_isometric_obj **pp_isometric_obj_player)
{
	behavior_player_set_pointers(pp_isometric_obj_player);

	// Initialization after entering a room. Set orientation according to previous room
	orient_guardian(player_last_orientation);
	p_isometric_obj->behavior = behavior_controller_player;
	behavior_controller_player(pp_isometric_obj_player);
}

#define BEHAVIOR_CINEMATIC_UPDOWN_SPEEDZ ((int8)(p_isometric_obj->behavior_variables.b_1))
#define BEHAVIOR_CINEMATIC_UPDOWN_DELAY_COUNTER (p_isometric_obj->behavior_variables.b_2)
void behavior_cinematic_updown(t_isometric_obj **pp_isometric_obj)
{
	p_isometric_obj = *pp_isometric_obj;

    // Variables initialization
    if(p_isometric_obj->behavior_variables.b_0 == 0)
    {
        p_isometric_obj->behavior_variables.b_0 = 1;
        BEHAVIOR_CINEMATIC_UPDOWN_SPEEDZ = -1;
    }

	if(BEHAVIOR_CINEMATIC_UPDOWN_SPEEDZ < 0)
	{
		/*if(p_isometric_obj->physics.flags & PHYS_BOX3D_FLAG_TOUCH_D)
		{
			p_isometric_obj->physics.speed_z = 0;
		}
		else */if(p_isometric_obj->physics.box3d.pos_z <= 10)
		{
			p_isometric_obj->physics.speed_z = BEHAVIOR_CINEMATIC_UPDOWN_SPEEDZ = 0;
			BEHAVIOR_CINEMATIC_UPDOWN_DELAY_COUNTER = 32;
		}
		else
		{
			p_isometric_obj->physics.speed_z = BEHAVIOR_CINEMATIC_UPDOWN_SPEEDZ;
		}
	}
	else if(BEHAVIOR_CINEMATIC_UPDOWN_SPEEDZ > 0)
	{
		/*if((p_isometric_obj->physics.flags & PHYS_BOX3D_FLAG_TOUCH_U) && (p_isometric_obj->physics.p_phys_obj_touching_u->flags & PHYS_BOX3D_FLAG_TOUCH_U ))
		{
			p_isometric_obj->physics.speed_z = 0;
		}
		else */if(p_isometric_obj->physics.box3d.pos_z >= 20)
		{
			p_isometric_obj->physics.speed_z = BEHAVIOR_CINEMATIC_UPDOWN_SPEEDZ = 0;
			BEHAVIOR_CINEMATIC_UPDOWN_DELAY_COUNTER = 32;
		}
		else
		{
			p_isometric_obj->physics.speed_z = BEHAVIOR_CINEMATIC_UPDOWN_SPEEDZ;
		}
	}
	else
	{
		if(!(BEHAVIOR_CINEMATIC_UPDOWN_DELAY_COUNTER--))
		{
			if(p_isometric_obj->physics.box3d.pos_z >= 20)
				p_isometric_obj->physics.speed_z = BEHAVIOR_CINEMATIC_UPDOWN_SPEEDZ = -1;
			else
				p_isometric_obj->physics.speed_z = BEHAVIOR_CINEMATIC_UPDOWN_SPEEDZ = 1;
		}
		else
			p_isometric_obj->physics.speed_z = 0;
	}

}

void behavior_gate_init(t_isometric_obj **pp_isometric_obj)
{
	p_isometric_obj = *pp_isometric_obj;

	p_isometric_obj->sprite.required_graphic_state = p_isometric_obj->bahavior_parameter;
	p_isometric_obj->behavior = NULL;
}