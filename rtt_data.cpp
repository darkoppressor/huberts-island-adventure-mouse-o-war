/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "rtt_data.h"
#include "world.h"

using namespace std;

Rtt_Data::Rtt_Data(){
    texture=0;
    w=0.0;
    h=0.0;
}

void Rtt_Data::create_texture(double get_w,double get_h){
    w=get_w;
    h=get_h;

    texture=main_window.create_texture(SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,w,h);

    SDL_SetTextureBlendMode(texture,SDL_BLENDMODE_BLEND);
}

void Rtt_Data::unload_texture(){
    SDL_DestroyTexture(texture);

    texture=0;
    w=0.0;
    h=0.0;
}
