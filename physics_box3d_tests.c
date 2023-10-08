
#include "physics_box3d_tests.h"

byte isometric_max_x_3d = ISOMETRIC_MAX_X_3D_DEFFAULT;
byte isometric_max_y_3d = ISOMETRIC_MAX_Y_3D_DEFFAULT;
byte isometric_max_z_3d = ISOMETRIC_MAX_Z_3D_DEFFAULT;

t_physics_box3d *phys_box3d_objects_table[N_MAX_PHYS_BOX3D_OBJECTS];
byte n_phys_box3d_objects = 0;

byte predivisor_gravity = 0;

void phys_box3d_add_object(t_physics_box3d *p_phys_obj)
{
    if(n_phys_box3d_objects >= N_MAX_PHYS_BOX3D_OBJECTS) return;

    // Añade a la tabla de objetos fisicos
    phys_box3d_objects_table[n_phys_box3d_objects++] = p_phys_obj;
}

void phys_box3d_step(void)
{
    byte i, j;
    t_physics_box3d **pp_phys_obj, *p_phys_obj,**pp_phys_another_obj, *p_phys_another_obj;

    // step de la aceletacion de la gravedad
    ++predivisor_gravity;

    for(i = 0, pp_phys_obj = phys_box3d_objects_table; i < n_phys_box3d_objects; i++, pp_phys_obj++)
    {
        p_phys_obj = *pp_phys_obj;

        // Amplica gravedad
        if(((predivisor_gravity) % 7) == 0)
        {
            p_phys_obj->speed_z -= 1;
        }

        if((int8)(p_phys_obj->box3d.pos_z - p_phys_obj->box3d.height) < -p_phys_obj->speed_z )
        {
            p_phys_obj->speed_z = -(p_phys_obj->box3d.pos_z - p_phys_obj->box3d.height);
        }
    }

    for(i = 0, pp_phys_obj = phys_box3d_objects_table; i < n_phys_box3d_objects; i++, pp_phys_obj++)
    {
        p_phys_obj = *pp_phys_obj;

        for(j = 0, pp_phys_another_obj = phys_box3d_objects_table; j < n_phys_box3d_objects; j++, pp_phys_another_obj++)
        {
            if(i == j) continue;

            p_phys_another_obj = *pp_phys_another_obj;

            if(!(((p_phys_obj->box3d.pos_y + p_phys_obj->box3d.width_y) <= (p_phys_another_obj->box3d.pos_y - p_phys_another_obj->box3d.width_y)) || ((p_phys_obj->box3d.pos_y - p_phys_obj->box3d.width_y) >= (p_phys_another_obj->box3d.pos_y + p_phys_another_obj->box3d.width_y)) ||
                    ((p_phys_obj->box3d.pos_z + p_phys_obj->box3d.height) <= (p_phys_another_obj->box3d.pos_z - p_phys_another_obj->box3d.height)) || ((p_phys_obj->box3d.pos_z - p_phys_obj->box3d.height) >= (p_phys_another_obj->box3d.pos_z + p_phys_another_obj->box3d.height))))
            {
                if((p_phys_obj->speed_x < 0) && (p_phys_obj->box3d.pos_x > p_phys_another_obj->box3d.pos_x))
                {
                    if((p_phys_obj->box3d.pos_x - p_phys_obj->box3d.width_x + p_phys_obj->speed_x) < (p_phys_another_obj->box3d.pos_x + p_phys_another_obj->box3d.width_x + p_phys_another_obj->speed_x))
                    {
                        p_phys_obj->speed_x++;
                        p_phys_obj->box3d.pos_x = (p_phys_another_obj->box3d.pos_x + p_phys_another_obj->box3d.width_x + p_phys_another_obj->speed_x) + p_phys_obj->box3d.width_x - p_phys_obj->speed_x;
                        p_phys_another_obj->speed_x--;
                    }
                }
                else if((p_phys_obj->speed_x > 0) && (p_phys_obj->box3d.pos_x < p_phys_another_obj->box3d.pos_x))
                {
                    if((p_phys_obj->box3d.pos_x + p_phys_obj->box3d.width_x + p_phys_obj->speed_x) > (p_phys_another_obj->box3d.pos_x - p_phys_another_obj->box3d.width_x + p_phys_another_obj->speed_x))
                    {
                        p_phys_obj->speed_x--;
                        p_phys_obj->box3d.pos_x = (p_phys_another_obj->box3d.pos_x - p_phys_another_obj->box3d.width_x + p_phys_another_obj->speed_x) - p_phys_obj->box3d.width_x - p_phys_obj->speed_x;
                        p_phys_another_obj->speed_x++;
                    }
                }
            }

            if(!(((p_phys_obj->box3d.pos_x + p_phys_obj->box3d.width_x) <= (p_phys_another_obj->box3d.pos_x - p_phys_another_obj->box3d.width_x)) || ((p_phys_obj->box3d.pos_x - p_phys_obj->box3d.width_x) >= (p_phys_another_obj->box3d.pos_x + p_phys_another_obj->box3d.width_x)) ||
                    ((p_phys_obj->box3d.pos_z + p_phys_obj->box3d.height) <= (p_phys_another_obj->box3d.pos_z - p_phys_another_obj->box3d.height)) || ((p_phys_obj->box3d.pos_z - p_phys_obj->box3d.height) >= (p_phys_another_obj->box3d.pos_z + p_phys_another_obj->box3d.height))))
            {
                if((p_phys_obj->speed_y < 0) && (p_phys_obj->box3d.pos_y > p_phys_another_obj->box3d.pos_y))
                {
                    if((p_phys_obj->box3d.pos_y - p_phys_obj->box3d.width_y + p_phys_obj->speed_y) < (p_phys_another_obj->box3d.pos_y + p_phys_another_obj->box3d.width_y + p_phys_another_obj->speed_y))
                    {
                        p_phys_obj->speed_y++;
                        p_phys_obj->box3d.pos_y = (p_phys_another_obj->box3d.pos_y + p_phys_another_obj->box3d.width_y + p_phys_another_obj->speed_y) + p_phys_obj->box3d.width_y - p_phys_obj->speed_y;
                        p_phys_another_obj->speed_y--;
                    }
                }
                else if((p_phys_obj->speed_y > 0) && (p_phys_obj->box3d.pos_y < p_phys_another_obj->box3d.pos_y))
                {
                    if((p_phys_obj->box3d.pos_y + p_phys_obj->box3d.width_y + p_phys_obj->speed_y) > (p_phys_another_obj->box3d.pos_y - p_phys_another_obj->box3d.width_y + p_phys_another_obj->speed_y))
                    {
                        p_phys_obj->speed_y--;
                        p_phys_obj->box3d.pos_y = (p_phys_another_obj->box3d.pos_y - p_phys_another_obj->box3d.width_y + p_phys_another_obj->speed_y) - p_phys_obj->box3d.width_y - p_phys_obj->speed_y;
                        p_phys_another_obj->speed_y++;
                    }
                }
            }

            if(!(((p_phys_obj->box3d.pos_x + p_phys_obj->box3d.width_x) <= (p_phys_another_obj->box3d.pos_x - p_phys_another_obj->box3d.width_x)) || ((p_phys_obj->box3d.pos_x - p_phys_obj->box3d.width_x) >= (p_phys_another_obj->box3d.pos_x + p_phys_another_obj->box3d.width_x)) ||
                    ((p_phys_obj->box3d.pos_y + p_phys_obj->box3d.width_y) <= (p_phys_another_obj->box3d.pos_y - p_phys_another_obj->box3d.width_y)) || ((p_phys_obj->box3d.pos_y - p_phys_obj->box3d.width_y) >= (p_phys_another_obj->box3d.pos_y + p_phys_another_obj->box3d.width_y))))
            {
                if((p_phys_obj->speed_z < 0) && (p_phys_obj->box3d.pos_z > p_phys_another_obj->box3d.pos_z))
                {
                    if((p_phys_obj->box3d.pos_z - p_phys_obj->box3d.height + p_phys_obj->speed_z) < (p_phys_another_obj->box3d.pos_z + p_phys_another_obj->box3d.height + p_phys_another_obj->speed_z))
                    {
                        p_phys_obj->speed_z++;
                        p_phys_obj->box3d.pos_z = (p_phys_another_obj->box3d.pos_z + p_phys_another_obj->box3d.height + p_phys_another_obj->speed_z) + p_phys_obj->box3d.height - p_phys_obj->speed_z;
                        p_phys_another_obj->speed_z--;
                    }
                }
                else if((p_phys_obj->speed_z > 0) && (p_phys_obj->box3d.pos_z < p_phys_another_obj->box3d.pos_z))
                {
                    if((p_phys_obj->box3d.pos_z + p_phys_obj->box3d.height + p_phys_obj->speed_z) > (p_phys_another_obj->box3d.pos_z - p_phys_another_obj->box3d.height + p_phys_another_obj->speed_z))
                    {
                        p_phys_obj->speed_z--;
                        p_phys_obj->box3d.pos_z = (p_phys_another_obj->box3d.pos_z - p_phys_another_obj->box3d.height + p_phys_another_obj->speed_z) - p_phys_obj->box3d.height - p_phys_obj->speed_z;
                        p_phys_another_obj->speed_z++;
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
        }

        if(p_phys_obj->box3d.pos_x > (isometric_max_x_3d - p_phys_obj->box3d.width_x))
        {
            p_phys_obj->box3d.pos_x = isometric_max_x_3d - p_phys_obj->box3d.width_x;
            if(p_phys_obj->speed_x > 0) 
            {
                p_phys_obj->speed_x = 0;
            }
        }

        if(p_phys_obj->box3d.pos_y < p_phys_obj->box3d.width_y)
        {
            p_phys_obj->box3d.pos_y = p_phys_obj->box3d.width_y;
            if(p_phys_obj->speed_y < 0) 
            {
                p_phys_obj->speed_y = 0;
            }
        }

        if(p_phys_obj->box3d.pos_y > (isometric_max_y_3d - p_phys_obj->box3d.width_y))
        {
            p_phys_obj->box3d.pos_y = isometric_max_y_3d - p_phys_obj->box3d.width_y;
            if(p_phys_obj->speed_y > 0) 
            {
                p_phys_obj->speed_y = 0;
            }
        }

        if(p_phys_obj->box3d.pos_z < p_phys_obj->box3d.height)
        {
            p_phys_obj->box3d.pos_z = p_phys_obj->box3d.height;
            if(p_phys_obj->speed_z < 0) 
            {
                p_phys_obj->speed_z = 0;
            }
        }

        if(p_phys_obj->box3d.pos_z > (isometric_max_z_3d - p_phys_obj->box3d.height))
        {
            p_phys_obj->box3d.pos_z = isometric_max_z_3d - p_phys_obj->box3d.height;
            if(p_phys_obj->speed_z > 0) 
            {
                p_phys_obj->speed_z = 0;
            }
        }
    }

}