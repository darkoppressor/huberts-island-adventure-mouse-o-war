/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "render.h"
#include "world.h"
#include "pixels.h"

#include <SDL_gfxPrimitives.h>
#include <SDL_gfxBlitFunc.h>
#include <SDL_rotozoom.h>

using namespace std;

SDL_Surface* optimize_image_opengl(SDL_Surface* surface){
    SDL_PixelFormat RGBAFormat;
    RGBAFormat.palette=0;
    RGBAFormat.colorkey=0;
    RGBAFormat.alpha=0;
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

    SDL_Surface* conv=SDL_ConvertSurface(surface,&RGBAFormat,SDL_SWSURFACE);

    return conv;
}

SDL_Surface* load_image_opengl(string filename){
    SDL_Surface* loaded_image=0;
    SDL_Surface* optimized_image=0;

    //Load the image.
    loaded_image=IMG_Load(filename.c_str());

    if(loaded_image!=0){
        optimized_image=optimize_image_opengl(loaded_image);
        SDL_FreeSurface(loaded_image);
    }

    //Return the optimized image.
    return optimized_image;
}

SDL_Surface* load_image_sdl(string filename){
    SDL_Surface* loaded_image=0;
    SDL_Surface* optimized_image=0;

    //Load the image.
    loaded_image=IMG_Load(filename.c_str());

    if(loaded_image!=0){
        optimized_image=SDL_DisplayFormatAlpha(loaded_image);
        SDL_FreeSurface(loaded_image);
    }

    //Return the optimized image.
    return optimized_image;
}

GLuint surface_to_texture(SDL_Surface* surface){
    //This texture will be returned at the end of this function.
    GLuint texture;

    //Have OpenGL generate a texture object handle for us.
    glGenTextures(1,&texture);

    //Bind the texture object.
    glBindTexture(GL_TEXTURE_2D,texture);

    //Set the texture's properties:
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);///GL_LINEAR
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);///GL_LINEAR

    //Edit the texture object's image data using the information our image gives us:
    glTexImage2D(GL_TEXTURE_2D,0,surface->format->BytesPerPixel,surface->w,surface->h,0,GL_RGBA,GL_UNSIGNED_BYTE,surface->pixels);

    return texture;
}

GLuint load_texture(string filename,image_data* id){
    //This will temporarily store the name of the image we are loading.
    SDL_Surface* surface=0;

    //Load the image.
    surface=load_image_opengl(filename.c_str());

    GLuint texture=surface_to_texture(surface);

    id->w=surface->w;
    id->h=surface->h;

    //We have copied the image data into the new texture, so delete the image data.
    SDL_FreeSurface(surface);

    //Return the new texture we have created.
    return texture;
}

void render_fbo_texture(){
    glPushMatrix();
    //Bind the texture object.
    if(image.current_texture!=main_window.fbo_texture){
        glBindTexture(GL_TEXTURE_2D,main_window.fbo_texture);
        image.current_texture=main_window.fbo_texture;
    }

    glDisable(GL_BLEND);

    //Move to the offset of the image we want to place.
    glTranslated(0,0,0);

    color_data color=color_name_to_doubles(COLOR_WHITE);
    glColor4d(color.red,color.green,color.blue,1.0);

    //Start quad.
    glBegin(GL_QUADS);

    //Apply the texture to the screen:
    glTexCoord2d(0,1);  glVertex3d(0,0,0);
    glTexCoord2d(1,1);  glVertex3d(main_window.SCREEN_WIDTH,0,0);
    glTexCoord2d(1,0);  glVertex3d(main_window.SCREEN_WIDTH,main_window.SCREEN_HEIGHT,0);
    glTexCoord2d(0,0);  glVertex3d(0,main_window.SCREEN_HEIGHT,0);

    //End quad.
    glEnd();

    glEnable(GL_BLEND);

    //Reset.
    glPopMatrix();
}

void render_texture(double x,double y,image_data image_source,double opacity,short color_name){
    if(player.option_renderer==RENDERER_HARDWARE){
        render_texture_opengl(x,y,image_source,opacity,color_name);
    }
    else if(player.option_renderer==RENDERER_SOFTWARE){
        render_surface(x,y,image_source,opacity,color_name);
    }
}

void render_texture_opengl(double x,double y,image_data image_source,double opacity,short color_name){
    glPushMatrix();
    //Bind the texture object.
    if(image.current_texture!=image_source.texture){
        glBindTexture(GL_TEXTURE_2D,image_source.texture);
        image.current_texture=image_source.texture;
    }

    //Move to the offset of the image we want to place.
    glTranslated(x,y,0);

    color_data color=color_name_to_doubles(color_name);
    glColor4d(color.red,color.green,color.blue,opacity);

    //Start quad.
    glBegin(GL_QUADS);

    //Apply the texture to the screen:
    glTexCoord2d(0,0);  glVertex3d(0,0,0);
    glTexCoord2d(1,0);  glVertex3d(image_source.w,0,0);
    glTexCoord2d(1,1);  glVertex3d(image_source.w,image_source.h,0);
    glTexCoord2d(0,1);  glVertex3d(0,image_source.h,0);

    //End quad.
    glEnd();

    //Reset.
    glPopMatrix();
}

