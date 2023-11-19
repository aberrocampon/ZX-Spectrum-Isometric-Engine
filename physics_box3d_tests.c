
#include "physics_box3d_tests.h"

#define PHYS_BOX3D_GRAVITY_INTENSITY_INIT_COUNTER (4)
#define PHYS_BOX3D_STATIC_FRICTION_MASK (7)

byte isometric_max_x_3d = PHYS_BOX3D_MAX_X_3D_DEFFAULT;
byte isometric_max_y_3d = PHYS_BOX3D_MAX_Y_3D_DEFFAULT;
byte isometric_max_z_3d = PHYS_BOX3D_MAX_Z_3D_DEFFAULT;

byte half_isometric_max_x_3d = (PHYS_BOX3D_MAX_X_3D_DEFFAULT>>1);
byte half_isometric_max_y_3d = (PHYS_BOX3D_MAX_Y_3D_DEFFAULT>>1);
byte half_isometric_max_z_3d = (PHYS_BOX3D_MAX_Z_3D_DEFFAULT>>1);

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

t_physics_box3d **pp_phys_box3d_objects_table;
byte n_phys_box3d_objects;

byte predivisor_friction = 0;

#ifdef PHYS_BOX3D_STEP_IN_DEVELOPMENT
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
#else

t_physics_box3d *p_phys_obj1, *p_phys_obj2;
byte isometric_max_axis;
byte flag_touch1, flag_touch2;
t_physics_box3d *p_wall1, *p_wall2;
byte pos1, pos2;
byte width1, width2;
int8 speed1, speed2;
byte coord1, coord2;
int8 delta_speed_x2, delta_speed;
t_physics_box3d **pp_phy_obj_touching1_1, **pp_phy_obj_touching1_2, **pp_phy_obj_touching2_1, **pp_phy_obj_touching2_2;

void phys_box3d_step_subfunction(void)
{
    coord2 = pos2 - width2 + speed2;
    coord1 = pos1 + width1 + speed1;
    if((coord2 < coord1) && ((coord1 - coord2) < PHYS_BOX3D_CONTACT_TOLERANCE))
    {
        if( (p_phys_obj2->flags & (PHYS_BOX3D_FLAG_CINEMATIC | flag_touch2)) && !(p_phys_obj1->flags & (PHYS_BOX3D_FLAG_CINEMATIC | flag_touch1)) )
        {
            speed1 = (pos2 - width2 + speed2) - (pos1 + width1);
        }
        else if( !(p_phys_obj2->flags & (PHYS_BOX3D_FLAG_CINEMATIC | flag_touch2)) && (p_phys_obj1->flags & (PHYS_BOX3D_FLAG_CINEMATIC | flag_touch1)) )
        {
            speed2 = (pos1 + width1 + speed1) - (pos2 - width2);
        }
        else if( !(p_phys_obj2->flags & (PHYS_BOX3D_FLAG_CINEMATIC | flag_touch2)) && !(p_phys_obj1->flags & (PHYS_BOX3D_FLAG_CINEMATIC | flag_touch1)) )
        {
            delta_speed_x2 = (pos1 + width1 + speed1) - (pos2 - width2 + speed2) + 1; // + 1 => para redondear al dividor por 2 !!. En caso de no hacerlo uno de los dos queda quieto si el dividendo es 1
            delta_speed = delta_speed_x2 >> 1;
            speed2 += delta_speed;
            speed1 -= delta_speed;
        }
        else
        {
            speed2 = speed1 = 0;
        }

        if( (pos2 + speed2) > (isometric_max_axis - width2) )
        {
            speed2 = isometric_max_axis - width2 - pos2;
            speed1 = (pos2 - width2 + speed2) - (pos1 + width1);

            p_phys_obj2->flags |= flag_touch2;
            *pp_phy_obj_touching2_2 = p_wall2;
        }
        else if((pos1 + speed1) < width1)
        {
            speed1 = width1 - pos1;
            speed2 = (width1 + width1) - (pos2 - width2);

            p_phys_obj1->flags |= flag_touch1;
            *pp_phy_obj_touching1_1 = p_wall1;
        }

        p_phys_obj2->flags |= flag_touch1;
        *pp_phy_obj_touching2_1 = p_phys_obj1;
        p_phys_obj1->flags |= flag_touch2;
        *pp_phy_obj_touching1_2 = p_phys_obj2;
    }
}

byte i, j, k;
int8 delta_speed_x2, delta_speed;
t_physics_box3d **pp_phys_obj, *p_phys_obj,**pp_phys_another_obj, *p_phys_another_obj;

