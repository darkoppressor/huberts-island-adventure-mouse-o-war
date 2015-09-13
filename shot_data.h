/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef shot_data_h
#define shot_data_h

#include <vector>

class new_shot_data{
    public:
    double x;
    double y;
    double angle;
    short type;
    bool counts_as_trap;
    new_shot_data(double get_x,double get_y,double get_angle,short get_type,bool get_counts_as_trap);
};
extern std::vector<new_shot_data> new_shots;

//Shot types:

enum{
    SHOT_PLAYER,
    SHOT_PLAYER_SPLODE,
    SHOT_PLAYER_SPLOSION,
    SHOT_PLAYER_HOMING,
    SHOT_FRUIT,
    SHOT_LASER,
    SHOT_BALL,
    SHOT_ARROW,
    SHOT_SNOWBALL,
    SHOT_FIREBALL,
    SHOT_POISON,
    SHOT_HOMING_MISSILE,
    SHOT_BOSS_DEFEATED,
    SHOT_BOSS_HAUNTED_HOUSE,
    SHOT_BOSS_VOLCANO,
    SHOT_BOSS_PYRAMID,
    SHOT_BOSS_LIGHTHOUSE,
    SHOT_END
};

const short SHOT_TYPES_COUNT=SHOT_END;

const short SHOT_SPLOSION_W=96;
const short SHOT_SPLOSION_H=96;

const short SHOT_COST_PLAYER=1;
const short SHOT_COST_PLAYER_SPLODE=4;
const short SHOT_COST_PLAYER_HOMING=8;

struct shot_sprite_count_data{
    short sprites;
    double width;
    double height;
};

extern std::vector<shot_sprite_count_data> SHOT_SPRITE_COUNTS;

void prepare_shot_sprite_counts();

#endif
