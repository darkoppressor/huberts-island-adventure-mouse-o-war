/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef booster_h
#define booster_h

class Booster{
    private:
    public:
    Booster(double get_x,double get_y,double get_speed,short get_direction);

    void animate();

    void render(bool mirrored=false);

    double x,y;

    //The speed the booster sets/adds to the actor affected.
    double speed;

    short direction;

    short frame;
    short frame_counter;
};

#endif
