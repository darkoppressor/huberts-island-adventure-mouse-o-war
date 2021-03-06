/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "profile.h"
#include "world.h"
#include "message_log.h"
#include "file_io.h"

#include <fstream>

using namespace std;

Level_Properties Profile::load_level_properties_map(short level_to_check){
    Level_Properties lp;
    lp.current_sub_level=0;
    lp.level_beaten=false;

    if(player.game_mode==GAME_MODE_SP_ADVENTURE){
        //Create a string to hold the current level number.
        string current_level="";
        ss.clear();ss.str("");ss<<level_to_check;current_level=ss.str();

        File_IO_Load load;
        string level_to_load=get_home_directory()+"profiles/"+player.name+"/saves/"+current_level+"/level_properties.blazesave";
        load.open(level_to_load);

        if(load.is_opened()){
            istringstream data_stream(load.get_data());

            data_stream>>lp.current_sub_level;

            data_stream>>lp.level_beaten;

            load.close();
        }
        else{
        }
    }

    return lp;
}

Spawn_Coords Profile::return_level_spawn(short level_to_check){
    Spawn_Coords sc;
    sc.x=0.0;
    sc.y=0.0;

    if(player.game_mode==GAME_MODE_SP_ADVENTURE){
        //Setup the level string.
        string current_level="";
        ss.clear();ss.str("");ss<<level_to_check;current_level=ss.str();

        File_IO_Load load;
        string level_to_load="";

        //******************************//
        // Now, we will read the items: //
        //******************************//

        level_to_load="data/levels/"+current_level+"/items.blazelevel";
        load.open(level_to_load);

        //As long as the file opened successfully, proceed to load the data.
        if(load.is_opened()){
            istringstream data_stream(load.get_data());

            //Now we make a temporary variable for getting the type of each item being loaded.
            short type;
            double x;
            double y;
            int goal_level_to_load;
            bool goal_secret;

            //Then we iterate through all of the items in the file, setting each one.
            while(!data_stream.eof()){
                type=30000;
                x=0;
                y=0;
                goal_level_to_load=0;
                goal_secret=false;

                //For each item, load the type information from the file.
                data_stream >> type;
                data_stream >> x;
                data_stream >> y;
                data_stream >> goal_level_to_load;
                data_stream >> goal_secret;

                if(type==ITEM_SPAWNPOINT){
                    sc.x=x;
                    sc.y=y;
                    break;
                }
            }

            //When we are done using the level file, we close it, and clear load for its next use.
            load.close();
        }
        //If the level file could not be loaded for any reason, we print an error message letting the user know.
        else{
            update_error_log("Error reading level spawn point.");
        }
    }

    return sc;
}
