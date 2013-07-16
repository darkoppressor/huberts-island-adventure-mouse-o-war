/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef mirror_h
#define mirror_h

const double MIRROR_DISTANCE_X=15.0;

const double MIRROR_DISTANCE_Y=1.0;

const bool MIRRORED=true;

//Returns true if the object with passed data is touching a mirror tile.
//Returns false if the object is not touching any mirror tile.
bool touching_mirror(double x,double y,double w,double h);

#endif
