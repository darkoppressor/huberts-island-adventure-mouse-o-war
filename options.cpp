/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "options.h"
#include "world.h"
#include "version.h"
#include "message_log.h"
#include "file_io.h"

using namespace std;
using namespace boost::algorithm;

Version_Series::Version_Series(int get_major_1,int get_minor_1,int get_micro_1,int get_major_2,int get_minor_2,int get_micro_2){
    major_1=get_major_1;
    minor_1=get_minor_1;
    micro_1=get_micro_1;

    major_2=get_major_2;
    minor_2=get_minor_2;
    micro_2=get_micro_2;
}

bool load_current_profile(){
    File_IO_Load load;
    string file_to_load=profile.get_home_directory()+"profiles/current_profile.cfg";
    load.open(file_to_load);

    if(load.is_opened()){
        istringstream data_stream(load.get_data());

        data_stream>>player.name;

        if(data_stream.fail()){
            update_error_log("Failed to load 'current_profile.cfg.'");
            load.close();
            return false;
        }
        load.close();
    }
    else{
        if(!save_current_profile()){
            return false;
        }
    }

    return true;
}

bool save_current_profile(){
    //If the player's name is set to something, we know at least one profile exists,
    //and we can save safely.
    if(player.name!="\x1F"){
        profile.make_directories();
        ofstream save;
        string save_name=profile.get_home_directory()+"profiles/current_profile.cfg";
        save.open(save_name.c_str());

        save<<player.name;

        if(save.fail()){
            update_error_log("Failed to save 'current_profile.cfg.'");
            save.close();
            save.clear();
            return false;
        }

        save.close();
        save.clear();
    }
    //If the player's name is not set, no profile exists.
    else{
        fprintf(stdout,"No profile exists. Prompting the player to create one...\n");
        return false;
    }

    return true;
}

int options_version_compare(int major_1,int minor_1,int micro_1,int major_2,int minor_2,int micro_2){
    if(major_1==major_2){
        if(minor_1==minor_2){
            if(micro_1==micro_2){
                return 0;
            }
            else if(micro_1<micro_2){
                return -1;
            }
            else if(micro_1>micro_2){
                return 1;
            }
        }
        else if(minor_1<minor_2){
            return -1;
        }
        else if(minor_1>minor_2){
            return 1;
        }
    }
    else if(major_1<major_2){
        return -1;
    }
    else if(major_1>major_2){
        return 1;
    }
}

int options_which_version_series(vector<Version_Series>* version_series,int major,int minor,int micro){
    for(int i=0;i<version_series->size();i++){
        if(options_version_compare(major,minor,micro,version_series->at(i).major_1,version_series->at(i).minor_1,version_series->at(i).micro_1)>=0 &&
           options_version_compare(major,minor,micro,version_series->at(i).major_2,version_series->at(i).minor_2,version_series->at(i).micro_2)<=0){
            return i;
        }
    }

    return -1;
}

bool options_version_compatible(string name_to_check){
    int current_major=Version::MAJOR;
    int current_minor=Version::MINOR;
    int current_micro=Version::MICRO;

    File_IO_Load load;
    string file_to_load=profile.get_home_directory()+"profiles/";
    file_to_load+=name_to_check;
    file_to_load+="/options.cfg";
    load.open(file_to_load);

    string version="";

    if(load.is_opened()){
        istringstream data_stream(load.get_data());

        data_stream>>version;

        load.close();
    }
    else{
        update_error_log("Failed to load profile version!");

        return false;
    }

    int major=0;
    int minor=0;
    int micro=0;

    vector<string> version_strings;
    split(version_strings,version,is_any_of("."));

    major=strtol(version_strings[0].c_str(),NULL,0);
    minor=strtol(version_strings[1].c_str(),NULL,0);
    if(version_strings.size()>2){
       micro=strtol(version_strings[2].c_str(),NULL,0);
    }
    else{
        micro=0;
    }

    //Version series are defined by a start version and an end version.
    //The start version must be less than or equal to the end version.
    vector<Version_Series> version_series;

    version_series.push_back(Version_Series(1,0,0,1,1,0));
    version_series.push_back(Version_Series(1,2,0,1,2,0));
    version_series.push_back(Version_Series(1,3,0,1,3,0));
    version_series.push_back(Version_Series(1,4,0,1,4,0));
    version_series.push_back(Version_Series(1,5,0,1,5,0));

    if(options_which_version_series(&version_series,major,minor,micro)!=options_which_version_series(&version_series,current_major,current_minor,current_micro)){
        string error_message="Version incompatibility! The profile '"+name_to_check+"' was started with version "+player.num_to_string(major)+"."+player.num_to_string(minor)+"."+player.num_to_string(micro)+".";
        error_message+=" Current version is "+player.num_to_string(current_major)+"."+player.num_to_string(current_minor)+"."+player.num_to_string(current_micro)+".";
        update_error_log(error_message);

        return false;
    }

    return true;
}

