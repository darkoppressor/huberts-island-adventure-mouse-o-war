/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "npc_data.h"
#include "world.h"

#include <cmath>

using namespace std;

new_npc_data::new_npc_data(double get_x,double get_y,short get_type){
    x=get_x;
    y=get_y;
    type=get_type;
}
vector<new_npc_data> new_npcs;

vector<npc_sprite_count_data> NPC_SPRITE_COUNTS;

string return_npc_name(int i){
    if(fabs(i)==NPC_SLIME){
        return "Goopster";
    }
    else if(fabs(i)==NPC_COW){
        return "Mooer";
    }
    else if(fabs(i)==NPC_BEE){
        return "Buzzer";
    }
    else if(fabs(i)==NPC_REPLICATOR){
        return "Replicator";
    }
    else if(fabs(i)==NPC_FISH){
        return "Fishie";
    }
    else if(fabs(i)==NPC_MONKEY){
        return "Simian";
    }
    else if(fabs(i)==NPC_LASER_SPINNER){
        return "Laser Blaster";
    }
    else if(fabs(i)==NPC_PENGUIN){
        return "Tux";
    }
    else if(fabs(i)==NPC_FISH_ON_ICE){
        return "Frozen Fish";
    }
    else if(fabs(i)==NPC_PENGUIN_HARMLESS){
        return "Tux (Harmless)";
    }
    else if(fabs(i)==NPC_FISH_ON_ICE_HARMLESS){
        return "Frozen Fish (Harmless)";
    }
    else if(fabs(i)==NPC_SWORD_KNIGHT){
        return "Blade Knight";
    }
    else if(fabs(i)==NPC_BALL_KNIGHT){
        return "Flail Knight";
    }
    else if(fabs(i)==NPC_ARCHER){
        return "Archer";
    }
    else if(fabs(i)==NPC_SCARECROW){
        return "Scarecrow";
    }
    else if(fabs(i)==NPC_PUMPKIN){
        return "Pumpkin";
    }
    else if(fabs(i)==NPC_ZOMBIE_COW){
        return "Zombie Mooer";
    }
    else if(fabs(i)==NPC_MUMMY_COW){
        return "Mummy Mooer";
    }
    else if(fabs(i)==NPC_GHOST){
        return "Ghastly Mooer";
    }
    else if(fabs(i)==NPC_SPIDER){
        return "Spider";
    }
    else if(fabs(i)==NPC_CAT){
        return "Zoom";
    }
    else if(fabs(i)==NPC_SPIDER_WALKING){
        return "Spider";
    }
    else if(fabs(i)==NPC_RAM){
        return "Ram";
    }
    else if(fabs(i)==NPC_EAGLE){
        return "Eagle";
    }
    else if(fabs(i)==NPC_SNOWMAN){
        return "Snowman";
    }
    else if(fabs(i)==NPC_FIREBALL){
        return "Fireball";
    }
    else if(fabs(i)==NPC_DRAGON_WHELP){
        return "Whelpling";
    }
    else if(fabs(i)==NPC_FIREBALL_SHOOTER_LEFT){
        return "Flamethrower";
    }
    else if(fabs(i)==NPC_FIREBALL_SHOOTER_RIGHT){
        return "Flamethrower";
    }
    else if(fabs(i)==NPC_FIREBALL_SHOOTER_UP){
        return "Flamethrower";
    }
    else if(fabs(i)==NPC_FIREBALL_SHOOTER_DOWN){
        return "Flamethrower";
    }
    else if(fabs(i)==NPC_DUST_DEVIL){
        return "Dust Devil";
    }
    else if(fabs(i)==NPC_SCORPION){
        return "Scorpion";
    }
    else if(fabs(i)==NPC_SPHINX_LEFT){
        return "Sphinx";
    }
    else if(fabs(i)==NPC_SPHINX_RIGHT){
        return "Sphinx";
    }
    else if(fabs(i)==NPC_SHARK){
        return "Shark";
    }
    else if(fabs(i)==NPC_HENCHMOUSE){
        return "Henchmouse";
    }
    else if(fabs(i)==NPC_MECHSUIT_MOUSE){
        return "Mechsuit Mouse";
    }
    else if(fabs(i)==NPC_SALLY){
        return "Sally";
    }
    else if(fabs(i)==NPC_BOUNCING_COW){
        return "Bouncing Mooer";
    }
    else if(fabs(i)==NPC_BOUNCING_BALL){
        return "Bouncy Ball";
    }
    else if(fabs(i)==NPC_SEA_TURTLE){
        return "Sea Turtle";
    }
    else if(fabs(i)==NPC_STARFISH){
        return "Starfish";
    }
    else if(fabs(i)==NPC_BOSS_CASTLE){
        return "Boss - Castle";
    }
    else if(fabs(i)==NPC_BOSS_HAUNTED_HOUSE){
        return "Boss - Haunted House";
    }
    else if(fabs(i)==NPC_BOSS_VOLCANO){
        return "Boss - Volcano";
    }
    else if(fabs(i)==NPC_BOSS_PYRAMID){
        return "Boss - Pyramid";
    }
    else if(fabs(i)==NPC_BOSS_LIGHTHOUSE){
        return "Boss - Lighthouse";
    }

    string not_npc="";
    ss.clear();ss.str("");ss<<i;ss<<" is not an npc!";not_npc=ss.str();
    return not_npc;
}

string return_npc_description(int i){
    if(fabs(i)==NPC_SLIME){
        return "Slow, dumb, not much\nof a threat.";
    }
    else if(fabs(i)==NPC_COW){
        return "Friendly. Can be ridden!";
    }
    else if(fabs(i)==NPC_BEE){
        return "Buzzes about, occasionally\nswooping down at Hubert.";
    }
    else if(fabs(i)==NPC_REPLICATOR){
        return "";
    }
    else if(fabs(i)==NPC_FISH){
        return "A friendly fish.\nFollows Hubert around if he\nswims by.";
    }
    else if(fabs(i)==NPC_MONKEY){
        return "Tosses bananas at Hubert.";
    }
    else if(fabs(i)==NPC_LASER_SPINNER){
        return "";
    }
    else if(fabs(i)==NPC_PENGUIN){
        return "Slides around very quickly.";
    }
    else if(fabs(i)==NPC_FISH_ON_ICE){
        return "This angry fish is trapped\nin an ice block. Watch out!";
    }
    else if(fabs(i)==NPC_PENGUIN_HARMLESS){
        return "";
    }
    else if(fabs(i)==NPC_FISH_ON_ICE_HARMLESS){
        return "";
    }
    else if(fabs(i)==NPC_SWORD_KNIGHT){
        return "Marches about, swinging its\nsword at nearby adventurers.";
    }
    else if(fabs(i)==NPC_BALL_KNIGHT){
        return "Marches about, throwing spike\nballs at adventurers above it.";
    }
    else if(fabs(i)==NPC_ARCHER){
        return "Fires arrows at Hubert if he\ncomes into range.";
    }
    else if(fabs(i)==NPC_SCARECROW){
        return "Takes a whack at Hubert if he\ngets too close.";
    }
    else if(fabs(i)==NPC_PUMPKIN){
        return "Bounces about, only following\nHubert when his back is turned.";
    }
    else if(fabs(i)==NPC_ZOMBIE_COW){
        return "Follows Hubert relentlessly.\nLikes brains.";
    }
    else if(fabs(i)==NPC_MUMMY_COW){
        return "Slower than the "+return_npc_name(NPC_ZOMBIE_COW)+".";
    }
    else if(fabs(i)==NPC_GHOST){
        return "Floats around, passing through\nwalls. Only follows Hubert if\nhis back is turned.";
    }
    else if(fabs(i)==NPC_SPIDER){
        return "Can be found running quickly\nalong the ground, or crawling\nup and down walls.";
    }
    else if(fabs(i)==NPC_CAT){
        return "Zooms about. When she goes into\na pounce, look out!";
    }
    else if(fabs(i)==NPC_SPIDER_WALKING){
        return "";
    }
    else if(fabs(i)==NPC_RAM){
        return "Very dangerous\nwhen charging.";
    }
    else if(fabs(i)==NPC_EAGLE){
        return "Hangs around its perch until\nHubert comes into view.";
    }
    else if(fabs(i)==NPC_SNOWMAN){
        return "Jumps around, throwing snowballs\nat Hubert.";
    }
    else if(fabs(i)==NPC_FIREBALL){
        return "Flies all around.\nHot to the touch.";
    }
    else if(fabs(i)==NPC_DRAGON_WHELP){
        return "Beware its downward\nfireball attack!";
    }
    else if(fabs(i)==NPC_FIREBALL_SHOOTER_LEFT){
        return "";
    }
    else if(fabs(i)==NPC_FIREBALL_SHOOTER_RIGHT){
        return "";
    }
    else if(fabs(i)==NPC_FIREBALL_SHOOTER_UP){
        return "";
    }
    else if(fabs(i)==NPC_FIREBALL_SHOOTER_DOWN){
        return "";
    }
    else if(fabs(i)==NPC_DUST_DEVIL){
        return "Fairly slow until\nit detects Hubert.";
    }
    else if(fabs(i)==NPC_SCORPION){
        return "Fires poison at Hubert.";
    }
    else if(fabs(i)==NPC_SPHINX_LEFT){
        return "";
    }
    else if(fabs(i)==NPC_SPHINX_RIGHT){
        return "";
    }
    else if(fabs(i)==NPC_SHARK){
        return "One of the most dangerous creatures\nfound in the sea around the island.";
    }
    else if(fabs(i)==NPC_HENCHMOUSE){
        return "Usually terrified of Hubert.\nHowever, they will hunt Hubert\nmercilessly if an alarm sounds!";
    }
    else if(fabs(i)==NPC_MECHSUIT_MOUSE){
        return "A "+return_npc_name(NPC_HENCHMOUSE)+" in a specially\ndesigned hovercraft capable\nof firing homing missiles.";
    }
    else if(fabs(i)==NPC_SALLY){
        return "";
    }
    else if(fabs(i)==NPC_BOUNCING_COW){
        return "";
    }
    else if(fabs(i)==NPC_BOUNCING_BALL){
        return "";
    }
    else if(fabs(i)==NPC_SEA_TURTLE){
        return "A friendly sea creature.\nWhen it surfaces, Hubert can\nsurf on its back!";
    }
    else if(fabs(i)==NPC_STARFISH){
        return "Another friendly creature\nof the sea.";
    }
    else if(fabs(i)==NPC_BOSS_CASTLE){
        return "";
    }
    else if(fabs(i)==NPC_BOSS_HAUNTED_HOUSE){
        return "";
    }
    else if(fabs(i)==NPC_BOSS_VOLCANO){
        return "";
    }
    else if(fabs(i)==NPC_BOSS_PYRAMID){
        return "";
    }
    else if(fabs(i)==NPC_BOSS_LIGHTHOUSE){
        return "";
    }

    string not_npc="";
    ss.clear();ss.str("");ss<<i;ss<<" is not an npc!";not_npc=ss.str();
    return not_npc;
}

