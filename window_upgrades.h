/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef window_upgrades_h
#define window_upgrades_h

#include "window.h"

class Window_Upgrades: public Window{
    private:
    public:

    std::vector<std::string> upgrades;

    int upgrade_list_display_position;
    int upgrade_list_selection;

    Window_Upgrades(short get_x,short get_y,short get_w,short get_h,std::string get_title);

    void setup();

    void handle_input_events();

    void render();
};

#endif
