/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef window_manager_h
#define window_manager_h

#include "window.h"

class Window_Manager{
    public:

    //If true, allow mouseover events to make sounds.
    //If false, do not.
    //This is used to prevent an initial button mouseover sound.
    bool allow_button_sound;

    //These keep track of the index values for all of the buttons for each configuration for the main menu.
    std::vector<int> main_menu_buttons_no_game;
    std::vector<int> main_menu_buttons_sp_adventure;
    std::vector<int> main_menu_buttons_mp_adventure;
    std::vector<int> main_menu_buttons_survival;
    int return_to_world_map_sp_adventure;
    int return_to_world_map_mp_adventure;

    short which_mp_game_mode;

    Window_Manager();

    void close_windows(Window* window_to_keep=0);

    //Returns -1 if no window is open.
    //Returns a number representing the window that is open otherwise.
    short which_window_open();

    void configure_main_menu();

    //Sets the current button to the main menu's default.
    void set_main_menu_current_button();

    void configure_mp_setup();
};

#endif
