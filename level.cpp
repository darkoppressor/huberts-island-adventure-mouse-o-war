/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "level.h"
#include "world.h"
#include "render.h"
#include "update.h"
#include "holidays.h"
#include "message_log.h"
#include "distance.h"
#include "file_io.h"

#include <fstream>

#include <boost/algorithm/string.hpp>

using namespace std;

Level::Level(){
    level_x=0;
    level_y=0;

    leaves=0;
    cheese=0;

    alarm=false;

    amount_replicators=0;

    title_top_x=0.0;
    title_bottom_x=0.0;
    title_bg_y_top=0.0;
    title_bg_x_bottom=0.0;
    title_bg_x_left=0.0;
    title_top_string="";
    title_bottom_string="";
    title_moving=TITLE_NOT_MOVING;
    counter_title=0;

    seconds_in_current_level=0;

    survival_seconds_played=0;

    survival_never_spawned_items=true;
    survival_never_spawned_npcs=true;
}

short Level::return_level_region(int i){
    if(fabs(i)==3){
        return WORLDMAP_REGION_VALLEY;
    }
    else if(fabs(i)==4){
        return WORLDMAP_REGION_VALLEY;
    }
    else if(fabs(i)==5){
        return WORLDMAP_REGION_CASTLE;
    }
    else if(fabs(i)==6){
        return WORLDMAP_REGION_HAUNTED;
    }
    else if(fabs(i)==7){
        return WORLDMAP_REGION_HAUNTED;
    }
    else if(fabs(i)==8){
        return WORLDMAP_REGION_MOUNTAIN;
    }
    else if(fabs(i)==9){
        return WORLDMAP_REGION_MOUNTAIN_PEAK;
    }
    else if(fabs(i)==10){
        return WORLDMAP_REGION_VOLCANO;
    }
    else if(fabs(i)==11){
        return WORLDMAP_REGION_DESERT;
    }
    else if(fabs(i)==12){
        return WORLDMAP_REGION_DESERT;
    }
    else if(fabs(i)==13){
        return WORLDMAP_REGION_OCEAN;
    }
    else if(fabs(i)==14){
        return WORLDMAP_REGION_LIGHTHOUSE;
    }
    else{
        return WORLDMAP_REGION_NORMAL;
    }
}

bool Level::return_level_snow(int i){
    if(fabs(i)==0 && player.option_holiday_cheer && holiday==HOLIDAY_CHRISTMAS){
        return true;
    }
    else if(fabs(i)==1 && player.option_holiday_cheer && holiday==HOLIDAY_CHRISTMAS){
        return true;
    }
    else if(fabs(i)==2 && player.option_holiday_cheer && holiday==HOLIDAY_CHRISTMAS){
        return true;
    }
    else if(fabs(i)==4 && player.weather_machine){
        return true;
    }
    else if(fabs(i)==9){
        return true;
    }
    else{
        return false;
    }
}

bool Level::return_level_rain(int i){
    if(fabs(i)==11 && player.weather_machine){
        return true;
    }
    else{
        return false;
    }
}

string Level::return_level_name(int i){
    if(fabs(i)==0){
        return "Travel back to the Island";
    }
    else if(fabs(i)==1){
        return "Travel to Top Secret Area";
    }
    else if(fabs(i)==2){
        return "Travel to DevWorld";
    }
    else if(fabs(i)==3){
        return "Vibrant Valley";
    }
    else if(fabs(i)==4){
        return "Lively Lowlands";
    }
    else if(fabs(i)==5){
        return "Comely Keep";
    }
    else if(fabs(i)==6){
        return "Accursed Acreage";
    }
    else if(fabs(i)==7){
        return "Haunted Halls";
    }
    else if(fabs(i)==8){
        return "Mammoth Mountain";
    }
    else if(fabs(i)==9){
        return "Perilous Peak";
    }
    else if(fabs(i)==10){
        return "Vicious Volcano";
    }
    else if(fabs(i)==11){
        return "Desolate Desert";
    }
    else if(fabs(i)==12){
        return "Treacherous Tomb";
    }
    else if(fabs(i)==13){
        return "Forbidden Fortress";
    }
    else if(fabs(i)==14){
        return "Luminous Lighthouse";
    }
    else if(fabs(i)==15){
        return "Hubert's House";
    }
    else if(fabs(i)==16){
        return "Unused 1";
    }
    else if(fabs(i)==17){
        return "Unused 2";
    }
    else if(fabs(i)==18){
        return "Unused 3";
    }
    else if(fabs(i)==19){
        return "Unused 4";
    }
    else if(fabs(i)==20){
        return "Unused 5";
    }
    else if(fabs(i)==21){
        return "Dinosaur Dale";
    }
    else if(fabs(i)==22){
        return "Timeworn Retreat";
    }
    else if(fabs(i)==23){
        return "Green Gradient";
    }
    else if(fabs(i)==24){
        return "Alpha Zone";
    }
    else if(fabs(i)==25){
        return "Showcase Room";
    }
    else if(fabs(i)==26){
        return "Desert Walk";
    }
    else if(fabs(i)==27){
        return "Playground";
    }

    string not_level="";
    ss.clear();ss.str("");ss<<i;ss<<" is not a level!";not_level=ss.str();
    return not_level;
}

void Level::set_title_strings(int i){
    if(fabs(i)==0){
        title_top_string="The";
        title_bottom_string="Island";
    }
    else if(fabs(i)==1){
        title_top_string="Top Secret";
        title_bottom_string="Area";
    }
    else if(fabs(i)==2){
        title_top_string="Dev";
        title_bottom_string="World";
    }
    else if(fabs(i)==3){
        title_top_string="Vibrant";
        title_bottom_string="Valley";
    }
    else if(fabs(i)==4){
        title_top_string="Lively";
        title_bottom_string="Lowlands";
    }
    else if(fabs(i)==5){
        title_top_string="Comely";
        title_bottom_string="Keep";
    }
    else if(fabs(i)==6){
        title_top_string="Accursed";
        title_bottom_string="Acreage";
    }
    else if(fabs(i)==7){
        title_top_string="Haunted";
        title_bottom_string="Halls";
    }
    else if(fabs(i)==8){
        title_top_string="Mammoth";
        title_bottom_string="Mountain";
    }
    else if(fabs(i)==9){
        title_top_string="Perilous";
        title_bottom_string="Peak";
    }
    else if(fabs(i)==10){
        title_top_string="Vicious";
        title_bottom_string="Volcano";
    }
    else if(fabs(i)==11){
        title_top_string="Desolate";
        title_bottom_string="Desert";
    }
    else if(fabs(i)==12){
        title_top_string="Treacherous";
        title_bottom_string="Tomb";
    }
    else if(fabs(i)==13){
        title_top_string="Forbidden";
        title_bottom_string="Fortress";
    }
    else if(fabs(i)==14){
        title_top_string="Luminous";
        title_bottom_string="Lighthouse";
    }
    else if(fabs(i)==15){
        title_top_string="Hubert's";
        title_bottom_string="House";
    }
    else if(fabs(i)==16){
        title_top_string="Unused";
        title_bottom_string="1";
    }
    else if(fabs(i)==17){
        title_top_string="Unused";
        title_bottom_string="2";
    }
    else if(fabs(i)==18){
        title_top_string="Unused";
        title_bottom_string="3";
    }
    else if(fabs(i)==19){
        title_top_string="Unused";
        title_bottom_string="4";
    }
    else if(fabs(i)==20){
        title_top_string="Unused";
        title_bottom_string="5";
    }
    else if(fabs(i)==21){
        title_top_string="Dinosaur";
        title_bottom_string="Dale";
    }
    else if(fabs(i)==22){
        title_top_string="Timeworn";
        title_bottom_string="Retreat";
    }
    else if(fabs(i)==23){
        title_top_string="Green";
        title_bottom_string="Gradient";
    }
    else if(fabs(i)==24){
        title_top_string="Alpha";
        title_bottom_string="Zone";
    }
    else if(fabs(i)==25){
        title_top_string="Showcase";
        title_bottom_string="Room";
    }
    else if(fabs(i)==26){
        title_top_string="Desert";
        title_bottom_string="Walk";
    }
    else if(fabs(i)==27){
        title_top_string="Playground";
        title_bottom_string="";
    }
    else{
        title_top_string="Level has";
        title_bottom_string="no name!";
    }
}

