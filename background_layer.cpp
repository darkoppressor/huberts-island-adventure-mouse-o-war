/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "background_layer.h"
#include "world.h"
#include "render.h"

using namespace std;

Background_Layer::Background_Layer(double get_start_y,double get_width,double get_height,double get_parallax_speed_x,double get_parallax_speed_y){
    start_y=get_start_y;

    position_x_a=0;
    position_x_b=0;
    position_y=0;

    width=get_width;
    height=get_height;

    parallax_speed_x=get_parallax_speed_x;
    parallax_speed_y=get_parallax_speed_y;
}

void Background_Layer::update(double camera_delta_x){
    position_x_a-=camera_delta_x/parallax_speed_x;

    if(position_x_a+width<0){
        position_x_a=0;
    }
    else if(position_x_a>0){
        position_x_a=-width;
    }

    position_x_b=position_x_a+width;

    //If the y parallax speed is non-zero.
    //A speed of 0 means to not scroll at all.
    if(parallax_speed_y!=0){
        position_y=((height/parallax_speed_y)/level.level_y*player.camera_y)-start_y/parallax_speed_y;
    }
}

void Background_Layer::render(short layer_number){
    render_texture((int)position_x_a,-(int)position_y,image.background_layer_images[layer_number]);
    render_texture((int)position_x_b,-(int)position_y,image.background_layer_images[layer_number]);
}
