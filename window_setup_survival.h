/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef window_setup_survival_h
#define window_setup_survival_h

#include "window.h"

class Window_Setup_Survival: public Window{
    private:
    public:

    bool is_multiplayer;

    std::vector<std::string> levels;

    int level_list_display_position;
    int level_list_selection;

    unsigned long recall_seconds;

    Window_Setup_Survival(short get_x,short get_y,short get_w,short get_h,std::string get_title);

    void setup(bool multiplayer);

    void handle_input_events();

    void render();
};

#endif
