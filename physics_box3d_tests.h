
#ifndef PHYSICS_BOX3D_TESTS_H
#define PHYSICS_BOX3D_TESTS_H

#include "value_types.h"

#define PHYS_BOX3D_MAX_X_3D_DEFFAULT (125)
#define PHYS_BOX3D_MAX_Y_3D_DEFFAULT (125)
#define PHYS_BOX3D_MAX_Z_3D_DEFFAULT (60)

#define PHYS_BOX3D_MASK_TOUCH_N   (~1)
#define PHYS_BOX3D_FLAG_TOUCH_N   (1)
#define PHYS_BOX3D_MASK_TOUCH_S   (~2)
#define PHYS_BOX3D_FLAG_TOUCH_S   (2)
#define PHYS_BOX3D_MASK_TOUCH_E   (~4)
#define PHYS_BOX3D_FLAG_TOUCH_E   (4)
#define PHYS_BOX3D_MASK_TOUCH_W   (~8)
#define PHYS_BOX3D_FLAG_TOUCH_W   (8)
#define PHYS_BOX3D_MASK_TOUCH_U   (~16)
#define PHYS_BOX3D_FLAG_TOUCH_U   (16)
#define PHYS_BOX3D_MASK_TOUCH_D   (~32)
#define PHYS_BOX3D_FLAG_TOUCH_D   (32)
// No recibe impactos del resto de objeto que alteren su trayectoria y velocidad. Pero si ejerce impactos sobre el resto alterando la velocidad y trayectoria de estos. Este impacto frena al objeto cinematico. No experimenta gravedad tampoco.
#define PHYS_BOX3D_MASK_CINEMATIC (~64)
#define PHYS_BOX3D_FLAG_CINEMATIC (64)

#define PHYS_BOX3D_CONTACT_TOLERANCE (5)
#define N_MAX_PHYS_BOX3D_OBJECTS (16)

typedef struct physics_box3d
{
	// posicion 3d origen es el vertice del plano inferior y con menor x e y de la caja que contiene la forma
	t_box3d box3d;
	byte last_pos_x;
	byte last_pos_y;
	byte last_pos_z;
	int8 speed_x;
	int8 speed_y;
	int8 speed_z;
	byte enabled;
	// contador para reducir el efecto de la garvedad.
	byte gravity_count;
	// contacto del objeto caja en las 6 cara con otros objetos, o paredes, techo y suelo
	byte flags;
	struct physics_box3d *p_phys_obj_touching_n; // contacto en la cara norte
	struct physics_box3d *p_phys_obj_touching_s; // contacto en la cara sur
	struct physics_box3d *p_phys_obj_touching_e; // contacto en la cara este
	struct physics_box3d *p_phys_obj_touching_w; // contacto en la cara oeste
	struct physics_box3d *p_phys_obj_touching_u; // contacto en la cara de arriba
	struct physics_box3d *p_phys_obj_touching_d; // contacto en la cara de abajo
} t_physics_box3d;

extern byte isometric_max_x_3d;
extern byte isometric_max_y_3d;
extern byte isometric_max_z_3d;
extern byte half_isometric_max_x_3d;
extern byte half_isometric_max_y_3d;
extern byte half_isometric_max_z_3d;

extern t_physics_box3d phys_box3d_wall_n;
extern t_physics_box3d phys_box3d_wall_s;
extern t_physics_box3d phys_box3d_wall_e;
extern t_physics_box3d phys_box3d_wall_w;

extern t_physics_box3d **pp_phys_box3d_objects_table;
extern byte n_phys_box3d_objects;

void phys_box3d_step(void);

#endif