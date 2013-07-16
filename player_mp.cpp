/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "player_mp.h"
#include "enumerations.h"
#include "shot_data.h"
#include "world.h"
#include "counters.h"
#include "collision.h"
#include "score.h"
#include "mirror.h"
#include "render.h"
#include "bubble_mode.h"

using namespace std;

Player_Mp::Player_Mp(vector<Input_Data> get_keys,int get_which_mp_player,bool get_ai_controlled){
    keys=get_keys;
    which_mp_player=get_which_mp_player;
    ai_controlled=get_ai_controlled;

    reset();
}

void Player_Mp::reset(){
    option_character=CHARACTER_HUBERT;

    invulnerable=false;

    current_shot=SHOT_PLAYER;

    ammo=STARTING_AMMO;

    x=0;
    y=0;
    facing=0;

    if(player.on_worldmap()){
        w=PLAYER_WORLDMAP_W;
        h=PLAYER_WORLDMAP_H;
    }
    else{
        w=PLAYER_W;
        h=PLAYER_H;
    }

    //Reset AI keystates.
    for(short i=0;i<256;i++){
        ai_keystates[i]=false;
    }
    ai_key_events.clear();

    ptr_player_image=return_character_image();
    ptr_player_worldmap_image=return_character_worldmap_image();

    ptr_player_footstep=return_character_sound_footstep();
    ptr_player_footstep2=return_character_sound_footstep2();
    ptr_player_jump=return_character_sound_jump();
    ptr_player_start_slide=return_character_sound_start_slide();
    ptr_player_worldmap_footstep=return_character_sound_footstep();
    ptr_player_worldmap_footstep2=return_character_sound_footstep2();
}

void Player_Mp::load_data(){
    light_source.on=true;
    light_source.x=0;
    light_source.y=0;
    light_source.color=color_name_to_doubles(COLOR_WHITE);
    light_source.radius=15*(TILE_SIZE/LIGHTING_TILE_SIZE);
    light_source.dimness=0.0;
    light_source.falloff=0.035/(TILE_SIZE/LIGHTING_TILE_SIZE);

    FLYING=false;
    is_player=true;
    DYING=false;
    counter_bubble=0;
    death_direction=false;
    death_speed=2.5;
    death_bounces=0;
    sucked_left=false;
    sucked_right=false;
    undead=false;
    counter_jump_mercy=0;
    counter_jump_mode=0;
    hitbox_size_modifier=-4;
    delta_move_state=NONE;
    counter_swim_sound=0;
    worldmap_run_speed=5.0;

    bubble_mode=false;
    bubble_move_x=0.0;
    bubble_move_y=0.0;

    set_physics();

    speedometer=0.0;

    //Shooting:
    shoot_state=NONE;
    SHOOTING=false;
    shoot_render_direction=NONE;

    CROUCHING=false;
    crouching_at_frame_start=false;
    SLIDING=false;

    //Start the player off with his feet on neither air nor ground.
    //handle_events() will determine where the player's feet are.
    touching_air=false;
    touching_ground=false;
    touching_sloped_ground=false;
    touched_slope_angle=0.0;
    touching_sloped_ground_last_check=false;
    on_cloud=false;
    cloud_npc=false;

    //Climbing:
    CLIMBING=false;
    on_climbable_top_tile=false;
    climb_jump_timer=0;
    climb_jump_delay=20;
    climb_speed=3.5;
    climbing_jump_max=10.0;
    climbing_jump_min=8.0;
    CLIMBING_JUMP=false;
    climb_state=NONE;

    //Moving Platform:
    moving_platform_x=0.0;
    moving_platform_y=0.0;
    MOVING_PLATFORM_X_THIS_FRAME=false;
    MOVING_PLATFORM_IN_WATER=false;

    w=PLAYER_W;
    h=PLAYER_H;

    //Animation:
    frame_idle=0;
    frame_counter_idle=0;
    frame=0;
    frame_counter=0;
    frame_swim=0;
    frame_counter_swim=0;
    frame_jump=0;
    frame_counter_jump=0;
    frame_shoot=0;
    frame_counter_shoot=0;
    frame_climb=1;
    frame_counter_climb=0;
    frame_fly=0;
    frame_counter_fly=0;
    frame_death=0;
    frame_counter_death=0;
    frame_powerup=0;
    frame_counter_powerup=0;

    balloon_scale_direction_x=false;
    balloon_scale_x=1.0;
    balloon_scale_direction_y=true;
    balloon_scale_y=1.0;

    update_character();

    check_special_items();

    decision_type=AI_DECISION_NONE;
    counter_decision_cooldown_revive=0;

    counter_path_far=0;
    counter_path_medium=0;

    counter_path_update=0;
    counter_path_giveup=0;

    path.clear();
}

void Player_Mp::check_special_items(){
    if(player.check_inventory(ITEM_SWIMMING_GEAR)){
        swimming_gear=true;
    }
    if(player.check_inventory(ITEM_SUIT_DEADLY_WATER)){
        suit_deadly_water=true;
    }
    if(player.check_inventory(ITEM_SUIT_SHARP)){
        suit_sharp=true;
    }
    if(player.check_inventory(ITEM_SUIT_BANANA)){
        suit_banana=true;
    }
    if(player.check_inventory(ITEM_SHOT_HOMING)){
        shot_homing=true;
    }
    if(player.check_inventory(ITEM_TRANSLATOR)){
        translator=true;
    }
    if(player.check_inventory(ITEM_J_WING)){
        j_wing=true;
    }
}

void Player_Mp::set_physics(){
    //Movement:
    move_state=NONE;
    run_speed=0.0;
    max_speed=6.0;
    acceleration=0.275;
    deceleration=0.5;
    air_drag=0.3;
    air_drag_divisor=64.0;
    friction=acceleration;
    air_accel=acceleration;
    air_decel=deceleration;

    //Gravity:
    air_velocity=0;
    IN_AIR=false;
    gravity_max=10.0;
    gravity=0.5;

    //Jumping:
    jump_state=false;
    jump_max=10.0;
    jump_min=5.0;
    extra_jumps=0;

    ///Sonic Physics
    /**max_speed=6.0;
    acceleration=0.046875;
    deceleration=0.5;
    air_drag=0.125;
    air_drag_divisor=256.0;
    friction=acceleration;
    air_accel=acceleration*2;
    air_decel=air_accel;

    air_velocity=0;
    IN_AIR=false;
    gravity_max=16.0;
    gravity=0.21875;

    jump_state=false;
    jump_max=6.5;
    jump_min=4.0;
    extra_jumps=0;*/
    ///

    //Swimming:
    SWIMMING=false;
    underwater=false;
    water_running=false;
    SWIM_CAN_JUMP=false;
    swimming_gear=false;
    oxygen_max_capacity=75;
    oxygen=oxygen_max_capacity;
    max_swim_speed=6.5;
    max_buoyancy=2.0;
    float_speed=0.0;
    swim_acceleration=0.1375;
    swim_deceleration=0.375;
    swim_friction=swim_acceleration;

    //Other Upgrades:
    suit_deadly_water=false;
    suit_sharp=false;
    suit_banana=false;
    shot_homing=false;
    translator=false;
    j_wing=false;
}

void Player_Mp::update_character(){
    if(ptr_player_image!=return_character_image()){
        ptr_player_image=return_character_image();
    }
    if(ptr_player_worldmap_image!=return_character_worldmap_image()){
        ptr_player_worldmap_image=return_character_worldmap_image();
    }

    if(ptr_player_footstep!=return_character_sound_footstep()){
        ptr_player_footstep=return_character_sound_footstep();
    }
    if(ptr_player_footstep2!=return_character_sound_footstep2()){
        ptr_player_footstep2=return_character_sound_footstep2();
    }
    if(ptr_player_jump!=return_character_sound_jump()){
        ptr_player_jump=return_character_sound_jump();
    }
    if(ptr_player_start_slide!=return_character_sound_start_slide()){
        ptr_player_start_slide=return_character_sound_start_slide();
    }
    if(ptr_player_worldmap_footstep!=return_character_sound_footstep()){
        ptr_player_worldmap_footstep=return_character_sound_footstep();
    }
    if(ptr_player_worldmap_footstep2!=return_character_sound_footstep2()){
        ptr_player_worldmap_footstep2=return_character_sound_footstep2();
    }

    if(option_character==CHARACTER_HUBERT){
        set_physics();
    }
    else if(option_character==CHARACTER_SLIME_O){
        max_speed=5.0;
        deceleration=0.6;
        friction*=2.0;
        air_decel=deceleration;

        gravity_max=11.0;
        gravity=0.55;

        jump_max=12.0;
        jump_min=6.5;

        max_swim_speed=6.0;
        max_buoyancy=6.0;
        swim_deceleration=0.475;
        swim_friction=swim_acceleration*2.0;
    }
    else if(option_character==CHARACTER_SKETCH){
        max_speed=6.5;
        acceleration=0.375;
        deceleration=0.45;
        friction=acceleration*0.75;
        air_accel=acceleration;
        air_decel=deceleration;

        gravity_max=6.5;
        gravity=0.2;

        jump_max=6.5;
        jump_min=5.0;

        max_swim_speed=7.0;
        max_buoyancy=0.75;
        swim_acceleration=0.3375;
        swim_deceleration=0.375;
        swim_friction=swim_acceleration;
    }
    else if(option_character==CHARACTER_PENNY){
        max_speed=6.5;
        acceleration=0.375;
        deceleration=0.5;
        air_drag=0.125;
        air_drag_divisor=256.0;
        friction=acceleration;
        air_accel=acceleration;
        air_decel=deceleration;

        gravity=0.55;
        jump_max=10.0;
        jump_min=5.0;

        max_swim_speed=7.0;
        swim_acceleration=0.3375;
        swim_friction=swim_acceleration;
    }
}

