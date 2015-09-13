/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef effect_score_floater_h
#define effect_score_floater_h

#include <stdint.h>

class Effect_Score_Floater{
    private:
    double move_speed;

    public:
    Effect_Score_Floater(uint64_t get_score,double get_x,double get_y);

    void move();

    void render(bool mirrored=false);

    double x,y;

    bool exists;

    uint64_t score;

    int life;

    //Is the score floater moving to the left?
    bool moving_left;
    //How many pixels the score floater has moved since the last direction change.
    short pixels_moved;
};

#endif
