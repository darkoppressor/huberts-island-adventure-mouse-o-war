/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "score.h"
#include "enumerations.h"

using namespace std;

uint64_t SCORES_ITEMS[ITEM_END];

uint64_t SCORES_ACHIEVEMENTS[ACHIEVEMENT_END];

void prepare_scores(){
    SCORES_ITEMS[ITEM_LEAF]=4;
    SCORES_ITEMS[ITEM_CHEESE]=24;
    SCORES_ITEMS[ITEM_AMMO]=0;
    SCORES_ITEMS[ITEM_SWIMMING_GEAR]=32;
    SCORES_ITEMS[ITEM_KEY_RED]=32;
    SCORES_ITEMS[ITEM_KEY_BLUE]=32;
    SCORES_ITEMS[ITEM_KEY_GREEN]=32;
    SCORES_ITEMS[ITEM_KEY_YELLOW]=32;
    SCORES_ITEMS[ITEM_KEY_ORANGE]=32;
    SCORES_ITEMS[ITEM_KEY_PURPLE]=32;
    SCORES_ITEMS[ITEM_TOWEL]=32;
    SCORES_ITEMS[ITEM_SINK]=0;
    SCORES_ITEMS[ITEM_J_BALLOON]=0;
    SCORES_ITEMS[ITEM_AMMO_BARREL]=0;
    SCORES_ITEMS[ITEM_CANDY]=1;
    SCORES_ITEMS[ITEM_KEY_GRAY]=32;
    SCORES_ITEMS[ITEM_KEY_BROWN]=32;
    SCORES_ITEMS[ITEM_KEY_BLACK]=32;
    SCORES_ITEMS[ITEM_KEY_PINK]=32;
    SCORES_ITEMS[ITEM_KEY_CYAN]=32;
    SCORES_ITEMS[ITEM_SUIT_DEADLY_WATER]=32;
    SCORES_ITEMS[ITEM_SUIT_SHARP]=32;
    SCORES_ITEMS[ITEM_SUIT_BANANA]=32;
    SCORES_ITEMS[ITEM_SHOT_HOMING]=32;
    SCORES_ITEMS[ITEM_TRANSLATOR]=32;
    SCORES_ITEMS[ITEM_J_WING]=32;

    SCORES_ACHIEVEMENTS[ACHIEVEMENT_COLLECT_ALL_LEAVES_IN_LEVEL]=32;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_COLLECT_ALL_CHEESE_IN_LEVEL]=64;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_PLAY_LEVEL_30_MINUTES]=8;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_PLAY_LEVEL_1_HOUR]=16;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_PLAY_LEVEL_2_HOURS]=32;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_COLLECT_HALF_OF_LEAVES]=1024;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_COLLECT_HALF_OF_CHEESE]=1280;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_COLLECT_ALL_LEAVES]=8192;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_COLLECT_ALL_CHEESE]=9001;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_KILL_ENEMY_1]=4;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_KILL_ENEMY_25]=8;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_KILL_ENEMY_50]=16;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_KILL_ENEMY_100]=24;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_KILL_ENEMY_250]=32;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_KILL_ENEMY_500]=48;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_KILL_ENEMY_750]=64;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_KILL_ENEMY_1000]=128;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_ACTIVATE_CHECKPOINT_1]=4;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_ACTIVATE_CHECKPOINT_5]=8;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_ACTIVATE_CHECKPOINT_10]=16;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_ACTIVATE_CHECKPOINT_25]=24;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_ACTIVATE_CHECKPOINT_50]=32;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_ACTIVATE_CHECKPOINT_75]=48;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_ACTIVATE_CHECKPOINT_100]=64;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_COLLECT_AMMO_1]=4;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_COLLECT_AMMO_10]=8;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_COLLECT_AMMO_25]=16;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_COLLECT_AMMO_50]=24;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_COLLECT_AMMO_100]=32;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_COLLECT_AMMO_250]=48;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_COLLECT_AMMO_500]=64;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_SHOOT_1]=4;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_SHOOT_25]=8;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_SHOOT_50]=16;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_SHOOT_100]=24;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_SHOOT_250]=32;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_SHOOT_500]=48;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_SHOOT_750]=64;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_SHOOT_1000]=128;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_DIE_1]=4;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_DIE_5]=8;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_DIE_10]=16;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_DIE_25]=24;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_DIE_50]=32;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_DIE_75]=48;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_DIE_100]=64;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_DIE_TO_ENEMY_1]=4;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_DIE_TO_ENEMY_5]=8;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_DIE_TO_ENEMY_10]=16;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_DIE_TO_ENEMY_25]=24;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_DIE_TO_TRAP_1]=4;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_DIE_TO_TRAP_5]=8;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_DIE_TO_TRAP_10]=16;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_DIE_TO_TRAP_25]=24;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_REPLAY_LEVEL_1]=4;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_REPLAY_LEVEL_5]=8;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_REPLAY_LEVEL_10]=16;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_REPLAY_LEVEL_25]=24;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_REPLAY_LEVEL_50]=32;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_REPLAY_LEVEL_75]=64;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_REPLAY_LEVEL_100]=128;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_JUMP_1]=4;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_JUMP_25]=8;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_JUMP_50]=16;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_JUMP_100]=24;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_JUMP_250]=32;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_JUMP_500]=48;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_JUMP_750]=64;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_JUMP_1000]=128;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_PLAY_1_HOUR]=8;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_PLAY_2_HOURS]=16;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_PLAY_5_HOURS]=32;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_PLAY_10_HOURS]=64;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_TRAVEL_1_HOUR]=16;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_MENUS_1_HOUR]=16;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_PULL_LEVER_1]=4;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_PULL_LEVER_5]=8;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_PULL_LEVER_10]=16;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_PULL_LEVER_25]=24;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_PULL_LEVER_50]=32;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_PULL_LEVER_75]=48;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_PULL_LEVER_100]=64;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_RIDE_COW_1_SECOND]=4;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_RIDE_COW_1_MINUTE]=8;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_RIDE_COW_2_MINUTES]=16;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_RIDE_COW_3_MINUTES]=24;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_RIDE_COW_4_MINUTES]=32;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_RIDE_COW_5_MINUTES]=48;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_WATER_SHOT_1]=4;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_WATER_SHOT_5]=8;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_WATER_SHOT_10]=16;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_WATER_SHOT_25]=24;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_HUBERTS_HOUSE_1_HOUR]=16;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_ENTER_WORLDMAP]=4;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_ENTER_ANY_LEVEL]=4;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_DEFEAT_ALL_BOSSES_BUT_FINAL]=512;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_DEFEAT_ALL_BOSSES]=1024;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_BEAT_ANY_LEVEL]=8;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_BEAT_GAME]=16;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_BEAT_LEVEL_NO_KILLS]=16;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_BEAT_LEVEL_NO_SHOTS]=16;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_JOKE_ITEM]=16;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_FIRE_50_SHOTS_IN_LEVEL]=8;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_MOVE_INVENTORY_ITEM_5]=8;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_CHANGE_AN_OPTION]=8;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_VIEW_CREDITS]=8;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_DIE_AFTER_GOAL]=16;
    SCORES_ACHIEVEMENTS[ACHIEVEMENT_MAX_DEATH_BOUNCES]=16;
}