void Player_Mp::move(){
    bool events_handled=false;
    bool started_frame_on_ground=false;

    if(!bubble_mode){
        if(player.game_mode_is_multiplayer()){
            if(!DYING && which_mp_player!=player.cam_focused_index() && !collision_check(x,y,w,h,player.camera_x,player.camera_y,player.camera_w,player.camera_h)){
                bubble_move_x=run_speed;
                if(IN_AIR){
                    bubble_move_y=air_velocity;
                }
                else{
                    bubble_move_y=0.0;
                }
                mp_reset(x,y);
                bubble_mode=true;

                //Prevent the constant bubble forming sound if we are moving the camera around with dev controls.
                if(player.cam_state==CAM_STICKY){
                    play_positional_sound(sound_system.player_bubble_form);
                }
                return;
            }
        }

        speedometer=run_speed+moving_platform_x;
        if(sucked_left && !sucked_right){
            speedometer-=SUCK_SPEED;
        }
        else if(!sucked_left && sucked_right){
            speedometer+=SUCK_SPEED;
        }
        speedometer=fabs(speedometer);

        if(!IN_AIR){
            started_frame_on_ground=true;
        }

        if(crouching_at_frame_start && !CROUCHING){
            pushed_into_ceiling();
        }

        bool was_sliding=SLIDING;
        SLIDING=false;

        if(touching_sloped_ground && command_state(COMMAND_DOWN)){
            if(!was_sliding){
                play_positional_sound(*ptr_player_start_slide,x,y);
            }

            crouch_start();
            SLIDING=true;
            if(touched_slope_angle==45){
                move_state=LEFT;
            }
            else if(touched_slope_angle==135){
                move_state=RIGHT;
            }
        }
    }

    if(counter_jump_mercy>0){
        counter_jump_mercy--;
    }
    if(counter_jump_mode>0){
        counter_jump_mode--;
    }
    if(!DYING){
        handle_counters();
    }

    if(!bubble_mode){
        if(!CLIMBING){
            if(sucked_left && !sucked_right){
                move_suck(LEFT);
                handle_events();
                events_handled=true;
            }
            else if(!sucked_left && sucked_right){
                move_suck(RIGHT);
                handle_events();
                events_handled=true;
            }
        }

        //If the player is alive.
        if(!DYING){
            if(climb_jump_timer>0){
                climb_jump_timer--;
            }

            if(!CLIMBING){
                //*************************//
                // Handle x-axis movement: //
                //*************************//

                //////////////////////////////////////////////////
                //Move the player according to their move state://
                //////////////////////////////////////////////////

                double run_chunk;
                if(fabs(run_speed)<pixel_safety_x){
                    run_chunk=fabs(run_speed);
                }
                else{
                    run_chunk=pixel_safety_x;
                }

                for(double i=fabs(run_speed);i>0;){
                    //If we have run_chunk or more pixels left to move,
                    //we will move run_chunk pixels, call handle_events(), and loop back up here.

                    //Or, if we have less than run_chunk pixels left to move,
                    //we will move whatever pixels we have left and call handle_events() once more.
                    if(i<run_chunk){
                        run_chunk=i;
                        i=0;
                    }

                    //Move.
                    if(run_speed<0.0){
                        run_chunk*=-1;
                    }
                    x+=run_chunk;
                    if(run_speed<0.0){
                        run_chunk*=-1;
                    }

                    //If we still have pixels left to move.
                    if(i!=0){
                        i-=run_chunk;
                    }

                    handle_events();
                    //If the player is now climbing.
                    if(CLIMBING){
                        //Stop processing movement for this frame.
                        return;
                    }
                    events_handled=true;
                }

                if(!SWIMMING){
                    //Handle acceleration.
                    double accel_to_use=acceleration;
                    if(IN_AIR){
                        accel_to_use=air_accel;
                    }
                    double slide_bonus=0.0;
                    if(SLIDING){
                        slide_bonus=accel_to_use;
                    }
                    if(run_speed<max_speed && run_speed>max_speed*-1){
                        if(move_state==LEFT && run_speed<=0.0){
                            run_speed-=accel_to_use+slide_bonus;
                        }
                        else if(move_state==RIGHT && run_speed>=0.0){
                            run_speed+=accel_to_use+slide_bonus;
                        }
                        if(run_speed>max_speed){
                            run_speed=max_speed;
                        }
                        else if(run_speed<max_speed*-1){
                            run_speed=max_speed*-1;
                        }
                    }
                    //Handle deceleration.
                    double decel_to_use=deceleration;
                    if(IN_AIR){
                        decel_to_use=air_decel;
                    }
                    slide_bonus=0.0;
                    if(SLIDING){
                        slide_bonus=decel_to_use;
                    }
                    if(move_state==LEFT && run_speed>0.0){
                        if(run_speed-decel_to_use-slide_bonus<0.0){
                            run_speed=0.0-decel_to_use-slide_bonus;
                        }
                        else{
                            run_speed-=decel_to_use+slide_bonus;
                        }
                    }
                    else if(move_state==RIGHT && run_speed<0.0){
                        if(run_speed+decel_to_use+slide_bonus>0.0){
                            run_speed=decel_to_use+slide_bonus;
                        }
                        else{
                            run_speed+=decel_to_use+slide_bonus;
                        }
                    }
                    if(!IN_AIR){
                        //Handle friction.
                        if(move_state!=LEFT && move_state!=RIGHT && run_speed!=0.0){
                            if(run_speed<0.0){
                                run_speed+=friction;
                                if(run_speed>0.0){
                                    run_speed=0.0;
                                }
                            }
                            else if(run_speed>0.0){
                                run_speed-=friction;
                                if(run_speed<0.0){
                                    run_speed=0.0;
                                }
                            }
                        }
                        if(!MOVING_PLATFORM_X_THIS_FRAME && moving_platform_x!=0.0){
                            if(moving_platform_x<0.0){
                                moving_platform_x+=friction;
                                if(moving_platform_x>0.0){
                                    moving_platform_x=0.0;
                                }
                            }
                            else if(moving_platform_x>0.0){
                                moving_platform_x-=friction;
                                if(moving_platform_x<0.0){
                                    moving_platform_x=0.0;
                                }
                            }
                        }
                    }
                    else{
                        //Handle air drag.
                        if(air_velocity<0.0 && air_velocity>jump_min*-1 && air_drag!=0.0){
                            if(run_speed<0.0){
                                run_speed+=(fabs(run_speed)/air_drag)/air_drag_divisor;
                            }
                            else if(run_speed>0.0){
                                run_speed-=(fabs(run_speed)/air_drag)/air_drag_divisor;
                            }
                            if(moving_platform_x<0.0){
                                moving_platform_x+=(fabs(moving_platform_x)/air_drag)/air_drag_divisor;
                            }
                            else if(moving_platform_x>0.0){
                                moving_platform_x-=(fabs(moving_platform_x)/air_drag)/air_drag_divisor;
                            }
                        }
                    }
                }

                //////////////////////////////////////////////////////////////////
                //Move the player according to their moving platform x modifier://
                //////////////////////////////////////////////////////////////////

                if(fabs(moving_platform_x)<pixel_safety_x){
                    run_chunk=fabs(moving_platform_x);
                }
                else{
                    run_chunk=pixel_safety_x;
                }

                for(double i=fabs(moving_platform_x);i>0;){
                    //If we have run_chunk or more pixels left to move,
                    //we will move run_chunk pixels, call handle_events(), and loop back up here.

                    //Or, if we have less than run_chunk pixels left to move,
                    //we will move whatever pixels we have left and call handle_events() once more.
                    if(i<run_chunk){
                        run_chunk=i;
                        i=0;
                    }

                    //Move.
                    if(moving_platform_x<0.0){
                        run_chunk*=-1;
                    }
                    x+=run_chunk;
                    if(moving_platform_x<0.0){
                        run_chunk*=-1;
                    }

                    //If we still have pixels left to move.
                    if(i!=0){
                        i-=run_chunk;
                    }

                    handle_events();
                    //If the player is now climbing.
                    if(CLIMBING){
                        //Stop processing movement for this frame.
                        return;
                    }
                    events_handled=true;
                }

                //*************************//
                // Handle y-axis movement: //
                //*************************//

                //////////////////////////////////////////////////////
                //Move the player according to their swimming state://
                //////////////////////////////////////////////////////

                //If the player is swimming.
                if(SWIMMING){
                    double float_chunk;
                    if(fabs(float_speed)<pixel_safety_y){
                        float_chunk=fabs(float_speed);
                    }
                    else{
                        float_chunk=pixel_safety_y;
                    }

                    for(double i=fabs(float_speed);i>0;){
                        //If we have float_chunk or more pixels left to move,
                        //we will move float_chunk pixels, call handle_events(), and loop back up here.

                        //Or, if we have less than float_chunk pixels left to move,
                        //we will move whatever pixels we have left and call handle_events() once more.
                        if(i<float_chunk){
                            float_chunk=i;
                            i=0;
                        }

                        //Move.
                        if(float_speed<0){
                            float_chunk*=-1;
                        }

                        y+=float_chunk;

                        if(float_speed<0){
                            float_chunk*=-1;
                        }

                        //If we still have pixels left to move.
                        if(i!=0){
                            i-=float_chunk;
                        }

                        handle_events();
                        //If the player is now climbing.
                        if(CLIMBING){
                            //Stop processing movement for this frame.
                            return;
                        }
                        events_handled=true;
                    }
                }

                ////////////////////////////////////////////////////
                //Move the player according to their air_velocity://
                ////////////////////////////////////////////////////

                //If the player is in the air.
                if(IN_AIR){
                    double air_chunk;
                    if(fabs(air_velocity)<pixel_safety_y){
                        air_chunk=fabs(air_velocity);
                    }
                    else{
                        air_chunk=pixel_safety_y;
                    }

                    //First, translate the player based on his air velocity.
                    for(double i=fabs(air_velocity);i>0;){
                        //If we have air_chunk or more pixels left to move,
                        //we will move air_chunk pixels, call handle_events(), and loop back up here.

                        //Or, if we have less than air_chunk pixels left to move,
                        //we will move whatever pixels we have left and call handle_events() once more.
                        if(i<air_chunk){
                            air_chunk=i;
                            i=0;
                        }

                        //Move.
                        if(air_velocity<0){
                            y-=air_chunk;
                        }
                        else if(air_velocity>0){
                            y+=air_chunk;
                        }

                        //If we still have pixels left to move.
                        if(i!=0){
                            i-=air_chunk;
                        }

                        handle_events();
                        //If the player is now climbing.
                        if(CLIMBING){
                            //Stop processing movement for this frame.
                            return;
                        }
                        events_handled=true;
                    }

                    //Then, we handle gravity, which will affect the next tick.

                    //As long as air_velocity hasn't exceeded gravity_max, the maximum speed an object can fall, add gravity to air_velocity.
                    if(air_velocity<gravity_max){
                        air_velocity+=gravity;
                    }
                    if(air_velocity>gravity_max){
                        air_velocity=gravity_max;
                    }
                }

                //*******************************//
                // Handle swimming acceleration: //
                //*******************************//

                //If the player is swimming but isn't currently moving on the y-axis.
                if(SWIMMING && move_state!=UP && move_state!=LEFT_UP && move_state!=RIGHT_UP && move_state!=DOWN && move_state!=LEFT_DOWN && move_state!=RIGHT_DOWN){
                    //Float upwards.
                    if(float_speed>-max_buoyancy){
                        //If the time in water is long enough.
                        if(timer_time_in_water.get_ticks()>=500){
                            //Stop the timer so the player's buoyancy acceleration will slow to the standard rate.
                            timer_time_in_water.stop();
                        }

                        //If the timer is not running, buoyancy acceleration is normal.
                        if(!timer_time_in_water.is_started()){
                            float_speed-=swim_acceleration/4;
                        }
                        //If the timer is running, buoyancy acceleration is increased.
                        else if(timer_time_in_water.is_started()){
                            float_speed-=swim_acceleration*4;
                        }

                        if(float_speed<-max_buoyancy){
                            float_speed=-max_buoyancy;
                        }
                        if(float_speed<-max_swim_speed){
                            float_speed=-max_swim_speed;
                        }
                    }
                }
                //Slowly stop swimming on the x-axis.
                if(SWIMMING && move_state!=LEFT && move_state!=LEFT_DOWN && move_state!=LEFT_UP && move_state!=RIGHT && move_state!=RIGHT_DOWN && move_state!=RIGHT_UP){
                    if(run_speed>0.0){
                        run_speed-=swim_friction;
                        if(run_speed<0.0){
                            run_speed=0.0;
                        }
                    }
                    else if(run_speed<0.0){
                        run_speed+=swim_friction;
                        if(run_speed>0.0){
                            run_speed=0.0;
                        }
                    }
                }
                if(SWIMMING && moving_platform_x!=0.0){
                    if(moving_platform_x<0.0){
                        moving_platform_x+=friction;
                        if(moving_platform_x>0.0){
                            moving_platform_x=0.0;
                        }
                    }
                    else if(moving_platform_x>0.0){
                        moving_platform_x-=friction;
                        if(moving_platform_x<0.0){
                            moving_platform_x=0.0;
                        }
                    }
                }
                //If the player is swimming and is moving.
                //Don't allow the player to swim beyond the maximum swim speed.
                if(SWIMMING && move_state!=0){
                    if(run_speed<max_swim_speed && run_speed>max_swim_speed*-1){
                        if((move_state==LEFT || move_state==LEFT_DOWN || move_state==LEFT_UP) && run_speed<=0.0){
                            run_speed-=swim_acceleration;
                        }
                        else if((move_state==RIGHT || move_state==RIGHT_DOWN || move_state==RIGHT_UP) && run_speed>=0.0){
                            run_speed+=swim_acceleration;
                        }
                        if(run_speed>max_swim_speed){
                            run_speed=max_swim_speed;
                        }
                        else if(run_speed<max_swim_speed*-1){
                            run_speed=max_swim_speed*-1;
                        }
                    }
                    if(float_speed<max_swim_speed && float_speed>max_swim_speed*-1){
                        if((move_state==UP || move_state==LEFT_UP || move_state==RIGHT_UP) && float_speed<=0.0){
                            float_speed-=swim_acceleration;
                        }
                        else if((move_state==DOWN || move_state==LEFT_DOWN || move_state==RIGHT_DOWN) && float_speed>=0.0){
                            float_speed+=swim_acceleration;
                        }
                        if(float_speed>max_swim_speed){
                            float_speed=max_swim_speed;
                        }
                        else if(float_speed<max_swim_speed*-1){
                            float_speed=max_swim_speed*-1;
                        }
                    }
                    if((move_state==LEFT || move_state==LEFT_DOWN || move_state==LEFT_UP) && run_speed>0.0){
                        if(run_speed-swim_deceleration<0.0){
                            run_speed=0.0-swim_deceleration;
                        }
                        else{
                            run_speed-=swim_deceleration;
                        }
                    }
                    else if((move_state==RIGHT || move_state==RIGHT_DOWN || move_state==RIGHT_UP) && run_speed<0.0){
                        if(run_speed+swim_deceleration>0.0){
                            run_speed=swim_deceleration;
                        }
                        else{
                            run_speed+=swim_deceleration;
                        }
                    }
                    if((move_state==UP || move_state==LEFT_UP || move_state==RIGHT_UP) && float_speed>0.0){
                        if(float_speed-swim_deceleration<0.0){
                            float_speed=0.0-swim_deceleration;
                        }
                        else{
                            float_speed-=swim_deceleration;
                        }
                    }
                    else if((move_state==DOWN || move_state==LEFT_DOWN || move_state==RIGHT_DOWN) && float_speed<0.0){
                        if(float_speed+swim_deceleration>0.0){
                            float_speed=swim_deceleration;
                        }
                        else{
                            float_speed+=swim_deceleration;
                        }
                    }
                }
            }

            //***********************************************//
            // Handle y-axis movement for a climbing player: //
            //***********************************************//

            else if(CLIMBING){
                double speed=climb_speed;
                if(move_state==DOWN){
                    speed*=2;
                }

                double run_chunk;
                if(fabs(speed)<pixel_safety_y){
                    run_chunk=fabs(speed);
                }
                else{
                    run_chunk=pixel_safety_y;
                }

                for(double i=fabs(speed);i>0;){
                    //If we have run_chunk or more pixels left to move,
                    //we will move run_chunk pixels, call handle_events(), and loop back up here.

                    //Or, if we have less than run_chunk pixels left to move,
                    //we will move whatever pixels we have left and call handle_events() once more.
                    if(i<run_chunk){
                        run_chunk=i;
                        i=0;
                    }

                    //Move.
                    if(move_state==UP){
                        y-=run_chunk;
                    }
                    else if(move_state==DOWN){
                        y+=run_chunk;
                    }

                    //If we still have pixels left to move.
                    if(i!=0){
                        i-=run_chunk;
                    }

                    handle_events();
                    events_handled=true;
                }
            }
        }
        //If the player is dying.
        else{
            if(collision_check(x,y,w,h,0,0,level.level_x,level.level_y)){
                double chunk;
                if(fabs(air_velocity)<pixel_safety_y){
                    chunk=fabs(air_velocity);
                }
                else{
                    chunk=pixel_safety_y;
                }

                //First, translate the player based on his air velocity.
                for(double i=fabs(air_velocity);i>0;){
                    //If we have air_chunk or more pixels left to move,
                    //we will move air_chunk pixels, call handle_events(), and loop back up here.

                    //Or, if we have less than air_chunk pixels left to move,
                    //we will move whatever pixels we have left and call handle_events() once more.
                    if(i<chunk){
                        chunk=i;
                        i=0;
                    }

                    //Move.
                    if(air_velocity<0){
                        y-=chunk;
                    }
                    else if(air_velocity>0){
                        y+=chunk;
                    }

                    //If we still have pixels left to move.
                    if(i!=0){
                        i-=chunk;
                    }

                    handle_events();
                    events_handled=true;
                }

                //Then, we handle gravity, which will affect the next tick.

                //If a jump is in progress, gravity is handled there, in handle_input().
                //Otherwise, we handle gravity normally.
                //As long as air_velocity hasn't exceeded jump_max, the maximum speed an object can fall, add gravity to air_velocity.
                if(air_velocity<gravity_max){
                    air_velocity+=gravity;
                }
                if(air_velocity>gravity_max){
                    air_velocity=gravity_max;
                }

                if(fabs(death_speed)<pixel_safety_x){
                    chunk=fabs(death_speed);
                }
                else{
                    chunk=pixel_safety_x;
                }

                for(double i=fabs(death_speed);i>0;){
                    //If we have run_chunk or more pixels left to move,
                    //we will move run_chunk pixels, call handle_events(), and loop back up here.

                    //Or, if we have less than run_chunk pixels left to move,
                    //we will move whatever pixels we have left and call handle_events() once more.
                    if(i<chunk){
                        chunk=i;
                        i=0;
                    }

                    //Move.
                    if(death_direction){
                        x-=chunk;
                    }
                    else{
                        x+=chunk;
                    }

                    //If we still have pixels left to move.
                    if(i!=0){
                        i-=chunk;
                    }

                    handle_events();
                    events_handled=true;
                }
            }
        }
    }
    //If in bubble mode.
    else{
        double bubble_max_speed=BUBBLE_MAX_SPEED_ON_CAMERA;
        if(!collision_check(x,y,w,h,player.camera_x,player.camera_y,player.camera_w,player.camera_h)){
            bubble_max_speed=BUBBLE_MAX_SPEED_OFF_CAMERA;
        }

        if(fabs(player.cam_focused_x()-x)>=bubble_move_x){
            x+=bubble_move_x;
        }
        else{
            x=player.cam_focused_x();
        }

        if(fabs(player.cam_focused_y()-y)>=bubble_move_y){
            y+=bubble_move_y;
        }
        else{
            y=player.cam_focused_y();
        }

        if(fabs(player.cam_focused_x()-x)!=0.0){
            if(player.cam_focused_x()<x){
                if(bubble_move_x<=0.0){
                    bubble_move_x-=BUBBLE_ACCEL;
                }
                else{
                    bubble_move_x-=BUBBLE_DECEL;
                }
            }
            else{
                if(bubble_move_x>=0.0){
                    bubble_move_x+=BUBBLE_ACCEL;
                }
                else{
                    bubble_move_x+=BUBBLE_DECEL;
                }
            }
        }
        if(bubble_move_x<bubble_max_speed*-1){
            bubble_move_x=bubble_max_speed*-1;
        }
        if(bubble_move_x>bubble_max_speed){
            bubble_move_x=bubble_max_speed;
        }

        if(fabs(player.cam_focused_y()-y)!=0.0){
            if(player.cam_focused_y()<y){
                if(bubble_move_y<=0.0){
                    bubble_move_y-=BUBBLE_ACCEL;
                }
                else{
                    bubble_move_y-=BUBBLE_DECEL;
                }
            }
            else{
                if(bubble_move_y>=0.0){
                    bubble_move_y+=BUBBLE_ACCEL;
                }
                else{
                    bubble_move_y+=BUBBLE_DECEL;
                }
            }
        }
        if(bubble_move_y<bubble_max_speed*-1){
            bubble_move_y=bubble_max_speed*-1;
        }
        if(bubble_move_y>bubble_max_speed){
            bubble_move_y=bubble_max_speed;
        }

        if(!player.cam_focused_dying() && collision_check(x+w/4.0,y+h/8.0,w/2.0,h/4.0,player.cam_focused_x()+player.cam_focused_w()/4.0,player.cam_focused_y()+player.cam_focused_h()/8.0,player.cam_focused_w()/2.0,player.cam_focused_h()/4.0)){
            bubble_mode=false;
            air_velocity=-jump_min*1.5;
            IN_AIR=true;
            run_speed=0.0;

            y=player.cam_focused_y();

            //Prevent the constant bubble popping sound if we are moving the camera around with dev controls.
            if(player.cam_state==CAM_STICKY){
                play_positional_sound(sound_system.player_bubble_pop);
            }
        }
    }

    if(!bubble_mode){
        if(!events_handled){
            handle_events();
        }

        if(started_frame_on_ground && IN_AIR){
            counter_jump_mercy=JUMP_MERCY_TIME;
        }
    }

    sucked_left=false;
    sucked_right=false;
}