image_data* return_npc_image(int i){
    if(fabs(i)==NPC_SLIME){
        return &image.sprite_sheet_npc_slime;
    }
    else if(fabs(i)==NPC_COW){
        return &image.sprite_sheet_npc_cow;
    }
    else if(fabs(i)==NPC_BEE){
        return &image.sprite_sheet_npc_bee;
    }
    else if(fabs(i)==NPC_REPLICATOR){
        return &image.sprite_sheet_npc_replicator;
    }
    else if(fabs(i)==NPC_FISH){
        return &image.sprite_sheet_npc_fish;
    }
    else if(fabs(i)==NPC_MONKEY){
        return &image.sprite_sheet_npc_monkey;
    }
    else if(fabs(i)==NPC_LASER_SPINNER){
        return &image.sprite_sheet_npc_laser_spinner;
    }
    else if(fabs(i)==NPC_PENGUIN){
        return &image.sprite_sheet_npc_penguin;
    }
    else if(fabs(i)==NPC_FISH_ON_ICE){
        return &image.sprite_sheet_npc_fish_on_ice;
    }
    else if(fabs(i)==NPC_PENGUIN_HARMLESS){
        return &image.sprite_sheet_npc_penguin_harmless;
    }
    else if(fabs(i)==NPC_FISH_ON_ICE_HARMLESS){
        return &image.sprite_sheet_npc_fish_on_ice_harmless;
    }
    else if(fabs(i)==NPC_SWORD_KNIGHT){
        return &image.sprite_sheet_npc_sword_knight;
    }
    else if(fabs(i)==NPC_BALL_KNIGHT){
        return &image.sprite_sheet_npc_ball_knight;
    }
    else if(fabs(i)==NPC_ARCHER){
        return &image.sprite_sheet_npc_archer;
    }
    else if(fabs(i)==NPC_SCARECROW){
        return &image.sprite_sheet_npc_scarecrow;
    }
    else if(fabs(i)==NPC_PUMPKIN){
        return &image.sprite_sheet_npc_pumpkin;
    }
    else if(fabs(i)==NPC_ZOMBIE_COW){
        return &image.sprite_sheet_npc_zombie_cow;
    }
    else if(fabs(i)==NPC_MUMMY_COW){
        return &image.sprite_sheet_npc_mummy_cow;
    }
    else if(fabs(i)==NPC_GHOST){
        return &image.sprite_sheet_npc_ghost;
    }
    else if(fabs(i)==NPC_SPIDER){
        return &image.sprite_sheet_npc_spider;
    }
    else if(fabs(i)==NPC_CAT){
        return &image.sprite_sheet_npc_cat;
    }
    else if(fabs(i)==NPC_SPIDER_WALKING){
        return &image.sprite_sheet_npc_spider_walking;
    }
    else if(fabs(i)==NPC_RAM){
        return &image.sprite_sheet_npc_ram;
    }
    else if(fabs(i)==NPC_EAGLE){
        return &image.sprite_sheet_npc_eagle;
    }
    else if(fabs(i)==NPC_SNOWMAN){
        return &image.sprite_sheet_npc_snowman;
    }
    else if(fabs(i)==NPC_FIREBALL){
        return &image.sprite_sheet_npc_fireball;
    }
    else if(fabs(i)==NPC_DRAGON_WHELP){
        return &image.sprite_sheet_npc_dragon_whelp;
    }
    else if(fabs(i)==NPC_FIREBALL_SHOOTER_LEFT){
        return &image.sprite_sheet_npc_fireball_shooter_left;
    }
    else if(fabs(i)==NPC_FIREBALL_SHOOTER_RIGHT){
        return &image.sprite_sheet_npc_fireball_shooter_right;
    }
    else if(fabs(i)==NPC_FIREBALL_SHOOTER_UP){
        return &image.sprite_sheet_npc_fireball_shooter_up;
    }
    else if(fabs(i)==NPC_FIREBALL_SHOOTER_DOWN){
        return &image.sprite_sheet_npc_fireball_shooter_down;
    }
    else if(fabs(i)==NPC_DUST_DEVIL){
        return &image.sprite_sheet_npc_dust_devil;
    }
    else if(fabs(i)==NPC_SCORPION){
        return &image.sprite_sheet_npc_scorpion;
    }
    else if(fabs(i)==NPC_SPHINX_LEFT){
        return &image.sprite_sheet_npc_sphinx;
    }
    else if(fabs(i)==NPC_SPHINX_RIGHT){
        return &image.sprite_sheet_npc_sphinx;
    }
    else if(fabs(i)==NPC_SHARK){
        return &image.sprite_sheet_npc_shark;
    }
    else if(fabs(i)==NPC_HENCHMOUSE){
        return &image.sprite_sheet_npc_henchmouse;
    }
    else if(fabs(i)==NPC_MECHSUIT_MOUSE){
        return &image.sprite_sheet_npc_mechsuit_mouse;
    }
    else if(fabs(i)==NPC_SALLY){
        return &image.sprite_sheet_npc_sally;
    }
    else if(fabs(i)==NPC_BOUNCING_COW){
        return &image.sprite_sheet_npc_cow;
    }
    else if(fabs(i)==NPC_BOUNCING_BALL){
        return &image.sprite_sheet_npc_bouncing_ball;
    }
    else if(fabs(i)==NPC_SEA_TURTLE){
        return &image.sprite_sheet_npc_sea_turtle;
    }
    else if(fabs(i)==NPC_STARFISH){
        return &image.sprite_sheet_npc_starfish;
    }
    else if(fabs(i)==NPC_BOSS_CASTLE){
        return &image.sprite_sheet_npc_boss_castle;
    }
    else if(fabs(i)==NPC_BOSS_HAUNTED_HOUSE){
        return &image.sprite_sheet_npc_boss_haunted_house;
    }
    else if(fabs(i)==NPC_BOSS_VOLCANO){
        return &image.sprite_sheet_npc_boss_volcano;
    }
    else if(fabs(i)==NPC_BOSS_PYRAMID){
        return &image.sprite_sheet_npc_boss_pyramid;
    }
    else if(fabs(i)==NPC_BOSS_LIGHTHOUSE){
        return &image.sprite_sheet_npc_boss_lighthouse;
    }

    return NULL;
}

