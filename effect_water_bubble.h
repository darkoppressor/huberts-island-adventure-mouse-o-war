/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef effect_water_bubble_h
#define effect_water_bubble_h

#include "sprite_sheets.h"

#include <SDL.h>

class Effect_Water_Bubble{
    private:
    //Current frame of animation.
    short frame;
    short frame_counter;

    double move_speed;

    public:
    //The constructor takes starting coordinates.
    Effect_Water_Bubble(double get_x,double get_y);

    void move();

    void handle_events();

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

    //Is the bubble moving to the left?
    bool moving_left;
    //How many pixels the bubble has moved since the last direction change.
    short pixels_moved;
    //
    short time_to_move;

    //Is the bubble popping?
    bool pop;
};

#endif