void Player_Mp::handle_tiles(int check_x_start,int check_x_end,int check_y_start,int check_y_end){
    for(int int_y=check_y_start;int_y<check_y_end;int_y++){
        for(int int_x=check_x_start;int_x<check_x_end;int_x++){
            //As long as the current tile is within the level's boundaries.
            if(int_x>=0 && int_x<=(level.level_x/TILE_SIZE)-1 && int_y>=0 && int_y<=(level.level_y/TILE_SIZE)-1){
                //If the player is not climbing.
                if(!CLIMBING){

                    handle_collision_tiles(int_x,int_y,CROUCHING);

                    handle_tile_hazard(int_x,int_y);
                }
                //If the player is currently climbing.
                else{
                    handle_climbing(int_x,int_y);
                }
            }
        }
    }
}

void Player_Mp::handle_tile_hazard(int int_x,int int_y){
    if(level.tile_array[int_x][int_y].special==TILE_SPECIAL_HAZARD){
        if(collision_check(return_x(),return_y(),return_w(),return_h(),level.tile_array[int_x][int_y].return_x(),level.tile_array[int_x][int_y].return_y(),TILE_SIZE,TILE_SIZE)){
            handle_death(level.tile_array[int_x][int_y].return_x(),level.tile_array[int_x][int_y].return_y(),TILE_SIZE,TILE_SIZE);
        }
    }
    //If the tile is a water tile and water tiles are deadly.
    else if(player.deadly_water && !suit_deadly_water && SWIMMING && level.tile_array[int_x][int_y].special==TILE_SPECIAL_WATER){
        if(collision_check(return_x(),return_y(),return_w(),return_h(),level.tile_array[int_x][int_y].return_x(),level.tile_array[int_x][int_y].return_y(),TILE_SIZE,TILE_SIZE)){
            handle_death(level.tile_array[int_x][int_y].return_x(),level.tile_array[int_x][int_y].return_y(),TILE_SIZE,TILE_SIZE);
        }
    }
}

