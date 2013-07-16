/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "coords.h"

using namespace std;

Coords::Coords(){
    x=-1;
    y=-1;
}

Coords::Coords(int get_x,int get_y){
    x=get_x;
    y=get_y;
}