void prepare_npc_sprite_counts(){
    //Clear the npc sprite counts vector.
    NPC_SPRITE_COUNTS.clear();

    for(short i=0;i<NPC_END;i++){
        NPC_SPRITE_COUNTS.push_back(npc_sprite_count_data());
    }

    //Slime
    NPC_SPRITE_COUNTS[NPC_SLIME].walk_sprites=8;
    NPC_SPRITE_COUNTS[NPC_SLIME].jump_sprites=8;
    NPC_SPRITE_COUNTS[NPC_SLIME].swim_sprites=8;
    NPC_SPRITE_COUNTS[NPC_SLIME].fly_sprites=0;
    NPC_SPRITE_COUNTS[NPC_SLIME].special_attack_sprites=0;
    NPC_SPRITE_COUNTS[NPC_SLIME].dead_sprites=3;
    NPC_SPRITE_COUNTS[NPC_SLIME].width=32;
    NPC_SPRITE_COUNTS[NPC_SLIME].height=29;
    NPC_SPRITE_COUNTS[NPC_SLIME].dead_width=32;
    NPC_SPRITE_COUNTS[NPC_SLIME].dead_height=42;
    NPC_SPRITE_COUNTS[NPC_SLIME].special_width=0;
    NPC_SPRITE_COUNTS[NPC_SLIME].special_height=0;

    //Cow
    NPC_SPRITE_COUNTS[NPC_COW].walk_sprites=6;
    NPC_SPRITE_COUNTS[NPC_COW].jump_sprites=6;
    NPC_SPRITE_COUNTS[NPC_COW].swim_sprites=6;
    NPC_SPRITE_COUNTS[NPC_COW].fly_sprites=0;
    NPC_SPRITE_COUNTS[NPC_COW].special_attack_sprites=6;
    NPC_SPRITE_COUNTS[NPC_COW].dead_sprites=3;
    NPC_SPRITE_COUNTS[NPC_COW].width=81;
    NPC_SPRITE_COUNTS[NPC_COW].height=44;
    NPC_SPRITE_COUNTS[NPC_COW].dead_width=36;
    NPC_SPRITE_COUNTS[NPC_COW].dead_height=44;
    NPC_SPRITE_COUNTS[NPC_COW].special_width=81;
    NPC_SPRITE_COUNTS[NPC_COW].special_height=44;

    //Bee
    NPC_SPRITE_COUNTS[NPC_BEE].walk_sprites=0;
    NPC_SPRITE_COUNTS[NPC_BEE].jump_sprites=0;
    NPC_SPRITE_COUNTS[NPC_BEE].swim_sprites=0;
    NPC_SPRITE_COUNTS[NPC_BEE].fly_sprites=1;
    NPC_SPRITE_COUNTS[NPC_BEE].special_attack_sprites=0;
    NPC_SPRITE_COUNTS[NPC_BEE].dead_sprites=3;
    NPC_SPRITE_COUNTS[NPC_BEE].width=27;
    NPC_SPRITE_COUNTS[NPC_BEE].height=27;
    NPC_SPRITE_COUNTS[NPC_BEE].dead_width=29;
    NPC_SPRITE_COUNTS[NPC_BEE].dead_height=30;
    NPC_SPRITE_COUNTS[NPC_BEE].special_width=27;
    NPC_SPRITE_COUNTS[NPC_BEE].special_height=27;

    //Replicator
    NPC_SPRITE_COUNTS[NPC_REPLICATOR].walk_sprites=2;
    NPC_SPRITE_COUNTS[NPC_REPLICATOR].jump_sprites=2;
    NPC_SPRITE_COUNTS[NPC_REPLICATOR].swim_sprites=2;
    NPC_SPRITE_COUNTS[NPC_REPLICATOR].fly_sprites=0;
    NPC_SPRITE_COUNTS[NPC_REPLICATOR].special_attack_sprites=1;
    NPC_SPRITE_COUNTS[NPC_REPLICATOR].dead_sprites=1;
    NPC_SPRITE_COUNTS[NPC_REPLICATOR].width=42;
    NPC_SPRITE_COUNTS[NPC_REPLICATOR].height=29;
    NPC_SPRITE_COUNTS[NPC_REPLICATOR].dead_width=42;
    NPC_SPRITE_COUNTS[NPC_REPLICATOR].dead_height=29;
    NPC_SPRITE_COUNTS[NPC_REPLICATOR].special_width=42;
    NPC_SPRITE_COUNTS[NPC_REPLICATOR].special_height=29;

    //Fish
    NPC_SPRITE_COUNTS[NPC_FISH].walk_sprites=8;
    NPC_SPRITE_COUNTS[NPC_FISH].jump_sprites=8;
    NPC_SPRITE_COUNTS[NPC_FISH].swim_sprites=8;
    NPC_SPRITE_COUNTS[NPC_FISH].fly_sprites=0;
    NPC_SPRITE_COUNTS[NPC_FISH].special_attack_sprites=0;
    NPC_SPRITE_COUNTS[NPC_FISH].dead_sprites=0;
    NPC_SPRITE_COUNTS[NPC_FISH].width=28;
    NPC_SPRITE_COUNTS[NPC_FISH].height=15;
    NPC_SPRITE_COUNTS[NPC_FISH].dead_width=0;
    NPC_SPRITE_COUNTS[NPC_FISH].dead_height=0;
    NPC_SPRITE_COUNTS[NPC_FISH].special_width=0;
    NPC_SPRITE_COUNTS[NPC_FISH].special_height=0;

    //Monkey
    NPC_SPRITE_COUNTS[NPC_MONKEY].walk_sprites=1;
    NPC_SPRITE_COUNTS[NPC_MONKEY].jump_sprites=0;
    NPC_SPRITE_COUNTS[NPC_MONKEY].swim_sprites=0;
    NPC_SPRITE_COUNTS[NPC_MONKEY].fly_sprites=0;
    NPC_SPRITE_COUNTS[NPC_MONKEY].special_attack_sprites=2;
    NPC_SPRITE_COUNTS[NPC_MONKEY].dead_sprites=3;
    NPC_SPRITE_COUNTS[NPC_MONKEY].width=26;
    NPC_SPRITE_COUNTS[NPC_MONKEY].height=51;
    NPC_SPRITE_COUNTS[NPC_MONKEY].dead_width=26;
    NPC_SPRITE_COUNTS[NPC_MONKEY].dead_height=44;
    NPC_SPRITE_COUNTS[NPC_MONKEY].special_width=26;
    NPC_SPRITE_COUNTS[NPC_MONKEY].special_height=51;

    //Laser spinner
    NPC_SPRITE_COUNTS[NPC_LASER_SPINNER].walk_sprites=1;
    NPC_SPRITE_COUNTS[NPC_LASER_SPINNER].jump_sprites=0;
    NPC_SPRITE_COUNTS[NPC_LASER_SPINNER].swim_sprites=0;
    NPC_SPRITE_COUNTS[NPC_LASER_SPINNER].fly_sprites=0;
    NPC_SPRITE_COUNTS[NPC_LASER_SPINNER].special_attack_sprites=5;
    NPC_SPRITE_COUNTS[NPC_LASER_SPINNER].dead_sprites=0;
    NPC_SPRITE_COUNTS[NPC_LASER_SPINNER].width=32;
    NPC_SPRITE_COUNTS[NPC_LASER_SPINNER].height=32;
    NPC_SPRITE_COUNTS[NPC_LASER_SPINNER].dead_width=0;
    NPC_SPRITE_COUNTS[NPC_LASER_SPINNER].dead_height=0;
    NPC_SPRITE_COUNTS[NPC_LASER_SPINNER].special_width=32;
    NPC_SPRITE_COUNTS[NPC_LASER_SPINNER].special_height=32;

    //Penguin
    NPC_SPRITE_COUNTS[NPC_PENGUIN].walk_sprites=3;
    NPC_SPRITE_COUNTS[NPC_PENGUIN].jump_sprites=3;
    NPC_SPRITE_COUNTS[NPC_PENGUIN].swim_sprites=3;
    NPC_SPRITE_COUNTS[NPC_PENGUIN].fly_sprites=0;
    NPC_SPRITE_COUNTS[NPC_PENGUIN].special_attack_sprites=0;
    NPC_SPRITE_COUNTS[NPC_PENGUIN].dead_sprites=3;
    NPC_SPRITE_COUNTS[NPC_PENGUIN].width=32;
    NPC_SPRITE_COUNTS[NPC_PENGUIN].height=64;
    NPC_SPRITE_COUNTS[NPC_PENGUIN].dead_width=31;
    NPC_SPRITE_COUNTS[NPC_PENGUIN].dead_height=63;
    NPC_SPRITE_COUNTS[NPC_PENGUIN].special_width=0;
    NPC_SPRITE_COUNTS[NPC_PENGUIN].special_height=0;

    //Fish on ice
    NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE].walk_sprites=1;
    NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE].jump_sprites=1;
    NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE].swim_sprites=1;
    NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE].fly_sprites=0;
    NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE].special_attack_sprites=0;
    NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE].dead_sprites=3;
    NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE].width=32;
    NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE].height=32;
    NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE].dead_width=32;
    NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE].dead_height=32;
    NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE].special_width=0;
    NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE].special_height=0;

    //Penguin harmless
    NPC_SPRITE_COUNTS[NPC_PENGUIN_HARMLESS].walk_sprites=0;
    NPC_SPRITE_COUNTS[NPC_PENGUIN_HARMLESS].jump_sprites=0;
    NPC_SPRITE_COUNTS[NPC_PENGUIN_HARMLESS].swim_sprites=0;
    NPC_SPRITE_COUNTS[NPC_PENGUIN_HARMLESS].fly_sprites=0;
    NPC_SPRITE_COUNTS[NPC_PENGUIN_HARMLESS].special_attack_sprites=0;
    NPC_SPRITE_COUNTS[NPC_PENGUIN_HARMLESS].dead_sprites=9;
    NPC_SPRITE_COUNTS[NPC_PENGUIN_HARMLESS].width=31;
    NPC_SPRITE_COUNTS[NPC_PENGUIN_HARMLESS].height=64;
    NPC_SPRITE_COUNTS[NPC_PENGUIN_HARMLESS].dead_width=31;
    NPC_SPRITE_COUNTS[NPC_PENGUIN_HARMLESS].dead_height=64;
    NPC_SPRITE_COUNTS[NPC_PENGUIN_HARMLESS].special_width=0;
    NPC_SPRITE_COUNTS[NPC_PENGUIN_HARMLESS].special_height=0;

    //Fish on ice harmless
    NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE_HARMLESS].walk_sprites=0;
    NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE_HARMLESS].jump_sprites=0;
    NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE_HARMLESS].swim_sprites=0;
    NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE_HARMLESS].fly_sprites=0;
    NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE_HARMLESS].special_attack_sprites=0;
    NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE_HARMLESS].dead_sprites=7;
    NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE_HARMLESS].width=21;
    NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE_HARMLESS].height=32;
    NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE_HARMLESS].dead_width=21;
    NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE_HARMLESS].dead_height=32;
    NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE_HARMLESS].special_width=0;
    NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE_HARMLESS].special_height=0;

    //Sword knight
    NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].walk_sprites=7;
    NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].jump_sprites=7;
    NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].swim_sprites=7;
    NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].fly_sprites=0;
    NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].special_attack_sprites=7;
    NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].dead_sprites=3;
    NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].width=32;
    NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].height=78;
    NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].dead_width=32;
    NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].dead_height=88;
    NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].special_width=64;
    NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].special_height=78;

    //Ball knight
    NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].walk_sprites=7;
    NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].jump_sprites=7;
    NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].swim_sprites=7;
    NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].fly_sprites=0;
    NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].special_attack_sprites=9;
    NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].dead_sprites=3;
    NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].width=32;
    NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].height=78;
    NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].dead_width=32;
    NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].dead_height=88;
    NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].special_width=32;
    NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].special_height=78;

    //Archer
    NPC_SPRITE_COUNTS[NPC_ARCHER].walk_sprites=6;
    NPC_SPRITE_COUNTS[NPC_ARCHER].jump_sprites=0;
    NPC_SPRITE_COUNTS[NPC_ARCHER].swim_sprites=0;
    NPC_SPRITE_COUNTS[NPC_ARCHER].fly_sprites=0;
    NPC_SPRITE_COUNTS[NPC_ARCHER].special_attack_sprites=3;
    NPC_SPRITE_COUNTS[NPC_ARCHER].dead_sprites=3;
    NPC_SPRITE_COUNTS[NPC_ARCHER].width=16;
    NPC_SPRITE_COUNTS[NPC_ARCHER].height=32;
    NPC_SPRITE_COUNTS[NPC_ARCHER].dead_width=32;
    NPC_SPRITE_COUNTS[NPC_ARCHER].dead_height=32;
    NPC_SPRITE_COUNTS[NPC_ARCHER].special_width=16;
    NPC_SPRITE_COUNTS[NPC_ARCHER].special_height=32;

    //Scarecrow
    NPC_SPRITE_COUNTS[NPC_SCARECROW].walk_sprites=10;
    NPC_SPRITE_COUNTS[NPC_SCARECROW].jump_sprites=0;
    NPC_SPRITE_COUNTS[NPC_SCARECROW].swim_sprites=0;
    NPC_SPRITE_COUNTS[NPC_SCARECROW].fly_sprites=0;
    NPC_SPRITE_COUNTS[NPC_SCARECROW].special_attack_sprites=3;
    NPC_SPRITE_COUNTS[NPC_SCARECROW].dead_sprites=3;
    NPC_SPRITE_COUNTS[NPC_SCARECROW].width=59;
    NPC_SPRITE_COUNTS[NPC_SCARECROW].height=81;
    NPC_SPRITE_COUNTS[NPC_SCARECROW].dead_width=59;
    NPC_SPRITE_COUNTS[NPC_SCARECROW].dead_height=84;
    NPC_SPRITE_COUNTS[NPC_SCARECROW].special_width=76;
    NPC_SPRITE_COUNTS[NPC_SCARECROW].special_height=69;

    //Pumpkin
    NPC_SPRITE_COUNTS[NPC_PUMPKIN].walk_sprites=1;
    NPC_SPRITE_COUNTS[NPC_PUMPKIN].jump_sprites=1;
    NPC_SPRITE_COUNTS[NPC_PUMPKIN].swim_sprites=1;
    NPC_SPRITE_COUNTS[NPC_PUMPKIN].fly_sprites=0;
    NPC_SPRITE_COUNTS[NPC_PUMPKIN].special_attack_sprites=0;
    NPC_SPRITE_COUNTS[NPC_PUMPKIN].dead_sprites=3;
    NPC_SPRITE_COUNTS[NPC_PUMPKIN].width=43;
    NPC_SPRITE_COUNTS[NPC_PUMPKIN].height=35;
    NPC_SPRITE_COUNTS[NPC_PUMPKIN].dead_width=43;
    NPC_SPRITE_COUNTS[NPC_PUMPKIN].dead_height=35;
    NPC_SPRITE_COUNTS[NPC_PUMPKIN].special_width=0;
    NPC_SPRITE_COUNTS[NPC_PUMPKIN].special_height=0;

    //Zombie cow
    NPC_SPRITE_COUNTS[NPC_ZOMBIE_COW].walk_sprites=6;
    NPC_SPRITE_COUNTS[NPC_ZOMBIE_COW].jump_sprites=6;
    NPC_SPRITE_COUNTS[NPC_ZOMBIE_COW].swim_sprites=6;
    NPC_SPRITE_COUNTS[NPC_ZOMBIE_COW].fly_sprites=0;
    NPC_SPRITE_COUNTS[NPC_ZOMBIE_COW].special_attack_sprites=0;
    NPC_SPRITE_COUNTS[NPC_ZOMBIE_COW].dead_sprites=3;
    NPC_SPRITE_COUNTS[NPC_ZOMBIE_COW].width=44;
    NPC_SPRITE_COUNTS[NPC_ZOMBIE_COW].height=79;
    NPC_SPRITE_COUNTS[NPC_ZOMBIE_COW].dead_width=44;
    NPC_SPRITE_COUNTS[NPC_ZOMBIE_COW].dead_height=79;
    NPC_SPRITE_COUNTS[NPC_ZOMBIE_COW].special_width=44;
    NPC_SPRITE_COUNTS[NPC_ZOMBIE_COW].special_height=79;

    //Mummy cow
    NPC_SPRITE_COUNTS[NPC_MUMMY_COW].walk_sprites=6;
    NPC_SPRITE_COUNTS[NPC_MUMMY_COW].jump_sprites=6;
    NPC_SPRITE_COUNTS[NPC_MUMMY_COW].swim_sprites=6;
    NPC_SPRITE_COUNTS[NPC_MUMMY_COW].fly_sprites=0;
    NPC_SPRITE_COUNTS[NPC_MUMMY_COW].special_attack_sprites=0;
    NPC_SPRITE_COUNTS[NPC_MUMMY_COW].dead_sprites=3;
    NPC_SPRITE_COUNTS[NPC_MUMMY_COW].width=44;
    NPC_SPRITE_COUNTS[NPC_MUMMY_COW].height=79;
    NPC_SPRITE_COUNTS[NPC_MUMMY_COW].dead_width=44;
    NPC_SPRITE_COUNTS[NPC_MUMMY_COW].dead_height=79;
    NPC_SPRITE_COUNTS[NPC_MUMMY_COW].special_width=44;
    NPC_SPRITE_COUNTS[NPC_MUMMY_COW].special_height=79;

    //Ghost
    NPC_SPRITE_COUNTS[NPC_GHOST].walk_sprites=0;
    NPC_SPRITE_COUNTS[NPC_GHOST].jump_sprites=0;
    NPC_SPRITE_COUNTS[NPC_GHOST].swim_sprites=0;
    NPC_SPRITE_COUNTS[NPC_GHOST].fly_sprites=4;
    NPC_SPRITE_COUNTS[NPC_GHOST].special_attack_sprites=4;
    NPC_SPRITE_COUNTS[NPC_GHOST].dead_sprites=3;
    NPC_SPRITE_COUNTS[NPC_GHOST].width=44;
    NPC_SPRITE_COUNTS[NPC_GHOST].height=79;
    NPC_SPRITE_COUNTS[NPC_GHOST].dead_width=44;
    NPC_SPRITE_COUNTS[NPC_GHOST].dead_height=79;
    NPC_SPRITE_COUNTS[NPC_GHOST].special_width=44;
    NPC_SPRITE_COUNTS[NPC_GHOST].special_height=79;

    //Spider
    NPC_SPRITE_COUNTS[NPC_SPIDER].walk_sprites=0;
    NPC_SPRITE_COUNTS[NPC_SPIDER].jump_sprites=0;
    NPC_SPRITE_COUNTS[NPC_SPIDER].swim_sprites=0;
    NPC_SPRITE_COUNTS[NPC_SPIDER].fly_sprites=3;
    NPC_SPRITE_COUNTS[NPC_SPIDER].special_attack_sprites=0;
    NPC_SPRITE_COUNTS[NPC_SPIDER].dead_sprites=3;
    NPC_SPRITE_COUNTS[NPC_SPIDER].width=32;
    NPC_SPRITE_COUNTS[NPC_SPIDER].height=16;
    NPC_SPRITE_COUNTS[NPC_SPIDER].dead_width=32;
    NPC_SPRITE_COUNTS[NPC_SPIDER].dead_height=16;
    NPC_SPRITE_COUNTS[NPC_SPIDER].special_width=0;
    NPC_SPRITE_COUNTS[NPC_SPIDER].special_height=0;

    //Cat
    NPC_SPRITE_COUNTS[NPC_CAT].walk_sprites=2;
    NPC_SPRITE_COUNTS[NPC_CAT].jump_sprites=1;
    NPC_SPRITE_COUNTS[NPC_CAT].swim_sprites=2;
    NPC_SPRITE_COUNTS[NPC_CAT].fly_sprites=0;
    NPC_SPRITE_COUNTS[NPC_CAT].special_attack_sprites=5;
    NPC_SPRITE_COUNTS[NPC_CAT].dead_sprites=3;
    NPC_SPRITE_COUNTS[NPC_CAT].width=32;
    NPC_SPRITE_COUNTS[NPC_CAT].height=28;
    NPC_SPRITE_COUNTS[NPC_CAT].dead_width=38;
    NPC_SPRITE_COUNTS[NPC_CAT].dead_height=27;
    NPC_SPRITE_COUNTS[NPC_CAT].special_width=30;
    NPC_SPRITE_COUNTS[NPC_CAT].special_height=26;

    //Spider walking
    NPC_SPRITE_COUNTS[NPC_SPIDER_WALKING].walk_sprites=3;
    NPC_SPRITE_COUNTS[NPC_SPIDER_WALKING].jump_sprites=3;
    NPC_SPRITE_COUNTS[NPC_SPIDER_WALKING].swim_sprites=3;
    NPC_SPRITE_COUNTS[NPC_SPIDER_WALKING].fly_sprites=0;
    NPC_SPRITE_COUNTS[NPC_SPIDER_WALKING].special_attack_sprites=0;
    NPC_SPRITE_COUNTS[NPC_SPIDER_WALKING].dead_sprites=3;
    NPC_SPRITE_COUNTS[NPC_SPIDER_WALKING].width=32;
    NPC_SPRITE_COUNTS[NPC_SPIDER_WALKING].height=16;
    NPC_SPRITE_COUNTS[NPC_SPIDER_WALKING].dead_width=32;
    NPC_SPRITE_COUNTS[NPC_SPIDER_WALKING].dead_height=16;
    NPC_SPRITE_COUNTS[NPC_SPIDER_WALKING].special_width=0;
    NPC_SPRITE_COUNTS[NPC_SPIDER_WALKING].special_height=0;

    //Ram
    NPC_SPRITE_COUNTS[NPC_RAM].walk_sprites=6;
    NPC_SPRITE_COUNTS[NPC_RAM].jump_sprites=6;
    NPC_SPRITE_COUNTS[NPC_RAM].swim_sprites=6;
    NPC_SPRITE_COUNTS[NPC_RAM].fly_sprites=0;
    NPC_SPRITE_COUNTS[NPC_RAM].special_attack_sprites=6;
    NPC_SPRITE_COUNTS[NPC_RAM].dead_sprites=3;
    NPC_SPRITE_COUNTS[NPC_RAM].width=74;
    NPC_SPRITE_COUNTS[NPC_RAM].height=48;
    NPC_SPRITE_COUNTS[NPC_RAM].dead_width=74;
    NPC_SPRITE_COUNTS[NPC_RAM].dead_height=34;
    NPC_SPRITE_COUNTS[NPC_RAM].special_width=74;
    NPC_SPRITE_COUNTS[NPC_RAM].special_height=48;

    //Eagle
    NPC_SPRITE_COUNTS[NPC_EAGLE].walk_sprites=0;
    NPC_SPRITE_COUNTS[NPC_EAGLE].jump_sprites=0;
    NPC_SPRITE_COUNTS[NPC_EAGLE].swim_sprites=0;
    NPC_SPRITE_COUNTS[NPC_EAGLE].fly_sprites=1;
    NPC_SPRITE_COUNTS[NPC_EAGLE].special_attack_sprites=4;
    NPC_SPRITE_COUNTS[NPC_EAGLE].dead_sprites=3;
    NPC_SPRITE_COUNTS[NPC_EAGLE].width=53;
    NPC_SPRITE_COUNTS[NPC_EAGLE].height=35;
    NPC_SPRITE_COUNTS[NPC_EAGLE].dead_width=53;
    NPC_SPRITE_COUNTS[NPC_EAGLE].dead_height=35;
    NPC_SPRITE_COUNTS[NPC_EAGLE].special_width=53;
    NPC_SPRITE_COUNTS[NPC_EAGLE].special_height=35;

    //Snowman
    NPC_SPRITE_COUNTS[NPC_SNOWMAN].walk_sprites=1;
    NPC_SPRITE_COUNTS[NPC_SNOWMAN].jump_sprites=1;
    NPC_SPRITE_COUNTS[NPC_SNOWMAN].swim_sprites=1;
    NPC_SPRITE_COUNTS[NPC_SNOWMAN].fly_sprites=0;
    NPC_SPRITE_COUNTS[NPC_SNOWMAN].special_attack_sprites=3;
    NPC_SPRITE_COUNTS[NPC_SNOWMAN].dead_sprites=3;
    NPC_SPRITE_COUNTS[NPC_SNOWMAN].width=25;
    NPC_SPRITE_COUNTS[NPC_SNOWMAN].height=52;
    NPC_SPRITE_COUNTS[NPC_SNOWMAN].dead_width=25;
    NPC_SPRITE_COUNTS[NPC_SNOWMAN].dead_height=52;
    NPC_SPRITE_COUNTS[NPC_SNOWMAN].special_width=25;
    NPC_SPRITE_COUNTS[NPC_SNOWMAN].special_height=52;

    //Fireball
    NPC_SPRITE_COUNTS[NPC_FIREBALL].walk_sprites=0;
    NPC_SPRITE_COUNTS[NPC_FIREBALL].jump_sprites=0;
    NPC_SPRITE_COUNTS[NPC_FIREBALL].swim_sprites=0;
    NPC_SPRITE_COUNTS[NPC_FIREBALL].fly_sprites=3;
    NPC_SPRITE_COUNTS[NPC_FIREBALL].special_attack_sprites=0;
    NPC_SPRITE_COUNTS[NPC_FIREBALL].dead_sprites=3;
    NPC_SPRITE_COUNTS[NPC_FIREBALL].width=63;
    NPC_SPRITE_COUNTS[NPC_FIREBALL].height=29;
    NPC_SPRITE_COUNTS[NPC_FIREBALL].dead_width=63;
    NPC_SPRITE_COUNTS[NPC_FIREBALL].dead_height=29;
    NPC_SPRITE_COUNTS[NPC_FIREBALL].special_width=0;
    NPC_SPRITE_COUNTS[NPC_FIREBALL].special_height=0;

    //Dragon whelp
    NPC_SPRITE_COUNTS[NPC_DRAGON_WHELP].walk_sprites=0;
    NPC_SPRITE_COUNTS[NPC_DRAGON_WHELP].jump_sprites=0;
    NPC_SPRITE_COUNTS[NPC_DRAGON_WHELP].swim_sprites=0;
    NPC_SPRITE_COUNTS[NPC_DRAGON_WHELP].fly_sprites=4;
    NPC_SPRITE_COUNTS[NPC_DRAGON_WHELP].special_attack_sprites=2;
    NPC_SPRITE_COUNTS[NPC_DRAGON_WHELP].dead_sprites=3;
    NPC_SPRITE_COUNTS[NPC_DRAGON_WHELP].width=34;
    NPC_SPRITE_COUNTS[NPC_DRAGON_WHELP].height=46;
    NPC_SPRITE_COUNTS[NPC_DRAGON_WHELP].dead_width=34;
    NPC_SPRITE_COUNTS[NPC_DRAGON_WHELP].dead_height=44;
    NPC_SPRITE_COUNTS[NPC_DRAGON_WHELP].special_width=34;
    NPC_SPRITE_COUNTS[NPC_DRAGON_WHELP].special_height=46;

    //Fireball shooter left
    NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_LEFT].walk_sprites=1;
    NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_LEFT].jump_sprites=0;
    NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_LEFT].swim_sprites=0;
    NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_LEFT].fly_sprites=0;
    NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_LEFT].special_attack_sprites=2;
    NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_LEFT].dead_sprites=0;
    NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_LEFT].width=32;
    NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_LEFT].height=32;
    NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_LEFT].dead_width=0;
    NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_LEFT].dead_height=0;
    NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_LEFT].special_width=32;
    NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_LEFT].special_height=32;

    //Fireball shooter right
    NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_RIGHT].walk_sprites=1;
    NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_RIGHT].jump_sprites=0;
    NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_RIGHT].swim_sprites=0;
    NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_RIGHT].fly_sprites=0;
    NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_RIGHT].special_attack_sprites=2;
    NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_RIGHT].dead_sprites=0;
    NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_RIGHT].width=32;
    NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_RIGHT].height=32;
    NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_RIGHT].dead_width=0;
    NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_RIGHT].dead_height=0;
    NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_RIGHT].special_width=32;
    NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_RIGHT].special_height=32;

    //Fireball shooter up
    NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_UP].walk_sprites=1;
    NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_UP].jump_sprites=0;
    NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_UP].swim_sprites=0;
    NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_UP].fly_sprites=0;
    NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_UP].special_attack_sprites=2;
    NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_UP].dead_sprites=0;
    NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_UP].width=32;
    NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_UP].height=32;
    NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_UP].dead_width=0;
    NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_UP].dead_height=0;
    NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_UP].special_width=32;
    NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_UP].special_height=32;

    //Fireball shooter down
    NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_DOWN].walk_sprites=1;
    NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_DOWN].jump_sprites=0;
    NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_DOWN].swim_sprites=0;
    NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_DOWN].fly_sprites=0;
    NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_DOWN].special_attack_sprites=2;
    NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_DOWN].dead_sprites=0;
    NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_DOWN].width=32;
    NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_DOWN].height=32;
    NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_DOWN].dead_width=0;
    NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_DOWN].dead_height=0;
    NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_DOWN].special_width=32;
    NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_DOWN].special_height=32;

    //Dust devil
    NPC_SPRITE_COUNTS[NPC_DUST_DEVIL].walk_sprites=0;
    NPC_SPRITE_COUNTS[NPC_DUST_DEVIL].jump_sprites=0;
    NPC_SPRITE_COUNTS[NPC_DUST_DEVIL].swim_sprites=0;
    NPC_SPRITE_COUNTS[NPC_DUST_DEVIL].fly_sprites=2;
    NPC_SPRITE_COUNTS[NPC_DUST_DEVIL].special_attack_sprites=0;
    NPC_SPRITE_COUNTS[NPC_DUST_DEVIL].dead_sprites=3;
    NPC_SPRITE_COUNTS[NPC_DUST_DEVIL].width=64;
    NPC_SPRITE_COUNTS[NPC_DUST_DEVIL].height=64;
    NPC_SPRITE_COUNTS[NPC_DUST_DEVIL].dead_width=64;
    NPC_SPRITE_COUNTS[NPC_DUST_DEVIL].dead_height=64;
    NPC_SPRITE_COUNTS[NPC_DUST_DEVIL].special_width=0;
    NPC_SPRITE_COUNTS[NPC_DUST_DEVIL].special_height=0;

    //Scorpion
    NPC_SPRITE_COUNTS[NPC_SCORPION].walk_sprites=3;
    NPC_SPRITE_COUNTS[NPC_SCORPION].jump_sprites=3;
    NPC_SPRITE_COUNTS[NPC_SCORPION].swim_sprites=3;
    NPC_SPRITE_COUNTS[NPC_SCORPION].fly_sprites=0;
    NPC_SPRITE_COUNTS[NPC_SCORPION].special_attack_sprites=5;
    NPC_SPRITE_COUNTS[NPC_SCORPION].dead_sprites=3;
    NPC_SPRITE_COUNTS[NPC_SCORPION].width=40;
    NPC_SPRITE_COUNTS[NPC_SCORPION].height=20;
    NPC_SPRITE_COUNTS[NPC_SCORPION].dead_width=40;
    NPC_SPRITE_COUNTS[NPC_SCORPION].dead_height=20;
    NPC_SPRITE_COUNTS[NPC_SCORPION].special_width=40;
    NPC_SPRITE_COUNTS[NPC_SCORPION].special_height=20;

    //Sphinx left
    NPC_SPRITE_COUNTS[NPC_SPHINX_LEFT].walk_sprites=1;
    NPC_SPRITE_COUNTS[NPC_SPHINX_LEFT].jump_sprites=0;
    NPC_SPRITE_COUNTS[NPC_SPHINX_LEFT].swim_sprites=0;
    NPC_SPRITE_COUNTS[NPC_SPHINX_LEFT].fly_sprites=0;
    NPC_SPRITE_COUNTS[NPC_SPHINX_LEFT].special_attack_sprites=2;
    NPC_SPRITE_COUNTS[NPC_SPHINX_LEFT].dead_sprites=0;
    NPC_SPRITE_COUNTS[NPC_SPHINX_LEFT].width=100;
    NPC_SPRITE_COUNTS[NPC_SPHINX_LEFT].height=66;
    NPC_SPRITE_COUNTS[NPC_SPHINX_LEFT].dead_width=0;
    NPC_SPRITE_COUNTS[NPC_SPHINX_LEFT].dead_height=0;
    NPC_SPRITE_COUNTS[NPC_SPHINX_LEFT].special_width=110;
    NPC_SPRITE_COUNTS[NPC_SPHINX_LEFT].special_height=66;

    //Sphinx right
    NPC_SPRITE_COUNTS[NPC_SPHINX_RIGHT].walk_sprites=1;
    NPC_SPRITE_COUNTS[NPC_SPHINX_RIGHT].jump_sprites=0;
    NPC_SPRITE_COUNTS[NPC_SPHINX_RIGHT].swim_sprites=0;
    NPC_SPRITE_COUNTS[NPC_SPHINX_RIGHT].fly_sprites=0;
    NPC_SPRITE_COUNTS[NPC_SPHINX_RIGHT].special_attack_sprites=2;
    NPC_SPRITE_COUNTS[NPC_SPHINX_RIGHT].dead_sprites=0;
    NPC_SPRITE_COUNTS[NPC_SPHINX_RIGHT].width=100;
    NPC_SPRITE_COUNTS[NPC_SPHINX_RIGHT].height=66;
    NPC_SPRITE_COUNTS[NPC_SPHINX_RIGHT].dead_width=0;
    NPC_SPRITE_COUNTS[NPC_SPHINX_RIGHT].dead_height=0;
    NPC_SPRITE_COUNTS[NPC_SPHINX_RIGHT].special_width=110;
    NPC_SPRITE_COUNTS[NPC_SPHINX_RIGHT].special_height=66;

    //Shark
    NPC_SPRITE_COUNTS[NPC_SHARK].walk_sprites=0;
    NPC_SPRITE_COUNTS[NPC_SHARK].jump_sprites=0;
    NPC_SPRITE_COUNTS[NPC_SHARK].swim_sprites=2;
    NPC_SPRITE_COUNTS[NPC_SHARK].fly_sprites=0;
    NPC_SPRITE_COUNTS[NPC_SHARK].special_attack_sprites=0;
    NPC_SPRITE_COUNTS[NPC_SHARK].dead_sprites=0;
    NPC_SPRITE_COUNTS[NPC_SHARK].width=64;
    NPC_SPRITE_COUNTS[NPC_SHARK].height=32;
    NPC_SPRITE_COUNTS[NPC_SHARK].dead_width=0;
    NPC_SPRITE_COUNTS[NPC_SHARK].dead_height=0;
    NPC_SPRITE_COUNTS[NPC_SHARK].special_width=0;
    NPC_SPRITE_COUNTS[NPC_SHARK].special_height=0;

    //Henchmouse
    NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].walk_sprites=6;
    NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].jump_sprites=6;
    NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].swim_sprites=6;
    NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].fly_sprites=0;
    NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].special_attack_sprites=10;
    NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].dead_sprites=3;
    NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].width=29;
    NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].height=52;
    NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].dead_width=29;
    NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].dead_height=52;
    NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].special_width=29;
    NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].special_height=52;

    //Mechsuit mouse
    NPC_SPRITE_COUNTS[NPC_MECHSUIT_MOUSE].walk_sprites=0;
    NPC_SPRITE_COUNTS[NPC_MECHSUIT_MOUSE].jump_sprites=0;
    NPC_SPRITE_COUNTS[NPC_MECHSUIT_MOUSE].swim_sprites=0;
    NPC_SPRITE_COUNTS[NPC_MECHSUIT_MOUSE].fly_sprites=3;
    NPC_SPRITE_COUNTS[NPC_MECHSUIT_MOUSE].special_attack_sprites=3;
    NPC_SPRITE_COUNTS[NPC_MECHSUIT_MOUSE].dead_sprites=3;
    NPC_SPRITE_COUNTS[NPC_MECHSUIT_MOUSE].width=77;
    NPC_SPRITE_COUNTS[NPC_MECHSUIT_MOUSE].height=43;
    NPC_SPRITE_COUNTS[NPC_MECHSUIT_MOUSE].dead_width=77;
    NPC_SPRITE_COUNTS[NPC_MECHSUIT_MOUSE].dead_height=43;
    NPC_SPRITE_COUNTS[NPC_MECHSUIT_MOUSE].special_width=77;
    NPC_SPRITE_COUNTS[NPC_MECHSUIT_MOUSE].special_height=43;

    //Sally
    NPC_SPRITE_COUNTS[NPC_SALLY].walk_sprites=15;
    NPC_SPRITE_COUNTS[NPC_SALLY].jump_sprites=15;
    NPC_SPRITE_COUNTS[NPC_SALLY].swim_sprites=17;
    NPC_SPRITE_COUNTS[NPC_SALLY].fly_sprites=0;
    NPC_SPRITE_COUNTS[NPC_SALLY].special_attack_sprites=0;
    NPC_SPRITE_COUNTS[NPC_SALLY].dead_sprites=0;
    NPC_SPRITE_COUNTS[NPC_SALLY].width=26;
    NPC_SPRITE_COUNTS[NPC_SALLY].height=46;
    NPC_SPRITE_COUNTS[NPC_SALLY].dead_width=0;
    NPC_SPRITE_COUNTS[NPC_SALLY].dead_height=0;
    NPC_SPRITE_COUNTS[NPC_SALLY].special_width=0;
    NPC_SPRITE_COUNTS[NPC_SALLY].special_height=0;

    //Bouncing cow
    NPC_SPRITE_COUNTS[NPC_BOUNCING_COW].walk_sprites=6;
    NPC_SPRITE_COUNTS[NPC_BOUNCING_COW].jump_sprites=6;
    NPC_SPRITE_COUNTS[NPC_BOUNCING_COW].swim_sprites=6;
    NPC_SPRITE_COUNTS[NPC_BOUNCING_COW].fly_sprites=0;
    NPC_SPRITE_COUNTS[NPC_BOUNCING_COW].special_attack_sprites=0;
    NPC_SPRITE_COUNTS[NPC_BOUNCING_COW].dead_sprites=3;
    NPC_SPRITE_COUNTS[NPC_BOUNCING_COW].width=81;
    NPC_SPRITE_COUNTS[NPC_BOUNCING_COW].height=44;
    NPC_SPRITE_COUNTS[NPC_BOUNCING_COW].dead_width=36;
    NPC_SPRITE_COUNTS[NPC_BOUNCING_COW].dead_height=44;
    NPC_SPRITE_COUNTS[NPC_BOUNCING_COW].special_width=81;
    NPC_SPRITE_COUNTS[NPC_BOUNCING_COW].special_height=44;

    //Bouncing ball
    NPC_SPRITE_COUNTS[NPC_BOUNCING_BALL].walk_sprites=1;
    NPC_SPRITE_COUNTS[NPC_BOUNCING_BALL].jump_sprites=1;
    NPC_SPRITE_COUNTS[NPC_BOUNCING_BALL].swim_sprites=1;
    NPC_SPRITE_COUNTS[NPC_BOUNCING_BALL].fly_sprites=0;
    NPC_SPRITE_COUNTS[NPC_BOUNCING_BALL].special_attack_sprites=0;
    NPC_SPRITE_COUNTS[NPC_BOUNCING_BALL].dead_sprites=1;
    NPC_SPRITE_COUNTS[NPC_BOUNCING_BALL].width=16;
    NPC_SPRITE_COUNTS[NPC_BOUNCING_BALL].height=16;
    NPC_SPRITE_COUNTS[NPC_BOUNCING_BALL].dead_width=16;
    NPC_SPRITE_COUNTS[NPC_BOUNCING_BALL].dead_height=16;
    NPC_SPRITE_COUNTS[NPC_BOUNCING_BALL].special_width=0;
    NPC_SPRITE_COUNTS[NPC_BOUNCING_BALL].special_height=0;

    //Sea turtle
    NPC_SPRITE_COUNTS[NPC_SEA_TURTLE].walk_sprites=0;
    NPC_SPRITE_COUNTS[NPC_SEA_TURTLE].jump_sprites=0;
    NPC_SPRITE_COUNTS[NPC_SEA_TURTLE].swim_sprites=4;
    NPC_SPRITE_COUNTS[NPC_SEA_TURTLE].fly_sprites=0;
    NPC_SPRITE_COUNTS[NPC_SEA_TURTLE].special_attack_sprites=0;
    NPC_SPRITE_COUNTS[NPC_SEA_TURTLE].dead_sprites=0;
    NPC_SPRITE_COUNTS[NPC_SEA_TURTLE].width=54;
    NPC_SPRITE_COUNTS[NPC_SEA_TURTLE].height=17;
    NPC_SPRITE_COUNTS[NPC_SEA_TURTLE].dead_width=0;
    NPC_SPRITE_COUNTS[NPC_SEA_TURTLE].dead_height=0;
    NPC_SPRITE_COUNTS[NPC_SEA_TURTLE].special_width=0;
    NPC_SPRITE_COUNTS[NPC_SEA_TURTLE].special_height=0;

    //Starfish
    NPC_SPRITE_COUNTS[NPC_STARFISH].walk_sprites=0;
    NPC_SPRITE_COUNTS[NPC_STARFISH].jump_sprites=0;
    NPC_SPRITE_COUNTS[NPC_STARFISH].swim_sprites=1;
    NPC_SPRITE_COUNTS[NPC_STARFISH].fly_sprites=0;
    NPC_SPRITE_COUNTS[NPC_STARFISH].special_attack_sprites=0;
    NPC_SPRITE_COUNTS[NPC_STARFISH].dead_sprites=0;
    NPC_SPRITE_COUNTS[NPC_STARFISH].width=43;
    NPC_SPRITE_COUNTS[NPC_STARFISH].height=42;
    NPC_SPRITE_COUNTS[NPC_STARFISH].dead_width=0;
    NPC_SPRITE_COUNTS[NPC_STARFISH].dead_height=0;
    NPC_SPRITE_COUNTS[NPC_STARFISH].special_width=0;
    NPC_SPRITE_COUNTS[NPC_STARFISH].special_height=0;

    //Boss Castle
    NPC_SPRITE_COUNTS[NPC_BOSS_CASTLE].walk_sprites=0;
    NPC_SPRITE_COUNTS[NPC_BOSS_CASTLE].jump_sprites=0;
    NPC_SPRITE_COUNTS[NPC_BOSS_CASTLE].swim_sprites=0;
    NPC_SPRITE_COUNTS[NPC_BOSS_CASTLE].fly_sprites=3;
    NPC_SPRITE_COUNTS[NPC_BOSS_CASTLE].special_attack_sprites=0;
    NPC_SPRITE_COUNTS[NPC_BOSS_CASTLE].dead_sprites=0;
    NPC_SPRITE_COUNTS[NPC_BOSS_CASTLE].width=77;
    NPC_SPRITE_COUNTS[NPC_BOSS_CASTLE].height=43;
    NPC_SPRITE_COUNTS[NPC_BOSS_CASTLE].dead_width=77;
    NPC_SPRITE_COUNTS[NPC_BOSS_CASTLE].dead_height=43;
    NPC_SPRITE_COUNTS[NPC_BOSS_CASTLE].special_width=0;
    NPC_SPRITE_COUNTS[NPC_BOSS_CASTLE].special_height=0;

    //Boss Haunted House
    NPC_SPRITE_COUNTS[NPC_BOSS_HAUNTED_HOUSE].walk_sprites=0;
    NPC_SPRITE_COUNTS[NPC_BOSS_HAUNTED_HOUSE].jump_sprites=0;
    NPC_SPRITE_COUNTS[NPC_BOSS_HAUNTED_HOUSE].swim_sprites=0;
    NPC_SPRITE_COUNTS[NPC_BOSS_HAUNTED_HOUSE].fly_sprites=3;
    NPC_SPRITE_COUNTS[NPC_BOSS_HAUNTED_HOUSE].special_attack_sprites=0;
    NPC_SPRITE_COUNTS[NPC_BOSS_HAUNTED_HOUSE].dead_sprites=0;
    NPC_SPRITE_COUNTS[NPC_BOSS_HAUNTED_HOUSE].width=77;
    NPC_SPRITE_COUNTS[NPC_BOSS_HAUNTED_HOUSE].height=43;
    NPC_SPRITE_COUNTS[NPC_BOSS_HAUNTED_HOUSE].dead_width=77;
    NPC_SPRITE_COUNTS[NPC_BOSS_HAUNTED_HOUSE].dead_height=43;
    NPC_SPRITE_COUNTS[NPC_BOSS_HAUNTED_HOUSE].special_width=77;
    NPC_SPRITE_COUNTS[NPC_BOSS_HAUNTED_HOUSE].special_height=43;

    //Boss Volcano
    NPC_SPRITE_COUNTS[NPC_BOSS_VOLCANO].walk_sprites=3;
    NPC_SPRITE_COUNTS[NPC_BOSS_VOLCANO].jump_sprites=3;
    NPC_SPRITE_COUNTS[NPC_BOSS_VOLCANO].swim_sprites=0;
    NPC_SPRITE_COUNTS[NPC_BOSS_VOLCANO].fly_sprites=0;
    NPC_SPRITE_COUNTS[NPC_BOSS_VOLCANO].special_attack_sprites=0;
    NPC_SPRITE_COUNTS[NPC_BOSS_VOLCANO].dead_sprites=0;
    NPC_SPRITE_COUNTS[NPC_BOSS_VOLCANO].width=77;
    NPC_SPRITE_COUNTS[NPC_BOSS_VOLCANO].height=43;
    NPC_SPRITE_COUNTS[NPC_BOSS_VOLCANO].dead_width=77;
    NPC_SPRITE_COUNTS[NPC_BOSS_VOLCANO].dead_height=43;
    NPC_SPRITE_COUNTS[NPC_BOSS_VOLCANO].special_width=77;
    NPC_SPRITE_COUNTS[NPC_BOSS_VOLCANO].special_height=43;

    //Boss Pyramid
    NPC_SPRITE_COUNTS[NPC_BOSS_PYRAMID].walk_sprites=0;
    NPC_SPRITE_COUNTS[NPC_BOSS_PYRAMID].jump_sprites=0;
    NPC_SPRITE_COUNTS[NPC_BOSS_PYRAMID].swim_sprites=0;
    NPC_SPRITE_COUNTS[NPC_BOSS_PYRAMID].fly_sprites=3;
    NPC_SPRITE_COUNTS[NPC_BOSS_PYRAMID].special_attack_sprites=0;
    NPC_SPRITE_COUNTS[NPC_BOSS_PYRAMID].dead_sprites=0;
    NPC_SPRITE_COUNTS[NPC_BOSS_PYRAMID].width=77;
    NPC_SPRITE_COUNTS[NPC_BOSS_PYRAMID].height=43;
    NPC_SPRITE_COUNTS[NPC_BOSS_PYRAMID].dead_width=77;
    NPC_SPRITE_COUNTS[NPC_BOSS_PYRAMID].dead_height=43;
    NPC_SPRITE_COUNTS[NPC_BOSS_PYRAMID].special_width=77;
    NPC_SPRITE_COUNTS[NPC_BOSS_PYRAMID].special_height=43;

    //Boss Lighthouse
    NPC_SPRITE_COUNTS[NPC_BOSS_LIGHTHOUSE].walk_sprites=0;
    NPC_SPRITE_COUNTS[NPC_BOSS_LIGHTHOUSE].jump_sprites=0;
    NPC_SPRITE_COUNTS[NPC_BOSS_LIGHTHOUSE].swim_sprites=0;
    NPC_SPRITE_COUNTS[NPC_BOSS_LIGHTHOUSE].fly_sprites=3;
    NPC_SPRITE_COUNTS[NPC_BOSS_LIGHTHOUSE].special_attack_sprites=0;
    NPC_SPRITE_COUNTS[NPC_BOSS_LIGHTHOUSE].dead_sprites=0;
    NPC_SPRITE_COUNTS[NPC_BOSS_LIGHTHOUSE].width=77;
    NPC_SPRITE_COUNTS[NPC_BOSS_LIGHTHOUSE].height=43;
    NPC_SPRITE_COUNTS[NPC_BOSS_LIGHTHOUSE].dead_width=77;
    NPC_SPRITE_COUNTS[NPC_BOSS_LIGHTHOUSE].dead_height=43;
    NPC_SPRITE_COUNTS[NPC_BOSS_LIGHTHOUSE].special_width=77;
    NPC_SPRITE_COUNTS[NPC_BOSS_LIGHTHOUSE].special_height=43;
}