void Player_Mp::handle_events(bool being_pushed_up){
    prepare_for_events();

    //The current tile location for the actor.
    int actor_current_x=(int)((int)x/TILE_SIZE);
    int actor_current_y=(int)((int)y/TILE_SIZE);

    //Check all tiles in a square around the actor.
    int check_x_start=actor_current_x-4;
    int check_x_end=actor_current_x+4;
    int check_y_start=actor_current_y-4;
    int check_y_end=actor_current_y+4;

    //If the player is alive.
    if(!DYING){
        //**********************************//
        // Check for collisions with items: //
        //**********************************//
        for(int i=0;i<vector_items.size();i++){
            if(fabs(vector_items[i].x-x)<PROCESS_RANGE && fabs(vector_items[i].y-y)<PROCESS_RANGE){
                //Only do collision checks for the item if it exists.
                if(vector_items[i].exists){
                    if(collision_check(x,y,w,h,vector_items[i].x,vector_items[i].y,vector_items[i].w,vector_items[i].h)){
                        if(!(vector_items[i].type==ITEM_SPAWNPOINT || vector_items[i].type==ITEM_CHECKPOINT || vector_items[i].type==ITEM_ENDPOINT)){
                            //The item no longer exists.
                            vector_items[i].exists=false;

                            //If the item is a perma-item.
                            /**if(vector_items[i].type==ITEM_LEAF || vector_items[i].type==ITEM_CHEESE ||
                               (vector_items[i].type>=ITEM_SWIMMING_GEAR && vector_items[i].type<=ITEM_SINK) ||
                               vector_items[i].type==ITEM_AMMO_BARREL ||
                               (vector_items[i].type>=ITEM_KEY_GRAY && vector_items[i].type<=ITEM_KEY_CYAN)){
                                if(persistent_level_data){
                                    need_to_save_level_data=true;
                                }
                            }*/
                        }

                        if(vector_items[i].type==ITEM_LEAF){
                            player.gain_score(SCORES_ITEMS[ITEM_LEAF],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);

                            //Play the leaf item collection sound.
                            play_positional_sound(sound_system.item_collect_leaf,x,y);
                        }

                        else if(vector_items[i].type==ITEM_CHEESE){
                            player.gain_score(SCORES_ITEMS[ITEM_CHEESE],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);

                            //Play the cheese item collection sound.
                            play_positional_sound(sound_system.item_collect_cheese,x,y);
                        }

                        else if(vector_items[i].type==ITEM_AMMO){
                            int player_count=1+mp_players.size();

                            int ammo_per_player=floor((double)player.return_ammo_box_amount()/(double)player_count);
                            int difference=player.return_ammo_box_amount()%player_count;

                            ammo+=ammo_per_player+difference;

                            player.ammo+=ammo_per_player;
                            for(int n=0;n<mp_players.size();n++){
                                if(&mp_players[n]!=this){
                                    mp_players[n].ammo+=ammo_per_player;
                                }
                            }

                            ///player.gain_score(SCORES_ITEMS[ITEM_AMMO],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);

                            //Play the ammo box collection sound.
                            play_positional_sound(sound_system.item_collect_ammo,x,y);
                        }

                        else if(vector_items[i].type==ITEM_J_BALLOON){
                            counter_jump_mode=JUMP_MODE_TIME;

                            player.gain_score(SCORES_ITEMS[ITEM_J_BALLOON],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);

                            play_positional_sound(sound_system.item_collect_j_balloon,x,y);

                            player.play_balloonin_sound(sound_system.balloonin);
                        }

                        else if(vector_items[i].type==ITEM_AMMO_BARREL){
                            int player_count=1+mp_players.size();

                            int ammo_per_player=floor((double)player.return_ammo_barrel_amount()/(double)player_count);
                            int difference=player.return_ammo_barrel_amount()%player_count;

                            ammo+=ammo_per_player+difference;

                            player.ammo+=ammo_per_player;
                            for(int n=0;n<mp_players.size();n++){
                                if(&mp_players[n]!=this){
                                    mp_players[n].ammo+=ammo_per_player;
                                }
                            }

                            ///player.gain_score(SCORES_ITEMS[ITEM_AMMO_BARREL],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);

                            play_positional_sound(sound_system.item_collect_ammo_barrel,x,y);
                        }

                        else if(vector_items[i].type==ITEM_CANDY){
                            player.gain_score(player.return_candy_score(),vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);

                            play_positional_sound(sound_system.item_collect_candy,x,y);
                        }

                        else if((vector_items[i].type>=ITEM_SWIMMING_GEAR && vector_items[i].type<=ITEM_SINK) ||
                                (vector_items[i].type>=ITEM_KEY_GRAY && vector_items[i].type<=ITEM_J_WING)){
                            //Find the next available inventory slot.
                            short next_available_slot=player.next_available_inventory_slot();

                            //If there is a free inventory slot.
                            if(next_available_slot!=-1){
                                //The item no longer exists.
                                vector_items[i].exists=false;

                                player.inventory.push_back(inventory_item());

                                player.inventory[player.inventory.size()-1].type=vector_items[i].type;

                                player.inventory[player.inventory.size()-1].slot=next_available_slot;

                                player.inventory[player.inventory.size()-1].name=player.name_inventory_item(vector_items[i].type);

                                //Create an inventory item notification slider.
                                sliders.push_back(Slider(vector_items[i].type,false));

                                bool new_special_item=false;

                                if(vector_items[i].type==ITEM_SWIMMING_GEAR){
                                    new_special_item=true;

                                    oxygen=oxygen_max_capacity;
                                    player.oxygen=player.oxygen_max_capacity;
                                    for(int mps=0;mps<mp_players.size();mps++){
                                        if(this!=&mp_players[mps]){
                                            mp_players[mps].oxygen=mp_players[mps].oxygen_max_capacity;
                                        }
                                    }

                                    player.gain_score(SCORES_ITEMS[ITEM_SWIMMING_GEAR],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);
                                }
                                else if(vector_items[i].type==ITEM_KEY_RED){
                                    player.gain_score(SCORES_ITEMS[ITEM_KEY_RED],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);
                                }
                                else if(vector_items[i].type==ITEM_KEY_BLUE){
                                    player.gain_score(SCORES_ITEMS[ITEM_KEY_BLUE],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);
                                }
                                else if(vector_items[i].type==ITEM_KEY_GREEN){
                                    player.gain_score(SCORES_ITEMS[ITEM_KEY_GREEN],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);
                                }
                                else if(vector_items[i].type==ITEM_KEY_YELLOW){
                                    player.gain_score(SCORES_ITEMS[ITEM_KEY_YELLOW],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);
                                }
                                else if(vector_items[i].type==ITEM_KEY_ORANGE){
                                    player.gain_score(SCORES_ITEMS[ITEM_KEY_ORANGE],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);
                                }
                                else if(vector_items[i].type==ITEM_KEY_PURPLE){
                                    player.gain_score(SCORES_ITEMS[ITEM_KEY_PURPLE],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);
                                }
                                else if(vector_items[i].type==ITEM_TOWEL){
                                    player.gain_score(SCORES_ITEMS[ITEM_TOWEL],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);
                                }
                                else if(vector_items[i].type==ITEM_KEY_GRAY){
                                    player.gain_score(SCORES_ITEMS[ITEM_KEY_GRAY],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);
                                }
                                else if(vector_items[i].type==ITEM_KEY_BROWN){
                                    player.gain_score(SCORES_ITEMS[ITEM_KEY_BROWN],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);
                                }
                                else if(vector_items[i].type==ITEM_KEY_BLACK){
                                    player.gain_score(SCORES_ITEMS[ITEM_KEY_BLACK],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);
                                }
                                else if(vector_items[i].type==ITEM_KEY_PINK){
                                    player.gain_score(SCORES_ITEMS[ITEM_KEY_PINK],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);
                                }
                                else if(vector_items[i].type==ITEM_KEY_CYAN){
                                    player.gain_score(SCORES_ITEMS[ITEM_KEY_CYAN],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);
                                }
                                else if(vector_items[i].type==ITEM_SINK){
                                    player.gain_score(SCORES_ITEMS[ITEM_SINK],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);
                                }
                                else if(vector_items[i].type==ITEM_SUIT_DEADLY_WATER){
                                    new_special_item=true;

                                    player.gain_score(SCORES_ITEMS[ITEM_SUIT_DEADLY_WATER],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);
                                }
                                else if(vector_items[i].type==ITEM_SUIT_SHARP){
                                    new_special_item=true;

                                    player.gain_score(SCORES_ITEMS[ITEM_SUIT_SHARP],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);
                                }
                                else if(vector_items[i].type==ITEM_SUIT_BANANA){
                                    new_special_item=true;

                                    player.gain_score(SCORES_ITEMS[ITEM_SUIT_BANANA],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);
                                }
                                else if(vector_items[i].type==ITEM_SHOT_HOMING){
                                    new_special_item=true;

                                    player.gain_score(SCORES_ITEMS[ITEM_SHOT_HOMING],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);
                                }
                                else if(vector_items[i].type==ITEM_TRANSLATOR){
                                    new_special_item=true;

                                    player.gain_score(SCORES_ITEMS[ITEM_TRANSLATOR],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);
                                }
                                else if(vector_items[i].type==ITEM_J_WING){
                                    new_special_item=true;

                                    player.gain_score(SCORES_ITEMS[ITEM_J_WING],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);
                                }

                                if(new_special_item){
                                    check_special_items();
                                    player.check_special_items();
                                    for(int mps=0;mps<mp_players.size();mps++){
                                        if(this!=&mp_players[mps]){
                                            mp_players[mps].check_special_items();
                                        }
                                    }
                                }

                                if(vector_items[i].type==ITEM_SWIMMING_GEAR){
                                    //Play the swimming gear collection sound.
                                    play_positional_sound(sound_system.item_collect_swimming_gear,x,y);
                                }
                                else if((vector_items[i].type>=ITEM_KEY_RED && vector_items[i].type<=ITEM_KEY_PURPLE) ||
                                        (vector_items[i].type>=ITEM_KEY_GRAY && vector_items[i].type<=ITEM_KEY_CYAN)){
                                    //Play the key collection sound.
                                    play_positional_sound(sound_system.item_collect_key,x,y);
                                }
                                else if(vector_items[i].type==ITEM_TOWEL){
                                    //Play the towel collection sound.
                                    play_positional_sound(sound_system.item_collect_towel,x,y);
                                }
                                else if(vector_items[i].type==ITEM_SINK){
                                    //Play the sink collection sound.
                                    play_positional_sound(sound_system.item_collect_sink,x,y);
                                }
                                else if(vector_items[i].type==ITEM_SUIT_DEADLY_WATER){
                                    play_positional_sound(sound_system.item_collect_suit_deadly_water,x,y);
                                }
                                else if(vector_items[i].type==ITEM_SUIT_SHARP){
                                    play_positional_sound(sound_system.item_collect_suit_sharp,x,y);
                                }
                                else if(vector_items[i].type==ITEM_SUIT_BANANA){
                                    play_positional_sound(sound_system.item_collect_suit_banana,x,y);
                                }
                                else if(vector_items[i].type==ITEM_SHOT_HOMING){
                                    play_positional_sound(sound_system.item_collect_shot_homing,x,y);
                                }
                                else if(vector_items[i].type==ITEM_TRANSLATOR){
                                    play_positional_sound(sound_system.item_collect_translator,x,y);
                                }
                                else if(vector_items[i].type==ITEM_J_WING){
                                    play_positional_sound(sound_system.item_collect_j_wing,x,y);
                                }
                            }
                        }
                    }
                    if(vector_items[i].type==ITEM_SPAWNPOINT){
                        if(collision_check(x+w/4.0,y,w-w/2.0,h,vector_items[i].x+16,vector_items[i].y+16,1,16)){
                            if(player.survival_escape && !player.survival_complete){
                                player.survival_end_game(true);

                                return;
                            }
                            else{
                                bool player_respawned=false;

                                if(player.DYING){
                                    player.mp_reset(vector_items[i].x,vector_items[i].y-14);
                                    player_respawned=true;
                                }

                                for(int n=0;n<mp_players.size();n++){
                                    if(this!=&mp_players[n] && mp_players[n].DYING){
                                        mp_players[n].mp_reset(vector_items[i].x,vector_items[i].y-14);
                                        player_respawned=true;
                                    }
                                }

                                if(player_respawned){
                                    play_positional_sound(sound_system.player_respawn);
                                }
                            }

                            if(ptr_player_image!=return_character_image()){
                                double recall_run_speed=run_speed;
                                double recall_air_velocity=air_velocity;
                                bool recall_IN_AIR=IN_AIR;

                                update_character();

                                run_speed=recall_run_speed;
                                air_velocity=recall_air_velocity;
                                IN_AIR=recall_IN_AIR;
                            }
                        }
                    }
                    if(vector_items[i].type==ITEM_CHECKPOINT){
                        if(collision_check(x,y,w,h,vector_items[i].x,vector_items[i].y-64*2,32,96+64*2)){
                            //Set the animation states for the old checkpoint.
                            if(player.current_checkpoint!=-1){
                                vector_items[player.current_checkpoint].checkpoint_reached=false;
                                vector_items[player.current_checkpoint].checkpoint_unreached=true;
                            }

                            //Set the animation states for the newly reached checkpoint.
                            vector_items[i].checkpoint_reached=true;
                            vector_items[i].checkpoint_unreached=false;

                            //If the checkpoint is a new one.
                            if(player.current_checkpoint!=i){
                                play_positional_sound(sound_system.item_checkpoint,x,y);
                            }

                            player.current_checkpoint=i;

                            bool player_respawned=false;

                            if(player.DYING){
                                player.mp_reset(vector_items[i].x,vector_items[i].y+50);
                                player_respawned=true;
                            }

                            for(int n=0;n<mp_players.size();n++){
                                if(this!=&mp_players[n] && mp_players[n].DYING){
                                    mp_players[n].mp_reset(vector_items[i].x,vector_items[i].y+50);
                                    player_respawned=true;
                                }
                            }

                            if(player_respawned){
                                play_positional_sound(sound_system.player_respawn);
                            }
                        }
                        if(collision_check(x+w/4.0,y,w-w/2.0,h,vector_items[i].x+16,vector_items[i].y+80,1,16)){
                            if(ptr_player_image!=return_character_image()){
                                double recall_run_speed=run_speed;
                                double recall_air_velocity=air_velocity;
                                bool recall_IN_AIR=IN_AIR;

                                update_character();

                                run_speed=recall_run_speed;
                                air_velocity=recall_air_velocity;
                                IN_AIR=recall_IN_AIR;
                            }
                        }
                    }
                    //If the item is the end point, and the goal has not been crossed yet.
                    if(vector_items[i].type==ITEM_ENDPOINT && !player.goal_crossed){
                        if(collision_check(x,y,w,h,vector_items[i].x,vector_items[i].y,32,96)){
                            player.goal_crossed=true;
                            player.next_level=vector_items[i].goal_level_to_load;
                            UPDATE_RATE=DEFAULT_UPDATE_RATE/2.4;
                            SKIP_TICKS=1000.0/UPDATE_RATE;
                            player.counter_level_end=8*UPDATE_RATE;

                            if(!vector_items[i].goal_secret){
                                player.gain_score(SCORE_BEAT_LEVEL,x+w/2.0,y);
                            }
                            else{
                                player.gain_score(SCORE_BEAT_LEVEL*2,x+w/2.0,y);
                            }

                            if(counter_jump_mode>0){
                                player.gain_score(SCORE_LEVEL_END_BONUS_J_BALLOON,x+w/2.0,y);
                            }

                            for(int n=0;n<vector_npcs.size();n++){
                                if(!vector_npcs[n].friendly){
                                    vector_npcs[n].handle_death(true);
                                }
                            }
                            for(int n=0;n<vector_shots.size();n++){
                                if(!vector_shots[n].BOSS){
                                    vector_shots[n].dissipate();
                                }
                            }
                            for(int n=0;n<vector_traps.size();n++){
                                if(!vector_traps[n].BOSS){
                                    vector_traps[n].active=false;
                                    vector_traps[n].dangerous=false;
                                }
                            }

                            int random=random_range(0,99);
                            if(random>=0 && random<5){
                                level.explode_party_balls(ITEM_AMMO);
                            }
                            else{
                                level.explode_party_balls(ITEM_CANDY);
                            }

                            bool player_respawned=false;

                            if(player.DYING){
                                player.mp_reset(vector_items[i].x,vector_items[i].y+50);
                                player_respawned=true;
                            }

                            for(int n=0;n<mp_players.size();n++){
                                if(this!=&mp_players[n] && mp_players[n].DYING){
                                    mp_players[n].mp_reset(vector_items[i].x,vector_items[i].y+50);
                                    player_respawned=true;
                                }
                            }

                            if(player_respawned){
                                play_positional_sound(sound_system.player_respawn);
                            }

                            music.stop_track(0.25);
                            play_positional_sound(sound_system.goal_reached);
                        }
                    }
                }
            }
        }

        //*********************************************//
        // Check for collisions with moving platforms: //
        //*********************************************//
        handle_collision_moving_platforms(TILE_SOLIDITY_CLOUD,false);

        //*********************************//
        // Check for collisions with npcs: //
        //*********************************//
        for(int i=0;i<vector_npcs.size();i++){
            if(fabs(vector_npcs[i].x-x)<PROCESS_RANGE && fabs(vector_npcs[i].y-y)<PROCESS_RANGE){
                if(vector_npcs[i].exists){
                    if(!CLIMBING && !vector_npcs[i].CLIMBING){
                        short solidity;
                        if(!SWIMMING && vector_npcs[i].act_as_platform && !vector_npcs[i].ethereal){
                            solidity=TILE_SOLIDITY_CLOUD;
                        }
                        else{
                            solidity=TILE_SOLIDITY_PASSABLE;
                        }
                        handle_collision_solid(vector_npcs[i].x,vector_npcs[i].y,vector_npcs[i].w,vector_npcs[i].h,solidity,vector_npcs[i].type);

                        handle_npc_platform(i);
                    }

                    //If the player touches any part of the npc, and the npc exists and is deadly to the touch.
                    if(!SLIDING && ((!vector_npcs[i].special_attack_in_progress && vector_npcs[i].deadly_to_touch) || (vector_npcs[i].special_attack_in_progress && vector_npcs[i].deadly_while_special_attacking)) &&
                       collision_check(return_x(),return_y(),return_w(),return_h(),vector_npcs[i].return_x(),vector_npcs[i].return_y(),vector_npcs[i].return_w(),vector_npcs[i].return_h())){
                        //The player dies!
                        handle_death(vector_npcs[i].return_x(),vector_npcs[i].return_y(),vector_npcs[i].return_w(),vector_npcs[i].return_h());
                    }
                    else if(SLIDING && !vector_npcs[i].counts_as_trap &&
                            collision_check(return_x(),return_y(),return_w(),return_h(),vector_npcs[i].return_x(),vector_npcs[i].return_y(),vector_npcs[i].return_w(),vector_npcs[i].return_h())){
                        vector_npcs[i].handle_death();
                    }
                }
            }
        }

        //******************************************//
        // Check for collisions with other players: //
        //******************************************//
        if(!CLIMBING && !CROUCHING && !player.DYING && !player.bubble_mode && !player.CLIMBING && !player.CROUCHING){
            short solidity;
            if(!SWIMMING){
                solidity=TILE_SOLIDITY_CLOUD;
            }
            else{
                solidity=TILE_SOLIDITY_PASSABLE;
            }
            handle_collision_solid(player.return_x(),player.y,player.return_w(),player.h,solidity,NPC_END);

            ///handle_player_platform(-1);

            if(!player.SWIMMING){
                if(push_actor_up(&player,player.return_x(),player.return_w(),being_pushed_up,NPC_END)){
                    player.handle_events(true);
                }
            }
        }
        for(int i=0;i<mp_players.size();i++){
            if(this!=&mp_players[i] && !CLIMBING && !CROUCHING && !mp_players[i].DYING && !mp_players[i].bubble_mode && !mp_players[i].CLIMBING && !mp_players[i].CROUCHING){
                short solidity;
                if(!SWIMMING){
                    solidity=TILE_SOLIDITY_CLOUD;
                }
                else{
                    solidity=TILE_SOLIDITY_PASSABLE;
                }
                handle_collision_solid(mp_players[i].return_x(),mp_players[i].y,mp_players[i].return_w(),mp_players[i].h,solidity,NPC_END);

                ///handle_player_platform(i);

                if(!mp_players[i].SWIMMING){
                    if(push_actor_up(&mp_players[i],mp_players[i].return_x(),mp_players[i].return_w(),being_pushed_up,NPC_END)){
                        mp_players[i].handle_events(true);
                    }
                }
            }
        }

        //**********************************//
        // Check for collisions with doors: //
        //**********************************//
        handle_collision_doors();

        //************************************//
        // Check for collisions with springs: //
        //************************************//
        handle_collision_springs(TILE_SOLIDITY_SOLID);

        //*************************************//
        // Check for collisions with boosters: //
        //*************************************//
        handle_collision_boosters();

        //**********************************//
        // Check for collisions with traps: //
        //**********************************//
        collision_data_trap box_trap=handle_collision_traps(TILE_SOLIDITY_SOLID);
        if(box_trap.w!=-1.0){
            handle_death(box_trap.x,box_trap.y,box_trap.w,box_trap.h);
        }

        //**********************************//
        // Check for collisions with tiles: //
        //**********************************//
        handle_tiles(check_x_start,check_x_end,check_y_start,check_y_end);

        //If the player is underwater.
        if(underwater){
            //If the player is out of oxygen, they have drowned.
            if(oxygen<=0){
                handle_death(x,y,w,h);
            }
        }

        finish_events();
    }
    //If the player is dying.
    else{
        //*********************************//
        // Check for collisions with npcs: //
        //*********************************//
        for(int i=0;i<vector_npcs.size();i++){
            if(fabs(vector_npcs[i].x-x)<PROCESS_RANGE && fabs(vector_npcs[i].y-y)<PROCESS_RANGE){
                //Only do collision checks for the npc if it exists.
                if(vector_npcs[i].exists){
                    //If the player touches any part of the npc, and the npc is deadly to the touch.
                    if(((!vector_npcs[i].special_attack_in_progress && vector_npcs[i].deadly_to_touch) || (vector_npcs[i].special_attack_in_progress && vector_npcs[i].deadly_while_special_attacking)) &&
                       collision_check(return_x(),return_y(),return_w(),return_h(),vector_npcs[i].return_x(),vector_npcs[i].return_y(),vector_npcs[i].return_w(),vector_npcs[i].return_h())){
                        //The player dies!
                        handle_death(vector_npcs[i].return_x(),vector_npcs[i].return_y(),vector_npcs[i].return_w(),vector_npcs[i].return_h());
                    }
                }
            }
        }

        //**********************************//
        // Check for collisions with tiles: //
        //**********************************//
        for(int int_y=check_y_start;int_y<check_y_end;int_y++){
            for(int int_x=check_x_start;int_x<check_x_end;int_x++){
                //As long as the current tile is within the level's boundaries.
                if(int_x>=0 && int_x<=(level.level_x/TILE_SIZE)-1 && int_y>=0 && int_y<=(level.level_y/TILE_SIZE)-1){
                    handle_tile_hazard(int_x,int_y);
                }
            }
        }

        //**********************************//
        // Check for collisions with traps: //
        //**********************************//
        collision_data_trap box_trap=handle_collision_traps(TILE_SOLIDITY_PASSABLE);
        if(box_trap.w!=-1.0){
            handle_death(box_trap.x,box_trap.y,box_trap.w,box_trap.h);
        }

        if(y<0){
            y=0;
            //We set the air velocity to 0, so that the actor immediately begins falling once they bump their head on the top of the level.
            air_velocity=0;

            //The actor has bumped into a wall, so stop moving.
            if(SWIMMING){
                float_speed=0;
            }
        }
    }
}

