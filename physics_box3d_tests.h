
#ifndef PHYSICS_BOX3D_TESTS_H
#define PHYSICS_BOX3D_TESTS_H

#include "value_types.h"

#define ISOMETRIC_MAX_X_3D_DEFFAULT (125)
#define ISOMETRIC_MAX_Y_3D_DEFFAULT (125)
#define ISOMETRIC_MAX_Z_3D_DEFFAULT (60)

#define N_MAX_PHYS_BOX3D_OBJECTS (16)

typedef struct
{
	// posicion 3d origen es el vertice del plano inferior y con menor x e y de la caja que contiene la forma
	t_box3d box3d;
	int8 speed_x;
	int8 speed_y;
	int8 speed_z;
} t_physics_box3d;

extern byte isometric_max_x_3d;
extern byte isometric_max_y_3d;
extern byte isometric_max_z_3d;

extern byte n_phys_box3d_objects;

#define init_phys_box3d_step() {n_phys_box3d_objects = 0;}

void phys_box3d_add_object(t_physics_box3d *p_phys_obj);
void phys_box3d_step(void);

#endif