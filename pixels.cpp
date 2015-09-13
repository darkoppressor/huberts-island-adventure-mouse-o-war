/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "pixels.h"

using namespace std;

Uint32 surface_get_pixel(SDL_Surface *surface,int x,int y){
    //Convert the pixels to 32 bit.
    Uint32 *pixels=(Uint32 *)surface->pixels;

    //Get the requested pixels.
    return pixels[(y*surface->w)+x];
}

void surface_put_pixel(SDL_Surface *surface,int x,int y,Uint32 pixel){
    //Convert the pixels to 32 bit.
    Uint32 *pixels=(Uint32 *)surface->pixels;

    //Set the pixels.
    pixels[(y*surface->w)+x]=pixel;
}
