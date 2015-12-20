/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "hud_buttons.h"
#include "world.h"
#include "quit.h"

using namespace std;

void hud_buttons_handle_input_states(){
    int mouse_x,mouse_y;
    main_window.get_mouse_state(&mouse_x,&mouse_y);

    //Check to see if the mouse is hovering over any of the hud buttons.
    for(int i=0;i<hud_buttons.size();i++){
        //For each button, reset its moused over state before anything else.
        //Remember whether or not the button was moused over before this reset.
        bool already_moused_over=hud_buttons[i].reset_moused_over();

        //If the mouse is hovering over this button.
        if(hud_buttons[i].is_moused_over(mouse_x,mouse_y,0,0,i)){
            //The button is now being moused over.
            hud_buttons[i].mouse_over(already_moused_over);

            //Setup the button's tooltip.
            tooltip.setup(hud_buttons[i].return_tooltip_text(),mouse_x,mouse_y);

            player.current_button=i;
        }
    }
}

void hud_buttons_handle_input_events(){
    int mouse_x,mouse_y;
    main_window.get_mouse_state(&mouse_x,&mouse_y);

    switch(event.type){
        case SDL_QUIT:
            quit_game();
            break;

        case SDL_MOUSEBUTTONDOWN:
            if(event.button.button==SDL_BUTTON_LEFT){
                bool button_clicked=false;

                //Look through all of the hud buttons.
                for(int i=0;i<hud_buttons.size();i++){
                    //If this button is moused over,
                    //it has been clicked down on.
                    if(hud_buttons[i].is_moused_over(mouse_x,mouse_y,0,0,-2)){
                        hud_buttons[i].mouse_button_down();
                        //A button has just been clicked, so we keep that in mind.
                        button_clicked=true;
                    }
                }
            }
            break;

        case SDL_MOUSEBUTTONUP:
            if(event.button.button==SDL_BUTTON_LEFT){
                //Look through all of the hud buttons.
                for(int i=0;i<hud_buttons.size();i++){
                    //If this button is moused over,
                    //the mouse button has been released over it.
                    if(hud_buttons[i].is_moused_over(mouse_x,mouse_y,0,0,-2)){
                        hud_buttons[i].mouse_button_up(0);
                        hud_buttons[i].reset_clicked();
                    }
                    //Otherwise, the mouse was not released over this button.
                    else{
                        hud_buttons[i].reset_clicked();
                    }
                }
            }
            break;
    }
}
