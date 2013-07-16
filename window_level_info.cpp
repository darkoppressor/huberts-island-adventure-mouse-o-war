/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "window_level_info.h"
#include "world.h"
#include "button_events.h"
#include "render.h"
#include "quit.h"
#include "collision.h"

using namespace std;

Window_Level_Info::Window_Level_Info(short get_x,short get_y,short get_w,short get_h,string get_title,int get_window_identifier,int get_level_identifier){
    background_image=NULL;

    x=get_x;
    y=get_y;
    w=get_w;
    h=get_h;
    mouse_offset_x=0;
    mouse_offset_y=0;
    title=get_title;
    on=false;
    moving=false;

    window_identifier=get_window_identifier;
    level_identifier=get_level_identifier;

    leaves=0;
    leaves_max=0;

    cheese=0;
    cheese_max=0;

    seconds_total_time=0;
    seconds_best_time=4294967295;

    //Create the close button.
    create_button(w-23,5,"","X",&button_event_close_window,0,0,BUTTON_VISIBLE);
}

void Window_Level_Info::load_stats(){
    Leaves_And_Cheese collected=profile.load_collected_leaves_cheese(level_identifier);
    Leaves_And_Cheese max_in_level=profile.load_this_levels_leaves_cheese(level_identifier);

    leaves=collected.leaves;
    leaves_max=max_in_level.leaves;

    cheese=collected.cheese;
    cheese_max=max_in_level.cheese;

    Level_Stats level_stats=profile.return_level_stats(level_identifier);

    seconds_total_time=level_stats.seconds_total_time;
    seconds_best_time=level_stats.seconds_best_time;
}
