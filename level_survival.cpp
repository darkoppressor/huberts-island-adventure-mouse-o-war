/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "level.h"
#include "world.h"
#include "message_log.h"
#include "render.h"
#include "update.h"

#include <fstream>

#include <boost/algorithm/string.hpp>

using namespace std;

unsigned long Level::survival_spawn_items_time(){
    if(!survival_never_spawned_items){
        unsigned long minutes_played=survival_seconds_played/60;

        unsigned long base_seconds=(unsigned long)floor(((level_x/TILE_SIZE)*(level_y/TILE_SIZE))/500);
        if(base_seconds>30){
            base_seconds=30;
        }

        unsigned long seconds_for_spawn=base_seconds+minutes_played;

        return seconds_for_spawn;
    }
    else{
        survival_never_spawned_items=false;
        return 0;
    }
}

unsigned long Level::survival_spawn_npcs_time(){
    if(!survival_never_spawned_npcs){
        unsigned long minutes_played=survival_seconds_played/60;

        unsigned long base_seconds=(unsigned long)floor(((level_x/TILE_SIZE)*(level_y/TILE_SIZE))/500);
        if(base_seconds>30){
            base_seconds=30;
        }

        unsigned long seconds_for_spawn=base_seconds-minutes_played;
        if(seconds_for_spawn<10){
            seconds_for_spawn=10;
        }

        return seconds_for_spawn;
    }
    else{
        survival_never_spawned_npcs=false;
        return 0;
    }
}

int Level::survival_spawn_items_chance(){
    unsigned long minutes_played=survival_seconds_played/60;

    unsigned long base_chance=(unsigned long)floor(((double)(410.0/(double)((level_x/TILE_SIZE)*(level_y/TILE_SIZE)))*1000.0)*2.0);
    if(base_chance<50){
        base_chance=50;
    }

    int chance_for_spawn=base_chance-((int)floor(minutes_played*1.5));
    if(chance_for_spawn<35){
        chance_for_spawn=35;
    }

    return chance_for_spawn;
}

int Level::survival_spawn_npcs_chance(){
    unsigned long minutes_played=survival_seconds_played/60;

    unsigned long base_chance=(unsigned long)floor(((double)(410.0/(double)((level_x/TILE_SIZE)*(level_y/TILE_SIZE)))*1000.0)*2.0)/2;
    if(base_chance<70){
        base_chance=70;
    }

    int chance_for_spawn=base_chance+((int)floor(minutes_played*1.75));
    if(chance_for_spawn>95){
        chance_for_spawn=95;
    }

    return chance_for_spawn;
}

