/* Copyright (c) Cheese and Bacon Games */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef pixels_h
#define pixels_h

#include <SDL.h>

Uint32 surface_get_pixel(SDL_Surface* surface, int x, int y);

void surface_put_pixel(SDL_Surface* surface, int x, int y, Uint32 pixel);

#endif
