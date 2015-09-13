/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "input_data.h"
#include "world.h"

using namespace std;

Input_Data::Input_Data(SDLKey get_key){
    type=INPUT_TYPE_KEYBOARD;

    key=get_key;

    which_joystick=NULL;

    joy_button=NULL;

    joy_axis=NULL;

    joy_axis_direction=false;

    joy_hat=NULL;

    joy_hat_direction=SDL_HAT_CENTERED;

    joy_ball=NULL;

    joy_ball_direction=NONE;
}

Input_Data::Input_Data(){
    type=INPUT_TYPE_KEYBOARD;

    key=SDLK_FIRST;

    which_joystick=NULL;

    joy_button=NULL;

    joy_axis=NULL;

    joy_axis_direction=false;

    joy_hat=NULL;

    joy_hat_direction=SDL_HAT_CENTERED;

    joy_ball=NULL;

    joy_ball_direction=NONE;
}

void Input_Data::set_joy_button(Uint8 get_joystick,Uint8 get_joy_button){
    type=INPUT_TYPE_JOYSTICK_BUTTON;

    key=SDLK_FIRST;

    which_joystick=get_joystick;

    joy_button=get_joy_button;

    joy_axis=NULL;

    joy_axis_direction=false;

    joy_hat=NULL;

    joy_hat_direction=SDL_HAT_CENTERED;

    joy_ball=NULL;

    joy_ball_direction=NONE;
}

void Input_Data::set_joy_axis(Uint8 get_joystick,Uint8 get_joy_axis,bool get_joy_axis_direction){
    type=INPUT_TYPE_JOYSTICK_AXIS;

    key=SDLK_FIRST;

    which_joystick=get_joystick;

    joy_button=NULL;

    joy_axis=get_joy_axis;

    joy_axis_direction=get_joy_axis_direction;

    joy_hat=NULL;

    joy_hat_direction=SDL_HAT_CENTERED;

    joy_ball=NULL;

    joy_ball_direction=NONE;
}

void Input_Data::set_joy_hat(Uint8 get_joystick,Uint8 get_joy_hat,short get_joy_hat_direction){
    type=INPUT_TYPE_JOYSTICK_HAT;

    key=SDLK_FIRST;

    which_joystick=get_joystick;

    joy_button=NULL;

    joy_axis=NULL;

    joy_axis_direction=false;

    joy_hat=get_joy_hat;

    joy_hat_direction=get_joy_hat_direction;

    joy_ball=NULL;

    joy_ball_direction=NONE;
}

void Input_Data::set_joy_ball(Uint8 get_joystick,Uint8 get_joy_ball,short get_joy_ball_direction){
    type=INPUT_TYPE_JOYSTICK_BALL;

    key=SDLK_FIRST;

    which_joystick=get_joystick;

    joy_button=NULL;

    joy_axis=NULL;

    joy_axis_direction=false;

    joy_hat=NULL;

    joy_hat_direction=SDL_HAT_CENTERED;

    joy_ball=get_joy_ball;

    joy_ball_direction=get_joy_ball_direction;
}
