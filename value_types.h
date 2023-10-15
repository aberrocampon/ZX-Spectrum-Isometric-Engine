#ifndef VALUE_TYPES_H
#define VALUE_TYPES_H

#define NULL ((void *)0)

typedef unsigned char byte;
typedef char int8;
typedef struct
{
	byte x;
	byte y;
	byte z;
} t_b_vec3d;
typedef struct
{
    // posicion 3d origen es el punto central en la caja de contencion
	byte pos_x;
	byte pos_y;
	byte pos_z;
	// dimensiones de la caja de contencion
	byte width_x;
	byte width_y;
	byte height;
} t_box3d;

#endif