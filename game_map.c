#include "sprites_mask_tests.h"
#include "game_map.h"
#include "game_isometric_objects.h"
#include "game_isometric_objects_behavior.h"

// Numero de objetos isometricos de los que esta compuesto el jugador
// Nota: Si una dimension es muy grande es conveniente dividir el objeto isometrico en objetos mas pequeños y un objeto sin graficos puramente modelo fisico controla el comportamiento
// A su vez ese comportamiento controla las coordenadas de los objetos isometricos mas perqueños en los que queda descompuesto. Estos objetos con graficos no poseen modelo fisico ya que no los necesitan.
byte n_player_objects = 3;
t_b_vec3d game_map_player_initial_position_room_0 = { 70, 30, 23 }; // Posicion inicial del jugador en la room 0;
t_isometric_object_def player_isometric_objects[] =
{
    {
        GAME_ISOMETRIC_OBJ_TYPE_GUARDIAN_BOX3D,
        { 0, 0, 0 },
        1,
        0,
        behavior_player_init,
        0
    },
    {
        GAME_ISOMETRIC_OBJ_TYPE_GUARDIAN_TRUNK,
        { 0, 0, 0 },
        0,
        0,
        NULL,
        0
    },
    {
        GAME_ISOMETRIC_OBJ_TYPE_HUMAN_FEET,
        { 0, 0, 0 },
        0,
        0,
        NULL,
        0
    }
};

t_isometric_object_def game_room_0_isometric_objects[] =
{
    {
        GAME_ISOMETRIC_OBJ_TYPE_BLOCK,
        { 40, 50, 20 },
        1,
        PHYS_BOX3D_FLAG_CINEMATIC,
        behavior_cinematic_updown,
        0
    },
    {
        GAME_ISOMETRIC_OBJ_TYPE_BLOCK,
        { 70, 75, 40 },
        1,
        PHYS_BOX3D_FLAG_CINEMATIC,
        behavior_cinematic_updown,
        0
    },
    {
        GAME_ISOMETRIC_OBJ_TYPE_BLOCK,
        { 70, 30, 54 },
        1,
        0,
        NULL,
        0
    },
    {
        GAME_ISOMETRIC_OBJ_TYPE_BLOCK,
        { 50, 50, 35 },
        1,
        0,
        NULL,
        0
    },
    {
        GAME_ISOMETRIC_OBJ_TYPE_GATE_DINTEL_N_S,
        { (PHYS_BOX3D_MAX_X_3D_DEFFAULT>>1), 4, 35 },
        1,
        PHYS_BOX3D_FLAG_CINEMATIC,
        NULL,
        0
    },
    {
        GAME_ISOMETRIC_OBJ_TYPE_STONE_GATE_LEFT_N_S,
        { (PHYS_BOX3D_MAX_X_3D_DEFFAULT>>1) + 14 - 2, 4,  15},
        1,
        PHYS_BOX3D_FLAG_CINEMATIC,
        behavior_gate_init,
        SPRITE_GRAPHIC_STATE_FLIPPED_RIGHT
    },
    {
        GAME_ISOMETRIC_OBJ_TYPE_STONE_GATE_RIGHT_N_S,
        { (PHYS_BOX3D_MAX_X_3D_DEFFAULT>>1) - 14 + 2, 4,  15},
        1,
        PHYS_BOX3D_FLAG_CINEMATIC,
        behavior_gate_init,
        SPRITE_GRAPHIC_STATE_FLIPPED_RIGHT
    }
};

