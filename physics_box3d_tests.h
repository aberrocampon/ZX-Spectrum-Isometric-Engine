
#ifndef PHYSICS_BOX3D_TESTS_H
#define PHYSICS_BOX3D_TESTS_H

#include "value_types.h"

#define PHYS_BOX3D_MAX_X_3D_DEFFAULT (125)
#define PHYS_BOX3D_MAX_Y_3D_DEFFAULT (125)
#define PHYS_BOX3D_MAX_Z_3D_DEFFAULT (60)

#define PHYS_BOX3D_MASK_TOUCH_N   (~1)
#define PHYS_BOX3D_FLAG_TOUCH_N   1
#define PHYS_BOX3D_MASK_TOUCH_S   (~2)
#define PHYS_BOX3D_FLAG_TOUCH_S   2
#define PHYS_BOX3D_MASK_TOUCH_E   (~4)
#define PHYS_BOX3D_FLAG_TOUCH_E   4
#define PHYS_BOX3D_MASK_TOUCH_W   (~8)
#define PHYS_BOX3D_FLAG_TOUCH_W   8
#define PHYS_BOX3D_MASK_TOUCH_U   (~16)
#define PHYS_BOX3D_FLAG_TOUCH_U   16
#define PHYS_BOX3D_MASK_TOUCH_D   (~32)
#define PHYS_BOX3D_FLAG_TOUCH_D   32
// No recibe impactos del resto de objeto que alteren su trayectoria y velocidad. Pero si ejerce impactos sobre el resto alterando la velocidad y trayectoria de estos. Este impacto frena al objeto cinematico. No experimenta gravedad tampoco.
#define PHYS_BOX3D_MASK_CINEMATIC (~64)
#define PHYS_BOX3D_FLAG_CINEMATIC (64)

#define PHYS_BOX3D_CONTACT_TOLERANCE 5
#define N_MAX_PHYS_BOX3D_OBJECTS (16)

typedef struct physics_box3d
{
	byte enabled;
	// posicion 3d origen es el vertice del plano inferior y con menor x e y de la caja que contiene la forma
	t_box3d box3d;
	// precalculos para hacer mas eficiente la ejecucion del modelo fisico. Solo se calculan una vez a cambio de 6 bytes extra en cada onjeto.
	byte min_pos_x;
	byte max_pos_x;
	byte min_pos_y;
	byte max_pos_y;
	byte min_pos_z;
	byte max_pos_z;
	byte last_pos_x;
	byte last_pos_y;
	byte last_pos_z;
	int8 speed_x;
	int8 speed_y;
	int8 speed_z;
	// contador para reducir el efecto de la garvedad.
	byte gravity_count;
	// contacto del objeto caja en las 6 cara con otros objetos, o paredes, techo y suelo
	byte flags;
	struct physics_box3d *p_phys_obj_touching_n; // contacto en la cara norte
	struct physics_box3d *p_phys_obj_touching_s; // contacto en la cara sur
	struct physics_box3d *p_phys_obj_touching_e; // contacto en la cara este
	struct physics_box3d *p_phys_obj_touching_w; // contacto en la cara oeste
	struct physics_box3d *p_phys_obj_touching_d; // contacto en la cara de abajo
	struct physics_box3d *p_phys_obj_touching_u; // contacto en la cara de arriba
} t_physics_box3d;

#define T_PHYSICS_BOX3D_SIZE 33
#define T_PHYSICS_BOX3D_OFFSET_POS_X 1
#define T_PHYSICS_BOX3D_OFFSET_POS_Y 2
#define T_PHYSICS_BOX3D_OFFSET_POS_Z 3
#define T_PHYSICS_BOX3D_OFFSET_WIDTH_X 4
#define T_PHYSICS_BOX3D_OFFSET_WIDTH_Y 5
#define T_PHYSICS_BOX3D_OFFSET_HEIGHT 6
#define T_PHYSICS_BOX3D_OFFSET_MIN_POS_X 7
#define T_PHYSICS_BOX3D_OFFSET_MAX_POS_X 8
#define T_PHYSICS_BOX3D_OFFSET_MIN_POS_Y 9
#define T_PHYSICS_BOX3D_OFFSET_MAX_POS_Y 10
#define T_PHYSICS_BOX3D_OFFSET_MIN_POS_Z 11
#define T_PHYSICS_BOX3D_OFFSET_MAX_POS_Z 12
#define T_PHYSICS_BOX3D_OFFSET_SPEED_X 16
#define T_PHYSICS_BOX3D_OFFSET_SPEED_Y 17
#define T_PHYSICS_BOX3D_OFFSET_SPEED_Z 18
#define T_PHYSICS_BOX3D_OFFSET_GRAVITY_COUNT 19
#define T_PHYSICS_BOX3D_OFFSET_FLAGS 20
#define T_PHYSICS_BOX3D_OFFSET_P_PHYS_OBJ_TOUCH_N 21
#define T_PHYSICS_BOX3D_OFFSET_P_PHYS_OBJ_TOUCH_S 23
#define T_PHYSICS_BOX3D_OFFSET_P_PHYS_OBJ_TOUCH_E 25
#define T_PHYSICS_BOX3D_OFFSET_P_PHYS_OBJ_TOUCH_W 27
#define T_PHYSICS_BOX3D_OFFSET_P_PHYS_OBJ_TOUCH_D 29
#define T_PHYSICS_BOX3D_OFFSET_P_PHYS_OBJ_TOUCH_U 31

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