/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "profile.h"
#include "world.h"
#include "score.h"

using namespace std;

void Profile::earn_achievement(int achievement){
    if(player.game_mode==GAME_MODE_SP_ADVENTURE){
        //The achievement has been earned.
        player.achievements[achievement]=true;

        //Create an achievement notification slider.
        sliders.push_back(Slider(achievement));

        player.gain_score(SCORES_ACHIEVEMENTS[achievement],player.x+player.w/2.0,player.y,false);

        save_achievements();
    }
}

void Profile::update_achievements(int achievement){
    //If there is a profile.
    if(player.name!="\x1F"){
        if(player.game_mode==GAME_MODE_SP_ADVENTURE){
            //If an achievement has been passed.
            if(achievement!=-1){
                //If the achievement was not yet earned.
                if(!player.achievements[achievement]){
                    earn_achievement(achievement);
                }
            }
            //If no achievement was passed.
            else{
                //Update count achievements:
                for(int i=ACHIEVEMENT_BEGIN;i<ACHIEVEMENT_END;i++){
                    //If the achievement has not yet been earned.
                    if(!player.achievements[i]){

                        //*****************//
                        // Level-specific: //
                        //*****************//

                        if(i==ACHIEVEMENT_COLLECT_ALL_LEAVES_IN_LEVEL){
                            if(!player.on_worldmap() && level.leaves!=0 && player.current_level_leaves>=level.leaves){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_COLLECT_ALL_CHEESE_IN_LEVEL){
                            if(!player.on_worldmap() && level.cheese!=0 && player.current_level_cheese>=level.cheese){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_PLAY_LEVEL_30_MINUTES){
                            if(!player.on_worldmap() && player.seconds_total_current_level>=1800){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_PLAY_LEVEL_1_HOUR){
                            if(!player.on_worldmap() && player.seconds_total_current_level>=3600){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_PLAY_LEVEL_2_HOURS){
                            if(!player.on_worldmap() && player.seconds_total_current_level>=7200){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_HUBERTS_HOUSE_1_HOUR){
                            if(player.current_level==15 && player.seconds_total_current_level>=3600){
                                earn_achievement(i);
                            }
                        }

                        //**************************//
                        // Leaves/cheese collected: //
                        //**************************//

                        else if(i==ACHIEVEMENT_COLLECT_HALF_OF_LEAVES){
                            if(player.leaves>=player.leaves_max/2.0){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_COLLECT_HALF_OF_CHEESE){
                            if(player.cheese>=player.cheese_max/2.0){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_COLLECT_ALL_LEAVES){
                            if(player.leaves>=player.leaves_max){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_COLLECT_ALL_CHEESE){
                            if(player.cheese>=player.cheese_max){
                                earn_achievement(i);
                            }
                        }

                        //********//
                        // Kills: //
                        //********//

                        else if(i==ACHIEVEMENT_KILL_ENEMY_1){
                            if(player.stat_enemies_stunned>=1){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_KILL_ENEMY_25){
                            if(player.stat_enemies_stunned>=25){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_KILL_ENEMY_50){
                            if(player.stat_enemies_stunned>=50){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_KILL_ENEMY_100){
                            if(player.stat_enemies_stunned>=100){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_KILL_ENEMY_250){
                            if(player.stat_enemies_stunned>=250){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_KILL_ENEMY_500){
                            if(player.stat_enemies_stunned>=500){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_KILL_ENEMY_750){
                            if(player.stat_enemies_stunned>=750){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_KILL_ENEMY_1000){
                            if(player.stat_enemies_stunned>=1000){
                                earn_achievement(i);
                            }
                        }

                        //************************//
                        // Checkpoints activated: //
                        //************************//

                        else if(i==ACHIEVEMENT_ACTIVATE_CHECKPOINT_1){
                            if(player.stat_checkpoints_activated>=1){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_ACTIVATE_CHECKPOINT_5){
                            if(player.stat_checkpoints_activated>=5){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_ACTIVATE_CHECKPOINT_10){
                            if(player.stat_checkpoints_activated>=10){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_ACTIVATE_CHECKPOINT_25){
                            if(player.stat_checkpoints_activated>=25){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_ACTIVATE_CHECKPOINT_50){
                            if(player.stat_checkpoints_activated>=50){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_ACTIVATE_CHECKPOINT_75){
                            if(player.stat_checkpoints_activated>=75){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_ACTIVATE_CHECKPOINT_100){
                            if(player.stat_checkpoints_activated>=100){
                                earn_achievement(i);
                            }
                        }

                        //***********************//
                        // Ammo boxes collected: //
                        //***********************//

                        else if(i==ACHIEVEMENT_COLLECT_AMMO_1){
                            if(player.stat_ammo_picked_up>=1){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_COLLECT_AMMO_10){
                            if(player.stat_ammo_picked_up>=10){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_COLLECT_AMMO_25){
                            if(player.stat_ammo_picked_up>=25){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_COLLECT_AMMO_50){
                            if(player.stat_ammo_picked_up>=50){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_COLLECT_AMMO_100){
                            if(player.stat_ammo_picked_up>=100){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_COLLECT_AMMO_250){
                            if(player.stat_ammo_picked_up>=250){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_COLLECT_AMMO_500){
                            if(player.stat_ammo_picked_up>=500){
                                earn_achievement(i);
                            }
                        }

                        //**************//
                        // Shots fired: //
                        //**************//

                        else if(i==ACHIEVEMENT_SHOOT_1){
                            if(player.stat_shots_fired>=1){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_SHOOT_25){
                            if(player.stat_shots_fired>=25){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_SHOOT_50){
                            if(player.stat_shots_fired>=50){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_SHOOT_100){
                            if(player.stat_shots_fired>=100){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_SHOOT_250){
                            if(player.stat_shots_fired>=250){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_SHOOT_500){
                            if(player.stat_shots_fired>=500){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_SHOOT_750){
                            if(player.stat_shots_fired>=750){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_SHOOT_1000){
                            if(player.stat_shots_fired>=1000){
                                earn_achievement(i);
                            }
                        }

                        //*********//
                        // Deaths: //
                        //*********//

                        else if(i==ACHIEVEMENT_DIE_1){
                            if(player.stat_deaths_enemies+player.stat_deaths_traps+player.stat_deaths_drowning>=1){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_DIE_5){
                            if(player.stat_deaths_enemies+player.stat_deaths_traps+player.stat_deaths_drowning>=5){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_DIE_10){
                            if(player.stat_deaths_enemies+player.stat_deaths_traps+player.stat_deaths_drowning>=10){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_DIE_25){
                            if(player.stat_deaths_enemies+player.stat_deaths_traps+player.stat_deaths_drowning>=25){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_DIE_50){
                            if(player.stat_deaths_enemies+player.stat_deaths_traps+player.stat_deaths_drowning>=50){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_DIE_75){
                            if(player.stat_deaths_enemies+player.stat_deaths_traps+player.stat_deaths_drowning>=75){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_DIE_100){
                            if(player.stat_deaths_enemies+player.stat_deaths_traps+player.stat_deaths_drowning>=100){
                                earn_achievement(i);
                            }
                        }

                        //********************//
                        // Deaths to enemies: //
                        //********************//

                        else if(i==ACHIEVEMENT_DIE_TO_ENEMY_1){
                            if(player.stat_deaths_enemies>=1){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_DIE_TO_ENEMY_5){
                            if(player.stat_deaths_enemies>=5){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_DIE_TO_ENEMY_10){
                            if(player.stat_deaths_enemies>=10){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_DIE_TO_ENEMY_25){
                            if(player.stat_deaths_enemies>=25){
                                earn_achievement(i);
                            }
                        }

                        //******************//
                        // Deaths to traps: //
                        //******************//

                        else if(i==ACHIEVEMENT_DIE_TO_TRAP_1){
                            if(player.stat_deaths_traps>=1){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_DIE_TO_TRAP_5){
                            if(player.stat_deaths_traps>=5){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_DIE_TO_TRAP_10){
                            if(player.stat_deaths_traps>=10){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_DIE_TO_TRAP_25){
                            if(player.stat_deaths_traps>=25){
                                earn_achievement(i);
                            }
                        }

                        //******************//
                        // Levels replayed: //
                        //******************//

                        else if(i==ACHIEVEMENT_REPLAY_LEVEL_1){
                            if(player.stat_levels_replayed>=1){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_REPLAY_LEVEL_5){
                            if(player.stat_levels_replayed>=5){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_REPLAY_LEVEL_10){
                            if(player.stat_levels_replayed>=10){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_REPLAY_LEVEL_25){
                            if(player.stat_levels_replayed>=25){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_REPLAY_LEVEL_50){
                            if(player.stat_levels_replayed>=50){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_REPLAY_LEVEL_75){
                            if(player.stat_levels_replayed>=75){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_REPLAY_LEVEL_100){
                            if(player.stat_levels_replayed>=100){
                                earn_achievement(i);
                            }
                        }

                        //********//
                        // Jumps: //
                        //********//

                        else if(i==ACHIEVEMENT_JUMP_1){
                            if(player.stat_total_jumps>=1){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_JUMP_25){
                            if(player.stat_total_jumps>=25){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_JUMP_50){
                            if(player.stat_total_jumps>=50){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_JUMP_100){
                            if(player.stat_total_jumps>=100){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_JUMP_250){
                            if(player.stat_total_jumps>=250){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_JUMP_500){
                            if(player.stat_total_jumps>=500){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_JUMP_750){
                            if(player.stat_total_jumps>=750){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_JUMP_1000){
                            if(player.stat_total_jumps>=1000){
                                earn_achievement(i);
                            }
                        }

                        //**************//
                        // Time played: //
                        //**************//

                        else if(i==ACHIEVEMENT_PLAY_1_HOUR){
                            if(player.stat_seconds_playing>=3600){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_PLAY_2_HOURS){
                            if(player.stat_seconds_playing>=7200){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_PLAY_5_HOURS){
                            if(player.stat_seconds_playing>=18000){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_PLAY_10_HOURS){
                            if(player.stat_seconds_playing>=36000){
                                earn_achievement(i);
                            }
                        }

                        //***********************//
                        // Time spent traveling: //
                        //***********************//

                        else if(i==ACHIEVEMENT_TRAVEL_1_HOUR){
                            if(player.stat_seconds_traveling>=3600){
                                earn_achievement(i);
                            }
                        }

                        //**********************//
                        // Time spent on menus: //
                        //**********************//

                        else if(i==ACHIEVEMENT_MENUS_1_HOUR){
                            if(player.stat_seconds_menus>=3600){
                                earn_achievement(i);
                            }
                        }

                        //****************//
                        // Levers pulled: //
                        //****************//

                        else if(i==ACHIEVEMENT_PULL_LEVER_1){
                            if(player.stat_levers_pulled>=1){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_PULL_LEVER_5){
                            if(player.stat_levers_pulled>=5){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_PULL_LEVER_10){
                            if(player.stat_levers_pulled>=10){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_PULL_LEVER_25){
                            if(player.stat_levers_pulled>=25){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_PULL_LEVER_50){
                            if(player.stat_levers_pulled>=50){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_PULL_LEVER_75){
                            if(player.stat_levers_pulled>=75){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_PULL_LEVER_100){
                            if(player.stat_levers_pulled>=100){
                                earn_achievement(i);
                            }
                        }

                        //*************************//
                        // Time spent riding cows: //
                        //*************************//

                        else if(i==ACHIEVEMENT_RIDE_COW_1_SECOND){
                            if(player.stat_seconds_riding_cows>=1){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_RIDE_COW_1_MINUTE){
                            if(player.stat_seconds_riding_cows>=60){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_RIDE_COW_2_MINUTES){
                            if(player.stat_seconds_riding_cows>=120){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_RIDE_COW_3_MINUTES){
                            if(player.stat_seconds_riding_cows>=180){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_RIDE_COW_4_MINUTES){
                            if(player.stat_seconds_riding_cows>=240){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_RIDE_COW_5_MINUTES){
                            if(player.stat_seconds_riding_cows>=300){
                                earn_achievement(i);
                            }
                        }

                        //***********************//
                        // Bodies of water shot: //
                        //***********************//

                        else if(i==ACHIEVEMENT_WATER_SHOT_1){
                            if(player.stat_water_shot>=1){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_WATER_SHOT_5){
                            if(player.stat_water_shot>=5){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_WATER_SHOT_10){
                            if(player.stat_water_shot>=10){
                                earn_achievement(i);
                            }
                        }
                        else if(i==ACHIEVEMENT_WATER_SHOT_25){
                            if(player.stat_water_shot>=25){
                                earn_achievement(i);
                            }
                        }

                        //**************//
                        // Items moved: //
                        //**************//

                        else if(i==ACHIEVEMENT_MOVE_INVENTORY_ITEM_5){
                            if(player.stat_items_moved>=5){
                                earn_achievement(i);
                            }
                        }

                        //*************************//
                        // Shots fired this level: //
                        //*************************//

                        else if(i==ACHIEVEMENT_FIRE_50_SHOTS_IN_LEVEL){
                            if(player.special_count_shots_this_level>=50){
                                earn_achievement(i);
                            }
                        }
                    }
                }
            }
        }
    }
}
