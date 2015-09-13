/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "special_info_strings.h"
#include "world.h"
#include "version.h"

using namespace std;

special_info_return_data set_special_info_string(short special_info){
    string text="";

    short command=0;

    //Used for figuring percentage of game complete.
    double top_n;
    double bottom_n;

    //Used for SPECIAL_INFO_STAT_ACCURACY.
    double accuracy=0.0;

    //Used for times.
    string temp="";
    unsigned long seconds=0;
    unsigned long minutes=0;
    unsigned long hours=0;

    //Used for level stats.
    int which_window=-1;

    switch(special_info){

    case SPECIAL_INFO_CURRENT_PROFILE:
        ss.clear();ss.str("");ss<<"Current profile:\xA";ss<<player.name;text=ss.str();
        break;

    case SPECIAL_INFO_NO_PROFILE:
        if(player.name=="\x1F" && !profile.version_mismatch){
            text="No profiles exist yet,\nso you must create one.";
        }
        else{
            text="";
        }
        break;

    case SPECIAL_INFO_PROFILE_TAKEN:
        if(profile.profile_list.size()==7){
            text="Maximum number of\nprofiles reached.";
        }
        else if(profile.does_profile_exist(creating_profile)){
            text="That profile name\nalready exists.";
        }
        else{
            text="";
        }
        break;

    case SPECIAL_INFO_CREATING_PROFILE:
        text=creating_profile;
        break;

    case SPECIAL_INFO_DIFFICULTY:
        if(player.option_difficulty==DIFFICULTY_EASY){
            ss.clear();ss.str("");ss<<"Easy";text=ss.str();
        }
        else if(player.option_difficulty==DIFFICULTY_NORMAL){
            ss.clear();ss.str("");ss<<"Normal";text=ss.str();
        }
        else if(player.option_difficulty==DIFFICULTY_HARD){
            ss.clear();ss.str("");ss<<"Hard";text=ss.str();
        }
        break;

    case SPECIAL_INFO_KEY_TOGGLE_MAIN_MENU:
        command=COMMAND_TOGGLE_MAIN_MENU;
        break;

    case SPECIAL_INFO_KEY_TOGGLE_INVENTORY:
        command=COMMAND_TOGGLE_INVENTORY;
        break;

    case SPECIAL_INFO_KEY_TOGGLE_MAP:
        command=COMMAND_TOGGLE_MAP;
        break;

    case SPECIAL_INFO_KEY_SCREENSHOT:
        command=COMMAND_SCREENSHOT;
        break;

    case SPECIAL_INFO_KEY_LEFT:
        command=COMMAND_LEFT;
        break;

    case SPECIAL_INFO_KEY_UP:
        command=COMMAND_UP;
        break;

    case SPECIAL_INFO_KEY_RIGHT:
        command=COMMAND_RIGHT;
        break;

    case SPECIAL_INFO_KEY_DOWN:
        command=COMMAND_DOWN;
        break;

    case SPECIAL_INFO_KEY_LOOK:
        command=COMMAND_LOOK;
        break;

    case SPECIAL_INFO_KEY_JUMP:
        command=COMMAND_JUMP;
        break;

    case SPECIAL_INFO_KEY_SHOOT:
        command=COMMAND_SHOOT;
        break;

    case SPECIAL_INFO_KEY_CHANGE_CHARACTER:
        command=COMMAND_CHANGE_CHARACTER;
        break;

    case SPECIAL_INFO_KEY_PAUSE:
        command=COMMAND_PAUSE;
        break;

    case SPECIAL_INFO_KEY_CHANGE_SHOT:
        command=COMMAND_CHANGE_SHOT;
        break;

    case SPECIAL_INFO_KEY_1_LEFT:
        command=COMMAND_LEFT;
        break;

    case SPECIAL_INFO_KEY_1_UP:
        command=COMMAND_UP;
        break;

    case SPECIAL_INFO_KEY_1_RIGHT:
        command=COMMAND_RIGHT;
        break;

    case SPECIAL_INFO_KEY_1_DOWN:
        command=COMMAND_DOWN;
        break;

    case SPECIAL_INFO_KEY_1_JUMP:
        command=COMMAND_JUMP;
        break;

    case SPECIAL_INFO_KEY_1_SHOOT:
        command=COMMAND_SHOOT;
        break;

    case SPECIAL_INFO_KEY_1_CHANGE_SHOT:
        command=COMMAND_CHANGE_SHOT;
        break;

    case SPECIAL_INFO_KEY_2_LEFT:
        command=COMMAND_LEFT;
        break;

    case SPECIAL_INFO_KEY_2_UP:
        command=COMMAND_UP;
        break;

    case SPECIAL_INFO_KEY_2_RIGHT:
        command=COMMAND_RIGHT;
        break;

    case SPECIAL_INFO_KEY_2_DOWN:
        command=COMMAND_DOWN;
        break;

    case SPECIAL_INFO_KEY_2_JUMP:
        command=COMMAND_JUMP;
        break;

    case SPECIAL_INFO_KEY_2_SHOOT:
        command=COMMAND_SHOOT;
        break;

    case SPECIAL_INFO_KEY_2_CHANGE_SHOT:
        command=COMMAND_CHANGE_SHOT;
        break;

    case SPECIAL_INFO_KEY_3_LEFT:
        command=COMMAND_LEFT;
        break;

    case SPECIAL_INFO_KEY_3_UP:
        command=COMMAND_UP;
        break;

    case SPECIAL_INFO_KEY_3_RIGHT:
        command=COMMAND_RIGHT;
        break;

    case SPECIAL_INFO_KEY_3_DOWN:
        command=COMMAND_DOWN;
        break;

    case SPECIAL_INFO_KEY_3_JUMP:
        command=COMMAND_JUMP;
        break;

    case SPECIAL_INFO_KEY_3_SHOOT:
        command=COMMAND_SHOOT;
        break;

    case SPECIAL_INFO_KEY_3_CHANGE_SHOT:
        command=COMMAND_CHANGE_SHOT;
        break;

    case SPECIAL_INFO_PROFILE_1:
        text=profile.profile_list[0];
        break;

    case SPECIAL_INFO_PROFILE_2:
        text=profile.profile_list[1];
        break;

    case SPECIAL_INFO_PROFILE_3:
        text=profile.profile_list[2];
        break;

    case SPECIAL_INFO_PROFILE_4:
        text=profile.profile_list[3];
        break;

    case SPECIAL_INFO_PROFILE_5:
        text=profile.profile_list[4];
        break;

    case SPECIAL_INFO_PROFILE_6:
        text=profile.profile_list[5];
        break;

    case SPECIAL_INFO_PROFILE_7:
        text=profile.profile_list[6];
        break;

    case SPECIAL_INFO_VERSION:
        ss.clear();ss.str("");ss<<"  Version: ";ss<<Version::MAJOR;ss<<".";ss<<Version::MINOR;ss<<".";ss<<Version::MICRO;ss<<" ";ss<<Version::STATUS;text=ss.str();
        break;

    case SPECIAL_INFO_BUILD_DATE:
        ss.clear();ss.str("");ss<<"Build date: ";ss<<Version::YEAR;ss<<"-";ss<<Version::MONTH;ss<<"-";ss<<Version::DAY;text=ss.str();
        break;

    case SPECIAL_INFO_MP_PLAYER_TEXT_0:
        if(player.mp_character==CHARACTER_HUBERT){
            text="Hubert";
        }
        else if(player.mp_character==CHARACTER_SLIME_O){
            text="Slime-O";
        }
        else if(player.mp_character==CHARACTER_SKETCH){
            text="Sketch";
        }
        else if(player.mp_character==CHARACTER_PENNY){
            text="Penny";
        }
        break;

    case SPECIAL_INFO_MP_PLAYER_TEXT_1:
        if(mp_players.size()>=1){
            if(mp_players[0].option_character==CHARACTER_HUBERT){
                text="Hubert";
            }
            else if(mp_players[0].option_character==CHARACTER_SLIME_O){
                text="Slime-O";
            }
            else if(mp_players[0].option_character==CHARACTER_SKETCH){
                text="Sketch";
            }
            else if(mp_players[0].option_character==CHARACTER_PENNY){
                text="Penny";
            }
        }
        break;

    case SPECIAL_INFO_MP_PLAYER_TEXT_2:
        if(mp_players.size()>=2){
            if(mp_players[1].option_character==CHARACTER_HUBERT){
                text="Hubert";
            }
            else if(mp_players[1].option_character==CHARACTER_SLIME_O){
                text="Slime-O";
            }
            else if(mp_players[1].option_character==CHARACTER_SKETCH){
                text="Sketch";
            }
            else if(mp_players[1].option_character==CHARACTER_PENNY){
                text="Penny";
            }
        }
        break;

    case SPECIAL_INFO_MP_PLAYER_TEXT_3:
        if(mp_players.size()>=3){
            if(mp_players[2].option_character==CHARACTER_HUBERT){
                text="Hubert";
            }
            else if(mp_players[2].option_character==CHARACTER_SLIME_O){
                text="Slime-O";
            }
            else if(mp_players[2].option_character==CHARACTER_SKETCH){
                text="Sketch";
            }
            else if(mp_players[2].option_character==CHARACTER_PENNY){
                text="Penny";
            }
        }
        break;

    case SPECIAL_INFO_MP_PLAYER_COUNT:
        ss.clear();ss.str("");ss<<"Players: ";ss<<player.mp_player_count;text=ss.str();
        break;

    case SPECIAL_INFO_MP_AI_1:
        if(mp_players.size()>=1){
            if(player.mp_ai[0]){
                text="  AI controlled";
            }
            else{
                text="Player controlled";
            }
        }
        break;

    case SPECIAL_INFO_MP_AI_2:
        if(mp_players.size()>=2){
            if(player.mp_ai[1]){
                text="  AI controlled";
            }
            else{
                text="Player controlled";
            }
        }
        break;

    case SPECIAL_INFO_MP_AI_3:
        if(mp_players.size()>=3){
            if(player.mp_ai[2]){
                text="  AI controlled";
            }
            else{
                text="Player controlled";
            }
        }
        break;

    case SPECIAL_INFO_STAT_PERCENT_COMPLETE:
        top_n=player.leaves+player.cheese;
        top_n+=player.check_inventory(ITEM_SWIMMING_GEAR)+player.check_inventory(ITEM_SUIT_DEADLY_WATER)+player.check_inventory(ITEM_SUIT_SHARP)+player.check_inventory(ITEM_SUIT_BANANA);
        top_n+=player.check_inventory(ITEM_SHOT_HOMING)+player.check_inventory(ITEM_TRANSLATOR)+player.check_inventory(ITEM_J_WING);

        bottom_n=player.leaves_max+player.cheese_max+7.0;

        ss.clear();ss.str("");ss<<"Percent complete: ";ss<<(top_n/bottom_n)*100.0;ss<<" %";text=ss.str();
        break;

    case SPECIAL_INFO_STAT_LEAVES_COLLECTED:
        ss.clear();ss.str("");ss<<"Leaves collected: ";ss<<player.leaves;ss<<"/";ss<<player.leaves_max;text=ss.str();
        break;

    case SPECIAL_INFO_STAT_CHEESE_COLLECTED:
        ss.clear();ss.str("");ss<<"Cheese collected: ";ss<<player.cheese;ss<<"/";ss<<player.cheese_max;text=ss.str();
        break;

    case SPECIAL_INFO_STAT_ENEMIES_STUNNED:
        ss.clear();ss.str("");ss<<"Enemies blasted: ";ss<<player.stat_enemies_stunned;text=ss.str();
        break;

    case SPECIAL_INFO_STAT_CHECKPOINTS_ACTIVATED:
        ss.clear();ss.str("");ss<<"Checkpoints activated: ";ss<<player.stat_checkpoints_activated;text=ss.str();
        break;

    case SPECIAL_INFO_STAT_AMMO_PICKED_UP:
        ss.clear();ss.str("");ss<<"Ammo boxes collected: ";ss<<player.stat_ammo_picked_up;text=ss.str();
        break;

    case SPECIAL_INFO_STAT_SHOTS_FIRED:
        ss.clear();ss.str("");ss<<"Shots fired: ";ss<<player.stat_shots_fired;text=ss.str();
        break;

    case SPECIAL_INFO_STAT_TRIGGERS_SHOT:
        ss.clear();ss.str("");ss<<"Levers shot: ";ss<<player.stat_triggers_shot;text=ss.str();
        break;

    case SPECIAL_INFO_STAT_ACCURACY:
        if(player.stat_shots_fired>0){
            accuracy=100*((double)(player.stat_enemies_stunned+player.stat_triggers_shot)/player.stat_shots_fired);
        }
        ss.clear();ss.str("");ss<<"Accuracy: ";ss<<accuracy;ss<<" %";text=ss.str();
        break;

    case SPECIAL_INFO_STAT_DEATHS_TOTAL:
        ss.clear();ss.str("");ss<<"Total deaths: ";ss<<player.stat_deaths_enemies+player.stat_deaths_traps+player.stat_deaths_drowning;text=ss.str();
        break;

    case SPECIAL_INFO_STAT_DEATHS_ENEMIES:
        ss.clear();ss.str("");ss<<"Deaths by enemy: ";ss<<player.stat_deaths_enemies;text=ss.str();
        break;

    case SPECIAL_INFO_STAT_DEATHS_TRAPS:
        ss.clear();ss.str("");ss<<"Deaths by trap: ";ss<<player.stat_deaths_traps;text=ss.str();
        break;

    case SPECIAL_INFO_STAT_DEATHS_DROWNING:
        ss.clear();ss.str("");ss<<"Deaths by drowning: ";ss<<player.stat_deaths_drowning;text=ss.str();
        break;

    case SPECIAL_INFO_STAT_LEVELS_REPLAYED:
        ss.clear();ss.str("");ss<<"Levels replayed: ";ss<<player.stat_levels_replayed;text=ss.str();
        break;

    case SPECIAL_INFO_STAT_TOTAL_JUMPS:
        ss.clear();ss.str("");ss<<"Number of jumps: ";ss<<player.stat_total_jumps;text=ss.str();
        break;

    case SPECIAL_INFO_STAT_SECONDS_PLAYING:
        seconds=player.stat_seconds_playing;
        minutes=0;
        hours=0;
        for(int i=seconds;i>=60;){
            minutes++;
            i-=60;
            if(i<60){
                seconds=i;
            }
        }
        for(int i=minutes;i>=60;){
            hours++;
            i-=60;
            if(i<60){
                minutes=i;
            }
        }
        temp="Time spent playing: ";
        if(hours<10){
            ss.clear();ss.str("");ss<<"0";temp+=ss.str();
        }
        ss.clear();ss.str("");ss<<hours;temp+=ss.str();
        ss.clear();ss.str("");ss<<":";temp+=ss.str();

        if(minutes<10){
            ss.clear();ss.str("");ss<<"0";temp+=ss.str();
        }
        ss.clear();ss.str("");ss<<minutes;temp+=ss.str();
        ss.clear();ss.str("");ss<<":";temp+=ss.str();

        if(seconds<10){
            ss.clear();ss.str("");ss<<"0";temp+=ss.str();
        }
        ss.clear();ss.str("");ss<<seconds;temp+=ss.str();

        text=temp;
        break;

    case SPECIAL_INFO_STAT_SECONDS_TRAVELING:
        seconds=player.stat_seconds_traveling;
        minutes=0;
        hours=0;
        for(int i=seconds;i>=60;){
            minutes++;
            i-=60;
            if(i<60){
                seconds=i;
            }
        }
        for(int i=minutes;i>=60;){
            hours++;
            i-=60;
            if(i<60){
                minutes=i;
            }
        }
        temp="Time spent traveling: ";
        if(hours<10){
            ss.clear();ss.str("");ss<<"0";temp+=ss.str();
        }
        ss.clear();ss.str("");ss<<hours;temp+=ss.str();
        ss.clear();ss.str("");ss<<":";temp+=ss.str();

        if(minutes<10){
            ss.clear();ss.str("");ss<<"0";temp+=ss.str();
        }
        ss.clear();ss.str("");ss<<minutes;temp+=ss.str();
        ss.clear();ss.str("");ss<<":";temp+=ss.str();

        if(seconds<10){
            ss.clear();ss.str("");ss<<"0";temp+=ss.str();
        }
        ss.clear();ss.str("");ss<<seconds;temp+=ss.str();

        text=temp;
        break;

    case SPECIAL_INFO_STAT_SECONDS_MENUS:
        seconds=player.stat_seconds_menus;
        minutes=0;
        hours=0;
        for(int i=seconds;i>=60;){
            minutes++;
            i-=60;
            if(i<60){
                seconds=i;
            }
        }
        for(int i=minutes;i>=60;){
            hours++;
            i-=60;
            if(i<60){
                minutes=i;
            }
        }
        temp="Time spent on menus: ";
        if(hours<10){
            ss.clear();ss.str("");ss<<"0";temp+=ss.str();
        }
        ss.clear();ss.str("");ss<<hours;temp+=ss.str();
        ss.clear();ss.str("");ss<<":";temp+=ss.str();

        if(minutes<10){
            ss.clear();ss.str("");ss<<"0";temp+=ss.str();
        }
        ss.clear();ss.str("");ss<<minutes;temp+=ss.str();
        ss.clear();ss.str("");ss<<":";temp+=ss.str();

        if(seconds<10){
            ss.clear();ss.str("");ss<<"0";temp+=ss.str();
        }
        ss.clear();ss.str("");ss<<seconds;temp+=ss.str();

        text=temp;
        break;

    case SPECIAL_INFO_STAT_LEVERS_PULLED:
        ss.clear();ss.str("");ss<<"Levers pulled: ";ss<<player.stat_levers_pulled;text=ss.str();
        break;

    case SPECIAL_INFO_STAT_SECONDS_RIDING_COWS:
        seconds=player.stat_seconds_riding_cows;
        minutes=0;
        hours=0;
        for(int i=seconds;i>=60;){
            minutes++;
            i-=60;
            if(i<60){
                seconds=i;
            }
        }
        for(int i=minutes;i>=60;){
            hours++;
            i-=60;
            if(i<60){
                minutes=i;
            }
        }
        temp="Time spent riding cows: ";
        if(hours<10){
            ss.clear();ss.str("");ss<<"0";temp+=ss.str();
        }
        ss.clear();ss.str("");ss<<hours;temp+=ss.str();
        ss.clear();ss.str("");ss<<":";temp+=ss.str();

        if(minutes<10){
            ss.clear();ss.str("");ss<<"0";temp+=ss.str();
        }
        ss.clear();ss.str("");ss<<minutes;temp+=ss.str();
        ss.clear();ss.str("");ss<<":";temp+=ss.str();

        if(seconds<10){
            ss.clear();ss.str("");ss<<"0";temp+=ss.str();
        }
        ss.clear();ss.str("");ss<<seconds;temp+=ss.str();

        text=temp;
        break;

    case SPECIAL_INFO_STAT_FARTHEST_FALL:
        ss.clear();ss.str("");ss<<"Farthest fall: ";ss<<player.stat_farthest_fall/24;ss<<" feet";text=ss.str();
        break;

    case SPECIAL_INFO_STAT_WATER_SHOT:
        ss.clear();ss.str("");ss<<"Bodies of water assaulted: ";ss<<player.stat_water_shot;text=ss.str();
        break;

    case SPECIAL_INFO_STAT_BOSSES_DEFEATED:
        ss.clear();ss.str("");ss<<"Bosses defeated: ";ss<<player.stat_bosses_defeated;text=ss.str();
        break;

    case SPECIAL_INFO_STAT_ITEMS_MOVED:
        ss.clear();ss.str("");ss<<"Inventory items moved: ";ss<<player.stat_items_moved;text=ss.str();
        break;

    case SPECIAL_INFO_LEVEL_STAT_LEAVES:
        which_window=window_manager.which_window_open();
        if(which_window>=WHICH_WINDOW_LEVEL_INFO && which_window<WHICH_WINDOW_OTHER){
            ss.clear();ss.str("");ss<<"Leaves collected: ";ss<<windows_level_info[which_window-WHICH_WINDOW_LEVEL_INFO].leaves;ss<<"/";ss<<windows_level_info[which_window-WHICH_WINDOW_LEVEL_INFO].leaves_max;text=ss.str();
        }
        break;

    case SPECIAL_INFO_LEVEL_STAT_CHEESE:
        which_window=window_manager.which_window_open();
        if(which_window>=WHICH_WINDOW_LEVEL_INFO && which_window<WHICH_WINDOW_OTHER){
            ss.clear();ss.str("");ss<<"Cheese collected: ";ss<<windows_level_info[which_window-WHICH_WINDOW_LEVEL_INFO].cheese;ss<<"/";ss<<windows_level_info[which_window-WHICH_WINDOW_LEVEL_INFO].cheese_max;text=ss.str();
        }
        break;

    case SPECIAL_INFO_LEVEL_STAT_TOTAL_TIME:
        which_window=window_manager.which_window_open();
        if(which_window>=WHICH_WINDOW_LEVEL_INFO && which_window<WHICH_WINDOW_OTHER){
            seconds=windows_level_info[which_window-WHICH_WINDOW_LEVEL_INFO].seconds_total_time;
            minutes=0;
            hours=0;
            for(int i=seconds;i>=60;){
                minutes++;
                i-=60;
                if(i<60){
                    seconds=i;
                }
            }
            for(int i=minutes;i>=60;){
                hours++;
                i-=60;
                if(i<60){
                    minutes=i;
                }
            }
            temp="Time spent in level: ";
            if(hours<10){
                ss.clear();ss.str("");ss<<"0";temp+=ss.str();
            }
            ss.clear();ss.str("");ss<<hours;temp+=ss.str();
            ss.clear();ss.str("");ss<<":";temp+=ss.str();

            if(minutes<10){
                ss.clear();ss.str("");ss<<"0";temp+=ss.str();
            }
            ss.clear();ss.str("");ss<<minutes;temp+=ss.str();
            ss.clear();ss.str("");ss<<":";temp+=ss.str();

            if(seconds<10){
                ss.clear();ss.str("");ss<<"0";temp+=ss.str();
            }
            ss.clear();ss.str("");ss<<seconds;temp+=ss.str();

            text=temp;
        }
        break;

    case SPECIAL_INFO_LEVEL_STAT_BEST_TIME:
        which_window=window_manager.which_window_open();
        if(which_window>=WHICH_WINDOW_LEVEL_INFO && which_window<WHICH_WINDOW_OTHER){
            seconds=windows_level_info[which_window-WHICH_WINDOW_LEVEL_INFO].seconds_best_time;
            minutes=0;
            hours=0;

            if(seconds!=4294967295){
                for(int i=seconds;i>=60;){
                    minutes++;
                    i-=60;
                    if(i<60){
                        seconds=i;
                    }
                }
                for(int i=minutes;i>=60;){
                    hours++;
                    i-=60;
                    if(i<60){
                        minutes=i;
                    }
                }
                temp="Best time: ";
                if(hours<10){
                    ss.clear();ss.str("");ss<<"0";temp+=ss.str();
                }
                ss.clear();ss.str("");ss<<hours;temp+=ss.str();
                ss.clear();ss.str("");ss<<":";temp+=ss.str();

                if(minutes<10){
                    ss.clear();ss.str("");ss<<"0";temp+=ss.str();
                }
                ss.clear();ss.str("");ss<<minutes;temp+=ss.str();
                ss.clear();ss.str("");ss<<":";temp+=ss.str();

                if(seconds<10){
                    ss.clear();ss.str("");ss<<"0";temp+=ss.str();
                }
                ss.clear();ss.str("");ss<<seconds;temp+=ss.str();
            }
            else{
                temp="Best time: None";
            }

            text=temp;
        }
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_COLLECT_ALL_LEAVES_IN_LEVEL:
        ss.clear();ss.str("");ss<<"Cleaning Up the Yard";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_COLLECT_ALL_CHEESE_IN_LEVEL:
        ss.clear();ss.str("");ss<<"Picking Up the Cheese";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_PLAY_LEVEL_30_MINUTES:
        ss.clear();ss.str("");ss<<"Budding Explorer";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_PLAY_LEVEL_1_HOUR:
        ss.clear();ss.str("");ss<<"Professor Dyer, Geologist";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_PLAY_LEVEL_2_HOURS:
        ss.clear();ss.str("");ss<<"Professor Jones, Archaeologist";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_COLLECT_HALF_OF_LEAVES:
        ss.clear();ss.str("");ss<<"Baggin' Leaves";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_COLLECT_HALF_OF_CHEESE:
        ss.clear();ss.str("");ss<<"Cheese Lover";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_COLLECT_ALL_LEAVES:
        ss.clear();ss.str("");ss<<"Baggin' Some Serious Leaves";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_COLLECT_ALL_CHEESE:
        ss.clear();ss.str("");ss<<"Cheese Connoisseur";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_KILL_ENEMY_1:
        ss.clear();ss.str("");ss<<"1 Enemy Blasted";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_KILL_ENEMY_25:
        ss.clear();ss.str("");ss<<"25 Enemies Blasted";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_KILL_ENEMY_50:
        ss.clear();ss.str("");ss<<"50 Enemies Blasted";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_KILL_ENEMY_100:
        ss.clear();ss.str("");ss<<"100 Enemies Blasted";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_KILL_ENEMY_250:
        ss.clear();ss.str("");ss<<"250 Enemies Blasted";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_KILL_ENEMY_500:
        ss.clear();ss.str("");ss<<"500 Enemies Blasted";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_KILL_ENEMY_750:
        ss.clear();ss.str("");ss<<"750 Enemies Blasted";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_KILL_ENEMY_1000:
        ss.clear();ss.str("");ss<<"1000 Enemies Blasted";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_ACTIVATE_CHECKPOINT_1:
        ss.clear();ss.str("");ss<<"1 Checkpoint Activated";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_ACTIVATE_CHECKPOINT_5:
        ss.clear();ss.str("");ss<<"5 Checkpoints Activated";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_ACTIVATE_CHECKPOINT_10:
        ss.clear();ss.str("");ss<<"10 Checkpoints Activated";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_ACTIVATE_CHECKPOINT_25:
        ss.clear();ss.str("");ss<<"25 Checkpoints Activated";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_ACTIVATE_CHECKPOINT_50:
        ss.clear();ss.str("");ss<<"50 Checkpoints Activated";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_ACTIVATE_CHECKPOINT_75:
        ss.clear();ss.str("");ss<<"75 Checkpoints Activated";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_ACTIVATE_CHECKPOINT_100:
        ss.clear();ss.str("");ss<<"100 Checkpoints Activated";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_COLLECT_AMMO_1:
        ss.clear();ss.str("");ss<<"1 Ammo Box Collected";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_COLLECT_AMMO_10:
        ss.clear();ss.str("");ss<<"10 Ammo Boxes Collected";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_COLLECT_AMMO_25:
        ss.clear();ss.str("");ss<<"25 Ammo Boxes Collected";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_COLLECT_AMMO_50:
        ss.clear();ss.str("");ss<<"50 Ammo Boxes Collected";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_COLLECT_AMMO_100:
        ss.clear();ss.str("");ss<<"100 Ammo Boxes Collected";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_COLLECT_AMMO_250:
        ss.clear();ss.str("");ss<<"250 Ammo Boxes Collected";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_COLLECT_AMMO_500:
        ss.clear();ss.str("");ss<<"500 Ammo Boxes Collected";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_SHOOT_1:
        ss.clear();ss.str("");ss<<"1 Shot Fired";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_SHOOT_25:
        ss.clear();ss.str("");ss<<"25 Shots Fired";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_SHOOT_50:
        ss.clear();ss.str("");ss<<"50 Shots Fired";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_SHOOT_100:
        ss.clear();ss.str("");ss<<"100 Shots Fired";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_SHOOT_250:
        ss.clear();ss.str("");ss<<"250 Shots Fired";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_SHOOT_500:
        ss.clear();ss.str("");ss<<"500 Shots Fired";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_SHOOT_750:
        ss.clear();ss.str("");ss<<"750 Shots Fired";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_SHOOT_1000:
        ss.clear();ss.str("");ss<<"1000 Shots Fired";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_DIE_1:
        ss.clear();ss.str("");ss<<"1 Death";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_DIE_5:
        ss.clear();ss.str("");ss<<"5 Deaths";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_DIE_10:
        ss.clear();ss.str("");ss<<"10 Deaths";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_DIE_25:
        ss.clear();ss.str("");ss<<"25 Deaths";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_DIE_50:
        ss.clear();ss.str("");ss<<"50 Deaths";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_DIE_75:
        ss.clear();ss.str("");ss<<"75 Deaths";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_DIE_100:
        ss.clear();ss.str("");ss<<"100 Deaths";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_DIE_TO_ENEMY_1:
        ss.clear();ss.str("");ss<<"1 Death to Enemies";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_DIE_TO_ENEMY_5:
        ss.clear();ss.str("");ss<<"5 Deaths to Enemies";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_DIE_TO_ENEMY_10:
        ss.clear();ss.str("");ss<<"10 Deaths to Enemies";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_DIE_TO_ENEMY_25:
        ss.clear();ss.str("");ss<<"25 Deaths to Enemies";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_DIE_TO_TRAP_1:
        ss.clear();ss.str("");ss<<"1 Death to Traps";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_DIE_TO_TRAP_5:
        ss.clear();ss.str("");ss<<"5 Deaths to Traps";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_DIE_TO_TRAP_10:
        ss.clear();ss.str("");ss<<"10 Deaths to Traps";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_DIE_TO_TRAP_25:
        ss.clear();ss.str("");ss<<"25 Deaths to Traps";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_REPLAY_LEVEL_1:
        ss.clear();ss.str("");ss<<"1 Level Replayed";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_REPLAY_LEVEL_5:
        ss.clear();ss.str("");ss<<"5 Levels Replayed";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_REPLAY_LEVEL_10:
        ss.clear();ss.str("");ss<<"10 Levels Replayed";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_REPLAY_LEVEL_25:
        ss.clear();ss.str("");ss<<"25 Levels Replayed";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_REPLAY_LEVEL_50:
        ss.clear();ss.str("");ss<<"50 Levels Replayed";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_REPLAY_LEVEL_75:
        ss.clear();ss.str("");ss<<"75 Levels Replayed";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_REPLAY_LEVEL_100:
        ss.clear();ss.str("");ss<<"100 Levels Replayed";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_JUMP_1:
        ss.clear();ss.str("");ss<<"1 Jump";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_JUMP_25:
        ss.clear();ss.str("");ss<<"25 Jumps";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_JUMP_50:
        ss.clear();ss.str("");ss<<"50 Jumps";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_JUMP_100:
        ss.clear();ss.str("");ss<<"100 Jumps";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_JUMP_250:
        ss.clear();ss.str("");ss<<"250 Jumps";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_JUMP_500:
        ss.clear();ss.str("");ss<<"500 Jumps";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_JUMP_750:
        ss.clear();ss.str("");ss<<"750 Jumps";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_JUMP_1000:
        ss.clear();ss.str("");ss<<"1000 Jumps";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_PLAY_1_HOUR:
        ss.clear();ss.str("");ss<<"Keeping Busy";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_PLAY_2_HOURS:
        ss.clear();ss.str("");ss<<"Working Hard";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_PLAY_5_HOURS:
        ss.clear();ss.str("");ss<<"Determined";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_PLAY_10_HOURS:
        ss.clear();ss.str("");ss<<"Dedicated";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_TRAVEL_1_HOUR:
        ss.clear();ss.str("");ss<<"World Traveler";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_MENUS_1_HOUR:
        ss.clear();ss.str("");ss<<"The Game is More Fun if You Click Play";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_PULL_LEVER_1:
        ss.clear();ss.str("");ss<<"1 Lever Pulled";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_PULL_LEVER_5:
        ss.clear();ss.str("");ss<<"5 Levers Pulled";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_PULL_LEVER_10:
        ss.clear();ss.str("");ss<<"10 Levers Pulled";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_PULL_LEVER_25:
        ss.clear();ss.str("");ss<<"25 Levers Pulled";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_PULL_LEVER_50:
        ss.clear();ss.str("");ss<<"50 Levers Pulled";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_PULL_LEVER_75:
        ss.clear();ss.str("");ss<<"75 Levers Pulled";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_PULL_LEVER_100:
        ss.clear();ss.str("");ss<<"100 Levers Pulled";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_RIDE_COW_1_SECOND:
        ss.clear();ss.str("");ss<<"Cowboy";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_RIDE_COW_1_MINUTE:
        ss.clear();ss.str("");ss<<"Keep Moovin' Moovin' Moovin'";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_RIDE_COW_2_MINUTES:
        ss.clear();ss.str("");ss<<"Joy of the Ride";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_RIDE_COW_3_MINUTES:
        ss.clear();ss.str("");ss<<"Can't Stop Riding";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_RIDE_COW_4_MINUTES:
        ss.clear();ss.str("");ss<<"Cow Expert";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_RIDE_COW_5_MINUTES:
        ss.clear();ss.str("");ss<<"Having an Udderly Good Time";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_WATER_SHOT_1:
        ss.clear();ss.str("");ss<<"Water Doesn't Die";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_WATER_SHOT_5:
        ss.clear();ss.str("");ss<<"Water is Immortal";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_WATER_SHOT_10:
        ss.clear();ss.str("");ss<<"Water Hater";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_WATER_SHOT_25:
        ss.clear();ss.str("");ss<<"Seriously, Water Needs to Die";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_HUBERTS_HOUSE_1_HOUR:
        ss.clear();ss.str("");ss<<"Staying Home";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_ENTER_WORLDMAP:
        ss.clear();ss.str("");ss<<"It's a Dangerous Business";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_ENTER_ANY_LEVEL:
        ss.clear();ss.str("");ss<<"Go Time";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_DEFEAT_ALL_BOSSES_BUT_FINAL:
        ss.clear();ss.str("");ss<<"There's One More to Go!";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_DEFEAT_ALL_BOSSES:
        ss.clear();ss.str("");ss<<"Hubert Saves the Day!";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_BEAT_ANY_LEVEL:
        ss.clear();ss.str("");ss<<"First Blood";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_BEAT_GAME:
        ss.clear();ss.str("");ss<<"Game Over?";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_BEAT_LEVEL_NO_KILLS:
        ss.clear();ss.str("");ss<<"Practically Pacifist";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_BEAT_LEVEL_NO_SHOTS:
        ss.clear();ss.str("");ss<<"Pacifist";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_JOKE_ITEM:
        ss.clear();ss.str("");ss<<"Everything INCLUDING the Kitchen Sink";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_FIRE_50_SHOTS_IN_LEVEL:
        ss.clear();ss.str("");ss<<"Gun Nut";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_MOVE_INVENTORY_ITEM_5:
        ss.clear();ss.str("");ss<<"Organizer";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_CHANGE_AN_OPTION:
        ss.clear();ss.str("");ss<<"Configurator";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_VIEW_CREDITS:
        ss.clear();ss.str("");ss<<"I'd Like to Thank the Little People";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_DIE_AFTER_GOAL:
        ss.clear();ss.str("");ss<<"How Did That Happen?";text=ss.str();
        break;

    case SPECIAL_INFO_ACHIEVEMENT+ACHIEVEMENT_MAX_DEATH_BOUNCES:
        ss.clear();ss.str("");ss<<"Bouncing About";text=ss.str();
        break;

    }

    special_info_return_data return_data;
    return_data.command=command;
    return_data.text=text;

    return return_data;
}