void render_surface(double x,double y,image_data image_source,double opacity,short color_name){
    SDL_Rect position;
    position.x=x;
    position.y=y;

    if(opacity!=1.0){
        SDL_gfxSetAlpha(image_source.surface,opacity*255);
    }

    if(color_name!=COLOR_WHITE){
        color_data color=color_name_to_doubles(color_name);
        SDL_Surface* cover=SDL_CreateRGBSurface(image_source.surface->flags,image_source.surface->w,image_source.surface->h,image_source.surface->format->BitsPerPixel,image_source.surface->format->Rmask,image_source.surface->format->Gmask,image_source.surface->format->Bmask,image_source.surface->format->Amask);

        if(SDL_MUSTLOCK(image_source.surface)){
            SDL_LockSurface(image_source.surface);
        }
        for(int x=0;x<image_source.surface->w;x++){
            for(int y=0;y<image_source.surface->h;y++){
                Uint32 pixel=surface_get_pixel(image_source.surface,x,y);
                Uint8 red;
                Uint8 green;
                Uint8 blue;
                Uint8 alpha;
                SDL_GetRGBA(pixel,image_source.surface->format,&red,&green,&blue,&alpha);
                if(alpha!=0){
                    Uint32 pixel_cover=SDL_MapRGBA(image_source.surface->format,color.red*255,color.green*255,color.blue*255,191);
                    surface_put_pixel(cover,x,y,pixel_cover);
                }
                else{
                    Uint32 pixel_cover=SDL_MapRGBA(image_source.surface->format,0,0,0,0);
                    surface_put_pixel(cover,x,y,pixel_cover);
                }
            }
        }
        if(SDL_MUSTLOCK(image_source.surface)){
            SDL_UnlockSurface(image_source.surface);
        }

        SDL_BlitSurface(image_source.surface,NULL,main_window.screen,&position);
        SDL_BlitSurface(cover,NULL,main_window.screen,&position);
        SDL_FreeSurface(cover);
    }
    else{
        SDL_BlitSurface(image_source.surface,NULL,main_window.screen,&position);
    }
}

void render_sprite(double x,double y,image_data image_source,SDL_Rect* texture_clip,double opacity,double scale_x,double scale_y,double angle,short color_name,bool flip_x){
    if(player.option_renderer==RENDERER_HARDWARE){
        render_sprite_opengl(x,y,image_source,texture_clip,opacity,scale_x,scale_y,angle,color_name,flip_x);
    }
    else if(player.option_renderer==RENDERER_SOFTWARE){
        render_sprite_sdl(x,y,image_source,texture_clip,opacity,scale_x,scale_y,angle,color_name,flip_x);
    }
}

void render_sprite_opengl(double x,double y,image_data image_source,SDL_Rect* texture_clip,double opacity,double scale_x,double scale_y,double angle,short color_name,bool flip_x){
    glPushMatrix();
    //Bind the texture object.
    if(image.current_texture!=image_source.texture){
        glBindTexture(GL_TEXTURE_2D,image_source.texture);
        image.current_texture=image_source.texture;
    }

    //Move to the offset of the image we want to place.
    glTranslated(x,y,0);

    glTranslated(texture_clip->w/2.0,texture_clip->h/2.0,0);

    glRotated(angle,0,0,-1);

    glTranslated(-texture_clip->w/2.0,-texture_clip->h/2.0,0);

    glScaled(scale_x,scale_y,1.0);

    if(flip_x){
        glScalef(-1,1,1);
        glTranslated(-texture_clip->w,0,0);
    }

    color_data color=color_name_to_doubles(color_name);
    glColor4d(color.red,color.green,color.blue,opacity);

    //Start quad.
    glBegin(GL_QUADS);

    //Apply the texture to the screen:

    //Bottom left corner.
    glTexCoord2d((texture_clip->x)/image_source.w,(texture_clip->y+texture_clip->h)/image_source.h);
    glVertex3d(0,texture_clip->h,0);

    //Bottom right corner.
    glTexCoord2d((texture_clip->x+texture_clip->w)/image_source.w,(texture_clip->y+texture_clip->h)/image_source.h);
    glVertex3d(texture_clip->w,texture_clip->h,0);

    //Top right corner.
    glTexCoord2d((texture_clip->x+texture_clip->w)/image_source.w,(texture_clip->y)/image_source.h);
    glVertex3d(texture_clip->w,0,0);

    //Top left corner.
    glTexCoord2d((texture_clip->x)/image_source.w,(texture_clip->y)/image_source.h);
    glVertex3d(0,0,0);

    //End quad.
    glEnd();

    //Reset.
    glPopMatrix();
}

