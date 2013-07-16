/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef profile_h
#define profile_h

#include <SDL.h>

#include <vector>
#include <string>

#include "fov.h"

struct Level_Properties{
    short current_sub_level;
    bool level_beaten;
};

struct Leaves_And_Cheese{
    int leaves;
    int cheese;
};

struct Level_Stats{
    unsigned long seconds_total_time;
    unsigned long seconds_best_time;
};

struct Spawn_Coords{
    double x;
    double y;
};

class Profile{
    private:
    public:
    Profile();

    //Change any incorrect slashes to the correct type.
    void correct_slashes(std::string* str_input);

    //Returns a string with the home directory.
    std::string get_home_directory();

    //Create the home directory.
    void make_home_directory();

    //Create the entire needed directory structure.
    void make_directories();

    //Delete the passed profile.
    void delete_profile(int profile_to_delete);

    //Select the passed profile.
    void select_profile(int profile_to_select);

    //Create the profile currently represented by creating_profile (a string).
    void create_profile();

    //Returns true if the passed profile already exists.
    //Returns false if it does not exist.
    bool does_profile_exist(std::string profile_name);

    //Retrieve the list of all existing profiles.
    bool load_profile_list();

    //Save the list of all existing profiles.
    bool save_profile_list();

    //If this item of a collectable type?
    bool is_item_collectable(int type);

    //Save a profile backup.
    void save_backup();

    //Load profile data that is not level-specific.
    bool load_profile_global_data();

    //Save profile data that is not level-specific.
    bool save_profile_global_data();

    bool load_map();

    bool save_map();

    bool load_level_properties();

    bool save_level_properties();

    Level_Properties load_level_properties_weather_machine(short level_to_change);

    Level_Properties save_level_properties_weather_machine(short level_to_change,Level_Properties lp);

    Level_Properties load_level_properties_map(short level_to_check);

    Spawn_Coords return_level_spawn(short level_to_check);

    //Load the maximum leaves and cheese.
    void load_leaves_cheese_counts();

    //Returns the maximum leaves and cheese for the passed level.
    Leaves_And_Cheese load_this_levels_leaves_cheese(short level_to_check);

    //Returns the collected leaves and cheese for the passed level on the loaded profile.
    Leaves_And_Cheese load_collected_leaves_cheese(short level_to_check);

    bool load_level_data();

    bool save_level_data();

    bool load_level_stats();

    bool save_level_stats();

    Level_Stats return_level_stats(short level_to_check);

    bool load_inventory();

    bool save_inventory();

    bool load_stats();

    bool save_stats();

    bool load_achievements();

    bool save_achievements();

    bool load_boss_states();

    bool save_boss_states();

    //Used when starting a New Game Plus.
    void reset_world_map_data();
    void reset_ammo_barrels();
    void reset_weather_machine();
    void reset_level_beaten();

    bool save_mp_players();
    bool load_mp_players();

    bool save_shop();
    bool load_shop();

    bool save_upgrades();
    bool load_upgrades();

    void update_fov_map();

    //Set the achievement to true, create a slider, increase score.
    void earn_achievement(int achievement);

    //Pass an achievement if it has been earned.
    //Pass nothing to update the count achievements.
    void update_achievements(int achievement=-1);

    //A list of all existing profiles.
    std::vector<std::string> profile_list;

    //The map data for the currently loaded level.
    std::vector< std::vector<bool> > current_level_map_data;

    //Field of view map variables.
    fov_settings_type fov_settings_map;
    unsigned int fov_radius_map;

    //If true, we just prevented loading a profile due to a version mismatch.
    //If false, ignore this.
    bool version_mismatch;
};

#endif
