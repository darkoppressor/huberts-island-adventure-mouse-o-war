/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "window_inventory.h"
#include "world.h"
#include "render.h"
#include "collision.h"
#include "quit.h"
#include "button_events.h"
#include "holidays.h"

using namespace std;

Window_Inventory::Window_Inventory(short get_x,short get_y,short get_w,short get_h,string get_title){
    background_image=NULL;

    x=get_x;
    y=get_y;
    w=get_w;
    h=get_h;
    title=get_title;

    on=false;
    moving=false;

    mouse_offset_x=0;
    mouse_offset_y=0;

    //Slots:
    for(short slot_x=0;slot_x<10;slot_x++){
        for(short slot_y=0;slot_y<10;slot_y++){
            slots[slot_x][slot_y].x=3+36*slot_x;
            slots[slot_x][slot_y].y=39+36*slot_y;
            slots[slot_x][slot_y].slot=slot_x+slot_y*10;
        }
    }
    slot_w=32;
    slot_h=32;

    INVENTORY_MAX_SIZE=100;

    //Create the close button.
    create_button(w-23,5,"","X",&button_event_close_window,0,0,BUTTON_VISIBLE);
}

void Window_Inventory::toggle_on(){
    on=!on;

    //If the window was just turned off, stop its movement and drop any dragged item.
    if(!on){
        moving=false;

        //Whenever a window is closed, we unpause the game.
        player.toggle_pause(false);

        drop_dragged_item();
    }
    //If the window was just turned on.
    else{
        if(buttons.size()>1){
            player.current_button=1;
        }
        else{
            player.current_button=0;
        }
        window_manager.allow_button_sound=false;

        //Whenever a window is opened, we pause the game.
        player.toggle_pause(true);
    }
}

void Window_Inventory::turn_off(){
    on=false;

    //The window was just turned off, so stop its movement and drop any dragged item.
    moving=false;

    //Whenever a window is closed, we unpause the game.
    player.toggle_pause(false);

    drop_dragged_item();
}

void Window_Inventory::handle_input_states(){
    if(on){
        int mouse_x,mouse_y;
        main_window.get_mouse_state(&mouse_x,&mouse_y);

        //If the window is moving, center it on the mouse's current position - the offsets.
        if(moving){
            if(player.mouse_allowed()){
                x=mouse_x-mouse_offset_x;
                y=mouse_y-mouse_offset_y;

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
        }

        else if(!moving){
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
                    if(buttons[i].has_tooltip()){
                        tooltip.setup(buttons[i].return_tooltip_text(),mouse_x,mouse_y);
                    }

                    if(buttons[i].enabled){
                        player.current_button=i;
                    }
                }
            }

            //Determine which slot the mouse is hovering over, if any.
            moused_slot m_slot=determine_moused_slot(mouse_x,mouse_y);
            short slot_hover=m_slot.slot;

            //If the mouse is currently hovering over a slot.
            if(slot_hover!=-1){
                for(int i=0;i<player.inventory.size();i++){
                    if(player.inventory[i].slot==slot_hover){
                        msg=player.inventory[i].name;
                        tooltip.setup(msg,mouse_x,mouse_y);
                    }
                }
            }
        }
    }
}

