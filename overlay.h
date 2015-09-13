/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef overlay_h
#define overlay_h

#include "image_data.h"

#include <vector>

#define GLEW_STATIC
#include <glew.h>

class Overlay{
    private:
    public:

    Overlay(double get_width,double get_height,double get_parallax_speed_x,double get_parallax_speed_y,std::vector<image_data>* get_layers);

    void update(double camera_delta_x,double camera_delta_y);

    void render(short layer_number,double opacity=1.0);

    //Every layer uses two identical images.

    //The x and y positions of the first layer image.
    double position_x_a;
    double position_y_a;

    //The x and y positions of the second layer image.
    double position_x_b;
    double position_y_b;

    //The dimensions of layer images.
    double width;
    double height;

    //The rate at which the background scrolls.
    //Lower numbers scroll the background faster.
    double parallax_speed_x,parallax_speed_y;

    std::vector<image_data>* layers;
};

#endif
