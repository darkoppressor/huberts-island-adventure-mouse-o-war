/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "render.h"
#include "world.h"
#include "pixels.h"
#include "color.h"
#include "message_log.h"

using namespace std;

SDL_Surface* optimize_surface(SDL_Surface* surface){
    SDL_PixelFormat RGBAFormat;
    RGBAFormat.palette=0;
    RGBAFormat.BitsPerPixel=32;
    RGBAFormat.BytesPerPixel=4;

    if(SDL_BYTEORDER==SDL_BIG_ENDIAN){
        RGBAFormat.Rmask=0xFF000000; RGBAFormat.Rshift=0; RGBAFormat.Rloss=0;
        RGBAFormat.Gmask=0x00FF0000; RGBAFormat.Gshift=8; RGBAFormat.Gloss=0;
        RGBAFormat.Bmask=0x0000FF00; RGBAFormat.Bshift=16; RGBAFormat.Bloss=0;
        RGBAFormat.Amask=0x000000FF; RGBAFormat.Ashift=24; RGBAFormat.Aloss=0;
    }
    else{
        RGBAFormat.Rmask=0x000000FF; RGBAFormat.Rshift=24; RGBAFormat.Rloss=0;
        RGBAFormat.Gmask=0x0000FF00; RGBAFormat.Gshift=16; RGBAFormat.Gloss=0;
        RGBAFormat.Bmask=0x00FF0000; RGBAFormat.Bshift=8; RGBAFormat.Bloss=0;
        RGBAFormat.Amask=0xFF000000; RGBAFormat.Ashift=0; RGBAFormat.Aloss=0;
    }

    SDL_Surface* conv=SDL_ConvertSurface(surface,&RGBAFormat,0);

    if(conv==0){
        msg="Error converting surface: ";
        msg+=SDL_GetError();
        update_error_log(msg);
    }

    return conv;
}

SDL_Surface* load_image(string filename){
    SDL_Surface* loaded_image=0;
    SDL_Surface* optimized_image=0;

    loaded_image=IMG_Load(filename.c_str());

    if(loaded_image!=0){
        optimized_image=optimize_surface(loaded_image);
        SDL_FreeSurface(loaded_image);
    }
    else{
        update_error_log("Error loading image '"+filename+"': "+IMG_GetError());
    }

    return optimized_image;
}

SDL_Texture* load_texture(string filename,image_data* id){
    SDL_Surface* surface=load_image(filename.c_str());

    id->w=surface->w;
    id->h=surface->h;

    SDL_Texture* texture=main_window.create_texture_from_surface(surface);

    if(texture==0){
        msg="Error creating texture: ";
        msg+=SDL_GetError();
        update_error_log(msg);
    }

    SDL_FreeSurface(surface);

    return texture;
}

void render_rtt(double x,double y,Rtt_Data* rtt_source,double opacity,double scale_x,double scale_y,double angle,short color_name,bool flip_x,bool flip_y){
    SDL_Rect rect_dst;
    rect_dst.x=x;
    rect_dst.y=y;
    rect_dst.w=rtt_source->w*scale_x;
    rect_dst.h=rtt_source->h*scale_y;

    uint32_t flip=SDL_FLIP_NONE;
    if(flip_x && flip_y){
        flip=SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL;
    }
    else if(flip_x){
        flip=SDL_FLIP_HORIZONTAL;
    }
    else if(flip_y){
        flip=SDL_FLIP_VERTICAL;
    }

    SDL_SetTextureAlphaMod(rtt_source->texture,(uint8_t)(opacity*255.0));

    color_data colordata=color_name_to_doubles(color_name);
    Color color(colordata.red*255.0,colordata.green*255.0,colordata.blue*255.0,255);
    SDL_SetTextureColorMod(rtt_source->texture,(uint8_t)color.get_red(),(uint8_t)color.get_green(),(uint8_t)color.get_blue());

    main_window.render_copy_ex(rtt_source->texture,0,&rect_dst,-angle,0,(SDL_RendererFlip)flip);
}

