/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef window_message_h
#define window_message_h

#include "window.h"

#include <string>

class Window_Message: public Window{
    private:

    Uint8 *keystates;

    public:

    Window_Message();

    //Sets the 'on' state of the window to false.
    void turn_off();

    void set_dimensions(std::string message,short font_type);

    void set_message(std::string get_title,std::string message,short font_type=0);

    void handle_input_states();
};

#endif
