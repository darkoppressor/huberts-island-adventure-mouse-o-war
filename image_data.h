/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef image_data_h
#define image_data_h

#include <SDL.h>

#include <string>

class image_data{
public:

    SDL_Texture* texture;
    double w;
    double h;

    image_data();

    void load_image(std::string filename);

    void unload_image();
};

#endif
