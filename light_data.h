/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef light_data_h
#define light_data_h

struct color_data{
    double red;
    double green;
    double blue;
};

struct Light_Data{
    bool seen;
    color_data color;
    double light_intensity;
    double dimness;
};

struct Light_Source{
    bool on;
    int x;
    int y;
    color_data color;
    unsigned int radius;
    double dimness;
    double falloff;
};

#endif
