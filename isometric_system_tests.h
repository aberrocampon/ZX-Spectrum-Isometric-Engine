
#ifndef ISOMETRIC_SYSTEM_TESTS_H
#define ISOMETRIC_SYSTEM_TESTS_H

#include "value_types.h"
#include "physics_box3d_tests.h"
#include "sprites_mask_tests.h"

#define ISOMETRIC_ORIGEN_PROJ_X_DEFFAULT (127)
#define ISOMETRIC_ORIGEN_PROJ_Y_DEFFAULT (64)

#define ISOMETRIC_ORIENTATION_N (1)
#define ISOMETRIC_ORIENTATION_S (2)
#define ISOMETRIC_ORIENTATION_E (4)
#define ISOMETRIC_ORIENTATION_W (8)

#define N_MAX_ISOMETRIC_OBJECTS (16)
#define N_MAX_CREATED_ISOMETRIC_OBJECTS (16)
#define N_MAX_ORDERED_ISOMETRIC_OBJECTS (16)

typedef struct isometric_system_tests
{
	byte b_0;
	byte b_1;
	byte b_2;
	byte b_3;
	byte b_4;
	byte b_5;
	byte b_6;
	byte b_7;
} t_behavior_variables;


typedef struct isometric_obj
{
	// Coordenadas, caja de contencion, parametros fisicos y de comportamiento
	t_physics_box3d physics;
	// Funcion de control, comportamiento o intelligencia del objeto
	t_behavior_variables behavior_variables;
	byte bahavior_parameter;
	void (*behavior)(struct isometric_obj **);
	// sprite grafico
	t_sprite sprite;
	// identificador de tipo grafico del objeto isometrico. En la practica indice de una tabla de t_isometric_obj_graphic_def del juego
	byte graphic_type_index;
} t_isometric_obj;

#define T_ISOMETRIC_OBJ_OFFSET_SPRITE T_PHYSICS_BOX3D_SIZE + 11

typedef struct
{
	// dimensiones de la caja de contencion
	t_b_vec3d size;
	// sprite grafico
	t_sprite_graphic_def *p_sprite_def;
} t_isometric_obj_graphic_def;

typedef struct
{
    byte graphic_type_index; // en realidad indice de una tabla de t_isometric_obj_graphic_def del juego
	byte initial_required_graphic_state;
    t_b_vec3d init_pos;
    byte physics_enabled;
    byte init_flags;
    void (*behavior)(t_isometric_obj **);
    byte behavior_parameter;
} t_isometric_object_def;

extern byte isometric_origen_proj_x;
extern byte isometric_origen_proj_y;

extern t_isometric_obj *isometric_objects_table[];
extern byte n_isometric_objects;
extern byte n_bakckground_isometric_objects;
extern byte n_created_isometric_objects;
extern byte n_ordered_isometric_objects;

#define isometric_reset_table(n) \
							{ \
								n_created_isometric_objects = n_isometric_objects = 0; n_bakckground_isometric_objects = (n); \
								pp_phys_box3d_objects_table = (t_physics_box3d **)isometric_objects_table + (n); n_phys_box3d_objects = 0; \
							}
#define isometric_reset_objects_ordering(n) {n_ordered_isometric_objects = (n);}

void isometric_add_object_to_table(t_isometric_obj *p_isometric_obj);
t_isometric_obj *isometric_create_object(byte graphic_type_index, t_isometric_obj_graphic_def *p_isometric_obj_def, byte initial_required_graphic_state, t_b_vec3d *p_init_pos, byte physics_enabled, byte init_flags, void (*behavior)(t_isometric_obj **), byte behavior_parameter);
void isometric_create_and_add_objects_to_table(byte n_isometric_objects_def, t_isometric_object_def *isometric_objects_def_table, t_isometric_obj_graphic_def *isometric_obj_graphic_def_table);
void isometric_step(void);
void isometric_proj_obj(void);
void isometric_add_object_to_order(void);

#endif