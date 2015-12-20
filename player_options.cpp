/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "player.h"
#include "world.h"
#include "options.h"
#include "quit.h"

using namespace std;

void Player::toggle_fullscreen(){
    option_fullscreen=!option_fullscreen;
    global_options_save();

    //An option has been changed
    profile.update_achievements(ACHIEVEMENT_CHANGE_AN_OPTION);

    need_to_reinit=true;
}

void Player::toggle_fps(){
    option_fps=!option_fps;
    options_save();

    //An option has been changed
    profile.update_achievements(ACHIEVEMENT_CHANGE_AN_OPTION);
}

void Player::toggle_holiday_cheer(){
    option_holiday_cheer=!option_holiday_cheer;
    options_save();

    //An option has been changed
    profile.update_achievements(ACHIEVEMENT_CHANGE_AN_OPTION);

    need_to_reinit=true;

    music.reinitialize_menu_track();
    if(!game_in_progress){
        music.restart_track(MUSIC_MENU);
        music.play_track(MUSIC_MENU,1.0);
    }
}

void Player::toggle_time_of_day(){
    option_time_of_day=!option_time_of_day;
    options_save();

    //An option has been changed
    profile.update_achievements(ACHIEVEMENT_CHANGE_AN_OPTION);
}

void Player::toggle_time_of_day_levels(){
    option_time_of_day_levels=!option_time_of_day_levels;
    options_save();

    //An option has been changed
    profile.update_achievements(ACHIEVEMENT_CHANGE_AN_OPTION);
}

void Player::toggle_difficulty(){
    option_difficulty++;
    if(option_difficulty>DIFFICULTY_HARD){
        option_difficulty=DIFFICULTY_EASY;
    }
    options_save();

    //An option has been changed
    profile.update_achievements(ACHIEVEMENT_CHANGE_AN_OPTION);
}

void Player::toggle_hints(){
    option_hints=!option_hints;
    options_save();

    //An option has been changed
    profile.update_achievements(ACHIEVEMENT_CHANGE_AN_OPTION);
}

void Player::toggle_character(){
    option_character++;
    if(option_character>=CHARACTER_END){
        option_character=CHARACTER_BEGIN;
    }
    profile.save_inventory();

    ptr_player_worldmap_image=return_character_worldmap_image();

    ptr_player_worldmap_footstep=return_character_sound_footstep();
    ptr_player_worldmap_footstep2=return_character_sound_footstep2();
}

void Player::toggle_hardware_cursor(){
    option_hardware_cursor=!option_hardware_cursor;
    options_save();

    //Show or hide the hardware mouse cursor.
    if(option_hardware_cursor){
        SDL_ShowCursor(SDL_ENABLE);
    }
    else{
        SDL_ShowCursor(SDL_DISABLE);
    }

    //An option has been changed
    profile.update_achievements(ACHIEVEMENT_CHANGE_AN_OPTION);
}

void Player::toggle_mute_global(){
    option_mute_global=!option_mute_global;
    options_save();

    //An option has been changed
    profile.update_achievements(ACHIEVEMENT_CHANGE_AN_OPTION);
}

void Player::toggle_mute_sound(){
    option_mute_sound=!option_mute_sound;
    options_save();

    //An option has been changed
    profile.update_achievements(ACHIEVEMENT_CHANGE_AN_OPTION);
}

void Player::toggle_mute_music(){
    option_mute_music=!option_mute_music;
    options_save();

    //An option has been changed
    profile.update_achievements(ACHIEVEMENT_CHANGE_AN_OPTION);
}

void Player::volume_up_global(){
    option_volume_global+=0.1;

    if(option_volume_global>1.0){
        option_volume_global=1.0;
    }

    options_save();

    //An option has been changed
    profile.update_achievements(ACHIEVEMENT_CHANGE_AN_OPTION);
}

void Player::volume_down_global(){
    option_volume_global-=0.1;

    if(option_volume_global<0.0){
        option_volume_global=0.0;
    }

    options_save();

    //An option has been changed
    profile.update_achievements(ACHIEVEMENT_CHANGE_AN_OPTION);
}

void Player::volume_up_sound(){
    option_volume_sound+=0.05;

    if(option_volume_sound>0.5){
        option_volume_sound=0.5;
    }

    options_save();

    //An option has been changed
    profile.update_achievements(ACHIEVEMENT_CHANGE_AN_OPTION);
}

void Player::volume_down_sound(){
    option_volume_sound-=0.05;

    if(option_volume_sound<0.0){
        option_volume_sound=0.0;
    }

    options_save();

    //An option has been changed
    profile.update_achievements(ACHIEVEMENT_CHANGE_AN_OPTION);
}

