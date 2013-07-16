/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef render_h
#define render_h

#include "image_data.h"
#include "enumerations.h"
#include "light_data.h"

#include <string>

#include <SDL.h>
#include <SDL_image.h>

#define GLEW_STATIC
#include <glew.h>

SDL_Surface* optimize_image_opengl(SDL_Surface* surface);

//Load an image that will be used to create an OpenGL texture.
SDL_Surface* load_image_opengl(std::string filename);

SDL_Surface* load_image_sdl(std::string filename);

GLuint surface_to_texture(SDL_Surface* surface);

//Call this when loading image files to be used as textures throughout the program.
GLuint load_texture(std::string filename,image_data* id);

void render_fbo_texture();

//Render a full texture to the screen.
void render_texture(double x,double y,image_data image_source,double opacity=1.0,short color_name=COLOR_WHITE);
void render_texture_opengl(double x,double y,image_data image_source,double opacity=1.0,short color_name=COLOR_WHITE);
void render_surface(double x,double y,image_data image_source,double opacity=1.0,short color_name=COLOR_WHITE);

//Render a sprite from a spritesheet to the screen.
void render_sprite(double x,double y,image_data image_source,SDL_Rect* texture_clip=NULL,double opacity=1.0,double scale_x=1.0,double scale_y=1.0,double angle=0.0,short color_name=COLOR_WHITE,bool flip_x=false);
void render_sprite_opengl(double x,double y,image_data image_source,SDL_Rect* texture_clip=NULL,double opacity=1.0,double scale_x=1.0,double scale_y=1.0,double angle=0.0,short color_name=COLOR_WHITE,bool flip_x=false);
void render_sprite_sdl(double x,double y,image_data image_source,SDL_Rect* texture_clip=NULL,double opacity=1.0,double scale_x=1.0,double scale_y=1.0,double angle=0.0,short color_name=COLOR_WHITE,bool flip_x=false);

void render_rectangle(double x,double y,double w,double h,double opacity,short color_name);
void render_rectangle_opengl(double x,double y,double w,double h,double opacity,short color_name);
void render_rectangle_sdl(double x,double y,double w,double h,double opacity,short color_name);

//Specifically for lighting stuff.
void render_rectangle(double x,double y,double w,double h,double opacity,color_data color);
void render_rectangle_opengl(double x,double y,double w,double h,double opacity,color_data color);
void render_rectangle_sdl(double x,double y,double w,double h,double opacity,color_data color);

void render_line(double x1,double y1,double x2,double y2,double opacity,short color_name);
void render_line_opengl(double x1,double y1,double x2,double y2,double opacity,short color_name);
void render_line_sdl(double x1,double y1,double x2,double y2,double opacity,short color_name);

//Converts a color name (defined in enumerations.h) to red, green, blue color data in double format.
color_data color_name_to_doubles(short color_number);

//Converts red, green, blue color data in 0-255 format to double format.
color_data color_shorts_to_doubles(short red,short green,short blue);

#endif