t_isometric_object_def game_room_1_isometric_objects[] =
{
    {
        GAME_ISOMETRIC_OBJ_TYPE_GHOST,
        { 50, 50, 35 },
        1,
        0,
        NULL,
        0
    },
    {
        GAME_ISOMETRIC_OBJ_TYPE_GATE_DINTEL_N_S,
        { (PHYS_BOX3D_MAX_X_3D_DEFFAULT>>1), PHYS_BOX3D_MAX_Y_3D_DEFFAULT - 4, 35 },
        1,
        PHYS_BOX3D_FLAG_CINEMATIC,
        NULL,
        0
    },
    {
        GAME_ISOMETRIC_OBJ_TYPE_STONE_GATE_LEFT_N_S,
        { (PHYS_BOX3D_MAX_X_3D_DEFFAULT>>1) + 14 - 2, PHYS_BOX3D_MAX_Y_3D_DEFFAULT - 4,  15},
        1,
        PHYS_BOX3D_FLAG_CINEMATIC,
        behavior_gate_init,
        SPRITE_GRAPHIC_STATE_FLIPPED_RIGHT
    },
    {
        GAME_ISOMETRIC_OBJ_TYPE_STONE_GATE_RIGHT_N_S,
        { (PHYS_BOX3D_MAX_X_3D_DEFFAULT>>1) - 14 + 2, PHYS_BOX3D_MAX_Y_3D_DEFFAULT - 4,  15},
        1,
        PHYS_BOX3D_FLAG_CINEMATIC,
        behavior_gate_init,
        SPRITE_GRAPHIC_STATE_FLIPPED_RIGHT
    },

    {
        GAME_ISOMETRIC_OBJ_TYPE_GATE_DINTEL_E_W,
        { 4, (PHYS_BOX3D_MAX_Y_3D_DEFFAULT>>1), 35 },
        1,
        PHYS_BOX3D_FLAG_CINEMATIC,
        NULL,
        0
    },
    {
        GAME_ISOMETRIC_OBJ_TYPE_STONE_GATE_LEFT_E_W,
        { 4, (PHYS_BOX3D_MAX_Y_3D_DEFFAULT>>1) + 14 - 2,  15},
        1,
        PHYS_BOX3D_FLAG_CINEMATIC,
        behavior_gate_init,
        SPRITE_GRAPHIC_STATE_FLIPPED_LEFT
    },
    {
        GAME_ISOMETRIC_OBJ_TYPE_STONE_GATE_RIGHT_E_W,
        { 4, (PHYS_BOX3D_MAX_Y_3D_DEFFAULT>>1) - 14 + 2,  15},
        1,
        PHYS_BOX3D_FLAG_CINEMATIC,
        behavior_gate_init,
        SPRITE_GRAPHIC_STATE_FLIPPED_LEFT
    }
};

byte n_game_map_rooms = 2;
t_game_map_room game_map_rooms_table[] =
{
    {
        GAME_MAP_ROOM_FLAG_EXIT_N, 1, 0, 0, 0,
        7,
        game_room_0_isometric_objects
    },
    {
        GAME_MAP_ROOM_FLAG_EXIT_S, 0, 0, 0, 0,
        7,
        game_room_1_isometric_objects
    }
};

byte game_map_next_room;
byte game_map_actual_room;

void game_map_init_background()
{
    t_sprite sprite;

    sprite_clear_vdisplay();
    sprite_set_attrib(7);
    sprite_set_border(0);

    sprite_set_graphic_def(&sprite, &game_spr_graph_def_stone_corner_nw);
    sprite.pos_x_ref = ISOMETRIC_ORIGEN_PROJ_X_DEFFAULT - PHYS_BOX3D_MAX_X_3D_DEFFAULT;
    sprite.pos_y_ref = ISOMETRIC_ORIGEN_PROJ_Y_DEFFAULT + (PHYS_BOX3D_MAX_X_3D_DEFFAULT >> 1);
    sprite_draw(&sprite);

    sprite.required_graphic_state = SPRITE_GRAPHIC_STATE_FLIPPED_LEFT;
    sprite.pos_x_ref = ISOMETRIC_ORIGEN_PROJ_X_DEFFAULT + PHYS_BOX3D_MAX_Y_3D_DEFFAULT;
    sprite.pos_y_ref = ISOMETRIC_ORIGEN_PROJ_Y_DEFFAULT + (PHYS_BOX3D_MAX_Y_3D_DEFFAULT >> 1);
    sprite_draw(&sprite);

    sprite_set_graphic_def(&sprite, &game_spr_graph_def_stone_corner_ne_1);
    sprite.pos_x_ref = ISOMETRIC_ORIGEN_PROJ_X_DEFFAULT;
    sprite.pos_y_ref = ISOMETRIC_ORIGEN_PROJ_Y_DEFFAULT;
    sprite_draw(&sprite);

    sprite_set_graphic_def(&sprite, &game_spr_graph_def_stone_corner_ne_2);
    sprite.pos_x_ref = ISOMETRIC_ORIGEN_PROJ_X_DEFFAULT + 1;
    sprite.pos_y_ref = ISOMETRIC_ORIGEN_PROJ_Y_DEFFAULT;
    sprite_draw(&sprite);
}

void game_map_enter_room(void)
{
    isometric_reset_table();
    isometric_create_and_add_objects_to_table(game_map_rooms_table[game_map_next_room].n_isometric_objects, game_map_rooms_table[game_map_next_room].p_game_map_room_isometric_objects, game_isometric_objects_table);
    isometric_create_and_add_objects_to_table(n_player_objects, player_isometric_objects, game_isometric_objects_table);
    game_map_actual_room = game_map_next_room;

    game_map_init_background();
    sprite_transfer_vdisplay_2_background_vdisplay();
    sprite_transfer_vdisplay_2_phys_display();
}

void game_map_step(void)
{
    if(game_map_actual_room != game_map_next_room)
    {
        game_map_enter_room();
    }
}

void game_map_init(void)
{
    game_map_move_player_2_room(0, game_map_player_initial_position_room_0.x, game_map_player_initial_position_room_0.y, game_map_player_initial_position_room_0.z);
    game_map_enter_room();
}