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
        SPRITE_GRAPHIC_STATE_FLIPPED_RIGHT,
        { 0, 0, 0 },
        1,
        0,
        behavior_player_init,
        0
    },
    {
        GAME_ISOMETRIC_OBJ_TYPE_GUARDIAN_TRUNK,
        SPRITE_GRAPHIC_STATE_FLIPPED_RIGHT,
        { 0, 0, 0 },
        0,
        0,
        NULL,
        0
    },
    {
        GAME_ISOMETRIC_OBJ_TYPE_HUMAN_FEET,
        SPRITE_GRAPHIC_STATE_FLIPPED_RIGHT,
        { 0, 0, 0 },
        0,
        0,
        NULL,
        0
    }
};

// Sobreimpresion para las paredes de la habitacion, representan el fondo grafico tras los demas objetos que se mueven delante
byte n_stone_wall_objects = 4;
t_isometric_object_def stone_wall_isometric_objects[] =
{
    {
        GAME_ISOMETRIC_OBJ_TYPE_STONE_WALL_BACKG_NW,
        SPRITE_GRAPHIC_STATE_FLIPPED_RIGHT,
        { PHYS_BOX3D_MAX_X_3D_DEFFAULT, 0, 0 },
        0,
        0,
        NULL,
        0
    },
    {
        GAME_ISOMETRIC_OBJ_TYPE_STONE_WALL_BACKG_NW,
        SPRITE_GRAPHIC_STATE_FLIPPED_LEFT,
        { 0, PHYS_BOX3D_MAX_Y_3D_DEFFAULT, 0 },
        0,
        0,
        NULL,
        0
    },
    {
        GAME_ISOMETRIC_OBJ_TYPE_STONE_WALL_BACKG_NE_1,
        SPRITE_GRAPHIC_STATE_FLIPPED_RIGHT,
        { 0, 0, 0 },
        0,
        0,
        NULL,
        0
    },
    {
        GAME_ISOMETRIC_OBJ_TYPE_STONE_WALL_BACKG_NE_2,
        SPRITE_GRAPHIC_STATE_FLIPPED_RIGHT,
        { 0, 1, 0 },
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
        SPRITE_GRAPHIC_STATE_FLIPPED_RIGHT,
        { 40, 50, 20 },
        1,
        PHYS_BOX3D_FLAG_CINEMATIC,
        behavior_cinematic_updown,
        0
    },
    {
        GAME_ISOMETRIC_OBJ_TYPE_BLOCK,
        SPRITE_GRAPHIC_STATE_FLIPPED_RIGHT,
        { 70, 75, 40 },
        1,
        PHYS_BOX3D_FLAG_CINEMATIC,
        behavior_cinematic_updown,
        0
    },
    {
        GAME_ISOMETRIC_OBJ_TYPE_BLOCK,
        SPRITE_GRAPHIC_STATE_FLIPPED_RIGHT,
        { 70, 30, 54 },
        1,
        0,
        NULL,
        0
    },
    {
        GAME_ISOMETRIC_OBJ_TYPE_BLOCK,
        SPRITE_GRAPHIC_STATE_FLIPPED_RIGHT,
        { 50, 50, 35 },
        1,
        0,
        NULL,
        0
    },
    {
        GAME_ISOMETRIC_OBJ_TYPE_GATE_DINTEL_N_S,
        0,
        { (PHYS_BOX3D_MAX_X_3D_DEFFAULT>>1), 4, 35 },
        1,
        PHYS_BOX3D_FLAG_CINEMATIC,
        NULL,
        0
    },
    {
        GAME_ISOMETRIC_OBJ_TYPE_STONE_GATE_LEFT_N_S,
        SPRITE_GRAPHIC_STATE_FLIPPED_RIGHT,
        { (PHYS_BOX3D_MAX_X_3D_DEFFAULT>>1) + 14 - 2, 4,  15},
        1,
        PHYS_BOX3D_FLAG_CINEMATIC,
        NULL,
        0
    },
    {
        GAME_ISOMETRIC_OBJ_TYPE_STONE_GATE_RIGHT_N_S,
        SPRITE_GRAPHIC_STATE_FLIPPED_RIGHT,
        { (PHYS_BOX3D_MAX_X_3D_DEFFAULT>>1) - 14 + 2, 4,  15},
        1,
        PHYS_BOX3D_FLAG_CINEMATIC,
        NULL,
        0
    }
};