void Player_Mp::handle_death(double death_x,double death_y,double death_w,double death_h,bool override_invulnerability){
    //As long as the player is not invulnerable and not already dying,
    //he begins dying.
    if((override_invulnerability || !invulnerable) && !DYING){
        DYING=true;
        frame_death=0;
        death_bounces=0;

        //Make sure regular gravity applies when the player dies.
        gravity_max=10.0;
        gravity=0.5;
    }

    //If the player is already dying, increment the death frame.
    if(DYING){
        if(++death_bounces<=6){
            frame_death++;
            if(frame_death>PLAYER_DEATH_SPRITES-1){
                frame_death=0;
            }

            handle_collision_solid(death_x,death_y,death_w,death_h,TILE_SOLIDITY_SOLID,NPC_END);
            if(x+w<=death_x){
                death_direction=true;
            }
            else if(x>=death_x+death_w){
                death_direction=false;
            }
            else{
                death_direction=random_range(0,1);
            }
            air_velocity=-1*(int)random_range(jump_min,jump_max);
            player.play_death_sound(sound_system.player_death);
        }
    }
}

void Player_Mp::animate(){
    //Increment the frame counter.
    frame_counter_idle++;
    frame_counter++;
    frame_counter_swim++;
    frame_counter_jump++;
    frame_counter_shoot++;
    frame_counter_climb++;
    frame_counter_powerup++;

    //Handle idle animation.
    if(frame_counter_idle>=15){
        frame_counter_idle=0;

        //Now increment the frame.
        frame_idle++;

        if(frame_idle>PLAYER_IDLE_SPRITES-1){
            frame_idle=0;
        }
    }

    //Handle player animation.
    if(frame_counter>=8-fabs(run_speed)){
        frame_counter=0;

        //Now increment the frame.
        frame++;

        //If the player is walking, play an occasional footstep sound.
        if(!SWIMMING && !IN_AIR && !SHOOTING && !CLIMBING && !DYING && run_speed!=0.0 && frame%7==0){
            play_footstep_sound();
        }

        if(MOVING_PLATFORM_IN_WATER && fabs(moving_platform_x)>max_speed){
            //As long as the elements of the vector do not exceed the limit.
            if(vector_effect_water_splash.size()<player.option_effect_limit){
                vector_effect_water_splash.push_back(Effect_Water_Splash(x-3,y+h-32+12));
            }
        }

        if(frame>PLAYER_WALK_SPRITES-1){
            frame=0;
        }
    }

    //The swimming animation increments at different rates depending on whether or not the player is moving.
    short swim_frame_limit=0;
    if(move_state==0){
        swim_frame_limit=8;
    }
    else{
        swim_frame_limit=4;
    }

    //Handle swimming animation.
    if(frame_counter_swim>=swim_frame_limit){
        frame_counter_swim=0;

        //Now increment the frame.
        frame_swim++;

        if(frame_swim>PLAYER_SWIM_SPRITES-1){
            frame_swim=0;
        }
    }

    //Handle jumping animation.
    if((frame_counter_jump>=20 && frame_jump<3) || (frame_counter_jump>=30 && frame_jump==3) || (frame_counter_jump>=5 && frame_jump>=4)){
        frame_counter_jump=0;

        //Now increment the frame.
        frame_jump++;

        //If the player has reached the fear frame,
        //but is fairly close to the ground, skip the frame.
        if(frame_jump==3 && distance_to_ground()<=10){
            frame_jump=4;
        }
        //If the fear frame was not skipped, play the fear sound.
        if(IN_AIR && !SHOOTING && frame_jump==3){
            play_positional_sound(sound_system.player_fear,x,y);
        }

        if(frame_jump>PLAYER_JUMP_SPRITES-1){
            frame_jump=4;
        }
    }

    //Handle shooting animation.
    if(frame_counter_shoot>=15 && SHOOTING){
        frame_shoot++;

        if(frame_shoot>PLAYER_SHOOT_SPRITES-1){
            frame_shoot=0;
            SHOOTING=false;
            shoot_render_direction=0;
        }
    }

    //Handle climb animation.
    if(frame_counter_climb>=10){
        frame_counter_climb=0;

        //Now increment the frame.
        frame_climb++;

        if(frame_climb>PLAYER_CLIMB_SPRITES-1){
            frame_climb=1;
        }
    }

    if(frame_counter_powerup>=10){
        frame_counter_powerup=0;

        frame_powerup++;

        if(frame_powerup>POWERUP_SPRITES_JUMP-1){
            frame_powerup=0;
        }
    }

    if(balloon_scale_direction_x){
        balloon_scale_x+=0.01;
        if(balloon_scale_x>1.2){
            balloon_scale_x=1.2;
            balloon_scale_direction_x=!balloon_scale_direction_x;
        }
    }
    else{
        balloon_scale_x-=0.01;
        if(balloon_scale_x<0.80){
            balloon_scale_x=0.80;
            balloon_scale_direction_x=!balloon_scale_direction_x;
        }
    }
    if(balloon_scale_direction_y){
        balloon_scale_y+=0.01;
        if(balloon_scale_y>1.2){
            balloon_scale_y=1.2;
            balloon_scale_direction_y=!balloon_scale_direction_y;
        }
    }
    else{
        balloon_scale_y-=0.01;
        if(balloon_scale_y<0.80){
            balloon_scale_y=0.80;
            balloon_scale_direction_y=!balloon_scale_direction_y;
        }
    }
}

