#include "game_isometric_objects.h"
#include "keyboard.h"

t_isometric_obj *p_isometric_obj;
t_isometric_obj *p_isometric_subobj_1, *p_isometric_subobj_2;

byte nframes = 0;
byte ghost_last_orientation = ISOMETRIC_ORIENTATION_W;

void behavior_controller_player(t_isometric_obj **pp_isometric_obj_player)
{
	// asignar posicion de pies y tronco
	pp_isometric_obj_player[1]->physics.box3d.pos_x = pp_isometric_obj_player[0]->physics.box3d.pos_x;
	pp_isometric_obj_player[1]->physics.box3d.pos_y = pp_isometric_obj_player[0]->physics.box3d.pos_y;
	pp_isometric_obj_player[1]->physics.box3d.pos_z = pp_isometric_obj_player[0]->physics.box3d.pos_z + (13 - 10);

	pp_isometric_obj_player[2]->physics.box3d.pos_x = pp_isometric_obj_player[0]->physics.box3d.pos_x;
	pp_isometric_obj_player[2]->physics.box3d.pos_y = pp_isometric_obj_player[0]->physics.box3d.pos_y;
	pp_isometric_obj_player[2]->physics.box3d.pos_z = pp_isometric_obj_player[0]->physics.box3d.pos_z - (13 - 3);

	p_isometric_obj = *pp_isometric_obj_player;
	p_isometric_subobj_1 = pp_isometric_obj_player[1];
	p_isometric_subobj_2 = pp_isometric_obj_player[2];

    nframes++;

	if(keyboard_is_key_pressed_1()) 
	{
		if(p_isometric_obj->physics.flags & PHYS_BOX3D_FLAG_TOUCH_D)
		{
			p_isometric_obj->physics.speed_y = p_isometric_obj->physics.p_phys_obj_touching_d->speed_y - 1;
		}

		if(ghost_last_orientation != ISOMETRIC_ORIENTATION_N)
		{
			ghost_last_orientation = ISOMETRIC_ORIENTATION_N;
			sprite_set_frames_subset(&(p_isometric_subobj_1->sprite), GAME_SPRITE_FIRST_FRAME_GUARDIAN_TRUNK_MOVING_N_E, GAME_SPRITE_LAST_FRAME_GUARDIAN_TRUNK_MOVING_N_E);
			p_isometric_subobj_1->sprite.required_graphic_state = (p_isometric_subobj_1->sprite.required_graphic_state & (~SPRITE_GRAPHIC_STATE_MASK_FLIPPED_H)) | SPRITE_GRAPHIC_STATE_FLIPPED_LEFT;
			sprite_set_frames_subset(&(p_isometric_subobj_2->sprite), GAME_SPRITE_FIRST_FRAME_HUMAN_FEET_MOVING_N_E, GAME_SPRITE_LAST_FRAME_HUMAN_FEET_MOVING_N_E);
			p_isometric_subobj_2->sprite.required_graphic_state = (p_isometric_subobj_2->sprite.required_graphic_state & (~SPRITE_GRAPHIC_STATE_MASK_FLIPPED_H)) | SPRITE_GRAPHIC_STATE_FLIPPED_LEFT;
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

		if(ghost_last_orientation != ISOMETRIC_ORIENTATION_S)
		{
			ghost_last_orientation = ISOMETRIC_ORIENTATION_S;
			sprite_set_frames_subset(&(p_isometric_subobj_1->sprite), GAME_SPRITE_FIRST_FRAME_GUARDIAN_TRUNK_MOVING_S_W, GAME_SPRITE_LAST_FRAME_GUARDIAN_TRUNK_MOVING_S_W);
			p_isometric_subobj_1->sprite.required_graphic_state = (p_isometric_subobj_1->sprite.required_graphic_state & (~SPRITE_GRAPHIC_STATE_MASK_FLIPPED_H)) | SPRITE_GRAPHIC_STATE_FLIPPED_RIGHT;
			sprite_set_frames_subset(&(p_isometric_subobj_2->sprite), GAME_SPRITE_FIRST_FRAME_HUMAN_FEET_MOVING_S_W, GAME_SPRITE_LAST_FRAME_HUMAN_FEET_MOVING_S_W);
			p_isometric_subobj_2->sprite.required_graphic_state = (p_isometric_subobj_2->sprite.required_graphic_state & (~SPRITE_GRAPHIC_STATE_MASK_FLIPPED_H)) | SPRITE_GRAPHIC_STATE_FLIPPED_RIGHT;
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

		if(ghost_last_orientation != ISOMETRIC_ORIENTATION_E)
		{
			ghost_last_orientation = ISOMETRIC_ORIENTATION_E;
			sprite_set_frames_subset(&(p_isometric_subobj_1->sprite), GAME_SPRITE_FIRST_FRAME_GUARDIAN_TRUNK_MOVING_N_E, GAME_SPRITE_LAST_FRAME_GUARDIAN_TRUNK_MOVING_N_E);
			p_isometric_subobj_1->sprite.required_graphic_state = (p_isometric_subobj_1->sprite.required_graphic_state & (~SPRITE_GRAPHIC_STATE_MASK_FLIPPED_H)) | SPRITE_GRAPHIC_STATE_FLIPPED_RIGHT;
			sprite_set_frames_subset(&(p_isometric_subobj_2->sprite), GAME_SPRITE_FIRST_FRAME_HUMAN_FEET_MOVING_N_E, GAME_SPRITE_LAST_FRAME_HUMAN_FEET_MOVING_N_E);
			p_isometric_subobj_2->sprite.required_graphic_state = (p_isometric_subobj_2->sprite.required_graphic_state & (~SPRITE_GRAPHIC_STATE_MASK_FLIPPED_H)) | SPRITE_GRAPHIC_STATE_FLIPPED_RIGHT;
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

		if(ghost_last_orientation != ISOMETRIC_ORIENTATION_W)
		{
			ghost_last_orientation = ISOMETRIC_ORIENTATION_W;
			sprite_set_frames_subset(&(p_isometric_subobj_1->sprite), GAME_SPRITE_FIRST_FRAME_GUARDIAN_TRUNK_MOVING_S_W, GAME_SPRITE_LAST_FRAME_GUARDIAN_TRUNK_MOVING_S_W);
			p_isometric_subobj_1->sprite.required_graphic_state = (p_isometric_subobj_1->sprite.required_graphic_state & (~SPRITE_GRAPHIC_STATE_MASK_FLIPPED_H)) | SPRITE_GRAPHIC_STATE_FLIPPED_LEFT;
			sprite_set_frames_subset(&(p_isometric_subobj_2->sprite), GAME_SPRITE_FIRST_FRAME_HUMAN_FEET_MOVING_S_W, GAME_SPRITE_LAST_FRAME_HUMAN_FEET_MOVING_S_W);
			p_isometric_subobj_2->sprite.required_graphic_state = (p_isometric_subobj_2->sprite.required_graphic_state & (~SPRITE_GRAPHIC_STATE_MASK_FLIPPED_H)) | SPRITE_GRAPHIC_STATE_FLIPPED_LEFT;
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