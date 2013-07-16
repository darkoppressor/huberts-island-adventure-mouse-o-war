/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "shot_data.h"

using namespace std;

new_shot_data::new_shot_data(double get_x,double get_y,double get_angle,short get_type,bool get_counts_as_trap){
    x=get_x;
    y=get_y;
    angle=get_angle;
    type=get_type;
    counts_as_trap=get_counts_as_trap;
}
vector<new_shot_data> new_shots;

vector<shot_sprite_count_data> SHOT_SPRITE_COUNTS;

void prepare_shot_sprite_counts(){
    //Clear the shot sprite counts vector.
    SHOT_SPRITE_COUNTS.clear();

    for(short i=0;i<SHOT_TYPES_COUNT;i++){
        SHOT_SPRITE_COUNTS.push_back(shot_sprite_count_data());
    }

    //Player shot
    SHOT_SPRITE_COUNTS[SHOT_PLAYER].sprites=6;
    SHOT_SPRITE_COUNTS[SHOT_PLAYER].width=32;
    SHOT_SPRITE_COUNTS[SHOT_PLAYER].height=32;

    //Player shot splode
    SHOT_SPRITE_COUNTS[SHOT_PLAYER_SPLODE].sprites=6;
    SHOT_SPRITE_COUNTS[SHOT_PLAYER_SPLODE].width=16;
    SHOT_SPRITE_COUNTS[SHOT_PLAYER_SPLODE].height=16;

    //Player shot splosion
    SHOT_SPRITE_COUNTS[SHOT_PLAYER_SPLOSION].sprites=6;
    SHOT_SPRITE_COUNTS[SHOT_PLAYER_SPLOSION].width=SHOT_SPLOSION_W;
    SHOT_SPRITE_COUNTS[SHOT_PLAYER_SPLOSION].height=SHOT_SPLOSION_H;

    //Player shot homing
    SHOT_SPRITE_COUNTS[SHOT_PLAYER_HOMING].sprites=6;
    SHOT_SPRITE_COUNTS[SHOT_PLAYER_HOMING].width=16;
    SHOT_SPRITE_COUNTS[SHOT_PLAYER_HOMING].height=16;

    //Fruit
    SHOT_SPRITE_COUNTS[SHOT_FRUIT].sprites=6;
    SHOT_SPRITE_COUNTS[SHOT_FRUIT].width=17;
    SHOT_SPRITE_COUNTS[SHOT_FRUIT].height=18;

    //Laser
    SHOT_SPRITE_COUNTS[SHOT_LASER].sprites=7;
    SHOT_SPRITE_COUNTS[SHOT_LASER].width=16;
    SHOT_SPRITE_COUNTS[SHOT_LASER].height=16;

    //Ball
    SHOT_SPRITE_COUNTS[SHOT_BALL].sprites=4;
    SHOT_SPRITE_COUNTS[SHOT_BALL].width=32;
    SHOT_SPRITE_COUNTS[SHOT_BALL].height=32;

    //Arrow
    SHOT_SPRITE_COUNTS[SHOT_ARROW].sprites=4;
    SHOT_SPRITE_COUNTS[SHOT_ARROW].width=32;
    SHOT_SPRITE_COUNTS[SHOT_ARROW].height=7;

    //Snowball
    SHOT_SPRITE_COUNTS[SHOT_SNOWBALL].sprites=6;
    SHOT_SPRITE_COUNTS[SHOT_SNOWBALL].width=17;
    SHOT_SPRITE_COUNTS[SHOT_SNOWBALL].height=18;

    //Fireball
    SHOT_SPRITE_COUNTS[SHOT_FIREBALL].sprites=6;
    SHOT_SPRITE_COUNTS[SHOT_FIREBALL].width=32;
    SHOT_SPRITE_COUNTS[SHOT_FIREBALL].height=32;

    //Poison
    SHOT_SPRITE_COUNTS[SHOT_POISON].sprites=7;
    SHOT_SPRITE_COUNTS[SHOT_POISON].width=16;
    SHOT_SPRITE_COUNTS[SHOT_POISON].height=16;

    //Homing missile
    SHOT_SPRITE_COUNTS[SHOT_HOMING_MISSILE].sprites=5;
    SHOT_SPRITE_COUNTS[SHOT_HOMING_MISSILE].width=32;
    SHOT_SPRITE_COUNTS[SHOT_HOMING_MISSILE].height=7;

    //Boss Defeated
    SHOT_SPRITE_COUNTS[SHOT_BOSS_DEFEATED].sprites=6;
    SHOT_SPRITE_COUNTS[SHOT_BOSS_DEFEATED].width=128;
    SHOT_SPRITE_COUNTS[SHOT_BOSS_DEFEATED].height=128;

    //Boss Haunted House
    SHOT_SPRITE_COUNTS[SHOT_BOSS_HAUNTED_HOUSE].sprites=6;
    SHOT_SPRITE_COUNTS[SHOT_BOSS_HAUNTED_HOUSE].width=17;
    SHOT_SPRITE_COUNTS[SHOT_BOSS_HAUNTED_HOUSE].height=18;

    //Boss Volcano
    SHOT_SPRITE_COUNTS[SHOT_BOSS_VOLCANO].sprites=6;
    SHOT_SPRITE_COUNTS[SHOT_BOSS_VOLCANO].width=17;
    SHOT_SPRITE_COUNTS[SHOT_BOSS_VOLCANO].height=18;

    //Boss Pyramid
    SHOT_SPRITE_COUNTS[SHOT_BOSS_PYRAMID].sprites=4;
    SHOT_SPRITE_COUNTS[SHOT_BOSS_PYRAMID].width=24;
    SHOT_SPRITE_COUNTS[SHOT_BOSS_PYRAMID].height=24;

    //Boss Lighthouse
    SHOT_SPRITE_COUNTS[SHOT_BOSS_LIGHTHOUSE].sprites=6;
    SHOT_SPRITE_COUNTS[SHOT_BOSS_LIGHTHOUSE].width=12;
    SHOT_SPRITE_COUNTS[SHOT_BOSS_LIGHTHOUSE].height=12;
}
