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
        behavior_controller_player,
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
    }
};

byte n_game_map_rooms = 2;
t_game_map_room game_map_rooms_table[] =
{
    {
        GAME_MAP_ROOM_FLAG_EXIT_N, 1, 0, 0, 0,
        4,
        game_room_0_isometric_objects
    },
    {
        GAME_MAP_ROOM_FLAG_EXIT_S, 0, 0, 0, 0,
        1,
        game_room_1_isometric_objects
    }
};

byte game_map_next_room;
byte game_map_actual_room;

void game_map_enter_room(void)
{
    isometric_reset_table();
    isometric_create_and_add_objects_to_table(game_map_rooms_table[game_map_next_room].n_isometric_objects, game_map_rooms_table[game_map_next_room].p_game_map_room_isometric_objects, game_isometric_objects_table);
    isometric_create_and_add_objects_to_table(n_player_objects, player_isometric_objects, game_isometric_objects_table);
    game_map_actual_room = game_map_next_room;

    sprite_init_background();
    sprite_transfer_vdisplay();
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