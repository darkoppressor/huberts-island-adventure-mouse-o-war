/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "quit.h"
#include "world.h"
#include "options.h"
#include "button_events.h"

#include <SDL.h>
#include <SDL_mixer.h>

using namespace std;

void quit_game(){
    if(!demo_mode){
        profile.save_level_data();

        profile.save_profile_global_data();

        options_save();

        profile.save_backup();

        music.unload_tracks();

        fov_settings_free(&profile.fov_settings_map);

        fov_settings_free(&player.fov_settings_lighting);

        //This function frees any dynamically allocated memory that was loaded in load_world().
        unload_world();
        image.unload_images_level(player.current_level);

        SDL_FreeCursor(main_window.boring_cursor);

        main_window.deinitialize();

        //Tell the program to exit successfully.
        exit(EXIT_SUCCESS);
    }
    else{
        button_event_open_window_demo(NULL,0);
    }
}
