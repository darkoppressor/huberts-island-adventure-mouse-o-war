/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "image_data.h"
#include "render.h"
#include "world.h"

using namespace std;

image_data::image_data(){
    texture=0;
    surface=NULL;
    w=0;
    h=0;
}

void image_data::load_image(string filename){
    if(player.option_renderer==RENDERER_HARDWARE){
        texture=load_texture(filename,this);
    }
    else if(player.option_renderer==RENDERER_SOFTWARE){
        surface=load_image_sdl(filename);
        w=surface->w;
        h=surface->h;
    }
}

void image_data::unload_image(){
    if(player.option_renderer==RENDERER_HARDWARE){
        glDeleteTextures(1,&texture);
    }
    else if(player.option_renderer==RENDERER_SOFTWARE){
        SDL_FreeSurface(surface);
    }
}
