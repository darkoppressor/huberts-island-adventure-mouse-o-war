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
#include "file_io.h"

#include <fstream>
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

        #ifdef GAME_OS_ANDROID
            if(File_IO::external_storage_available()){
                const char* external_path=SDL_AndroidGetExternalStoragePath();

                if(external_path!=0){
                    str_home=external_path;
                    str_home+="/";
                }
                else{
                    msg="Error getting external storage path: ";
                    msg+=SDL_GetError();
                    update_error_log(msg);
                }
            }
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

            File_IO::create_directory(str_my_games);
        #endif

        string str_home=get_home_directory();

        //Remove the ending slash.
        str_home.erase(str_home.length()-1,1);

        correct_slashes(&str_home);

        File_IO::create_directory(str_home);
    }
}

void Profile::make_directories(){
    string temp="";

    make_home_directory();

    File_IO::create_directory(get_home_directory()+"profiles");
    File_IO::create_directory(get_home_directory()+"profiles/backups");

    //If there is a profile.
    if(player.name!="\x1F"){
        temp=get_home_directory()+"profiles/";
        temp+=player.name;
        File_IO::create_directory(temp);

        temp=get_home_directory()+"profiles/";
        temp+=player.name;
        temp+="/screenshots";
        File_IO::create_directory(temp);

        temp=get_home_directory()+"profiles/";
        temp+=player.name;
        temp+="/saves";
        File_IO::create_directory(temp);

        for(int i=0;i<=LAST_LEVEL;i++){
            temp=get_home_directory()+"profiles/";
            temp+=player.name;
            temp+="/saves/";
            ss.clear();ss.str("");ss<<i;temp+=ss.str();
            File_IO::create_directory(temp);
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
    File_IO::remove_directory(temp);

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

void Profile::create_profile(bool creating_default){
    if(creating_profile.length()>0 && !does_profile_exist(creating_profile)){
        version_mismatch=false;

        if(!creating_default){
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
        }

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

        if(!creating_default){
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
        else{
            creating_profile.clear();
        }
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

    File_IO_Load load;
    string file_to_load=get_home_directory()+"profiles/profile_list.cfg";
    load.open(file_to_load);

    if(load.is_opened()){
        istringstream data_stream(load.get_data());

        data_stream>>number_of_profiles;

        for(int i=0;i<number_of_profiles;i++){
            data_stream>>temp;

            profile_list.push_back(string(temp));
        }

        load.close();
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

            File_IO::remove_directory(profile_backup);

            File_IO::create_directory(profile_backup);

            for(File_IO_Directory_Iterator it(profile_directory);it.evaluate();it.iterate()){
                //If the file is not a directory
                if(it.is_regular_file()){
                    string path_name=it.get_full_path();

                    string destination_path=path_name;

                    boost::algorithm::replace_first(destination_path,profile_directory,profile_backup);

                    File_IO::copy_file(path_name,destination_path);
                }
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

        File_IO_Load load;
        string level_to_load=get_home_directory()+"profiles/"+player.name+"/saves/"+current_level+"/map.blazesave";
        load.open(level_to_load);

        if(load.is_opened()){
            istringstream data_stream(load.get_data());

            bool seen=false;

            for(int int_y=0;int_y<level.level_y/TILE_SIZE;int_y++){
                for(int int_x=0;int_x<level.level_x/TILE_SIZE;int_x++){
                    data_stream >> seen;
                    current_level_map_data[int_x][int_y]=seen;
                }
            }

            load.close();
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

            File_IO_Load load;
            string level_to_load=get_home_directory()+"profiles/"+player.name+"/saves/"+current_level+"/level_properties.blazesave";
            load.open(level_to_load);

            if(load.is_opened()){
                istringstream data_stream(load.get_data());

                data_stream>>player.current_sub_level;

                data_stream>>player.level_beaten;

                load.close();
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

                File_IO_Load load;
                string level_to_load="";

                //If the current level is not the world map.
                if(!player.on_worldmap()){
                    level_to_load=get_home_directory()+"profiles/"+player.name+"/saves/"+current_level+"/items.blazesave";
                    load.open(level_to_load);

                    if(load.is_opened()){
                        istringstream data_stream(load.get_data());

                        bool collected=false;

                        for(int i=0;i<vector_items.size();i++){
                            //If the item is of a collectable type.
                            if(is_item_collectable(vector_items[i].type)){
                                data_stream>>collected;
                                if(collected){
                                    vector_items[i].exists=false;
                                    if(vector_items[i].type==ITEM_LEAF){
                                        player.current_level_leaves++;
                                    }
                                    else if(vector_items[i].type==ITEM_CHEESE){
                                        player.current_level_cheese++;
                                    }

                                    int candy_width=vector_items[i].w/ITEM_W;
                                    int candy_height=vector_items[i].h/ITEM_H;

                                    for(int x=0;x<candy_width;x++){
                                        for(int y=0;y<candy_height;y++){
                                            vector_items.push_back(Item(vector_items[i].x+x*ITEM_W,vector_items[i].y+y*ITEM_H,false,ITEM_CANDY,0,false));
                                        }
                                    }
                                }
                            }
                        }

                        load.close();
                    }
                    else{
                        if(!save_level_data()){
                            return false;
                        }
                    }

                    level_to_load=get_home_directory()+"profiles/"+player.name+"/saves/"+current_level+"/doors.blazesave";
                    load.open(level_to_load);

                    if(load.is_opened()){
                        istringstream data_stream(load.get_data());

                        bool opened=false;

                        for(int i=0;i<vector_doors.size();i++){
                            if(vector_doors[i].type==DOOR_TYPE_KEY){
                                data_stream>>opened;
                                if(opened){
                                    vector_doors[i].open=true;
                                    vector_doors[i].frame=KEY_DOOR_SPRITES-1;
                                }
                            }
                        }

                        load.close();
                    }
                    else{
                        return false;
                    }

                    load_level_stats();
                }
                //If the current level is the world map.
                else{
                    level_to_load=get_home_directory()+"profiles/"+player.name+"/saves/"+current_level+"/world.blazesave";
                    load.open(level_to_load);

                    if(load.is_opened()){
                        istringstream data_stream(load.get_data());

                        data_stream>>player.world_x[player.current_level];
                        data_stream>>player.world_y[player.current_level];
                        player.x=player.world_x[player.current_level];
                        player.y=player.world_y[player.current_level];

                        data_stream>>player.facing;

                        load.close();
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
            File_IO_Load load;
            string file_to_load=get_home_directory()+"profiles/"+player.name+"/inventory.blazesave";
            load.open(file_to_load);

            if(load.is_opened()){
                istringstream data_stream(load.get_data());

                int inventory_size=0;
                player.inventory.clear();
                string name="";

                data_stream>>player.leaves;
                data_stream>>player.cheese;
                data_stream>>player.leaves_max;
                data_stream>>player.cheese_max;
                data_stream>>player.ammo;
                data_stream>>player.score;
                data_stream>>player.main_levels_beaten;
                data_stream>>player.weather_machine;
                data_stream>>player.current_shot;

                data_stream>>player.current_worldmap;
                player.current_level=player.current_worldmap;

                data_stream>>player.new_game_plus;

                data_stream>>player.option_character;

                data_stream>>inventory_size;

                for(int i=0;i<inventory_size;i++){
                    player.inventory.push_back(inventory_item());
                    data_stream>>player.inventory[i].type;
                    data_stream>>player.inventory[i].slot;
                    data_stream>>name;

                    while(name.rfind("SPACE")!=string::npos){
                        name.replace(name.rfind("SPACE"),5," ");
                    }
                    while(name.rfind("NEWLINE")!=string::npos){
                        name.replace(name.rfind("NEWLINE"),7,"\xA");
                    }

                    player.inventory[i].name=name;
                }

                load.close();
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
            File_IO_Load load;
            string file_to_load=get_home_directory()+"profiles/"+player.name+"/stats.blazesave";
            load.open(file_to_load);

            if(load.is_opened()){
                istringstream data_stream(load.get_data());

                data_stream>>player.stat_enemies_stunned;
                data_stream>>player.stat_triggers_shot;
                data_stream>>player.stat_checkpoints_activated;
                data_stream>>player.stat_ammo_picked_up;
                data_stream>>player.stat_shots_fired;
                data_stream>>player.stat_deaths_enemies;
                data_stream>>player.stat_deaths_traps;
                data_stream>>player.stat_deaths_drowning;
                data_stream>>player.stat_levels_replayed;
                data_stream>>player.stat_total_jumps;
                data_stream>>player.stat_seconds_playing;
                data_stream>>player.stat_seconds_traveling;
                data_stream>>player.stat_seconds_menus;
                data_stream>>player.stat_levers_pulled;
                data_stream>>player.stat_seconds_riding_cows;
                data_stream>>player.stat_farthest_fall;
                data_stream>>player.stat_water_shot;
                data_stream>>player.stat_bosses_defeated;
                data_stream>>player.stat_items_moved;

                load.close();
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
            File_IO_Load load;
            string file_to_load=get_home_directory()+"profiles/"+player.name+"/achievements.blazesave";
            load.open(file_to_load);

            if(load.is_opened()){
                istringstream data_stream(load.get_data());

                for(int i=0;i<ACHIEVEMENT_END;i++){
                    data_stream>>player.achievements[i];
                }

                load.close();
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
            File_IO_Load load;
            string file_to_load=get_home_directory()+"profiles/"+player.name+"/bosses.blazesave";
            load.open(file_to_load);

            if(load.is_opened()){
                istringstream data_stream(load.get_data());

                for(int i=0;i<=LAST_LEVEL;i++){
                    data_stream>>player.bosses[i];
                }

                load.close();
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

                File_IO::remove_file(world_map_save);
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

                File_IO_Load load;
                string level_to_load="";

                level_to_load="data/levels/"+current_level+"/level_properties.blazelevel";
                load.open(level_to_load);

                if(load.is_opened()){
                    istringstream data_stream(load.get_data());

                    short number_of_background_layers=0;
                    double unneeded=0.0;

                    data_stream >> unneeded;
                    data_stream >> unneeded;

                    data_stream >> unneeded;

                    data_stream >> unneeded;
                    data_stream >> unneeded;
                    data_stream >> unneeded;

                    data_stream >> number_of_background_layers;

                    for(int i=0;i<number_of_background_layers;i++){
                        data_stream >> unneeded;
                        data_stream >> unneeded;
                    }

                    data_stream >> persistent_level_data;

                    load.close();
                }

                if(persistent_level_data){
                    vector<Item> temp_items;

                    level_to_load="data/levels/"+current_level+"/items.blazelevel";
                    load.open(level_to_load);

                    if(load.is_opened()){
                        istringstream data_stream(load.get_data());

                        short type;
                        double x;
                        double y;
                        int goal_level_to_load;
                        bool goal_secret;

                        while(!data_stream.eof()){
                            type=30000;
                            x=0;
                            y=0;
                            goal_level_to_load=0;
                            goal_secret=false;

                            data_stream >> type;
                            data_stream >> x;
                            data_stream >> y;
                            data_stream >> goal_level_to_load;
                            data_stream >> goal_secret;

                            if(type!=30000){
                                temp_items.push_back(Item(x,y,false,type,goal_level_to_load,goal_secret));
                            }
                        }

                        load.close();
                    }

                    if(temp_items.size()>0){
                        level_to_load=get_home_directory()+"profiles/"+player.name+"/saves/"+current_level+"/items.blazesave";
                        load.open(level_to_load);

                        if(load.is_opened()){
                            istringstream data_stream(load.get_data());

                            bool collected=false;

                            for(int i=0;i<temp_items.size();i++){
                                if(is_item_collectable(temp_items[i].type)){
                                    data_stream>>collected;
                                    if(collected){
                                        temp_items[i].exists=false;
                                    }

                                    if(temp_items[i].type==ITEM_AMMO_BARREL){
                                        temp_items[i].exists=true;
                                    }
                                }
                            }

                            load.close();

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

                File_IO_Load load;
                string level_to_load=get_home_directory()+"profiles/"+player.name+"/saves/"+current_level+"/level_properties.blazesave";
                load.open(level_to_load);

                if(load.is_opened()){
                    istringstream data_stream(load.get_data());

                    short current_sub_level=0;
                    bool level_beaten=false;

                    data_stream>>current_sub_level;

                    data_stream>>level_beaten;

                    load.close();

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
        File_IO_Load load;
        string file_to_load=get_home_directory()+"profiles/"+player.name+"/mp_players.blazesave";
        load.open(file_to_load);

        if(load.is_opened()){
            istringstream data_stream(load.get_data());

            data_stream>>player.mp_player_count;

            data_stream>>player.mp_character;

            player.mp_available_characters.clear();
            int mp_available_characters_size=0;
            data_stream>>mp_available_characters_size;
            for(int i=0;i<mp_available_characters_size;i++){
                short data=0;
                data_stream>>data;

                player.mp_available_characters.push_back(data);
            }

            player.mp_ai.clear();
            int mp_ai_size=0;
            data_stream>>mp_ai_size;
            for(int i=0;i<mp_ai_size;i++){
                bool data=false;
                data_stream>>data;

                player.mp_ai.push_back(data);
            }

            mp_players.clear();
            int mp_players_size=0;
            data_stream>>mp_players_size;
            for(int i=0;i<mp_players_size;i++){
                int which_mp_player=0;
                short option_character=0;

                data_stream>>which_mp_player;
                data_stream>>option_character;

                mp_players.push_back(Player_Mp(player.mp_keys[which_mp_player],which_mp_player,player.mp_ai[which_mp_player]));
                mp_players[mp_players.size()-1].option_character=option_character;
            }

            load.close();
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
        File_IO_Load load;
        string file_to_load=get_home_directory()+"profiles/"+player.name+"/shop.blazesave";
        load.open(file_to_load);

        if(load.is_opened()){
            istringstream data_stream(load.get_data());

            player.shop_upgrades.clear();
            int shop_upgrades_size=0;
            data_stream>>shop_upgrades_size;
            for(int i=0;i<shop_upgrades_size;i++){
                string data="";
                data_stream>>data;

                player.shop_upgrades.push_back(data);
            }

            load.close();
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
        File_IO_Load load;
        string file_to_load=get_home_directory()+"profiles/"+player.name+"/upgrades.blazesave";
        load.open(file_to_load);

        if(load.is_opened()){
            istringstream data_stream(load.get_data());

            player.upgrades.clear();
            int upgrades_size=0;
            data_stream>>upgrades_size;
            for(int i=0;i<upgrades_size;i++){
                string data="";
                data_stream>>data;

                bool upgrade_state=false;
                data_stream>>upgrade_state;

                player.upgrades.insert(make_pair(data,upgrade_state));
            }

            load.close();
        }
        else{
            if(!save_upgrades()){
                return false;
            }
        }

        return true;
    }
}