void render_sprite_sdl(double x,double y,image_data image_source,SDL_Rect* texture_clip,double opacity,double scale_x,double scale_y,double angle,short color_name,bool flip_x){
    SDL_Rect position;
    position.x=x;
    position.y=y;

    if(opacity!=1.0){
        SDL_gfxSetAlpha(image_source.surface,opacity*255);
    }

    if(scale_x!=1.0 || scale_y!=1.0 || angle!=0.0){
        SDL_Surface* temp_surface1=SDL_CreateRGBSurface(main_window.screen->flags,texture_clip->w,texture_clip->h,main_window.SCREEN_BPP,main_window.screen->format->Rmask,main_window.screen->format->Gmask,main_window.screen->format->Bmask,main_window.screen->format->Amask);

        SDL_BlitSurface(image_source.surface,texture_clip,temp_surface1,NULL);

        SDL_Surface* temp_surface2=rotozoomSurfaceXY(temp_surface1,angle,scale_x,scale_y,SMOOTHING_OFF);

        if(color_name!=COLOR_WHITE){
            color_data color=color_name_to_doubles(color_name);
            SDL_Surface* cover=SDL_CreateRGBSurface(temp_surface2->flags,temp_surface2->w,temp_surface2->h,temp_surface2->format->BitsPerPixel,temp_surface2->format->Rmask,temp_surface2->format->Gmask,temp_surface2->format->Bmask,temp_surface2->format->Amask);

            if(SDL_MUSTLOCK(temp_surface2)){
                SDL_LockSurface(temp_surface2);
            }
            for(int x=0;x<temp_surface2->w;x++){
                for(int y=0;y<temp_surface2->h;y++){
                    Uint32 pixel=surface_get_pixel(temp_surface2,x,y);
                    Uint8 red;
                    Uint8 green;
                    Uint8 blue;
                    Uint8 alpha;
                    SDL_GetRGBA(pixel,temp_surface2->format,&red,&green,&blue,&alpha);
                    if(alpha!=0){
                        Uint32 pixel_cover=SDL_MapRGBA(temp_surface2->format,color.red*255,color.green*255,color.blue*255,191);
                        surface_put_pixel(cover,x,y,pixel_cover);
                    }
                    else{
                        Uint32 pixel_cover=SDL_MapRGBA(temp_surface2->format,0,0,0,0);
                        surface_put_pixel(cover,x,y,pixel_cover);
                    }
                }
            }
            if(SDL_MUSTLOCK(temp_surface2)){
                SDL_UnlockSurface(temp_surface2);
            }

            SDL_BlitSurface(temp_surface2,NULL,main_window.screen,&position);
            SDL_BlitSurface(cover,NULL,main_window.screen,&position);
            SDL_FreeSurface(cover);
        }
        else{
            SDL_BlitSurface(temp_surface2,NULL,main_window.screen,&position);
        }

        SDL_FreeSurface(temp_surface1);
        SDL_FreeSurface(temp_surface2);
    }
    else{
        if(color_name!=COLOR_WHITE){
            color_data color=color_name_to_doubles(color_name);
            SDL_Surface* cover=SDL_CreateRGBSurface(image_source.surface->flags,texture_clip->w,texture_clip->h,image_source.surface->format->BitsPerPixel,image_source.surface->format->Rmask,image_source.surface->format->Gmask,image_source.surface->format->Bmask,image_source.surface->format->Amask);

            if(SDL_MUSTLOCK(image_source.surface)){
                SDL_LockSurface(image_source.surface);
            }
            for(int x=texture_clip->x,x_cover=0;x<texture_clip->x+texture_clip->w;x++,x_cover++){
                for(int y=texture_clip->y,y_cover=0;y<texture_clip->y+texture_clip->h;y++,y_cover++){
                    Uint32 pixel=surface_get_pixel(image_source.surface,x,y);
                    Uint8 red;
                    Uint8 green;
                    Uint8 blue;
                    Uint8 alpha;
                    SDL_GetRGBA(pixel,image_source.surface->format,&red,&green,&blue,&alpha);
                    if(alpha!=0){
                        Uint32 pixel_cover=SDL_MapRGBA(image_source.surface->format,color.red*255,color.green*255,color.blue*255,191);
                        surface_put_pixel(cover,x_cover,y_cover,pixel_cover);
                    }
                    else{
                        Uint32 pixel_cover=SDL_MapRGBA(image_source.surface->format,0,0,0,0);
                        surface_put_pixel(cover,x_cover,y_cover,pixel_cover);
                    }
                }
            }
            if(SDL_MUSTLOCK(image_source.surface)){
                SDL_UnlockSurface(image_source.surface);
            }

            SDL_BlitSurface(image_source.surface,texture_clip,main_window.screen,&position);
            SDL_BlitSurface(cover,NULL,main_window.screen,&position);
            SDL_FreeSurface(cover);
        }
        else{
            SDL_BlitSurface(image_source.surface,texture_clip,main_window.screen,&position);
        }
    }
}

