/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "door.h"
#include "render.h"
#include "world.h"
#include "mirror.h"

using namespace std;

Door::Door(double get_x,double get_y,short get_type,short get_number,bool get_open,short get_identifier){
    x=get_x;
    y=get_y;

    w=32;
    h=96;

    type=get_type;

    number=get_number;

    open=get_open;

    identifier=get_identifier;

    light_source.on=false;
    light_source.x=0;
    light_source.y=0;
    light_source.color=color_name_to_doubles(COLOR_WHITE);
    light_source.radius=6*(TILE_SIZE/LIGHTING_TILE_SIZE);
    light_source.dimness=0.0;
    light_source.falloff=0.0/(TILE_SIZE/LIGHTING_TILE_SIZE);

    glow_direction=true;

    if(type==DOOR_TYPE_LOCAL){
        frame=1;

        if(number==0){
            light_source.on=true;
            light_source.color=color_name_to_doubles(COLOR_RED);
        }
        else if(number==1){
            light_source.on=true;
            light_source.color=color_name_to_doubles(COLOR_GREEN);
        }
        else if(number==2){
            light_source.on=true;
            light_source.color=color_name_to_doubles(COLOR_BLUE);
        }
        else if(number==3){
            light_source.on=true;
            light_source.color=color_name_to_doubles(COLOR_YELLOW);
        }
    }
    else if(type==DOOR_TYPE_KEY){
        if(!open){
            frame=0;
        }
        else{
            frame=KEY_DOOR_SPRITES-1;
        }
    }
    else if(type==DOOR_TYPE_STANDARD){
        if(!open){
            frame=0;
        }
        else{
            frame=DOOR_STANDARD_SPRITES-1;
        }
    }
    frame_counter=0;
    frame_direction=random_range(0,1);
}

void Door::animate(){
    if(type==DOOR_TYPE_LOCAL && !open){
        //Increment the frame counter.
        frame_counter++;

        if(frame_counter>=5){
            frame_counter=0;

            //Now increment or decrement the frame.
            if(frame_direction){
                frame++;
            }
            else{
                frame--;
            }

            //If the frame has reached the maximum or minimum, switch directions.
            if(frame>DOOR_SPRITES-1){
                frame=DOOR_SPRITES-1;
                frame_direction=!frame_direction;
            }
            else if(frame<1){
                frame=1;
                frame_direction=!frame_direction;
            }
        }
    }
    else if(type==DOOR_TYPE_KEY && open && frame<KEY_DOOR_SPRITES-1){
        frame_counter++;

        if(frame_counter>=10){
            frame_counter=0;

            frame++;
        }
    }
    else if(type==DOOR_TYPE_STANDARD && open && frame<DOOR_STANDARD_SPRITES-1){
        frame_counter++;

        if(frame_counter>=5){
            frame_counter=0;

            frame++;
        }
    }

    if(light_source.on && !open){
        double sprite_rate=((DOOR_SPRITES-1)*5);
        if(glow_direction){
            light_source.falloff+=1.0/sprite_rate;
            light_source.dimness+=1.0/sprite_rate;
            if(light_source.falloff>0.2){
                light_source.falloff=0.2;
            }
            if(light_source.dimness>1.0){
                light_source.dimness=1.0;
                glow_direction=!glow_direction;
            }
        }
        else{
            light_source.falloff-=1.0/sprite_rate;
            light_source.dimness-=1.0/sprite_rate;
            if(light_source.falloff<0.1){
                light_source.falloff=0.1;
            }
            if(light_source.dimness<0.0){
                light_source.dimness=0.0;
                glow_direction=!glow_direction;
            }
        }
    }
}

