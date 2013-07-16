/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "tooltip.h"
#include "world.h"
#include "render.h"
#include "holidays.h"

using namespace std;

Tooltip::Tooltip(){
    on=false;

    message="";

    //Starting coordinates.
    x=0;
    y=0;

    //Dimensions.
    w=0;
    h=0;
}

void Tooltip::setup(string get_message,int mouse_x,int mouse_y){
    message=get_message;
    on=true;
    x=mouse_x;
    y=mouse_y+24;
    set_dimensions();

    //If the tooltip would be displayed off the screen, move it.

    if(x+w>main_window.SCREEN_WIDTH){
        x=mouse_x-w;
    }

    if(y+h>main_window.SCREEN_HEIGHT){
        y=mouse_y-24-h;
    }
}

void Tooltip::set_dimensions(){
    short current_width=11;
    w=11;
    h=24;

    for(short i=0;i<message.length();i++){
        if(message[i]!='\xA'){
            current_width+=11;
        }
        //If we hit a new line.
        else{
            h+=24;
            if(current_width>w){
                w=current_width;
            }
            current_width=11;
        }

        if(i==message.length()-1){
            if(current_width>w){
                w=current_width;
            }
        }
    }
}

void Tooltip::render(){
    //As long as the tooltip is on and has content.
    if(on && message.length()>0){
        render_rectangle(x-4,y-4,w,h,1.0,return_gui_color(holiday,0));
        render_rectangle(x-4+2,y-4+2,w-4,h-4,1.0,return_gui_color(holiday,1));

        font.show(x+2,y+2,message,COLOR_BLACK);
        font.show(x,y,message,return_gui_color(holiday,3));
    }
}
