/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "profile.h"
#include "world.h"
#include "inventory.h"
#include "render.h"
#include "string_stuff.h"
#include "options.h"
#include "holidays.h"
#include "message_log.h"
#include "button_events.h"

#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#ifdef GAME_OS_OSX
    #include <CoreServices/CoreServices.h>
#endif

using namespace std;

Profile::Profile(){
    fov_radius_map=10;

    version_mismatch=false;
}

void Profile::correct_slashes(string* str_input){
    boost::algorithm::replace_all(*str_input,"\\","/");
}

string Profile::get_home_directory(){
    string str_home="";
    string game_name="huberts-island-adventure-mouse-o-war";

    #ifdef GAME_DEMO
        game_name+="-demo";
    #endif

    if(option_save_location==SAVE_LOCATION_HOME){
        #ifdef GAME_OS_WINDOWS
            str_home=getenv("USERPROFILE");
            str_home+="/My Documents/My Games/";
            str_home+=game_name;
            str_home+="/";
        #endif

        #ifdef GAME_OS_LINUX
            str_home=getenv("HOME");
            str_home+="/.";
            str_home+=game_name;
            str_home+="/";
        #endif

        #ifdef GAME_OS_OSX
            FSRef fsref;
            OSType folder_type=kApplicationSupportFolderType;
            char path[PATH_MAX];
            FSFindFolder(kUserDomain,folder_type,kCreateFolder,&fsref);
            FSRefMakePath(&fsref,(uint8_t*)&path,PATH_MAX);

            str_home=path;
            str_home+="/";
            str_home+=game_name;
            str_home+="/";
        #endif

        correct_slashes(&str_home);
    }

    return str_home;
}

void Profile::make_home_directory(){
    if(option_save_location==SAVE_LOCATION_HOME){
        #ifdef GAME_OS_WINDOWS
            string str_my_games=getenv("USERPROFILE");
            str_my_games+="/My Documents/My Games";

            correct_slashes(&str_my_games);

            boost::filesystem::create_directory(str_my_games);
        #endif

        string str_home=get_home_directory();

        //Remove the ending slash.
        str_home.erase(str_home.length()-1,1);

        correct_slashes(&str_home);

        boost::filesystem::create_directory(str_home);
    }
}

void Profile::make_directories(){
    string temp="";

    make_home_directory();

    boost::filesystem::create_directory(get_home_directory()+"profiles");
    boost::filesystem::create_directory(get_home_directory()+"profiles/backups");

    //If there is a profile.
    if(player.name!="\x1F"){
        temp=get_home_directory()+"profiles/";
        temp+=player.name;
        boost::filesystem::create_directory(temp.c_str());

        temp=get_home_directory()+"profiles/";
        temp+=player.name;
        temp+="/screenshots";
        boost::filesystem::create_directory(temp.c_str());

        temp=get_home_directory()+"profiles/";
        temp+=player.name;
        temp+="/saves";
        boost::filesystem::create_directory(temp.c_str());

        for(int i=0;i<=LAST_LEVEL;i++){
            temp=get_home_directory()+"profiles/";
            temp+=player.name;
            temp+="/saves/";
            ss.clear();ss.str("");ss<<i;temp+=ss.str();
            boost::filesystem::create_directory(temp.c_str());
        }
    }
}

void Profile::delete_profile(int profile_to_delete){
    bool deleting_this_profile=false;

    //If the profile to be deleted is the current profile.
    if(profile_list[profile_to_delete]==player.name){
        deleting_this_profile=true;

        player.stop_game();

        sliders.clear();

        window_manager.configure_main_menu();
    }

    string temp=get_home_directory()+"profiles/";
    temp+=profile_list[profile_to_delete];
    boost::filesystem::remove_all(temp.c_str());

    profile_list.erase(profile_list.begin()+profile_to_delete);

    save_profile_list();

    set_change_profile_buttons();

    if(deleting_this_profile){
        if(profile_list.size()==0){
            button_event_open_window_create_profile(NULL,0);
        }
        else{
            if(options_version_compatible(profile_list[0])){
                //Set the profile name.
                player.name=profile_list[0];

                //Load this new profile.
                options_load();
                load_profile_global_data();

                save_current_profile();
            }
            else{
                profile.version_mismatch=true;
                player.reset();
                button_event_open_window_create_profile(NULL,0);
            }
        }
    }

    //Show or hide the hardware mouse cursor.
    if(player.option_hardware_cursor){
        SDL_ShowCursor(SDL_ENABLE);
    }
    else{
        SDL_ShowCursor(SDL_DISABLE);
    }

    global_options_load();
}

