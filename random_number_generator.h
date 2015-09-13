/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef random_number_generator_h
#define random_number_generator_h

#include "MersenneTwister.h"

#include <stdint.h>

class Random_Number_Generator{
    private:
    public:
    MTRand mrand_main;

    MTRand mrand_render;
};

uint32_t random_range(int lownum,int highnum);

uint32_t random_range_render(int lownum,int highnum);

uint32_t random_range_pass(MTRand* the_rng,int lownum,int highnum);

#endif