void render_texture(double x,double y,image_data image_source,double opacity,short color_name,double scale_x,double scale_y){
    SDL_Rect rect_dst;
    rect_dst.x=x;
    rect_dst.y=y;
    rect_dst.w=image_source.w*scale_x;
    rect_dst.h=image_source.h*scale_y;

    SDL_SetTextureAlphaMod(image_source.texture,(uint8_t)(opacity*255.0));

    color_data colordata=color_name_to_doubles(color_name);
    Color color(colordata.red*255.0,colordata.green*255.0,colordata.blue*255.0,255);
    SDL_SetTextureColorMod(image_source.texture,(uint8_t)color.get_red(),(uint8_t)color.get_green(),(uint8_t)color.get_blue());

    main_window.render_copy_ex(image_source.texture,0,&rect_dst,0.0,0,SDL_FLIP_NONE);
}

void render_sprite(double x,double y,image_data image_source,SDL_Rect* texture_clip,double opacity,double scale_x,double scale_y,double angle,short color_name,bool flip_x){
    SDL_Rect rect_src;
    rect_src.x=texture_clip->x;
    rect_src.y=texture_clip->y;
    rect_src.w=texture_clip->w;
    rect_src.h=texture_clip->h;

    SDL_Rect rect_dst;
    rect_dst.x=x;
    rect_dst.y=y;
    rect_dst.w=texture_clip->w*scale_x;
    rect_dst.h=texture_clip->h*scale_y;

    uint32_t flip=SDL_FLIP_NONE;
    if(flip_x){
        flip=SDL_FLIP_HORIZONTAL;
    }

    SDL_SetTextureAlphaMod(image_source.texture,(uint8_t)(opacity*255.0));

    color_data colordata=color_name_to_doubles(color_name);
    Color color(colordata.red*255.0,colordata.green*255.0,colordata.blue*255.0,255);
    SDL_SetTextureColorMod(image_source.texture,(uint8_t)color.get_red(),(uint8_t)color.get_green(),(uint8_t)color.get_blue());

    main_window.render_copy_ex(image_source.texture,&rect_src,&rect_dst,-angle,0,(SDL_RendererFlip)flip);
}

void render_rectangle(double x,double y,double w,double h,double opacity,short color_name){
    main_window.set_render_draw_blend_mode(SDL_BLENDMODE_BLEND);

    color_data colordata=color_name_to_doubles(color_name);
    Color color(colordata.red*255.0,colordata.green*255.0,colordata.blue*255.0,255);
    main_window.set_render_draw_color(color,opacity);

    SDL_Rect rect;
    rect.x=x;
    rect.y=y;
    rect.w=w;
    rect.h=h;

    main_window.render_fill_rect(&rect);
}

void render_rectangle(double x,double y,double w,double h,double opacity,color_data colordata){
    main_window.set_render_draw_blend_mode(SDL_BLENDMODE_BLEND);

    Color color(colordata.red*255.0,colordata.green*255.0,colordata.blue*255.0,255);
    main_window.set_render_draw_color(color,opacity);

    SDL_Rect rect;
    rect.x=x;
    rect.y=y;
    rect.w=w;
    rect.h=h;

    main_window.render_fill_rect(&rect);
}

void render_line(double x1,double y1,double x2,double y2,double opacity,short color_name){
    main_window.set_render_draw_blend_mode(SDL_BLENDMODE_BLEND);

    color_data colordata=color_name_to_doubles(color_name);
    Color color(colordata.red*255.0,colordata.green*255.0,colordata.blue*255.0,255);
    main_window.set_render_draw_color(color,opacity);

    main_window.render_draw_line(x1,y1,x2,y2);
}

