/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "trigger.h"
#include "render.h"
#include "world.h"
#include "collision.h"
#include "mirror.h"

using namespace std;

Trigger::Trigger(double get_x,double get_y,double get_w,double get_h,vector<target> get_targets,bool get_trigger_method,short get_user_type,bool get_repeating,short get_repeat_time,short get_render_trigger){
    x=get_x;
    y=get_y;

    w=get_w;
    h=get_h;

    active=true;

    targets=get_targets;

    trigger_method=get_trigger_method;

    user_type=get_user_type;

    repeating=get_repeating;

    repeat_time=get_repeat_time;
    repeat_current=0;

    render_trigger=get_render_trigger;

    frame=0;
    frame_counter=0;
}

void Trigger::use(){
    //Loop through all of the targets.
    for(int n=0;n<targets.size();n++){
        //If the target is a moving platform.
        if(targets[n].type==0){
            //Loop through all of the moving platforms.
            for(int i=0;i<vector_moving_platforms.size();i++){
                //If the moving platform is being targeted by this trigger.
                if(vector_moving_platforms[i].identifier==targets[n].identifier){
                    vector_moving_platforms[i].active=!vector_moving_platforms[i].active;
                }
            }
        }

        //If the target is a door.
        else if(targets[n].type==1){
            //Loop through all of the doors.
            for(int i=0;i<vector_doors.size();i++){
                //If the door is being targeted by this trigger.
                if(vector_doors[i].identifier==targets[n].identifier){
                    vector_doors[i].toggle_open();
                }
            }
        }

        //If the target is a trap.
        else if(targets[n].type==2){
            //Loop through all of the traps.
            for(int i=0;i<vector_traps.size();i++){
                //If the trap is being targeted by this trigger.
                if(vector_traps[i].identifier==targets[n].identifier){
                    //If the trap's trigger effect is "toggle active."
                    if(!vector_traps[i].trigger_effect){
                        vector_traps[i].toggle_active();
                    }
                    //If the trap's trigger effect is "trigger special function".
                    else{
                        vector_traps[i].trigger_special_function();
                    }
                    /**if(vector_doors[i].open){
                        play_positional_sound(sound_system.door_open,vector_doors[i].x,vector_doors[i].y);
                    }
                    else{
                        play_positional_sound(sound_system.door_close,vector_doors[i].x,vector_doors[i].y);
                    }*/
                }
            }
        }
    }

    if(frame==0){
        frame=1;
    }
    else if(frame==1){
        frame=0;
    }

    //If the trigger is not repeating, it now becomes inactive.
    if(!repeating){
        active=false;
    }
    //If the trigger is repeating, it becomes inactive.
    //The repeat timer is also reset.
    else{
        active=false;
        repeat_current=0;
    }
}

bool Trigger::targets_trap(short trap_type){
    for(int n=0;n<targets.size();n++){
        if(targets[n].type==2){
            for(int i=0;i<vector_traps.size();i++){
                if(vector_traps[i].identifier==targets[n].identifier){
                    if(vector_traps[i].type==trap_type){
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

void Trigger::handle_events(){
    //If the trigger is active, we handle uses.
    if(active){
        //If the trigger is used by touch.
        if(trigger_method){
            //If this trigger's user type is 'any npc'.
            if(user_type==TRIGGER_USER_ALL){
                //Check for collisions with npcs.
                for(int i=0;i<vector_npcs.size();i++){
                    //Only do collision checks for the npc if it exists.
                    if(vector_npcs[i].exists){
                        if(collision_check(x,y,w,h,vector_npcs[i].x,vector_npcs[i].y,vector_npcs[i].w,vector_npcs[i].h)){
                            use();
                        }
                    }
                }
            }

            //Check for a collision with the player.
            if(collision_check(x,y,w,h,player.x,player.y,player.w,player.h)){
                use();
            }

            for(int i=0;i<mp_players.size();i++){
                if(collision_check(x,y,w,h,mp_players[i].x,mp_players[i].y,mp_players[i].w,mp_players[i].h)){
                    use();
                }
            }
        }
    }

    //If the trigger is inactive but is a repeating trigger.
    else if(!active && repeating){
        repeat_current++;

        if(repeat_current>=repeat_time){
            repeat_current=0;
            active=true;
        }
    }
}

void Trigger::animate(){
    //Increment the frame counter.
    /**frame_counter++;

    //
    if(frame_counter>=10){
        frame_counter=0;

        //Now increment the frame.
        frame++;

        //
        if(frame>MOVING_PLATFORM_SPRITES-1){
            frame=0;
        }
    }*/
}

void Trigger::render(bool mirrored){
    if(render_trigger!=0){
        //If in camera bounds, render.
        if(x>=player.camera_x-w && x<=player.camera_x+player.camera_w && y>=player.camera_y-h && y<=player.camera_y+player.camera_h){
            double render_x=x;
            double render_y=y;
            if(mirrored && touching_mirror(x+MIRROR_DISTANCE_X,y+MIRROR_DISTANCE_Y,w,h)){
                render_x+=MIRROR_DISTANCE_X;
                render_y+=MIRROR_DISTANCE_Y;
            }

            render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),image.sprite_sheet_triggers,&sprites_triggers[render_trigger-1].change_states[frame]);
        }
    }
}
