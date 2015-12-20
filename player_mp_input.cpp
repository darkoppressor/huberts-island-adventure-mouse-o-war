/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "player_mp.h"
#include "enumerations.h"
#include "world.h"
#include "collision.h"
#include "quit.h"

using namespace std;

void Player_Mp::prepare_for_input(){
    crouching_at_frame_start=CROUCHING;

    set_solid_above();
}

void Player_Mp::handle_command_event(short command){
    switch(command){

    case COMMAND_JUMP:
        //If a window is open.
        if(window_manager.which_window_open()!=-1){
        }
        //As long as the game is not paused, and is in progress.
        else if(!player.pause && player.game_in_progress && !bubble_mode){
            //As long as the player isn't dying.
            if(!DYING){
                bool fell_through_cloud=false;

                //If all the conditions for falling through a cloud are met.
                if(command_state(COMMAND_JUMP) && command_state(COMMAND_DOWN) && !IN_AIR && !SHOOTING && !CLIMBING){
                    if(on_cloud){
                        fall_through_cloud();
                        fell_through_cloud=true;
                    }
                }

                //If all the conditions for a jump are met.
                if(!fell_through_cloud && (counter_jump_mode>0 || (extra_jumps>0 && ((SWIMMING && SWIM_CAN_JUMP) || !SWIMMING)) ||
                                           ((!IN_AIR || (IN_AIR && counter_jump_mercy>0)) && !jump_state && ((SWIMMING && SWIM_CAN_JUMP) || !SWIMMING)))){
                    if(extra_jumps>0 && IN_AIR && counter_jump_mercy==0){
                        if(!player.get_upgrade_state("infinite_jumps")){
                            extra_jumps--;
                        }
                    }

                    counter_jump_mercy=0;

                    if(CLIMBING){
                        CLIMBING=false;
                        climb_jump_timer=climb_jump_delay;
                        CLIMBING_JUMP=true;
                    }

                    jump_state=true;

                    if(!CLIMBING_JUMP){
                        air_velocity=jump_max*-1;
                        /**double p_angle=touched_slope_angle;
                        if((p_angle>=0 && p_angle<=90) || (p_angle>=180 && p_angle<=270)){
                            p_angle+=90;
                        }
                        else if((p_angle>90 && p_angle<180) || (p_angle>270 && p_angle<360)){
                            p_angle-=90;
                        }
                        run_speed+=jump_max*(cos(p_angle*(M_PI/180)));
                        air_velocity=jump_max*-(sin(p_angle*(M_PI/180)));*/
                    }
                    else{
                        air_velocity=climbing_jump_max*-1;
                    }

                    //The player has begun a jump, so set them to be in the air.
                    IN_AIR=true;
                    frame_jump=0;

                    //If the player is under the effect of a J Balloon.
                    if(counter_jump_mode>0){
                        play_positional_sound(sound_system.player_jump_j_balloon,x,y);
                    }
                    else{
                        play_positional_sound(*ptr_player_jump,x,y);
                    }
                }
            }
        }
        else if(!player.pause && player.game_in_progress && bubble_mode){
            if(collision_check(x,y,w,h,player.camera_x,player.camera_y,player.camera_w,player.camera_h) && in_open_air()){
                bubble_mode=false;
                air_velocity=-jump_min*1.5;
                IN_AIR=true;
                run_speed=0.0;

                //Prevent the constant bubble popping sound if we are moving the camera around with dev controls.
                if(player.cam_state==CAM_STICKY){
                    play_positional_sound(sound_system.player_bubble_pop);
                }
            }
        }
        break;

    case COMMAND_SHOOT:
        if(window_manager.which_window_open()!=-1){
        }
        //As long as the game is not paused and is in progress.
        else if(!player.pause && player.game_in_progress && !bubble_mode){
            //If the shooting key is pressed and the player has ammo.
            if(!SWIMMING && !SHOOTING && !DYING){
                crouch_stop();

                short shot_cost=1;
                if(current_shot==SHOT_PLAYER){
                    shot_cost=SHOT_COST_PLAYER;
                }
                else if(current_shot==SHOT_PLAYER_SPLODE){
                    shot_cost=SHOT_COST_PLAYER_SPLODE;
                }
                else if(current_shot==SHOT_PLAYER_HOMING){
                    shot_cost=SHOT_COST_PLAYER_HOMING;
                }

                //Pretty straightforward. If shoot_state is non-zero, then the player is holding a directional key, so shoot in the corresponding direction and subtract one ammo.
                //If the player is not holding any directional keys, shoot in the direction that the player is facing and subtract one ammo.
                //An exception is shooting downwards. Shooting down is only allowed while the player is in the air.

                if(shoot_state==UP){
                    if(ammo>=shot_cost){
                        vector_shots.push_back(Shot(x-4,y-13,90,current_shot,false));
                        ammo-=shot_cost;
                    }
                    else{
                        play_positional_sound(sound_system.player_no_ammo,x,y);
                    }

                    //The player is now shooting.
                    SHOOTING=true;
                    shoot_render_direction=UP;
                    //Reset the shooting frame.
                    frame_shoot=0;
                    frame_counter_shoot=0;

                    //Reset other variables.
                    frame=0;
                    frame_counter=0;
                    shoot_state=0;
                }
                else if(shoot_state==DOWN){
                    if(IN_AIR || CLIMBING){
                        if(ammo>=shot_cost){
                            vector_shots.push_back(Shot(x-4,y+h-13,270,current_shot,false));
                            ammo-=shot_cost;
                        }
                        else{
                            play_positional_sound(sound_system.player_no_ammo,x,y);
                        }

                        //The player is now shooting.
                        SHOOTING=true;
                        shoot_render_direction=DOWN;
                        //Reset the shooting frame.
                        frame_shoot=0;
                        frame_counter_shoot=0;

                        //Reset other variables.
                        frame=0;
                        frame_counter=0;
                        shoot_state=0;
                    }
                }
                else if(facing==LEFT || facing==RIGHT){
                    //If the player has any ammo remaining.
                    if(ammo>=shot_cost){
                        if(facing==LEFT){
                            vector_shots.push_back(Shot(x-13,y+12,180,current_shot,false));
                            ammo-=shot_cost;
                        }
                        else if(facing==RIGHT){
                            vector_shots.push_back(Shot(x+w-13,y+12,0,current_shot,false));
                            ammo-=shot_cost;
                        }
                    }
                    else{
                        play_positional_sound(sound_system.player_no_ammo,x,y);
                    }

                    //The player is now shooting.
                    SHOOTING=true;

                    if(facing==LEFT){
                        shoot_render_direction=LEFT;
                    }
                    else if(facing==RIGHT){
                        shoot_render_direction=RIGHT;
                    }

                    //Reset the shooting frame.
                    frame_shoot=0;
                    frame_counter_shoot=0;

                    //Reset other variables.
                    frame=0;
                    frame_counter=0;
                    shoot_state=0;
                }
            }
        }
        break;

    case COMMAND_CHANGE_SHOT:
        if(player.game_in_progress){
            if(shot_homing){
                if(current_shot==SHOT_PLAYER){
                    current_shot=SHOT_PLAYER_HOMING;
                }
                else{
                    current_shot=SHOT_PLAYER;
                }
                play_positional_sound(sound_system.player_change_shot,x,y);
            }
            else{
                play_positional_sound(sound_system.player_no_ammo,x,y);
            }
        }
        break;

    case COMMAND_UP: case COMMAND_LEFT:
        //If a window is open.
        if(window_manager.which_window_open()!=-1){
        }
        else{
            if(command==COMMAND_UP){
                //As long as the game is not paused and is in progress.
                if(!player.pause && player.game_in_progress && !bubble_mode){
                    //If the current level is not the world map.
                    if(!DYING){
                        bool trigger_used=false;

                        for(int i=0;i<vector_triggers.size();i++){
                            if(fabs(vector_triggers[i].x-x)<PROCESS_RANGE && fabs(vector_triggers[i].y-y)<PROCESS_RANGE){
                                //If the trigger is active, its user type is the player, and it is manually activated.
                                if(vector_triggers[i].active && !vector_triggers[i].trigger_method){
                                    if(collision_check(x,y,w,h,vector_triggers[i].x,vector_triggers[i].y,vector_triggers[i].w,vector_triggers[i].h)){
                                        vector_triggers[i].use();
                                        play_positional_sound(sound_system.trigger_click,vector_triggers[i].x,vector_triggers[i].y);
                                        trigger_used=true;
                                        break;
                                    }
                                }
                            }
                        }

                        if(!trigger_used){
                            /**for(int i=0;i<vector_signs.size();i++){
                                if(fabs(vector_signs[i].x-x)<PROCESS_RANGE && fabs(vector_signs[i].y-y)<PROCESS_RANGE){
                                    if(collision_check(x,y,w,h,vector_signs[i].x,vector_signs[i].y,SIGN_W,SIGN_H)){
                                        window_message[0].set_message("",vector_signs[i].message,vector_signs[i].font_type);

                                        play_positional_sound(sound_system.read_sign,x,y);
                                        break;
                                    }
                                }
                            }*/
                        }
                    }
                }
            }
        }
        break;

    }
}