void render_rectangle(double x,double y,double w,double h,double opacity,short color_name){
    if(player.option_renderer==RENDERER_HARDWARE){
        render_rectangle_opengl(x,y,w,h,opacity,color_name);
    }
    else if(player.option_renderer==RENDERER_SOFTWARE){
        render_rectangle_sdl(x,y,w,h,opacity,color_name);
    }
}

void render_rectangle_opengl(double x,double y,double w,double h,double opacity,short color_name){
    glPushMatrix();
    //Bind the texture object.
    glBindTexture(GL_TEXTURE_2D,NULL);
    image.current_texture=0;

    //Move to the offset of the image we want to place.
    glTranslated(x,y,0);

    color_data color=color_name_to_doubles(color_name);
    glColor4d(color.red,color.green,color.blue,opacity);

    //Start quad.
    glBegin(GL_QUADS);

    //Apply the texture to the screen:
    glTexCoord2d(0,0);  glVertex3d(0,0,0);
    glTexCoord2d(1,0);  glVertex3d(w,0,0);
    glTexCoord2d(1,1);  glVertex3d(w,h,0);
    glTexCoord2d(0,1);  glVertex3d(0,h,0);

    //End quad.
    glEnd();

    //Reset.
    glPopMatrix();
}

void render_rectangle_sdl(double x,double y,double w,double h,double opacity,short color_name){
    color_data color=color_name_to_doubles(color_name);

    boxRGBA(main_window.screen,x+w,y,x,y+h,color.red*255,color.green*255,color.blue*255,opacity*255);
}

void render_rectangle(double x,double y,double w,double h,double opacity,color_data color){
    if(player.option_renderer==RENDERER_HARDWARE){
        render_rectangle_opengl(x,y,w,h,opacity,color);
    }
    else if(player.option_renderer==RENDERER_SOFTWARE){
        render_rectangle_sdl(x,y,w,h,opacity,color);
    }
}

void render_rectangle_opengl(double x,double y,double w,double h,double opacity,color_data color){
    glPushMatrix();
    //Bind the texture object.
    glBindTexture(GL_TEXTURE_2D,NULL);
    image.current_texture=0;

    //Move to the offset of the image we want to place.
    glTranslated(x,y,0);

    glColor4d(color.red,color.green,color.blue,opacity);

    //Start quad.
    glBegin(GL_QUADS);

    //Apply the texture to the screen:
    glTexCoord2d(0,0);  glVertex3d(0,0,0);
    glTexCoord2d(1,0);  glVertex3d(w,0,0);
    glTexCoord2d(1,1);  glVertex3d(w,h,0);
    glTexCoord2d(0,1);  glVertex3d(0,h,0);

    //End quad.
    glEnd();

    //Reset.
    glPopMatrix();
}

void render_rectangle_sdl(double x,double y,double w,double h,double opacity,color_data color){
    boxRGBA(main_window.screen,x+w,y,x,y+h,color.red*255,color.green*255,color.blue*255,opacity*255);
}

void render_line(double x1,double y1,double x2,double y2,double opacity,short color_name){
    if(player.option_renderer==RENDERER_HARDWARE){
        render_line_opengl(x1,y1,x2,y2,opacity,color_name);
    }
    else if(player.option_renderer==RENDERER_SOFTWARE){
        render_line_sdl(x1,y1,x2,y2,opacity,color_name);
    }
}

void render_line_opengl(double x1,double y1,double x2,double y2,double opacity,short color_name){
    glPushMatrix();
    //Bind the texture object.
    glBindTexture(GL_TEXTURE_2D,NULL);
    image.current_texture=0;

    color_data color=color_name_to_doubles(color_name);
    glColor4d(color.red,color.green,color.blue,opacity);

    glLineWidth(1);

    glBegin(GL_LINES);

    //Apply the texture to the screen:
    glVertex2d(x1,y1);
    glVertex2d(x2,y2);

    glEnd();

    //Reset.
    glPopMatrix();
}

void render_line_sdl(double x1,double y1,double x2,double y2,double opacity,short color_name){
    color_data color=color_name_to_doubles(color_name);

    lineRGBA(main_window.screen,x1,y1,x2,y2,color.red*255,color.green*255,color.blue*255,opacity*255);
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
