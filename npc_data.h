/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef npc_data_h
#define npc_data_h

#include "image_data.h"

#include <vector>
#include <string>

class new_npc_data{
    public:
    double x;
    double y;
    short type;
    new_npc_data(double get_x,double get_y,short get_type);
};
extern std::vector<new_npc_data> new_npcs;

std::string return_npc_name(int i);

std::string return_npc_description(int i);

image_data* return_npc_image(int i);

//NPC Types:

enum{
    //0 is reserved for "no npc".
    NPC_RESERVED,
    NPC_SLIME,
    NPC_COW,
    NPC_BEE,
    NPC_REPLICATOR,
    NPC_FISH,
    NPC_MONKEY,
    NPC_LASER_SPINNER,
    NPC_PENGUIN,
    NPC_FISH_ON_ICE,
    NPC_PENGUIN_HARMLESS,
    NPC_FISH_ON_ICE_HARMLESS,
    NPC_SWORD_KNIGHT,
    NPC_BALL_KNIGHT,
    NPC_ARCHER,
    NPC_SCARECROW,
    NPC_PUMPKIN,
    NPC_ZOMBIE_COW,
    NPC_MUMMY_COW,
    NPC_GHOST,
    NPC_SPIDER,
    NPC_CAT,
    NPC_SPIDER_WALKING,
    NPC_RAM,
    NPC_EAGLE,
    NPC_SNOWMAN,
    NPC_FIREBALL,
    NPC_DRAGON_WHELP,
    NPC_FIREBALL_SHOOTER_LEFT,
    NPC_FIREBALL_SHOOTER_RIGHT,
    NPC_FIREBALL_SHOOTER_UP,
    NPC_FIREBALL_SHOOTER_DOWN,
    NPC_DUST_DEVIL,
    NPC_SCORPION,
    NPC_SPHINX_LEFT,
    NPC_SPHINX_RIGHT,
    NPC_SHARK,
    NPC_HENCHMOUSE,
    NPC_MECHSUIT_MOUSE,
    NPC_SALLY,
    NPC_BOUNCING_COW,
    NPC_BOUNCING_BALL,
    NPC_SEA_TURTLE,
    NPC_STARFISH,
    NPC_BOSS_CASTLE,
    NPC_BOSS_HAUNTED_HOUSE,
    NPC_BOSS_VOLCANO,
    NPC_BOSS_PYRAMID,
    NPC_BOSS_LIGHTHOUSE,
    NPC_END
};

//NPC Special Attacks:

const short SPECIAL_ATTACK_NONE=-1;

enum{
    SPECIAL_ATTACK_COW_MOO,
    SPECIAL_ATTACK_BEE_DIVEBOMB,
    SPECIAL_ATTACK_REPLICATE,
    SPECIAL_ATTACK_MONKEY_THROW_FRUIT,
    SPECIAL_ATTACK_LASER_SPINNER_FIRE_LASER,
    SPECIAL_ATTACK_SWORD_KNIGHT_SWORD_SLASH,
    SPECIAL_ATTACK_BALL_KNIGHT_THROW_BALL,
    SPECIAL_ATTACK_ARCHER_FIRE_ARROW,
    SPECIAL_ATTACK_SCARECROW_WHACK,
    SPECIAL_ATTACK_GHOST_HIDE,
    SPECIAL_ATTACK_CAT_POUNCE,
    SPECIAL_ATTACK_RAM_CHARGE,
    SPECIAL_ATTACK_EAGLE_DIVEBOMB,
    SPECIAL_ATTACK_SNOWMAN_THROW_SNOWBALL,
    SPECIAL_ATTACK_DRAGON_WHELP_SHOOT_FIREBALL,
    SPECIAL_ATTACK_FIREBALL_SHOOTER_SHOOT_LEFT,
    SPECIAL_ATTACK_FIREBALL_SHOOTER_SHOOT_RIGHT,
    SPECIAL_ATTACK_FIREBALL_SHOOTER_SHOOT_UP,
    SPECIAL_ATTACK_FIREBALL_SHOOTER_SHOOT_DOWN,
    SPECIAL_ATTACK_SCORPION_SHOOT_POISON,
    SPECIAL_ATTACK_SPHINX_SUCK_LEFT,
    SPECIAL_ATTACK_SPHINX_SUCK_RIGHT,
    SPECIAL_ATTACK_HENCHMOUSE_SOUND_ALARM,
    SPECIAL_ATTACK_MECHSUIT_MOUSE_FIRE_MISSILE,
    SPECIAL_ATTACK_BOSS_HAUNTED_HOUSE,
    SPECIAL_ATTACK_BOSS_VOLCANO,
    SPECIAL_ATTACK_BOSS_PYRAMID,
    SPECIAL_ATTACK_BOSS_LIGHTHOUSE,
    SPECIAL_ATTACK_ZOMBIE_COW_MOO,
    SPECIAL_ATTACK_MUMMY_COW_MOO
};

//The speed applied by sucking things.
const double SUCK_SPEED=2.5;

struct npc_sprite_count_data{
    short walk_sprites;
    short jump_sprites;
    short swim_sprites;
    short fly_sprites;
    short special_attack_sprites;
    short dead_sprites;
    double width;
    double height;
    double dead_width;
    double dead_height;
    double special_width;
    double special_height;
};

extern std::vector<npc_sprite_count_data> NPC_SPRITE_COUNTS;

void prepare_npc_sprite_counts();

#endif
