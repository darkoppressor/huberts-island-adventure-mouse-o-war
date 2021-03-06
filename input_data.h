/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef input_data_h
#define input_data_h

#include <string>

#include <SDL.h>

class Input_Data{
    private:
    public:
    Input_Data(SDL_Scancode get_key);
    Input_Data();

    void set_joy_button(Uint8 get_joystick,Uint8 get_joy_button);
    void set_joy_axis(Uint8 get_joystick,Uint8 get_joy_axis,bool get_joy_axis_direction);
    void set_joy_hat(Uint8 get_joystick,Uint8 get_joy_hat,short get_joy_hat_direction);
    void set_joy_ball(Uint8 get_joystick,Uint8 get_joy_ball,short get_joy_ball_direction);

    void reset_axis_last_direction();
    void set_axis_last_direction_up();
    void set_axis_last_direction_down();
    bool was_axis_last_direction_up();
    bool was_axis_last_direction_down();

    static Uint8 joy_instance_to_index(SDL_JoystickID instance_id);

    short type;
    SDL_Scancode key;
    Uint8 which_joystick;
    Uint8 joy_button;
    Uint8 joy_axis;
    bool joy_axis_direction;
    Uint8 joy_hat;
    short joy_hat_direction;
    Uint8 joy_ball;
    short joy_ball_direction;

    std::string axis_last_direction;
};

#endif