bool Level::return_title_pause(){
    if(level.title_moving==TITLE_BG_MOVING_IN || level.title_moving==TITLE_MOVING_IN || level.title_moving==TITLE_WAITING){
        return true;
    }
    else{
        return false;
    }
}

void Level::process_title(){
    double SPEED_BG_IN_Y=25.0;
    double SPEED_IN=20.0;

    double SPEED_BG_IN_X=SPEED_BG_IN_Y*1.3;
    double SPEED_BG_OUT_X=SPEED_BG_IN_X*2;
    double SPEED_BG_OUT_Y=SPEED_BG_IN_Y*2;
    double SPEED_OUT=SPEED_IN*2;

    if(title_moving==TITLE_BG_MOVING_IN){
        title_bg_y_top+=SPEED_BG_IN_Y;
        title_bg_x_bottom-=SPEED_BG_IN_X;
        title_bg_x_left+=SPEED_BG_IN_X;

        if(title_bg_y_top>=0.0){
            title_bg_y_top=0.0;
        }
        if(title_bg_x_bottom<=0.0){
            title_bg_x_bottom=0.0;
        }
        if(title_bg_x_left>=0.0){
            title_bg_x_left=0.0;
            title_moving=TITLE_MOVING_IN;
        }
    }
    else if(title_moving==TITLE_MOVING_IN){
        title_top_x-=SPEED_IN;
        title_bottom_x+=SPEED_IN;

        bool top_done=false;
        bool bottom_done=false;

        if(title_top_x<=(main_window.SCREEN_WIDTH-title_top_string.length()*font_large.spacing_x*2.0)/2.0){
            title_top_x=(main_window.SCREEN_WIDTH-title_top_string.length()*font_large.spacing_x*2.0)/2.0;
            top_done=true;
        }
        if(title_bottom_x>=(main_window.SCREEN_WIDTH-title_bottom_string.length()*font_large.spacing_x*2.0)/2.0){
            title_bottom_x=(main_window.SCREEN_WIDTH-title_bottom_string.length()*font_large.spacing_x*2.0)/2.0;
            bottom_done=true;
        }

        if(top_done && bottom_done){
            title_moving=TITLE_WAITING;
            counter_title=2*UPDATE_RATE;
        }
    }
    else if(title_moving==TITLE_WAITING){
        if(counter_title>0){
            counter_title--;
            if(counter_title==0){
                title_moving=TITLE_BG_MOVING_OUT;

                //Unpause the game.
                player.toggle_pause(false);
            }
        }
    }
    else if(title_moving==TITLE_BG_MOVING_OUT){
        title_bg_y_top-=SPEED_BG_OUT_Y;
        title_bg_x_bottom+=SPEED_BG_OUT_X;
        title_bg_x_left-=SPEED_BG_OUT_X;

        if(title_bg_y_top+main_window.SCREEN_HEIGHT<0 && title_bg_x_bottom>main_window.SCREEN_WIDTH && title_bg_x_left+main_window.SCREEN_WIDTH<0){
            title_moving=TITLE_WAIT_BEFORE_MOVE_OUT;
            counter_title=1*UPDATE_RATE;
        }
    }
    else if(title_moving==TITLE_WAIT_BEFORE_MOVE_OUT){
        if(counter_title>0){
            counter_title--;
            if(counter_title==0){
                title_moving=TITLE_MOVING_OUT;
            }
        }
    }
    else if(title_moving==TITLE_MOVING_OUT){
        title_top_x+=SPEED_OUT;
        title_bottom_x-=SPEED_OUT;

        if(title_top_x>main_window.SCREEN_WIDTH && title_bottom_x+title_bottom_string.length()*font_large.spacing_x*2.0<0){
            title_moving=TITLE_NOT_MOVING;
        }
    }
}

void Level::render_title(){
    if(title_moving!=TITLE_NOT_MOVING){
        if(title_moving==TITLE_BG_MOVING_IN || title_moving==TITLE_MOVING_IN){
            render_rectangle(0,0,main_window.SCREEN_WIDTH,main_window.SCREEN_HEIGHT,1.0,COLOR_BLACK);
        }

        render_texture(0,title_bg_y_top,image.level_title_background_0);
        render_texture(title_bg_x_bottom,0,image.level_title_background_1);
        render_texture(title_bg_x_left,0,image.level_title_background_2);

        font_large.show(title_top_x+4,(main_window.SCREEN_HEIGHT-font_large.spacing_y*2.0)/2.0-font_large.spacing_y*2.0+4,title_top_string,COLOR_BLACK,1.0,2.0);
        font_large.show(title_top_x,(main_window.SCREEN_HEIGHT-font_large.spacing_y*2.0)/2.0-font_large.spacing_y*2.0,title_top_string,return_gui_color(holiday,3),1.0,2.0);

        font_large.show(title_bottom_x+4,(main_window.SCREEN_HEIGHT-font_large.spacing_y*2.0)/2.0+font_large.spacing_y*2.0+4,title_bottom_string,COLOR_BLACK,1.0,2.0);
        font_large.show(title_bottom_x,(main_window.SCREEN_HEIGHT-font_large.spacing_y*2.0)/2.0+font_large.spacing_y*2.0,title_bottom_string,return_gui_color(holiday,3),1.0,2.0);
    }
}

void Level::explode_party_balls(short item_type){
    for(int int_x=0;int_x<level_x/TILE_SIZE;int_x++){
        for(int int_y=0;int_y<level_y/TILE_SIZE;int_y++){
            if(tile_array[int_x][int_y].special==TILE_SPECIAL_ITEM_HOLDER){
                tile_array[int_x][int_y].special=TILE_SPECIAL_NONE;
                tile_array[int_x][int_y].solidity=TILE_SOLIDITY_PASSABLE;
                tile_array[int_x][int_y].sprite=0;
                tile_array[int_x][int_y].foreground=false;

                double ball_x=tile_array[int_x][int_y].x+TILE_SIZE/2.0;
                double ball_y=tile_array[int_x][int_y].y+TILE_SIZE/2.0;

                int random=random_range(30,60);
                for(int i=0;i<random;i++){
                    vector_items.push_back(Item(ball_x,ball_y,true,item_type,0,false,1,100,1,175,false,15));
                }
                play_positional_sound(sound_system.party_ball_explode,ball_x,ball_y,256);
            }
        }
    }
}

