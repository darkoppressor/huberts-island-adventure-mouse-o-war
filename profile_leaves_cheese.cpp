/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "profile.h"
#include "world.h"
#include "message_log.h"

#include <fstream>
#include <boost/filesystem.hpp>

using namespace std;

void Profile::load_leaves_cheese_counts(){
    if(player.game_mode==GAME_MODE_SP_ADVENTURE){
        player.leaves_max=0;
        player.cheese_max=0;

        for(int i=0;i<=LAST_LEVEL;i++){
            Leaves_And_Cheese lac=load_this_levels_leaves_cheese(i);
            player.leaves_max+=lac.leaves;
            player.cheese_max+=lac.cheese;
        }
    }
}

Leaves_And_Cheese Profile::load_this_levels_leaves_cheese(short level_to_check){
    Leaves_And_Cheese lac;
    lac.leaves=0;
    lac.cheese=0;

    if(player.game_mode==GAME_MODE_SP_ADVENTURE){
        //Setup the level string.
        string current_level="";
        ss.clear();ss.str("");ss<<level_to_check;current_level=ss.str();

        ifstream load;
        string level_to_load="";

        //******************************//
        // Now, we will read the items: //
        //******************************//

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

                if(type==ITEM_LEAF){
                    lac.leaves++;
                }
                else if(type==ITEM_CHEESE){
                    lac.cheese++;
                }
            }

            //When we are done using the level file, we close it, and clear load for its next use.
            load.close();
            load.clear();
        }
        //If the level file could not be loaded for any reason, we print an error message letting the user know.
        else{
            update_error_log("Error reading level's leaves and cheese.");
        }
    }

    return lac;
}

Leaves_And_Cheese Profile::load_collected_leaves_cheese(short level_to_check){
    Leaves_And_Cheese lac;
    lac.leaves=0;
    lac.cheese=0;

    if(player.game_mode==GAME_MODE_SP_ADVENTURE){
        //Create a string to hold the current level number.
        string current_level="";
        ss.clear();ss.str("");ss<<level_to_check;current_level=ss.str();

        ifstream load;
        string level_to_load="";

        vector<Item> temp_items;

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

                if(type!=30000){
                    temp_items.push_back(Item(x,y,false,type,goal_level_to_load,goal_secret));
                }
            }

            //When we are done using the level file, we close it, and clear load for its next use.
            load.close();
            load.clear();
        }
        //If the level file could not be loaded for any reason, we print an error message letting the user know.
        else{
            update_error_log("Error loading level items for leaf/cheese collected count.");
        }

        if(temp_items.size()>0){
            level_to_load=get_home_directory()+"profiles/"+player.name+"/saves/"+current_level+"/items.blazesave";
            load.open(level_to_load.c_str(),ifstream::in);

            if(load!=NULL){
                bool collected=false;

                for(int i=0;i<temp_items.size();i++){
                    //If the item is of a collectable type.
                    if(is_item_collectable(temp_items[i].type)){
                        load>>collected;
                        if(collected){
                            if(temp_items[i].type==ITEM_LEAF){
                                lac.leaves++;
                            }
                            else if(temp_items[i].type==ITEM_CHEESE){
                                lac.cheese++;
                            }
                        }
                    }
                }

                load.close();
                load.clear();
            }
            else{
            }
        }
    }

    return lac;
}
