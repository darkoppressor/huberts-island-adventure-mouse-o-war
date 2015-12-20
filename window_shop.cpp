/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "window_shop.h"
#include "button_events.h"
#include "world.h"
#include "holidays.h"
#include "render.h"
#include "collision.h"
#include "quit.h"

using namespace std;

Window_Shop::Window_Shop(short get_x,short get_y,short get_w,short get_h,string get_title){
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

    upgrade_list_display_position=0;
    upgrade_list_selection=0;

    //Create the close button.
    create_button(w-23,5,"","X",&button_event_close_window,0,0,BUTTON_VISIBLE,-1);

    short back_button_space=37;
    short button_sound=0;

    msg="Close Shoppe-O-Matic";
    button_sound=12-1;
    create_button(16,h-back_button_space,"",msg,&button_event_close_window,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Purchase Toy";
    button_sound=10-1;
    create_button(w-(msg.length()*font.spacing_x)-16,h-back_button_space,"",msg,&button_event_shop_buy,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);
}

void Window_Shop::setup(){
    profile.load_shop();

    //After loading the shop's upgrades, add any that are in the default list but not in the shop or our upgrades list.
    for(int i=0;i<default_shop_upgrades.size();i++){
        bool in_shop=false;
        for(int n=0;n<player.shop_upgrades.size();n++){
            if(player.shop_upgrades[n]==default_shop_upgrades[i]){
                in_shop=true;
                break;
            }
        }

        bool in_upgrades=false;
        for(map<string,bool>::iterator it=player.upgrades.begin();it!=player.upgrades.end();it++){
            if(it->first==default_shop_upgrades[i]){
                in_upgrades=true;
                break;
            }
        }

        if(!in_shop && !in_upgrades){
            player.shop_upgrades.push_back(default_shop_upgrades[i]);
        }
    }

    upgrades.clear();

    title="Shoppe-O-Matic";

    for(int i=0;i<player.shop_upgrades.size();i++){
        upgrades.push_back(player.shop_upgrades[i]);
    }

    if(upgrade_list_display_position>upgrades.size()-1){
        upgrade_list_display_position=upgrades.size()-1;
    }

    if(upgrade_list_selection>upgrades.size()-1){
        upgrade_list_selection=upgrades.size()-1;
    }
}

void Window_Shop::handle_input_events(){
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
                    if(upgrade_list_display_position<upgrades.size()-1){
                        upgrade_list_display_position++;
                    }
                }
                else if(event.wheel.y>0){
                    if(upgrade_list_display_position>0){
                        upgrade_list_display_position--;
                    }
                }
                break;

            case SDL_MOUSEBUTTONUP:
                if(event.button.button==SDL_BUTTON_LEFT){
                    //Stop moving the window.
                    moving=false;

                    for(int i=upgrade_list_display_position,n=0;i<upgrade_list_display_position+18;i++,n++){
                        //If this is a valid list item.
                        if(i<upgrades.size()){
                            if(collision_check(mouse_x,mouse_y,1,1,0,70+font.spacing_y*2+n*font.spacing_y,w,font.spacing_y)){
                                upgrade_list_selection=i;
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

void Window_Shop::render(){
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

        uint64_t i_cost=player.get_upgrade_cost(upgrades[upgrade_list_selection]);
        string cost="";
        if(i_cost==1){
            cost=player.num_to_string(i_cost)+" candy";
        }
        else if(i_cost==0){
            cost="Free";
        }
        else{
            cost=player.num_to_string(i_cost)+" candies";
        }
        string upgrade_string="Toy Price: "+cost;
        font.show(x+10+2,40+2,upgrade_string,COLOR_BLACK);
        font.show(x+10,40,upgrade_string,return_gui_color(holiday,3));

        upgrade_string="Current Candy: "+player.num_to_string(player.score);
        font.show(x+w-15-upgrade_string.length()*font.spacing_x+2,40+2,upgrade_string,COLOR_BLACK);
        font.show(x+w-15-upgrade_string.length()*font.spacing_x,40,upgrade_string,return_gui_color(holiday,3));

        upgrade_string="Effect: "+player.get_upgrade_description(upgrades[upgrade_list_selection]);
        font.show(x+(w-upgrade_string.length()*font.spacing_x)/2+2,70+2,upgrade_string,COLOR_BLACK);
        font.show(x+(w-upgrade_string.length()*font.spacing_x)/2,70,upgrade_string,return_gui_color(holiday,3));

        //Render list background.
        render_rectangle(x+10,y+110,w-20,h-152,1.0,return_gui_color(holiday,0));
        render_rectangle(x+10+5,y+110+5,w-20-10,h-152-10,1.0,return_gui_color(holiday,1));

        for(int i=upgrade_list_display_position,n=0;i<upgrade_list_display_position+18;i++,n++){
            //If this is a valid list item.
            if(i<upgrades.size()){
                bool selected=false;
                bool moused_over=false;

                //If the mouse is over this item, highlight it.
                int mouse_x,mouse_y;
                main_window.get_mouse_state(&mouse_x,&mouse_y);

                if(i==upgrade_list_selection){
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

                string upgrade=player.get_upgrade_name(upgrades[i]);

                font.show(x+(w-upgrade.length()*font.spacing_x)/2+2,70+font.spacing_y*2+n*font.spacing_y+2,upgrade,COLOR_BLACK);
                font.show(x+(w-upgrade.length()*font.spacing_x)/2,70+font.spacing_y*2+n*font.spacing_y,upgrade,return_gui_color(holiday,3));
            }
        }

        //Render the buttons.
        for(int i=0;i<buttons.size();i++){
            buttons[i].render(x,y,i);
        }
    }
}