void Level::unload_level(){
    //Unload the current level's images.
    image.unload_images_level(player.current_level);

    music.unload_track(MUSIC_BOSS);
    music.unload_track(MUSIC_BOSS_FAST);
    music.unload_track(MUSIC_LEVEL_NORMAL);
    music.unload_track(MUSIC_LEVEL_WATER);

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
}

void Level::load_level(){
    if(player.game_mode==GAME_MODE_SP_ADVENTURE){
        load_level_adventure();
    }
    else if(player.game_mode==GAME_MODE_MP_ADVENTURE){
        load_level_adventure();
    }
    else if(player.game_mode==GAME_MODE_SP_SURVIVAL){
        player.swimming_gear=true;

        load_level_survival();
    }
    else if(player.game_mode==GAME_MODE_MP_SURVIVAL){
        player.swimming_gear=true;

        for(int i=0;i<mp_players.size();i++){
            mp_players[i].swimming_gear=true;
        }

        load_level_survival();
    }
}

void Level::load_level_adventure(){
    //Pause the game.
    player.toggle_pause(true);

    if(player.on_worldmap()){
        player.light_source.dimness=0.0;
        player.light_source.falloff=0.0925/(TILE_SIZE/LIGHTING_TILE_SIZE);
    }
    else{
        player.light_source.dimness=0.0;
        player.light_source.falloff=0.035/(TILE_SIZE/LIGHTING_TILE_SIZE);

        for(int i=0;i<mp_players.size();i++){
            mp_players[i].light_source.dimness=0.0;
            mp_players[i].light_source.falloff=0.035/(TILE_SIZE/LIGHTING_TILE_SIZE);
        }
    }

    devworld_tiles.clear_tiles();

    alarm=false;

    //The npc vector index of this level's boss, if any.
    int boss_index=-1;

    int things_loaded=0;
    int things_to_load=3;

    //Load the level properties save file.
    profile.load_level_properties();

    if(player.game_mode==GAME_MODE_MP_ADVENTURE){
        player.mp_load_level_properties();
    }

    //As long as a level has been loaded before
    //and the last level was not the same as this one.
    if(player.previous_level!=-1 && (player.previous_level!=player.current_level || player.previous_sub_level!=player.current_sub_level)){
        player.tracer_manager.clear_tracers();

        //Unload the previous level's images.
        image.unload_images_level(player.previous_level);

        //Update achievement counts.
        //I do this here so any special achievement counts (like how many shots were fired in the level)
        //can be checked. Then, I reset said counts.

        //If a level has been played (so previous_level!=-1), and the last level
        //was not the world map.
        if(!player.is_level_worldmap(player.previous_level)){
            if(player.special_count_shots_this_level==0){
                profile.update_achievements(ACHIEVEMENT_BEAT_LEVEL_NO_SHOTS);
            }
            if(player.special_count_kills_this_level==0){
                profile.update_achievements(ACHIEVEMENT_BEAT_LEVEL_NO_KILLS);
            }
        }

        //Reset the special achievement counts.
        player.special_count_shots_this_level=0;
        player.special_count_kills_this_level=0;

        //Save a profile backup.
        profile.save_backup();
    }

    //Setup the level and sub level strings.
    string current_level="";
    ss.clear();ss.str("");ss<<player.current_level;current_level=ss.str();

    string current_sub_level="";
    ss.clear();ss.str("");ss<<player.current_sub_level;current_sub_level=ss.str();

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
    File_IO_Load load;

    string level_to_load="";

    //If the current sub level is 0, meaning "load the main level."
    if(player.current_sub_level==0){
        level_to_load="data/levels/"+current_level+"/level_properties.blazelevel";
    }
    //If there is a current sub level to load instead of the main one.
    else{
        level_to_load="data/levels/"+current_level+"/"+current_sub_level+"/level_properties.blazelevel";
    }
    load.open(level_to_load);

    //As long as the file opened successfully, proceed to load the level data.
    if(load.is_opened()){
        istringstream data_stream(load.get_data());

        //Create any variables that will be used to pass on information from the level file.
        double start_y=0;
        double width=0;
        double height=0;
        short number_of_background_layers=0;
        double parallax_speed_x=0;
        double parallax_speed_y=0;

        //The first two numbers in the level file are the x and y dimensions of the entire level's map.
        data_stream >> level_x;
        data_stream >> level_y;

        //The next number is the direction the player should be facing.
        data_stream >> player.facing;

        data_stream >> start_y;
        data_stream >> width;
        data_stream >> height;

        //Load in the number of background layers.
        data_stream >> number_of_background_layers;

        for(int i=0;i<number_of_background_layers;i++){
            data_stream >> parallax_speed_x;
            data_stream >> parallax_speed_y;
            background_layers.push_back(Background_Layer(start_y,width,height,parallax_speed_x,parallax_speed_y));
        }

        data_stream >> player.persistent_level_data;

        data_stream >> player.night_mode;

        data_stream >> player.deadly_water;

        if(player.game_mode_is_multiplayer()){
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

            if(player.game_mode!=GAME_MODE_MP_ADVENTURE){
                player.persistent_level_data=false;
            }
        }

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
    }
    //If the level file could not be loaded for any reason, we print an error message letting the user know.
    else{
        update_error_log("Error loading level properties.");
    }

    //***************************************//
    // Next, we will read the terrain tiles: //
    //***************************************//

    //First, we open the file for reading.

    //If the current sub level is 0, meaning "load the main level."
    if(player.current_sub_level==0){
        level_to_load="data/levels/"+current_level+"/tiles.blazelevel";
    }
    //If there is a current sub level to load instead of the main one.
    else{
        level_to_load="data/levels/"+current_level+"/"+current_sub_level+"/tiles.blazelevel";
    }
    load.open(level_to_load);

    //As long as the file opened successfully, proceed to load the level data.
    if(load.is_opened()){
        istringstream data_stream(load.get_data());

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
                data_stream >> solidity;
                data_stream >> sprite;
                data_stream >> special;
                data_stream >> foreground;
                data_stream >> slope;

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
                        if(player.current_level==10){
                            tile_array[int_x][int_y].light_source.on=true;
                            tile_array[int_x][int_y].light_source.color=color_name_to_doubles(COLOR_RED);
                        }
                        else{
                            tile_array[int_x][int_y].light_source.on=true;
                            tile_array[int_x][int_y].light_source.color=color_name_to_doubles(COLOR_GREEN);
                        }
                    }
                }
            }
        }

        //When we are done using the level file, we close it, and clear load for its next use.
        load.close();
    }
    //If the level file could not be loaded for any reason, we print an error message letting the user know.
    else{
        update_error_log("Error loading terrain tiles.");
    }

    //******************************************//
    // Next, we will read the background tiles: //
    //******************************************//

    //We only read the background tiles if the level is not the world map.
    if(!player.on_worldmap()){
        //First, we open the file for reading.

        //If the current sub level is 0, meaning "load the main level."
        if(player.current_sub_level==0){
            level_to_load="data/levels/"+current_level+"/tiles_background.blazelevel";
        }
        //If there is a current sub level to load instead of the main one.
        else{
            level_to_load="data/levels/"+current_level+"/"+current_sub_level+"/tiles_background.blazelevel";
        }
        load.open(level_to_load);

        //As long as the file opened successfully, proceed to load the level data.
        if(load.is_opened()){
            istringstream data_stream(load.get_data());

            tile_background_array.clear();

            tile_background_array.resize(level_x/TILE_SIZE,vector<Tile_Background>(level_y/TILE_SIZE));

            //Now we make a temporary variable for getting the sprite of each tile being set.
            short sprite=0;

            //Then we iterate through all of the tiles in the file, setting each one.
            for(int int_y=0;int_y<(level_y/TILE_SIZE);int_y++){
                for(int int_x=0;int_x<(level_x/TILE_SIZE);int_x++){
                    //For each tile, load the type information from the level file.
                    data_stream >> sprite;

                    //Then generate a new tile in the tile vector with the proper coordinates and the type loaded from the level file.
                    tile_background_array[int_x][int_y].x=int_x*TILE_SIZE;
                    tile_background_array[int_x][int_y].y=int_y*TILE_SIZE;
                    tile_background_array[int_x][int_y].sprite=sprite;
                }
            }

            //When we are done using the level file, we close it, and clear load for its next use.
            load.close();
        }
        //If the level file could not be loaded for any reason, we print an error message letting the user know.
        else{
            update_error_log("Error loading background tiles.");
        }
    }
    else{
        tile_background_array.clear();

        tile_background_array.resize(level_x/TILE_SIZE,vector<Tile_Background>(level_y/TILE_SIZE));
    }

    //******************************//
    // Now, we will read the items: //
    //******************************//

    //First, we open the file for reading.

    //If the current sub level is 0, meaning "load the main level."
    if(player.current_sub_level==0){
        level_to_load="data/levels/"+current_level+"/items.blazelevel";
    }
    //If there is a current sub level to load instead of the main one.
    else{
        level_to_load="data/levels/"+current_level+"/"+current_sub_level+"/items.blazelevel";
    }
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

            if(type!=30000){
                vector_items.push_back(Item(x,y,false,type,goal_level_to_load,goal_secret));
            }
        }

        //When we are done using the level file, we close it, and clear load for its next use.
        load.close();
    }
    //If the level file could not be loaded for any reason, we print an error message letting the user know.
    else{
        update_error_log("Error loading level items.");
    }

    //*****************************//
    // Now, we will read the npcs: //
    //*****************************//

    //We only read the npcs if the level is not the world map.
    if(!player.on_worldmap()){
        //First, we open the file for reading.

        //If the current sub level is 0, meaning "load the main level."
        if(player.current_sub_level==0){
            level_to_load="data/levels/"+current_level+"/npcs.blazelevel";
        }
        //If there is a current sub level to load instead of the main one.
        else{
            level_to_load="data/levels/"+current_level+"/"+current_sub_level+"/npcs.blazelevel";
        }
        load.open(level_to_load);

        //As long as the file opened successfully, proceed to load the data.
        if(load.is_opened()){
            istringstream data_stream(load.get_data());

            //Now we make a temporary variable for getting the type of each npc being loaded.
            short type;
            double x;
            double y;

            //Then we iterate through all of the items in the file, setting each one.
            while(!data_stream.eof()){
                type=0;
                x=0;
                y=0;

                //For each item, load the type information from the file.
                data_stream >> type;
                data_stream >> x;
                data_stream >> y;

                bool should_spawn=true;
                if(!player.game_mode_is_multiplayer() || player.game_mode==GAME_MODE_MP_ADVENTURE){
                    //If the npc is a main boss and has either not spawned or already been defeated.
                    if(player.npc_is_main_boss(type) && (player.bosses[player.current_level]==BOSS_STATE_NOT_SPAWNED ||
                        player.bosses[player.current_level]==BOSS_STATE_DEFEATED)){
                        should_spawn=false;
                    }
                    ///If npc is a holiday boss.

                    //If the npc is Sally and the main final boss has not yet been defeated.
                    if(type==NPC_SALLY && player.new_game_plus==0){
                        should_spawn=false;
                    }
                }

                if(type!=0 && should_spawn){
                    vector_npcs.push_back(Npc(x,y,type));

                    //If the new npc is a boss.
                    if(vector_npcs[vector_npcs.size()-1].BOSS){
                        //We have found this level's boss.
                        boss_index=vector_npcs.size()-1;
                    }
                }
            }

            //When we are done using the level file, we close it, and clear load for its next use.
            load.close();
        }
        //If the level file could not be loaded for any reason, we print an error message letting the user know.
        else{
            update_error_log("Error loading level npcs.");
        }
    }

    //*****************************************//
    // Now, we will read the moving platforms: //
    //*****************************************//

    //We only read the moving platforms if the level is not the world map.
    if(!player.on_worldmap()){
        //First, we open the file for reading.

        //If the current sub level is 0, meaning "load the main level."
        if(player.current_sub_level==0){
            level_to_load="data/levels/"+current_level+"/moving_platforms.blazelevel";
        }
        //If there is a current sub level to load instead of the main one.
        else{
            level_to_load="data/levels/"+current_level+"/"+current_sub_level+"/moving_platforms.blazelevel";
        }
        load.open(level_to_load);

        //As long as the file opened successfully, proceed to load the level data.
        if(load.is_opened()){
            istringstream data_stream(load.get_data());

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
            while(!data_stream.eof()){
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
                data_stream >> move_speed;
                data_stream >> start_point_x;
                data_stream >> start_point_y;
                data_stream >> end_point_x;
                data_stream >> end_point_y;
                data_stream >> active;
                data_stream >> round_trip;
                data_stream >> movement_type;
                data_stream >> identifier;
                data_stream >> number_of_waypoints;

                for(int i=0;i<number_of_waypoints;i++){
                    waypoints.push_back(moving_platform_waypoint());
                    data_stream >> waypoints[i].x;
                    data_stream >> waypoints[i].y;
                }

                if(move_speed!=0){
                    vector_moving_platforms.push_back(Moving_Platform(move_speed,start_point_x,start_point_y,end_point_x,end_point_y,waypoints,active,round_trip,movement_type,identifier));
                }
            }

            //When we are done using the level file, we close it, and clear load for its next use.
            load.close();
        }
        //If the level file could not be loaded for any reason, we print an error message letting the user know.
        else{
            update_error_log("Error loading moving platforms.");
        }
    }

    //*********************************//
    // Now, we will read the triggers: //
    //*********************************//

    //We only read the triggers if the level is not the world map.
    if(!player.on_worldmap()){
        //First, we open the file for reading.

        //If the current sub level is 0, meaning "load the main level."
        if(player.current_sub_level==0){
            level_to_load="data/levels/"+current_level+"/triggers.blazelevel";
        }
        //If there is a current sub level to load instead of the main one.
        else{
            level_to_load="data/levels/"+current_level+"/"+current_sub_level+"/triggers.blazelevel";
        }
        load.open(level_to_load);

        //As long as the file opened successfully, proceed to load the level data.
        if(load.is_opened()){
            istringstream data_stream(load.get_data());

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
            while(!data_stream.eof()){
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
                data_stream >> x;
                data_stream >> y;
                data_stream >> w;
                data_stream >> h;
                data_stream >> number_of_targets;

                for(int i=0;i<number_of_targets;i++){
                    targets.push_back(target());
                    data_stream >> targets[i].type;
                    data_stream >> targets[i].identifier;
                }

                data_stream >> trigger_method;
                data_stream >> user_type;
                data_stream >> repeating;
                data_stream >> repeat_time;
                data_stream >> render_trigger;

                if(w!=0){
                    vector_triggers.push_back(Trigger(x,y,w,h,targets,trigger_method,user_type,repeating,repeat_time,render_trigger));
                }
            }

            //When we are done using the level file, we close it.
            load.close();
        }
        //If the level file could not be loaded for any reason, we print an error message letting the user know.
        else{
            update_error_log("Error loading triggers.");
        }
    }

    //******************************//
    // Now, we will read the doors: //
    //******************************//

    //We only read the doors if the level is not the world map.
    if(!player.on_worldmap()){
        //First, we open the file for reading.

        //If the current sub level is 0, meaning "load the main level."
        if(player.current_sub_level==0){
            level_to_load="data/levels/"+current_level+"/doors.blazelevel";
        }
        //If there is a current sub level to load instead of the main one.
        else{
            level_to_load="data/levels/"+current_level+"/"+current_sub_level+"/doors.blazelevel";
        }
        load.open(level_to_load);

        //As long as the file opened successfully, proceed to load the level data.
        if(load.is_opened()){
            istringstream data_stream(load.get_data());

            //Now we make temporary variables.
            double x,y;
            short type;
            short number;
            bool open;
            short identifier;

            //Then we iterate through all of the doors in the file, setting each one.
            while(!data_stream.eof()){
                x=0;
                y=0;
                type=0;
                number=0;
                open=false;
                identifier=-1;

                //For each one, load the type information from the file.
                data_stream >> x;
                data_stream >> y;
                data_stream >> type;
                data_stream >> number;
                data_stream >> open;
                data_stream >> identifier;

                if(identifier!=-1){
                    vector_doors.push_back(Door(x,y,type,number,open,identifier));
                }
            }

            //When we are done using the level file, we close it.
            load.close();
        }
        //If the level file could not be loaded for any reason, we print an error message letting the user know.
        else{
            update_error_log("Error loading doors.");
        }
    }

    //******************************//
    // Now, we will read the traps: //
    //******************************//

    //We only read the traps if the level is not the world map.
    if(!player.on_worldmap()){
        //First, we open the file for reading.

        //If the current sub level is 0, meaning "load the main level."
        if(player.current_sub_level==0){
            level_to_load="data/levels/"+current_level+"/traps.blazelevel";
        }
        //If there is a current sub level to load instead of the main one.
        else{
            level_to_load="data/levels/"+current_level+"/"+current_sub_level+"/traps.blazelevel";
        }
        load.open(level_to_load);

        //As long as the file opened successfully, proceed to load the level data.
        if(load.is_opened()){
            istringstream data_stream(load.get_data());

            //Now we make temporary variables.
            double x,y;
            short type;
            bool active;
            short identifier;

            //Then we iterate through all of the traps in the file, setting each one.
            while(!data_stream.eof()){
                x=0;
                y=0;
                type=0;
                active=false;
                identifier=-1;

                //For each one, load the type information from the file.
                data_stream >> x;
                data_stream >> y;
                data_stream >> type;
                data_stream >> active;
                data_stream >> identifier;

                if(identifier!=-1){
                    vector_traps.push_back(Trap(x,y,type,active,identifier));
                }
            }

            //When we are done using the level file, we close it.
            load.close();
        }
        //If the level file could not be loaded for any reason, we print an error message letting the user know.
        else{
            update_error_log("Error loading traps.");
        }
    }

    //******************************//
    // Now, we will read the signs: //
    //******************************//

    //We only read the signs if the level is not the world map.
    if(!player.on_worldmap()){
        //First, we open the file for reading.

        //If the current sub level is 0, meaning "load the main level."
        if(player.current_sub_level==0){
            level_to_load="data/levels/"+current_level+"/signs.blazelevel";
        }
        //If there is a current sub level to load instead of the main one.
        else{
            level_to_load="data/levels/"+current_level+"/"+current_sub_level+"/signs.blazelevel";
        }
        load.open(level_to_load);

        //As long as the file opened successfully, proceed to load the level data.
        if(load.is_opened()){
            istringstream data_stream(load.get_data());

            //Now we make temporary variables.
            double x,y;
            string message;
            short sign_type;
            short font_type;

            //Then we iterate through all of the signs in the file, setting each one.
            while(!data_stream.eof()){
                x=-1;
                y=-1;
                message="";
                sign_type=0;
                font_type=0;

                //For each one, load the type information from the file.
                data_stream >> x;
                data_stream >> y;
                data_stream >> message;
                data_stream >> sign_type;
                data_stream >> font_type;

                boost::algorithm::replace_all(message,"<SPACE>"," ");
                boost::algorithm::replace_all(message,"<NEWLINE>","\xA");

                if(x!=-1 && y!=-1){
                    vector_signs.push_back(Sign(x,y,message,sign_type,font_type));
                }
            }

            //When we are done using the level file, we close it.
            load.close();
        }
        //If the level file could not be loaded for any reason, we print an error message letting the user know.
        else{
            update_error_log("Error loading signs.");
        }
    }

    //********************************//
    // Now, we will read the springs: //
    //********************************//

    //We only read the springs if the level is not the world map.
    if(!player.on_worldmap()){
        //First, we open the file for reading.

        //If the current sub level is 0, meaning "load the main level."
        if(player.current_sub_level==0){
            level_to_load="data/levels/"+current_level+"/springs.blazelevel";
        }
        //If there is a current sub level to load instead of the main one.
        else{
            level_to_load="data/levels/"+current_level+"/"+current_sub_level+"/springs.blazelevel";
        }
        load.open(level_to_load);

        //As long as the file opened successfully, proceed to load the level data.
        if(load.is_opened()){
            istringstream data_stream(load.get_data());

            //Now we make temporary variables.
            double x,y;
            double speed;
            short direction;

            //Then we iterate through all of the springs in the file, setting each one.
            while(!data_stream.eof()){
                x=-1;
                y=-1;
                speed=0.0;
                direction=LEFT;

                //For each one, load the type information from the file.
                data_stream >> x;
                data_stream >> y;
                data_stream >> speed;
                data_stream >> direction;

                if(x!=-1 && y!=-1){
                    vector_springs.push_back(Spring(x,y,speed,direction));
                }
            }

            //When we are done using the level file, we close it.
            load.close();
        }
        //If the level file could not be loaded for any reason, we print an error message letting the user know.
        else{
            update_error_log("Error loading springs.");
        }
    }

    //*********************************//
    // Now, we will read the boosters: //
    //*********************************//

    //We only read the boosters if the level is not the world map.
    if(!player.on_worldmap()){
        //First, we open the file for reading.

        //If the current sub level is 0, meaning "load the main level."
        if(player.current_sub_level==0){
            level_to_load="data/levels/"+current_level+"/boosters.blazelevel";
        }
        //If there is a current sub level to load instead of the main one.
        else{
            level_to_load="data/levels/"+current_level+"/"+current_sub_level+"/boosters.blazelevel";
        }
        load.open(level_to_load);

        //As long as the file opened successfully, proceed to load the level data.
        if(load.is_opened()){
            istringstream data_stream(load.get_data());

            //Now we make temporary variables.
            double x,y;
            double speed;
            short direction;

            //Then we iterate through all of the boosters in the file, setting each one.
            while(!data_stream.eof()){
                x=-1;
                y=-1;
                speed=0.0;
                direction=LEFT;

                //For each one, load the type information from the file.
                data_stream >> x;
                data_stream >> y;
                data_stream >> speed;
                data_stream >> direction;

                if(x!=-1 && y!=-1){
                    vector_boosters.push_back(Booster(x,y,speed,direction));
                }
            }

            //When we are done using the level file, we close it.
            load.close();
        }
        //If the level file could not be loaded for any reason, we print an error message letting the user know.
        else{
            update_error_log("Error loading boosters.");
        }
    }

    //Set the weather machine lever to its correct state, if it is in this level.
    bool weather_machine_check=false;
    for(int i=0;i<vector_triggers.size() && !weather_machine_check;i++){
        for(int n=0;n<vector_traps.size() && !weather_machine_check;n++){
            if(vector_traps[n].type==TRAP_WEATHER_MACHINE &&
               fabs(vector_traps[n].x-vector_triggers[i].x)<128 && fabs(vector_traps[n].y-vector_triggers[i].y)<128){
                if(player.weather_machine){
                    vector_triggers[i].frame=0;
                }
                else{
                    vector_triggers[i].frame=1;
                }
                weather_machine_check=true;
            }
        }
    }

    //As long as the last level was not the same as this one.
    if(player.previous_level!=player.current_level || player.previous_sub_level!=player.current_sub_level){
        //If there is a boss, we will also be loading boss music.
        if(boss_index!=-1){
            things_to_load+=2;
        }
        render_loading_screen((double)++things_loaded/(double)things_to_load,player.current_level);

        music.unload_track(MUSIC_BOSS);
        music.unload_track(MUSIC_BOSS_FAST);
        music.unload_track(MUSIC_LEVEL_NORMAL);
        music.unload_track(MUSIC_LEVEL_WATER);

        //If there is a boss in this level.
        if(boss_index!=-1){
            music.load_track("data/music/"+current_level+"/boss.ogg",MUSIC_BOSS);
            render_loading_screen((double)++things_loaded/(double)things_to_load,player.current_level);

            music.load_track("data/music/"+current_level+"/boss_fast.ogg",MUSIC_BOSS_FAST);
            render_loading_screen((double)++things_loaded/(double)things_to_load,player.current_level);
        }

        //If the current sub level is 0, meaning "load the main level."
        if(player.current_sub_level==0){
            music.load_track("data/music/"+current_level+"/normal.ogg",MUSIC_LEVEL_NORMAL);
            render_loading_screen((double)++things_loaded/(double)things_to_load,player.current_level);

            music.load_track("data/music/"+current_level+"/water.ogg",MUSIC_LEVEL_WATER);
            render_loading_screen((double)++things_loaded/(double)things_to_load,player.current_level);
        }
        else{
            music.load_track("data/music/"+current_level+"/"+current_sub_level+"/normal.ogg",MUSIC_LEVEL_NORMAL);
            render_loading_screen((double)++things_loaded/(double)things_to_load,player.current_level);

            music.load_track("data/music/"+current_level+"/"+current_sub_level+"/water.ogg",MUSIC_LEVEL_WATER);
            render_loading_screen((double)++things_loaded/(double)things_to_load,player.current_level);
        }

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

        if(!player.on_worldmap()){
            set_title_strings(player.current_level);
            title_top_x=main_window.SCREEN_WIDTH+font_large.spacing_x*2.0;
            title_bottom_x=0.0-title_bottom_string.length()*font_large.spacing_x*2.0-font_large.spacing_x*2.0;
            title_bg_y_top=0.0-main_window.SCREEN_HEIGHT;
            title_bg_x_bottom=main_window.SCREEN_WIDTH;
            title_bg_x_left=0.0-main_window.SCREEN_WIDTH;
            title_moving=TITLE_BG_MOVING_IN;
        }
        else{
            title_top_x=0.0;
            title_bottom_x=0.0;
            title_bg_y_top=0.0;
            title_bg_x_bottom=0.0;
            title_bg_x_left=0.0;
            title_top_string="";
            title_bottom_string="";
            title_moving=TITLE_NOT_MOVING;
            counter_title=0;
        }

        timer_current_level.start();
        seconds_in_current_level=0;
        player.seconds_total_current_level=0;
        player.seconds_best_current_level=4294967295;
    }

    if(title_moving==TITLE_NOT_MOVING){
        //Unpause the game.
        player.toggle_pause(false);
    }

    if(player.game_mode_is_multiplayer()){
        //Replace items with other items.
        for(int i=0;i<vector_items.size();i++){
            vector<Item> item_replace;
            short new_type=ITEM_NONE;

            /**if(vector_items[i].type==ITEM_CHEESE){
                new_type=ITEM_AMMO;
            }*/
            /**else if((vector_items[i].type>=ITEM_SWIMMING_GEAR && vector_items[i].type<=ITEM_SINK) ||
                    (vector_items[i].type>=ITEM_KEY_GRAY && vector_items[i].type<=ITEM_KEY_CYAN)){
                new_type=ITEM_AMMO;
            }*/

            if(new_type!=ITEM_NONE){
                item_replace.push_back(Item(vector_items[i].x,vector_items[i].y,false,new_type,0,false));
                vector_items.insert(vector_items.begin()+i,item_replace[0]);
                vector_items.erase(vector_items.begin()+i+1);
            }
        }
    }

    if(player.get_upgrade_state("monkeys")){
        for(int i=0;i<vector_npcs.size();i++){
            if(!vector_npcs[i].BOSS && !vector_npcs[i].counts_as_trap && !vector_npcs[i].friendly && vector_npcs[i].type!=NPC_MONKEY){
                double x=vector_npcs[i].x;
                double y=vector_npcs[i].y;

                vector_npcs.erase(vector_npcs.begin()+i);
                i--;

                vector_npcs.push_back(Npc(x,y,NPC_MONKEY));
            }
        }
    }

    //Add or remove npc's based on difficulty.
    short difficulty=player.return_actual_difficulty();

    Random_Number_Generator temp_rng;
    temp_rng.mrand_main.seed((uint32_t)player.current_level);

    if(difficulty==DIFFICULTY_EASY){
        for(int i=0;i<vector_npcs.size();i++){
            if(random_range_pass(&temp_rng.mrand_main,0,99)<25 && !vector_npcs[i].BOSS && vector_npcs[i].type!=NPC_SALLY && !vector_npcs[i].friendly){
                vector_npcs.erase(vector_npcs.begin()+i);
                i--;
            }
        }
    }
    else if(difficulty>=DIFFICULTY_HARD){
        vector<int> indexes;
        for(int i=0;i<vector_npcs.size();i++){
            bool on_list=false;
            for(int n=0;n<indexes.size();n++){
                if(vector_npcs[i].type==vector_npcs[indexes[n]].type){
                    on_list=true;
                    break;
                }
            }

            if(!vector_npcs[i].BOSS && vector_npcs[i].exists && !vector_npcs[i].counts_as_trap && !vector_npcs[i].friendly && vector_npcs[i].can_be_duplicated && !on_list){
                indexes.push_back(i);
            }
        }

        int spawn_protection=768;
        int max_new_npcs=45;

        for(int i=0;i<indexes.size();i++){
            int max_new_this_npc=max_new_npcs/indexes.size();

            int new_npc_count=(difficulty-DIFFICULTY_NORMAL)*3;
            if(new_npc_count>max_new_this_npc){
                new_npc_count=max_new_this_npc;
            }

            for(int n=0;n<new_npc_count;n++){
                Npc* npc=&vector_npcs[indexes[i]];

                bool location_found=false;
                int emergency_loop_break=0;
                int x=0;
                int y=0;
                double x_coord=0.0;
                double y_coord=0.0;

                while(emergency_loop_break<5000){
                    x=random_range_pass(&temp_rng.mrand_main,0,(level_x/TILE_SIZE)-1);
                    y=random_range_pass(&temp_rng.mrand_main,0,(level_y/TILE_SIZE)-1);

                    if(x>=0 && x<=(level_x/TILE_SIZE)-1 && y>=0 && y<=(level_y/TILE_SIZE)-1){
                        bool near_spawn=false;
                        for(int items=0;items<vector_items.size();items++){
                            if((vector_items[items].type==ITEM_SPAWNPOINT || vector_items[items].type==ITEM_CHECKPOINT || vector_items[items].type==ITEM_ENDPOINT) &&
                               abs(vector_items[items].x-x*TILE_SIZE)<spawn_protection && abs(vector_items[items].y-y*TILE_SIZE)<spawn_protection){
                                near_spawn=true;

                                break;
                            }
                        }

                        if(!near_spawn){
                            Tile* tile=&tile_array[x][y];

                            //The npc's dimensions in tiles.
                            int npc_width=(int)ceil(npc->w/(double)TILE_SIZE);
                            int npc_height=(int)ceil(npc->h/(double)TILE_SIZE);

                            bool location_valid=false;

                            if(npc->FLYING){
                                bool invalid_tile=false;
                                for(int int_x=x;int_x<x+npc_width && !invalid_tile;int_x++){
                                    for(int int_y=y;int_y<y+npc_height && !invalid_tile;int_y++){
                                        if(int_x>=0 && int_x<=(level_x/TILE_SIZE)-1 && int_y>=0 && int_y<=(level_y/TILE_SIZE)-1){
                                            Tile* tile_check=&tile_array[int_x][int_y];
                                            if(tile_check->solidity!=TILE_SOLIDITY_PASSABLE || tile_check->special==TILE_SPECIAL_HAZARD ||
                                               tile_check->special==TILE_SPECIAL_WATER || tile_check->foreground){
                                                invalid_tile=true;
                                            }
                                        }
                                        else{
                                            invalid_tile=true;
                                        }
                                    }
                                }

                                if(!invalid_tile){
                                    location_valid=true;
                                }
                            }
                            else if(npc->swimming_gear){
                                bool invalid_tile=false;
                                for(int int_x=x;int_x<x+npc_width && !invalid_tile;int_x++){
                                    for(int int_y=y;int_y<y+npc_height && !invalid_tile;int_y++){
                                        if(int_x>=0 && int_x<=(level_x/TILE_SIZE)-1 && int_y>=0 && int_y<=(level_y/TILE_SIZE)-1){
                                            Tile* tile_check=&tile_array[int_x][int_y];
                                            if(tile_check->special!=TILE_SPECIAL_WATER){
                                                invalid_tile=true;
                                            }
                                        }
                                        else{
                                            invalid_tile=true;
                                        }
                                    }
                                }

                                if(!invalid_tile){
                                    location_valid=true;
                                }
                            }
                            else{
                                bool invalid_tile=false;
                                for(int int_x=x;int_x<x+npc_width && !invalid_tile;int_x++){
                                    for(int int_y=y;int_y<y+npc_height && !invalid_tile;int_y++){
                                        if(int_x>=0 && int_x<=(level_x/TILE_SIZE)-1 && int_y>=0 && int_y<=(level_y/TILE_SIZE)-1){
                                            Tile* tile_check=&tile_array[int_x][int_y];
                                            if(tile_check->solidity!=TILE_SOLIDITY_PASSABLE || tile_check->special==TILE_SPECIAL_HAZARD ||
                                               tile_check->special==TILE_SPECIAL_WATER || tile_check->foreground){
                                                invalid_tile=true;
                                            }
                                        }
                                        else{
                                            invalid_tile=true;
                                        }
                                    }
                                }

                                if(!invalid_tile){
                                    if(y+npc_height<=(level_y/TILE_SIZE)-1 && tile_array[x][y+npc_height].solidity!=TILE_SOLIDITY_PASSABLE && tile_array[x][y+npc_height].slope==180){
                                        location_valid=true;
                                    }
                                }
                            }

                            if(location_valid){
                                x_coord=x*TILE_SIZE;
                                y_coord=y*TILE_SIZE;

                                location_found=true;

                                break;
                            }
                        }
                    }
                    else{
                        emergency_loop_break++;
                    }
                }

                if(location_found){
                    vector_npcs.push_back(Npc(x_coord,y_coord,npc->type));
                }
            }
        }
    }

    //The difficulty stuff above can invalidate the boss index, so
    //if there is one, fix it.
    if(boss_index!=-1){
        for(int i=0;i<vector_npcs.size();i++){
            if(vector_npcs[i].BOSS){
                boss_index=i;
                break;
            }
        }

        //Set all boss traps' boss pointers.
        for(int i=0;i<vector_traps.size();i++){
            if(vector_traps[i].BOSS){
                vector_traps[i].boss_index=boss_index;
            }
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

    profile.load_level_data();

    if(player.game_mode==GAME_MODE_MP_ADVENTURE){
        player.mp_load_level_data();

        player.x=player.world_x[player.current_level];
        player.y=player.world_y[player.current_level];
    }

    bool spawnpoint_found=false;

    //Determine the player's starting position and item counts.
    for(int i=0;i<vector_items.size();i++){
        //If the player has not yet reached a checkpoint, the player will spawn at the spawn point.
        if(player.current_checkpoint==-1 && vector_items[i].type==ITEM_SPAWNPOINT){
            if(player.on_worldmap()){
                if(player.y==-1 || player.x==-1){
                    player.x=vector_items[i].x;
                    player.y=vector_items[i].y;

                    for(int n=0;n<mp_players.size();n++){
                        mp_players[n].x=vector_items[i].x;
                        mp_players[n].y=vector_items[i].y;
                    }

                    player.world_x[player.current_level]=player.x;
                    player.world_y[player.current_level]=player.y;
                }
            }
            else{
                player.x=vector_items[i].x;
                player.y=vector_items[i].y-14;

                for(int n=0;n<mp_players.size();n++){
                    mp_players[n].x=vector_items[i].x;
                    mp_players[n].y=vector_items[i].y-14;
                }
            }
            spawnpoint_found=true;
        }
        //Otherwise, spawn at the current checkpoint.
        else if(player.current_checkpoint!=-1 && vector_items[i].type==ITEM_CHECKPOINT){
            if(i==player.current_checkpoint){
                player.x=vector_items[i].x;
                player.y=vector_items[i].y+50;

                for(int n=0;n<mp_players.size();n++){
                    mp_players[n].x=vector_items[i].x;
                    mp_players[n].y=vector_items[i].y+50;
                }

                spawnpoint_found=true;

                vector_items[i].frame=ITEM_SPRITES_CHECKPOINT-1;
            }
        }

        if(vector_items[i].type==ITEM_LEAF){
            leaves++;
        }
        else if(vector_items[i].type==ITEM_CHEESE){
            cheese++;
        }

        //If we have looked at all items and not found a spawn point, set the player's coordinates accordingly.
        if(i==vector_items.size()-1 && !spawnpoint_found){
            player.x=0;
            player.y=0;

            for(int n=0;n<mp_players.size();n++){
                mp_players[n].x=0;
                mp_players[n].y=0;
            }
        }
    }

    player.camera_trap_x=player.x;
    if(!player.on_worldmap()){
        player.camera_trap_y=player.y+player.h-CAMERA_TRAP_H;
    }
    else{
        player.camera_trap_y=player.y+player.h-CAMERA_TRAP_WORLDMAP_H;
    }

    profile.save_level_data();

    //Snap the camera to the starting position.
    player.set_camera();

    //Load this level's images.
    if(player.previous_level!=player.current_level || player.previous_sub_level!=player.current_sub_level){
        image.load_images_level(player.current_level,player.current_sub_level,player.non_number_level);
    }

    //If the current level is the world map.
    if(player.on_worldmap()){
        profile.update_achievements(ACHIEVEMENT_ENTER_WORLDMAP);
    }
    //If the current level is not the world map.
    else{
        profile.update_achievements(ACHIEVEMENT_ENTER_ANY_LEVEL);
    }

    //If this is DevWorld.
    if(player.current_level==2){
        devworld_tiles.setup(level_x,level_y);
    }
}

void Level::generate_level(){
}

bool Level::onMap_map(int x,int y){
	return (x>=0 && x<level_x/TILE_SIZE && y>=0 && y<level_y/TILE_SIZE);
}

bool Level::blockLOS_map(int x,int y){
	if(!onMap_map(x,y)){
        return true;
	}

	return false;
}

void Level::setSeen_map(int x,int y){
	if(!onMap_map(x,y)){
        return;
	}

    profile.current_level_map_data[x][y]=true;
}

bool Level::onMap_lighting(int x,int y){
	return (x>=0 && x<level_x/LIGHTING_TILE_SIZE && y>=0 && y<level_y/LIGHTING_TILE_SIZE);
}

bool Level::blockLOS_lighting(int x,int y){
	if(!onMap_lighting(x,y)){
        return true;
	}

	int tile_current_x=(int)(((int)x*LIGHTING_TILE_SIZE)/TILE_SIZE);
    int tile_current_y=(int)(((int)y*LIGHTING_TILE_SIZE)/TILE_SIZE);

    bool blocks_light=false;

    if(tile_array[tile_current_x][tile_current_y].solidity==TILE_SOLIDITY_SOLID &&
       tile_array[tile_current_x][tile_current_y].slope==180){
        blocks_light=true;
    }
    else if(tile_array[tile_current_x][tile_current_y].foreground &&
            tile_array[tile_current_x][tile_current_y].special!=TILE_SPECIAL_WATER){
        blocks_light=true;
    }

	return blocks_light;
}

void Level::setSeen_lighting(int x,int y,Light_Source* src){
	if(!onMap_lighting(x,y)){
        return;
	}

    player.light_data[x][y].seen=true;

    double dimness=src->dimness+(src->falloff*distance_between_points(x,y,src->x,src->y));///(fabs(x-src->x)+fabs(y-src->y))
    if(dimness>1.0){
        dimness=1.0;
    }
    else if(dimness<0.0){
        dimness=0.0;
    }

    if(player.light_data[x][y].color.red==-1.0){
        player.light_data[x][y].color.red=src->color.red;
        player.light_data[x][y].color.green=src->color.green;
        player.light_data[x][y].color.blue=src->color.blue;

        player.light_data[x][y].color.red-=src->color.red*dimness;
        player.light_data[x][y].color.green-=src->color.green*dimness;
        player.light_data[x][y].color.blue-=src->color.blue*dimness;

        if(player.light_data[x][y].color.red<0.0){
            player.light_data[x][y].color.red=0.0;
        }
        if(player.light_data[x][y].color.green<0.0){
            player.light_data[x][y].color.green=0.0;
        }
        if(player.light_data[x][y].color.blue<0.0){
            player.light_data[x][y].color.blue=0.0;
        }
    }
    else{
        player.light_data[x][y].color.red*=src->color.red+src->color.red*dimness;
        player.light_data[x][y].color.green*=src->color.green+src->color.green*dimness;
        player.light_data[x][y].color.blue*=src->color.blue+src->color.blue*dimness;

        player.light_data[x][y].color.red+=0.5+dimness/4.0;
        player.light_data[x][y].color.green+=0.5+dimness/4.0;
        player.light_data[x][y].color.blue+=0.5+dimness/4.0;

        if(player.light_data[x][y].color.red>1.0){
            player.light_data[x][y].color.red=1.0;
        }
        if(player.light_data[x][y].color.green>1.0){
            player.light_data[x][y].color.green=1.0;
        }
        if(player.light_data[x][y].color.blue>1.0){
            player.light_data[x][y].color.blue=1.0;
        }
    }

    if(player.light_data[x][y].light_intensity==-1.0){
        player.light_data[x][y].light_intensity=dimness;
    }
    else{
        player.light_data[x][y].light_intensity*=dimness;
    }

    if(player.light_data[x][y].dimness==-1.0){
        player.light_data[x][y].dimness=dimness;
    }
    else{
        player.light_data[x][y].dimness*=dimness;
    }
}

Light_Data Level::blend_light(double src_opacity,color_data src_color,double dest_opacity,color_data dest_color){
    Light_Data result_light;

    result_light.light_intensity=src_opacity+(dest_opacity*(1.0-src_opacity));

    result_light.color.red=((src_color.red*src_opacity)+(dest_color.red*(dest_opacity*(1.0-src_opacity))))/result_light.light_intensity;
    result_light.color.green=((src_color.green*src_opacity)+(dest_color.green*(dest_opacity*(1.0-src_opacity))))/result_light.light_intensity;
    result_light.color.blue=((src_color.blue*src_opacity)+(dest_color.blue*(dest_opacity*(1.0-src_opacity))))/result_light.light_intensity;

    return result_light;
}