void Window_Inventory::handle_input_events(){
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

                    //Begin dragging an item.
                    else if(!button_clicked && player.dragged_item.size()==0 && collision_check(mouse_x,mouse_y,2,2,x,y,w,h)){
                        //Determine which slot was clicked, if any.
                        moused_slot m_slot=determine_moused_slot(mouse_x,mouse_y);
                        short slot_clicked=m_slot.slot;

                        //If a slot was clicked.
                        if(slot_clicked!=-1){
                            for(int i=0;i<player.inventory.size();i++){
                                if(player.inventory[i].slot==slot_clicked){
                                    //Set the dragged item to this item.
                                    player.dragged_item.push_back(dragged_inventory_item());
                                    player.dragged_item[0].type=player.inventory[i].type;
                                    player.dragged_item[0].slot=player.inventory[i].slot;
                                    player.dragged_item[0].name=player.inventory[i].name;
                                    player.dragged_item[0].offset_x=m_slot.offset_x;
                                    player.dragged_item[0].offset_y=m_slot.offset_y;

                                    //Set this item to slot -1, meaning it is being dragged and is not to be displayed in any inventory window slot.
                                    player.inventory[i].slot=-1;

                                    //Play the inventory grab sound.
                                    play_positional_sound(sound_system.inventory_grab);
                                }
                            }
                        }
                    }
                }
                break;

            case SDL_MOUSEBUTTONUP:
                if(event.button.button==SDL_BUTTON_LEFT){
                    //Stop moving the inventory window.
                    moving=false;

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

                    //If we were dragging an item.
                    if(player.dragged_item.size()>0){
                        //Determine which slot is being hovered over, if any.
                        moused_slot m_slot=determine_moused_slot(mouse_x,mouse_y);
                        short slot_hover=m_slot.slot;

                        //If a slot is hovered over.
                        if(slot_hover!=-1){
                            bool slot_occupied=false;

                            for(int i=0;i<player.inventory.size();i++){
                                if(player.inventory[i].slot==slot_hover){
                                    slot_occupied=true;
                                    break;
                                }
                            }

                            //If the slot is empty.
                            if(!slot_occupied){
                                //Find the dragged item (the item with a slot of -1).
                                for(int i=0;i<player.inventory.size();i++){
                                    //Ok, we've found the dragged item in the inventory.
                                    if(player.inventory[i].slot==-1){
                                        //Set this item's slot to the hovered slot.
                                        player.inventory[i].slot=slot_hover;
                                        //Clear the dragged item vector.
                                        player.dragged_item.clear();

                                        //Play the inventory drop sound.
                                        play_positional_sound(sound_system.inventory_drop);

                                        //Increment the items moved stat.
                                        player.stat_items_moved++;

                                        break;
                                    }
                                }
                            }
                            //If the slot is occupied, trade items.
                            else{
                                //Find the item currently occupying this slot.
                                for(int i=0;i<player.inventory.size();i++){
                                    //Ok, we've found the occupying item in the inventory.
                                    if(player.inventory[i].slot==slot_hover){
                                        //Set this item's slot to the dragged item's old slot.
                                        player.inventory[i].slot=player.dragged_item[0].slot;
                                        break;
                                    }
                                }

                                //Find the dragged item (the item with a slot of -1).
                                for(int i=0;i<player.inventory.size();i++){
                                    //Ok, we've found the dragged item in the inventory.
                                    if(player.inventory[i].slot==-1){
                                        //Set this item's slot to the hovered slot.
                                        player.inventory[i].slot=slot_hover;
                                        //Clear the dragged item vector.
                                        player.dragged_item.clear();

                                        //Play the inventory trade sound.
                                        play_positional_sound(sound_system.inventory_trade);

                                        //Increment the items moved stat.
                                        player.stat_items_moved++;

                                        break;
                                    }
                                }
                            }
                        }
                        //If no slot is hovered over.
                        else{
                            //Find the dragged item (the item with a slot of -1).
                            for(int i=0;i<player.inventory.size();i++){
                                //Ok, we've found the dragged item in the inventory.
                                if(player.inventory[i].slot==-1){
                                    //Set the item's slot back to its original slot.
                                    player.inventory[i].slot=player.dragged_item[0].slot;
                                    //Clear the dragged item vector.
                                    player.dragged_item.clear();

                                    //Play the inventory drop sound.
                                    play_positional_sound(sound_system.inventory_drop);

                                    break;
                                }
                            }
                        }
                    }
                }
                break;
        }
    }
}