bool options_load(){
    //If there is no profile.
    if(player.name=="\x1F"){
        return false;
    }
    //If there is a profile.
    else{
        if(!options_version_compatible(player.name)){
            return false;
        }
        else{
            File_IO_Load load;
            string file_to_load=profile.get_home_directory()+"profiles/";
            file_to_load+=player.name;
            file_to_load+="/options.cfg";
            load.open(file_to_load);

            string version="";

            if(load.is_opened()){
                istringstream data_stream(load.get_data());

                //Temporary variables for loading command data.
                short type;
                int key;
                unsigned int which_joystick;
                unsigned int joy_button;
                unsigned int joy_axis;
                bool joy_axis_direction;
                unsigned int joy_hat;
                short joy_hat_direction;
                unsigned int joy_ball;
                short joy_ball_direction;

                data_stream>>version;

                data_stream>>player.option_fps;
                data_stream>>player.option_dev;
                data_stream>>player.option_effect_limit;
                data_stream>>player.option_volume_global;
                data_stream>>player.option_mute_global;
                data_stream>>player.option_volume_sound;
                data_stream>>player.option_mute_sound;
                data_stream>>player.option_volume_music;
                data_stream>>player.option_mute_music;
                data_stream>>player.option_holiday_cheer;
                data_stream>>player.option_time_of_day;
                data_stream>>player.option_time_of_day_levels;
                data_stream>>player.option_difficulty;
                data_stream>>player.option_hardware_cursor;
                data_stream>>player.option_hints;

                for(int i=0;i<COMMAND_END;i++){
                    data_stream>>type;
                    data_stream>>key;
                    data_stream>>which_joystick;
                    data_stream>>joy_button;
                    data_stream>>joy_axis;
                    data_stream>>joy_axis_direction;
                    data_stream>>joy_hat;
                    data_stream>>joy_hat_direction;
                    data_stream>>joy_ball;
                    data_stream>>joy_ball_direction;

                    player.keys[i].type=type;
                    player.keys[i].key=(SDL_Scancode)key;
                    player.keys[i].which_joystick=(Uint8)which_joystick;
                    player.keys[i].joy_button=(Uint8)joy_button;
                    player.keys[i].joy_axis=(Uint8)joy_axis;
                    player.keys[i].joy_axis_direction=joy_axis_direction;
                    player.keys[i].joy_hat=(Uint8)joy_hat;
                    player.keys[i].joy_hat_direction=joy_hat_direction;
                    player.keys[i].joy_ball=(Uint8)joy_ball;
                    player.keys[i].joy_ball_direction=joy_ball_direction;
                }

                for(int i=0;i<3;i++){
                    for(int n=0;n<COMMAND_END;n++){
                        data_stream>>type;
                        data_stream>>key;
                        data_stream>>which_joystick;
                        data_stream>>joy_button;
                        data_stream>>joy_axis;
                        data_stream>>joy_axis_direction;
                        data_stream>>joy_hat;
                        data_stream>>joy_hat_direction;
                        data_stream>>joy_ball;
                        data_stream>>joy_ball_direction;

                        player.mp_keys[i][n].type=type;
                        player.mp_keys[i][n].key=(SDL_Scancode)key;
                        player.mp_keys[i][n].which_joystick=(Uint8)which_joystick;
                        player.mp_keys[i][n].joy_button=(Uint8)joy_button;
                        player.mp_keys[i][n].joy_axis=(Uint8)joy_axis;
                        player.mp_keys[i][n].joy_axis_direction=joy_axis_direction;
                        player.mp_keys[i][n].joy_hat=(Uint8)joy_hat;
                        player.mp_keys[i][n].joy_hat_direction=joy_hat_direction;
                        player.mp_keys[i][n].joy_ball=(Uint8)joy_ball;
                        player.mp_keys[i][n].joy_ball_direction=joy_ball_direction;
                    }
                }

                load.close();
            }
            else{
                //Save options to options.cfg.
                if(!options_save()){
                    update_error_log("Failed to load 'options.cfg.'");
                    return false;
                }
            }

            return true;
        }
    }
}