void Player::volume_up_music(){
    option_volume_music+=0.015;

    if(option_volume_music>0.15){
        option_volume_music=0.15;
    }

    options_save();

    //An option has been changed
    profile.update_achievements(ACHIEVEMENT_CHANGE_AN_OPTION);
}

void Player::volume_down_music(){
    option_volume_music-=0.015;

    if(option_volume_music<0.0){
        option_volume_music=0.0;
    }

    options_save();

    //An option has been changed
    profile.update_achievements(ACHIEVEMENT_CHANGE_AN_OPTION);
}

void Player::begin_setting_command(short command,short player_to_set){
    command_to_set=command;
    command_to_set_player=player_to_set;
    input_already_bound=-1;
    input_already_bound_player=-1;
}

bool Player::check_key(Input_Data checked_key){
    //Check through all of the commands.
    for(int i=0;i<keys.size();i++){
        //If the current command has the same input type as the checked command, and if the command is not the one being set.
        //We do the second check so if a command is set to enter, and we are setting that command, enter will not count as taken.
        if(keys[i].type==checked_key.type && (i!=command_to_set || -1!=command_to_set_player)){
            switch(checked_key.type){

            case INPUT_TYPE_KEYBOARD:
                if(keys[i].key==checked_key.key){
                    input_already_bound=i;
                    input_already_bound_player=-1;
                    return true;
                }
                break;

            case INPUT_TYPE_JOYSTICK_BUTTON:
                if(keys[i].which_joystick==checked_key.which_joystick && keys[i].joy_button==checked_key.joy_button){
                    input_already_bound=i;
                    input_already_bound_player=-1;
                    return true;
                }
                break;

            case INPUT_TYPE_JOYSTICK_AXIS:
                if(keys[i].which_joystick==checked_key.which_joystick && keys[i].joy_axis==checked_key.joy_axis && keys[i].joy_axis_direction==checked_key.joy_axis_direction){
                    input_already_bound=i;
                    input_already_bound_player=-1;
                    return true;
                }
                break;

            case INPUT_TYPE_JOYSTICK_HAT:
                if(keys[i].which_joystick==checked_key.which_joystick && keys[i].joy_hat==checked_key.joy_hat && keys[i].joy_hat_direction==checked_key.joy_hat_direction){
                    input_already_bound=i;
                    input_already_bound_player=-1;
                    return true;
                }
                break;

            case INPUT_TYPE_JOYSTICK_BALL:
                if(keys[i].which_joystick==checked_key.which_joystick && keys[i].joy_ball==checked_key.joy_ball && keys[i].joy_ball_direction==checked_key.joy_ball_direction){
                    input_already_bound=i;
                    input_already_bound_player=-1;
                    return true;
                }
                break;

            }
        }
    }

    for(int i=0;i<mp_keys.size();i++){
        for(int n=0;n<mp_keys[i].size();n++){
            //If the current command has the same input type as the checked command, and if the command is not the one being set.
            //We do the second check so if a command is set to enter, and we are setting that command, enter will not count as taken.
            if(mp_keys[i][n].type==checked_key.type && (n!=command_to_set || i!=command_to_set_player)){
                switch(checked_key.type){

                case INPUT_TYPE_KEYBOARD:
                    if(mp_keys[i][n].key==checked_key.key){
                        input_already_bound=n;
                        input_already_bound_player=i;
                        return true;
                    }
                    break;

                case INPUT_TYPE_JOYSTICK_BUTTON:
                    if(mp_keys[i][n].which_joystick==checked_key.which_joystick && mp_keys[i][n].joy_button==checked_key.joy_button){
                        input_already_bound=n;
                        input_already_bound_player=i;
                        return true;
                    }
                    break;

                case INPUT_TYPE_JOYSTICK_AXIS:
                    if(mp_keys[i][n].which_joystick==checked_key.which_joystick && mp_keys[i][n].joy_axis==checked_key.joy_axis && mp_keys[i][n].joy_axis_direction==checked_key.joy_axis_direction){
                        input_already_bound=n;
                        input_already_bound_player=i;
                        return true;
                    }
                    break;

                case INPUT_TYPE_JOYSTICK_HAT:
                    if(mp_keys[i][n].which_joystick==checked_key.which_joystick && mp_keys[i][n].joy_hat==checked_key.joy_hat && mp_keys[i][n].joy_hat_direction==checked_key.joy_hat_direction){
                        input_already_bound=n;
                        input_already_bound_player=i;
                        return true;
                    }
                    break;

                case INPUT_TYPE_JOYSTICK_BALL:
                    if(mp_keys[i][n].which_joystick==checked_key.which_joystick && mp_keys[i][n].joy_ball==checked_key.joy_ball && mp_keys[i][n].joy_ball_direction==checked_key.joy_ball_direction){
                        input_already_bound=n;
                        input_already_bound_player=i;
                        return true;
                    }
                    break;

                }
            }
        }
    }

    return false;
}

