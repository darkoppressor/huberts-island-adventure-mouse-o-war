/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "random_number_generator.h"
#include "world.h"

#include <iostream>

using namespace std;

//This function randomly selects a number from lownum to highnum and then returns it.
//Using this we can randomly generate numbers anywhere in the game whenever we need them.
uint32_t random_range(int lownum,int highnum){
	if(lownum > highnum)
	swap(lownum,highnum);
	int range = highnum - lownum + 1;
	return uint32_t(rng.mrand_main.randInt()%range)+lownum;
}

//Identical to random_range(), except uses a different mrand object, thus not affecting the random numbers used for everything else.
uint32_t random_range_render(int lownum,int highnum){
	if(lownum > highnum)
	swap(lownum,highnum);
	int range = highnum - lownum + 1;
	return (rng.mrand_render.randInt()%range)+lownum;
}

uint32_t random_range_pass(MTRand* the_rng,int lownum,int highnum){
	if(lownum > highnum)
	swap(lownum,highnum);
	int range = highnum - lownum + 1;
	return uint32_t(the_rng->randInt()%range)+lownum;
}