void Door::render(bool mirrored){
    //If the door is in camera bounds, render it.
    if(x>=player.camera_x-w && x<=player.camera_x+player.camera_w && y>=player.camera_y-h && y<=player.camera_y+player.camera_h){
        double render_x=x;
        double render_y=y;
        if(mirrored && touching_mirror(x+MIRROR_DISTANCE_X,y+MIRROR_DISTANCE_Y,w,h)){
            render_x+=MIRROR_DISTANCE_X;
            render_y+=MIRROR_DISTANCE_Y;
        }

        if(type==DOOR_TYPE_LOCAL){
            //If the door is open.
            if(open){
                render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),image.sprite_sheet_doors,&sprites_doors[0][number]);
            }
            //If the door is closed.
            else{
                render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),image.sprite_sheet_doors,&sprites_doors[frame][number]);
            }
        }
        else if(type==DOOR_TYPE_KEY){
            render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),image.sprite_sheet_doors,&sprites_key_doors[frame][number]);
        }
        else if(type==DOOR_TYPE_STANDARD){
            render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),image.sprite_sheet_door_standard,&sprites_door_standard[frame]);
        }
    }
}

void Door::toggle_open(){
    open=!open;

    if(type==DOOR_TYPE_LOCAL){
        if(open){
            play_positional_sound(sound_system.door_open,x,y);
        }
        else{
            play_positional_sound(sound_system.door_close,x,y);
        }
    }
    else if(type==DOOR_TYPE_KEY){
        if(open){
            play_positional_sound(sound_system.door_unlock,x,y);
        }
        else{
            ///play_positional_sound(sound_system.door_close,x,y);
        }
    }
    else if(type==DOOR_TYPE_STANDARD){
        if(open){
            play_positional_sound(sound_system.door_standard_open,x,y);
        }
        else{
            ///play_positional_sound(sound_system.door_standard_close,x,y);
        }
    }
}

bool Door::attempt_unlock(){
    if(type==DOOR_TYPE_KEY && !open){
        //Check to see if the player has a key for this door.
        //If he does, use a key of this type to open the door.

        if(number==0 && player.check_inventory(ITEM_KEY_RED)){
            toggle_open();
            player.remove_item(ITEM_KEY_RED);
            return true;
        }
        else if(number==1 && player.check_inventory(ITEM_KEY_BLUE)){
            toggle_open();
            player.remove_item(ITEM_KEY_BLUE);
            return true;
        }
        else if(number==2 && player.check_inventory(ITEM_KEY_GREEN)){
            toggle_open();
            player.remove_item(ITEM_KEY_GREEN);
            return true;
        }
        else if(number==3 && player.check_inventory(ITEM_KEY_YELLOW)){
            toggle_open();
            player.remove_item(ITEM_KEY_YELLOW);
            return true;
        }
        else if(number==4 && player.check_inventory(ITEM_KEY_ORANGE)){
            toggle_open();
            player.remove_item(ITEM_KEY_ORANGE);
            return true;
        }
        else if(number==5 && player.check_inventory(ITEM_KEY_PURPLE)){
            toggle_open();
            player.remove_item(ITEM_KEY_PURPLE);
            return true;
        }
        else if(number==6 && player.check_inventory(ITEM_TOWEL)){
            toggle_open();
            player.remove_item(ITEM_TOWEL);
            return true;
        }
        else if(number==7 && player.check_inventory(ITEM_KEY_GRAY)){
            toggle_open();
            player.remove_item(ITEM_KEY_GRAY);
            return true;
        }
        else if(number==8 && player.check_inventory(ITEM_KEY_BROWN)){
            toggle_open();
            player.remove_item(ITEM_KEY_BROWN);
            return true;
        }
        else if(number==9 && player.check_inventory(ITEM_KEY_BLACK)){
            toggle_open();
            player.remove_item(ITEM_KEY_BLACK);
            return true;
        }
        else if(number==10 && player.check_inventory(ITEM_KEY_PINK)){
            toggle_open();
            player.remove_item(ITEM_KEY_PINK);
            return true;
        }
        else if(number==11 && player.check_inventory(ITEM_KEY_CYAN)){
            toggle_open();
            player.remove_item(ITEM_KEY_CYAN);
            return true;
        }
    }
    else if(type==DOOR_TYPE_STANDARD && !open){
        toggle_open();
        return true;
    }

    return false;
}
