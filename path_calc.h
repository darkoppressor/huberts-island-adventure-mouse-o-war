/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef path_calc_h
#define path_calc_h

#include "coords.h"

class Path_Calc{
public:

    Coords coords;

    Coords parent;

    int cost_g;
    int cost_f;

    Path_Calc(int get_coords_x,int get_coords_y,int get_parent_x,int get_parent_y,int get_cost_g,int get_cost_f);
};

#endif