void Window_Inventory::display_inventory_item(short type,short slot){
    //If slot==-1, the item is not currently in any inventory window slot, and is instead being dragged.
    if(slot!=-1){
        short slot_x,slot_y;

        slot_x=x;
        slot_y=y;

        if(slot>=0 && slot<=9){
            slot_y+=39;
        }
        else if(slot>=10 && slot<=19){
            slot_y+=39+36;
            slot-=10;
        }
        else if(slot>=20 && slot<=29){
            slot_y+=39+36*2;
            slot-=20;
        }
        else if(slot>=30 && slot<=39){
            slot_y+=39+36*3;
            slot-=30;
        }
        else if(slot>=40 && slot<=49){
            slot_y+=39+36*4;
            slot-=40;
        }
        else if(slot>=50 && slot<=59){
            slot_y+=39+36*5;
            slot-=50;
        }
        else if(slot>=60 && slot<=69){
            slot_y+=39+36*6;
            slot-=60;
        }
        else if(slot>=70 && slot<=79){
            slot_y+=39+36*7;
            slot-=70;
        }
        else if(slot>=80 && slot<=89){
            slot_y+=39+36*8;
            slot-=80;
        }
        else if(slot>=90 && slot<=99){
            slot_y+=39+36*9;
            slot-=90;
        }

        slot_x+=3+(36*slot);

        if(type==ITEM_SWIMMING_GEAR){
            render_sprite(slot_x,slot_y,image.sprite_sheet_items,&sprites_item_swimming_gear[0]);
        }
        else if(type==ITEM_KEY_RED){
            render_sprite(slot_x,slot_y,image.sprite_sheet_items,&sprites_item_key_red[0]);
        }
        else if(type==ITEM_KEY_BLUE){
            render_sprite(slot_x,slot_y,image.sprite_sheet_items,&sprites_item_key_blue[0]);
        }
        else if(type==ITEM_KEY_GREEN){
            render_sprite(slot_x,slot_y,image.sprite_sheet_items,&sprites_item_key_green[0]);
        }
        else if(type==ITEM_KEY_YELLOW){
            render_sprite(slot_x,slot_y,image.sprite_sheet_items,&sprites_item_key_yellow[0]);
        }
        else if(type==ITEM_KEY_ORANGE){
            render_sprite(slot_x,slot_y,image.sprite_sheet_items,&sprites_item_key_orange[0]);
        }
        else if(type==ITEM_KEY_PURPLE){
            render_sprite(slot_x,slot_y,image.sprite_sheet_items,&sprites_item_key_purple[0]);
        }
        else if(type==ITEM_TOWEL){
            render_sprite(slot_x,slot_y,image.sprite_sheet_items,&sprites_item_towel[0]);
        }
        else if(type==ITEM_KEY_GRAY){
            render_sprite(slot_x,slot_y,image.sprite_sheet_items,&sprites_item_key_gray[0]);
        }
        else if(type==ITEM_KEY_BROWN){
            render_sprite(slot_x,slot_y,image.sprite_sheet_items,&sprites_item_key_brown[0]);
        }
        else if(type==ITEM_KEY_BLACK){
            render_sprite(slot_x,slot_y,image.sprite_sheet_items,&sprites_item_key_black[0]);
        }
        else if(type==ITEM_KEY_PINK){
            render_sprite(slot_x,slot_y,image.sprite_sheet_items,&sprites_item_key_pink[0]);
        }
        else if(type==ITEM_KEY_CYAN){
            render_sprite(slot_x,slot_y,image.sprite_sheet_items,&sprites_item_key_cyan[0]);
        }
        else if(type==ITEM_SINK){
            render_sprite(slot_x,slot_y,image.sprite_sheet_items,&sprites_item_sink[0]);
        }
        else if(type==ITEM_SUIT_DEADLY_WATER){
            render_sprite(slot_x,slot_y,image.sprite_sheet_items,&sprites_item_suit_deadly_water[0]);
        }
        else if(type==ITEM_SUIT_SHARP){
            render_sprite(slot_x,slot_y,image.sprite_sheet_items,&sprites_item_suit_sharp[0]);
        }
        else if(type==ITEM_SUIT_BANANA){
            render_sprite(slot_x,slot_y,image.sprite_sheet_items,&sprites_item_suit_banana[0]);
        }
        else if(type==ITEM_SHOT_HOMING){
            render_sprite(slot_x,slot_y,image.sprite_sheet_items,&sprites_item_shot_homing[0]);
        }
        else if(type==ITEM_TRANSLATOR){
            render_sprite(slot_x,slot_y,image.sprite_sheet_items,&sprites_item_translator[0]);
        }
        else if(type==ITEM_J_WING){
            render_sprite(slot_x,slot_y,image.sprite_sheet_items,&sprites_item_j_wing[0]);
        }
    }
}