void Profile::select_profile(int profile_to_select){
    if(player.name!=profile_list[profile_to_select]){
        if(options_version_compatible(profile_list[profile_to_select])){
            version_mismatch=false;

            //If a game was already in progress, save the current profile before loading the new one.
            if(player.game_in_progress){
                save_level_data();
            }

            options_save();
            save_profile_global_data();
            save_current_profile();
            save_profile_list();

            //If a game was in progress with the previous profile, it should be stopped.
            player.stop_game();

            sliders.clear();

            //Set the profile name.
            player.name=profile_list[profile_to_select];

            //Load this new profile.
            options_load();
            load_profile_global_data();

            save_current_profile();

            window_manager.close_windows(0);

            window_manager.configure_main_menu();

            vector_windows[WINDOW_MAIN_MENU].turn_on();

            //Show or hide the hardware mouse cursor.
            if(player.option_hardware_cursor){
                SDL_ShowCursor(SDL_ENABLE);
            }
            else{
                SDL_ShowCursor(SDL_DISABLE);
            }

            global_options_load();
        }
    }
}

void Profile::create_profile(){
    if(creating_profile.length()>0 && !does_profile_exist(creating_profile)){
        version_mismatch=false;

        //If a game was already in progress, save the current profile before creating the new one.
        if(player.game_in_progress){
            save_level_data();
        }

        options_save();
        save_profile_global_data();
        save_current_profile();
        save_profile_list();

        //If a game was in progress with the previous profile, it should be stopped.
        player.stop_game();

        sliders.clear();

        //Set the profile name.
        player.name=creating_profile;

        //Create a new entry in the profile list for this profile.
        profile_list.push_back(string(creating_profile));

        //Load the max leaf and cheese count from the level files.
        load_leaves_cheese_counts();

        //Save this new profile.
        options_save();
        save_profile_global_data();
        save_current_profile();
        save_profile_list();

        window_manager.close_windows(0);

        window_manager.configure_main_menu();

        vector_windows[WINDOW_MAIN_MENU].turn_on();

        //Show or hide the hardware mouse cursor.
        if(player.option_hardware_cursor){
            SDL_ShowCursor(SDL_ENABLE);
        }
        else{
            SDL_ShowCursor(SDL_DISABLE);
        }

        global_options_load();

        play_game_start_sound();
    }
}

bool Profile::does_profile_exist(string profile_name){
    string profile_string=make_string_lower_case(profile_name);
    string temp="";

    for(int i=0;i<profile_list.size();i++){
        temp=make_string_lower_case(profile_list[i]);
        if(temp==profile_string){
            return true;
        }
    }

    return false;
}

bool Profile::load_profile_list(){
    string temp="";
    int number_of_profiles=0;

    profile_list.clear();

    ifstream load;
    string file_to_load=get_home_directory()+"profiles/profile_list.cfg";
    load.open(file_to_load.c_str(),ifstream::in);

    if(load!=NULL){

        load>>number_of_profiles;

        for(int i=0;i<number_of_profiles;i++){
            load>>temp;

            profile_list.push_back(string(temp));
        }

        load.close();
        load.clear();
    }
    else{
        if(!save_profile_list()){
            return false;
        }
    }

    return true;
}

bool Profile::save_profile_list(){
    make_directories();
    ofstream save;
    string save_name=get_home_directory()+"profiles/profile_list.cfg";
    save.open(save_name.c_str());

    if(save!=NULL){

        save<<profile_list.size();
        save<<"\n";

        for(int i=0;i<profile_list.size();i++){
            save<<profile_list[i];
            save<<"\n";
        }

        save.close();
        save.clear();
    }
    else{
        return false;
    }

    return true;
}

void Profile::save_backup(){
    //If there is a profile.
    if(player.name!="\x1F"){
        if(player.game_mode==GAME_MODE_SP_ADVENTURE){
            make_directories();

            string profile_directory=get_home_directory()+"profiles/";
            profile_directory+=player.name;

            string profile_backup=get_home_directory()+"profiles/backups/";
            profile_backup+=player.name;

            try{
                boost::filesystem::remove_all(profile_backup.c_str());

                boost::filesystem::create_directory(profile_backup.c_str());
            }
            catch(boost::filesystem::filesystem_error& e){
                msg="Filesystem error: ";
                msg+=e.what();
                update_error_log(msg);
            }

            for(boost::filesystem::recursive_directory_iterator end_of_files,dir(profile_directory);dir!=end_of_files;dir++){
                boost::filesystem::path temp_path(*dir);
                string destination_file=temp_path.string();
                boost::algorithm::replace_first(destination_file,profile_directory,profile_backup);
                boost::filesystem::copy(*dir,destination_file);
            }
        }
    }
}

bool Profile::load_profile_global_data(){
    if(!load_inventory()){
        return false;
    }
    if(!load_stats()){
        return false;
    }
    if(!load_achievements()){
        return false;
    }
    if(!load_boss_states()){
        return false;
    }
    if(!load_upgrades()){
        return false;
    }

    return true;
}

bool Profile::save_profile_global_data(){
    if(player.game_mode==GAME_MODE_SP_ADVENTURE){
        if(!save_inventory()){
            return false;
        }
        if(!save_stats()){
            return false;
        }
        /**if(!save_achievements()){
            return false;
        }*/
        if(!save_boss_states()){
            return false;
        }
    }

    return true;
}

