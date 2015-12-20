/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "window_message.h"
#include "world.h"
#include "button_events.h"
#include "render.h"
#include "holidays.h"

using namespace std;

Window_Message::Window_Message(){
    background_image=NULL;

    x=0;
    y=0;
    w=main_window.SCREEN_WIDTH;
    h=main_window.SCREEN_HEIGHT;
    title="";

    on=false;
    moving=false;

    mouse_offset_x=0;
    mouse_offset_y=0;

    //Create the close button.
    create_button(w-23,5,"","X",&button_event_close_window,0,0,BUTTON_VISIBLE);

    //Create the OK button.
    create_button((w-22)/2,h-34,"OK","OK",&button_event_close_window,0,0,BUTTON_VISIBLE,0);
}

void Window_Message::turn_off(){
    on=false;

    //The window was just turned off, so stop its movement.
    moving=false;

    //Whenever a window is closed, we unpause the game.
    player.toggle_pause(false);

    player.set_firework_rate();

    if(player.game_mode==GAME_MODE_SP_ADVENTURE){
        if(player.game_beginning_cutscene>0){
            player.game_beginning_cutscene++;

            if(player.game_beginning_cutscene==2){
                set_message("","But there, the horizon, what could it be?\nOn closer inspection 'twas a ship on the sea!\nAnd soon it had landed, and what was on board?\nMice, many mice, and it seemed they meant war!");
                music.play_track(MUSIC_INTRO2);
            }
            else if(player.game_beginning_cutscene==3){
                set_message("","Sally was taken, with the leaves and the cheese,\nAnd then they were gone, like a quick fearsome breeze.\nBut Hubert's resolve was firm and unyielding:\nHe set off at once to recover what they were stealing.");
                music.play_track(MUSIC_INTRO3);
            }
            else if(player.game_beginning_cutscene==4){
                set_message("","And that, my dear friends, is where we are now.\nWithout further ado, to the Island with you!");
                music.play_track(MUSIC_INTRO4);
            }
            else if(player.game_beginning_cutscene==5){
                music.unload_track(MUSIC_INTRO1);
                music.unload_track(MUSIC_INTRO2);
                music.unload_track(MUSIC_INTRO3);
                music.unload_track(MUSIC_INTRO4);

                player.game_beginning_cutscene=0;
                hud_buttons[0].set_invisibility(true);
                hud_buttons[0].set_enabled(false);
                player.start_game();
            }
        }
        else if(player.new_game_plus_notification){
            player.new_game_plus_notification=false;

            set_message("","New Game + "+player.num_to_roman_numeral(player.new_game_plus)+"!");
        }
    }
    else if(player.game_mode==GAME_MODE_MP_ADVENTURE){
        if(player.game_beginning_cutscene>0){
            player.game_beginning_cutscene++;

            if(player.game_beginning_cutscene==2){
                set_message("","But there, the horizon, what could it be?\nOn closer inspection 'twas a ship on the sea!\nAnd soon it had landed, and what was on board?\nMice, many mice, and it seemed they meant war!");
                music.play_track(MUSIC_INTRO2);
            }
            else if(player.game_beginning_cutscene==3){
                set_message("","Sally was taken, with the leaves and the cheese,\nAnd then they were gone, like a quick fearsome breeze.\nBut Hubert's resolve was firm and unyielding:\nHe set off at once to recover what they were stealing.");
                music.play_track(MUSIC_INTRO3);
            }
            else if(player.game_beginning_cutscene==4){
                set_message("","And that, my dear friends, is where we are now.\nWithout further ado, to the Island with you!");
                music.play_track(MUSIC_INTRO4);
            }
            else if(player.game_beginning_cutscene==5){
                music.unload_track(MUSIC_INTRO1);
                music.unload_track(MUSIC_INTRO2);
                music.unload_track(MUSIC_INTRO3);
                music.unload_track(MUSIC_INTRO4);

                player.game_beginning_cutscene=0;
                hud_buttons[0].set_invisibility(true);
                hud_buttons[0].set_enabled(false);
                player.start_game();
            }
        }
        else if(player.mp_adventure_ending){
            player.end_game();
        }
    }
    else if(player.game_mode==GAME_MODE_SP_SURVIVAL || player.game_mode==GAME_MODE_MP_SURVIVAL){
        if(player.survival_complete){
            bool multiplayer=player.game_mode_is_multiplayer();

            player.no_game_in_progress();

            if(multiplayer){
                button_event_open_window_select_game_mode(NULL,0);
            }
            else{
                button_event_open_window_setup_sp_survival(NULL,0);
            }
        }
    }
}

void Window_Message::set_dimensions(string message,short font_type){
    short current_width=60;
    w=60;
    h=104;

    BitmapFont* ptr_font=NULL;
    if(font_type==0){
        ptr_font=&font;
    }
    else if(font_type==1){
        ptr_font=&font_sga;
    }

    for(int i=0;i<message.length();i++){
        if(message[i]!='\xA'){
            current_width+=ptr_font->spacing_x;
        }
        //If we hit a new line.
        else{
            h+=ptr_font->spacing_y;
            if(current_width>w){
                w=current_width;
            }
            current_width=60;
        }

        if(i==message.length()-1){
            if(current_width>w){
                w=current_width;
            }
        }
    }

    buttons[0].set_coordinates(w-23,5);
    buttons[1].set_coordinates((w-22)/2,h-34);
}

void Window_Message::set_message(string get_title,string message,short font_type){
    clear_information();

    create_information(15,35,"",message,0,NO_SPECIAL_INFO,0,return_gui_color(holiday,3),font_type);

    title=get_title;

    set_dimensions(message,font_type);

    x=(main_window.SCREEN_WIDTH-w)/2;
    y=(main_window.SCREEN_HEIGHT-h)/2;

    turn_on();
}

void Window_Message::handle_input_states(){
    if(on){
        int mouse_x,mouse_y;
        main_window.get_mouse_state(&mouse_x,&mouse_y);

        //If the window is moving, center it on the mouse's current position - the offsets.
        if(moving){
            x=mouse_x-mouse_offset_x;
            y=mouse_y-mouse_offset_y;

            //Don't allow the window to leave the screen.
            if(x<0){
                x=0;
            }
            if(y<0){
                y=0;
            }
            if(x+w>main_window.SCREEN_WIDTH){
                x=main_window.SCREEN_WIDTH-w;
            }
            if(y+h>main_window.SCREEN_HEIGHT){
                y=main_window.SCREEN_HEIGHT-h;
            }
        }

        //If the window is not moving.
        else if(!moving){
            //Check to see if the mouse is hovering over any information pieces.
            for(int i=0;i<informations.size();i++){
                informations[i].handle_input(mouse_x,mouse_y,x,y);
            }

            //Check to see if the mouse is hovering over any of this window's buttons.
            for(int i=0;i<buttons.size();i++){
                //For each button, reset its moused over state before anything else.
                //Remember whether or not the button was moused over before this reset.
                bool already_moused_over=buttons[i].reset_moused_over();

                //If the mouse is hovering over this button.
                if(buttons[i].is_moused_over(mouse_x,mouse_y,x,y,i)){
                    //The button is now being moused over.
                    buttons[i].mouse_over(already_moused_over);

                    //Setup the button's tooltip.
                    /**if(buttons[i].has_tooltip()){
                        tooltip.setup(buttons[i].return_tooltip_text(),mouse_x,mouse_y);
                    }*/

                    if(buttons[i].enabled){
                        player.current_button=i;
                    }
                }
            }
        }
    }
}