void phys_box3d_step(void)
{
    ++predivisor_friction;

    // step de la aceletacion de la gravedad
    for(i = 0, pp_phys_obj = pp_phys_box3d_objects_table; i < n_phys_box3d_objects; i++, pp_phys_obj++)
    {
        p_phys_obj = *pp_phys_obj;
        if(!(p_phys_obj->enabled)) continue; // tiene el objeto deshabilitado su modelo fisico ?

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

    // for(k = 0; k < 1; k++)   // Cuando el jugador empuja 2 objetos en cadena y chocan con una pared, con 2 iteraciones no "rebotan"
    {
        for(i = 0, pp_phys_obj = pp_phys_box3d_objects_table; i < n_phys_box3d_objects - 1; i++, pp_phys_obj++)
        {
            p_phys_obj = *pp_phys_obj;
            if(!(p_phys_obj->enabled)) continue; // tiene el objeto deshabilitado su modelo fisico ?

            j = i + 1; // bug del compilador ??
            for(pp_phys_another_obj = pp_phys_obj + 1; j < n_phys_box3d_objects; j++, pp_phys_another_obj++)
            {
                p_phys_another_obj = *pp_phys_another_obj;
                if(!(p_phys_another_obj->enabled)) continue; // tiene el objeto deshabilitado su modelo fisico ?

                if(!(((p_phys_obj->box3d.pos_y + p_phys_obj->box3d.width_y) <= (p_phys_another_obj->box3d.pos_y - p_phys_another_obj->box3d.width_y)) || ((p_phys_obj->box3d.pos_y - p_phys_obj->box3d.width_y) >= (p_phys_another_obj->box3d.pos_y + p_phys_another_obj->box3d.width_y)) ||
                        ((p_phys_obj->box3d.pos_z + p_phys_obj->box3d.height) <= (p_phys_another_obj->box3d.pos_z - p_phys_another_obj->box3d.height)) || ((p_phys_obj->box3d.pos_z - p_phys_obj->box3d.height) >= (p_phys_another_obj->box3d.pos_z + p_phys_another_obj->box3d.height))))
                {
                    if(/*((p_phys_obj->speed_x - p_phys_another_obj->speed_x) < 0) &&*/ (p_phys_obj->box3d.pos_x > p_phys_another_obj->box3d.pos_x))
                    {
                        p_phys_obj1 = p_phys_another_obj;
                        p_phys_obj2 = p_phys_obj;
                    }
                    else
                    {
                        p_phys_obj1 = p_phys_obj;
                        p_phys_obj2 = p_phys_another_obj;
                    }

                    isometric_max_axis = isometric_max_x_3d;
                    flag_touch1 = PHYS_BOX3D_FLAG_TOUCH_E;
                    flag_touch2 = PHYS_BOX3D_FLAG_TOUCH_W;
                    p_wall1 = &phys_box3d_wall_e;
                    p_wall2 = &phys_box3d_wall_w;
                    pos1 = p_phys_obj1->box3d.pos_x;
                    pos2 = p_phys_obj2->box3d.pos_x;
                    width1 = p_phys_obj1->box3d.width_x;
                    width2 = p_phys_obj2->box3d.width_x;
                    speed1 = p_phys_obj1->speed_x;
                    speed2 = p_phys_obj2->speed_x;
                    pp_phy_obj_touching1_1 = &(p_phys_obj1->p_phys_obj_touching_e);
                    pp_phy_obj_touching1_2 = &(p_phys_obj1->p_phys_obj_touching_w);
                    pp_phy_obj_touching2_1 = &(p_phys_obj2->p_phys_obj_touching_e);
                    pp_phy_obj_touching2_2 = &(p_phys_obj2->p_phys_obj_touching_w);

                    phys_box3d_step_subfunction();

                    p_phys_obj1->speed_x = speed1;
                    p_phys_obj2->speed_x = speed2;
                }

                if(!(((p_phys_obj->box3d.pos_x + p_phys_obj->box3d.width_x) <= (p_phys_another_obj->box3d.pos_x - p_phys_another_obj->box3d.width_x)) || ((p_phys_obj->box3d.pos_x - p_phys_obj->box3d.width_x) >= (p_phys_another_obj->box3d.pos_x + p_phys_another_obj->box3d.width_x)) ||
                        ((p_phys_obj->box3d.pos_z + p_phys_obj->box3d.height) <= (p_phys_another_obj->box3d.pos_z - p_phys_another_obj->box3d.height)) || ((p_phys_obj->box3d.pos_z - p_phys_obj->box3d.height) >= (p_phys_another_obj->box3d.pos_z + p_phys_another_obj->box3d.height))))
                {
                    if(/*((p_phys_obj->speed_y - p_phys_another_obj->speed_y) < 0) &&*/ (p_phys_obj->box3d.pos_y > p_phys_another_obj->box3d.pos_y))
                    {
                        p_phys_obj1 = p_phys_another_obj;
                        p_phys_obj2 = p_phys_obj;
                    }
                    else
                    {
                        p_phys_obj1 = p_phys_obj;
                        p_phys_obj2 = p_phys_another_obj;
                    }

                    isometric_max_axis = isometric_max_y_3d;
                    flag_touch1 = PHYS_BOX3D_FLAG_TOUCH_N;
                    flag_touch2 = PHYS_BOX3D_FLAG_TOUCH_S;
                    p_wall1 = &phys_box3d_wall_n;
                    p_wall2 = &phys_box3d_wall_s;
                    pos1 = p_phys_obj1->box3d.pos_y;
                    pos2 = p_phys_obj2->box3d.pos_y;
                    width1 = p_phys_obj1->box3d.width_y;
                    width2 = p_phys_obj2->box3d.width_y;
                    speed1 = p_phys_obj1->speed_y;
                    speed2 = p_phys_obj2->speed_y;
                    pp_phy_obj_touching1_1 = &(p_phys_obj1->p_phys_obj_touching_n);
                    pp_phy_obj_touching1_2 = &(p_phys_obj1->p_phys_obj_touching_s);
                    pp_phy_obj_touching2_1 = &(p_phys_obj2->p_phys_obj_touching_n);
                    pp_phy_obj_touching2_2 = &(p_phys_obj2->p_phys_obj_touching_s);

                    phys_box3d_step_subfunction();

                    p_phys_obj1->speed_y = speed1;
                    p_phys_obj2->speed_y = speed2;
                }

                if(!(((p_phys_obj->box3d.pos_x + p_phys_obj->box3d.width_x) <= (p_phys_another_obj->box3d.pos_x - p_phys_another_obj->box3d.width_x)) || ((p_phys_obj->box3d.pos_x - p_phys_obj->box3d.width_x) >= (p_phys_another_obj->box3d.pos_x + p_phys_another_obj->box3d.width_x)) ||
                        ((p_phys_obj->box3d.pos_y + p_phys_obj->box3d.width_y) <= (p_phys_another_obj->box3d.pos_y - p_phys_another_obj->box3d.width_y)) || ((p_phys_obj->box3d.pos_y - p_phys_obj->box3d.width_y) >= (p_phys_another_obj->box3d.pos_y + p_phys_another_obj->box3d.width_y))))
                {
                    if(/*((p_phys_obj->speed_z - p_phys_another_obj->speed_z) < 0) &&*/ (p_phys_obj->box3d.pos_z > p_phys_another_obj->box3d.pos_z))
                    {
                        p_phys_obj1 = p_phys_another_obj;
                        p_phys_obj2 = p_phys_obj;
                    }
                    else
                    {
                        p_phys_obj1 = p_phys_obj;
                        p_phys_obj2 = p_phys_another_obj;
                    }

                    isometric_max_axis = isometric_max_z_3d;
                    flag_touch1 = PHYS_BOX3D_FLAG_TOUCH_D;
                    flag_touch2 = PHYS_BOX3D_FLAG_TOUCH_U;
                    p_wall1 = &phys_box3d_floor;
                    p_wall2 = &phys_box3d_ceiling;
                    pos1 = p_phys_obj1->box3d.pos_z;
                    pos2 = p_phys_obj2->box3d.pos_z;
                    width1 = p_phys_obj1->box3d.height;
                    width2 = p_phys_obj2->box3d.height;
                    speed1 = p_phys_obj1->speed_z;
                    speed2 = p_phys_obj2->speed_z;
                    pp_phy_obj_touching1_1 = &(p_phys_obj1->p_phys_obj_touching_d);
                    pp_phy_obj_touching1_2 = &(p_phys_obj1->p_phys_obj_touching_u);
                    pp_phy_obj_touching2_1 = &(p_phys_obj2->p_phys_obj_touching_d);
                    pp_phy_obj_touching2_2 = &(p_phys_obj2->p_phys_obj_touching_u);

                    phys_box3d_step_subfunction();

                    p_phys_obj1->speed_z = speed1;
                    p_phys_obj2->speed_z = speed2;
                }
            }
        }
    }

    for(i = 0, pp_phys_obj = pp_phys_box3d_objects_table; i < n_phys_box3d_objects; i++, pp_phys_obj++)
    {
        p_phys_obj = *pp_phys_obj;
        if(!(p_phys_obj->enabled)) continue; // tiene el objeto deshabilitado su modelo fisico ?

        coord1 = p_phys_obj->box3d.pos_x;
        p_phys_obj->last_pos_x = coord1;
        coord1 += p_phys_obj->speed_x;
        p_phys_obj->box3d.pos_x = coord1;

        coord1 = p_phys_obj->box3d.pos_y;
        p_phys_obj->last_pos_y = coord1;
        coord1 += p_phys_obj->speed_y;
        p_phys_obj->box3d.pos_y = coord1;

        coord1 = p_phys_obj->box3d.pos_z;
        p_phys_obj->last_pos_z = coord1;
        coord1 += p_phys_obj->speed_z;
        p_phys_obj->box3d.pos_z = coord1;

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
#endif