#include "game_isometric_objects.h"
#include "keyboard.h"

byte nframes = 0;
byte ghost_last_orientation = ISOMETRIC_ORIENTATION_W;

void behavior_controller_player(t_isometric_obj *p_isometric_obj_player)
{
    nframes++;

	if(keyboard_is_key_pressed_1()) 
	{
		if(p_isometric_obj_player->physics.flags & PHYS_BOX3D_FLAG_TOUCH_D)
		{
			p_isometric_obj_player->physics.speed_y = p_isometric_obj_player->physics.p_phys_obj_touching_d->speed_y - 1;
		}

		if(ghost_last_orientation != ISOMETRIC_ORIENTATION_N)
		{
			ghost_last_orientation = ISOMETRIC_ORIENTATION_N;
			sprite_set_frames_subset(&(p_isometric_obj_player->sprite), GAME_SPRITE_FIRST_FRAME_HUMAN_FEET_MOVING_N_E, GAME_SPRITE_LAST_FRAME_HUMAN_FEET_MOVING_N_E);
			p_isometric_obj_player->sprite.required_graphic_state = (p_isometric_obj_player->sprite.required_graphic_state & (~SPRITE_GRAPHIC_STATE_MASK_FLIPPED_H)) | SPRITE_GRAPHIC_STATE_FLIPPED_LEFT;
		}
		else
		{
			if(!(nframes & 1)) sprite_next_frame(&(p_isometric_obj_player->sprite));
		}
	}

	if(keyboard_is_key_pressed_q())
	{
		if(p_isometric_obj_player->physics.flags & PHYS_BOX3D_FLAG_TOUCH_D)
		{
			p_isometric_obj_player->physics.speed_y = p_isometric_obj_player->physics.p_phys_obj_touching_d->speed_y + 1;
		}

		if(ghost_last_orientation != ISOMETRIC_ORIENTATION_S)
		{
			ghost_last_orientation = ISOMETRIC_ORIENTATION_S;
			sprite_set_frames_subset(&(p_isometric_obj_player->sprite), GAME_SPRITE_FIRST_FRAME_HUMAN_FEET_MOVING_S_W, GAME_SPRITE_LAST_FRAME_HUMAN_FEET_MOVING_S_W);
			p_isometric_obj_player->sprite.required_graphic_state = (p_isometric_obj_player->sprite.required_graphic_state & (~SPRITE_GRAPHIC_STATE_MASK_FLIPPED_H)) | SPRITE_GRAPHIC_STATE_FLIPPED_RIGHT;
		}
		else
		{
			if(!(nframes & 1)) sprite_next_frame(&(p_isometric_obj_player->sprite));
		}
	}

	if(keyboard_is_key_pressed_3())
	{
		if(p_isometric_obj_player->physics.flags & PHYS_BOX3D_FLAG_TOUCH_D)
		{
			p_isometric_obj_player->physics.speed_x = p_isometric_obj_player->physics.p_phys_obj_touching_d->speed_x - 1;
		}

		if(ghost_last_orientation != ISOMETRIC_ORIENTATION_E)
		{
			ghost_last_orientation = ISOMETRIC_ORIENTATION_E;
			sprite_set_frames_subset(&(p_isometric_obj_player->sprite), GAME_SPRITE_FIRST_FRAME_HUMAN_FEET_MOVING_N_E, GAME_SPRITE_LAST_FRAME_HUMAN_FEET_MOVING_N_E);
			p_isometric_obj_player->sprite.required_graphic_state = (p_isometric_obj_player->sprite.required_graphic_state & (~SPRITE_GRAPHIC_STATE_MASK_FLIPPED_H)) | SPRITE_GRAPHIC_STATE_FLIPPED_RIGHT;
		}
		else
		{
			if(!(nframes & 1)) sprite_next_frame(&(p_isometric_obj_player->sprite));
		}
	}

	if(keyboard_is_key_pressed_2()) 
	{
		if(p_isometric_obj_player->physics.flags & PHYS_BOX3D_FLAG_TOUCH_D)
		{
			p_isometric_obj_player->physics.speed_x = p_isometric_obj_player->physics.p_phys_obj_touching_d->speed_x + 1;
		}

		if(ghost_last_orientation != ISOMETRIC_ORIENTATION_W)
		{
			ghost_last_orientation = ISOMETRIC_ORIENTATION_W;
			sprite_set_frames_subset(&(p_isometric_obj_player->sprite), GAME_SPRITE_FIRST_FRAME_HUMAN_FEET_MOVING_S_W, GAME_SPRITE_LAST_FRAME_HUMAN_FEET_MOVING_S_W);
			p_isometric_obj_player->sprite.required_graphic_state = (p_isometric_obj_player->sprite.required_graphic_state & (~SPRITE_GRAPHIC_STATE_MASK_FLIPPED_H)) | SPRITE_GRAPHIC_STATE_FLIPPED_LEFT;
		}
		else
		{
			if(!(nframes & 1)) sprite_next_frame(&(p_isometric_obj_player->sprite));
		}
	}

	if(keyboard_is_key_pressed_4()) 
	{
		if(p_isometric_obj_player->physics.flags & PHYS_BOX3D_FLAG_TOUCH_D)
		{
			p_isometric_obj_player->physics.speed_z = 3;
		}

		if(!(nframes & 1)) sprite_next_frame(&(p_isometric_obj_player->sprite));
	}

	if(keyboard_is_key_pressed_5()) 
	{
		if(!(nframes & 1)) sprite_next_frame(&(p_isometric_obj_player->sprite));
	}
}

#define BEHAVIOR_CINEMATIC_UPDOWN_SPEEDZ ((int8)(p_isometric_obj->behavior_variables.b_1))
#define BEHAVIOR_CINEMATIC_UPDOWN_DELAY_COUNTER (p_isometric_obj->behavior_variables.b_2)
void behavior_cinematic_updown(t_isometric_obj *p_isometric_obj)
{
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