bool options_save(){
    //If there is no profile.
    if(player.name=="\x1F"){
        return false;
    }
    //If there is a profile.
    else{
        //Update the keys for any active MP players.
        for(int i=0;i<player.mp_keys.size();i++){
            if(i<mp_players.size()){
                mp_players[i].keys=player.mp_keys[i];
            }
        }

        profile.make_directories();
        ofstream save;

        string save_name=profile.get_home_directory()+"profiles/";
        save_name+=player.name;
        save_name+="/options.cfg";
        save.open(save_name.c_str());

        string current_version="";
        ss.clear();ss.str("");ss<<Version::MAJOR;ss<<".";ss<<Version::MINOR;ss<<".";ss<<Version::MICRO;current_version=ss.str();

        if(save!=NULL){
            save<<current_version;
            save<<"\n";
            save<<player.option_fps;
            save<<"\n";
            save<<player.option_dev;
            save<<"\n";
            save<<player.option_effect_limit;
            save<<"\n";
            save<<player.option_volume_global;
            save<<"\n";
            save<<player.option_mute_global;
            save<<"\n";
            save<<player.option_volume_sound;
            save<<"\n";
            save<<player.option_mute_sound;
            save<<"\n";
            save<<player.option_volume_music;
            save<<"\n";
            save<<player.option_mute_music;
            save<<"\n";
            save<<player.option_holiday_cheer;
            save<<"\n";
            save<<player.option_time_of_day;
            save<<"\n";
            save<<player.option_time_of_day_levels;
            save<<"\n";
            save<<player.option_difficulty;
            save<<"\n";
            save<<player.option_hardware_cursor;
            save<<"\n";
            save<<player.option_hints;
            save<<"\n";

            for(int i=0;i<COMMAND_END;i++){
                save<<player.keys[i].type;
                save<<"\n";
                save<<(int)player.keys[i].key;
                save<<"\n";
                save<<(unsigned int)player.keys[i].which_joystick;
                save<<"\n";
                save<<(unsigned int)player.keys[i].joy_button;
                save<<"\n";
                save<<(unsigned int)player.keys[i].joy_axis;
                save<<"\n";
                save<<player.keys[i].joy_axis_direction;
                save<<"\n";
                save<<(unsigned int)player.keys[i].joy_hat;
                save<<"\n";
                save<<player.keys[i].joy_hat_direction;
                save<<"\n";
                save<<(unsigned int)player.keys[i].joy_ball;
                save<<"\n";
                save<<player.keys[i].joy_ball_direction;
                save<<"\n";
            }

            for(int i=0;i<3;i++){
                for(int n=0;n<COMMAND_END;n++){
                    save<<player.mp_keys[i][n].type;
                    save<<"\n";
                    save<<(int)player.mp_keys[i][n].key;
                    save<<"\n";
                    save<<(unsigned int)player.mp_keys[i][n].which_joystick;
                    save<<"\n";
                    save<<(unsigned int)player.mp_keys[i][n].joy_button;
                    save<<"\n";
                    save<<(unsigned int)player.mp_keys[i][n].joy_axis;
                    save<<"\n";
                    save<<player.mp_keys[i][n].joy_axis_direction;
                    save<<"\n";
                    save<<(unsigned int)player.mp_keys[i][n].joy_hat;
                    save<<"\n";
                    save<<player.mp_keys[i][n].joy_hat_direction;
                    save<<"\n";
                    save<<(unsigned int)player.mp_keys[i][n].joy_ball;
                    save<<"\n";
                    save<<player.mp_keys[i][n].joy_ball_direction;
                    save<<"\n";
                }
            }

            save.close();
            save.clear();
        }
        else{
            update_error_log("Failed to save 'options.cfg.'");
            return false;
        }

        return true;
    }
}

