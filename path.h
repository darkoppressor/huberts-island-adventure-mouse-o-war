/* Copyright (c) Cheese and Bacon Games */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef path_h
#define path_h

#include "coords.h"

class Path {
    public:
        Coords coords;

        Path (int get_coords_x, int get_coords_y);
};

#endif
