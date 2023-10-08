
#ifndef ISOMETRIC_SYSTEM_TESTS_H
#define ISOMETRIC_SYSTEM_TESTS_H

#include "value_types.h"
#include "physics_box3d_tests.h"
#include "sprites_mask_tests.h"

#define ISOMETRIC_ORIGEN_PROJ_X_DEFFAULT (127)
#define ISOMETRIC_ORIGEN_PROJ_Y_DEFFAULT (64)

#define ISOMETRIC_ORIENTATION_MASK_NS (1)
#define ISOMETRIC_ORIENTATION_N (0)
#define ISOMETRIC_ORIENTATION_S (1)
#define ISOMETRIC_ORIENTATION_MASK_EW (2)
#define ISOMETRIC_ORIENTATION_E (0)
#define ISOMETRIC_ORIENTATION_W (2)

#define N_MAX_ORDERED_ISOMETRIC_OBJECTS (16)

typedef struct
{
	// Coordenadas, caja de contencion, parametros fisicos y de comportamiento
	t_physics_box3d physics;
	// sprite grafico
	t_sprite sprite;
} t_isometric_obj;

extern byte isometric_origen_proj_x;
extern byte isometric_origen_proj_y;

extern t_isometric_obj *ordered_isometric_objects_table[];
extern byte n_ordered_isometric_objects;

#define reset_isometric_objects_ordering() {n_ordered_isometric_objects = 0;}

void isometric_proj_obj(t_isometric_obj *p_isometric_obj);
void isometric_add_object_to_order(t_isometric_obj *p_isometric_obj);

#endif