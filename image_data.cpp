/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "image_data.h"
#include "render.h"
#include "world.h"

using namespace std;

image_data::image_data(){
    texture=0;
    w=0;
    h=0;
}

void image_data::load_image(string filename){
    texture=load_texture(filename,this);

    SDL_SetTextureBlendMode(texture,SDL_BLENDMODE_BLEND);
}

void image_data::unload_image(){
    SDL_DestroyTexture(texture);

    texture=0;
    w=0.0;
    h=0.0;
}