void Level::load_level_survival(){
    //Pause the game.
    player.toggle_pause(true);

    player.light_source.dimness=0.0;
    player.light_source.falloff=0.035/(TILE_SIZE/LIGHTING_TILE_SIZE);

    for(int i=0;i<mp_players.size();i++){
        mp_players[i].light_source.dimness=0.0;
        mp_players[i].light_source.falloff=0.035/(TILE_SIZE/LIGHTING_TILE_SIZE);
    }

    devworld_tiles.clear_tiles();

    alarm=false;

    int things_loaded=0;
    int things_to_load=3;

    //As long as a level has been loaded before
    //and the last level was not the same as this one.
    if(player.previous_level!=-1 && player.previous_level!=player.current_level){
        player.tracer_manager.clear_tracers();

        //Unload the previous level's images.
        image.unload_images_level(player.previous_level);

        //Reset the special achievement counts.
        player.special_count_shots_this_level=0;
        player.special_count_kills_this_level=0;
    }

    string current_level="survival/";
    current_level+=player.non_number_level;

    //Clear the vectors. This ensures that any previous data held in the vectors is deleted, and that any memory that was allocated by the objects in the vectors is freed.
    background_layers.clear();

    vector_items.clear();
    vector_npcs.clear();
    vector_shots.clear();
    vector_moving_platforms.clear();
    vector_triggers.clear();
    vector_doors.clear();
    vector_traps.clear();
    vector_signs.clear();
    vector_springs.clear();
    vector_boosters.clear();
    vector_effect_water_splash.clear();
    vector_effect_water_bubble.clear();
    vector_effect_score_floater.clear();
    vector_effect_firework.clear();
    vector_spawners.clear();

    //*******************************************//
    // First, we will read the level properties: //
    //*******************************************//

    //First, we open the file for reading.
    ifstream load;

    string level_to_load="";

    level_to_load="data/levels/"+current_level+"/level_properties.blazelevel";

    load.open(level_to_load.c_str(),ifstream::in);

    bool items_stay=false;

    //As long as the file opened successfully, proceed to load the level data.
    if(load!=NULL){

        //Create any variables that will be used to pass on information from the level file.
        double start_y=0;
        double width=0;
        double height=0;
        short number_of_background_layers=0;
        double parallax_speed_x=0;
        double parallax_speed_y=0;

        //The first two numbers in the level file are the x and y dimensions of the entire level's map.
        load >> level_x;
        load >> level_y;

        load >> player.ammo;
        player.ammo=floor((double)player.ammo*(1+((double)mp_players.size()*0.5)));
        for(int i=0;i<mp_players.size();i++){
            mp_players[i].ammo=player.ammo;
        }

        load >> start_y;
        load >> width;
        load >> height;

        //Load in the number of background layers.
        load >> number_of_background_layers;

        for(int i=0;i<number_of_background_layers;i++){
            load >> parallax_speed_x;
            load >> parallax_speed_y;
            background_layers.push_back(Background_Layer(start_y,width,height,parallax_speed_x,parallax_speed_y));
        }

        load >> items_stay;

        load >> player.night_mode;

        load >> player.deadly_water;

        if(random_range(0,99)<50){
            player.facing=LEFT;
        }
        else{
            player.facing=RIGHT;
        }
        for(int i=0;i<mp_players.size();i++){
            if(random_range(0,99)<50){
                mp_players[i].facing=LEFT;
            }
            else{
                mp_players[i].facing=RIGHT;
            }
        }
        player.persistent_level_data=false;

        //Determine the date and time.
        /**time_t now;
        struct tm *tm_now;
        now=time(NULL);
        tm_now=localtime(&now);

        //If it is daytime.
        if(tm_now->tm_hour>=6 && tm_now->tm_hour<18){
            player.night_mode=false;
        }*/

        player.light_data.clear();
        player.light_data.resize(level_x/LIGHTING_TILE_SIZE,vector<Light_Data>(level_y/LIGHTING_TILE_SIZE));

        for(int int_y=0;int_y<level_y/LIGHTING_TILE_SIZE;int_y++){
            for(int int_x=0;int_x<level_x/LIGHTING_TILE_SIZE;int_x++){
                player.light_data[int_x][int_y].seen=false;
                player.light_data[int_x][int_y].color.red=-1.0;
                player.light_data[int_x][int_y].color.green=-1.0;
                player.light_data[int_x][int_y].color.blue=-1.0;
                player.light_data[int_x][int_y].light_intensity=-1.0;
                player.light_data[int_x][int_y].dimness=-1.0;
            }
        }

        //When we are done using the level file, we close it.
        load.close();
        load.clear();
    }
    //If the level file could not be loaded for any reason, we print an error message letting the user know.
    else{
        update_error_log("Error loading level properties.");
    }

    //***************************************//
    // Next, we will read the terrain tiles: //
    //***************************************//

    //First, we open the file for reading.

    level_to_load="data/levels/"+current_level+"/tiles.blazelevel";

    load.open(level_to_load.c_str(),ifstream::in);

    //As long as the file opened successfully, proceed to load the level data.
    if(load!=NULL){
        tile_array.clear();
        tile_array.resize(level_x/TILE_SIZE,vector<Tile>(level_y/TILE_SIZE));

        //Now we make temporary variables for getting the type of each tile being set.
        short solidity=0;
        short sprite=0;
        short special=0;
        bool foreground=0;
        short slope=0;

        //Then we iterate through all of the tiles in the file, setting each one.
        for(int int_y=0;int_y<level_y/TILE_SIZE;int_y++){
            for(int int_x=0;int_x<level_x/TILE_SIZE;int_x++){
                //For each tile, load the type information from the level file.
                load >> solidity;
                load >> sprite;
                load >> special;
                load >> foreground;
                load >> slope;

                //If the tile is an item holder, make it empty air.
                if(special==TILE_SPECIAL_ITEM_HOLDER){
                    solidity=0;
                    sprite=0;
                    special=0;
                    foreground=0;
                    slope=0;
                }

                //Then generate a new tile in the tile vector with the proper coordinates and the type loaded from the level file.
                tile_array[int_x][int_y].x=int_x*TILE_SIZE;
                tile_array[int_x][int_y].y=int_y*TILE_SIZE;
                tile_array[int_x][int_y].solidity=solidity;
                tile_array[int_x][int_y].sprite=sprite;
                tile_array[int_x][int_y].special=special;
                tile_array[int_x][int_y].foreground=foreground;
                tile_array[int_x][int_y].slope=slope;
                tile_array[int_x][int_y].set_height_mask();
            }
        }

        for(int int_x=0;int_x<level_x/TILE_SIZE;int_x++){
            for(int int_y=0;int_y<level_y/TILE_SIZE;int_y++){
                if(tile_array[int_x][int_y].special==TILE_SPECIAL_WATER && tile_array[int_x][int_y].sprite==1){
                    if(player.deadly_water){
                        ///if(player.current_level==10){
                            tile_array[int_x][int_y].light_source.on=true;
                            tile_array[int_x][int_y].light_source.color=color_name_to_doubles(COLOR_RED);
                        /**}
                        else{
                            tile_array[int_x][int_y].light_source.on=true;
                            tile_array[int_x][int_y].light_source.color=color_name_to_doubles(COLOR_GREEN);
                        }*/
                    }
                }
            }
        }

        //When we are done using the level file, we close it, and clear load for its next use.
        load.close();
        load.clear();
    }
    //If the level file could not be loaded for any reason, we print an error message letting the user know.
    else{
        update_error_log("Error loading terrain tiles.");
    }

    //******************************************//
    // Next, we will read the background tiles: //
    //******************************************//

    //First, we open the file for reading.

    level_to_load="data/levels/"+current_level+"/tiles_background.blazelevel";

    load.open(level_to_load.c_str(),ifstream::in);

    //As long as the file opened successfully, proceed to load the level data.
    if(load!=NULL){
        tile_background_array.clear();

        tile_background_array.resize(level_x/TILE_SIZE,vector<Tile_Background>(level_y/TILE_SIZE));

        //Now we make a temporary variable for getting the sprite of each tile being set.
        short sprite=0;

        //Then we iterate through all of the tiles in the file, setting each one.
        for(int int_y=0;int_y<(level_y/TILE_SIZE);int_y++){
            for(int int_x=0;int_x<(level_x/TILE_SIZE);int_x++){
                //For each tile, load the type information from the level file.
                load >> sprite;

                //Then generate a new tile in the tile vector with the proper coordinates and the type loaded from the level file.
                tile_background_array[int_x][int_y].x=int_x*TILE_SIZE;
                tile_background_array[int_x][int_y].y=int_y*TILE_SIZE;
                tile_background_array[int_x][int_y].sprite=sprite;
            }
        }

        //When we are done using the level file, we close it, and clear load for its next use.
        load.close();
        load.clear();
    }
    //If the level file could not be loaded for any reason, we print an error message letting the user know.
    else{
        update_error_log("Error loading background tiles.");
    }

    //******************************//
    // Now, we will read the items: //
    //******************************//

    //First, we open the file for reading.

    level_to_load="data/levels/"+current_level+"/items.blazelevel";

    load.open(level_to_load.c_str(),ifstream::in);

    //As long as the file opened successfully, proceed to load the data.
    if(load!=NULL){
        //Now we make a temporary variable for getting the type of each item being loaded.
        short type;
        double x;
        double y;
        int goal_level_to_load;
        bool goal_secret;

        //Then we iterate through all of the items in the file, setting each one.
        while(!load.eof()){
            type=30000;
            x=0;
            y=0;
            goal_level_to_load=0;
            goal_secret=false;

            //For each item, load the type information from the file.
            load >> type;
            load >> x;
            load >> y;
            load >> goal_level_to_load;
            load >> goal_secret;

            bool should_spawn=false;
            if(type!=ITEM_LEAF && type!=ITEM_CANDY){
                should_spawn=true;
            }

            if(type!=30000 && should_spawn){
                vector_items.push_back(Item(x,y,false,type,goal_level_to_load,goal_secret));
            }
        }

        //When we are done using the level file, we close it, and clear load for its next use.
        load.close();
        load.clear();
    }
    //If the level file could not be loaded for any reason, we print an error message letting the user know.
    else{
        update_error_log("Error loading level items.");
    }

    //*****************************//
    // Now, we will read the npcs: //
    //*****************************//

    //First, we open the file for reading.

    level_to_load="data/levels/"+current_level+"/npcs.blazelevel";

    load.open(level_to_load.c_str(),ifstream::in);

    //As long as the file opened successfully, proceed to load the data.
    if(load!=NULL){
        //Now we make a temporary variable for getting the type of each npc being loaded.
        short type;
        double x;
        double y;

        //Then we iterate through all of the items in the file, setting each one.
        while(!load.eof()){
            type=0;
            x=0;
            y=0;

            //For each item, load the type information from the file.
            load >> type;
            load >> x;
            load >> y;

            bool should_spawn=true;
            if(player.npc_is_main_boss(type)){
                should_spawn=false;

                //Replace bosses with two ammo boxes each.
                vector_items.push_back(Item(x,y,false,ITEM_AMMO,0,false));
                vector_items.push_back(Item(x+32,y,false,ITEM_AMMO,0,false));
            }

            if(type!=0 && should_spawn){
                vector_npcs.push_back(Npc(x,y,type,false));
                vector_npcs[vector_npcs.size()-1].ethereal_to_npcs=true;
            }
        }

        //When we are done using the level file, we close it, and clear load for its next use.
        load.close();
        load.clear();
    }
    //If the level file could not be loaded for any reason, we print an error message letting the user know.
    else{
        update_error_log("Error loading level npcs.");
    }

    //*****************************************//
    // Now, we will read the moving platforms: //
    //*****************************************//

    //First, we open the file for reading.

    level_to_load="data/levels/"+current_level+"/moving_platforms.blazelevel";

    load.open(level_to_load.c_str(),ifstream::in);

    //As long as the file opened successfully, proceed to load the level data.
    if(load!=NULL){
        //Now we make temporary variables.
        double move_speed;
        double start_point_x;
        double start_point_y;
        double end_point_x;
        double end_point_y;
        bool active;
        bool round_trip;
        bool movement_type;
        short identifier;
        int number_of_waypoints;
        vector<moving_platform_waypoint> waypoints;

        //Then we iterate through all of the npcs in the file, setting each one.
        while(!load.eof()){
            move_speed=0;
            start_point_x=0;
            start_point_y=0;
            end_point_x=0;
            end_point_y=0;
            active=false;
            round_trip=false;
            movement_type=false;
            identifier=0;
            number_of_waypoints=0;
            waypoints.clear();

            //For each item, load the type information from the file.
            load >> move_speed;
            load >> start_point_x;
            load >> start_point_y;
            load >> end_point_x;
            load >> end_point_y;
            load >> active;
            load >> round_trip;
            load >> movement_type;
            load >> identifier;
            load >> number_of_waypoints;

            for(int i=0;i<number_of_waypoints;i++){
                waypoints.push_back(moving_platform_waypoint());
                load >> waypoints[i].x;
                load >> waypoints[i].y;
            }

            if(move_speed!=0){
                vector_moving_platforms.push_back(Moving_Platform(move_speed,start_point_x,start_point_y,end_point_x,end_point_y,waypoints,active,round_trip,movement_type,identifier));
            }
        }

        //When we are done using the level file, we close it, and clear load for its next use.
        load.close();
        load.clear();
    }
    //If the level file could not be loaded for any reason, we print an error message letting the user know.
    else{
        update_error_log("Error loading moving platforms.");
    }

    //*********************************//
    // Now, we will read the triggers: //
    //*********************************//

    //First, we open the file for reading.

    level_to_load="data/levels/"+current_level+"/triggers.blazelevel";

    load.open(level_to_load.c_str(),ifstream::in);

    //As long as the file opened successfully, proceed to load the level data.
    if(load!=NULL){
        //Now we make temporary variables.
        double x,y;
        double w,h;
        int number_of_targets;
        vector<target> targets;
        bool trigger_method;
        short user_type;
        bool repeating;
        short repeat_time;
        short render_trigger;

        //Then we iterate through all of the triggers in the file, setting each one.
        while(!load.eof()){
            x=0;
            y=0;
            w=0;
            h=0;
            number_of_targets=0;
            targets.clear();
            trigger_method=false;
            user_type=0;
            repeating=false;
            repeat_time=0;
            render_trigger=0;

            //For each item, load the type information from the file.
            load >> x;
            load >> y;
            load >> w;
            load >> h;
            load >> number_of_targets;

            for(int i=0;i<number_of_targets;i++){
                targets.push_back(target());
                load >> targets[i].type;
                load >> targets[i].identifier;
            }

            load >> trigger_method;
            load >> user_type;
            load >> repeating;
            load >> repeat_time;
            load >> render_trigger;

            if(w!=0){
                vector_triggers.push_back(Trigger(x,y,w,h,targets,trigger_method,user_type,repeating,repeat_time,render_trigger));
            }
        }

        //When we are done using the level file, we close it.
        load.close();
        load.clear();
    }
    //If the level file could not be loaded for any reason, we print an error message letting the user know.
    else{
        update_error_log("Error loading triggers.");
    }

    //******************************//
    // Now, we will read the doors: //
    //******************************//

    //First, we open the file for reading.

    level_to_load="data/levels/"+current_level+"/doors.blazelevel";

    load.open(level_to_load.c_str(),ifstream::in);

    //As long as the file opened successfully, proceed to load the level data.
    if(load!=NULL){
        //Now we make temporary variables.
        double x,y;
        short type;
        short number;
        bool open;
        short identifier;

        //Then we iterate through all of the doors in the file, setting each one.
        while(!load.eof()){
            x=0;
            y=0;
            type=0;
            number=0;
            open=false;
            identifier=-1;

            //For each one, load the type information from the file.
            load >> x;
            load >> y;
            load >> type;
            load >> number;
            load >> open;
            load >> identifier;

            if(type==DOOR_TYPE_KEY){
                open=true;
            }

            if(identifier!=-1){
                vector_doors.push_back(Door(x,y,type,number,open,identifier));
            }
        }

        //When we are done using the level file, we close it.
        load.close();
        load.clear();
    }
    //If the level file could not be loaded for any reason, we print an error message letting the user know.
    else{
        update_error_log("Error loading doors.");
    }

    //******************************//
    // Now, we will read the traps: //
    //******************************//

    //First, we open the file for reading.

    level_to_load="data/levels/"+current_level+"/traps.blazelevel";

    load.open(level_to_load.c_str(),ifstream::in);

    //As long as the file opened successfully, proceed to load the level data.
    if(load!=NULL){
        //Now we make temporary variables.
        double x,y;
        short type;
        bool active;
        short identifier;

        //Then we iterate through all of the traps in the file, setting each one.
        while(!load.eof()){
            x=0;
            y=0;
            type=0;
            active=false;
            identifier=-1;

            //For each one, load the type information from the file.
            load >> x;
            load >> y;
            load >> type;
            load >> active;
            load >> identifier;

            bool should_spawn=true;
            if(type==TRAP_WEATHER_MACHINE || type==TRAP_OPEN_SHOP || type==TRAP_OPEN_UPGRADES || type==TRAP_BOSS_CASTLE || type==TRAP_BOSS_PYRAMID){
                should_spawn=false;
            }

            if(identifier!=-1 && should_spawn){
                vector_traps.push_back(Trap(x,y,type,active,identifier));
            }
        }

        //When we are done using the level file, we close it.
        load.close();
        load.clear();
    }
    //If the level file could not be loaded for any reason, we print an error message letting the user know.
    else{
        update_error_log("Error loading traps.");
    }

    //******************************//
    // Now, we will read the signs: //
    //******************************//

    //First, we open the file for reading.

    level_to_load="data/levels/"+current_level+"/signs.blazelevel";

    load.open(level_to_load.c_str(),ifstream::in);

    //As long as the file opened successfully, proceed to load the level data.
    if(load!=NULL){
        //Now we make temporary variables.
        double x,y;
        string message;
        short sign_type;
        short font_type;

        //Then we iterate through all of the signs in the file, setting each one.
        while(!load.eof()){
            x=-1;
            y=-1;
            message="";
            sign_type=0;
            font_type=0;

            //For each one, load the type information from the file.
            load >> x;
            load >> y;
            load >> message;
            load >> sign_type;
            load >> font_type;

            boost::algorithm::replace_all(message,"<SPACE>"," ");
            boost::algorithm::replace_all(message,"<NEWLINE>","\xA");

            if(x!=-1 && y!=-1){
                vector_signs.push_back(Sign(x,y,message,sign_type,font_type));
            }
        }

        //When we are done using the level file, we close it.
        load.close();
        load.clear();
    }
    //If the level file could not be loaded for any reason, we print an error message letting the user know.
    else{
        update_error_log("Error loading signs.");
    }

    //********************************//
    // Now, we will read the springs: //
    //********************************//

    //First, we open the file for reading.

    level_to_load="data/levels/"+current_level+"/springs.blazelevel";

    load.open(level_to_load.c_str(),ifstream::in);

    //As long as the file opened successfully, proceed to load the level data.
    if(load!=NULL){
        //Now we make temporary variables.
        double x,y;
        double speed;
        short direction;

        //Then we iterate through all of the springs in the file, setting each one.
        while(!load.eof()){
            x=-1;
            y=-1;
            speed=0.0;
            direction=LEFT;

            //For each one, load the type information from the file.
            load >> x;
            load >> y;
            load >> speed;
            load >> direction;

            if(x!=-1 && y!=-1){
                vector_springs.push_back(Spring(x,y,speed,direction));
            }
        }

        //When we are done using the level file, we close it.
        load.close();
        load.clear();
    }
    //If the level file could not be loaded for any reason, we print an error message letting the user know.
    else{
        update_error_log("Error loading springs.");
    }

    //*********************************//
    // Now, we will read the boosters: //
    //*********************************//

    //First, we open the file for reading.

    level_to_load="data/levels/"+current_level+"/boosters.blazelevel";

    load.open(level_to_load.c_str(),ifstream::in);

    //As long as the file opened successfully, proceed to load the level data.
    if(load!=NULL){
        //Now we make temporary variables.
        double x,y;
        double speed;
        short direction;

        //Then we iterate through all of the boosters in the file, setting each one.
        while(!load.eof()){
            x=-1;
            y=-1;
            speed=0.0;
            direction=LEFT;

            //For each one, load the type information from the file.
            load >> x;
            load >> y;
            load >> speed;
            load >> direction;

            if(x!=-1 && y!=-1){
                vector_boosters.push_back(Booster(x,y,speed,direction));
            }
        }

        //When we are done using the level file, we close it.
        load.close();
        load.clear();
    }
    //If the level file could not be loaded for any reason, we print an error message letting the user know.
    else{
        update_error_log("Error loading boosters.");
    }

    //As long as the last level was not the same as this one.
    if(player.previous_level!=player.current_level){
        render_loading_screen((double)++things_loaded/(double)things_to_load,player.current_level);

        music.unload_track(MUSIC_BOSS);
        music.unload_track(MUSIC_BOSS_FAST);
        music.unload_track(MUSIC_LEVEL_NORMAL);
        music.unload_track(MUSIC_LEVEL_WATER);

        string random_music=num_to_string(random_range(1,3));

        music.load_track("data/music/survival/normal"+random_music+".ogg",MUSIC_LEVEL_NORMAL);
        render_loading_screen((double)++things_loaded/(double)things_to_load,player.current_level);

        music.load_track("data/music/survival/water"+random_music+".ogg",MUSIC_LEVEL_WATER);
        render_loading_screen((double)++things_loaded/(double)things_to_load,player.current_level);

        music.restart_track(MUSIC_ALARM);
        music.restart_track(MUSIC_WORLDMAP_VALLEY);
        music.restart_track(MUSIC_WORLDMAP_CASTLE);
        music.restart_track(MUSIC_WORLDMAP_HAUNTED);
        music.restart_track(MUSIC_WORLDMAP_MOUNTAIN);
        music.restart_track(MUSIC_WORLDMAP_VOLCANO);
        music.restart_track(MUSIC_WORLDMAP_DESERT);
        music.restart_track(MUSIC_WORLDMAP_OCEAN);
        music.restart_track(MUSIC_WORLDMAP_MOUNTAIN_PEAK);
        music.restart_track(MUSIC_WORLDMAP_LIGHTHOUSE);
        music.restart_track(MUSIC_BOSS);
        music.restart_track(MUSIC_BOSS_FAST);
        music.restart_track(MUSIC_LEVEL_NORMAL);
        music.restart_track(MUSIC_LEVEL_WATER);
        music.play_track(MUSIC_LEVEL_NORMAL);

        title_top_x=0.0;
        title_bottom_x=0.0;
        title_bg_y_top=0.0;
        title_bg_x_bottom=0.0;
        title_bg_x_left=0.0;
        title_top_string="";
        title_bottom_string="";
        title_moving=TITLE_NOT_MOVING;
        counter_title=0;

        timer_current_level.start();
        seconds_in_current_level=0;
        player.seconds_total_current_level=0;
        player.seconds_best_current_level=4294967295;
    }

    if(title_moving==TITLE_NOT_MOVING){
        //Unpause the game.
        player.toggle_pause(false);
    }

    survival_seconds_played=0;
    survival_never_spawned_items=true;
    survival_never_spawned_npcs=true;
    timer_survival_escape_counter.start();
    timer_survival_spawn_items.start();
    timer_survival_spawn_npcs.start();

    //Replace items with item spawners.
    for(int i=0;i<vector_items.size();i++){
        if(vector_items[i].type==ITEM_CHECKPOINT || vector_items[i].type==ITEM_ENDPOINT){
            vector_items[i].type=ITEM_SPAWNPOINT;
            vector_items[i].y+=64;
        }
        else if(vector_items[i].type==ITEM_CHEESE || vector_items[i].type==ITEM_AMMO_BARREL ||
                vector_items[i].type==ITEM_SWIMMING_GEAR ||
                (vector_items[i].type>=ITEM_KEY_RED && vector_items[i].type<=ITEM_KEY_PURPLE) ||
                vector_items[i].type==ITEM_TOWEL || vector_items[i].type==ITEM_SINK ||
                (vector_items[i].type>=ITEM_KEY_GRAY && vector_items[i].type<=ITEM_J_WING)){
            vector_items[i].type=ITEM_AMMO;
        }

        if(vector_items[i].type!=ITEM_SPAWNPOINT){
            vector_spawners.push_back(Spawner(SPAWN_ITEM,vector_items[i].type,vector_items[i].x,vector_items[i].y,false,items_stay));

            vector_items.erase(vector_items.begin()+i);
            i--;
        }
    }

    //Replace NPCs with NPC spawners.
    for(int i=0;i<vector_npcs.size();i++){
        bool should_become_spawner=true;
        bool disallow_doubles=false;

        if(vector_npcs[i].invulnerable || vector_npcs[i].counts_as_trap || vector_npcs[i].BOSS ||
           !vector_npcs[i].exists){
            should_become_spawner=false;
        }

        if(vector_npcs[i].type==NPC_SHARK){
            should_become_spawner=true;
        }

        if(vector_npcs[i].type==NPC_MONKEY || vector_npcs[i].type==NPC_ARCHER || vector_npcs[i].type==NPC_SCARECROW ||
           vector_npcs[i].type==NPC_EAGLE){
            disallow_doubles=true;
        }

        if(should_become_spawner){
            vector_spawners.push_back(Spawner(SPAWN_NPC,vector_npcs[i].type,vector_npcs[i].x,vector_npcs[i].y,disallow_doubles,false));

            vector_npcs.erase(vector_npcs.begin()+i);
            i--;
        }
    }

    //Reset the item counts for the current level, as these will be loaded next.
    leaves=0;
    cheese=0;
    player.current_level_leaves=0;
    player.current_level_cheese=0;
    player.x=-1;
    player.y=-1;
    for(int i=0;i<mp_players.size();i++){
        mp_players[i].x=-1;
        mp_players[i].y=-1;
    }

    //Compile a list of spawn points.
    vector<int> spawns;
    for(int i=0;i<vector_items.size();i++){
        if(vector_items[i].type==ITEM_SPAWNPOINT){
            spawns.push_back(i);
        }
    }

    //If there are no spawns, just spawn at the default location.
    if(spawns.size()==0){
        player.x=0;
        player.y=0;

        for(int i=0;i<mp_players.size();i++){
            mp_players[i].x=0;
            mp_players[i].y=0;
        }
    }
    //Otherwise, choose one randomly and spawn at it.
    else{
        int random_spawn=random_range(0,spawns.size()-1);

        player.x=vector_items[spawns[random_spawn]].x;
        player.y=vector_items[spawns[random_spawn]].y-14;

        for(int i=0;i<mp_players.size();i++){
            mp_players[i].x=vector_items[spawns[random_spawn]].x;
            mp_players[i].y=vector_items[spawns[random_spawn]].y-14;
        }
    }

    player.camera_trap_x=player.x;
    player.camera_trap_y=player.y+player.h-CAMERA_TRAP_H;

    //Snap the camera to the starting position.
    player.set_camera();

    //Load this level's images.
    if(player.previous_level!=player.current_level){
        image.load_images_level(player.current_level,player.current_sub_level,player.non_number_level);
    }
}
