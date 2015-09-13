/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "sound_data.h"

using namespace std;

sound_data::sound_data(){
    chunk=NULL;
}

void sound_data::load_sound(string filename){
    chunk=Mix_LoadWAV(filename.c_str());
}

void sound_data::unload_sound(){
    Mix_FreeChunk(chunk);
}
