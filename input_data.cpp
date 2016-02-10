/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "input_data.h"
#include "world.h"

using namespace std;

Input_Data::Input_Data(SDL_Scancode get_key){
    type=INPUT_TYPE_KEYBOARD;

    key=get_key;

    which_joystick=0;

    joy_button=NULL;

    joy_axis=NULL;

    joy_axis_direction=false;

    joy_hat=NULL;

    joy_hat_direction=SDL_HAT_CENTERED;

    joy_ball=NULL;

    joy_ball_direction=NONE;

    axis_last_direction="none";
}

Input_Data::Input_Data(){
    type=INPUT_TYPE_KEYBOARD;

    key=SDL_SCANCODE_UNKNOWN;

    which_joystick=0;

    joy_button=NULL;

    joy_axis=NULL;

    joy_axis_direction=false;

    joy_hat=NULL;

    joy_hat_direction=SDL_HAT_CENTERED;

    joy_ball=NULL;

    joy_ball_direction=NONE;

    axis_last_direction="none";
}

void Input_Data::set_joy_button(Uint8 get_joystick,Uint8 get_joy_button){
    type=INPUT_TYPE_JOYSTICK_BUTTON;

    key=SDL_SCANCODE_UNKNOWN;

    which_joystick=get_joystick;

    joy_button=get_joy_button;

    joy_axis=NULL;

    joy_axis_direction=false;

    joy_hat=NULL;

    joy_hat_direction=SDL_HAT_CENTERED;

    joy_ball=NULL;

    joy_ball_direction=NONE;

    axis_last_direction="none";
}

void Input_Data::set_joy_axis(Uint8 get_joystick,Uint8 get_joy_axis,bool get_joy_axis_direction){
    type=INPUT_TYPE_JOYSTICK_AXIS;

    key=SDL_SCANCODE_UNKNOWN;

    which_joystick=get_joystick;

    joy_button=NULL;

    joy_axis=get_joy_axis;

    joy_axis_direction=get_joy_axis_direction;

    joy_hat=NULL;

    joy_hat_direction=SDL_HAT_CENTERED;

    joy_ball=NULL;

    joy_ball_direction=NONE;

    axis_last_direction="none";
}

void Input_Data::set_joy_hat(Uint8 get_joystick,Uint8 get_joy_hat,short get_joy_hat_direction){
    type=INPUT_TYPE_JOYSTICK_HAT;

    key=SDL_SCANCODE_UNKNOWN;

    which_joystick=get_joystick;

    joy_button=NULL;

    joy_axis=NULL;

    joy_axis_direction=false;

    joy_hat=get_joy_hat;

    joy_hat_direction=get_joy_hat_direction;

    joy_ball=NULL;

    joy_ball_direction=NONE;

    axis_last_direction="none";
}

void Input_Data::set_joy_ball(Uint8 get_joystick,Uint8 get_joy_ball,short get_joy_ball_direction){
    type=INPUT_TYPE_JOYSTICK_BALL;

    key=SDL_SCANCODE_UNKNOWN;

    which_joystick=get_joystick;

    joy_button=NULL;

    joy_axis=NULL;

    joy_axis_direction=false;

    joy_hat=NULL;

    joy_hat_direction=SDL_HAT_CENTERED;

    joy_ball=get_joy_ball;

    joy_ball_direction=get_joy_ball_direction;

    axis_last_direction="none";
}

void Input_Data::reset_axis_last_direction(){
    if(type==INPUT_TYPE_JOYSTICK_AXIS){
        for(int i=0;i<joystick.size();i++){
            if(SDL_JoystickGetAttached(joystick[i].joy) && i==which_joystick){
                Sint16 axis_value=SDL_JoystickGetAxis(joystick[i].joy,joy_axis);

                if(axis_value>=JOYSTICK_NEUTRAL_NEGATIVE && axis_value<=JOYSTICK_NEUTRAL_POSITIVE){
                    axis_last_direction="none";

                    break;
                }
            }
        }
    }
}

void Input_Data::set_axis_last_direction_up(){
    axis_last_direction="up";
}

void Input_Data::set_axis_last_direction_down(){
    axis_last_direction="down";
}

bool Input_Data::was_axis_last_direction_up(){
    return axis_last_direction=="up";
}

bool Input_Data::was_axis_last_direction_down(){
    return axis_last_direction=="down";
}

Uint8 Input_Data::joy_instance_to_index(SDL_JoystickID instance_id){
    Uint8 index=0;

    for(int i=0;i<joystick.size();i++){
        if(SDL_JoystickInstanceID(joystick[i].joy)==instance_id){
            index=i;

            break;
        }
    }

    return index;
}
