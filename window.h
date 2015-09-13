/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef window_h
#define window_h

#include <vector>
#include <string>

#include "button.h"
#include "information.h"

class Window{
    protected:

    //The coordinate location and dimensions.
    short x,y;
    short w,h;

    //The offsets from the mouse to the edge of the window.
    //Used for window movement.
    short mouse_offset_x,mouse_offset_y;

    //Title.
    std::string title;

    //If true, the window is processed.
    //If false, the window is not processed.
    bool on;

    //If true, the window is currently being dragged with the mouse.
    //If false, the window is not moving.
    bool moving;

    //Does this window have any special input?
    //If non-zero, the corresponding special input function(s) are called in this window's input code.
    short special_input;

    image_data* background_image;

    public:

    //All of the information pieces that this window has.
    std::vector<Information> informations;

    Window(short get_x,short get_y,short get_w,short get_h,std::string get_title,short get_special_input,image_data* get_background_image=NULL);

    Window();

    virtual ~Window();

    //Returns the state of on.
    bool return_on();

    //Toggles the 'on' state of the window.
    virtual void toggle_on();

    //Sets the 'on' state of the window to false.
    virtual void turn_off();

    //Set the 'on' state of the window to true.
    void turn_on();

    void special_input_create_profile();

    void clear_buttons();

    void clear_information();

    //Create a new button.
    void create_button(short b_x,short b_y,std::string b_tooltip_text,std::string b_text,void (*b_event_function)(Window*,int),sound_data* get_mouse_over_sound,sound_data* get_event_fire_sound,bool get_invisible,int get_special_data=-1,bool get_enabled=BUTTON_ENABLED);

    //Create an information piece.
    void create_information(short get_x,short get_y,std::string get_tooltip_text,std::string get_text,image_data* get_info_image,short get_special_info,SDL_Rect* get_info_sprite_sheet=0,short get_text_color=COLOR_RAINBOW,short get_font_type=0);

    virtual void handle_input_states();

    virtual void handle_input_events();

    virtual void render();

    //All of the buttons that this window has.
    std::vector<Button> buttons;
};

#endif
