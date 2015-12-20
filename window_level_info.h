/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef window_level_info_h
#define window_level_info_h

#include "window.h"

#include <string>

class Window_Level_Info: public Window{
    private:

    int window_identifier;

    public:

    int level_identifier;

    int leaves;
    int leaves_max;
    int cheese;
    int cheese_max;

    unsigned long seconds_total_time;
    unsigned long seconds_best_time;

    Window_Level_Info(short get_x,short get_y,short get_w,short get_h,std::string get_title,int get_window_identifier,int get_level_identifier);

    void load_stats();
};

#endif
