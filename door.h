/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef door_h
#define door_h

#include "light_data.h"

class Door{
    private:
    public:
    Door(double get_x,double get_y,short get_type,short get_number,bool get_open,short get_identifier);

    //Animate the sprite.
    void animate();

    //Render the sprite.
    void render(bool mirrored=false);

    //Toggles the open state of the door.
    void toggle_open();

    //Attempts to unlock a key door with a key from the player's inventory.
    //Returns true if the door was unlocked.
    //Returns false if the door was not unlocked.
    bool attempt_unlock();

    //The type of door.
    short type;

    //If type is 0 (i.e. this door is a local level door), this is the door number.
    //The door number determines what door sprite will be used for this door.
    short number;

    //If true, the door is open.
    //If false, the door is closed.
    bool open;

    short identifier;

    //Used to determine if the animation frame is currently rising or falling.
    //True = frame is rising.
    //False = frame is falling.
    bool frame_direction;

    //The current coordinates of the platform.
    double x,y;

    //The dimensions of the platform.
    double w,h;

    //Animation frame and frame counter.
    short frame,frame_counter;

    //The object's light source data.
    Light_Source light_source;

    //If true, dimness is rising.
    //If false, dimness is falling.
    bool glow_direction;
};

#endif
