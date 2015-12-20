/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef rtt_data_h
#define rtt_data_h

#include <SDL.h>

class Rtt_Data{
public:

    SDL_Texture* texture;
    double w;
    double h;

    Rtt_Data();

    void create_texture(double get_w,double get_h);
    void unload_texture();
};

#endif