void Player_Mp::render(bool mirrored){
    double render_x=x;
    double render_y=y;
    if(mirrored && touching_mirror(x+MIRROR_DISTANCE_X,y+MIRROR_DISTANCE_Y,w,h)){
        render_x+=MIRROR_DISTANCE_X;
        render_y+=MIRROR_DISTANCE_Y;
    }

    //If the player is dying.
    if(DYING){
        render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*ptr_player_image,&sprites_player_death[frame_death]);
    }
    //If the player is on the ground.
    else if(!SWIMMING && !IN_AIR && !SHOOTING && !CLIMBING && !CROUCHING){
        if(run_speed==0.0 && facing==LEFT){
            render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*ptr_player_image,&sprites_player_idle_left[frame_idle]);
        }
        else if(run_speed==0.0 && facing==RIGHT){
            render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*ptr_player_image,&sprites_player_idle_right[frame_idle]);
        }
        else if(run_speed!=0.0 && facing==LEFT){
            render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*ptr_player_image,&sprites_player_left[frame]);
        }
        else if(run_speed!=0.0 && facing==RIGHT){
            render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*ptr_player_image,&sprites_player_right[frame]);
        }
    }
    //If the player is crouching.
    else if(CROUCHING){
        if(facing==LEFT || facing==LEFT_UP || facing==LEFT_DOWN){
            render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*ptr_player_image,&sprites_player_crouch_left[0]);
        }
        else if(facing==RIGHT || facing==RIGHT_UP || facing==RIGHT_DOWN){
            render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*ptr_player_image,&sprites_player_crouch_right[0]);
        }
    }
    //If the player is swimming.
    else if(SWIMMING){
        if(move_state==0 && facing==LEFT){
            render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*ptr_player_image,&sprites_player_swim_left[frame_swim]);
        }
        else if(move_state==0 && facing==RIGHT){
            render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*ptr_player_image,&sprites_player_swim_right[frame_swim]);
        }
        else if(move_state==UP && facing==RIGHT){
            render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*ptr_player_image,&sprites_player_swim_up_right[frame_swim]);
        }
        else if(move_state==UP && facing==LEFT){
            render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*ptr_player_image,&sprites_player_swim_up_left[frame_swim]);
        }
        else if(move_state==LEFT || move_state==LEFT_DOWN || move_state==LEFT_UP || (move_state==DOWN && facing==LEFT)){
            render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*ptr_player_image,&sprites_player_swim_left[frame_swim]);
        }
        else if(move_state==RIGHT || move_state==RIGHT_DOWN || move_state==RIGHT_UP || (move_state==DOWN && facing==RIGHT)){
            render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*ptr_player_image,&sprites_player_swim_right[frame_swim]);
        }
    }
    //If the player is in the air and not shooting.
    else if(IN_AIR && !SHOOTING){
        if(facing==RIGHT){
            render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*ptr_player_image,&sprites_player_jump_right[frame_jump]);
        }
        else if(facing==LEFT){
            render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*ptr_player_image,&sprites_player_jump_left[frame_jump]);
        }
    }
    //If the player is shooting and not in the air or climbing.
    else if(SHOOTING && !IN_AIR && !CLIMBING){
        if(shoot_render_direction==UP && facing==LEFT){
            render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*ptr_player_image,&sprites_player_shoot_up_left[frame_shoot]);
        }
        else if(shoot_render_direction==UP && facing==RIGHT){
            render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*ptr_player_image,&sprites_player_shoot_up_right[frame_shoot]);
        }
        else if(shoot_render_direction==DOWN && facing==LEFT){
            render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*ptr_player_image,&sprites_player_shoot_down_left_air[frame_shoot]);
        }
        else if(shoot_render_direction==DOWN && facing==RIGHT){
            render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*ptr_player_image,&sprites_player_shoot_down_right_air[frame_shoot]);
        }
        else if(shoot_render_direction==RIGHT){
            render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*ptr_player_image,&sprites_player_shoot_right[frame_shoot]);
        }
        else if(shoot_render_direction==LEFT){
            render_sprite((int)((int)render_x-4-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*ptr_player_image,&sprites_player_shoot_left[frame_shoot]);
        }
    }
    //If the player is shooting and in the air.
    else if(SHOOTING && IN_AIR){
        if(shoot_render_direction==UP && facing==LEFT){
            render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*ptr_player_image,&sprites_player_shoot_up_left_air[frame_shoot]);
        }
        else if(shoot_render_direction==UP && facing==RIGHT){
            render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*ptr_player_image,&sprites_player_shoot_up_right_air[frame_shoot]);
        }
        else if(shoot_render_direction==DOWN && facing==LEFT){
            render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*ptr_player_image,&sprites_player_shoot_down_left_air[frame_shoot]);
        }
        else if(shoot_render_direction==DOWN && facing==RIGHT){
            render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*ptr_player_image,&sprites_player_shoot_down_right_air[frame_shoot]);
        }
        else if(shoot_render_direction==RIGHT){
            render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*ptr_player_image,&sprites_player_shoot_right_air[frame_shoot]);
        }
        else if(shoot_render_direction==LEFT){
            render_sprite((int)((int)render_x-4-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*ptr_player_image,&sprites_player_shoot_left_air[frame_shoot]);
        }
    }
    //If the player is shooting and climbing.
    else if(SHOOTING && CLIMBING){
        if(shoot_render_direction==UP && facing==RIGHT){
            render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*ptr_player_image,&sprites_player_shoot_climb_up_right[frame_shoot]);
        }
        else if(shoot_render_direction==UP && facing==LEFT){
            render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*ptr_player_image,&sprites_player_shoot_climb_up_left[frame_shoot]);
        }
        else if(shoot_render_direction==DOWN && facing==RIGHT){
            render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*ptr_player_image,&sprites_player_shoot_climb_down_right[frame_shoot]);
        }
        else if(shoot_render_direction==DOWN && facing==LEFT){
            render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*ptr_player_image,&sprites_player_shoot_climb_down_left[frame_shoot]);
        }
        else if(shoot_render_direction==RIGHT){
            render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*ptr_player_image,&sprites_player_shoot_climb_right[frame_shoot]);
        }
        else if(shoot_render_direction==LEFT){
            render_sprite((int)((int)render_x-4-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*ptr_player_image,&sprites_player_shoot_climb_left[frame_shoot]);
        }
    }
    //If the player is climbing and not shooting or looking.
    else if(CLIMBING && !SHOOTING){
        if((move_state!=UP && move_state!=DOWN) && facing==RIGHT){
            render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*ptr_player_image,&sprites_player_climb_up_right[0]);
        }
        else if((move_state!=UP && move_state!=DOWN) && facing==LEFT){
            render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*ptr_player_image,&sprites_player_climb_up_left[0]);
        }
        else if(move_state==UP && facing==RIGHT){
            render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*ptr_player_image,&sprites_player_climb_up_right[frame_climb]);
        }
        else if(move_state==UP && facing==LEFT){
            render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*ptr_player_image,&sprites_player_climb_up_left[frame_climb]);
        }
        else if(move_state==DOWN && facing==RIGHT){
            render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*ptr_player_image,&sprites_player_climb_down_right[frame_climb]);
        }
        else if(move_state==DOWN && facing==LEFT){
            render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*ptr_player_image,&sprites_player_climb_down_left[frame_climb]);
        }
    }

    if(counter_jump_mode>0 && !DYING){
        double scale_y=1.0;

        if(CROUCHING){
            scale_y=0.5;
        }

        bool render_wings=true;

        if(counter_jump_mode<240 && counter_jump_mode>=120 && counter_jump_mode%10>=0 && counter_jump_mode%10<=5){
            render_wings=false;
        }
        else if(counter_jump_mode<120 && counter_jump_mode>=60 && counter_jump_mode%10>=6 && counter_jump_mode%10<=10){
            render_wings=false;
        }
        else if(counter_jump_mode<60 && counter_jump_mode>=0 && counter_jump_mode%2==0){
            render_wings=false;
        }

        if(render_wings){
            render_sprite((int)((int)x-8-(int)player.camera_x),(int)((int)y-(int)player.camera_y),image.powerup_jump,&sprites_powerup_jump[frame_powerup],1.0,1.0,scale_y);
        }
    }

    if(bubble_mode && !DYING){
        SDL_Rect balloon_rect;
        balloon_rect.x=0;
        balloon_rect.y=0;
        balloon_rect.w=BALLOON_W;
        balloon_rect.h=BALLOON_H;
        int balloon_width=(int)((double)BALLOON_W*balloon_scale_x);
        int balloon_height=(int)((double)BALLOON_H*balloon_scale_y);
        render_sprite((int)((int)x-(balloon_width-w)/2-(int)player.camera_x),(int)((int)y-(balloon_height-h)/2-(int)player.camera_y),image.balloon,&balloon_rect,1.0,balloon_scale_x,balloon_scale_y,0.0,player.get_player_color(which_mp_player));
    }

    ///render_rectangle((int)(camera_trap_x-camera_x),(int)(camera_trap_y-camera_y),CAMERA_TRAP_W,CAMERA_TRAP_H,0.5,COLOR_RED);

    ///render_rectangle((int)(x-camera_x),(int)(y-camera_y),w,h,0.5,COLOR_BLUE);
    ///render_rectangle((int)((int)return_x()-(int)camera_x),(int)((int)return_y()-(int)camera_y),return_w(),return_h(),0.5,COLOR_RED);
    ///render_rectangle((int)(x-camera_x+w/2),(int)(y-camera_y),1,h,0.5,COLOR_GREEN);
}