bool Profile::load_map(){
    //If there is no profile.
    if(player.name=="\x1F"){
        return false;
    }
    //If there is a profile.
    else{
        current_level_map_data.clear();
        current_level_map_data.resize(level.level_x/TILE_SIZE,vector<bool>(level.level_y/TILE_SIZE));

        for(int int_y=0;int_y<level.level_y/TILE_SIZE;int_y++){
            for(int int_x=0;int_x<level.level_x/TILE_SIZE;int_x++){
                current_level_map_data[int_x][int_y]=false;
            }
        }

        //Create a string to hold the current level number.
        string current_level="";
        ss.clear();ss.str("");ss<<player.current_level;current_level=ss.str();

        ifstream load;
        string level_to_load=get_home_directory()+"profiles/"+player.name+"/saves/"+current_level+"/map.blazesave";
        load.open(level_to_load.c_str(),ifstream::in);

        if(load!=NULL){
            bool seen=false;

            for(int int_y=0;int_y<level.level_y/TILE_SIZE;int_y++){
                for(int int_x=0;int_x<level.level_x/TILE_SIZE;int_x++){
                    load >> seen;
                    current_level_map_data[int_x][int_y]=seen;
                }
            }

            load.close();
            load.clear();
        }
        else{
            if(!save_map()){
                return false;
            }
        }

        return true;
    }
}

