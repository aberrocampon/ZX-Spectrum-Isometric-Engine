
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
    1,
    {
        PHYS_BOX3D_MAX_X_3D_DEFFAULT/2, PHYS_BOX3D_MAX_Y_3D_DEFFAULT/2, 0,
        PHYS_BOX3D_MAX_X_3D_DEFFAULT/2, PHYS_BOX3D_MAX_Y_3D_DEFFAULT/2, 0
    },
    0, PHYS_BOX3D_MAX_X_3D_DEFFAULT,
    0, PHYS_BOX3D_MAX_Y_3D_DEFFAULT,
    0, 0
};

t_physics_box3d phys_box3d_ceiling = 
{
    1,
    {
        PHYS_BOX3D_MAX_X_3D_DEFFAULT/2, PHYS_BOX3D_MAX_Y_3D_DEFFAULT/2, PHYS_BOX3D_MAX_Z_3D_DEFFAULT,
        PHYS_BOX3D_MAX_X_3D_DEFFAULT/2, PHYS_BOX3D_MAX_Y_3D_DEFFAULT/2, 0
    },
    0, PHYS_BOX3D_MAX_X_3D_DEFFAULT,
    0, PHYS_BOX3D_MAX_Y_3D_DEFFAULT,
    PHYS_BOX3D_MAX_Z_3D_DEFFAULT, PHYS_BOX3D_MAX_Z_3D_DEFFAULT
};

t_physics_box3d phys_box3d_wall_n = 
{
    1,
    {
        PHYS_BOX3D_MAX_X_3D_DEFFAULT/2, 0, PHYS_BOX3D_MAX_Z_3D_DEFFAULT/2,
        PHYS_BOX3D_MAX_X_3D_DEFFAULT, 0, PHYS_BOX3D_MAX_Z_3D_DEFFAULT
    },
    -PHYS_BOX3D_MAX_X_3D_DEFFAULT/2, PHYS_BOX3D_MAX_X_3D_DEFFAULT/2 + PHYS_BOX3D_MAX_X_3D_DEFFAULT,
    0, 0,
    -PHYS_BOX3D_MAX_Z_3D_DEFFAULT/2, PHYS_BOX3D_MAX_Z_3D_DEFFAULT/2 + PHYS_BOX3D_MAX_Z_3D_DEFFAULT
};

t_physics_box3d phys_box3d_wall_s = 
{
    1,
    {
        PHYS_BOX3D_MAX_X_3D_DEFFAULT/2, PHYS_BOX3D_MAX_Y_3D_DEFFAULT, PHYS_BOX3D_MAX_Z_3D_DEFFAULT/2,
        PHYS_BOX3D_MAX_X_3D_DEFFAULT/2, 0, PHYS_BOX3D_MAX_Z_3D_DEFFAULT/2
    },
    0, PHYS_BOX3D_MAX_X_3D_DEFFAULT,
    PHYS_BOX3D_MAX_Y_3D_DEFFAULT, PHYS_BOX3D_MAX_Y_3D_DEFFAULT,
    0, PHYS_BOX3D_MAX_Z_3D_DEFFAULT
};

t_physics_box3d phys_box3d_wall_e = 
{
    1,
    {
        0, PHYS_BOX3D_MAX_Y_3D_DEFFAULT/2, PHYS_BOX3D_MAX_Z_3D_DEFFAULT/2,
        0, PHYS_BOX3D_MAX_Y_3D_DEFFAULT/2, PHYS_BOX3D_MAX_Z_3D_DEFFAULT/2
    },
    0, 0,
    0, PHYS_BOX3D_MAX_Y_3D_DEFFAULT,
    0, PHYS_BOX3D_MAX_Z_3D_DEFFAULT
};

t_physics_box3d phys_box3d_wall_w = 
{
    1,
    {
        PHYS_BOX3D_MAX_X_3D_DEFFAULT, PHYS_BOX3D_MAX_Y_3D_DEFFAULT/2, PHYS_BOX3D_MAX_Z_3D_DEFFAULT/2,
        0, PHYS_BOX3D_MAX_Y_3D_DEFFAULT/2, PHYS_BOX3D_MAX_Z_3D_DEFFAULT/2
    },
    PHYS_BOX3D_MAX_X_3D_DEFFAULT, PHYS_BOX3D_MAX_X_3D_DEFFAULT,
    0, PHYS_BOX3D_MAX_Y_3D_DEFFAULT,
    0, PHYS_BOX3D_MAX_Z_3D_DEFFAULT
};

t_physics_box3d **pp_phys_box3d_objects_table;
byte n_phys_box3d_objects;

byte predivisor_friction = 0;