bool global_options_load(){
    File_IO_Load load;
    string file_to_load=profile.get_home_directory()+"global_options.cfg";
    load.open(file_to_load);

    if(load.is_opened()){
        bool multi_line_comment=false;

        //As long as we haven't reached the end of the file.
        while(!load.eof()){
            string line="";

            //The option strings used in the file.

            string str_fullscreen_mode="fullscreen mode:";
            string str_screen_width="screen width:";
            string str_screen_height="screen height:";
            string str_fullscreen="fullscreen:";
            string str_display_number="display_number:";
            string str_lighting_tile_size="lighting tile size:";
            string str_npc_fade_adventure="npc fade time adventure:";
            string str_npc_fade_survival="npc fade time survival:";

            //Grab the next line of the file.
            load.getline(&line);

            //Clear initial whitespace from the line.
            trim(line);

            //If the line ends a multi-line comment.
            if(icontains(line,"*/")){
                multi_line_comment=false;
            }
            //If the line starts a multi-line comment.
            if(!multi_line_comment && istarts_with(line,"/*")){
                multi_line_comment=true;
            }
            //If the line is a comment.
            else if(!multi_line_comment && istarts_with(line,"//")){
                //Ignore this line.
            }

            //Load data based on the line.

            //Fullscreen mode
            else if(!multi_line_comment && icontains(line,str_fullscreen_mode)){
                //Clear the data name.
                line.erase(0,str_fullscreen_mode.length());

                player.option_fullscreen_mode=line;
            }
            //Screen width
            else if(!multi_line_comment && icontains(line,str_screen_width)){
                //Clear the data name.
                line.erase(0,str_screen_width.length());

                player.option_screen_width=atoi(line.c_str());
            }
            //Screen height
            else if(!multi_line_comment && icontains(line,str_screen_height)){
                //Clear the data name.
                line.erase(0,str_screen_height.length());

                player.option_screen_height=atoi(line.c_str());
            }
            //Fullscreen
            else if(!multi_line_comment && icontains(line,str_fullscreen)){
                //Clear the data name.
                line.erase(0,str_fullscreen.length());

                player.option_fullscreen=(bool)atoi(line.c_str());
            }
            //Display number
            else if(!multi_line_comment && icontains(line,str_display_number)){
                //Clear the data name.
                line.erase(0,str_display_number.length());

                player.option_display_number=atoi(line.c_str());
            }
            //Lighting tile size
            else if(!multi_line_comment && icontains(line,str_lighting_tile_size)){
                //Clear the data name.
                line.erase(0,str_lighting_tile_size.length());

                player.option_lighting_tile_size=atoi(line.c_str());
            }
            //NPC fade time adventure
            else if(!multi_line_comment && icontains(line,str_npc_fade_adventure)){
                //Clear the data name.
                line.erase(0,str_npc_fade_adventure.length());

                player.option_npc_fade_adventure=atoi(line.c_str());
            }
            //NPC fade time survival
            else if(!multi_line_comment && icontains(line,str_npc_fade_survival)){
                //Clear the data name.
                line.erase(0,str_npc_fade_survival.length());

                player.option_npc_fade_survival=atoi(line.c_str());
            }
        }

        LIGHTING_TILE_SIZE=player.option_lighting_tile_size;

        load.close();
    }
    else{
        if(!global_options_save()){
            return false;
        }
    }

    return true;
}

