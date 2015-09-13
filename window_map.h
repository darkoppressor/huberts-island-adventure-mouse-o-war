/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef window_map_h
#define window_map_h

#include "window.h"
#include "image_data.h"
#include "item.h"

class Window_Map: public Window{
    private:

    Uint8 *keystates;

    short cam_state;

    short camera_speed;

    double map_camera_x,map_camera_y;

    //Level data for the level currently loaded for map viewing.
    bool persistent_level_data;
    int level_x;
    int level_y;

    //The level and sub level that are currently loaded into memory for map viewing.
    short current_viewable_level;
    short current_viewable_sub_level;

    struct map_data{
        bool seen;
        short sprite;
        short background_sprite;
    };
    //The map data that has been loaded for viewing.
    std::vector< std::vector<map_data> > viewable_map_data;

    std::vector<Item> viewable_items;

    public:

    image_data viewable_tiles;
    image_data viewable_background_tiles;
    image_data viewable_water_tiles;

    double map_camera_w,map_camera_h;

    Window_Map(short get_x,short get_y,short get_w,short get_h,std::string get_title);

    ~Window_Map();

    void set_camera_coordinates(double cam_x,double cam_y);

    void set_map_level(short get_level,short get_sub_level);

    void load_map_level_properties();

    void load_map_images();

    void load_map_data();

    void display_map();

    void handle_map_input();

    void handle_input_states();

    void handle_input_events();

    void render();
};

#endif