t_physics_box3d *p_phys_obj1, *p_phys_obj2;
byte isometric_max_axis;
byte flag_touch1, flag_touch2;
t_physics_box3d *p_wall1, *p_wall2;
byte pos1, pos2;
byte max_pos1, min_pos2;
byte width1, width2;
int8 speed1, speed2;
byte coord1, coord2;
int8 delta_speed_x2, delta_speed;
t_physics_box3d **pp_phy_obj_touching1_1, **pp_phy_obj_touching1_2, **pp_phy_obj_touching2_1, **pp_phy_obj_touching2_2;

void phys_box3d_step_subfunction(void)
{
    // coord2 = min_pos2 + speed2;
    // coord1 = max_pos1 + speed1;
    // if((coord2 < coord1) && ((coord1 - coord2) < PHYS_BOX3D_CONTACT_TOLERANCE) && (speed1 != speed2))
    // {
    //     if( (p_phys_obj2->flags & (PHYS_BOX3D_FLAG_CINEMATIC | flag_touch2)) && !(p_phys_obj1->flags & (PHYS_BOX3D_FLAG_CINEMATIC | flag_touch1)) )
    //     {
    //         speed1 = (coord2) - (max_pos1);
    //     }
    //     else if( !(p_phys_obj2->flags & (PHYS_BOX3D_FLAG_CINEMATIC | flag_touch2)) && (p_phys_obj1->flags & (PHYS_BOX3D_FLAG_CINEMATIC | flag_touch1)) )
    //     {
    //         speed2 = (coord1) - (min_pos2);
    //     }
    //     else if( !(p_phys_obj2->flags & (PHYS_BOX3D_FLAG_CINEMATIC | flag_touch2)) && !(p_phys_obj1->flags & (PHYS_BOX3D_FLAG_CINEMATIC | flag_touch1)) )
    //     {
    //         delta_speed_x2 = (coord1) - (coord2) + 1; // + 1 => para redondear al dividor por 2 !!. En caso de no hacerlo uno de los dos queda quieto si el dividendo es 1
    //         delta_speed = delta_speed_x2 >> 1;
    //         speed2 += delta_speed;
    //         speed1 -= delta_speed;
    //     }
    //     else
    //     {
    //         speed2 = speed1 = 0;
    //     }

    //     if( (pos2 + speed2) > (isometric_max_axis - width2) )
    //     {
    //         speed2 = isometric_max_axis - width2 - pos2;
    //         speed1 = (coord2) - (max_pos1);

    //         p_phys_obj2->flags |= flag_touch2;
    //         *pp_phy_obj_touching2_2 = p_wall2;
    //     }
    //     else if((pos1 + speed1) < width1)
    //     {
    //         speed1 = width1 - pos1;
    //         speed2 = (width1 + width1) - (min_pos2);

    //         p_phys_obj1->flags |= flag_touch1;
    //         *pp_phy_obj_touching1_1 = p_wall1;
    //     }

    //     p_phys_obj2->flags |= flag_touch1;
    //     *pp_phy_obj_touching2_1 = p_phys_obj1;
    //     p_phys_obj1->flags |= flag_touch2;
    //     *pp_phy_obj_touching1_2 = p_phys_obj2;
    // }
    #asm
        ld a, (_max_pos1)
        ld hl, _speed1
        add (hl)
        ld (_coord1), a
        ld a, (_min_pos2)
        ld hl, _speed2
        add (hl)
        ld (_coord2), a
        ld hl, _coord1
        cp (hl)
        jp nc, l_phys_box3d_step_subfunction_7
        neg
        add (hl)
        cp PHYS_BOX3D_CONTACT_TOLERANCE
        jp nc, l_phys_box3d_step_subfunction_7
        ld hl, _speed1
        ld a, (_speed2)
        cp (hl)
        jp z, l_phys_box3d_step_subfunction_7

        ld hl, (_p_phys_obj2)
        ld bc, T_PHYSICS_BOX3D_OFFSET_FLAGS
        add hl, bc
        ld a, (_flag_touch2)
        or PHYS_BOX3D_FLAG_CINEMATIC
        and (hl)
        jr z, l_phys_box3d_step_subfunction_1
        ld hl, (_p_phys_obj1)
        add hl, bc
        ld a, (_flag_touch1)
        or PHYS_BOX3D_FLAG_CINEMATIC
        and (hl)
        jr nz, l_phys_box3d_step_subfunction_3
        ld a, (_coord2)
        ld hl, _max_pos1
        sub (hl)
        ld (_speed1), a
        jr l_phys_box3d_step_subfunction_4
    l_phys_box3d_step_subfunction_1:
        ld hl, (_p_phys_obj1)
        add hl, bc
        ld a, (_flag_touch1)
        or PHYS_BOX3D_FLAG_CINEMATIC
        and (hl)
        jr z, l_phys_box3d_step_subfunction_2
        ld a, (_coord1)
        ld hl, _min_pos2
        sub (hl)
        ld (_speed2), a
        jr l_phys_box3d_step_subfunction_4
    l_phys_box3d_step_subfunction_2:
        ld a, (_coord1)
        ld hl, _coord2
        sub (hl)
        inc a
        rra
        ld d, a
        ld hl, _speed2
        add (hl)
        ld (hl), a
        dec hl ; direccion de _speed1 !!
        ld a, (hl)
        sub d
        ld (hl), a
        jr l_phys_box3d_step_subfunction_4
    l_phys_box3d_step_subfunction_3:
        xor a
        ld (_speed1), a
        ld (_speed2), a

    l_phys_box3d_step_subfunction_4:
        ld a, (_pos2)
        ld hl, _speed2
        add (hl)
        ld d, a
        ld a, (_isometric_max_axis)
        ld hl, _width2
        sub (hl)
        cp d
        jr nc, l_phys_box3d_step_subfunction_5
        ld a, (_isometric_max_axis)
        sub (hl)
        ld hl, _pos2
        sub (hl)
        ld (_speed2), a
        ld a, (_coord2)
        ld hl, _max_pos1
        sub (hl)
        ld (_speed1), a
        ld hl, (_p_phys_obj2)
        add hl, bc
        ld a, (_flag_touch2)
        or (hl)
        ld (hl), a
        ld hl, (_p_wall2)
        ex de, hl
        ld hl, (_pp_phy_obj_touching2_2)
        ld (hl), e
        inc hl
        ld (hl), d
        jr l_phys_box3d_step_subfunction_6
    l_phys_box3d_step_subfunction_5:
        ld a, (_pos1)
        ld hl, _speed1
        add (hl)
        ld hl, _width1
        cp (hl)
        jr nc, l_phys_box3d_step_subfunction_6
        ld a, (_width1)
        ld hl, _pos1
        sub (hl)
        ld (_speed1), a
        ld a, (_width1)
        add a
        ld hl, _min_pos2
        sub (hl)
        ld (_speed2), a
        ld hl, (_p_phys_obj1)
        add hl, bc
        ld a, (_flag_touch1)
        or (hl)
        ld (hl), a
        ld hl, (_p_wall1)
        ex de, hl
        ld hl, (_pp_phy_obj_touching1_1)
        ld (hl), e
        inc hl
        ld (hl), d
    l_phys_box3d_step_subfunction_6:
        ld hl, (_p_phys_obj2)
        add hl, bc
        ld a, (_flag_touch1)
        or (hl)
        ld (hl), a
        ld hl, (_p_phys_obj1)
        ex de, hl
        ld hl, (_pp_phy_obj_touching2_1)
        ld (hl), e
        inc hl
        ld (hl), d
        ld hl, (_p_phys_obj1)
        add hl, bc
        ld a, (_flag_touch2)
        or (hl)
        ld (hl), a
        ld hl, (_p_phys_obj2)
        ex de, hl
        ld hl, (_pp_phy_obj_touching1_2)
        ld (hl), e
        inc hl
        ld (hl), d
    l_phys_box3d_step_subfunction_7:
    #endasm
}