void Player::check_for_command_set(){
    //If we are currently waiting for a key to be hit.
    if(command_to_set!=-1){
        Input_Data key_to_check;

        switch(event.type){
            case SDL_QUIT:
                quit_game();
                break;

            case SDL_KEYDOWN:
                //Here I can place any keys that should not be available for binding.
                if(event.key.keysym.scancode==SDL_SCANCODE_ESCAPE || event.key.keysym.scancode==SDL_SCANCODE_AC_BACK || event.key.keysym.scancode==SDL_SCANCODE_MENU){
                }
                //Otherwise, assign the new key to the command.
                else{
                    key_to_check.type=INPUT_TYPE_KEYBOARD;
                    key_to_check.key=event.key.keysym.scancode;

                    //If the key is not already bound to a command.
                    if(!check_key(key_to_check)){
                        if(command_to_set_player==-1){
                            keys[command_to_set].type=key_to_check.type;
                            keys[command_to_set].key=key_to_check.key;
                        }
                        else{
                            mp_keys[command_to_set_player][command_to_set].type=key_to_check.type;
                            mp_keys[command_to_set_player][command_to_set].key=key_to_check.key;
                        }

                        command_to_set=-1;
                        options_save();
                    }
                }
                break;

            case SDL_JOYBUTTONDOWN:
                key_to_check.type=INPUT_TYPE_JOYSTICK_BUTTON;
                key_to_check.which_joystick=Input_Data::joy_instance_to_index(event.jbutton.which);
                key_to_check.joy_button=event.jbutton.button;

                //If the joystick button is not already bound to a command.
                if(!check_key(key_to_check)){
                    if(command_to_set_player==-1){
                        keys[command_to_set].type=key_to_check.type;
                        keys[command_to_set].which_joystick=key_to_check.which_joystick;
                        keys[command_to_set].joy_button=key_to_check.joy_button;
                    }
                    else{
                        mp_keys[command_to_set_player][command_to_set].type=key_to_check.type;
                        mp_keys[command_to_set_player][command_to_set].which_joystick=key_to_check.which_joystick;
                        mp_keys[command_to_set_player][command_to_set].joy_button=key_to_check.joy_button;
                    }

                    command_to_set=-1;
                    options_save();
                }
                break;

            case SDL_JOYAXISMOTION:
                //If the axis is outside the neutral zone and held to the negative direction.
                if(event.jaxis.value<JOYSTICK_NEUTRAL_NEGATIVE){
                    key_to_check.type=INPUT_TYPE_JOYSTICK_AXIS;
                    key_to_check.which_joystick=Input_Data::joy_instance_to_index(event.jaxis.which);
                    key_to_check.joy_axis=event.jaxis.axis;
                    key_to_check.joy_axis_direction=false;

                    //If the joystick axis is not already bound to a command.
                    if(!check_key(key_to_check)){
                        if(command_to_set_player==-1){
                            keys[command_to_set].type=key_to_check.type;
                            keys[command_to_set].which_joystick=key_to_check.which_joystick;
                            keys[command_to_set].joy_axis=key_to_check.joy_axis;
                            keys[command_to_set].joy_axis_direction=key_to_check.joy_axis_direction;
                        }
                        else{
                            mp_keys[command_to_set_player][command_to_set].type=key_to_check.type;
                            mp_keys[command_to_set_player][command_to_set].which_joystick=key_to_check.which_joystick;
                            mp_keys[command_to_set_player][command_to_set].joy_axis=key_to_check.joy_axis;
                            mp_keys[command_to_set_player][command_to_set].joy_axis_direction=key_to_check.joy_axis_direction;
                        }

                        command_to_set=-1;
                        options_save();
                    }
                }
                //If the axis is outside the neutral zone and held to the positive direction.
                else if(event.jaxis.value>JOYSTICK_NEUTRAL_POSITIVE){
                    key_to_check.type=INPUT_TYPE_JOYSTICK_AXIS;
                    key_to_check.which_joystick=Input_Data::joy_instance_to_index(event.jaxis.which);
                    key_to_check.joy_axis=event.jaxis.axis;
                    key_to_check.joy_axis_direction=true;

                    //If the joystick axis is not already bound to a command.
                    if(!check_key(key_to_check)){
                        if(command_to_set_player==-1){
                            keys[command_to_set].type=key_to_check.type;
                            keys[command_to_set].which_joystick=key_to_check.which_joystick;
                            keys[command_to_set].joy_axis=key_to_check.joy_axis;
                            keys[command_to_set].joy_axis_direction=key_to_check.joy_axis_direction;
                        }
                        else{
                            mp_keys[command_to_set_player][command_to_set].type=key_to_check.type;
                            mp_keys[command_to_set_player][command_to_set].which_joystick=key_to_check.which_joystick;
                            mp_keys[command_to_set_player][command_to_set].joy_axis=key_to_check.joy_axis;
                            mp_keys[command_to_set_player][command_to_set].joy_axis_direction=key_to_check.joy_axis_direction;
                        }

                        command_to_set=-1;
                        options_save();
                    }
                }
                break;

            case SDL_JOYHATMOTION:
                //If the hat is not centered.
                if(event.jhat.value!=SDL_HAT_CENTERED){
                    key_to_check.type=INPUT_TYPE_JOYSTICK_HAT;
                    key_to_check.which_joystick=Input_Data::joy_instance_to_index(event.jhat.which);
                    key_to_check.joy_hat=event.jhat.hat;
                    key_to_check.joy_hat_direction=event.jhat.value;

                    //If the joystick hat is not already bound to a command.
                    if(!check_key(key_to_check)){
                        if(command_to_set_player==-1){
                            keys[command_to_set].type=key_to_check.type;
                            keys[command_to_set].which_joystick=key_to_check.which_joystick;
                            keys[command_to_set].joy_hat=key_to_check.joy_hat;
                            keys[command_to_set].joy_hat_direction=key_to_check.joy_hat_direction;
                        }
                        else{
                            mp_keys[command_to_set_player][command_to_set].type=key_to_check.type;
                            mp_keys[command_to_set_player][command_to_set].which_joystick=key_to_check.which_joystick;
                            mp_keys[command_to_set_player][command_to_set].joy_hat=key_to_check.joy_hat;
                            mp_keys[command_to_set_player][command_to_set].joy_hat_direction=key_to_check.joy_hat_direction;
                        }

                        command_to_set=-1;
                        options_save();
                    }
                }
                break;

            case SDL_JOYBALLMOTION:
                key_to_check.type=INPUT_TYPE_JOYSTICK_BALL;
                key_to_check.which_joystick=Input_Data::joy_instance_to_index(event.jball.which);
                key_to_check.joy_ball=event.jball.ball;
                ///I don't know if this is correct.
                ///I am assuming that on the x-axis, negative relative motion means left and positive relative motion means right.
                ///Ditto for the y-axis.
                if(event.jball.xrel<0 && event.jball.yrel<0){
                    key_to_check.joy_ball_direction=LEFT_UP;
                }
                else if(event.jball.xrel<0 && event.jball.yrel>0){
                    key_to_check.joy_ball_direction=LEFT_DOWN;
                }
                else if(event.jball.xrel>0 && event.jball.yrel<0){
                    key_to_check.joy_ball_direction=RIGHT_UP;
                }
                else if(event.jball.xrel>0 && event.jball.yrel>0){
                    key_to_check.joy_ball_direction=RIGHT_DOWN;
                }
                else if(event.jball.xrel<0){
                    key_to_check.joy_ball_direction=LEFT;
                }
                else if(event.jball.xrel>0){
                    key_to_check.joy_ball_direction=RIGHT;
                }
                else if(event.jball.yrel<0){
                    key_to_check.joy_ball_direction=UP;
                }
                else if(event.jball.yrel>0){
                    key_to_check.joy_ball_direction=DOWN;
                }

                //If the joystick ball is not already bound to a command.
                if(!check_key(key_to_check)){
                    if(command_to_set_player==-1){
                        keys[command_to_set].type=key_to_check.type;
                        keys[command_to_set].which_joystick=key_to_check.which_joystick;
                        keys[command_to_set].joy_ball=key_to_check.joy_ball;
                        keys[command_to_set].joy_ball_direction=key_to_check.joy_ball_direction;
                    }
                    else{
                        mp_keys[command_to_set_player][command_to_set].type=key_to_check.type;
                        mp_keys[command_to_set_player][command_to_set].which_joystick=key_to_check.which_joystick;
                        mp_keys[command_to_set_player][command_to_set].joy_ball=key_to_check.joy_ball;
                        mp_keys[command_to_set_player][command_to_set].joy_ball_direction=key_to_check.joy_ball_direction;
                    }

                    command_to_set=-1;
                    options_save();
                }
                break;
        }

        //If a command was just set.
        if(command_to_set==-1){
            //An option has been changed
            profile.update_achievements(ACHIEVEMENT_CHANGE_AN_OPTION);
        }
    }
}

short Player::return_input_already_bound(){
    return input_already_bound;
}

short Player::current_command_to_set(){
    return command_to_set;
}
