/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "path_calc.h"

using namespace std;

Path_Calc::Path_Calc(int get_coords_x,int get_coords_y,int get_parent_x,int get_parent_y,int get_cost_g,int get_cost_f){
    coords.x=get_coords_x;
    coords.y=get_coords_y;

    parent.x=get_parent_x;
    parent.y=get_parent_y;

    cost_g=get_cost_g;
    cost_f=get_cost_f;
}