void Player_Mp::crouch_start(){
    if(!CROUCHING && !water_running){
        CROUCHING=true;

        y+=PLAYER_CROUCH_H;

        //Set the player's dimensions to the crouching dimensions.
        w=PLAYER_CROUCH_W;
        h=PLAYER_CROUCH_H;
    }
}

void Player_Mp::crouch_stop(){
    if(CROUCHING && !solid_above){
        //If the player is holding the down key, crouching will be activated.
        CROUCHING=false;

        y-=PLAYER_CROUCH_H;

        //Set the player's dimensions to the standard dimensions.
        w=PLAYER_W;
        h=PLAYER_H;
    }
}

void Player_Mp::handle_input_states(){
    //If the player is alive.
    if(!DYING && !bubble_mode){
        //Begin here not crouching.
        crouch_stop();

        //************************//
        // Handle direction keys: //
        //************************//

        climb_state=NONE;

        //If the left directional key is pressed.
        if(command_state(COMMAND_LEFT)){
            //Set their move state to match the directional key.
            move_state=LEFT;

            //Set their facing variable to match the directional key.
            facing=LEFT;

            //When only one directional key is pressed, shoot_state and move_state are the same.
            shoot_state=move_state;
        }

        //If the up directional key is pressed.
        if(command_state(COMMAND_UP)){
            move_state=UP;

            //When only one directional key is pressed, shoot_state and move_state are the same.
            shoot_state=move_state;

            climb_state=move_state;
        }

        //If the player is swimming and the jump key is pressed, mimic the up direction key's swimming function.
        if(SWIMMING && command_state(COMMAND_JUMP)){
            move_state=UP;
        }

        //If the right directional key is pressed.
        if(command_state(COMMAND_RIGHT)){
            //Set their move state to match the directional key.
            move_state=RIGHT;

            //Set their facing variable to match the directional key.
            facing=RIGHT;

            //When only one directional key is pressed, shoot_state and move_state are the same.
            shoot_state=move_state;
        }

        //If the down directional key is pressed.
        if(command_state(COMMAND_DOWN)){
            move_state=DOWN;

            //When only one directional key is pressed, shoot_state and move_state are the same.
            shoot_state=move_state;

            climb_state=move_state;

            if(!SWIMMING && !CLIMBING && !SHOOTING){
                crouch_start();
            }
        }

        //*******************************************************//
        // Handle multiple direction keys being pressed at once: //
        //*******************************************************//

        //When swimming, 8 directions of movement are possible.
        if(SWIMMING && !CLIMBING){
            //Priorities:
            //Movement: Left, Up, Right, Down.

            if(command_state(COMMAND_LEFT) && (command_state(COMMAND_UP) || command_state(COMMAND_JUMP))){
                move_state=LEFT_UP;
            }
            if(command_state(COMMAND_RIGHT) && (command_state(COMMAND_UP) || command_state(COMMAND_JUMP))){
                move_state=RIGHT_UP;
            }
            if(command_state(COMMAND_RIGHT) && command_state(COMMAND_DOWN)){
                move_state=RIGHT_DOWN;
            }
            if(command_state(COMMAND_LEFT) && command_state(COMMAND_DOWN)){
                move_state=LEFT_DOWN;
            }
            if(command_state(COMMAND_LEFT) && command_state(COMMAND_RIGHT)){
                move_state=LEFT;
            }
            if((command_state(COMMAND_UP) || command_state(COMMAND_JUMP)) && command_state(COMMAND_DOWN)){
                move_state=UP;
            }
            if(command_state(COMMAND_LEFT) && (command_state(COMMAND_UP) || command_state(COMMAND_JUMP)) && command_state(COMMAND_RIGHT)){
                move_state=LEFT_UP;
            }
            if(command_state(COMMAND_LEFT) && command_state(COMMAND_DOWN) && command_state(COMMAND_RIGHT)){
                move_state=LEFT_DOWN;
            }
            if(command_state(COMMAND_LEFT) && (command_state(COMMAND_UP) || command_state(COMMAND_JUMP)) && command_state(COMMAND_DOWN)){
                move_state=LEFT_UP;
            }
            if((command_state(COMMAND_UP) || command_state(COMMAND_JUMP)) && command_state(COMMAND_RIGHT) && command_state(COMMAND_DOWN)){
                move_state=RIGHT_UP;
            }
            if(command_state(COMMAND_LEFT) && (command_state(COMMAND_UP) || command_state(COMMAND_JUMP)) && command_state(COMMAND_RIGHT) && command_state(COMMAND_DOWN)){
                move_state=LEFT_UP;
            }

            //If no directional keys are pressed, the player is not moving.
            if(!command_state(COMMAND_LEFT) && (!command_state(COMMAND_UP) && !command_state(COMMAND_JUMP)) && !command_state(COMMAND_RIGHT) && !command_state(COMMAND_DOWN)){
                move_state=0;
            }
        }

        //When on land or in the air, 4 directions of movement are possible.
        else if(!SWIMMING && !CLIMBING){
            //Priorities:
            //Movement: Left, Up, Right, Down.
            //Shooting on the ground: Up, Left, Right, Down.
            //Shooting in the air: Up, Down, Left, Right.

            if(command_state(COMMAND_LEFT) && command_state(COMMAND_UP)){
                move_state=LEFT;
                facing=LEFT;
                shoot_state=UP;
                climb_state=UP;
            }
            if(command_state(COMMAND_RIGHT) && command_state(COMMAND_UP)){
                move_state=RIGHT;
                facing=RIGHT;
                shoot_state=UP;
                climb_state=UP;
            }
            if(command_state(COMMAND_RIGHT) && command_state(COMMAND_DOWN)){
                move_state=RIGHT;
                facing=RIGHT;
                if(!IN_AIR){
                    shoot_state=RIGHT;
                }
                if(IN_AIR){
                    shoot_state=DOWN;
                }
                climb_state=DOWN;
            }
            if(command_state(COMMAND_LEFT) && command_state(COMMAND_DOWN)){
                move_state=LEFT;
                facing=LEFT;
                if(!IN_AIR){
                    shoot_state=LEFT;
                }
                if(IN_AIR){
                    shoot_state=DOWN;
                }
                climb_state=DOWN;
            }
            if(command_state(COMMAND_LEFT) && command_state(COMMAND_RIGHT)){
                move_state=LEFT;
                facing=LEFT;
                shoot_state=LEFT;
            }
            if(command_state(COMMAND_UP) && command_state(COMMAND_DOWN)){
                move_state=UP;
                shoot_state=UP;
                climb_state=UP;
            }
            if(command_state(COMMAND_LEFT) && command_state(COMMAND_UP) && command_state(COMMAND_RIGHT)){
                move_state=LEFT;
                facing=LEFT;
                shoot_state=UP;
                climb_state=UP;
            }
            if(command_state(COMMAND_LEFT) && command_state(COMMAND_DOWN) && command_state(COMMAND_RIGHT)){
                move_state=LEFT;
                facing=LEFT;
                if(!IN_AIR){
                    shoot_state=LEFT;
                }
                if(IN_AIR){
                    shoot_state=DOWN;
                }
                climb_state=DOWN;
            }
            if(command_state(COMMAND_LEFT) && command_state(COMMAND_UP) && command_state(COMMAND_DOWN)){
                move_state=LEFT;
                facing=LEFT;
                shoot_state=UP;
                climb_state=UP;
            }
            if(command_state(COMMAND_UP) && command_state(COMMAND_RIGHT) && command_state(COMMAND_DOWN)){
                move_state=RIGHT;
                facing=RIGHT;
                shoot_state=UP;
                climb_state=UP;
            }
            if(command_state(COMMAND_LEFT) && command_state(COMMAND_UP) && command_state(COMMAND_RIGHT) && command_state(COMMAND_DOWN)){
                move_state=LEFT;
                facing=LEFT;
                shoot_state=UP;
                climb_state=UP;
            }

            //If no directional keys are pressed, the player is not moving.
            if(!command_state(COMMAND_LEFT) && !command_state(COMMAND_UP) && !command_state(COMMAND_RIGHT) && !command_state(COMMAND_DOWN)){
                move_state=0;
                shoot_state=0;
                climb_state=0;
            }
        }

        //When climbing, only 2 directions of movement are possible.
        else if(CLIMBING && !SWIMMING){
            //Priorities:
            //Movement: Up, Down.

            if(command_state(COMMAND_LEFT) && command_state(COMMAND_UP)){
                move_state=UP;
                facing=LEFT;
                shoot_state=UP;
            }
            if(command_state(COMMAND_RIGHT) && command_state(COMMAND_UP)){
                move_state=UP;
                facing=RIGHT;
                shoot_state=UP;
            }
            if(command_state(COMMAND_RIGHT) && command_state(COMMAND_DOWN)){
                move_state=DOWN;
                facing=RIGHT;
                shoot_state=DOWN;
            }
            if(command_state(COMMAND_LEFT) && command_state(COMMAND_DOWN)){
                move_state=DOWN;
                facing=LEFT;
                shoot_state=DOWN;
            }
            if(command_state(COMMAND_LEFT) && command_state(COMMAND_RIGHT)){
                facing=LEFT;
                shoot_state=LEFT;
            }
            if(command_state(COMMAND_UP) && command_state(COMMAND_DOWN)){
                move_state=UP;
                shoot_state=UP;
            }
            if(command_state(COMMAND_LEFT) && command_state(COMMAND_UP) && command_state(COMMAND_RIGHT)){
                move_state=UP;
                facing=LEFT;
                shoot_state=UP;
            }
            if(command_state(COMMAND_LEFT) && command_state(COMMAND_DOWN) && command_state(COMMAND_RIGHT)){
                move_state=DOWN;
                facing=LEFT;
                shoot_state=DOWN;
            }
            if(command_state(COMMAND_LEFT) && command_state(COMMAND_UP) && command_state(COMMAND_DOWN)){
                move_state=UP;
                facing=LEFT;
                shoot_state=UP;
            }
            if(command_state(COMMAND_UP) && command_state(COMMAND_RIGHT) && command_state(COMMAND_DOWN)){
                move_state=UP;
                facing=RIGHT;
                shoot_state=UP;
            }
            if(command_state(COMMAND_LEFT) && command_state(COMMAND_UP) && command_state(COMMAND_RIGHT) && command_state(COMMAND_DOWN)){
                move_state=UP;
                facing=LEFT;
                shoot_state=UP;
            }

            //If no directional keys are pressed, the player is not moving.
            if(!command_state(COMMAND_LEFT) && !command_state(COMMAND_UP) && !command_state(COMMAND_RIGHT) && !command_state(COMMAND_DOWN)){
                frame=0;
                frame_counter=0;
                move_state=0;
                shoot_state=0;
            }
        }

        if(CROUCHING){
            frame=0;
            frame_counter=0;
            if(!IN_AIR){
                move_state=0;
            }
        }

        //*****************//
        // Handle jumping: //
        //*****************//

        if(!command_state(COMMAND_JUMP) && jump_state){
            jump_state=false;

            if(!CLIMBING_JUMP && air_velocity<jump_min*-1){
                air_velocity=jump_min*-1;
            }
            else if(CLIMBING_JUMP && air_velocity<climbing_jump_min*-1){
                air_velocity=climbing_jump_min*-1;
            }

            CLIMBING_JUMP=false;
        }

        after_input();
    }
}

