/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "profile.h"
#include "world.h"
#include "file_io.h"

#include <fstream>

using namespace std;

Level_Stats Profile::return_level_stats(short level_to_check){
    Level_Stats level_stats;
    level_stats.seconds_total_time=0;
    level_stats.seconds_best_time=4294967295;

    if(player.game_mode==GAME_MODE_SP_ADVENTURE){
        //Create a string to hold the current level number.
        string current_level="";
        ss.clear();ss.str("");ss<<level_to_check;current_level=ss.str();

        File_IO_Load load;
        string level_to_load="";

        level_to_load=get_home_directory()+"profiles/"+player.name+"/saves/"+current_level+"/stats.blazesave";
        load.open(level_to_load);

        if(load.is_opened()){
            istringstream data_stream(load.get_data());

            data_stream>>level_stats.seconds_total_time;
            data_stream>>level_stats.seconds_best_time;

            load.close();
        }
        else{
        }
    }

    return level_stats;
}

bool Profile::load_level_stats(){
    //If there is no profile.
    if(player.name=="\x1F"){
        return false;
    }
    //If there is a profile.
    else{
        if(player.game_mode==GAME_MODE_SP_ADVENTURE){
            //Create a string to hold the current level number.
            string current_level="";
            ss.clear();ss.str("");ss<<player.current_level;current_level=ss.str();

            File_IO_Load load;
            string level_to_load="";

            level_to_load=get_home_directory()+"profiles/"+player.name+"/saves/"+current_level+"/stats.blazesave";
            load.open(level_to_load);

            if(load.is_opened()){
                istringstream data_stream(load.get_data());

                data_stream>>player.seconds_total_current_level;
                data_stream>>player.seconds_best_current_level;

                load.close();
            }
            else{
                return false;
            }
        }

        return true;
    }
}

bool Profile::save_level_stats(){
    //If there is no profile.
    if(player.name=="\x1F"){
        return false;
    }
    //If there is a profile.
    else{
        if(player.game_mode==GAME_MODE_SP_ADVENTURE){
            //Create a string to hold the current level number.
            string current_level="";
            ss.clear();ss.str("");ss<<player.current_level;current_level=ss.str();

            ofstream save;
            string save_name="";

            save_name=get_home_directory()+"profiles/"+player.name+"/saves/"+current_level+"/stats.blazesave";
            save.open(save_name.c_str());

            if(save.is_open()){
                save<<player.seconds_total_current_level;
                save<<"\n";
                save<<player.seconds_best_current_level;
                save<<"\n";

                save.close();
                save.clear();
            }
            else{
                return false;
            }
        }

        return true;
    }
}