void Player_Mp::render_path(){
    if(ai_controlled){
        for(int i=0;i<path.size();i++){
            double render_x=path[i].coords.x*TILE_SIZE;
            double render_y=path[i].coords.y*TILE_SIZE;

            if(render_x>=player.camera_x-TILE_SIZE && render_x<=player.camera_x+player.camera_w && render_y>=player.camera_y-TILE_SIZE && render_y<=player.camera_y+player.camera_h){
                render_rectangle((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),TILE_SIZE,TILE_SIZE,0.75,player.get_player_color(which_mp_player));
            }
        }

        string type_name=get_decision_type();
        font.show((int)((int)x-((type_name.length()*font.spacing_x)/2)+w/2-(int)player.camera_x),(int)((int)y-font.spacing_y-(int)player.camera_y),type_name,COLOR_WHITE);
    }
}

string Player_Mp::get_decision_type(){
    if(decision_type==AI_DECISION_NONE){
        return "None";
    }
    else if(decision_type==AI_DECISION_FOLLOW){
        return "Follow Player";
    }
    else if(decision_type==AI_DECISION_REVIVE){
        return "Revive Player(s)";
    }
    else if(decision_type==AI_DECISION_WANDER){
        return "Wander";
    }
    else if(decision_type==AI_DECISION_ITEM){
        return "Get Item";
    }

    return "Unknown";
}