bool Player_Mp::command_state(short command){
    const uint8_t* keystates=SDL_GetKeyboardState(NULL);

    Sint16 axis_value=0;
    Uint8 hat_value=SDL_HAT_CENTERED;
    short ball_direction=NONE;
    int ball_delta_x=0;
    int ball_delta_y=0;

    if((ai_controlled || player.on_worldmap()) && ai_keystates[command]){
        return true;
    }

    if(!player.on_worldmap()){
        //If this command's bound input is a keyboard key, and the key is being pressed.
        if(keys[command].type==INPUT_TYPE_KEYBOARD && keystates[keys[command].key]){
            return true;
        }

        //Check all available joysticks for input.
        for(int i=0;i<joystick.size();i++){
            //As long as this joystick is opened properly, and is the joystick associated with this command.
            if(SDL_JoystickGetAttached(joystick[i].joy) && i==keys[command].which_joystick){
                //If this command's bound input is a joystick button, and the button is being pressed.
                if(keys[command].type==INPUT_TYPE_JOYSTICK_BUTTON && SDL_JoystickGetButton(joystick[i].joy,keys[command].joy_button)){
                    return true;
                }

                //If this command's bound input is a joystick axis.
                else if(keys[command].type==INPUT_TYPE_JOYSTICK_AXIS){
                    //Get the current value of this command's bound axis.
                    axis_value=SDL_JoystickGetAxis(joystick[i].joy,keys[command].joy_axis);

                    //If the axis is outside of the neutral zone and held in the correct direction.
                    if(!keys[command].joy_axis_direction && axis_value<JOYSTICK_NEUTRAL_NEGATIVE){
                        return true;
                    }
                    else if(keys[command].joy_axis_direction && axis_value>JOYSTICK_NEUTRAL_POSITIVE){
                        return true;
                    }
                }

                //If this command's bound input is a joystick hat.
                else if(keys[command].type==INPUT_TYPE_JOYSTICK_HAT){
                    //Get the current value of this command's bound hat.
                    hat_value=SDL_JoystickGetHat(joystick[i].joy,keys[command].joy_hat);

                    //If the hat is held in the correct direction.
                    if(hat_value==keys[command].joy_hat_direction){
                        return true;
                    }
                }

                //If this command's bound input is a joystick ball.
                else if(keys[command].type==INPUT_TYPE_JOYSTICK_BALL){
                    //Get the current delta movement of this command's bound ball.
                    SDL_JoystickGetBall(joystick[i].joy,keys[command].joy_ball,&ball_delta_x,&ball_delta_y);

                    //Set the ball's direction using the deltas.
                    if(ball_delta_x<0 && ball_delta_y<0){
                        ball_direction=LEFT_UP;
                    }
                    else if(ball_delta_x<0 && ball_delta_y>0){
                        ball_direction=LEFT_DOWN;
                    }
                    else if(ball_delta_x>0 && ball_delta_y<0){
                        ball_direction=RIGHT_UP;
                    }
                    else if(ball_delta_x>0 && ball_delta_y>0){
                        ball_direction=RIGHT_DOWN;
                    }
                    else if(ball_delta_x<0){
                        ball_direction=LEFT;
                    }
                    else if(ball_delta_x>0){
                        ball_direction=RIGHT;
                    }
                    else if(ball_delta_y<0){
                        ball_direction=UP;
                    }
                    else if(ball_delta_y>0){
                        ball_direction=DOWN;
                    }

                    //If the ball has moved in the correct direction.
                    if(ball_direction==keys[command].joy_ball_direction){
                        return true;
                    }
                }
            }
        }
    }

    //If neither the key nor the joystick input bound to this command are active,
    //return false to let the game know this command is not currently being given.
    return false;
}