byte i, j, k;
int8 delta_speed_x2, delta_speed;
t_physics_box3d **pp_phys_obj, *p_phys_obj,**pp_phys_another_obj, *p_phys_another_obj;

void phys_box3d_step(void)
{
    // Precalculos en asm
    // for(i = 0, pp_phys_obj = pp_phys_box3d_objects_table; i < n_phys_box3d_objects; i++, pp_phys_obj++)
    // {
    //     p_phys_obj = *pp_phys_obj;
    //     if(!(p_phys_obj->enabled)) continue; // tiene el objeto deshabilitado su modelo fisico ?

    //     // Precalculos --> asm
    //     p_phys_obj->min_pos_x = p_phys_obj->box3d.pos_x - p_phys_obj->box3d.width_x;
    //     p_phys_obj->max_pos_x = p_phys_obj->box3d.pos_x + p_phys_obj->box3d.width_x;
    //     p_phys_obj->min_pos_y = p_phys_obj->box3d.pos_y - p_phys_obj->box3d.width_y;
    //     p_phys_obj->max_pos_y = p_phys_obj->box3d.pos_y + p_phys_obj->box3d.width_y;
    //     p_phys_obj->min_pos_z = p_phys_obj->box3d.pos_z - p_phys_obj->box3d.height;
    //     p_phys_obj->max_pos_z = p_phys_obj->box3d.pos_z + p_phys_obj->box3d.height;
    // }
    #asm
        ld a, (_n_phys_box3d_objects)
        ld b, a
        ld hl, (_pp_phys_box3d_objects_table)
    l_phys_box3d_step_precalculate_box3d_limits:
        ld e, (hl)
        inc hl
        ld d, (hl)
        inc hl
        ld a, (de)
        and a
        jr z, l_phys_box3d_step_precalculate_box3d_limits_not_enabled
        ld (_pp_phys_obj), hl
        ex de, hl
        inc hl

        ld de, -6
        
        ld a, (hl) ; pos_x, y, z
        inc hl
        inc hl
        inc hl
        ld c, (hl) ; width_x, y, height
        sub c
        inc hl
        inc hl
        inc hl
        ld (hl), a ; min_pos_x, y, z
        add c
        add c
        inc hl
        ld (hl), a ; max_pos_x, y, z
        add hl, de
        dec e

        ld a, (hl) ; pos_x, y, z
        inc hl
        inc hl
        inc hl
        ld c, (hl) ; width_x, y, height
        sub c
        inc hl
        inc hl
        inc hl
        inc hl
        ld (hl), a ; min_pos_x, y, z
        add c
        add c
        inc hl
        ld (hl), a ; max_pos_x, y, z
        add hl, de
        dec e

        ld a, (hl) ; pos_x, y, z
        inc hl
        inc hl
        inc hl
        ld c, (hl) ; width_x, y, height
        sub c
        inc hl
        inc hl
        inc hl
        inc hl
        inc hl
        ld (hl), a ; min_pos_x, y, z
        add c
        add c
        inc hl
        ld (hl), a ; max_pos_x, y, z

        ld hl, (_pp_phys_obj)
    l_phys_box3d_step_precalculate_box3d_limits_not_enabled:
        djnz l_phys_box3d_step_precalculate_box3d_limits
    #endasm

    ++predivisor_friction;

    for(i = 0, pp_phys_obj = pp_phys_box3d_objects_table; i < n_phys_box3d_objects; i++, pp_phys_obj++)
    {
        p_phys_obj = *pp_phys_obj;
        if(!(p_phys_obj->enabled)) continue; // tiene el objeto deshabilitado su modelo fisico ?

        // Aplica gravedad
        if( !(p_phys_obj->flags & PHYS_BOX3D_FLAG_CINEMATIC) && !(--p_phys_obj->gravity_count) )
        {
            p_phys_obj->speed_z -= 1;
            p_phys_obj->gravity_count = PHYS_BOX3D_GRAVITY_INTENSITY_INIT_COUNTER;
        }

        // reset touching flags
        p_phys_obj->flags &= (PHYS_BOX3D_MASK_TOUCH_N & PHYS_BOX3D_MASK_TOUCH_S & PHYS_BOX3D_MASK_TOUCH_E & PHYS_BOX3D_MASK_TOUCH_W & PHYS_BOX3D_MASK_TOUCH_U);
        if( !( (p_phys_obj->flags & PHYS_BOX3D_FLAG_TOUCH_D) && ((p_phys_obj->min_pos_z) == (p_phys_obj->p_phys_obj_touching_d->max_pos_z)) &&
            !(((p_phys_obj->max_pos_x) <= (p_phys_obj->p_phys_obj_touching_d->min_pos_x)) || ((p_phys_obj->min_pos_x) >= (p_phys_obj->p_phys_obj_touching_d->max_pos_x)) ||
                    ((p_phys_obj->max_pos_y) <= (p_phys_obj->p_phys_obj_touching_d->min_pos_y)) || ((p_phys_obj->min_pos_y) >= (p_phys_obj->p_phys_obj_touching_d->max_pos_y))) &&
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

                // if(!(((p_phys_obj->max_pos_y) <= (p_phys_another_obj->min_pos_y)) || ((p_phys_obj->min_pos_y) >= (p_phys_another_obj->max_pos_y)) ||
                //         ((p_phys_obj->max_pos_z) <= (p_phys_another_obj->min_pos_z)) || ((p_phys_obj->min_pos_z) >= (p_phys_another_obj->max_pos_z))))
                // {
                //     if(/*((p_phys_obj->speed_x - p_phys_another_obj->speed_x) < 0) &&*/ (p_phys_obj->box3d.pos_x > p_phys_another_obj->box3d.pos_x))
                //     {
                //         p_phys_obj1 = p_phys_another_obj;
                //         p_phys_obj2 = p_phys_obj;
                //     }
                //     else
                //     {
                //         p_phys_obj1 = p_phys_obj;
                //         p_phys_obj2 = p_phys_another_obj;
                //     }

                //     isometric_max_axis = isometric_max_x_3d;
                //     flag_touch1 = PHYS_BOX3D_FLAG_TOUCH_E;
                //     flag_touch2 = PHYS_BOX3D_FLAG_TOUCH_W;
                //     p_wall1 = &phys_box3d_wall_e;
                //     p_wall2 = &phys_box3d_wall_w;
                //     pos1 = p_phys_obj1->box3d.pos_x;
                //     pos2 = p_phys_obj2->box3d.pos_x;
                //     max_pos1 = p_phys_obj1->max_pos_x;
                //     min_pos2 = p_phys_obj2->min_pos_x;
                //     width1 = p_phys_obj1->box3d.width_x;
                //     width2 = p_phys_obj2->box3d.width_x;
                //     speed1 = p_phys_obj1->speed_x;
                //     speed2 = p_phys_obj2->speed_x;
                //     pp_phy_obj_touching1_1 = &(p_phys_obj1->p_phys_obj_touching_e);
                //     pp_phy_obj_touching1_2 = &(p_phys_obj1->p_phys_obj_touching_w);
                //     pp_phy_obj_touching2_1 = &(p_phys_obj2->p_phys_obj_touching_e);
                //     pp_phy_obj_touching2_2 = &(p_phys_obj2->p_phys_obj_touching_w);

                //     phys_box3d_step_subfunction();

                //     p_phys_obj1->speed_x = speed1;
                //     p_phys_obj2->speed_x = speed2;
                // }
                #asm
                    ld de, (_p_phys_obj)
                    ld hl, (_p_phys_another_obj)
                    ld bc, T_PHYSICS_BOX3D_OFFSET_MIN_POS_Y
                    add hl, bc
                    ld a, (hl)
                    ex de, hl
                    inc bc
                    add hl, bc
                    cp (hl)
                    jp nc, l_phys_box3d_step_process_colision_axis_x_end
                    dec hl
                    ld a, (hl)
                    ex de, hl
                    inc hl
                    cp (hl)
                    jp nc, l_phys_box3d_step_process_colision_axis_x_end
                    ld bc, T_PHYSICS_BOX3D_OFFSET_MIN_POS_Z - T_PHYSICS_BOX3D_OFFSET_MAX_POS_Y
                    add hl, bc
                    ld a, (hl)
                    ex de, hl
                    inc bc
                    inc bc
                    add hl, bc
                    cp (hl)
                    jp nc, l_phys_box3d_step_process_colision_axis_x_end
                    dec hl
                    ld a, (hl)
                    ex de, hl
                    inc hl
                    cp (hl)
                    jp nc, l_phys_box3d_step_process_colision_axis_x_end

                    ld bc, T_PHYSICS_BOX3D_OFFSET_POS_X - T_PHYSICS_BOX3D_OFFSET_MAX_POS_Z
                    add hl, bc
                    ld a, (hl)
                    ex de, hl
                    inc bc
                    add hl, bc
                    cp (hl)
                    jr c, l_phys_box3d_step_process_colision_axis_x_set_globals
                    ex de, hl
                
                l_phys_box3d_step_process_colision_axis_x_set_globals:
                    dec de
                    ld (_p_phys_obj1), de
                    dec hl
                    ld (_p_phys_obj2), hl

                    ld a, (_isometric_max_x_3d)
                    ld (_isometric_max_axis), a
                    ld a, PHYS_BOX3D_FLAG_TOUCH_E
                    ld (_flag_touch1), a
                    ld a, PHYS_BOX3D_FLAG_TOUCH_W
                    ld (_flag_touch2), a
                    inc de
                    inc hl
                    ld a, (de)
                    ld (_pos1), a
                    ld a, (hl)
                    ld (_pos2), a
                    inc de
                    inc de
                    inc de
                    ld a, (de)
                    ld (_width1), a
                    inc hl
                    inc hl
                    inc hl
                    ld a, (hl)
                    ld (_width2), a
                    ld bc, T_PHYSICS_BOX3D_OFFSET_SPEED_X - T_PHYSICS_BOX3D_OFFSET_WIDTH_X
                    add hl, bc
                    ld a, (hl)
                    ld (_speed2), a
                    ex de, hl
                    add hl, bc
                    ld a, (hl)
                    ld (_speed1), a
                    ld bc, T_PHYSICS_BOX3D_OFFSET_MAX_POS_X - T_PHYSICS_BOX3D_OFFSET_SPEED_X
                    add hl, bc
                    ld a, (hl)
                    ld (_max_pos1), a
                    ex de, hl
                    dec bc
                    add hl, bc
                    ld a, (hl)
                    ld (_min_pos2), a
                    ld bc, T_PHYSICS_BOX3D_OFFSET_P_PHYS_OBJ_TOUCH_E - T_PHYSICS_BOX3D_OFFSET_MIN_POS_X
                    add hl, bc
                    ld (_pp_phy_obj_touching2_1), hl
                    inc hl
                    inc hl
                    ld (_pp_phy_obj_touching2_2), hl
                    ex de, hl
                    dec bc
                    add hl, bc
                    ld (_pp_phy_obj_touching1_1), hl
                    inc hl
                    inc hl
                    ld (_pp_phy_obj_touching1_2), hl
                    ld hl, _phys_box3d_wall_e
                    ld (_p_wall1), hl
                    ld hl, _phys_box3d_wall_w
                    ld (_p_wall2), hl

                    call _phys_box3d_step_subfunction

                    ld hl, (_p_phys_obj1)
                    ld bc, T_PHYSICS_BOX3D_OFFSET_SPEED_X
                    add hl, bc
                    ld a, (_speed1)
                    ld (hl), a
                    ld hl, (_p_phys_obj2)
                    add hl, bc
                    ld a, (_speed2)
                    ld (hl), a

                l_phys_box3d_step_process_colision_axis_x_end:
                #endasm

                // if(!(((p_phys_obj->max_pos_x) <= (p_phys_another_obj->min_pos_x)) || ((p_phys_obj->min_pos_x) >= (p_phys_another_obj->max_pos_x)) ||
                //         ((p_phys_obj->max_pos_z) <= (p_phys_another_obj->min_pos_z)) || ((p_phys_obj->min_pos_z) >= (p_phys_another_obj->max_pos_z))))
                // {
                //     if(/*((p_phys_obj->speed_y - p_phys_another_obj->speed_y) < 0) &&*/ (p_phys_obj->box3d.pos_y > p_phys_another_obj->box3d.pos_y))
                //     {
                //         p_phys_obj1 = p_phys_another_obj;
                //         p_phys_obj2 = p_phys_obj;
                //     }
                //     else
                //     {
                //         p_phys_obj1 = p_phys_obj;
                //         p_phys_obj2 = p_phys_another_obj;
                //     }

                //     isometric_max_axis = isometric_max_y_3d;
                //     flag_touch1 = PHYS_BOX3D_FLAG_TOUCH_N;
                //     flag_touch2 = PHYS_BOX3D_FLAG_TOUCH_S;
                //     p_wall1 = &phys_box3d_wall_n;
                //     p_wall2 = &phys_box3d_wall_s;
                //     pos1 = p_phys_obj1->box3d.pos_y;
                //     pos2 = p_phys_obj2->box3d.pos_y;
                //     max_pos1 = p_phys_obj1->max_pos_y;
                //     min_pos2 = p_phys_obj2->min_pos_y;
                //     width1 = p_phys_obj1->box3d.width_y;
                //     width2 = p_phys_obj2->box3d.width_y;
                //     speed1 = p_phys_obj1->speed_y;
                //     speed2 = p_phys_obj2->speed_y;
                //     pp_phy_obj_touching1_1 = &(p_phys_obj1->p_phys_obj_touching_n);
                //     pp_phy_obj_touching1_2 = &(p_phys_obj1->p_phys_obj_touching_s);
                //     pp_phy_obj_touching2_1 = &(p_phys_obj2->p_phys_obj_touching_n);
                //     pp_phy_obj_touching2_2 = &(p_phys_obj2->p_phys_obj_touching_s);

                //     phys_box3d_step_subfunction();

                //     p_phys_obj1->speed_y = speed1;
                //     p_phys_obj2->speed_y = speed2;
                // }
                #asm
                    ld de, (_p_phys_obj)
                    ld hl, (_p_phys_another_obj)
                    ld bc, T_PHYSICS_BOX3D_OFFSET_MIN_POS_X
                    add hl, bc
                    ld a, (hl)
                    ex de, hl
                    inc bc
                    add hl, bc
                    cp (hl)
                    jp nc, l_phys_box3d_step_process_colision_axis_y_end
                    dec hl
                    ld a, (hl)
                    ex de, hl
                    inc hl
                    cp (hl)
                    jp nc, l_phys_box3d_step_process_colision_axis_y_end
                    ld bc, T_PHYSICS_BOX3D_OFFSET_MIN_POS_Z - T_PHYSICS_BOX3D_OFFSET_MAX_POS_X
                    add hl, bc
                    ld a, (hl)
                    ex de, hl
                    inc bc
                    inc bc
                    add hl, bc
                    cp (hl)
                    jp nc, l_phys_box3d_step_process_colision_axis_y_end
                    dec hl
                    ld a, (hl)
                    ex de, hl
                    inc hl
                    cp (hl)
                    jp nc, l_phys_box3d_step_process_colision_axis_y_end

                    ld bc, T_PHYSICS_BOX3D_OFFSET_POS_Y - T_PHYSICS_BOX3D_OFFSET_MAX_POS_Z
                    add hl, bc
                    ld a, (hl)
                    ex de, hl
                    inc bc
                    add hl, bc
                    cp (hl)
                    jr c, l_phys_box3d_step_process_colision_axis_y_set_globals
                    ex de, hl
                
                l_phys_box3d_step_process_colision_axis_y_set_globals:
                    dec de
                    dec de
                    ld (_p_phys_obj1), de
                    dec hl
                    dec hl
                    ld (_p_phys_obj2), hl

                    ld a, (_isometric_max_y_3d)
                    ld (_isometric_max_axis), a
                    ld a, PHYS_BOX3D_FLAG_TOUCH_N
                    ld (_flag_touch1), a
                    ld a, PHYS_BOX3D_FLAG_TOUCH_S
                    ld (_flag_touch2), a
                    inc de
                    inc de
                    inc hl
                    inc hl
                    ld a, (de)
                    ld (_pos1), a
                    ld a, (hl)
                    ld (_pos2), a
                    inc de
                    inc de
                    inc de
                    ld a, (de)
                    ld (_width1), a
                    inc hl
                    inc hl
                    inc hl
                    ld a, (hl)
                    ld (_width2), a
                    ld bc, T_PHYSICS_BOX3D_OFFSET_SPEED_Y - T_PHYSICS_BOX3D_OFFSET_WIDTH_Y
                    add hl, bc
                    ld a, (hl)
                    ld (_speed2), a
                    ex de, hl
                    add hl, bc
                    ld a, (hl)
                    ld (_speed1), a
                    ld bc, T_PHYSICS_BOX3D_OFFSET_MAX_POS_Y - T_PHYSICS_BOX3D_OFFSET_SPEED_Y
                    add hl, bc
                    ld a, (hl)
                    ld (_max_pos1), a
                    ex de, hl
                    dec bc
                    add hl, bc
                    ld a, (hl)
                    ld (_min_pos2), a
                    ld bc, T_PHYSICS_BOX3D_OFFSET_P_PHYS_OBJ_TOUCH_N - T_PHYSICS_BOX3D_OFFSET_MIN_POS_Y
                    add hl, bc
                    ld (_pp_phy_obj_touching2_1), hl
                    inc hl
                    inc hl
                    ld (_pp_phy_obj_touching2_2), hl
                    ex de, hl
                    dec bc
                    add hl, bc
                    ld (_pp_phy_obj_touching1_1), hl
                    inc hl
                    inc hl
                    ld (_pp_phy_obj_touching1_2), hl
                    ld hl, _phys_box3d_wall_n
                    ld (_p_wall1), hl
                    ld hl, _phys_box3d_wall_s
                    ld (_p_wall2), hl

                    call _phys_box3d_step_subfunction

                    ld hl, (_p_phys_obj1)
                    ld bc, T_PHYSICS_BOX3D_OFFSET_SPEED_Y
                    add hl, bc
                    ld a, (_speed1)
                    ld (hl), a
                    ld hl, (_p_phys_obj2)
                    add hl, bc
                    ld a, (_speed2)
                    ld (hl), a

                l_phys_box3d_step_process_colision_axis_y_end:
                #endasm

                // if(!(((p_phys_obj->max_pos_x) <= (p_phys_another_obj->min_pos_x)) || ((p_phys_obj->min_pos_x) >= (p_phys_another_obj->max_pos_x)) ||
                //         ((p_phys_obj->max_pos_y) <= (p_phys_another_obj->min_pos_y)) || ((p_phys_obj->min_pos_y) >= (p_phys_another_obj->max_pos_y))))
                // {
                //     if(/*((p_phys_obj->speed_z - p_phys_another_obj->speed_z) < 0) &&*/ (p_phys_obj->box3d.pos_z > p_phys_another_obj->box3d.pos_z))
                //     {
                //         p_phys_obj1 = p_phys_another_obj;
                //         p_phys_obj2 = p_phys_obj;
                //     }
                //     else
                //     {
                //         p_phys_obj1 = p_phys_obj;
                //         p_phys_obj2 = p_phys_another_obj;
                //     }

                //     isometric_max_axis = isometric_max_z_3d;
                //     flag_touch1 = PHYS_BOX3D_FLAG_TOUCH_D;
                //     flag_touch2 = PHYS_BOX3D_FLAG_TOUCH_U;
                //     p_wall1 = &phys_box3d_floor;
                //     p_wall2 = &phys_box3d_ceiling;
                //     pos1 = p_phys_obj1->box3d.pos_z;
                //     pos2 = p_phys_obj2->box3d.pos_z;
                //     max_pos1 = p_phys_obj1->max_pos_z;
                //     min_pos2 = p_phys_obj2->min_pos_z;
                //     width1 = p_phys_obj1->box3d.height;
                //     width2 = p_phys_obj2->box3d.height;
                //     speed1 = p_phys_obj1->speed_z;
                //     speed2 = p_phys_obj2->speed_z;
                //     pp_phy_obj_touching1_1 = &(p_phys_obj1->p_phys_obj_touching_d);
                //     pp_phy_obj_touching1_2 = &(p_phys_obj1->p_phys_obj_touching_u);
                //     pp_phy_obj_touching2_1 = &(p_phys_obj2->p_phys_obj_touching_d);
                //     pp_phy_obj_touching2_2 = &(p_phys_obj2->p_phys_obj_touching_u);

                //     phys_box3d_step_subfunction();

                //     p_phys_obj1->speed_z = speed1;
                //     p_phys_obj2->speed_z = speed2;
                // }
                #asm
                    ld de, (_p_phys_obj)
                    ld hl, (_p_phys_another_obj)
                    ld bc, T_PHYSICS_BOX3D_OFFSET_MIN_POS_X
                    add hl, bc
                    ld a, (hl)
                    ex de, hl
                    inc bc
                    add hl, bc
                    cp (hl)
                    jp nc, l_phys_box3d_step_process_colision_axis_z_end
                    dec hl
                    ld a, (hl)
                    ex de, hl
                    inc hl
                    cp (hl)
                    jp nc, l_phys_box3d_step_process_colision_axis_z_end
                    ld bc, T_PHYSICS_BOX3D_OFFSET_MIN_POS_Y - T_PHYSICS_BOX3D_OFFSET_MAX_POS_X
                    add hl, bc
                    ld a, (hl)
                    ex de, hl
                    inc bc
                    inc bc
                    add hl, bc
                    cp (hl)
                    jp nc, l_phys_box3d_step_process_colision_axis_z_end
                    dec hl
                    ld a, (hl)
                    ex de, hl
                    inc hl
                    cp (hl)
                    jp nc, l_phys_box3d_step_process_colision_axis_z_end

                    ld bc, T_PHYSICS_BOX3D_OFFSET_POS_Z - T_PHYSICS_BOX3D_OFFSET_MAX_POS_Y
                    add hl, bc
                    ld a, (hl)
                    ex de, hl
                    inc bc
                    add hl, bc
                    cp (hl)
                    jr c, l_phys_box3d_step_process_colision_axis_z_set_globals
                    ex de, hl
                
                l_phys_box3d_step_process_colision_axis_z_set_globals:
                    dec de
                    dec de
                    dec de
                    ld (_p_phys_obj1), de
                    dec hl
                    dec hl
                    dec hl
                    ld (_p_phys_obj2), hl

                    ld a, (_isometric_max_z_3d)
                    ld (_isometric_max_axis), a
                    ld a, PHYS_BOX3D_FLAG_TOUCH_D
                    ld (_flag_touch1), a
                    ld a, PHYS_BOX3D_FLAG_TOUCH_U
                    ld (_flag_touch2), a
                    inc de
                    inc de
                    inc de
                    inc hl
                    inc hl
                    inc hl
                    ld a, (de)
                    ld (_pos1), a
                    ld a, (hl)
                    ld (_pos2), a
                    inc de
                    inc de
                    inc de
                    ld a, (de)
                    ld (_width1), a
                    inc hl
                    inc hl
                    inc hl
                    ld a, (hl)
                    ld (_width2), a
                    ld bc, T_PHYSICS_BOX3D_OFFSET_SPEED_Z - T_PHYSICS_BOX3D_OFFSET_HEIGHT
                    add hl, bc
                    ld a, (hl)
                    ld (_speed2), a
                    ex de, hl
                    add hl, bc
                    ld a, (hl)
                    ld (_speed1), a
                    ld bc, T_PHYSICS_BOX3D_OFFSET_MAX_POS_Z - T_PHYSICS_BOX3D_OFFSET_SPEED_Z
                    add hl, bc
                    ld a, (hl)
                    ld (_max_pos1), a
                    ex de, hl
                    dec bc
                    add hl, bc
                    ld a, (hl)
                    ld (_min_pos2), a
                    ld bc, T_PHYSICS_BOX3D_OFFSET_P_PHYS_OBJ_TOUCH_D - T_PHYSICS_BOX3D_OFFSET_MIN_POS_Z
                    add hl, bc
                    ld (_pp_phy_obj_touching2_1), hl
                    inc hl
                    inc hl
                    ld (_pp_phy_obj_touching2_2), hl
                    ex de, hl
                    dec bc
                    add hl, bc
                    ld (_pp_phy_obj_touching1_1), hl
                    inc hl
                    inc hl
                    ld (_pp_phy_obj_touching1_2), hl
                    ld hl, _phys_box3d_floor
                    ld (_p_wall1), hl
                    ld hl, _phys_box3d_ceiling
                    ld (_p_wall2), hl

                    call _phys_box3d_step_subfunction

                    ld hl, (_p_phys_obj1)
                    ld bc, T_PHYSICS_BOX3D_OFFSET_SPEED_Z
                    add hl, bc
                    ld a, (_speed1)
                    ld (hl), a
                    ld hl, (_p_phys_obj2)
                    add hl, bc
                    ld a, (_speed2)
                    ld (hl), a

                l_phys_box3d_step_process_colision_axis_z_end:
                #endasm
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