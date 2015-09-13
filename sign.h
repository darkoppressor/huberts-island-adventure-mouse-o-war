/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef sign_h
#define sign_h

#include <string>

class Sign{
    private:
    public:
    Sign(double get_x,double get_y,std::string get_message,short get_type,short get_font_type);

    void render(bool mirrored=false);

    std::string message;

    double x,y;

    short type;

    short font_type;
};

#endif