t_isometric_object_def game_room_1_isometric_objects[] =
{
    {
        GAME_ISOMETRIC_OBJ_TYPE_GHOST,
        SPRITE_GRAPHIC_STATE_FLIPPED_RIGHT,
        { 50, 50, 35 },
        1,
        0,
        NULL,
        0
    },
    {
        GAME_ISOMETRIC_OBJ_TYPE_GATE_DINTEL_N_S,
        0,
        { (PHYS_BOX3D_MAX_X_3D_DEFFAULT>>1), PHYS_BOX3D_MAX_Y_3D_DEFFAULT - 4, 35 },
        1,
        PHYS_BOX3D_FLAG_CINEMATIC,
        NULL,
        0
    },
    {
        GAME_ISOMETRIC_OBJ_TYPE_STONE_GATE_LEFT_N_S,
        SPRITE_GRAPHIC_STATE_FLIPPED_RIGHT,
        { (PHYS_BOX3D_MAX_X_3D_DEFFAULT>>1) + 14 - 2, PHYS_BOX3D_MAX_Y_3D_DEFFAULT - 4,  15},
        1,
        PHYS_BOX3D_FLAG_CINEMATIC,
        NULL,
        0
    },
    {
        GAME_ISOMETRIC_OBJ_TYPE_STONE_GATE_RIGHT_N_S,
        SPRITE_GRAPHIC_STATE_FLIPPED_RIGHT,
        { (PHYS_BOX3D_MAX_X_3D_DEFFAULT>>1) - 14 + 2, PHYS_BOX3D_MAX_Y_3D_DEFFAULT - 4,  15},
        1,
        PHYS_BOX3D_FLAG_CINEMATIC,
        NULL,
        0
    },

    {
        GAME_ISOMETRIC_OBJ_TYPE_GATE_DINTEL_E_W,
        0,
        { 4, (PHYS_BOX3D_MAX_Y_3D_DEFFAULT>>1), 35 },
        1,
        PHYS_BOX3D_FLAG_CINEMATIC,
        NULL,
        0
    },
    {
        GAME_ISOMETRIC_OBJ_TYPE_STONE_GATE_LEFT_E_W,
        SPRITE_GRAPHIC_STATE_FLIPPED_LEFT,
        { 4, (PHYS_BOX3D_MAX_Y_3D_DEFFAULT>>1) + 14 - 2,  15},
        1,
        PHYS_BOX3D_FLAG_CINEMATIC,
        NULL,
        0
    },
    {
        GAME_ISOMETRIC_OBJ_TYPE_STONE_GATE_RIGHT_E_W,
        SPRITE_GRAPHIC_STATE_FLIPPED_LEFT,
        { 4, (PHYS_BOX3D_MAX_Y_3D_DEFFAULT>>1) - 14 + 2,  15},
        1,
        PHYS_BOX3D_FLAG_CINEMATIC,
        NULL,
        0
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

void game_map_enter_room(void)
{
    sprite_clear_display();
    sprite_clear_vdisplay();
    sprite_set_attrib(7);
    sprite_set_border(0);

    isometric_reset_table(n_stone_wall_objects);
    isometric_reset_objects_ordering(0);

    //background (walls)
    isometric_create_and_add_objects_to_table(n_stone_wall_objects, stone_wall_isometric_objects, game_isometric_objects_table);
    sprite_set_required_graphic_state(&(isometric_objects_table[1]->sprite), SPRITE_GRAPHIC_STATE_FLIPPED_LEFT);
    sprite_set_pos_from_posref(&(isometric_objects_table[1]->sprite));

    // room objects
    isometric_create_and_add_objects_to_table(game_map_rooms_table[game_map_next_room].n_isometric_objects, game_map_rooms_table[game_map_next_room].p_game_map_room_isometric_objects, game_isometric_objects_table);

    // player
    isometric_create_and_add_objects_to_table(n_player_objects, player_isometric_objects, game_isometric_objects_table);

    game_map_actual_room = game_map_next_room;
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