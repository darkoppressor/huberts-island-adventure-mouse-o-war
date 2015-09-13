/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "overlay.h"
#include "world.h"
#include "render.h"
#include "pixels.h"

using namespace std;

Overlay::Overlay(double get_width,double get_height,double get_parallax_speed_x,double get_parallax_speed_y,std::vector<image_data>* get_layers){
    width=get_width;
    height=get_height;

    parallax_speed_x=get_parallax_speed_x;
    parallax_speed_y=get_parallax_speed_y;

    layers=get_layers;

    position_x_a=0;
    position_x_b=0;
    position_y_a=0;
    position_y_b=0;
}

void Overlay::update(double camera_delta_x,double camera_delta_y){
    //Update the x position.

    position_x_a-=camera_delta_x/parallax_speed_x;

    if(position_x_a+width<0){
        position_x_a=0;
    }
    else if(position_x_a>0){
        position_x_a=-width;
    }

    position_x_b=position_x_a+width;

    //Update the y position.

    position_y_a-=camera_delta_y/parallax_speed_y;

    if(position_y_a+height<0){
        position_y_a=0;
    }
    else if(position_y_a>0){
        position_y_a=-height;
    }

    position_y_b=position_y_a+height;
}

void Overlay::render(short layer_number,double opacity){
    render_texture((int)position_x_a,(int)position_y_a,layers->at(layer_number),opacity);
    render_texture((int)position_x_b,(int)position_y_a,layers->at(layer_number),opacity);
    render_texture((int)position_x_a,(int)position_y_b,layers->at(layer_number),opacity);
    render_texture((int)position_x_b,(int)position_y_b,layers->at(layer_number),opacity);
}