void Window_Inventory::display_dragged_item(){
    //If there is a dragged item.
    if(player.dragged_item.size()>0){
        int mouse_x,mouse_y;
        main_window.get_mouse_state(&mouse_x,&mouse_y);

        if(player.dragged_item[0].type==ITEM_SWIMMING_GEAR){
            render_sprite(mouse_x-player.dragged_item[0].offset_x+2,mouse_y-player.dragged_item[0].offset_y+2,image.sprite_sheet_items,&sprites_item_swimming_gear[0],1.0,1.0,1.0,0.0,COLOR_BLACK);
            render_sprite(mouse_x-player.dragged_item[0].offset_x,mouse_y-player.dragged_item[0].offset_y,image.sprite_sheet_items,&sprites_item_swimming_gear[0]);
        }
        else if(player.dragged_item[0].type==ITEM_KEY_RED){
            render_sprite(mouse_x-player.dragged_item[0].offset_x+2,mouse_y-player.dragged_item[0].offset_y+2,image.sprite_sheet_items,&sprites_item_key_red[0],1.0,1.0,1.0,0.0,COLOR_BLACK);
            render_sprite(mouse_x-player.dragged_item[0].offset_x,mouse_y-player.dragged_item[0].offset_y,image.sprite_sheet_items,&sprites_item_key_red[0]);
        }
        else if(player.dragged_item[0].type==ITEM_KEY_BLUE){
            render_sprite(mouse_x-player.dragged_item[0].offset_x+2,mouse_y-player.dragged_item[0].offset_y+2,image.sprite_sheet_items,&sprites_item_key_blue[0],1.0,1.0,1.0,0.0,COLOR_BLACK);
            render_sprite(mouse_x-player.dragged_item[0].offset_x,mouse_y-player.dragged_item[0].offset_y,image.sprite_sheet_items,&sprites_item_key_blue[0]);
        }
        else if(player.dragged_item[0].type==ITEM_KEY_GREEN){
            render_sprite(mouse_x-player.dragged_item[0].offset_x+2,mouse_y-player.dragged_item[0].offset_y+2,image.sprite_sheet_items,&sprites_item_key_green[0],1.0,1.0,1.0,0.0,COLOR_BLACK);
            render_sprite(mouse_x-player.dragged_item[0].offset_x,mouse_y-player.dragged_item[0].offset_y,image.sprite_sheet_items,&sprites_item_key_green[0]);
        }
        else if(player.dragged_item[0].type==ITEM_KEY_YELLOW){
            render_sprite(mouse_x-player.dragged_item[0].offset_x+2,mouse_y-player.dragged_item[0].offset_y+2,image.sprite_sheet_items,&sprites_item_key_yellow[0],1.0,1.0,1.0,0.0,COLOR_BLACK);
            render_sprite(mouse_x-player.dragged_item[0].offset_x,mouse_y-player.dragged_item[0].offset_y,image.sprite_sheet_items,&sprites_item_key_yellow[0]);
        }
        else if(player.dragged_item[0].type==ITEM_KEY_ORANGE){
            render_sprite(mouse_x-player.dragged_item[0].offset_x+2,mouse_y-player.dragged_item[0].offset_y+2,image.sprite_sheet_items,&sprites_item_key_orange[0],1.0,1.0,1.0,0.0,COLOR_BLACK);
            render_sprite(mouse_x-player.dragged_item[0].offset_x,mouse_y-player.dragged_item[0].offset_y,image.sprite_sheet_items,&sprites_item_key_orange[0]);
        }
        else if(player.dragged_item[0].type==ITEM_KEY_PURPLE){
            render_sprite(mouse_x-player.dragged_item[0].offset_x+2,mouse_y-player.dragged_item[0].offset_y+2,image.sprite_sheet_items,&sprites_item_key_purple[0],1.0,1.0,1.0,0.0,COLOR_BLACK);
            render_sprite(mouse_x-player.dragged_item[0].offset_x,mouse_y-player.dragged_item[0].offset_y,image.sprite_sheet_items,&sprites_item_key_purple[0]);
        }
        else if(player.dragged_item[0].type==ITEM_TOWEL){
            render_sprite(mouse_x-player.dragged_item[0].offset_x+2,mouse_y-player.dragged_item[0].offset_y+2,image.sprite_sheet_items,&sprites_item_towel[0],1.0,1.0,1.0,0.0,COLOR_BLACK);
            render_sprite(mouse_x-player.dragged_item[0].offset_x,mouse_y-player.dragged_item[0].offset_y,image.sprite_sheet_items,&sprites_item_towel[0]);
        }
        else if(player.dragged_item[0].type==ITEM_KEY_GRAY){
            render_sprite(mouse_x-player.dragged_item[0].offset_x+2,mouse_y-player.dragged_item[0].offset_y+2,image.sprite_sheet_items,&sprites_item_key_gray[0],1.0,1.0,1.0,0.0,COLOR_BLACK);
            render_sprite(mouse_x-player.dragged_item[0].offset_x,mouse_y-player.dragged_item[0].offset_y,image.sprite_sheet_items,&sprites_item_key_gray[0]);
        }
        else if(player.dragged_item[0].type==ITEM_KEY_BROWN){
            render_sprite(mouse_x-player.dragged_item[0].offset_x+2,mouse_y-player.dragged_item[0].offset_y+2,image.sprite_sheet_items,&sprites_item_key_brown[0],1.0,1.0,1.0,0.0,COLOR_BLACK);
            render_sprite(mouse_x-player.dragged_item[0].offset_x,mouse_y-player.dragged_item[0].offset_y,image.sprite_sheet_items,&sprites_item_key_brown[0]);
        }
        else if(player.dragged_item[0].type==ITEM_KEY_BLACK){
            render_sprite(mouse_x-player.dragged_item[0].offset_x+2,mouse_y-player.dragged_item[0].offset_y+2,image.sprite_sheet_items,&sprites_item_key_black[0],1.0,1.0,1.0,0.0,COLOR_BLACK);
            render_sprite(mouse_x-player.dragged_item[0].offset_x,mouse_y-player.dragged_item[0].offset_y,image.sprite_sheet_items,&sprites_item_key_black[0]);
        }
        else if(player.dragged_item[0].type==ITEM_KEY_PINK){
            render_sprite(mouse_x-player.dragged_item[0].offset_x+2,mouse_y-player.dragged_item[0].offset_y+2,image.sprite_sheet_items,&sprites_item_key_pink[0],1.0,1.0,1.0,0.0,COLOR_BLACK);
            render_sprite(mouse_x-player.dragged_item[0].offset_x,mouse_y-player.dragged_item[0].offset_y,image.sprite_sheet_items,&sprites_item_key_pink[0]);
        }
        else if(player.dragged_item[0].type==ITEM_KEY_CYAN){
            render_sprite(mouse_x-player.dragged_item[0].offset_x+2,mouse_y-player.dragged_item[0].offset_y+2,image.sprite_sheet_items,&sprites_item_key_cyan[0],1.0,1.0,1.0,0.0,COLOR_BLACK);
            render_sprite(mouse_x-player.dragged_item[0].offset_x,mouse_y-player.dragged_item[0].offset_y,image.sprite_sheet_items,&sprites_item_key_cyan[0]);
        }
        else if(player.dragged_item[0].type==ITEM_SINK){
            render_sprite(mouse_x-player.dragged_item[0].offset_x+2,mouse_y-player.dragged_item[0].offset_y+2,image.sprite_sheet_items,&sprites_item_sink[0],1.0,1.0,1.0,0.0,COLOR_BLACK);
            render_sprite(mouse_x-player.dragged_item[0].offset_x,mouse_y-player.dragged_item[0].offset_y,image.sprite_sheet_items,&sprites_item_sink[0]);
        }
        else if(player.dragged_item[0].type==ITEM_SUIT_DEADLY_WATER){
            render_sprite(mouse_x-player.dragged_item[0].offset_x+2,mouse_y-player.dragged_item[0].offset_y+2,image.sprite_sheet_items,&sprites_item_suit_deadly_water[0],1.0,1.0,1.0,0.0,COLOR_BLACK);
            render_sprite(mouse_x-player.dragged_item[0].offset_x,mouse_y-player.dragged_item[0].offset_y,image.sprite_sheet_items,&sprites_item_suit_deadly_water[0]);
        }
        else if(player.dragged_item[0].type==ITEM_SUIT_SHARP){
            render_sprite(mouse_x-player.dragged_item[0].offset_x+2,mouse_y-player.dragged_item[0].offset_y+2,image.sprite_sheet_items,&sprites_item_suit_sharp[0],1.0,1.0,1.0,0.0,COLOR_BLACK);
            render_sprite(mouse_x-player.dragged_item[0].offset_x,mouse_y-player.dragged_item[0].offset_y,image.sprite_sheet_items,&sprites_item_suit_sharp[0]);
        }
        else if(player.dragged_item[0].type==ITEM_SUIT_BANANA){
            render_sprite(mouse_x-player.dragged_item[0].offset_x+2,mouse_y-player.dragged_item[0].offset_y+2,image.sprite_sheet_items,&sprites_item_suit_banana[0],1.0,1.0,1.0,0.0,COLOR_BLACK);
            render_sprite(mouse_x-player.dragged_item[0].offset_x,mouse_y-player.dragged_item[0].offset_y,image.sprite_sheet_items,&sprites_item_suit_banana[0]);
        }
        else if(player.dragged_item[0].type==ITEM_SHOT_HOMING){
            render_sprite(mouse_x-player.dragged_item[0].offset_x+2,mouse_y-player.dragged_item[0].offset_y+2,image.sprite_sheet_items,&sprites_item_shot_homing[0],1.0,1.0,1.0,0.0,COLOR_BLACK);
            render_sprite(mouse_x-player.dragged_item[0].offset_x,mouse_y-player.dragged_item[0].offset_y,image.sprite_sheet_items,&sprites_item_shot_homing[0]);
        }
        else if(player.dragged_item[0].type==ITEM_TRANSLATOR){
            render_sprite(mouse_x-player.dragged_item[0].offset_x+2,mouse_y-player.dragged_item[0].offset_y+2,image.sprite_sheet_items,&sprites_item_translator[0],1.0,1.0,1.0,0.0,COLOR_BLACK);
            render_sprite(mouse_x-player.dragged_item[0].offset_x,mouse_y-player.dragged_item[0].offset_y,image.sprite_sheet_items,&sprites_item_translator[0]);
        }
        else if(player.dragged_item[0].type==ITEM_J_WING){
            render_sprite(mouse_x-player.dragged_item[0].offset_x+2,mouse_y-player.dragged_item[0].offset_y+2,image.sprite_sheet_items,&sprites_item_j_wing[0],1.0,1.0,1.0,0.0,COLOR_BLACK);
            render_sprite(mouse_x-player.dragged_item[0].offset_x,mouse_y-player.dragged_item[0].offset_y,image.sprite_sheet_items,&sprites_item_j_wing[0]);
        }
    }
}

