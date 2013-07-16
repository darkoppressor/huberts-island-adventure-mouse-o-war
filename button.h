/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef button_h
#define button_h

#include "enumerations.h"
#include "sound_data.h"

#include <string>

#include <SDL_mixer.h>

//Incomplete declaration of Window.
//We have to do this here, because a pointer to Window objects is used by the event-related functions, but Window relies on Button.
class Window;

class Button{
    private:

    //The coordinate location.
    //This location, as used by a window, is an offset from the window, not the actual screen.
    short x,y;

    //If true, the mouse is currently over this button.
    //If false, it is not.
    bool moused_over;

    //If true, the mouse button has clicked down on this button.
    //If false, it has not.
    bool clicked;

    //If true, the button is never rendered.
    //If false, the button is rendered normally.
    bool invisible;

    //A value of -1 means there is no special data.
    int special_data;

    //Function pointer. Used to store which event function the button will call when it fires its event.
    void (*event_function)(Window*,int);

    //Pointers to sounds for mousing over and firing the button's event.
    sound_data* mouse_over_sound;
    sound_data* event_fire_sound;

    void set_dimensions();

    public:

    //Dimensions.
    short w,h;

    std::string text;
    std::string tooltip_text;

    //If true, button functions normally.
    //If false, button is completely non-functional.
    bool enabled;

    Button(short get_x,short get_y,std::string get_tooltip_text,std::string get_text,void (*get_event_function)(Window*,int),sound_data* get_mouse_over_sound,sound_data* get_event_fire_sound,bool get_invisible,int get_special_data=-1,bool get_enabled=BUTTON_ENABLED);

    void set_invisibility(bool invis);

    void set_enabled(bool get_enabled);

    void set_coordinates(short get_x,short get_y);

    bool is_moused_over(int mouse_x,int mouse_y,short x_offset,short y_offset,int index);

    //Resets moused over state.
    //Returns true if the button was moused over.
    //Returns false if the button was not moused over.
    bool reset_moused_over();

    //Resets clicked state.
    void reset_clicked();

    //The mouse is over this button.
    void mouse_over(bool previously_moused_over);

    //The mouse button has clicked down on this button.
    void mouse_button_down();

    //The mouse button has been released over this button.
    void mouse_button_up(Window* parent_window);

    std::string return_tooltip_text();

    bool has_tooltip();

    void fire_event(Window* parent_window);

    //Render the button, offset by x_offset and y_offset.
    void render(short x_offset,short y_offset,int index);
};

#endif