image_data* Player_Mp::return_character_image(){
    image_data* ptr_image=&image.sprite_sheet_player;

    if(option_character==CHARACTER_HUBERT){
        ptr_image=&image.sprite_sheet_player;
    }
    else if(option_character==CHARACTER_SLIME_O){
        ptr_image=&image.sprite_sheet_character_slime_o;
    }
    else if(option_character==CHARACTER_SKETCH){
        ptr_image=&image.sprite_sheet_character_sketch;
    }
    else if(option_character==CHARACTER_PENNY){
        ptr_image=&image.sprite_sheet_character_penny;
    }

    return ptr_image;
}

image_data* Player_Mp::return_character_worldmap_image(){
    image_data* ptr_image=&image.sprite_sheet_player_worldmap;

    if(option_character==CHARACTER_HUBERT){
        ptr_image=&image.sprite_sheet_player_worldmap;
    }
    else if(option_character==CHARACTER_SLIME_O){
        ptr_image=&image.sprite_sheet_character_worldmap_slime_o;
    }
    else if(option_character==CHARACTER_SKETCH){
        ptr_image=&image.sprite_sheet_character_worldmap_sketch;
    }
    else if(option_character==CHARACTER_PENNY){
        ptr_image=&image.sprite_sheet_character_worldmap_penny;
    }

    return ptr_image;
}

sound_data* Player_Mp::return_character_sound_footstep(){
    sound_data* ptr_sound=&sound_system.player_footstep;

    if(option_character==CHARACTER_HUBERT){
        ptr_sound=&sound_system.player_footstep;
    }
    else if(option_character==CHARACTER_SKETCH){
        ptr_sound=&sound_system.character_sketch_footstep;
    }
    else if(option_character==CHARACTER_SLIME_O){
        ptr_sound=&sound_system.character_slime_o_footstep;
    }
    else if(option_character==CHARACTER_PENNY){
        ptr_sound=&sound_system.character_penny_footstep;
    }

    return ptr_sound;
}

sound_data* Player_Mp::return_character_sound_footstep2(){
    sound_data* ptr_sound=&sound_system.player_footstep2;

    if(option_character==CHARACTER_HUBERT){
        ptr_sound=&sound_system.player_footstep2;
    }
    else if(option_character==CHARACTER_SKETCH){
        ptr_sound=&sound_system.character_sketch_footstep2;
    }
    else if(option_character==CHARACTER_SLIME_O){
        ptr_sound=&sound_system.character_slime_o_footstep2;
    }
    else if(option_character==CHARACTER_PENNY){
        ptr_sound=&sound_system.character_penny_footstep2;
    }

    return ptr_sound;
}

sound_data* Player_Mp::return_character_sound_jump(){
    sound_data* ptr_sound=&sound_system.player_jump;

    if(option_character==CHARACTER_HUBERT){
        ptr_sound=&sound_system.player_jump;
    }
    else if(option_character==CHARACTER_SKETCH){
        ptr_sound=&sound_system.character_sketch_jump;
    }
    else if(option_character==CHARACTER_SLIME_O){
        ptr_sound=&sound_system.character_slime_o_jump;
    }
    else if(option_character==CHARACTER_PENNY){
        ptr_sound=&sound_system.character_penny_jump;
    }

    return ptr_sound;
}

sound_data* Player_Mp::return_character_sound_start_slide(){
    sound_data* ptr_sound=&sound_system.player_start_slide;

    if(option_character==CHARACTER_HUBERT){
        ptr_sound=&sound_system.player_start_slide;
    }
    else if(option_character==CHARACTER_SKETCH){
        ptr_sound=&sound_system.character_sketch_start_slide;
    }
    else if(option_character==CHARACTER_SLIME_O){
        ptr_sound=&sound_system.character_slime_o_start_slide;
    }
    else if(option_character==CHARACTER_PENNY){
        ptr_sound=&sound_system.character_penny_start_slide;
    }

    return ptr_sound;
}

void Player_Mp::mp_reset(double new_x,double new_y){
    uint64_t recall_ammo=ammo;
    double recall_bubble_move_x=bubble_move_x;
    double recall_bubble_move_y=bubble_move_y;
    short recall_counter_jump_mode=counter_jump_mode;

    load_data();

    ammo=recall_ammo;
    bubble_move_x=recall_bubble_move_x;
    bubble_move_y=recall_bubble_move_y;
    counter_jump_mode=recall_counter_jump_mode;

    x=new_x;
    y=new_y;
    if(player.game_mode!=GAME_MODE_MP_ADVENTURE){
        swimming_gear=true;
    }
    air_velocity=((double)random_range((uint32_t)(jump_min*10.0),(uint32_t)(jump_max*0.75*10.0))/10.0)*-1.0;
    IN_AIR=true;
    frame_jump=0;
    if(random_range(0,99)<50){
        run_speed=((double)random_range((uint32_t)(max_speed/4.0*10.0),(uint32_t)(max_speed/2.0*10.0))/10.0)*-1.0;
    }
    else{
        run_speed=(double)random_range((uint32_t)(max_speed/4.0*10.0),(uint32_t)(max_speed/2.0*10.0))/10.0;
    }
}
