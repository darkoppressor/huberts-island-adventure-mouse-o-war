/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "profile.h"
#include "world.h"
#include "file_io.h"

#include <fstream>

using namespace std;

Level_Properties Profile::load_level_properties_weather_machine(short level_to_change){
    Level_Properties lp;
    lp.current_sub_level=-1;
    lp.level_beaten=false;

    if(player.game_mode==GAME_MODE_SP_ADVENTURE){
        //Create a string to hold the current level number.
        string current_level="";
        ss.clear();ss.str("");ss<<level_to_change;current_level=ss.str();

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
            lp=save_level_properties_weather_machine(level_to_change,lp);
        }
    }

    return lp;
}

Level_Properties Profile::save_level_properties_weather_machine(short level_to_change,Level_Properties lp){
    if(player.game_mode==GAME_MODE_SP_ADVENTURE){
        make_directories();
        //Create a string to hold the current level number.
        string current_level="";
        ss.clear();ss.str("");ss<<level_to_change;current_level=ss.str();

        ofstream save;
        string save_name=get_home_directory()+"profiles/"+player.name+"/saves/"+current_level+"/level_properties.blazesave";
        save.open(save_name.c_str());

        if(lp.current_sub_level==-1){
            lp.current_sub_level=0;
            lp.level_beaten=false;
        }

        if(save.is_open()){
            save<<lp.current_sub_level;
            save<<"\n";
            save<<lp.level_beaten;
            save<<"\n";

            save.close();
            save.clear();
        }
    }

    return lp;
}
