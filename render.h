/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef render_h
#define render_h

#include "image_data.h"
#include "enumerations.h"
#include "light_data.h"
#include "rtt_data.h"

#include <string>

#include <SDL.h>
#include <SDL_image.h>

SDL_Surface* optimize_surface(SDL_Surface* surface);

SDL_Surface* load_image(std::string filename);

SDL_Texture* load_texture(std::string filename,image_data* id);

void render_rtt(double x,double y,Rtt_Data* rtt_source,double opacity=1.0,double scale_x=1.0,double scale_y=1.0,double angle=0.0,short color_name=COLOR_WHITE,bool flip_x=false,bool flip_y=false);

void render_texture(double x,double y,image_data image_source,double opacity=1.0,short color_name=COLOR_WHITE,double scale_x=1.0,double scale_y=1.0);

void render_sprite(double x,double y,image_data image_source,SDL_Rect* texture_clip=0,double opacity=1.0,double scale_x=1.0,double scale_y=1.0,double angle=0.0,short color_name=COLOR_WHITE,bool flip_x=false);

void render_rectangle(double x,double y,double w,double h,double opacity,short color_name);

void render_rectangle_empty(double x,double y,double w,double h,double opacity,short color_name,double line_width);

//Specifically for lighting stuff.
void render_rectangle(double x,double y,double w,double h,double opacity,color_data colordata);

void render_line(double x1,double y1,double x2,double y2,double opacity,short color_name);

//Converts a color name (defined in enumerations.h) to red, green, blue color data in double format.
color_data color_name_to_doubles(short color_number);

//Converts red, green, blue color data in 0-255 format to double format.
color_data color_shorts_to_doubles(short red,short green,short blue);

#endif
