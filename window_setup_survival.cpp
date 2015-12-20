/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "window_setup_survival.h"
#include "button_events.h"
#include "world.h"
#include "time_string.h"
#include "holidays.h"
#include "render.h"
#include "collision.h"
#include "quit.h"
#include "file_io.h"

using namespace std;

Window_Setup_Survival::Window_Setup_Survival(short get_x,short get_y,short get_w,short get_h,string get_title){
    x=get_x;
    y=get_y;
    w=get_w;
    h=get_h;
    mouse_offset_x=0;
    mouse_offset_y=0;
    title=get_title;
    on=false;
    moving=false;

    special_input=NO_SPECIAL_INPUT;
    background_image=NULL;

    is_multiplayer=false;

    level_list_display_position=0;
    level_list_selection=0;
    recall_seconds=player.survival_seconds_to_escape;

    //Create the close button.
    create_button(w-23,5,"","X",&button_event_close_window,0,0,BUTTON_VISIBLE,-1);

    short back_button_space=37;
    short button_sound=0;
    short y_position=0;
    short button_spacing=120;

    msg="Very Short";
    button_sound=4-1;
    y_position=70;
    create_button(button_spacing,y_position,"",msg,&button_event_survival_time_very_short,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);
    button_spacing+=buttons[buttons.size()-1].w+30;

    msg="Short";
    button_sound=6-1;
    y_position=70;
    create_button(button_spacing,y_position,"",msg,&button_event_survival_time_short,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);
    button_spacing+=buttons[buttons.size()-1].w+30;

    msg="Medium";
    button_sound=8-1;
    y_position=70;
    create_button(button_spacing,y_position,"",msg,&button_event_survival_time_medium,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);
    button_spacing+=buttons[buttons.size()-1].w+30;

    msg="Long";
    button_sound=5-1;
    y_position=70;
    create_button(button_spacing,y_position,"",msg,&button_event_survival_time_long,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);
    button_spacing+=buttons[buttons.size()-1].w+30;

    msg="Very Long";
    button_sound=7-1;
    y_position=70;
    create_button(button_spacing,y_position,"",msg,&button_event_survival_time_very_long,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Back to Game Mode Select";
    button_sound=12-1;
    create_button(16,h-back_button_space,"",msg,&button_event_open_window_select_game_mode,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Start";
    button_sound=10-1;
    create_button(w-(msg.length()*font.spacing_x)-16,h-back_button_space,"",msg,&button_event_start_game_survival,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);
}

void Window_Setup_Survival::setup(bool multiplayer){
    levels.clear();

    is_multiplayer=multiplayer;

    if(is_multiplayer){
        title="Multiplayer Survival - Setup";
    }
    else{
        title="Singleplayer Survival - Setup";
    }

    //Look through all of the levels in the directory.
    for(File_IO_Directory_Iterator it("data/levels/survival");it.evaluate();it.iterate()){
        if(it.is_directory()){
            string file_name=it.get_file_name();

            levels.push_back(file_name);
        }
    }

    if(level_list_display_position>levels.size()-1){
        level_list_display_position=levels.size()-1;
    }

    if(level_list_selection>levels.size()-1){
        level_list_selection=levels.size()-1;
    }
}

void Window_Setup_Survival::handle_input_events(){
    if(on){
        int mouse_x,mouse_y;
        main_window.get_mouse_state(&mouse_x,&mouse_y);

        switch(event.type){
            case SDL_QUIT:
                quit_game();
                break;

            case SDL_MOUSEBUTTONDOWN:
                if(event.button.button==SDL_BUTTON_LEFT){
                    bool button_clicked=false;

                    //Look through all of the buttons.
                    for(int i=0;i<buttons.size();i++){
                        //If this button is moused over,
                        //it has been clicked down on.
                        if(buttons[i].is_moused_over(mouse_x,mouse_y,x,y,-2)){
                            buttons[i].mouse_button_down();
                            //A button has just been clicked, so we keep that in mind.
                            button_clicked=true;
                        }
                    }

                    //If no buttons were just clicked and the title bar of the window is clicked.
                    if(!button_clicked && collision_check(mouse_x,mouse_y,2,2,x,y,w,30)){
                        //Begin moving the window.
                        moving=true;
                        mouse_offset_x=mouse_x-x;
                        mouse_offset_y=mouse_y-y;
                    }
                }
                break;

            case SDL_MOUSEWHEEL:
                if(event.wheel.y<0){
                    if(level_list_display_position<levels.size()-1){
                        level_list_display_position++;
                    }
                }
                else if(event.wheel.y>0){
                    if(level_list_display_position>0){
                        level_list_display_position--;
                    }
                }
                break;

            case SDL_MOUSEBUTTONUP:
                if(event.button.button==SDL_BUTTON_LEFT){
                    //Stop moving the window.
                    moving=false;

                    for(int i=level_list_display_position,n=0;i<level_list_display_position+18;i++,n++){
                        //If this is a valid level.
                        if(i<levels.size()){
                            if(collision_check(mouse_x,mouse_y,1,1,0,70+font.spacing_y*2+n*font.spacing_y,w,font.spacing_y)){
                                level_list_selection=i;
                            }
                        }
                    }

                    //Look through all of the buttons.
                    for(int i=0;i<buttons.size();i++){
                        //If this button is moused over,
                        //the mouse button has been released over it.
                        if(buttons[i].is_moused_over(mouse_x,mouse_y,x,y,-2)){
                            buttons[i].mouse_button_up(this);
                            buttons[i].reset_clicked();
                        }
                        //Otherwise, the mouse was not released over this button.
                        else{
                            buttons[i].reset_clicked();
                        }
                    }
                }
                break;
        }
    }
}

void Window_Setup_Survival::render(){
    if(on){
        //Render the border.
        render_rectangle(x,y,w,h,1.0,return_gui_color(holiday,0));

        //Render the background.
        if(background_image==NULL){
            render_rectangle(x+5,y+5,w-10,h-10,1.0,return_gui_color(holiday,1));
        }
        //Render the background image.
        else{
            short bg_w=w-10;
            short bg_h=h-10;

            SDL_Rect quickie;
            quickie.x=0;
            quickie.y=0;
            quickie.w=background_image->w;
            quickie.h=background_image->h;
            render_sprite(x+5,y+5,*background_image,&quickie,0.25,bg_w/background_image->w,bg_h/background_image->h);
        }

        //Render the title bar.
        render_rectangle(x+10,y+10,w-20,20,1.0,return_gui_color(holiday,2));

        //Display the window's title.
        font.show(x+(w-(title.length()*12))/2+2,y+12+2,title,COLOR_BLACK,1.0);
        font.show(x+(w-(title.length()*12))/2,y+12,title,return_gui_color(holiday,3),1.0);

        //Render the information pieces.
        for(int i=0;i<informations.size();i++){
            informations[i].render(x,y);
        }

        string time_string="Time until escape: "+get_time_string_long(player.survival_seconds_to_escape);
        font.show(x+(w-time_string.length()*font.spacing_x)/2+2,40+2,time_string,COLOR_BLACK);
        font.show(x+(w-time_string.length()*font.spacing_x)/2,40,time_string,return_gui_color(holiday,3));

        //Render level list background.
        render_rectangle(x+10,y+110,w-20,h-152,1.0,return_gui_color(holiday,0));
        render_rectangle(x+10+5,y+110+5,w-20-10,h-152-10,1.0,return_gui_color(holiday,1));

        for(int i=level_list_display_position,n=0;i<level_list_display_position+18;i++,n++){
            //If this is a valid level.
            if(i<levels.size()){
                bool selected=false;
                bool moused_over=false;

                //If the mouse is over this item, highlight it.
                int mouse_x,mouse_y;
                main_window.get_mouse_state(&mouse_x,&mouse_y);

                if(i==level_list_selection){
                    selected=true;
                }
                if(collision_check(mouse_x,mouse_y,1,1,0,70+font.spacing_y*2+n*font.spacing_y,w,font.spacing_y)){
                    moused_over=true;
                }

                if(selected && !moused_over){
                    render_rectangle(x+20,70+font.spacing_y*2+n*font.spacing_y,w-40,font.spacing_y,1.0,return_gui_color(holiday,0));
                    render_rectangle(x+21,1+70+font.spacing_y*2+n*font.spacing_y,w-40-2,font.spacing_y-2,1.0,return_gui_color(holiday,4));
                }
                else if(!selected && moused_over){
                    render_rectangle(x+20,70+font.spacing_y*2+n*font.spacing_y,w-40,font.spacing_y,1.0,return_gui_color(holiday,0));
                    render_rectangle(x+21,1+70+font.spacing_y*2+n*font.spacing_y,w-40-2,font.spacing_y-2,1.0,return_gui_color(holiday,1));
                }
                else if(selected && moused_over){
                    render_rectangle(x+20,70+font.spacing_y*2+n*font.spacing_y,w-40,font.spacing_y,1.0,return_gui_color(holiday,0));
                    render_rectangle(x+21,1+70+font.spacing_y*2+n*font.spacing_y,w-40-2,font.spacing_y-2,1.0,return_gui_color(holiday,2));
                }

                font.show(x+(w-levels[i].length()*font.spacing_x)/2+2,70+font.spacing_y*2+n*font.spacing_y+2,levels[i],COLOR_BLACK);
                font.show(x+(w-levels[i].length()*font.spacing_x)/2,70+font.spacing_y*2+n*font.spacing_y,levels[i],return_gui_color(holiday,3));
            }
        }

        //Render the buttons.
        for(int i=0;i<buttons.size();i++){
            buttons[i].render(x,y,i);
        }
    }
}
