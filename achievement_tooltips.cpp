/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "achievement_tooltips.h"
#include "enumerations.h"

using namespace std;

string achievement_tooltips[ACHIEVEMENT_END];

void set_achievement_tooltips(){
    for(int i=ACHIEVEMENT_BEGIN;i<ACHIEVEMENT_END;i++){
        achievement_tooltips[i]="";
    }

    achievement_tooltips[ACHIEVEMENT_COLLECT_ALL_LEAVES_IN_LEVEL]="Collect all of the\nleaves in a single level.";
    achievement_tooltips[ACHIEVEMENT_COLLECT_ALL_CHEESE_IN_LEVEL]="Collect all of the\ncheese in a single level.";
    achievement_tooltips[ACHIEVEMENT_PLAY_LEVEL_30_MINUTES]="Spend 30 minutes\nin a single level.";
    achievement_tooltips[ACHIEVEMENT_PLAY_LEVEL_1_HOUR]="Spend 1 hour\nin a single level.";
    achievement_tooltips[ACHIEVEMENT_PLAY_LEVEL_2_HOURS]="Spend 2 hours\nin a single level.";
    achievement_tooltips[ACHIEVEMENT_COLLECT_HALF_OF_LEAVES]="Collect half of all\nthe leaves in the game.";
    achievement_tooltips[ACHIEVEMENT_COLLECT_HALF_OF_CHEESE]="Collect half of all\nthe cheese in the game.";
    achievement_tooltips[ACHIEVEMENT_COLLECT_ALL_LEAVES]="Collect all of\nthe leaves in the game.";
    achievement_tooltips[ACHIEVEMENT_COLLECT_ALL_CHEESE]="Collect all of\nthe cheese in the game.";
    achievement_tooltips[ACHIEVEMENT_PLAY_1_HOUR]="Spend 1 hour playing the game.";
    achievement_tooltips[ACHIEVEMENT_PLAY_2_HOURS]="Spend 2 hours playing the game.";
    achievement_tooltips[ACHIEVEMENT_PLAY_5_HOURS]="Spend 5 hours playing the game.";
    achievement_tooltips[ACHIEVEMENT_PLAY_10_HOURS]="Spend 10 hours playing the game.";
    achievement_tooltips[ACHIEVEMENT_TRAVEL_1_HOUR]="Spend 1 hour traveling\non the world map.";
    achievement_tooltips[ACHIEVEMENT_MENUS_1_HOUR]="Spend 1 hour in the menus.";
    achievement_tooltips[ACHIEVEMENT_RIDE_COW_1_SECOND]="Ride a cow.";
    achievement_tooltips[ACHIEVEMENT_RIDE_COW_1_MINUTE]="Ride a cow for 1 minute.";
    achievement_tooltips[ACHIEVEMENT_RIDE_COW_2_MINUTES]="Ride a cow for 2 minutes.";
    achievement_tooltips[ACHIEVEMENT_RIDE_COW_3_MINUTES]="Ride a cow for 3 minutes.";
    achievement_tooltips[ACHIEVEMENT_RIDE_COW_4_MINUTES]="Ride a cow for 4 minutes.";
    achievement_tooltips[ACHIEVEMENT_RIDE_COW_5_MINUTES]="Ride a cow for 5 minutes.";
    achievement_tooltips[ACHIEVEMENT_WATER_SHOT_1]="Shoot water once.";
    achievement_tooltips[ACHIEVEMENT_WATER_SHOT_5]="Shoot water 5 times.";
    achievement_tooltips[ACHIEVEMENT_WATER_SHOT_10]="Shoot water 10 times.";
    achievement_tooltips[ACHIEVEMENT_WATER_SHOT_25]="Shoot water 25 times.";
    achievement_tooltips[ACHIEVEMENT_HUBERTS_HOUSE_1_HOUR]="Spend 1 hour in Hubert's house.";
    achievement_tooltips[ACHIEVEMENT_ENTER_WORLDMAP]="Embark on the world map.";
    achievement_tooltips[ACHIEVEMENT_ENTER_ANY_LEVEL]="Enter any level.";
    achievement_tooltips[ACHIEVEMENT_DEFEAT_ALL_BOSSES_BUT_FINAL]="Defeat all of the bosses,\nexcept for Dakkar.";
    achievement_tooltips[ACHIEVEMENT_DEFEAT_ALL_BOSSES]="Defeat all of the bosses.";
    achievement_tooltips[ACHIEVEMENT_BEAT_ANY_LEVEL]="Beat a level,\nany level.";
    achievement_tooltips[ACHIEVEMENT_BEAT_GAME]="\"Beat\" the game by reaching\nthe goal in the lighthouse.";
    achievement_tooltips[ACHIEVEMENT_BEAT_LEVEL_NO_KILLS]="Beat a level without\nshooting any enemies.";
    achievement_tooltips[ACHIEVEMENT_BEAT_LEVEL_NO_SHOTS]="Beat a level without\nfiring a single shot.";
    achievement_tooltips[ACHIEVEMENT_JOKE_ITEM]="Find the kitchen sink.";
    achievement_tooltips[ACHIEVEMENT_FIRE_50_SHOTS_IN_LEVEL]="Fire 50 shots in a single\nplaythrough of a level.";
    achievement_tooltips[ACHIEVEMENT_MOVE_INVENTORY_ITEM_5]="Move an item around\nin your inventory 5 times.";
    achievement_tooltips[ACHIEVEMENT_CHANGE_AN_OPTION]="Change an option.";
    achievement_tooltips[ACHIEVEMENT_VIEW_CREDITS]="View the credits.";
    achievement_tooltips[ACHIEVEMENT_DIE_AFTER_GOAL]="Manage to die after\nreaching the goal in a level.";
    achievement_tooltips[ACHIEVEMENT_MAX_DEATH_BOUNCES]="Bounce many times after dying.";
}
