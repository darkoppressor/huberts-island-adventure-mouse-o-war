/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "button.h"
#include "render.h"
#include "world.h"
#include "collision.h"
#include "button_events.h"
#include "holidays.h"

using namespace std;

Button::Button(short get_x,short get_y,string get_tooltip_text,string get_text,void (*get_event_function)(Window*,int),sound_data* get_mouse_over_sound,sound_data* get_event_fire_sound,bool get_invisible,int get_special_data,bool get_enabled){
    x=get_x;
    y=get_y;

    moused_over=false;
    clicked=false;

    text=get_text;
    tooltip_text=get_tooltip_text;

    event_function=get_event_function;

    mouse_over_sound=get_mouse_over_sound;
    event_fire_sound=get_event_fire_sound;

    invisible=get_invisible;

    special_data=get_special_data;

    enabled=get_enabled;

    set_dimensions();
}

void Button::set_invisibility(bool invis){
    invisible=invis;
}

void Button::set_enabled(bool get_enabled){
    enabled=get_enabled;
}

void Button::set_coordinates(short get_x,short get_y){
    x=get_x;
    y=get_y;
}

void Button::set_dimensions(){
    short current_width=11;
    w=11;
    h=24;

    for(short i=0;i<text.length();i++){
        if(text[i]!='\xA'){
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

        if(i==text.length()-1){
            if(current_width>w){
                w=current_width;
            }
        }
    }
}

bool Button::is_moused_over(int mouse_x,int mouse_y,short x_offset,short y_offset,int index){
    if(index==player.current_button || (player.mouse_allowed() && collision_check(mouse_x,mouse_y,2,2,x_offset+x,y_offset+y,w,h))){
        return true;
    }

    return false;
}

bool Button::reset_moused_over(){
    if(moused_over){
        moused_over=false;
        return true;
    }
    else{
        return false;
    }
}

void Button::reset_clicked(){
    clicked=false;
}

void Button::mouse_over(bool previously_moused_over){
    //If the mouse was not already over this button before this frame, and mouse_over_sound points to some sound,
    //and the button is enabled.
    if(!previously_moused_over && mouse_over_sound!=0 && enabled && !clicked){
        if(window_manager.allow_button_sound){
            play_positional_sound(*mouse_over_sound);
        }
        else{
            window_manager.allow_button_sound=true;
        }
    }

    moused_over=true;
}

void Button::mouse_button_down(){
    clicked=true;
}

void Button::mouse_button_up(Window* parent_window){
    //If this button was clicked down on, and the button is enabled.
    if(clicked && enabled){
        //If event_fire_sound points to some sound and the event function is the fullscreen toggler,
        //we play the event fire sound before the actual event is fired.
        //Otherwise, we will play the sound after the event is fired (see below the call to fire_event()).
        if(event_fire_sound!=0 && event_function==&button_event_option_fullscreen){
            play_positional_sound(*event_fire_sound);
        }

        //Fire the button's event.
        fire_event(parent_window);

        if(event_fire_sound!=0 && event_function!=&button_event_option_fullscreen){
            play_positional_sound(*event_fire_sound);
        }
    }
}

string Button::return_tooltip_text(){
    return tooltip_text;
}

bool Button::has_tooltip(){
    if(tooltip_text.length()>0){
        return true;
    }
    else{
        return false;
    }
}

void Button::fire_event(Window* parent_window){
    (*event_function)(parent_window,special_data);
}

void Button::render(short x_offset,short y_offset,int index){
    //If the button is not invisible.
    if(!invisible){
        //Normal.
        if(!moused_over && !clicked){
            //Render the border.
            render_rectangle(x_offset+x-3,y_offset+y,w,h+3,1.0,return_gui_color(holiday,0));

            //Render the background.
            render_rectangle(x_offset+x+3-3,y_offset+y+3,w-6,h-3,1.0,return_gui_color(holiday,1));

            //Display the button's text.
            font.show(x_offset+x+2,y_offset+y+6+2,text,COLOR_BLACK,1.0);
            font.show(x_offset+x,y_offset+y+6,text,return_gui_color(holiday,3),1.0);
        }
        //Moused over.
        else if(moused_over && !clicked){
            //Render the border.
            render_rectangle(x_offset+x-3,y_offset+y,w,h+3,1.0,return_gui_color(holiday,0));

            //Render the background.
            render_rectangle(x_offset+x+3-3,y_offset+y+3,w-6,h-3,1.0,return_gui_color(holiday,4));

            //Display the button's text.
            font.show(x_offset+x+2,y_offset+y+6+2,text,COLOR_BLACK,1.0);
            font.show(x_offset+x,y_offset+y+6,text,return_gui_color(holiday,3),1.0);
        }
        //Clicked down on.
        else if(clicked){
            //Render the border.
            render_rectangle(x_offset+x-3,y_offset+y,w,h+3,1.0,return_gui_color(holiday,0));

            //Render the background.
            render_rectangle(x_offset+x+3-3,y_offset+y+3,w-6,h-3,1.0,return_gui_color(holiday,2));

            //Display the button's text.
            font.show(x_offset+x+2,y_offset+y+6+2,text,COLOR_BLACK,1.0);
            font.show(x_offset+x,y_offset+y+6,text,return_gui_color(holiday,3),1.0);
        }
    }

    if(player.current_button==index){
        //Top
        render_rectangle(x_offset+x-6,y_offset+y-4,w+6,4,1.0,return_gui_color(holiday,0));
        //Bottom
        render_rectangle(x_offset+x-6,y_offset+y+h,w+6,6,1.0,return_gui_color(holiday,0));
        //Left
        render_rectangle(x_offset+x-6,y_offset+y-4,6,h+4,1.0,return_gui_color(holiday,0));
        //Right
        render_rectangle(x_offset+x+w-5,y_offset+y-4,5,h+10,1.0,return_gui_color(holiday,0));
    }
}
