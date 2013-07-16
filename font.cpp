/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "font.h"
#include "render.h"
#include "world.h"

using namespace std;

BitmapFont::BitmapFont(){
    w=0;
    h=0;
    spacing_x=0;
    spacing_y=0;
}

void BitmapFont::unload_font(){
    bitmap_font_texture.unload_image();
}

void BitmapFont::build_font(string font_location,double get_w,double get_h,short get_spacing_x,short get_spacing_y){
    bitmap_font_texture.load_image(font_location);

    //Set the texture dimensions:
    w=get_w;
    h=get_h;
    bitmap_font_texture.w=w;
    bitmap_font_texture.h=h;

    //Set the spacing amounts.
    spacing_x=get_spacing_x;
    spacing_y=get_spacing_y;

    //Set the cell dimensions:
    double cellW=w/256;
    double cellH=h;

    //The current character we are setting.
    short currentChar=0;

    //Go through the cell columns.
    for(short cols=0;cols<256;cols++){
        //Set the character offset:
        chars[currentChar].x=cellW*cols;
        chars[currentChar].y=0;
        //Set the dimensions of the character:
        chars[currentChar].w=cellW;
        chars[currentChar].h=cellH;
        //Go to the next character.
        currentChar++;
    }
}

double BitmapFont::get_letter_width(){
    return bitmap_font_texture.w/256.0;
}
double BitmapFont::get_letter_height(){
    return bitmap_font_texture.h;
}

void BitmapFont::show(double x,double y,string text,short font_color,double opacity,double scale){
    //Temporary offsets.
    double X=x,Y=y;

    short scaled_spacing_x=(short)(spacing_x*scale);
    short scaled_spacing_y=(short)(spacing_y*scale);

    //Go through the text.
    for(short show=0;text[show]!='\0';show++){
        //Get the ASCII value of the character.
        short ascii=(unsigned char)text[show];
        if(text[show]!='\xA'){
            if(X+scaled_spacing_x>=0 && X<=main_window.SCREEN_WIDTH && Y+scaled_spacing_y>=0 && Y<=main_window.SCREEN_HEIGHT){
                //Show the character.
                render_sprite((int)X,(int)Y,bitmap_font_texture,&chars[ascii],opacity,scale,scale,0.0,font_color);
            }

            //Move over the width of the character with one pixel of padding.
            X+=scaled_spacing_x;
        }
        else{
            Y+=scaled_spacing_y;
            X=x;
        }
    }
}