bool global_options_save(){
    ofstream save;

    string save_name=profile.get_home_directory()+"global_options.cfg";
    save.open(save_name.c_str());

    if(save!=NULL){
        save<<"//Sets the fullscreen mode to use.\n//Note that this does not toggle between fullscreen and windowed. It sets the method of making the window fullscreen.\n";
        save<<"//standard - Attempts to make the window fullscreen.\n//windowed - The window remains windowed, but is stretched to the screen dimensions, and the titlebar is removed.\n//desktop - attempts to match the desktop mode.\n//Default: windowed"<<"\n";
        save<<"fullscreen mode:"<<player.option_fullscreen_mode<<"\n\n";

        save<<"//Sets the dimensions of the window.\n//Default: 800x600"<<"\n";
        save<<"screen width:"<<player.option_screen_width<<"\n";
        save<<"screen height:"<<player.option_screen_height<<"\n\n";

        save<<"//Sets the fullscreen status of the game window.\n//0 - Windowed.\n//1 - Fullscreen.\n//Default: 0"<<"\n";
        save<<"fullscreen:"<<player.option_fullscreen<<"\n\n";

        save<<"//The display number that the game's window should be placed on.\n//Default: 0"<<"\n";
        save<<"display_number:"<<player.option_display_number<<"\n\n";

        save<<"//Sets the resolution of the lighting tiles.\n//Size should be a multiple of 2.\n";
        save<<"//Smaller sizes will cause lighting to be smoother, but may lead to significant performance loss.\n";
        save<<"//Sizes larger than 32 will look very weird.\n//Default: 24"<<"\n";
        save<<"lighting tile size:"<<player.option_lighting_tile_size<<"\n\n";

        save<<"//Sets the number of seconds before stunned NPCs fade away.\n";
        save<<"//A value of 0 means that bodies never fade.\n//Defaults:\n//Adventure: 30\n//Survival: 15"<<"\n";
        save<<"npc fade time adventure:"<<player.option_npc_fade_adventure<<"\n";
        save<<"npc fade time survival:"<<player.option_npc_fade_survival<<"\n";

        save.close();
        save.clear();
    }
    else{
        update_error_log("Failed to save 'global_options.cfg.'");
        return false;
    }

    return true;
}

bool save_location_load(){
    File_IO_Load load("save_location.cfg");

    if(load.is_opened()){
        istringstream data_stream(load.get_data());

        bool multi_line_comment=false;

        //As long as we haven't reached the end of the file.
        while(!load.eof()){
            string line="";

            //The option strings used in the file.

            string str_save_location="save location:";

            //Grab the next line of the file.
            load.getline(&line);

            //Clear initial whitespace from the line.
            trim(line);

            //If the line ends a multi-line comment.
            if(icontains(line,"*/")){
                multi_line_comment=false;
            }
            //If the line starts a multi-line comment.
            if(!multi_line_comment && istarts_with(line,"/*")){
                multi_line_comment=true;
            }
            //If the line is a comment.
            else if(!multi_line_comment && istarts_with(line,"//")){
                //Ignore this line.
            }

            //Load data based on the line.

            //Save location
            else if(!multi_line_comment && icontains(line,str_save_location)){
                //Clear the data name.
                line.erase(0,str_save_location.length());

                option_save_location=atoi(line.c_str());
            }
        }

        load.close();
    }
    else{
        if(!save_location_save()){
            return false;
        }
    }

    return true;
}

bool save_location_save(){
    ofstream save;

    string save_name="save_location.cfg";
    save.open(save_name.c_str());

    if(save!=NULL){
        save<<"//Sets the save location to use for user data.\n";
        save<<"//0 - Local. Saves in the game directory.\n//1 - Home. Saves in:";
        save<<"\n//Windows (Full Game) - <$USERPROFILE>/My Documents/My Games/huberts-island-adventure-mouse-o-war";
        save<<"\n//Windows (Demo) - <$USERPROFILE>/My Documents/My Games/huberts-island-adventure-mouse-o-war-demo";
        save<<"\n//Linux (Full Game) - <$HOME>/.huberts-island-adventure-mouse-o-war";
        save<<"\n//Linux (Demo) - <$HOME>/.huberts-island-adventure-mouse-o-war-demo";
        save<<"\n//OS X (Full Game) - /Users/<your name>/Library/Application Support/huberts-island-adventure-mouse-o-war";
        save<<"\n//OS X (Demo) - /Users/<your name>/Library/Application Support/huberts-island-adventure-mouse-o-war-demo";
        save<<"\n//Default: 1\n";
        save<<"save location:"<<option_save_location<<"\n\n";

        save.close();
        save.clear();
    }
    else{
        update_error_log("Failed to save 'save_location.cfg.'");
        return false;
    }

    return true;
}