color_data color_name_to_doubles(short color_number){
    color_data new_color={1.0,1.0,1.0};

    switch(color_number){

    case COLOR_RAINBOW:
        new_color=color_name_to_doubles(random_range_render(COLOR_BEGIN,COLOR_END-1));
        break;
    case COLOR_BLACK:
        new_color=color_shorts_to_doubles(0,0,0);
        break;
    case COLOR_GRAY:
        new_color=color_shorts_to_doubles(128,128,128);
        break;
    case COLOR_WHITE:
        new_color=color_shorts_to_doubles(255,255,255);
        break;
    case COLOR_BROWN:
        new_color=color_shorts_to_doubles(150,75,0);
        break;
    case COLOR_DARK_BROWN:
        new_color=color_shorts_to_doubles(101,67,33);
        break;
    case COLOR_COPPER:
        new_color=color_shorts_to_doubles(184,115,51);
        break;
    case COLOR_BRONZE:
        new_color=color_shorts_to_doubles(205,127,50);
        break;
    case COLOR_SEPIA:
        new_color=color_shorts_to_doubles(112,66,20);
        break;
    case COLOR_SANDY_BROWN:
        new_color=color_shorts_to_doubles(244,164,96);
        break;
    case COLOR_WHEAT:
        new_color=color_shorts_to_doubles(245,222,179);
        break;
    case COLOR_TAN:
        new_color=color_shorts_to_doubles(210,180,140);
        break;
    case COLOR_PALE_BROWN:
        new_color=color_shorts_to_doubles(152,118,84);
        break;
    case COLOR_CHAMPAGNE:
        new_color=color_shorts_to_doubles(247,231,206);
        break;
    case COLOR_YELLOW:
        new_color=color_shorts_to_doubles(255,205,0);
        break;
    case COLOR_SYSTEM:
        new_color=color_shorts_to_doubles(255,216,0);
        break;
    case COLOR_GOLD:
        new_color=color_shorts_to_doubles(212,175,55);
        break;
    case COLOR_YELLOW_PASTEL:
        new_color=color_shorts_to_doubles(255,255,91);
        break;
    case COLOR_YELLOW_FULL:
        new_color=color_shorts_to_doubles(255,255,0);
        break;
    case COLOR_ORANGE:
        new_color=color_shorts_to_doubles(255,159,0);
        break;
    case COLOR_PUMPKIN:
        new_color=color_shorts_to_doubles(255,117,24);
        break;
    case COLOR_ORANGE_PASTEL:
        new_color=color_shorts_to_doubles(255,203,88);
        break;
    case COLOR_ORANGE_FULL:
        new_color=color_shorts_to_doubles(255,128,0);
        break;
    case COLOR_RED:
        new_color=color_shorts_to_doubles(255,0,0);
        break;
    case COLOR_DARK_RED:
        new_color=color_shorts_to_doubles(206,22,32);
        break;
    case COLOR_FLAME:
        new_color=color_shorts_to_doubles(226,88,34);
        break;
    case COLOR_WINE:
        new_color=color_shorts_to_doubles(114,47,55);
        break;
    case COLOR_PINK:
        new_color=color_shorts_to_doubles(255,192,203);
        break;
    case COLOR_ROSE:
        new_color=color_shorts_to_doubles(255,0,127);
        break;
    case COLOR_SHOCKING_PINK:
        new_color=color_shorts_to_doubles(252,15,192);
        break;
    case COLOR_PURPLE:
        new_color=color_shorts_to_doubles(128,0,128);
        break;
    case COLOR_VIOLET:
        new_color=color_shorts_to_doubles(143,0,255);
        break;
    case COLOR_INDIGO:
        new_color=color_shorts_to_doubles(111,0,255);
        break;
    case COLOR_PURPLE_PASTEL:
        new_color=color_shorts_to_doubles(181,94,181);
        break;
    case COLOR_PURPLE_FULL:
        new_color=color_shorts_to_doubles(128,0,255);
        break;
    case COLOR_BLUE:
        new_color=color_shorts_to_doubles(0,0,255);
        break;
    case COLOR_SKY_BLUE:
        new_color=color_shorts_to_doubles(60,210,255);
        break;
    case COLOR_UN_BLUE:
        new_color=color_shorts_to_doubles(75,146,219);
        break;
    case COLOR_BLUE_OCEAN:
        new_color=color_shorts_to_doubles(68,136,255);
        break;
    case COLOR_GREEN:
        new_color=color_shorts_to_doubles(0,255,0);
        break;
    case COLOR_JUNGLE:
        new_color=color_shorts_to_doubles(41,171,135);
        break;
    case COLOR_SPRING:
        new_color=color_shorts_to_doubles(0,255,127);
        break;
    case COLOR_DARK_GREEN:
        new_color=color_shorts_to_doubles(0,191,0);
        break;
    case COLOR_GRASS_GREEN:
        new_color=color_shorts_to_doubles(68,204,34);
        break;

    }

    return new_color;
}

color_data color_shorts_to_doubles(short red,short green,short blue){
    color_data new_color={1.0,1.0,1.0};

    new_color.red=(double)red/255.0;
    new_color.green=(double)green/255.0;
    new_color.blue=(double)blue/255.0;

    return new_color;
}
