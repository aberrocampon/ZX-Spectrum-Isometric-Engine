#ifndef VALUE_TYPES_H
#define VALUE_TYPES_H

#define NULL ((void *)0)

typedef unsigned char byte;
typedef char int8;
typedef struct
{
    // posicion 3d origen es el vertice del plano inferior y con menor x e y de la caja que contiene la forma
	byte pos_x;
	byte pos_y;
	byte pos_z;
	// dimensiones de la caja de contencion
	byte width_x;
	byte width_y;
	byte height;
} t_box3d;

#endif