void Window_Inventory::drop_dragged_item(){
    //If there is a dragged item.
    if(player.dragged_item.size()>0){
        //Find the dragged item (the item with a slot of -1).
        for(int i=0;i<player.inventory.size();i++){
            //Ok, we've found the dragged item in the inventory.
            if(player.inventory[i].slot==-1){
                //Set the item's slot back to its original slot.
                player.inventory[i].slot=player.dragged_item[0].slot;
                //Clear the dragged item vector.
                player.dragged_item.clear();

                //Play the inventory drop sound.
                play_positional_sound(sound_system.inventory_drop);

                break;
            }
        }
    }
}

moused_slot Window_Inventory::determine_moused_slot(int mouse_x,int mouse_y){
    moused_slot m_slot;
    m_slot.slot=-1;
    m_slot.offset_x=0;
    m_slot.offset_y=0;

    for(short slot_x=0;slot_x<10;slot_x++){
        for(short slot_y=0;slot_y<10;slot_y++){
            if(collision_check(mouse_x,mouse_y,2,2,x+slots[slot_x][slot_y].x,y+slots[slot_x][slot_y].y,slot_w,slot_h)){
                m_slot.slot=slots[slot_x][slot_y].slot;
                m_slot.offset_x=mouse_x-slots[slot_x][slot_y].x-x;
                m_slot.offset_y=mouse_y-slots[slot_x][slot_y].y-y;
                return m_slot;
            }
        }
    }

    //If no slot is being moused over.
    return m_slot;
}

void Window_Inventory::render(){
    if(on){
        //Render the border.
        render_rectangle(x,y,w,h,1.0,return_gui_color(holiday,0));

        //Render the inventory slots.
        for(int int_x=0;int_x<10;int_x++){
            for(int int_y=0;int_y<10;int_y++){
                render_rectangle(x+3+int_x*36,y+39+int_y*36,32,32,1.0,return_gui_color(holiday,1));
            }
        }

        //Render the title bar.
        render_rectangle(x+10,y+10,w-20,20,1.0,return_gui_color(holiday,2));

        //Display the window's title.
        font.show(x+(w-(title.length()*12))/2+2,y+12+2,title,COLOR_BLACK,1.0);
        font.show(x+(w-(title.length()*12))/2,y+12,title,return_gui_color(holiday,3),1.0);

        //Show items.
        for(int i=0;i<player.inventory.size();i++){
            display_inventory_item(player.inventory[i].type,player.inventory[i].slot);
        }

        //Render the buttons.
        for(int i=0;i<buttons.size();i++){
            buttons[i].render(x,y,i);
        }

        //Show the dragged item, if there is one.
        display_dragged_item();
    }
}