bool Profile::save_map(){
    //If there is no profile.
    if(player.name=="\x1F"){
        return false;
    }
    //If there is a profile.
    else{
        if(player.game_mode==GAME_MODE_SP_ADVENTURE){
            make_directories();
            //Create a string to hold the current level number.
            string current_level="";
            ss.clear();ss.str("");ss<<player.current_level;current_level=ss.str();

            ofstream save;
            string save_name=get_home_directory()+"profiles/"+player.name+"/saves/"+current_level+"/map.blazesave";
            save.open(save_name.c_str());

            if(save!=NULL){
                for(int y=0;y<level.level_y/TILE_SIZE;y++){
                    for(int x=0;x<level.level_x/TILE_SIZE;x++){
                        save << current_level_map_data[x][y];
                        save << " ";
                    }
                }

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

bool Profile::load_level_properties(){
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

            ifstream load;
            string level_to_load=get_home_directory()+"profiles/"+player.name+"/saves/"+current_level+"/level_properties.blazesave";
            load.open(level_to_load.c_str(),ifstream::in);

            if(load!=NULL){
                load>>player.current_sub_level;

                load>>player.level_beaten;

                load.close();
                load.clear();
            }
            else{
                if(!save_level_properties()){
                    return false;
                }
            }
        }

        return true;
    }
}

bool Profile::save_level_properties(){
    //If there is no profile.
    if(player.name=="\x1F"){
        return false;
    }
    //If there is a profile.
    else{
        if(player.game_mode==GAME_MODE_SP_ADVENTURE){
            make_directories();
            //Create a string to hold the current level number.
            string current_level="";
            ss.clear();ss.str("");ss<<player.current_level;current_level=ss.str();

            ofstream save;
            string save_name=get_home_directory()+"profiles/"+player.name+"/saves/"+current_level+"/level_properties.blazesave";
            save.open(save_name.c_str());

            if(save!=NULL){
                save<<player.current_sub_level;
                save<<"\n";
                save<<player.level_beaten;
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

bool Profile::is_item_collectable(int type){
    if(type==ITEM_LEAF || type==ITEM_CHEESE || (type>=ITEM_SWIMMING_GEAR && type<=ITEM_SINK) ||
        type==ITEM_AMMO_BARREL || (type>=ITEM_KEY_GRAY && type<=ITEM_J_WING)){
        return true;
    }

    return false;
}

bool Profile::load_level_data(){
    //If there is no profile.
    if(player.name=="\x1F"){
        return false;
    }
    //If there is a profile.
    else{
        if(player.game_mode==GAME_MODE_SP_ADVENTURE){
            if(player.game_in_progress && player.persistent_level_data){
                load_map();

                //Create a string to hold the current level number.
                string current_level="";
                ss.clear();ss.str("");ss<<player.current_level;current_level=ss.str();

                ifstream load;
                string level_to_load="";

                //If the current level is not the world map.
                if(!player.on_worldmap()){
                    level_to_load=get_home_directory()+"profiles/"+player.name+"/saves/"+current_level+"/items.blazesave";
                    load.open(level_to_load.c_str(),ifstream::in);

                    if(load!=NULL){
                        bool collected=false;

                        for(int i=0;i<vector_items.size();i++){
                            //If the item is of a collectable type.
                            if(is_item_collectable(vector_items[i].type)){
                                load>>collected;
                                if(collected){
                                    vector_items[i].exists=false;
                                    if(vector_items[i].type==ITEM_LEAF){
                                        player.current_level_leaves++;
                                    }
                                    else if(vector_items[i].type==ITEM_CHEESE){
                                        player.current_level_cheese++;
                                    }
                                }
                            }
                        }

                        load.close();
                        load.clear();
                    }
                    else{
                        if(!save_level_data()){
                            return false;
                        }
                    }

                    level_to_load=get_home_directory()+"profiles/"+player.name+"/saves/"+current_level+"/doors.blazesave";
                    load.open(level_to_load.c_str(),ifstream::in);

                    if(load!=NULL){
                        bool opened=false;

                        for(int i=0;i<vector_doors.size();i++){
                            if(vector_doors[i].type==DOOR_TYPE_KEY){
                                load>>opened;
                                if(opened){
                                    vector_doors[i].open=true;
                                    vector_doors[i].frame=KEY_DOOR_SPRITES-1;
                                }
                            }
                        }

                        load.close();
                        load.clear();
                    }
                    else{
                        return false;
                    }

                    load_level_stats();
                }
                //If the current level is the world map.
                else{
                    level_to_load=get_home_directory()+"profiles/"+player.name+"/saves/"+current_level+"/world.blazesave";
                    load.open(level_to_load.c_str(),ifstream::in);

                    if(load!=NULL){
                        load>>player.world_x[player.current_level];
                        load>>player.world_y[player.current_level];
                        player.x=player.world_x[player.current_level];
                        player.y=player.world_y[player.current_level];

                        load>>player.facing;

                        load.close();
                        load.clear();
                    }
                    else{
                        player.world_x[player.current_level]=-1;
                        player.world_y[player.current_level]=-1;

                        if(!save_level_data()){
                            return false;
                        }
                    }
                }

                return true;
            }
            else{
                return false;
            }
        }
    }
}

bool Profile::save_level_data(){
    //If there is no profile.
    if(player.name=="\x1F"){
        return false;
    }
    //If there is a profile.
    else{
        if(player.game_mode==GAME_MODE_MP_ADVENTURE){
            player.mp_save_level_data();
        }

        if(player.game_mode==GAME_MODE_SP_ADVENTURE){
            if(player.game_in_progress && player.persistent_level_data){
                make_directories();
                save_level_properties();
                save_map();

                //Create a string to hold the current level number.
                string current_level="";
                ss.clear();ss.str("");ss<<player.current_level;current_level=ss.str();

                ofstream save;
                string save_name="";

                //If the current level is not the world map.
                if(!player.on_worldmap()){
                    save_name=get_home_directory()+"profiles/"+player.name+"/saves/"+current_level+"/items.blazesave";
                    save.open(save_name.c_str());

                    if(save!=NULL){
                        bool collected=false;

                        for(int i=0;i<vector_items.size();i++){
                            //If the item is of a collectable type.
                            if(is_item_collectable(vector_items[i].type)){
                                if(!vector_items[i].exists){
                                    collected=true;
                                }
                                else{
                                    collected=false;
                                }

                                save<<collected;
                                save<<" ";
                            }
                        }

                        save.close();
                        save.clear();
                    }
                    else{
                        return false;
                    }

                    save_name=get_home_directory()+"profiles/"+player.name+"/saves/"+current_level+"/doors.blazesave";
                    save.open(save_name.c_str());

                    if(save!=NULL){
                        bool opened=false;

                        for(int i=0;i<vector_doors.size();i++){
                            if(vector_doors[i].type==DOOR_TYPE_KEY){
                                if(vector_doors[i].open){
                                    opened=true;
                                }
                                else{
                                    opened=false;
                                }

                                save<<opened;
                                save<<" ";
                            }
                        }

                        save.close();
                        save.clear();
                    }
                    else{
                        return false;
                    }

                    save_level_stats();
                }
                //If the current level is the world map.
                else{
                    save_name=get_home_directory()+"profiles/"+player.name+"/saves/"+current_level+"/world.blazesave";
                    save.open(save_name.c_str());

                    double x=0;
                    double y=0;

                    if(player.world_x[player.current_level]==-1){
                        x=-1;
                    }
                    else{
                        player.world_x[player.current_level]=player.x;
                        x=player.world_x[player.current_level];
                    }

                    if(player.world_y[player.current_level]==-1){
                        y=-1;
                    }
                    else{
                        player.world_y[player.current_level]=player.y;
                        y=player.world_y[player.current_level];
                    }

                    if(save!=NULL){
                        save<<x;
                        save<<"\n";
                        save<<y;
                        save<<"\n";
                        save<<player.facing;
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
            else{
                return false;
            }
        }
    }
}

bool Profile::load_inventory(){
    //If there is no profile.
    if(player.name=="\x1F"){
        return false;
    }
    //If there is a profile.
    else{
        if(player.game_mode==GAME_MODE_SP_ADVENTURE){
            ifstream load;
            string file_to_load=get_home_directory()+"profiles/"+player.name+"/inventory.blazesave";
            load.open(file_to_load.c_str(),ifstream::in);

            if(load!=NULL){
                int inventory_size=0;
                player.inventory.clear();
                string name="";

                load>>player.leaves;
                load>>player.cheese;
                load>>player.leaves_max;
                load>>player.cheese_max;
                load>>player.ammo;
                load>>player.score;
                load>>player.main_levels_beaten;
                load>>player.weather_machine;
                load>>player.current_shot;

                load>>player.current_worldmap;
                player.current_level=player.current_worldmap;

                load>>player.new_game_plus;

                load>>player.option_character;

                load>>inventory_size;

                for(int i=0;i<inventory_size;i++){
                    player.inventory.push_back(inventory_item());
                    load>>player.inventory[i].type;
                    load>>player.inventory[i].slot;
                    load>>name;

                    while(name.rfind("SPACE")!=string::npos){
                        name.replace(name.rfind("SPACE"),5," ");
                    }
                    while(name.rfind("NEWLINE")!=string::npos){
                        name.replace(name.rfind("NEWLINE"),7,"\xA");
                    }

                    player.inventory[i].name=name;
                }

                load.close();
                load.clear();
            }
            else{
                if(!save_inventory()){
                    return false;
                }
            }
        }

        return true;
    }
}

bool Profile::save_inventory(){
    //If there is no profile.
    if(player.name=="\x1F"){
        return false;
    }
    //If there is a profile.
    else{
        if(player.game_mode==GAME_MODE_SP_ADVENTURE){
            make_directories();
            ofstream save;
            string save_name=get_home_directory()+"profiles/"+player.name+"/inventory.blazesave";
            save.open(save_name.c_str());

            string name="";
            window_inventory[0].drop_dragged_item();

            if(save!=NULL){
                save<<player.leaves;
                save<<"\n";
                save<<player.cheese;
                save<<"\n";
                save<<player.leaves_max;
                save<<"\n";
                save<<player.cheese_max;
                save<<"\n";
                save<<player.ammo;
                save<<"\n";
                save<<player.score;
                save<<"\n";
                save<<player.main_levels_beaten;
                save<<"\n";
                save<<player.weather_machine;
                save<<"\n";
                save<<player.current_shot;
                save<<"\n";
                save<<player.current_worldmap;
                save<<"\n";
                save<<player.new_game_plus;
                save<<"\n";
                save<<player.option_character;
                save<<"\n";
                save<<player.inventory.size();
                save<<"\n";

                for(int i=0;i<player.inventory.size();i++){
                    save<<player.inventory[i].type;
                    save<<" ";
                    save<<player.inventory[i].slot;
                    save<<" ";

                    name=player.inventory[i].name;

                    while(name.rfind(" ")!=string::npos){
                        name.replace(name.rfind(" "),1,"SPACE");
                    }
                    while(name.rfind("\xA")!=string::npos){
                        name.replace(name.rfind("\xA"),1,"NEWLINE");
                    }

                    save<<name;
                    save<<"\n";
                }

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

bool Profile::load_stats(){
    //If there is no profile.
    if(player.name=="\x1F"){
        return false;
    }
    //If there is a profile.
    else{
        if(player.game_mode==GAME_MODE_SP_ADVENTURE){
            ifstream load;
            string file_to_load=get_home_directory()+"profiles/"+player.name+"/stats.blazesave";
            load.open(file_to_load.c_str(),ifstream::in);

            if(load!=NULL){
                load>>player.stat_enemies_stunned;
                load>>player.stat_triggers_shot;
                load>>player.stat_checkpoints_activated;
                load>>player.stat_ammo_picked_up;
                load>>player.stat_shots_fired;
                load>>player.stat_deaths_enemies;
                load>>player.stat_deaths_traps;
                load>>player.stat_deaths_drowning;
                load>>player.stat_levels_replayed;
                load>>player.stat_total_jumps;
                load>>player.stat_seconds_playing;
                load>>player.stat_seconds_traveling;
                load>>player.stat_seconds_menus;
                load>>player.stat_levers_pulled;
                load>>player.stat_seconds_riding_cows;
                load>>player.stat_farthest_fall;
                load>>player.stat_water_shot;
                load>>player.stat_bosses_defeated;
                load>>player.stat_items_moved;

                load.close();
                load.clear();
            }
            else{
                if(!save_stats()){
                    return false;
                }
            }
        }

        return true;
    }
}

bool Profile::save_stats(){
    //If there is no profile.
    if(player.name=="\x1F"){
        return false;
    }
    //If there is a profile.
    else{
        if(player.game_mode==GAME_MODE_SP_ADVENTURE){
            make_directories();
            ofstream save;
            string save_name=get_home_directory()+"profiles/"+player.name+"/stats.blazesave";
            save.open(save_name.c_str());

            if(save!=NULL){
                save<<player.stat_enemies_stunned;
                save<<"\n";
                save<<player.stat_triggers_shot;
                save<<"\n";
                save<<player.stat_checkpoints_activated;
                save<<"\n";
                save<<player.stat_ammo_picked_up;
                save<<"\n";
                save<<player.stat_shots_fired;
                save<<"\n";
                save<<player.stat_deaths_enemies;
                save<<"\n";
                save<<player.stat_deaths_traps;
                save<<"\n";
                save<<player.stat_deaths_drowning;
                save<<"\n";
                save<<player.stat_levels_replayed;
                save<<"\n";
                save<<player.stat_total_jumps;
                save<<"\n";
                save<<player.stat_seconds_playing;
                save<<"\n";
                save<<player.stat_seconds_traveling;
                save<<"\n";
                save<<player.stat_seconds_menus;
                save<<"\n";
                save<<player.stat_levers_pulled;
                save<<"\n";
                save<<player.stat_seconds_riding_cows;
                save<<"\n";
                save<<player.stat_farthest_fall;
                save<<"\n";
                save<<player.stat_water_shot;
                save<<"\n";
                save<<player.stat_bosses_defeated;
                save<<"\n";
                save<<player.stat_items_moved;
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

bool Profile::load_achievements(){
    //If there is no profile.
    if(player.name=="\x1F"){
        return false;
    }
    //If there is a profile.
    else{
        if(player.game_mode==GAME_MODE_SP_ADVENTURE){
            ifstream load;
            string file_to_load=get_home_directory()+"profiles/"+player.name+"/achievements.blazesave";
            load.open(file_to_load.c_str(),ifstream::in);

            if(load!=NULL){
                for(int i=0;i<ACHIEVEMENT_END;i++){
                    load>>player.achievements[i];
                }

                load.close();
                load.clear();
            }
            else{
                if(!save_achievements()){
                    return false;
                }
            }
        }

        return true;
    }
}

bool Profile::save_achievements(){
    //If there is no profile.
    if(player.name=="\x1F"){
        return false;
    }
    //If there is a profile.
    else{
        if(player.game_mode==GAME_MODE_SP_ADVENTURE){
            make_directories();
            ofstream save;
            string save_name=get_home_directory()+"profiles/"+player.name+"/achievements.blazesave";
            save.open(save_name.c_str());

            if(save!=NULL){
                for(int i=0;i<ACHIEVEMENT_END;i++){
                    save<<player.achievements[i];
                    save<<"\n";
                }

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

bool Profile::load_boss_states(){
    //If there is no profile.
    if(player.name=="\x1F"){
        return false;
    }
    //If there is a profile.
    else{
        if(player.game_mode==GAME_MODE_SP_ADVENTURE){
            ifstream load;
            string file_to_load=get_home_directory()+"profiles/"+player.name+"/bosses.blazesave";
            load.open(file_to_load.c_str(),ifstream::in);

            if(load!=NULL){
                for(int i=0;i<=LAST_LEVEL;i++){
                    load>>player.bosses[i];
                }

                load.close();
                load.clear();
            }
            else{
                if(!save_boss_states()){
                    return false;
                }
            }
        }

        return true;
    }
}

bool Profile::save_boss_states(){
    //If there is no profile.
    if(player.name=="\x1F"){
        return false;
    }
    //If there is a profile.
    else{
        if(player.game_mode==GAME_MODE_SP_ADVENTURE){
            make_directories();
            ofstream save;
            string save_name=get_home_directory()+"profiles/"+player.name+"/bosses.blazesave";
            save.open(save_name.c_str());

            if(save!=NULL){
                for(int i=0;i<=LAST_LEVEL;i++){
                    save<<player.bosses[i];
                    save<<"\n";
                }

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

void Profile::reset_world_map_data(){
    //If there is a profile.
    if(player.name!="\x1F"){
        if(player.game_mode==GAME_MODE_SP_ADVENTURE){
            make_directories();

            for(int i=0;i<3;i++){
                string current_level="";
                ss.clear();ss.str("");ss<<i;current_level=ss.str();

                string world_map_save=get_home_directory()+"profiles/"+player.name+"/saves/"+current_level+"/world.blazesave";

                boost::filesystem::remove(world_map_save.c_str());
            }
        }
    }
}

void Profile::reset_ammo_barrels(){
    //If there is a profile.
    if(player.name!="\x1F"){
        if(player.game_mode==GAME_MODE_SP_ADVENTURE){
            make_directories();

            //Loop through all levels, skipping the first 3 since they are world maps.
            for(int n=3;n<=LAST_LEVEL;n++){
                string current_level="";
                ss.clear();ss.str("");ss<<n;current_level=ss.str();

                bool persistent_level_data=false;

                ifstream load;
                string level_to_load="";

                level_to_load="data/levels/"+current_level+"/level_properties.blazelevel";
                load.open(level_to_load.c_str(),ifstream::in);

                if(load!=NULL){
                    short number_of_background_layers=0;
                    double unneeded=0.0;

                    load >> unneeded;
                    load >> unneeded;

                    load >> unneeded;

                    load >> unneeded;
                    load >> unneeded;
                    load >> unneeded;

                    load >> number_of_background_layers;

                    for(int i=0;i<number_of_background_layers;i++){
                        load >> unneeded;
                        load >> unneeded;
                    }

                    load >> persistent_level_data;

                    load.close();
                    load.clear();
                }

                if(persistent_level_data){
                    vector<Item> temp_items;

                    level_to_load="data/levels/"+current_level+"/items.blazelevel";
                    load.open(level_to_load.c_str(),ifstream::in);

                    if(load!=NULL){
                        short type;
                        double x;
                        double y;
                        int goal_level_to_load;
                        bool goal_secret;

                        while(!load.eof()){
                            type=30000;
                            x=0;
                            y=0;
                            goal_level_to_load=0;
                            goal_secret=false;

                            load >> type;
                            load >> x;
                            load >> y;
                            load >> goal_level_to_load;
                            load >> goal_secret;

                            if(type!=30000){
                                temp_items.push_back(Item(x,y,false,type,goal_level_to_load,goal_secret));
                            }
                        }

                        load.close();
                        load.clear();
                    }

                    if(temp_items.size()>0){
                        level_to_load=get_home_directory()+"profiles/"+player.name+"/saves/"+current_level+"/items.blazesave";
                        load.open(level_to_load.c_str(),ifstream::in);

                        if(load!=NULL){
                            bool collected=false;

                            for(int i=0;i<temp_items.size();i++){
                                if(is_item_collectable(temp_items[i].type)){
                                    load>>collected;
                                    if(collected){
                                        temp_items[i].exists=false;
                                    }

                                    if(temp_items[i].type==ITEM_AMMO_BARREL){
                                        temp_items[i].exists=true;
                                    }
                                }
                            }

                            load.close();
                            load.clear();

                            ofstream save;
                            string save_name=get_home_directory()+"profiles/"+player.name+"/saves/"+current_level+"/items.blazesave";
                            save.open(save_name.c_str());

                            if(save!=NULL){
                                bool collected=false;

                                for(int i=0;i<temp_items.size();i++){
                                    if(is_item_collectable(temp_items[i].type)){
                                        if(!temp_items[i].exists){
                                            collected=true;
                                        }
                                        else{
                                            collected=false;
                                        }

                                        save<<collected;
                                        save<<" ";
                                    }
                                }

                                save.close();
                                save.clear();
                            }
                        }
                    }
                }
            }
        }
    }
}

void Profile::reset_weather_machine(){
    Level_Properties lp=load_level_properties_weather_machine(4);
    lp.current_sub_level=0;
    save_level_properties_weather_machine(4,lp);

    lp=load_level_properties_weather_machine(11);
    lp.current_sub_level=0;
    save_level_properties_weather_machine(11,lp);
}

void Profile::reset_level_beaten(){
    //If there is a profile.
    if(player.name!="\x1F"){
        if(player.game_mode==GAME_MODE_SP_ADVENTURE){
            make_directories();

            //Loop through all levels, skipping the first 3 since they are world maps.
            for(int n=3;n<=LAST_LEVEL;n++){
                string current_level="";
                ss.clear();ss.str("");ss<<n;current_level=ss.str();

                ifstream load;
                string level_to_load=get_home_directory()+"profiles/"+player.name+"/saves/"+current_level+"/level_properties.blazesave";
                load.open(level_to_load.c_str(),ifstream::in);

                if(load!=NULL){
                    short current_sub_level=0;
                    bool level_beaten=false;

                    load>>current_sub_level;

                    load>>level_beaten;

                    load.close();
                    load.clear();

                    level_beaten=false;

                    ofstream save;
                    string save_name=get_home_directory()+"profiles/"+player.name+"/saves/"+current_level+"/level_properties.blazesave";
                    save.open(save_name.c_str());

                    if(save!=NULL){
                        save<<current_sub_level;
                        save<<"\n";
                        save<<level_beaten;
                        save<<"\n";

                        save.close();
                        save.clear();
                    }
                }
            }
        }
    }
}

bool Profile::save_mp_players(){
    //If there is no profile.
    if(player.name=="\x1F"){
        return false;
    }
    //If there is a profile.
    else{
        make_directories();
        ofstream save;
        string save_name=get_home_directory()+"profiles/"+player.name+"/mp_players.blazesave";
        save.open(save_name.c_str());

        if(save!=NULL){
            save<<player.mp_player_count;
            save<<"\n";

            save<<player.mp_character;
            save<<"\n";

            save<<player.mp_available_characters.size();
            save<<"\n";
            for(int i=0;i<player.mp_available_characters.size();i++){
                save<<player.mp_available_characters[i];
                save<<"\n";
            }

            save<<player.mp_ai.size();
            save<<"\n";
            for(int i=0;i<player.mp_ai.size();i++){
                save<<player.mp_ai[i];
                save<<"\n";
            }

            save<<mp_players.size();
            save<<"\n";
            for(int i=0;i<mp_players.size();i++){
                save<<mp_players[i].which_mp_player;
                save<<"\n";

                save<<mp_players[i].option_character;
                save<<"\n";
            }

            save.close();
            save.clear();
        }
        else{
            return false;
        }

        return true;
    }
}

bool Profile::load_mp_players(){
    //If there is no profile.
    if(player.name=="\x1F"){
        return false;
    }
    //If there is a profile.
    else{
        ifstream load;
        string file_to_load=get_home_directory()+"profiles/"+player.name+"/mp_players.blazesave";
        load.open(file_to_load.c_str(),ifstream::in);

        if(load!=NULL){
            load>>player.mp_player_count;

            load>>player.mp_character;

            player.mp_available_characters.clear();
            int mp_available_characters_size=0;
            load>>mp_available_characters_size;
            for(int i=0;i<mp_available_characters_size;i++){
                short data=0;
                load>>data;

                player.mp_available_characters.push_back(data);
            }

            player.mp_ai.clear();
            int mp_ai_size=0;
            load>>mp_ai_size;
            for(int i=0;i<mp_ai_size;i++){
                bool data=false;
                load>>data;

                player.mp_ai.push_back(data);
            }

            mp_players.clear();
            int mp_players_size=0;
            load>>mp_players_size;
            for(int i=0;i<mp_players_size;i++){
                int which_mp_player=0;
                short option_character=0;

                load>>which_mp_player;
                load>>option_character;

                mp_players.push_back(Player_Mp(player.mp_keys[which_mp_player],which_mp_player,player.mp_ai[which_mp_player]));
                mp_players[mp_players.size()-1].option_character=option_character;
            }

            load.close();
            load.clear();
        }
        else{
            if(!save_mp_players()){
                return false;
            }
        }

        return true;
    }
}

bool Profile::save_shop(){
    //If there is no profile.
    if(player.name=="\x1F"){
        return false;
    }
    //If there is a profile.
    else{
        make_directories();
        ofstream save;
        string save_name=get_home_directory()+"profiles/"+player.name+"/shop.blazesave";
        save.open(save_name.c_str());

        if(save!=NULL){
            save<<player.shop_upgrades.size();
            save<<"\n";
            for(int i=0;i<player.shop_upgrades.size();i++){
                save<<player.shop_upgrades[i];
                save<<"\n";
            }

            save.close();
            save.clear();
        }
        else{
            return false;
        }

        return true;
    }
}

bool Profile::load_shop(){
    //If there is no profile.
    if(player.name=="\x1F"){
        return false;
    }
    //If there is a profile.
    else{
        ifstream load;
        string file_to_load=get_home_directory()+"profiles/"+player.name+"/shop.blazesave";
        load.open(file_to_load.c_str(),ifstream::in);

        if(load!=NULL){
            player.shop_upgrades.clear();
            int shop_upgrades_size=0;
            load>>shop_upgrades_size;
            for(int i=0;i<shop_upgrades_size;i++){
                string data="";
                load>>data;

                player.shop_upgrades.push_back(data);
            }

            load.close();
            load.clear();
        }
        else{
            if(!save_shop()){
                return false;
            }
        }

        return true;
    }
}

bool Profile::save_upgrades(){
    //If there is no profile.
    if(player.name=="\x1F"){
        return false;
    }
    //If there is a profile.
    else{
        make_directories();
        ofstream save;
        string save_name=get_home_directory()+"profiles/"+player.name+"/upgrades.blazesave";
        save.open(save_name.c_str());

        if(save!=NULL){
            save<<player.upgrades.size();
            save<<"\n";
            for(map<string,bool>::iterator it=player.upgrades.begin();it!=player.upgrades.end();it++){
                save<<it->first;
                save<<"\n";

                save<<it->second;
                save<<"\n";
            }

            save.close();
            save.clear();
        }
        else{
            return false;
        }

        return true;
    }
}

bool Profile::load_upgrades(){
    //If there is no profile.
    if(player.name=="\x1F"){
        return false;
    }
    //If there is a profile.
    else{
        ifstream load;
        string file_to_load=get_home_directory()+"profiles/"+player.name+"/upgrades.blazesave";
        load.open(file_to_load.c_str(),ifstream::in);

        if(load!=NULL){
            player.upgrades.clear();
            int upgrades_size=0;
            load>>upgrades_size;
            for(int i=0;i<upgrades_size;i++){
                string data="";
                load>>data;

                bool upgrade_state=false;
                load>>upgrade_state;

                player.upgrades.insert(make_pair(data,upgrade_state));
            }

            load.close();
            load.clear();
        }
        else{
            if(!save_upgrades()){
                return false;
            }
        }

        return true;
    }
}
