
#include "physics_box3d_tests.h"

#define PHYS_BOX3D_GRAVITY_INTENSITY_INIT_COUNTER (4)
#define PHYS_BOX3D_STATIC_FRICTION_MASK (7)

byte isometric_max_x_3d = PHYS_BOX3D_MAX_X_3D_DEFFAULT;
byte isometric_max_y_3d = PHYS_BOX3D_MAX_Y_3D_DEFFAULT;
byte isometric_max_z_3d = PHYS_BOX3D_MAX_Z_3D_DEFFAULT;

t_physics_box3d phys_box3d_floor = 
{
    {
        PHYS_BOX3D_MAX_X_3D_DEFFAULT/2, PHYS_BOX3D_MAX_Y_3D_DEFFAULT/2, 0,
        PHYS_BOX3D_MAX_X_3D_DEFFAULT/2, PHYS_BOX3D_MAX_Y_3D_DEFFAULT/2, 0
    },
    0, 0, 0
};

t_physics_box3d phys_box3d_ceiling = 
{
    {
        PHYS_BOX3D_MAX_X_3D_DEFFAULT/2, PHYS_BOX3D_MAX_Y_3D_DEFFAULT/2, PHYS_BOX3D_MAX_Z_3D_DEFFAULT,
        PHYS_BOX3D_MAX_X_3D_DEFFAULT/2, PHYS_BOX3D_MAX_Y_3D_DEFFAULT/2, 0
    },
    0, 0, 0
};

t_physics_box3d phys_box3d_wall_n = 
{
    {
        PHYS_BOX3D_MAX_X_3D_DEFFAULT/2, 0, PHYS_BOX3D_MAX_Z_3D_DEFFAULT/2,
        PHYS_BOX3D_MAX_X_3D_DEFFAULT, 0, PHYS_BOX3D_MAX_Z_3D_DEFFAULT
    },
    0, 0, 0
};

t_physics_box3d phys_box3d_wall_s = 
{
    {
        PHYS_BOX3D_MAX_X_3D_DEFFAULT/2, PHYS_BOX3D_MAX_Y_3D_DEFFAULT, PHYS_BOX3D_MAX_Z_3D_DEFFAULT/2,
        PHYS_BOX3D_MAX_X_3D_DEFFAULT/2, 0, PHYS_BOX3D_MAX_Z_3D_DEFFAULT/2
    },
    0, 0, 0
};

t_physics_box3d phys_box3d_wall_e = 
{
    {
        0, PHYS_BOX3D_MAX_Y_3D_DEFFAULT/2, PHYS_BOX3D_MAX_Z_3D_DEFFAULT/2,
        0, PHYS_BOX3D_MAX_Y_3D_DEFFAULT/2, PHYS_BOX3D_MAX_Z_3D_DEFFAULT/2
    },
    0, 0, 0
};

t_physics_box3d phys_box3d_wall_w = 
{
    {
        PHYS_BOX3D_MAX_X_3D_DEFFAULT, PHYS_BOX3D_MAX_Y_3D_DEFFAULT/2, PHYS_BOX3D_MAX_Z_3D_DEFFAULT/2,
        0, PHYS_BOX3D_MAX_Y_3D_DEFFAULT/2, PHYS_BOX3D_MAX_Z_3D_DEFFAULT/2
    },
    0, 0, 0
};

t_physics_box3d *phys_box3d_objects_table[N_MAX_PHYS_BOX3D_OBJECTS];
byte n_phys_box3d_objects = 0;

byte predivisor_friction = 0;

void phys_box3d_add_object_to_table(t_physics_box3d *p_phys_obj)
{
    if(n_phys_box3d_objects >= N_MAX_PHYS_BOX3D_OBJECTS) return;

    // Añade a la tabla de objetos fisicos
    phys_box3d_objects_table[n_phys_box3d_objects++] = p_phys_obj;
}

