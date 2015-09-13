/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef level_h
#define level_h

#include "tile.h"
#include "tile_background.h"
#include "background_layer.h"
#include "devworld.h"
#include "timer.h"
#include "light_data.h"

#include <vector>
#include <string>

#include <SDL.h>
#include <SDL_mixer.h>

class Level{
    private:
    public:
    Level();

    void load_level_adventure();
    void load_level_survival();

    //This function loads whatever level is needed from the corresponding level files.
    void load_level();

    //Unload from memory anything needed.
    void unload_level();

    //Randomly generate a level.
    void generate_level();

    //Returns which region of the worldmap a given level's entrance is part of.
    short return_level_region(int i);

    //Returns true if the level has snow.
    //Returns false if it does not.
    bool return_level_snow(int i);

    //Returns true if the level has rain.
    //Returns false if it does not.
    bool return_level_rain(int i);

    std::string return_level_name(int i);

    void set_title_strings(int i);

    //Returns true if the game is paused due to title display.
    //Returns false otherwise.
    bool return_title_pause();

    void process_title();

    void render_title();

    //Turn all party balls into an explosion of items.
    void explode_party_balls(short item_type);

    //Survival functions.
    unsigned long survival_spawn_items_time();
    unsigned long survival_spawn_npcs_time();
    int survival_spawn_items_chance();
    int survival_spawn_npcs_chance();

    //The dimensions of the level map.
    int level_x,level_y;

    //The current level's total available items.
    short leaves,cheese;

    //If true, the alarm is sounding.
    //If false, the alarm is not sounding.
    bool alarm;

    //The number of replicators in the current level.
    unsigned int amount_replicators;

    std::vector<Background_Layer> background_layers;

    //The current level's tile array.
    std::vector< std::vector<Tile> > tile_array;

    //The current level's background tile array.
    std::vector< std::vector<Tile_Background> > tile_background_array;

    //The position of the level's title and title background.
    double title_top_x;
    double title_bottom_x;
    double title_bg_y_top;
    double title_bg_x_bottom;
    double title_bg_x_left;

    //The level title.
    std::string title_top_string;
    std::string title_bottom_string;

    short title_moving;

    //When the title reaches the middle, this counts down to its removal.
    short counter_title;

    //Special tile distortion data for DevWorld.
    DevWorld devworld_tiles;

    //Keeps track of time spent in this level.
    Timer timer_current_level;

    //Keeps track of the seconds spent in this playthrough of this level.
    unsigned long seconds_in_current_level;

    //Survival mode variables.
    unsigned long survival_seconds_played;
    bool survival_never_spawned_items;
    bool survival_never_spawned_npcs;
    Timer timer_survival_escape_counter;
    Timer timer_survival_spawn_items;
    Timer timer_survival_spawn_npcs;

    //FOV:
    void setSeen_map(int x,int y);
	bool onMap_map(int x,int y);
	bool blockLOS_map(int x,int y);

	void setSeen_lighting(int x,int y,Light_Source* src);
	bool onMap_lighting(int x,int y);
	bool blockLOS_lighting(int x,int y);

	Light_Data blend_light(double src_opacity,color_data src_color,double dest_opacity,color_data dest_color);
};

#endif
