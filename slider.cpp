/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "slider.h"
#include "render.h"
#include "world.h"
#include "special_info_strings.h"
#include "holidays.h"

using namespace std;

Slider::Slider(int get_display_identifier,bool get_achievement){
    display_identifier=get_display_identifier;
    achievement=get_achievement;

    played_sound=false;

    //Set the display string:

    if(achievement){
        special_info_return_data special_info_data;
        special_info_data=set_special_info_string(SPECIAL_INFO_ACHIEVEMENT+display_identifier);

        display_string="Achievement earned:\xA";
        display_string+=special_info_data.text;
    }
    else{
        //We'll just lie and let it think we already played the sound, so it doesn't play it.
        played_sound=true;

        display_string="Item acquired:\xA";
        display_string+=player.name_inventory_item(display_identifier);
    }

    //Set the dimensions of the slider.
    set_dimensions();

    if(achievement){
        x=main_window.SCREEN_WIDTH-w;
        y=main_window.SCREEN_HEIGHT;
    }
    else{
        x=9;
        y=main_window.SCREEN_HEIGHT;
    }

    counter=120;
}

void Slider::set_dimensions(){
    short current_width=11;
    w=11;
    h=24;

    for(short i=0;i<display_string.length();i++){
        //
        if(display_string[i]!='\xA'){
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

        if(i==display_string.length()-1){
            if(current_width>w){
                w=current_width;
            }
        }
    }
}

bool Slider::move(){
    //If the slider has not reached its maximum height and the counter has not counted down yet.
    if(y>main_window.SCREEN_HEIGHT-h && counter>0){
        //Raise the slider.
        y--;
    }
    //If the slider is at its maximum height and the counter has not counted down yet.
    else if(y<=main_window.SCREEN_HEIGHT-h && counter>0){
        //Count down.
        counter--;
    }
    //If the counter has counted down.
    else if(counter==0){
        //Move the slider down.
        y++;
    }

    //If the counter has counted down and the slider has gone off the bottom of the screen.
    if(counter==0 && y>main_window.SCREEN_HEIGHT){
        return false;
    }

    return true;
}

void Slider::render(){
    render_rectangle(x-4,y-4,w,h,1.0,return_gui_color(holiday,0));
    render_rectangle(x-4+2,y-4+2,w-4,h-4,1.0,return_gui_color(holiday,1));

    font.show(x+2,y+2,display_string,COLOR_BLACK);
    font.show(x,y,display_string,return_gui_color(holiday,3));

    if(!played_sound){
        played_sound=true;

        play_positional_sound(sound_system.achievement_earned);
    }
}