void phys_box3d_step(void)
{
    byte i, j, k;
    byte coord1, coord2;
    int8 delta_speed_x2, delta_speed;
    t_physics_box3d **pp_phys_obj, *p_phys_obj,**pp_phys_another_obj, *p_phys_another_obj;

    ++predivisor_friction;

    // step de la aceletacion de la gravedad
    for(i = 0, pp_phys_obj = phys_box3d_objects_table; i < n_phys_box3d_objects; i++, pp_phys_obj++)
    {
        p_phys_obj = *pp_phys_obj;

        // Amplica gravedad
        if( !(p_phys_obj->flags & PHYS_BOX3D_FLAG_CINEMATIC) && !(--p_phys_obj->gravity_count) )
        {
            p_phys_obj->speed_z -= 1;
            p_phys_obj->gravity_count = PHYS_BOX3D_GRAVITY_INTENSITY_INIT_COUNTER;
        }

        // reset touching flags
        p_phys_obj->flags &= (PHYS_BOX3D_MASK_TOUCH_N & PHYS_BOX3D_MASK_TOUCH_S & PHYS_BOX3D_MASK_TOUCH_E & PHYS_BOX3D_MASK_TOUCH_W & PHYS_BOX3D_MASK_TOUCH_U);
        if( !( (p_phys_obj->flags & PHYS_BOX3D_FLAG_TOUCH_D) && ((p_phys_obj->box3d.pos_z - p_phys_obj->box3d.height) == (p_phys_obj->p_phys_obj_touching_d->box3d.pos_z + p_phys_obj->p_phys_obj_touching_d->box3d.height)) &&
            !(((p_phys_obj->box3d.pos_x + p_phys_obj->box3d.width_x) <= (p_phys_obj->p_phys_obj_touching_d->box3d.pos_x - p_phys_obj->p_phys_obj_touching_d->box3d.width_x)) || ((p_phys_obj->box3d.pos_x - p_phys_obj->box3d.width_x) >= (p_phys_obj->p_phys_obj_touching_d->box3d.pos_x + p_phys_obj->p_phys_obj_touching_d->box3d.width_x)) ||
                    ((p_phys_obj->box3d.pos_y + p_phys_obj->box3d.width_y) <= (p_phys_obj->p_phys_obj_touching_d->box3d.pos_y - p_phys_obj->p_phys_obj_touching_d->box3d.width_y)) || ((p_phys_obj->box3d.pos_y - p_phys_obj->box3d.width_y) >= (p_phys_obj->p_phys_obj_touching_d->box3d.pos_y + p_phys_obj->p_phys_obj_touching_d->box3d.width_y))) &&
            (p_phys_obj->speed_z == p_phys_obj->p_phys_obj_touching_d->speed_z) && (p_phys_obj->speed_z == 0) ) )
        {
            p_phys_obj->flags &= PHYS_BOX3D_MASK_TOUCH_D;
        }

        // Truco para estabilizar la coordenada z del objeto que se encuentre sobre un objeto que este a su vez sobre el suelo
        if((int8)(p_phys_obj->box3d.pos_z - p_phys_obj->box3d.height) < -p_phys_obj->speed_z )
        {
            p_phys_obj->speed_z = -(p_phys_obj->box3d.pos_z - p_phys_obj->box3d.height);
            p_phys_obj->flags |= PHYS_BOX3D_FLAG_TOUCH_D;
            p_phys_obj->p_phys_obj_touching_d = &phys_box3d_floor;
        }
    }

    for(k = 0; k < 1; k++)
    {
        for(i = 0, pp_phys_obj = phys_box3d_objects_table; i < n_phys_box3d_objects - 1; i++, pp_phys_obj++)
        {
            p_phys_obj = *pp_phys_obj;

            j = i + 1; // bug del compilador ??
            for(pp_phys_another_obj = pp_phys_obj + 1; j < n_phys_box3d_objects; j++, pp_phys_another_obj++)
            {
                p_phys_another_obj = *pp_phys_another_obj;

                if(!(((p_phys_obj->box3d.pos_y + p_phys_obj->box3d.width_y) <= (p_phys_another_obj->box3d.pos_y - p_phys_another_obj->box3d.width_y)) || ((p_phys_obj->box3d.pos_y - p_phys_obj->box3d.width_y) >= (p_phys_another_obj->box3d.pos_y + p_phys_another_obj->box3d.width_y)) ||
                        ((p_phys_obj->box3d.pos_z + p_phys_obj->box3d.height) <= (p_phys_another_obj->box3d.pos_z - p_phys_another_obj->box3d.height)) || ((p_phys_obj->box3d.pos_z - p_phys_obj->box3d.height) >= (p_phys_another_obj->box3d.pos_z + p_phys_another_obj->box3d.height))))
                {
                    if(/*((p_phys_obj->speed_x - p_phys_another_obj->speed_x) < 0) &&*/ (p_phys_obj->box3d.pos_x > p_phys_another_obj->box3d.pos_x))
                    {
                        coord1 = p_phys_obj->box3d.pos_x - p_phys_obj->box3d.width_x + p_phys_obj->speed_x;
                        coord2 = p_phys_another_obj->box3d.pos_x + p_phys_another_obj->box3d.width_x + p_phys_another_obj->speed_x;
                        if((coord1 < coord2) && ((coord2 - coord1) < PHYS_BOX3D_CONTACT_TOLERANCE))
                        {
                            if( (p_phys_obj->flags & (PHYS_BOX3D_FLAG_CINEMATIC | PHYS_BOX3D_FLAG_TOUCH_W)) && !(p_phys_another_obj->flags & (PHYS_BOX3D_FLAG_CINEMATIC | PHYS_BOX3D_FLAG_TOUCH_E)) )
                            {
                                p_phys_another_obj->speed_x = (p_phys_obj->box3d.pos_x - p_phys_obj->box3d.width_x + p_phys_obj->speed_x) - (p_phys_another_obj->box3d.pos_x + p_phys_another_obj->box3d.width_x);
                            }
                            else if( !(p_phys_obj->flags & (PHYS_BOX3D_FLAG_CINEMATIC | PHYS_BOX3D_FLAG_TOUCH_W)) && (p_phys_another_obj->flags & (PHYS_BOX3D_FLAG_CINEMATIC | PHYS_BOX3D_FLAG_TOUCH_E)) )
                            {
                                p_phys_obj->speed_x = (p_phys_another_obj->box3d.pos_x + p_phys_another_obj->box3d.width_x + p_phys_another_obj->speed_x) - (p_phys_obj->box3d.pos_x - p_phys_obj->box3d.width_x);
                            }
                            else if( !(p_phys_obj->flags & (PHYS_BOX3D_FLAG_CINEMATIC | PHYS_BOX3D_FLAG_TOUCH_W)) && !(p_phys_another_obj->flags & (PHYS_BOX3D_FLAG_CINEMATIC | PHYS_BOX3D_FLAG_TOUCH_E)) )
                            {
                                delta_speed_x2 = (p_phys_another_obj->box3d.pos_x + p_phys_another_obj->box3d.width_x + p_phys_another_obj->speed_x) - (p_phys_obj->box3d.pos_x - p_phys_obj->box3d.width_x + p_phys_obj->speed_x) + 1;
                                delta_speed = delta_speed_x2 >> 1;
                                p_phys_obj->speed_x += delta_speed;
                                p_phys_another_obj->speed_x -= delta_speed;
                            }
                            else
                            {
                                p_phys_obj->speed_x = p_phys_another_obj->speed_x = 0;
                            }

                            if( (p_phys_obj->box3d.pos_x + p_phys_obj->speed_x) > (isometric_max_x_3d - p_phys_obj->box3d.width_x) )
                            {
                                p_phys_obj->speed_x = isometric_max_x_3d - p_phys_obj->box3d.width_x - p_phys_obj->box3d.pos_x;
                                p_phys_another_obj->speed_x = (p_phys_obj->box3d.pos_x - p_phys_obj->box3d.width_x + p_phys_obj->speed_x) - (p_phys_another_obj->box3d.pos_x + p_phys_another_obj->box3d.width_x);

                                p_phys_obj->flags |= PHYS_BOX3D_FLAG_TOUCH_W;
                                p_phys_obj->p_phys_obj_touching_w = &phys_box3d_wall_w;
                            }
                            else if((p_phys_another_obj->box3d.pos_x + p_phys_another_obj->speed_x) < p_phys_another_obj->box3d.width_x)
                            {
                                p_phys_another_obj->speed_x = p_phys_another_obj->box3d.width_x - p_phys_another_obj->box3d.pos_x;
                                p_phys_obj->speed_x = (p_phys_another_obj->box3d.width_x + p_phys_another_obj->box3d.width_x) - (p_phys_obj->box3d.pos_x - p_phys_obj->box3d.width_x);

                                p_phys_another_obj->flags |= PHYS_BOX3D_FLAG_TOUCH_E;
                                p_phys_another_obj->p_phys_obj_touching_e = &phys_box3d_wall_e;
                            }

                            p_phys_obj->flags |= PHYS_BOX3D_FLAG_TOUCH_E;
                            p_phys_obj->p_phys_obj_touching_e = p_phys_another_obj;
                            p_phys_another_obj->flags |= PHYS_BOX3D_FLAG_TOUCH_W;
                            p_phys_another_obj->p_phys_obj_touching_w = p_phys_obj;
                        }
                    }
                    else if(/*((p_phys_obj->speed_x - p_phys_another_obj->speed_x) > 0) &&*/ (p_phys_obj->box3d.pos_x < p_phys_another_obj->box3d.pos_x))
                    {
                        coord1 = p_phys_obj->box3d.pos_x + p_phys_obj->box3d.width_x + p_phys_obj->speed_x;
                        coord2 = p_phys_another_obj->box3d.pos_x - p_phys_another_obj->box3d.width_x + p_phys_another_obj->speed_x;
                        if((coord1 > coord2) && ((coord1 - coord2) < PHYS_BOX3D_CONTACT_TOLERANCE))
                        {
                            if( (p_phys_obj->flags & (PHYS_BOX3D_FLAG_CINEMATIC | PHYS_BOX3D_FLAG_TOUCH_E)) && !(p_phys_another_obj->flags & (PHYS_BOX3D_FLAG_CINEMATIC | PHYS_BOX3D_FLAG_TOUCH_W)) )
                            {
                                p_phys_another_obj->speed_x = (p_phys_obj->box3d.pos_x + p_phys_obj->box3d.width_x + p_phys_obj->speed_x) - (p_phys_another_obj->box3d.pos_x - p_phys_another_obj->box3d.width_x);
                            }
                            else if( !(p_phys_obj->flags & (PHYS_BOX3D_FLAG_CINEMATIC | PHYS_BOX3D_FLAG_TOUCH_E)) && (p_phys_another_obj->flags & (PHYS_BOX3D_FLAG_CINEMATIC | PHYS_BOX3D_FLAG_TOUCH_W)) )
                            {
                                p_phys_obj->speed_x = (p_phys_another_obj->box3d.pos_x - p_phys_another_obj->box3d.width_x + p_phys_another_obj->speed_x) - (p_phys_obj->box3d.pos_x + p_phys_obj->box3d.width_x);
                            }
                            else if( !(p_phys_obj->flags & (PHYS_BOX3D_FLAG_CINEMATIC | PHYS_BOX3D_FLAG_TOUCH_E)) && !(p_phys_another_obj->flags & (PHYS_BOX3D_FLAG_CINEMATIC | PHYS_BOX3D_FLAG_TOUCH_W)) )
                            {
                                delta_speed_x2 = (p_phys_obj->box3d.pos_x + p_phys_obj->box3d.width_x + p_phys_obj->speed_x) - (p_phys_another_obj->box3d.pos_x - p_phys_another_obj->box3d.width_x + p_phys_another_obj->speed_x) + 1;
                                delta_speed = delta_speed_x2 >> 1;
                                p_phys_obj->speed_x -= delta_speed;
                                p_phys_another_obj->speed_x += delta_speed;
                            }
                            else
                            {
                                p_phys_obj->speed_x = p_phys_another_obj->speed_x = 0;
                            }

                            if( (p_phys_another_obj->box3d.pos_x + p_phys_another_obj->speed_x) > (isometric_max_x_3d - p_phys_another_obj->box3d.width_x) )
                            {
                                p_phys_another_obj->speed_x = isometric_max_x_3d - p_phys_another_obj->box3d.width_x - p_phys_another_obj->box3d.pos_x;
                                p_phys_obj->speed_x = (p_phys_another_obj->box3d.pos_x - p_phys_another_obj->box3d.width_x + p_phys_another_obj->speed_x) - (p_phys_obj->box3d.pos_x + p_phys_obj->box3d.width_x);

                                p_phys_another_obj->flags |= PHYS_BOX3D_FLAG_TOUCH_W;
                                p_phys_another_obj->p_phys_obj_touching_w = &phys_box3d_wall_w;
                            }
                            else if((p_phys_obj->box3d.pos_x + p_phys_obj->speed_x) < p_phys_obj->box3d.width_x)
                            {
                                p_phys_obj->speed_x = p_phys_obj->box3d.width_x - p_phys_obj->box3d.pos_x;
                                p_phys_another_obj->speed_x = (p_phys_obj->box3d.width_x + p_phys_obj->box3d.width_x) - (p_phys_another_obj->box3d.pos_x - p_phys_another_obj->box3d.width_x);

                                p_phys_obj->flags |= PHYS_BOX3D_FLAG_TOUCH_E;
                                p_phys_obj->p_phys_obj_touching_e = &phys_box3d_wall_e;
                            }

                            p_phys_obj->flags |= PHYS_BOX3D_FLAG_TOUCH_W;
                            p_phys_obj->p_phys_obj_touching_w = p_phys_another_obj;
                            p_phys_another_obj->flags |= PHYS_BOX3D_FLAG_TOUCH_E;
                            p_phys_another_obj->p_phys_obj_touching_e = p_phys_obj;
                        }
                    }
                }

                if(!(((p_phys_obj->box3d.pos_x + p_phys_obj->box3d.width_x) <= (p_phys_another_obj->box3d.pos_x - p_phys_another_obj->box3d.width_x)) || ((p_phys_obj->box3d.pos_x - p_phys_obj->box3d.width_x) >= (p_phys_another_obj->box3d.pos_x + p_phys_another_obj->box3d.width_x)) ||
                        ((p_phys_obj->box3d.pos_z + p_phys_obj->box3d.height) <= (p_phys_another_obj->box3d.pos_z - p_phys_another_obj->box3d.height)) || ((p_phys_obj->box3d.pos_z - p_phys_obj->box3d.height) >= (p_phys_another_obj->box3d.pos_z + p_phys_another_obj->box3d.height))))
                {
                    if(/*((p_phys_obj->speed_y - p_phys_another_obj->speed_y) < 0) &&*/ (p_phys_obj->box3d.pos_y > p_phys_another_obj->box3d.pos_y))
                    {
                        coord1 = p_phys_obj->box3d.pos_y - p_phys_obj->box3d.width_y + p_phys_obj->speed_y;
                        coord2 = p_phys_another_obj->box3d.pos_y + p_phys_another_obj->box3d.width_y + p_phys_another_obj->speed_y;
                        if((coord1 < coord2) && ((coord2 - coord1) < PHYS_BOX3D_CONTACT_TOLERANCE))
                        {
                            if( (p_phys_obj->flags & (PHYS_BOX3D_FLAG_CINEMATIC | PHYS_BOX3D_FLAG_TOUCH_S)) && !(p_phys_another_obj->flags & (PHYS_BOX3D_FLAG_CINEMATIC | PHYS_BOX3D_FLAG_TOUCH_N)) )
                            {
                                p_phys_another_obj->speed_y = (p_phys_obj->box3d.pos_y - p_phys_obj->box3d.width_y + p_phys_obj->speed_y) - (p_phys_another_obj->box3d.pos_y + p_phys_another_obj->box3d.width_y);
                            }
                            else if( !(p_phys_obj->flags & (PHYS_BOX3D_FLAG_CINEMATIC | PHYS_BOX3D_FLAG_TOUCH_S)) && (p_phys_another_obj->flags & (PHYS_BOX3D_FLAG_CINEMATIC | PHYS_BOX3D_FLAG_TOUCH_N)) )
                            {
                                p_phys_obj->speed_y = (p_phys_another_obj->box3d.pos_y + p_phys_another_obj->box3d.width_y + p_phys_another_obj->speed_y) - (p_phys_obj->box3d.pos_y - p_phys_obj->box3d.width_y);
                            }
                            else if( !(p_phys_obj->flags & (PHYS_BOX3D_FLAG_CINEMATIC | PHYS_BOX3D_FLAG_TOUCH_S)) && !(p_phys_another_obj->flags & (PHYS_BOX3D_FLAG_CINEMATIC | PHYS_BOX3D_FLAG_TOUCH_N)) )
                            {
                                delta_speed_x2 = (p_phys_another_obj->box3d.pos_y + p_phys_another_obj->box3d.width_y + p_phys_another_obj->speed_y) - (p_phys_obj->box3d.pos_y - p_phys_obj->box3d.width_y + p_phys_obj->speed_y) + 1;
                                delta_speed = delta_speed_x2 >> 1;
                                p_phys_obj->speed_y += delta_speed;
                                p_phys_another_obj->speed_y -= delta_speed;
                            }
                            else
                            {
                                p_phys_obj->speed_y = p_phys_another_obj->speed_y = 0;
                            }

                            if( (p_phys_obj->box3d.pos_y + p_phys_obj->speed_y) > (isometric_max_y_3d - p_phys_obj->box3d.width_y) )
                            {
                                p_phys_obj->speed_y = isometric_max_y_3d - p_phys_obj->box3d.width_y - p_phys_obj->box3d.pos_y;
                                p_phys_another_obj->speed_y = (p_phys_obj->box3d.pos_y - p_phys_obj->box3d.width_y + p_phys_obj->speed_y) - (p_phys_another_obj->box3d.pos_y + p_phys_another_obj->box3d.width_y);

                                p_phys_obj->flags |= PHYS_BOX3D_FLAG_TOUCH_S;
                                p_phys_obj->p_phys_obj_touching_s = &phys_box3d_wall_s;
                            }
                            else if((p_phys_another_obj->box3d.pos_y + p_phys_another_obj->speed_y) < p_phys_another_obj->box3d.width_y)
                            {
                                p_phys_another_obj->speed_y = p_phys_another_obj->box3d.width_y - p_phys_another_obj->box3d.pos_y;
                                p_phys_obj->speed_y = (p_phys_another_obj->box3d.width_y + p_phys_another_obj->box3d.width_y) - (p_phys_obj->box3d.pos_y - p_phys_obj->box3d.width_y);

                                p_phys_another_obj->flags |= PHYS_BOX3D_FLAG_TOUCH_N;
                                p_phys_another_obj->p_phys_obj_touching_n = &phys_box3d_wall_n;
                            }

                            p_phys_obj->flags |= PHYS_BOX3D_FLAG_TOUCH_N;
                            p_phys_obj->p_phys_obj_touching_n = p_phys_another_obj;
                            p_phys_another_obj->flags |= PHYS_BOX3D_FLAG_TOUCH_S;
                            p_phys_another_obj->p_phys_obj_touching_s = p_phys_obj;
                        }
                    }
                    else if(/*((p_phys_obj->speed_y - p_phys_another_obj->speed_y) > 0) &&*/ (p_phys_obj->box3d.pos_y < p_phys_another_obj->box3d.pos_y))
                    {
                        coord1 = p_phys_obj->box3d.pos_y + p_phys_obj->box3d.width_y + p_phys_obj->speed_y;
                        coord2 = p_phys_another_obj->box3d.pos_y - p_phys_another_obj->box3d.width_y + p_phys_another_obj->speed_y;
                        if((coord1 > coord2) && ((coord1 - coord2) < PHYS_BOX3D_CONTACT_TOLERANCE))
                        {
                            if( (p_phys_obj->flags & (PHYS_BOX3D_FLAG_CINEMATIC | PHYS_BOX3D_FLAG_TOUCH_N)) && !(p_phys_another_obj->flags & (PHYS_BOX3D_FLAG_CINEMATIC | PHYS_BOX3D_FLAG_TOUCH_S)) )
                            {
                                p_phys_another_obj->speed_y = (p_phys_obj->box3d.pos_y + p_phys_obj->box3d.width_y + p_phys_obj->speed_y) - (p_phys_another_obj->box3d.pos_y - p_phys_another_obj->box3d.width_y);
                            }
                            else if( !(p_phys_obj->flags & (PHYS_BOX3D_FLAG_CINEMATIC | PHYS_BOX3D_FLAG_TOUCH_N)) && (p_phys_another_obj->flags & (PHYS_BOX3D_FLAG_CINEMATIC | PHYS_BOX3D_FLAG_TOUCH_S)) )
                            {
                                p_phys_obj->speed_y = (p_phys_another_obj->box3d.pos_y - p_phys_another_obj->box3d.width_y + p_phys_another_obj->speed_y) - (p_phys_obj->box3d.pos_y + p_phys_obj->box3d.width_y);
                            }
                            else if( !(p_phys_obj->flags & (PHYS_BOX3D_FLAG_CINEMATIC | PHYS_BOX3D_FLAG_TOUCH_N)) && !(p_phys_another_obj->flags & (PHYS_BOX3D_FLAG_CINEMATIC | PHYS_BOX3D_FLAG_TOUCH_S)) )
                            {
                                delta_speed_x2 = (p_phys_obj->box3d.pos_y + p_phys_obj->box3d.width_y + p_phys_obj->speed_y) - (p_phys_another_obj->box3d.pos_y - p_phys_another_obj->box3d.width_y + p_phys_another_obj->speed_y) + 1;
                                delta_speed = delta_speed_x2 >> 1;
                                p_phys_obj->speed_y -= delta_speed;
                                p_phys_another_obj->speed_y += delta_speed;
                            }
                            else
                            {
                                p_phys_obj->speed_y = p_phys_another_obj->speed_y = 0;
                            }

                            if( (p_phys_another_obj->box3d.pos_y + p_phys_another_obj->speed_y) > (isometric_max_y_3d - p_phys_another_obj->box3d.width_y) )
                            {
                                p_phys_another_obj->speed_y = isometric_max_y_3d - p_phys_another_obj->box3d.width_y - p_phys_another_obj->box3d.pos_y;
                                p_phys_obj->speed_y = (p_phys_another_obj->box3d.pos_y - p_phys_another_obj->box3d.width_y + p_phys_another_obj->speed_y) - (p_phys_obj->box3d.pos_y + p_phys_obj->box3d.width_y);

                                p_phys_another_obj->flags |= PHYS_BOX3D_FLAG_TOUCH_S;
                                p_phys_another_obj->p_phys_obj_touching_s = &phys_box3d_wall_s;
                            }
                            else if((p_phys_obj->box3d.pos_y + p_phys_obj->speed_y) < p_phys_obj->box3d.width_y)
                            {
                                p_phys_obj->speed_y = p_phys_obj->box3d.width_y - p_phys_obj->box3d.pos_y;
                                p_phys_another_obj->speed_y = (p_phys_obj->box3d.width_y + p_phys_obj->box3d.width_y) - (p_phys_another_obj->box3d.pos_y - p_phys_another_obj->box3d.width_y);

                                p_phys_obj->flags |= PHYS_BOX3D_FLAG_TOUCH_N;
                                p_phys_obj->p_phys_obj_touching_n = &phys_box3d_wall_n;
                            }

                            p_phys_obj->flags |= PHYS_BOX3D_FLAG_TOUCH_S;
                            p_phys_obj->p_phys_obj_touching_s = p_phys_another_obj;
                            p_phys_another_obj->flags |= PHYS_BOX3D_FLAG_TOUCH_N;
                            p_phys_another_obj->p_phys_obj_touching_n = p_phys_obj;
                        }
                    }
                }

                if(!(((p_phys_obj->box3d.pos_x + p_phys_obj->box3d.width_x) <= (p_phys_another_obj->box3d.pos_x - p_phys_another_obj->box3d.width_x)) || ((p_phys_obj->box3d.pos_x - p_phys_obj->box3d.width_x) >= (p_phys_another_obj->box3d.pos_x + p_phys_another_obj->box3d.width_x)) ||
                        ((p_phys_obj->box3d.pos_y + p_phys_obj->box3d.width_y) <= (p_phys_another_obj->box3d.pos_y - p_phys_another_obj->box3d.width_y)) || ((p_phys_obj->box3d.pos_y - p_phys_obj->box3d.width_y) >= (p_phys_another_obj->box3d.pos_y + p_phys_another_obj->box3d.width_y))))
                {
                    if(/*((p_phys_obj->speed_z - p_phys_another_obj->speed_z) < 0) &&*/ (p_phys_obj->box3d.pos_z > p_phys_another_obj->box3d.pos_z))
                    {
                        coord1 = p_phys_obj->box3d.pos_z - p_phys_obj->box3d.height + p_phys_obj->speed_z;
                        coord2 = p_phys_another_obj->box3d.pos_z + p_phys_another_obj->box3d.height + p_phys_another_obj->speed_z;
                        if((coord1 < coord2) && ((coord2 - coord1) < PHYS_BOX3D_CONTACT_TOLERANCE))
                        {
                            if( (p_phys_obj->flags & (PHYS_BOX3D_FLAG_CINEMATIC | PHYS_BOX3D_FLAG_TOUCH_U)) && !(p_phys_another_obj->flags & (PHYS_BOX3D_FLAG_CINEMATIC | PHYS_BOX3D_FLAG_TOUCH_D)) )
                            {
                                p_phys_another_obj->speed_z = (p_phys_obj->box3d.pos_z - p_phys_obj->box3d.height + p_phys_obj->speed_z) - (p_phys_another_obj->box3d.pos_z + p_phys_another_obj->box3d.height);
                            }
                            else if( !(p_phys_obj->flags & (PHYS_BOX3D_FLAG_CINEMATIC | PHYS_BOX3D_FLAG_TOUCH_U)) && (p_phys_another_obj->flags & (PHYS_BOX3D_FLAG_CINEMATIC | PHYS_BOX3D_FLAG_TOUCH_D)) )
                            {
                                p_phys_obj->speed_z = (p_phys_another_obj->box3d.pos_z + p_phys_another_obj->box3d.height + p_phys_another_obj->speed_z) - (p_phys_obj->box3d.pos_z - p_phys_obj->box3d.height);
                            }
                            else if( !(p_phys_obj->flags & (PHYS_BOX3D_FLAG_CINEMATIC | PHYS_BOX3D_FLAG_TOUCH_U)) && !(p_phys_another_obj->flags & (PHYS_BOX3D_FLAG_CINEMATIC | PHYS_BOX3D_FLAG_TOUCH_D)) )
                            {
                                delta_speed_x2 = (p_phys_another_obj->box3d.pos_z + p_phys_another_obj->box3d.height + p_phys_another_obj->speed_z) - (p_phys_obj->box3d.pos_z - p_phys_obj->box3d.height + p_phys_obj->speed_z) + 1;
                                delta_speed = delta_speed_x2 >> 1;
                                p_phys_obj->speed_z += delta_speed;
                                p_phys_another_obj->speed_z -= delta_speed;
                            }
                            else
                            {
                                p_phys_obj->speed_z = p_phys_another_obj->speed_z = 0;
                            }

                            if( (p_phys_obj->box3d.pos_z + p_phys_obj->speed_z) > (isometric_max_z_3d - p_phys_obj->box3d.height) )
                            {
                                p_phys_obj->speed_z = isometric_max_z_3d - p_phys_obj->box3d.height - p_phys_obj->box3d.pos_z;
                                p_phys_another_obj->speed_z = (p_phys_obj->box3d.pos_z - p_phys_obj->box3d.height + p_phys_obj->speed_z) - (p_phys_another_obj->box3d.pos_z + p_phys_another_obj->box3d.height);

                                p_phys_obj->flags |= PHYS_BOX3D_FLAG_TOUCH_U;
                                p_phys_obj->p_phys_obj_touching_u = &phys_box3d_ceiling;
                            }
                            else if((p_phys_another_obj->box3d.pos_z + p_phys_another_obj->speed_z) < p_phys_another_obj->box3d.height)
                            {
                                p_phys_another_obj->speed_z = p_phys_another_obj->box3d.height - p_phys_another_obj->box3d.pos_z;
                                p_phys_obj->speed_z = (p_phys_another_obj->box3d.height + p_phys_another_obj->box3d.height) - (p_phys_obj->box3d.pos_z - p_phys_obj->box3d.height);

                                p_phys_another_obj->flags |= PHYS_BOX3D_FLAG_TOUCH_D;
                                p_phys_another_obj->p_phys_obj_touching_d = &phys_box3d_floor;
                            }

                            p_phys_obj->flags |= PHYS_BOX3D_FLAG_TOUCH_D;
                            p_phys_obj->p_phys_obj_touching_d = p_phys_another_obj;
                            p_phys_another_obj->flags |= PHYS_BOX3D_FLAG_TOUCH_U;
                            p_phys_another_obj->p_phys_obj_touching_u = p_phys_obj;
                        }
                    }
                    else if(/*((p_phys_obj->speed_z - p_phys_another_obj->speed_z) > 0) &&*/ (p_phys_obj->box3d.pos_z < p_phys_another_obj->box3d.pos_z))
                    {
                        coord1 = p_phys_obj->box3d.pos_z + p_phys_obj->box3d.height + p_phys_obj->speed_z;
                        coord2 = p_phys_another_obj->box3d.pos_z - p_phys_another_obj->box3d.height + p_phys_another_obj->speed_z;
                        if((coord1 > coord2) && ((coord1 - coord2) < PHYS_BOX3D_CONTACT_TOLERANCE))
                        {
                            if( (p_phys_obj->flags & (PHYS_BOX3D_FLAG_CINEMATIC | PHYS_BOX3D_FLAG_TOUCH_D)) && !(p_phys_another_obj->flags & (PHYS_BOX3D_FLAG_CINEMATIC | PHYS_BOX3D_FLAG_TOUCH_U)) )
                            {
                                p_phys_another_obj->speed_z = (p_phys_obj->box3d.pos_z + p_phys_obj->box3d.height + p_phys_obj->speed_z) - (p_phys_another_obj->box3d.pos_z - p_phys_another_obj->box3d.height);
                            }
                            else if( !(p_phys_obj->flags & (PHYS_BOX3D_FLAG_CINEMATIC | PHYS_BOX3D_FLAG_TOUCH_D)) && (p_phys_another_obj->flags & (PHYS_BOX3D_FLAG_CINEMATIC | PHYS_BOX3D_FLAG_TOUCH_U)) )
                            {
                                p_phys_obj->speed_z = (p_phys_another_obj->box3d.pos_z - p_phys_another_obj->box3d.height + p_phys_another_obj->speed_z) - (p_phys_obj->box3d.pos_z + p_phys_obj->box3d.height);
                            }
                            else if( !(p_phys_obj->flags & (PHYS_BOX3D_FLAG_CINEMATIC | PHYS_BOX3D_FLAG_TOUCH_D)) && !(p_phys_another_obj->flags & (PHYS_BOX3D_FLAG_CINEMATIC | PHYS_BOX3D_FLAG_TOUCH_U)) )
                            {
                                delta_speed_x2 = (p_phys_obj->box3d.pos_z + p_phys_obj->box3d.height + p_phys_obj->speed_z) - (p_phys_another_obj->box3d.pos_z - p_phys_another_obj->box3d.height + p_phys_another_obj->speed_z) + 1;
                                delta_speed = delta_speed_x2 >> 1;
                                p_phys_obj->speed_z -= delta_speed;
                                p_phys_another_obj->speed_z += delta_speed;
                            }
                            else
                            {
                                p_phys_obj->speed_z = p_phys_another_obj->speed_z = 0;
                            }

                            if( (p_phys_another_obj->box3d.pos_z + p_phys_another_obj->speed_z) > (isometric_max_z_3d - p_phys_another_obj->box3d.height) )
                            {
                                p_phys_another_obj->speed_z = isometric_max_z_3d - p_phys_another_obj->box3d.height - p_phys_another_obj->box3d.pos_z;
                                p_phys_obj->speed_z = (p_phys_another_obj->box3d.pos_z - p_phys_another_obj->box3d.height + p_phys_another_obj->speed_z) - (p_phys_obj->box3d.pos_z + p_phys_obj->box3d.height);

                                p_phys_another_obj->flags |= PHYS_BOX3D_FLAG_TOUCH_U;
                                p_phys_another_obj->p_phys_obj_touching_u = &phys_box3d_ceiling;
                            }
                            else if((p_phys_obj->box3d.pos_z + p_phys_obj->speed_z) < p_phys_obj->box3d.height)
                            {
                                p_phys_obj->speed_z = p_phys_obj->box3d.height - p_phys_obj->box3d.pos_z;
                                p_phys_another_obj->speed_z = (p_phys_obj->box3d.height + p_phys_obj->box3d.height) - (p_phys_another_obj->box3d.pos_z - p_phys_another_obj->box3d.height);

                                p_phys_obj->flags |= PHYS_BOX3D_FLAG_TOUCH_D;
                                p_phys_obj->p_phys_obj_touching_d = &phys_box3d_floor;
                            }

                            p_phys_obj->flags |= PHYS_BOX3D_FLAG_TOUCH_U;
                            p_phys_obj->p_phys_obj_touching_u = p_phys_another_obj;
                            p_phys_another_obj->flags |= PHYS_BOX3D_FLAG_TOUCH_D;
                            p_phys_another_obj->p_phys_obj_touching_d = p_phys_obj;
                        }
                    }
                }
            }
        }
    }

    for(i = 0, pp_phys_obj = phys_box3d_objects_table; i < n_phys_box3d_objects; i++, pp_phys_obj++)
    {
        p_phys_obj = *pp_phys_obj;

        p_phys_obj->box3d.pos_x += p_phys_obj->speed_x;
        p_phys_obj->box3d.pos_y += p_phys_obj->speed_y;
        p_phys_obj->box3d.pos_z += p_phys_obj->speed_z;

        if(p_phys_obj->box3d.pos_x < p_phys_obj->box3d.width_x)
        {
            p_phys_obj->box3d.pos_x = p_phys_obj->box3d.width_x;
            if(p_phys_obj->speed_x < 0) 
            {
                p_phys_obj->speed_x = 0;
            }

            p_phys_obj->flags |= PHYS_BOX3D_FLAG_TOUCH_E;
            p_phys_obj->p_phys_obj_touching_e = &phys_box3d_wall_e;
        }

        if(p_phys_obj->box3d.pos_x > (isometric_max_x_3d - p_phys_obj->box3d.width_x))
        {
            p_phys_obj->box3d.pos_x = isometric_max_x_3d - p_phys_obj->box3d.width_x;
            if(p_phys_obj->speed_x > 0) 
            {
                p_phys_obj->speed_x = 0;
            }

            p_phys_obj->flags |= PHYS_BOX3D_FLAG_TOUCH_W;
            p_phys_obj->p_phys_obj_touching_w = &phys_box3d_wall_w;
        }

        if(p_phys_obj->box3d.pos_y < p_phys_obj->box3d.width_y)
        {
            p_phys_obj->box3d.pos_y = p_phys_obj->box3d.width_y;
            if(p_phys_obj->speed_y < 0) 
            {
                p_phys_obj->speed_y = 0;
            }

            p_phys_obj->flags |= PHYS_BOX3D_FLAG_TOUCH_N;
            p_phys_obj->p_phys_obj_touching_n = &phys_box3d_wall_n;
        }

        if(p_phys_obj->box3d.pos_y > (isometric_max_y_3d - p_phys_obj->box3d.width_y))
        {
            p_phys_obj->box3d.pos_y = isometric_max_y_3d - p_phys_obj->box3d.width_y;
            if(p_phys_obj->speed_y > 0) 
            {
                p_phys_obj->speed_y = 0;
            }

            p_phys_obj->flags |= PHYS_BOX3D_FLAG_TOUCH_S;
            p_phys_obj->p_phys_obj_touching_s = &phys_box3d_wall_s;
        }

        if(p_phys_obj->box3d.pos_z <= p_phys_obj->box3d.height)
        {
            p_phys_obj->box3d.pos_z = p_phys_obj->box3d.height;
            if(p_phys_obj->speed_z < 0) 
            {
                p_phys_obj->speed_z = 0;
            }

            p_phys_obj->flags |= PHYS_BOX3D_FLAG_TOUCH_D;
            p_phys_obj->p_phys_obj_touching_d = &phys_box3d_floor;
        }

        if(p_phys_obj->box3d.pos_z > (isometric_max_z_3d - p_phys_obj->box3d.height))
        {
            p_phys_obj->box3d.pos_z = isometric_max_z_3d - p_phys_obj->box3d.height;
            if(p_phys_obj->speed_z > 0) 
            {
                p_phys_obj->speed_z = 0;
            }

            p_phys_obj->flags |= PHYS_BOX3D_FLAG_TOUCH_U;
            p_phys_obj->p_phys_obj_touching_u = &phys_box3d_ceiling;
        }

        if(!(predivisor_friction & PHYS_BOX3D_STATIC_FRICTION_MASK) && (p_phys_obj->flags & PHYS_BOX3D_FLAG_TOUCH_D) && !(p_phys_obj->flags & PHYS_BOX3D_FLAG_CINEMATIC))
        {
            p_phys_another_obj = p_phys_obj->p_phys_obj_touching_d;

            if(p_phys_obj->speed_x > p_phys_another_obj->speed_x)
            {
                p_phys_obj->speed_x--;
            }
            else if(p_phys_obj->speed_x < p_phys_another_obj->speed_x)
            {
                p_phys_obj->speed_x++;
            }

            if(p_phys_obj->speed_y > p_phys_another_obj->speed_y)
            {
                p_phys_obj->speed_y--;
            }
            else if(p_phys_obj->speed_y < p_phys_another_obj->speed_y)
            {
                p_phys_obj->speed_y++;
            }
        }

        // Reset contador de reduccion de intensidad de la gravedad
        if(p_phys_obj->flags & (PHYS_BOX3D_FLAG_TOUCH_D | PHYS_BOX3D_FLAG_TOUCH_U))
        {
            p_phys_obj->gravity_count = PHYS_BOX3D_GRAVITY_INTENSITY_INIT_COUNTER;
        }
    }

}