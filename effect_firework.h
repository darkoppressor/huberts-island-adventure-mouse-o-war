/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef effect_firework_h
#define effect_firework_h

#include "light_data.h"

#include <vector>

class Effect_Firework_Particle{
    public:
    //If true, the particle is flying outward.
    //If false, the particle is now falling.
    bool exploding;

    color_data color;

    bool fade_to_black;

    double opacity;

    double move_vector_x;
    double move_vector_y;

    double x,y;

    bool exists;

    double decel;
    double fade;
    double color_fade;

    Effect_Firework_Particle(double get_x,double get_y,double get_vector_x,double get_vector_y,short get_color);

    void set_coords(double get_x,double get_y);

    void stop_exploding();

    void move();

    void render();
};

class Effect_Firework{
    private:
    public:
    bool exists;

    //If true, the firework is exploding.
    //If false, the firework is flying upwards.
    bool exploding;

    double x,y;

    double move_vector_x;
    double move_vector_y;

    color_data color;

    double decel;
    double light_fade;

    std::vector<Effect_Firework_Particle> particles;

    //The object's light source data.
    Light_Source light_source;

    Effect_Firework(double get_x,double get_y,int magnitude,short get_color,short shape,int thickness,double speed_x,double speed_y);

    void move();

    void render();
};

#endif