void Player_Mp::handle_input_events(){
    if(ai_controlled){
        while(ai_key_events.size()>0){
            handle_command_event(ai_key_events[ai_key_events.size()-1]);

            ai_key_events.pop_back();
        }
    }
    else{
        if(event.type==SDL_QUIT){
            quit_game();
            return;
        }

        bool allow_input_event=true;

        //As long as the player's input events are not being blocked.
        if(allow_input_event){
            //Look through all commands, and see if any should be triggered by this input event.
            for(int i=0;i<keys.size();i++){
                SDL_Joystick* joystick_ptr=0;

                if(keys[i].which_joystick<joystick.size()){
                    joystick_ptr=joystick[keys[i].which_joystick].joy;
                }

                //If the input type is a keyboard keypress, and the event is a keyboard keypress.
                if(keys[i].type==INPUT_TYPE_KEYBOARD && event.type==SDL_KEYDOWN){
                    if(event.key.keysym.scancode==keys[i].key){
                        handle_command_event(i);
                        return;
                    }
                }
                //If the input type is a joystick button press, and the event is a joystick button press, and the event joystick is the command's bound joystick.
                else if(keys[i].type==INPUT_TYPE_JOYSTICK_BUTTON && event.type==SDL_JOYBUTTONDOWN && event.jbutton.which==SDL_JoystickInstanceID(joystick_ptr)){
                    if(event.jbutton.button==keys[i].joy_button){
                        handle_command_event(i);
                        return;
                    }
                }
                //If the input type is a joystick axis motion, and the event is a joystick axis motion, and the event joystick is the command's bound joystick.
                else if(keys[i].type==INPUT_TYPE_JOYSTICK_AXIS && event.type==SDL_JOYAXISMOTION && event.jaxis.which==SDL_JoystickInstanceID(joystick_ptr)){
                    if(event.jaxis.axis==keys[i].joy_axis){
                        if(!keys[i].joy_axis_direction && event.jaxis.value<JOYSTICK_NEUTRAL_NEGATIVE){
                            handle_command_event(i);
                            return;
                        }
                        else if(keys[i].joy_axis_direction && event.jaxis.value>JOYSTICK_NEUTRAL_POSITIVE){
                            handle_command_event(i);
                            return;
                        }
                    }
                }
                //If the input type is a joystick hat motion, and the event is a joystick hat motion, and the event joystick is the command's bound joystick.
                else if(keys[i].type==INPUT_TYPE_JOYSTICK_HAT && event.type==SDL_JOYHATMOTION && event.jhat.which==SDL_JoystickInstanceID(joystick_ptr)){
                    if(event.jhat.hat==keys[i].joy_hat){
                        if(keys[i].joy_hat_direction==event.jhat.value){
                            handle_command_event(i);
                            return;
                        }
                    }
                }
                //If the input type is a joystick ball motion, and the event is a joystick ball motion, and the event joystick is the command's bound joystick.
                else if(keys[i].type==INPUT_TYPE_JOYSTICK_BALL && event.type==SDL_JOYBALLMOTION && event.jball.which==SDL_JoystickInstanceID(joystick_ptr)){
                    if(event.jball.ball==keys[i].joy_ball){
                        short ball_direction=NONE;

                        //Set the ball's direction using the deltas.
                        if(event.jball.xrel<0 && event.jball.yrel<0){
                            ball_direction=LEFT_UP;
                        }
                        else if(event.jball.xrel<0 && event.jball.yrel>0){
                            ball_direction=LEFT_DOWN;
                        }
                        else if(event.jball.xrel>0 && event.jball.yrel<0){
                            ball_direction=RIGHT_UP;
                        }
                        else if(event.jball.xrel>0 && event.jball.yrel>0){
                            ball_direction=RIGHT_DOWN;
                        }
                        else if(event.jball.xrel<0){
                            ball_direction=LEFT;
                        }
                        else if(event.jball.xrel>0){
                            ball_direction=RIGHT;
                        }
                        else if(event.jball.yrel<0){
                            ball_direction=UP;
                        }
                        else if(event.jball.yrel>0){
                            ball_direction=DOWN;
                        }

                        //If the ball has moved in the correct direction.
                        if(ball_direction==keys[i].joy_ball_direction){
                            handle_command_event(i);
                            return;
                        }
                    }
                }
            }
        }
    }
}
