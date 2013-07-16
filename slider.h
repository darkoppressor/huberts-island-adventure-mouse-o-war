/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef slider_h
#define slider_h

#include <string>

class Slider{
    private:

    //The coordinate location and dimensions.
    short x,y;
    short w,h;

    //The identifier of the thing to display.
    int display_identifier;

    bool achievement;

    //If true, this slider has already played its sound, if any.
    //If false, it has not.
    bool played_sound;

    //The message to display.
    std::string display_string;

    //This counter is used to determine when the slider should leave the screen.
    short counter;

    public:

    Slider(int get_display_identifier,bool get_achievement=true);

    void set_dimensions();

    //Moves the slider.
    //Returns false if the slider has reached the bottom and should be deleted.
    //Returns true otherwise.
    bool move();

    void render();
};

#endif
