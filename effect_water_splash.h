/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef effect_water_splash_h
#define effect_water_splash_h

#include "sprite_sheets.h"

#include <SDL.h>

class Effect_Water_Splash{
    private:
    //Current frame of animation.
    short frame;
    short frame_counter;

    public:
    //The constructor takes starting coordinates.
    Effect_Water_Splash(double get_x,double get_y);

    //Animate the sprite.
    void animate();

    //Render the sprite.
    void render(bool mirrored=false);

    //The current coordinates of the player.
    double x,y;

    //The dimensions of the player. This is used for collision detection.
    double w,h;

    //
    bool exists;

    //
    bool dissipate;
};

#endif
