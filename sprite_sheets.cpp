/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "sprite_sheets.h"
#include "world.h"
#include "render.h"

using namespace std;

//Player:
SDL_Rect sprites_player_worldmap_idle_up[PLAYER_WORLDMAP_IDLE_SPRITES];
SDL_Rect sprites_player_worldmap_idle_down[PLAYER_WORLDMAP_IDLE_SPRITES];
SDL_Rect sprites_player_worldmap_idle_left[PLAYER_WORLDMAP_IDLE_SPRITES];
SDL_Rect sprites_player_worldmap_idle_right[PLAYER_WORLDMAP_IDLE_SPRITES];

SDL_Rect sprites_player_worldmap_walk_up[PLAYER_WORLDMAP_WALK_SPRITES];
SDL_Rect sprites_player_worldmap_walk_down[PLAYER_WORLDMAP_WALK_SPRITES];
SDL_Rect sprites_player_worldmap_walk_left[PLAYER_WORLDMAP_WALK_SPRITES];
SDL_Rect sprites_player_worldmap_walk_right[PLAYER_WORLDMAP_WALK_SPRITES];

SDL_Rect sprites_player_idle_right[PLAYER_IDLE_SPRITES];
SDL_Rect sprites_player_idle_left[PLAYER_IDLE_SPRITES];

SDL_Rect sprites_player_right[PLAYER_WALK_SPRITES];
SDL_Rect sprites_player_left[PLAYER_WALK_SPRITES];

SDL_Rect sprites_player_jump_right[PLAYER_JUMP_SPRITES];
SDL_Rect sprites_player_jump_left[PLAYER_JUMP_SPRITES];

SDL_Rect sprites_player_swim_up_right[PLAYER_SWIM_SPRITES];
SDL_Rect sprites_player_swim_up_left[PLAYER_SWIM_SPRITES];
SDL_Rect sprites_player_swim_right[PLAYER_SWIM_SPRITES];
SDL_Rect sprites_player_swim_left[PLAYER_SWIM_SPRITES];

SDL_Rect sprites_player_shoot_right[PLAYER_SHOOT_SPRITES];
SDL_Rect sprites_player_shoot_left[PLAYER_SHOOT_SPRITES];
SDL_Rect sprites_player_shoot_right_air[PLAYER_SHOOT_SPRITES];
SDL_Rect sprites_player_shoot_left_air[PLAYER_SHOOT_SPRITES];
SDL_Rect sprites_player_shoot_up_left[PLAYER_SHOOT_SPRITES];
SDL_Rect sprites_player_shoot_up_right[PLAYER_SHOOT_SPRITES];
SDL_Rect sprites_player_shoot_up_left_air[PLAYER_SHOOT_SPRITES];
SDL_Rect sprites_player_shoot_up_right_air[PLAYER_SHOOT_SPRITES];
SDL_Rect sprites_player_shoot_down_left_air[PLAYER_SHOOT_SPRITES];
SDL_Rect sprites_player_shoot_down_right_air[PLAYER_SHOOT_SPRITES];
SDL_Rect sprites_player_shoot_climb_right[PLAYER_SHOOT_SPRITES];
SDL_Rect sprites_player_shoot_climb_left[PLAYER_SHOOT_SPRITES];
SDL_Rect sprites_player_shoot_climb_up_right[PLAYER_SHOOT_SPRITES];
SDL_Rect sprites_player_shoot_climb_up_left[PLAYER_SHOOT_SPRITES];
SDL_Rect sprites_player_shoot_climb_down_right[PLAYER_SHOOT_SPRITES];
SDL_Rect sprites_player_shoot_climb_down_left[PLAYER_SHOOT_SPRITES];

SDL_Rect sprites_player_climb_up_left[PLAYER_CLIMB_SPRITES];
SDL_Rect sprites_player_climb_up_right[PLAYER_CLIMB_SPRITES];
SDL_Rect sprites_player_climb_down_left[PLAYER_CLIMB_SPRITES];
SDL_Rect sprites_player_climb_down_right[PLAYER_CLIMB_SPRITES];

SDL_Rect sprites_player_look_right[PLAYER_LOOK_SPRITES];
SDL_Rect sprites_player_look_right_up[PLAYER_LOOK_SPRITES];
SDL_Rect sprites_player_look_right_down[PLAYER_LOOK_SPRITES];
SDL_Rect sprites_player_look_right_climb_up[PLAYER_LOOK_SPRITES];
SDL_Rect sprites_player_look_right_climb_down[PLAYER_LOOK_SPRITES];

SDL_Rect sprites_player_look_left[PLAYER_LOOK_SPRITES];
SDL_Rect sprites_player_look_left_up[PLAYER_LOOK_SPRITES];
SDL_Rect sprites_player_look_left_down[PLAYER_LOOK_SPRITES];
SDL_Rect sprites_player_look_left_climb_up[PLAYER_LOOK_SPRITES];
SDL_Rect sprites_player_look_left_climb_down[PLAYER_LOOK_SPRITES];

SDL_Rect sprites_player_death[PLAYER_DEATH_SPRITES];

SDL_Rect sprites_player_crouch_right[PLAYER_CROUCH_SPRITES];
SDL_Rect sprites_player_crouch_left[PLAYER_CROUCH_SPRITES];

//Powerups:
vector<SDL_Rect> sprites_powerup_jump;

//Tiles:
SDL_Rect sprites_tiles[TILE_SPRITES];
SDL_Rect sprites_water_tiles[WATER_SPRITES];
SDL_Rect worldmap_sprites_tiles[WORLDMAP_TILES];

//Background Tiles:
SDL_Rect sprites_tiles_background[BACKGROUND_TILES];

//Items:
SDL_Rect sprites_item_leaf[ITEM_SPRITES_LEAF];
SDL_Rect sprites_item_cheese[ITEM_SPRITES_CHEESE];
SDL_Rect sprites_item_ammo[ITEM_SPRITES_AMMO];
SDL_Rect sprites_item_spawnpoint[ITEM_SPRITES_SPAWNPOINT];
SDL_Rect sprites_item_checkpoint[ITEM_SPRITES_CHECKPOINT];
SDL_Rect sprites_item_endpoint[ITEM_SPRITES_ENDPOINT];
SDL_Rect sprites_item_endpoint_secret[ITEM_SPRITES_ENDPOINT_SECRET];
SDL_Rect sprites_item_swimming_gear[ITEM_SPRITES_SWIMMING_GEAR];
SDL_Rect sprites_item_key_red[ITEM_SPRITES_KEY_RED];
SDL_Rect sprites_item_key_blue[ITEM_SPRITES_KEY_BLUE];
SDL_Rect sprites_item_key_green[ITEM_SPRITES_KEY_GREEN];
SDL_Rect sprites_item_key_yellow[ITEM_SPRITES_KEY_YELLOW];
SDL_Rect sprites_item_key_orange[ITEM_SPRITES_KEY_ORANGE];
SDL_Rect sprites_item_key_purple[ITEM_SPRITES_KEY_PURPLE];
SDL_Rect sprites_item_towel[ITEM_SPRITES_TOWEL];
SDL_Rect sprites_item_key_gray[ITEM_SPRITES_KEY_GRAY];
SDL_Rect sprites_item_key_brown[ITEM_SPRITES_KEY_BROWN];
SDL_Rect sprites_item_key_black[ITEM_SPRITES_KEY_BLACK];
SDL_Rect sprites_item_key_pink[ITEM_SPRITES_KEY_PINK];
SDL_Rect sprites_item_key_cyan[ITEM_SPRITES_KEY_CYAN];
SDL_Rect sprites_item_sink[ITEM_SPRITES_SINK];
SDL_Rect sprites_item_j_balloon[ITEM_SPRITES_J_BALLOON];
SDL_Rect sprites_item_ammo_barrel[ITEM_SPRITES_AMMO_BARREL];
SDL_Rect sprites_item_candy[ITEM_SPRITES_CANDY];
SDL_Rect sprites_item_candy_black[ITEM_SPRITES_CANDY];
SDL_Rect sprites_item_suit_deadly_water[ITEM_SPRITES_SUIT_DEADLY_WATER];
SDL_Rect sprites_item_suit_sharp[ITEM_SPRITES_SUIT_SHARP];
SDL_Rect sprites_item_suit_banana[ITEM_SPRITES_SUIT_BANANA];
SDL_Rect sprites_item_shot_homing[ITEM_SPRITES_SHOT_HOMING];
SDL_Rect sprites_item_translator[ITEM_SPRITES_TRANSLATOR];
SDL_Rect sprites_item_j_wing[ITEM_SPRITES_J_WING];

//NPCs:
vector<npc_sprite_data> SPRITES_NPC;

//Traps:
vector<trap_sprite_data> SPRITES_TRAP;

//Shots:
vector<shot_sprite_data> SPRITES_SHOT;

//Moving Platforms:
SDL_Rect sprites_moving_platform_left[MOVING_PLATFORM_SPRITES];
SDL_Rect sprites_moving_platform_right[MOVING_PLATFORM_SPRITES];
SDL_Rect sprites_moving_platform_up[MOVING_PLATFORM_SPRITES];
SDL_Rect sprites_moving_platform_down[MOVING_PLATFORM_SPRITES];
SDL_Rect sprites_moving_platform_idle[MOVING_PLATFORM_SPRITES+1];

//Triggers:
vector<trigger_data> sprites_triggers;

//Doors:
SDL_Rect sprites_doors[DOOR_SPRITES][NUMBER_OF_DOORS];
SDL_Rect sprites_key_doors[KEY_DOOR_SPRITES][NUMBER_OF_KEY_DOORS];
SDL_Rect sprites_door_standard[DOOR_STANDARD_SPRITES];

//Signs:
SDL_Rect sprites_signs[SIGN_COUNT];

//Springs:
SDL_Rect sprites_spring[SPRING_SPRITES];

//Boosters:
SDL_Rect sprites_booster[BOOSTER_SPRITES];

//Effects:

//Water Bubble Effect:
SDL_Rect sprites_effect_water_bubble[4];

//Water Splash Effect:
SDL_Rect sprites_effect_water_splash[3];

//Survival:
vector<SDL_Rect> sprites_survival_spawn;

//Interface
vector<SDL_Rect> sprites_shot_types;
vector<SDL_Rect> sprites_moon;

void create_sprite_sheets_npcs(){
    SPRITES_NPC.clear();

    for(short i=0;i<NPC_END;i++){
        SPRITES_NPC.push_back(npc_sprite_data());
    }

    //Slime
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_SLIME].walk_sprites;i++){
        SPRITES_NPC[NPC_SLIME].walk_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SLIME].walk_right[i].x=i*NPC_SPRITE_COUNTS[NPC_SLIME].width;
        SPRITES_NPC[NPC_SLIME].walk_right[i].y=0;
        SPRITES_NPC[NPC_SLIME].walk_right[i].w=NPC_SPRITE_COUNTS[NPC_SLIME].width;
        SPRITES_NPC[NPC_SLIME].walk_right[i].h=NPC_SPRITE_COUNTS[NPC_SLIME].height;

        SPRITES_NPC[NPC_SLIME].walk_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SLIME].walk_left[i].x=i*NPC_SPRITE_COUNTS[NPC_SLIME].width;
        SPRITES_NPC[NPC_SLIME].walk_left[i].y=NPC_SPRITE_COUNTS[NPC_SLIME].height;
        SPRITES_NPC[NPC_SLIME].walk_left[i].w=NPC_SPRITE_COUNTS[NPC_SLIME].width;
        SPRITES_NPC[NPC_SLIME].walk_left[i].h=NPC_SPRITE_COUNTS[NPC_SLIME].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_SLIME].jump_sprites;i++){
        SPRITES_NPC[NPC_SLIME].jump_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SLIME].jump_right[i].x=i*NPC_SPRITE_COUNTS[NPC_SLIME].width;
        SPRITES_NPC[NPC_SLIME].jump_right[i].y=0;
        SPRITES_NPC[NPC_SLIME].jump_right[i].w=NPC_SPRITE_COUNTS[NPC_SLIME].width;
        SPRITES_NPC[NPC_SLIME].jump_right[i].h=NPC_SPRITE_COUNTS[NPC_SLIME].height;

        SPRITES_NPC[NPC_SLIME].jump_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SLIME].jump_left[i].x=i*NPC_SPRITE_COUNTS[NPC_SLIME].width;
        SPRITES_NPC[NPC_SLIME].jump_left[i].y=NPC_SPRITE_COUNTS[NPC_SLIME].height;
        SPRITES_NPC[NPC_SLIME].jump_left[i].w=NPC_SPRITE_COUNTS[NPC_SLIME].width;
        SPRITES_NPC[NPC_SLIME].jump_left[i].h=NPC_SPRITE_COUNTS[NPC_SLIME].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_SLIME].swim_sprites;i++){
        SPRITES_NPC[NPC_SLIME].swim_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SLIME].swim_right[i].x=i*NPC_SPRITE_COUNTS[NPC_SLIME].width;
        SPRITES_NPC[NPC_SLIME].swim_right[i].y=0;
        SPRITES_NPC[NPC_SLIME].swim_right[i].w=NPC_SPRITE_COUNTS[NPC_SLIME].width;
        SPRITES_NPC[NPC_SLIME].swim_right[i].h=NPC_SPRITE_COUNTS[NPC_SLIME].height;

        SPRITES_NPC[NPC_SLIME].swim_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SLIME].swim_left[i].x=i*NPC_SPRITE_COUNTS[NPC_SLIME].width;
        SPRITES_NPC[NPC_SLIME].swim_left[i].y=NPC_SPRITE_COUNTS[NPC_SLIME].height;
        SPRITES_NPC[NPC_SLIME].swim_left[i].w=NPC_SPRITE_COUNTS[NPC_SLIME].width;
        SPRITES_NPC[NPC_SLIME].swim_left[i].h=NPC_SPRITE_COUNTS[NPC_SLIME].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_SLIME].dead_sprites;i++){
        SPRITES_NPC[NPC_SLIME].dead.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SLIME].dead[i].x=i*NPC_SPRITE_COUNTS[NPC_SLIME].dead_width;
        SPRITES_NPC[NPC_SLIME].dead[i].y=NPC_SPRITE_COUNTS[NPC_SLIME].height*2;
        SPRITES_NPC[NPC_SLIME].dead[i].w=NPC_SPRITE_COUNTS[NPC_SLIME].dead_width;
        SPRITES_NPC[NPC_SLIME].dead[i].h=NPC_SPRITE_COUNTS[NPC_SLIME].dead_height;
    }

    //Cow
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_COW].walk_sprites;i++){
        SPRITES_NPC[NPC_COW].walk_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_COW].walk_right[i].x=i*NPC_SPRITE_COUNTS[NPC_COW].width;
        SPRITES_NPC[NPC_COW].walk_right[i].y=0;
        SPRITES_NPC[NPC_COW].walk_right[i].w=NPC_SPRITE_COUNTS[NPC_COW].width;
        SPRITES_NPC[NPC_COW].walk_right[i].h=NPC_SPRITE_COUNTS[NPC_COW].height;

        SPRITES_NPC[NPC_COW].walk_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_COW].walk_left[i].x=i*NPC_SPRITE_COUNTS[NPC_COW].width;
        SPRITES_NPC[NPC_COW].walk_left[i].y=NPC_SPRITE_COUNTS[NPC_COW].height;
        SPRITES_NPC[NPC_COW].walk_left[i].w=NPC_SPRITE_COUNTS[NPC_COW].width;
        SPRITES_NPC[NPC_COW].walk_left[i].h=NPC_SPRITE_COUNTS[NPC_COW].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_COW].jump_sprites;i++){
        SPRITES_NPC[NPC_COW].jump_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_COW].jump_right[i].x=i*NPC_SPRITE_COUNTS[NPC_COW].width;
        SPRITES_NPC[NPC_COW].jump_right[i].y=0;
        SPRITES_NPC[NPC_COW].jump_right[i].w=NPC_SPRITE_COUNTS[NPC_COW].width;
        SPRITES_NPC[NPC_COW].jump_right[i].h=NPC_SPRITE_COUNTS[NPC_COW].height;

        SPRITES_NPC[NPC_COW].jump_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_COW].jump_left[i].x=i*NPC_SPRITE_COUNTS[NPC_COW].width;
        SPRITES_NPC[NPC_COW].jump_left[i].y=NPC_SPRITE_COUNTS[NPC_COW].height;
        SPRITES_NPC[NPC_COW].jump_left[i].w=NPC_SPRITE_COUNTS[NPC_COW].width;
        SPRITES_NPC[NPC_COW].jump_left[i].h=NPC_SPRITE_COUNTS[NPC_COW].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_COW].swim_sprites;i++){
        SPRITES_NPC[NPC_COW].swim_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_COW].swim_right[i].x=i*NPC_SPRITE_COUNTS[NPC_COW].width;
        SPRITES_NPC[NPC_COW].swim_right[i].y=0;
        SPRITES_NPC[NPC_COW].swim_right[i].w=NPC_SPRITE_COUNTS[NPC_COW].width;
        SPRITES_NPC[NPC_COW].swim_right[i].h=NPC_SPRITE_COUNTS[NPC_COW].height;

        SPRITES_NPC[NPC_COW].swim_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_COW].swim_left[i].x=i*NPC_SPRITE_COUNTS[NPC_COW].width;
        SPRITES_NPC[NPC_COW].swim_left[i].y=NPC_SPRITE_COUNTS[NPC_COW].height;
        SPRITES_NPC[NPC_COW].swim_left[i].w=NPC_SPRITE_COUNTS[NPC_COW].width;
        SPRITES_NPC[NPC_COW].swim_left[i].h=NPC_SPRITE_COUNTS[NPC_COW].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_COW].special_attack_sprites;i++){
        SPRITES_NPC[NPC_COW].special_attack_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_COW].special_attack_right[i].x=i*NPC_SPRITE_COUNTS[NPC_COW].special_width;
        SPRITES_NPC[NPC_COW].special_attack_right[i].y=NPC_SPRITE_COUNTS[NPC_COW].height*2;
        SPRITES_NPC[NPC_COW].special_attack_right[i].w=NPC_SPRITE_COUNTS[NPC_COW].special_width;
        SPRITES_NPC[NPC_COW].special_attack_right[i].h=NPC_SPRITE_COUNTS[NPC_COW].special_height;

        SPRITES_NPC[NPC_COW].special_attack_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_COW].special_attack_left[i].x=i*NPC_SPRITE_COUNTS[NPC_COW].special_width;
        SPRITES_NPC[NPC_COW].special_attack_left[i].y=NPC_SPRITE_COUNTS[NPC_COW].height*3;
        SPRITES_NPC[NPC_COW].special_attack_left[i].w=NPC_SPRITE_COUNTS[NPC_COW].special_width;
        SPRITES_NPC[NPC_COW].special_attack_left[i].h=NPC_SPRITE_COUNTS[NPC_COW].special_height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_COW].dead_sprites;i++){
        SPRITES_NPC[NPC_COW].dead.push_back(SDL_Rect());
        SPRITES_NPC[NPC_COW].dead[i].x=i*NPC_SPRITE_COUNTS[NPC_COW].dead_width;
        SPRITES_NPC[NPC_COW].dead[i].y=NPC_SPRITE_COUNTS[NPC_COW].height*4;
        SPRITES_NPC[NPC_COW].dead[i].w=NPC_SPRITE_COUNTS[NPC_COW].dead_width;
        SPRITES_NPC[NPC_COW].dead[i].h=NPC_SPRITE_COUNTS[NPC_COW].dead_height;
    }

    //Bee
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_BEE].fly_sprites;i++){
        SPRITES_NPC[NPC_BEE].fly_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_BEE].fly_right[i].x=i*NPC_SPRITE_COUNTS[NPC_BEE].width;
        SPRITES_NPC[NPC_BEE].fly_right[i].y=0;
        SPRITES_NPC[NPC_BEE].fly_right[i].w=NPC_SPRITE_COUNTS[NPC_BEE].width;
        SPRITES_NPC[NPC_BEE].fly_right[i].h=NPC_SPRITE_COUNTS[NPC_BEE].height;

        SPRITES_NPC[NPC_BEE].fly_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_BEE].fly_left[i].x=i*NPC_SPRITE_COUNTS[NPC_BEE].width;
        SPRITES_NPC[NPC_BEE].fly_left[i].y=NPC_SPRITE_COUNTS[NPC_BEE].height;
        SPRITES_NPC[NPC_BEE].fly_left[i].w=NPC_SPRITE_COUNTS[NPC_BEE].width;
        SPRITES_NPC[NPC_BEE].fly_left[i].h=NPC_SPRITE_COUNTS[NPC_BEE].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_BEE].dead_sprites;i++){
        SPRITES_NPC[NPC_BEE].dead.push_back(SDL_Rect());
        SPRITES_NPC[NPC_BEE].dead[i].x=i*NPC_SPRITE_COUNTS[NPC_BEE].dead_width;
        SPRITES_NPC[NPC_BEE].dead[i].y=NPC_SPRITE_COUNTS[NPC_BEE].height*2;
        SPRITES_NPC[NPC_BEE].dead[i].w=NPC_SPRITE_COUNTS[NPC_BEE].dead_width;
        SPRITES_NPC[NPC_BEE].dead[i].h=NPC_SPRITE_COUNTS[NPC_BEE].dead_height;
    }

    //Replicator
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_REPLICATOR].walk_sprites;i++){
        SPRITES_NPC[NPC_REPLICATOR].walk_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_REPLICATOR].walk_right[i].x=i*NPC_SPRITE_COUNTS[NPC_REPLICATOR].width;
        SPRITES_NPC[NPC_REPLICATOR].walk_right[i].y=0;
        SPRITES_NPC[NPC_REPLICATOR].walk_right[i].w=NPC_SPRITE_COUNTS[NPC_REPLICATOR].width;
        SPRITES_NPC[NPC_REPLICATOR].walk_right[i].h=NPC_SPRITE_COUNTS[NPC_REPLICATOR].height;

        SPRITES_NPC[NPC_REPLICATOR].walk_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_REPLICATOR].walk_left[i].x=i*NPC_SPRITE_COUNTS[NPC_REPLICATOR].width;
        SPRITES_NPC[NPC_REPLICATOR].walk_left[i].y=0;
        SPRITES_NPC[NPC_REPLICATOR].walk_left[i].w=NPC_SPRITE_COUNTS[NPC_REPLICATOR].width;
        SPRITES_NPC[NPC_REPLICATOR].walk_left[i].h=NPC_SPRITE_COUNTS[NPC_REPLICATOR].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_REPLICATOR].jump_sprites;i++){
        SPRITES_NPC[NPC_REPLICATOR].jump_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_REPLICATOR].jump_right[i].x=i*NPC_SPRITE_COUNTS[NPC_REPLICATOR].width;
        SPRITES_NPC[NPC_REPLICATOR].jump_right[i].y=0;
        SPRITES_NPC[NPC_REPLICATOR].jump_right[i].w=NPC_SPRITE_COUNTS[NPC_REPLICATOR].width;
        SPRITES_NPC[NPC_REPLICATOR].jump_right[i].h=NPC_SPRITE_COUNTS[NPC_REPLICATOR].height;

        SPRITES_NPC[NPC_REPLICATOR].jump_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_REPLICATOR].jump_left[i].x=i*NPC_SPRITE_COUNTS[NPC_REPLICATOR].width;
        SPRITES_NPC[NPC_REPLICATOR].jump_left[i].y=0;
        SPRITES_NPC[NPC_REPLICATOR].jump_left[i].w=NPC_SPRITE_COUNTS[NPC_REPLICATOR].width;
        SPRITES_NPC[NPC_REPLICATOR].jump_left[i].h=NPC_SPRITE_COUNTS[NPC_REPLICATOR].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_REPLICATOR].swim_sprites;i++){
        SPRITES_NPC[NPC_REPLICATOR].swim_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_REPLICATOR].swim_right[i].x=i*NPC_SPRITE_COUNTS[NPC_REPLICATOR].width;
        SPRITES_NPC[NPC_REPLICATOR].swim_right[i].y=0;
        SPRITES_NPC[NPC_REPLICATOR].swim_right[i].w=NPC_SPRITE_COUNTS[NPC_REPLICATOR].width;
        SPRITES_NPC[NPC_REPLICATOR].swim_right[i].h=NPC_SPRITE_COUNTS[NPC_REPLICATOR].height;

        SPRITES_NPC[NPC_REPLICATOR].swim_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_REPLICATOR].swim_left[i].x=i*NPC_SPRITE_COUNTS[NPC_REPLICATOR].width;
        SPRITES_NPC[NPC_REPLICATOR].swim_left[i].y=0;
        SPRITES_NPC[NPC_REPLICATOR].swim_left[i].w=NPC_SPRITE_COUNTS[NPC_REPLICATOR].width;
        SPRITES_NPC[NPC_REPLICATOR].swim_left[i].h=NPC_SPRITE_COUNTS[NPC_REPLICATOR].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_REPLICATOR].special_attack_sprites;i++){
        SPRITES_NPC[NPC_REPLICATOR].special_attack_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_REPLICATOR].special_attack_right[i].x=i*NPC_SPRITE_COUNTS[NPC_REPLICATOR].special_width;
        SPRITES_NPC[NPC_REPLICATOR].special_attack_right[i].y=0;
        SPRITES_NPC[NPC_REPLICATOR].special_attack_right[i].w=NPC_SPRITE_COUNTS[NPC_REPLICATOR].special_width;
        SPRITES_NPC[NPC_REPLICATOR].special_attack_right[i].h=NPC_SPRITE_COUNTS[NPC_REPLICATOR].special_height;

        SPRITES_NPC[NPC_REPLICATOR].special_attack_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_REPLICATOR].special_attack_left[i].x=i*NPC_SPRITE_COUNTS[NPC_REPLICATOR].special_width;
        SPRITES_NPC[NPC_REPLICATOR].special_attack_left[i].y=0;
        SPRITES_NPC[NPC_REPLICATOR].special_attack_left[i].w=NPC_SPRITE_COUNTS[NPC_REPLICATOR].special_width;
        SPRITES_NPC[NPC_REPLICATOR].special_attack_left[i].h=NPC_SPRITE_COUNTS[NPC_REPLICATOR].special_height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_REPLICATOR].dead_sprites;i++){
        SPRITES_NPC[NPC_REPLICATOR].dead.push_back(SDL_Rect());
        SPRITES_NPC[NPC_REPLICATOR].dead[i].x=i*NPC_SPRITE_COUNTS[NPC_REPLICATOR].dead_width;
        SPRITES_NPC[NPC_REPLICATOR].dead[i].y=NPC_SPRITE_COUNTS[NPC_REPLICATOR].height;
        SPRITES_NPC[NPC_REPLICATOR].dead[i].w=NPC_SPRITE_COUNTS[NPC_REPLICATOR].dead_width;
        SPRITES_NPC[NPC_REPLICATOR].dead[i].h=NPC_SPRITE_COUNTS[NPC_REPLICATOR].dead_height;
    }

    //Fish
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_FISH].walk_sprites;i++){
        SPRITES_NPC[NPC_FISH].walk_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_FISH].walk_right[i].x=i*NPC_SPRITE_COUNTS[NPC_FISH].width;
        SPRITES_NPC[NPC_FISH].walk_right[i].y=0;
        SPRITES_NPC[NPC_FISH].walk_right[i].w=NPC_SPRITE_COUNTS[NPC_FISH].width;
        SPRITES_NPC[NPC_FISH].walk_right[i].h=NPC_SPRITE_COUNTS[NPC_FISH].height;

        SPRITES_NPC[NPC_FISH].walk_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_FISH].walk_left[i].x=i*NPC_SPRITE_COUNTS[NPC_FISH].width;
        SPRITES_NPC[NPC_FISH].walk_left[i].y=NPC_SPRITE_COUNTS[NPC_FISH].height;
        SPRITES_NPC[NPC_FISH].walk_left[i].w=NPC_SPRITE_COUNTS[NPC_FISH].width;
        SPRITES_NPC[NPC_FISH].walk_left[i].h=NPC_SPRITE_COUNTS[NPC_FISH].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_FISH].jump_sprites;i++){
        SPRITES_NPC[NPC_FISH].jump_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_FISH].jump_right[i].x=i*NPC_SPRITE_COUNTS[NPC_FISH].width;
        SPRITES_NPC[NPC_FISH].jump_right[i].y=0;
        SPRITES_NPC[NPC_FISH].jump_right[i].w=NPC_SPRITE_COUNTS[NPC_FISH].width;
        SPRITES_NPC[NPC_FISH].jump_right[i].h=NPC_SPRITE_COUNTS[NPC_FISH].height;

        SPRITES_NPC[NPC_FISH].jump_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_FISH].jump_left[i].x=i*NPC_SPRITE_COUNTS[NPC_FISH].width;
        SPRITES_NPC[NPC_FISH].jump_left[i].y=NPC_SPRITE_COUNTS[NPC_FISH].height;
        SPRITES_NPC[NPC_FISH].jump_left[i].w=NPC_SPRITE_COUNTS[NPC_FISH].width;
        SPRITES_NPC[NPC_FISH].jump_left[i].h=NPC_SPRITE_COUNTS[NPC_FISH].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_FISH].swim_sprites;i++){
        SPRITES_NPC[NPC_FISH].swim_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_FISH].swim_right[i].x=i*NPC_SPRITE_COUNTS[NPC_FISH].width;
        SPRITES_NPC[NPC_FISH].swim_right[i].y=0;
        SPRITES_NPC[NPC_FISH].swim_right[i].w=NPC_SPRITE_COUNTS[NPC_FISH].width;
        SPRITES_NPC[NPC_FISH].swim_right[i].h=NPC_SPRITE_COUNTS[NPC_FISH].height;

        SPRITES_NPC[NPC_FISH].swim_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_FISH].swim_left[i].x=i*NPC_SPRITE_COUNTS[NPC_FISH].width;
        SPRITES_NPC[NPC_FISH].swim_left[i].y=NPC_SPRITE_COUNTS[NPC_FISH].height;
        SPRITES_NPC[NPC_FISH].swim_left[i].w=NPC_SPRITE_COUNTS[NPC_FISH].width;
        SPRITES_NPC[NPC_FISH].swim_left[i].h=NPC_SPRITE_COUNTS[NPC_FISH].height;
    }

    //Monkey
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_MONKEY].walk_sprites;i++){
        SPRITES_NPC[NPC_MONKEY].walk_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_MONKEY].walk_right[i].x=i*NPC_SPRITE_COUNTS[NPC_MONKEY].width;
        SPRITES_NPC[NPC_MONKEY].walk_right[i].y=0;
        SPRITES_NPC[NPC_MONKEY].walk_right[i].w=NPC_SPRITE_COUNTS[NPC_MONKEY].width;
        SPRITES_NPC[NPC_MONKEY].walk_right[i].h=NPC_SPRITE_COUNTS[NPC_MONKEY].height;

        SPRITES_NPC[NPC_MONKEY].walk_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_MONKEY].walk_left[i].x=i*NPC_SPRITE_COUNTS[NPC_MONKEY].width;
        SPRITES_NPC[NPC_MONKEY].walk_left[i].y=NPC_SPRITE_COUNTS[NPC_MONKEY].height*2;
        SPRITES_NPC[NPC_MONKEY].walk_left[i].w=NPC_SPRITE_COUNTS[NPC_MONKEY].width;
        SPRITES_NPC[NPC_MONKEY].walk_left[i].h=NPC_SPRITE_COUNTS[NPC_MONKEY].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_MONKEY].special_attack_sprites;i++){
        SPRITES_NPC[NPC_MONKEY].special_attack_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_MONKEY].special_attack_right[i].x=i*NPC_SPRITE_COUNTS[NPC_MONKEY].special_width;
        SPRITES_NPC[NPC_MONKEY].special_attack_right[i].y=NPC_SPRITE_COUNTS[NPC_MONKEY].height;
        SPRITES_NPC[NPC_MONKEY].special_attack_right[i].w=NPC_SPRITE_COUNTS[NPC_MONKEY].special_width;
        SPRITES_NPC[NPC_MONKEY].special_attack_right[i].h=NPC_SPRITE_COUNTS[NPC_MONKEY].special_height;

        SPRITES_NPC[NPC_MONKEY].special_attack_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_MONKEY].special_attack_left[i].x=i*NPC_SPRITE_COUNTS[NPC_MONKEY].special_width;
        SPRITES_NPC[NPC_MONKEY].special_attack_left[i].y=NPC_SPRITE_COUNTS[NPC_MONKEY].height*3;
        SPRITES_NPC[NPC_MONKEY].special_attack_left[i].w=NPC_SPRITE_COUNTS[NPC_MONKEY].special_width;
        SPRITES_NPC[NPC_MONKEY].special_attack_left[i].h=NPC_SPRITE_COUNTS[NPC_MONKEY].special_height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_MONKEY].dead_sprites;i++){
        SPRITES_NPC[NPC_MONKEY].dead.push_back(SDL_Rect());
        SPRITES_NPC[NPC_MONKEY].dead[i].x=i*NPC_SPRITE_COUNTS[NPC_MONKEY].dead_width;
        SPRITES_NPC[NPC_MONKEY].dead[i].y=NPC_SPRITE_COUNTS[NPC_MONKEY].height*4;
        SPRITES_NPC[NPC_MONKEY].dead[i].w=NPC_SPRITE_COUNTS[NPC_MONKEY].dead_width;
        SPRITES_NPC[NPC_MONKEY].dead[i].h=NPC_SPRITE_COUNTS[NPC_MONKEY].dead_height;
    }

    //Laser spinner
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_LASER_SPINNER].walk_sprites;i++){
        SPRITES_NPC[NPC_LASER_SPINNER].walk_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_LASER_SPINNER].walk_right[i].x=i*NPC_SPRITE_COUNTS[NPC_LASER_SPINNER].width;
        SPRITES_NPC[NPC_LASER_SPINNER].walk_right[i].y=0;
        SPRITES_NPC[NPC_LASER_SPINNER].walk_right[i].w=NPC_SPRITE_COUNTS[NPC_LASER_SPINNER].width;
        SPRITES_NPC[NPC_LASER_SPINNER].walk_right[i].h=NPC_SPRITE_COUNTS[NPC_LASER_SPINNER].height;

        SPRITES_NPC[NPC_LASER_SPINNER].walk_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_LASER_SPINNER].walk_left[i].x=i*NPC_SPRITE_COUNTS[NPC_LASER_SPINNER].width;
        SPRITES_NPC[NPC_LASER_SPINNER].walk_left[i].y=NPC_SPRITE_COUNTS[NPC_LASER_SPINNER].height*2;
        SPRITES_NPC[NPC_LASER_SPINNER].walk_left[i].w=NPC_SPRITE_COUNTS[NPC_LASER_SPINNER].width;
        SPRITES_NPC[NPC_LASER_SPINNER].walk_left[i].h=NPC_SPRITE_COUNTS[NPC_LASER_SPINNER].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_LASER_SPINNER].special_attack_sprites;i++){
        SPRITES_NPC[NPC_LASER_SPINNER].special_attack_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_LASER_SPINNER].special_attack_right[i].x=i*NPC_SPRITE_COUNTS[NPC_LASER_SPINNER].special_width;
        SPRITES_NPC[NPC_LASER_SPINNER].special_attack_right[i].y=NPC_SPRITE_COUNTS[NPC_LASER_SPINNER].height;
        SPRITES_NPC[NPC_LASER_SPINNER].special_attack_right[i].w=NPC_SPRITE_COUNTS[NPC_LASER_SPINNER].special_width;
        SPRITES_NPC[NPC_LASER_SPINNER].special_attack_right[i].h=NPC_SPRITE_COUNTS[NPC_LASER_SPINNER].special_height;

        SPRITES_NPC[NPC_LASER_SPINNER].special_attack_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_LASER_SPINNER].special_attack_left[i].x=i*NPC_SPRITE_COUNTS[NPC_LASER_SPINNER].special_width;
        SPRITES_NPC[NPC_LASER_SPINNER].special_attack_left[i].y=NPC_SPRITE_COUNTS[NPC_LASER_SPINNER].height*3;
        SPRITES_NPC[NPC_LASER_SPINNER].special_attack_left[i].w=NPC_SPRITE_COUNTS[NPC_LASER_SPINNER].special_width;
        SPRITES_NPC[NPC_LASER_SPINNER].special_attack_left[i].h=NPC_SPRITE_COUNTS[NPC_LASER_SPINNER].special_height;
    }

    //Penguin
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_PENGUIN].walk_sprites;i++){
        SPRITES_NPC[NPC_PENGUIN].walk_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_PENGUIN].walk_right[i].x=i*NPC_SPRITE_COUNTS[NPC_PENGUIN].width;
        SPRITES_NPC[NPC_PENGUIN].walk_right[i].y=0;
        SPRITES_NPC[NPC_PENGUIN].walk_right[i].w=NPC_SPRITE_COUNTS[NPC_PENGUIN].width;
        SPRITES_NPC[NPC_PENGUIN].walk_right[i].h=NPC_SPRITE_COUNTS[NPC_PENGUIN].height;

        SPRITES_NPC[NPC_PENGUIN].walk_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_PENGUIN].walk_left[i].x=i*NPC_SPRITE_COUNTS[NPC_PENGUIN].width;
        SPRITES_NPC[NPC_PENGUIN].walk_left[i].y=NPC_SPRITE_COUNTS[NPC_PENGUIN].height;
        SPRITES_NPC[NPC_PENGUIN].walk_left[i].w=NPC_SPRITE_COUNTS[NPC_PENGUIN].width;
        SPRITES_NPC[NPC_PENGUIN].walk_left[i].h=NPC_SPRITE_COUNTS[NPC_PENGUIN].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_PENGUIN].jump_sprites;i++){
        SPRITES_NPC[NPC_PENGUIN].jump_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_PENGUIN].jump_right[i].x=i*NPC_SPRITE_COUNTS[NPC_PENGUIN].width;
        SPRITES_NPC[NPC_PENGUIN].jump_right[i].y=0;
        SPRITES_NPC[NPC_PENGUIN].jump_right[i].w=NPC_SPRITE_COUNTS[NPC_PENGUIN].width;
        SPRITES_NPC[NPC_PENGUIN].jump_right[i].h=NPC_SPRITE_COUNTS[NPC_PENGUIN].height;

        SPRITES_NPC[NPC_PENGUIN].jump_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_PENGUIN].jump_left[i].x=i*NPC_SPRITE_COUNTS[NPC_PENGUIN].width;
        SPRITES_NPC[NPC_PENGUIN].jump_left[i].y=NPC_SPRITE_COUNTS[NPC_PENGUIN].height;
        SPRITES_NPC[NPC_PENGUIN].jump_left[i].w=NPC_SPRITE_COUNTS[NPC_PENGUIN].width;
        SPRITES_NPC[NPC_PENGUIN].jump_left[i].h=NPC_SPRITE_COUNTS[NPC_PENGUIN].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_PENGUIN].swim_sprites;i++){
        SPRITES_NPC[NPC_PENGUIN].swim_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_PENGUIN].swim_right[i].x=i*NPC_SPRITE_COUNTS[NPC_PENGUIN].width;
        SPRITES_NPC[NPC_PENGUIN].swim_right[i].y=0;
        SPRITES_NPC[NPC_PENGUIN].swim_right[i].w=NPC_SPRITE_COUNTS[NPC_PENGUIN].width;
        SPRITES_NPC[NPC_PENGUIN].swim_right[i].h=NPC_SPRITE_COUNTS[NPC_PENGUIN].height;

        SPRITES_NPC[NPC_PENGUIN].swim_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_PENGUIN].swim_left[i].x=i*NPC_SPRITE_COUNTS[NPC_PENGUIN].width;
        SPRITES_NPC[NPC_PENGUIN].swim_left[i].y=NPC_SPRITE_COUNTS[NPC_PENGUIN].height;
        SPRITES_NPC[NPC_PENGUIN].swim_left[i].w=NPC_SPRITE_COUNTS[NPC_PENGUIN].width;
        SPRITES_NPC[NPC_PENGUIN].swim_left[i].h=NPC_SPRITE_COUNTS[NPC_PENGUIN].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_PENGUIN].dead_sprites;i++){
        SPRITES_NPC[NPC_PENGUIN].dead.push_back(SDL_Rect());
        SPRITES_NPC[NPC_PENGUIN].dead[i].x=i*NPC_SPRITE_COUNTS[NPC_PENGUIN].dead_width;
        SPRITES_NPC[NPC_PENGUIN].dead[i].y=NPC_SPRITE_COUNTS[NPC_PENGUIN].height*2;
        SPRITES_NPC[NPC_PENGUIN].dead[i].w=NPC_SPRITE_COUNTS[NPC_PENGUIN].dead_width;
        SPRITES_NPC[NPC_PENGUIN].dead[i].h=NPC_SPRITE_COUNTS[NPC_PENGUIN].dead_height;
    }

    //Fish on ice
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE].walk_sprites;i++){
        SPRITES_NPC[NPC_FISH_ON_ICE].walk_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_FISH_ON_ICE].walk_right[i].x=i*NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE].width;
        SPRITES_NPC[NPC_FISH_ON_ICE].walk_right[i].y=0;
        SPRITES_NPC[NPC_FISH_ON_ICE].walk_right[i].w=NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE].width;
        SPRITES_NPC[NPC_FISH_ON_ICE].walk_right[i].h=NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE].height;

        SPRITES_NPC[NPC_FISH_ON_ICE].walk_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_FISH_ON_ICE].walk_left[i].x=i*NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE].width;
        SPRITES_NPC[NPC_FISH_ON_ICE].walk_left[i].y=NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE].height;
        SPRITES_NPC[NPC_FISH_ON_ICE].walk_left[i].w=NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE].width;
        SPRITES_NPC[NPC_FISH_ON_ICE].walk_left[i].h=NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE].jump_sprites;i++){
        SPRITES_NPC[NPC_FISH_ON_ICE].jump_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_FISH_ON_ICE].jump_right[i].x=i*NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE].width;
        SPRITES_NPC[NPC_FISH_ON_ICE].jump_right[i].y=0;
        SPRITES_NPC[NPC_FISH_ON_ICE].jump_right[i].w=NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE].width;
        SPRITES_NPC[NPC_FISH_ON_ICE].jump_right[i].h=NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE].height;

        SPRITES_NPC[NPC_FISH_ON_ICE].jump_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_FISH_ON_ICE].jump_left[i].x=i*NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE].width;
        SPRITES_NPC[NPC_FISH_ON_ICE].jump_left[i].y=NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE].height;
        SPRITES_NPC[NPC_FISH_ON_ICE].jump_left[i].w=NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE].width;
        SPRITES_NPC[NPC_FISH_ON_ICE].jump_left[i].h=NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE].swim_sprites;i++){
        SPRITES_NPC[NPC_FISH_ON_ICE].swim_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_FISH_ON_ICE].swim_right[i].x=i*NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE].width;
        SPRITES_NPC[NPC_FISH_ON_ICE].swim_right[i].y=0;
        SPRITES_NPC[NPC_FISH_ON_ICE].swim_right[i].w=NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE].width;
        SPRITES_NPC[NPC_FISH_ON_ICE].swim_right[i].h=NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE].height;

        SPRITES_NPC[NPC_FISH_ON_ICE].swim_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_FISH_ON_ICE].swim_left[i].x=i*NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE].width;
        SPRITES_NPC[NPC_FISH_ON_ICE].swim_left[i].y=NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE].height;
        SPRITES_NPC[NPC_FISH_ON_ICE].swim_left[i].w=NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE].width;
        SPRITES_NPC[NPC_FISH_ON_ICE].swim_left[i].h=NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE].dead_sprites;i++){
        SPRITES_NPC[NPC_FISH_ON_ICE].dead.push_back(SDL_Rect());
        SPRITES_NPC[NPC_FISH_ON_ICE].dead[i].x=i*NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE].dead_width;
        SPRITES_NPC[NPC_FISH_ON_ICE].dead[i].y=NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE].height*2;
        SPRITES_NPC[NPC_FISH_ON_ICE].dead[i].w=NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE].dead_width;
        SPRITES_NPC[NPC_FISH_ON_ICE].dead[i].h=NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE].dead_height;
    }

    //Penguin harmless
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_PENGUIN_HARMLESS].dead_sprites;i++){
        SPRITES_NPC[NPC_PENGUIN_HARMLESS].dead.push_back(SDL_Rect());
        SPRITES_NPC[NPC_PENGUIN_HARMLESS].dead[i].x=i*NPC_SPRITE_COUNTS[NPC_PENGUIN_HARMLESS].dead_width;
        SPRITES_NPC[NPC_PENGUIN_HARMLESS].dead[i].y=0;
        SPRITES_NPC[NPC_PENGUIN_HARMLESS].dead[i].w=NPC_SPRITE_COUNTS[NPC_PENGUIN_HARMLESS].dead_width;
        SPRITES_NPC[NPC_PENGUIN_HARMLESS].dead[i].h=NPC_SPRITE_COUNTS[NPC_PENGUIN_HARMLESS].dead_height;
    }

    //Fish on ice harmless
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE_HARMLESS].dead_sprites;i++){
        SPRITES_NPC[NPC_FISH_ON_ICE_HARMLESS].dead.push_back(SDL_Rect());
        SPRITES_NPC[NPC_FISH_ON_ICE_HARMLESS].dead[i].x=i*NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE_HARMLESS].dead_width;
        SPRITES_NPC[NPC_FISH_ON_ICE_HARMLESS].dead[i].y=0;
        SPRITES_NPC[NPC_FISH_ON_ICE_HARMLESS].dead[i].w=NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE_HARMLESS].dead_width;
        SPRITES_NPC[NPC_FISH_ON_ICE_HARMLESS].dead[i].h=NPC_SPRITE_COUNTS[NPC_FISH_ON_ICE_HARMLESS].dead_height;
    }

    //Sword knight
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].walk_sprites;i++){
        SPRITES_NPC[NPC_SWORD_KNIGHT].walk_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SWORD_KNIGHT].walk_right[i].x=i*NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].width;
        SPRITES_NPC[NPC_SWORD_KNIGHT].walk_right[i].y=0;
        SPRITES_NPC[NPC_SWORD_KNIGHT].walk_right[i].w=NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].width;
        SPRITES_NPC[NPC_SWORD_KNIGHT].walk_right[i].h=NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].height;

        SPRITES_NPC[NPC_SWORD_KNIGHT].walk_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SWORD_KNIGHT].walk_left[i].x=i*NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].width;
        SPRITES_NPC[NPC_SWORD_KNIGHT].walk_left[i].y=NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].height;
        SPRITES_NPC[NPC_SWORD_KNIGHT].walk_left[i].w=NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].width;
        SPRITES_NPC[NPC_SWORD_KNIGHT].walk_left[i].h=NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].jump_sprites;i++){
        SPRITES_NPC[NPC_SWORD_KNIGHT].jump_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SWORD_KNIGHT].jump_right[i].x=i*NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].width;
        SPRITES_NPC[NPC_SWORD_KNIGHT].jump_right[i].y=0;
        SPRITES_NPC[NPC_SWORD_KNIGHT].jump_right[i].w=NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].width;
        SPRITES_NPC[NPC_SWORD_KNIGHT].jump_right[i].h=NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].height;

        SPRITES_NPC[NPC_SWORD_KNIGHT].jump_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SWORD_KNIGHT].jump_left[i].x=i*NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].width;
        SPRITES_NPC[NPC_SWORD_KNIGHT].jump_left[i].y=NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].height;
        SPRITES_NPC[NPC_SWORD_KNIGHT].jump_left[i].w=NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].width;
        SPRITES_NPC[NPC_SWORD_KNIGHT].jump_left[i].h=NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].swim_sprites;i++){
        SPRITES_NPC[NPC_SWORD_KNIGHT].swim_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SWORD_KNIGHT].swim_right[i].x=i*NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].width;
        SPRITES_NPC[NPC_SWORD_KNIGHT].swim_right[i].y=0;
        SPRITES_NPC[NPC_SWORD_KNIGHT].swim_right[i].w=NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].width;
        SPRITES_NPC[NPC_SWORD_KNIGHT].swim_right[i].h=NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].height;

        SPRITES_NPC[NPC_SWORD_KNIGHT].swim_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SWORD_KNIGHT].swim_left[i].x=i*NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].width;
        SPRITES_NPC[NPC_SWORD_KNIGHT].swim_left[i].y=NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].height;
        SPRITES_NPC[NPC_SWORD_KNIGHT].swim_left[i].w=NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].width;
        SPRITES_NPC[NPC_SWORD_KNIGHT].swim_left[i].h=NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].special_attack_sprites;i++){
        SPRITES_NPC[NPC_SWORD_KNIGHT].special_attack_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SWORD_KNIGHT].special_attack_right[i].x=i*NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].special_width;
        SPRITES_NPC[NPC_SWORD_KNIGHT].special_attack_right[i].y=NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].height*2;
        SPRITES_NPC[NPC_SWORD_KNIGHT].special_attack_right[i].w=NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].special_width;
        SPRITES_NPC[NPC_SWORD_KNIGHT].special_attack_right[i].h=NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].special_height;

        SPRITES_NPC[NPC_SWORD_KNIGHT].special_attack_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SWORD_KNIGHT].special_attack_left[i].x=i*NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].special_width;
        SPRITES_NPC[NPC_SWORD_KNIGHT].special_attack_left[i].y=NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].height*3;
        SPRITES_NPC[NPC_SWORD_KNIGHT].special_attack_left[i].w=NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].special_width;
        SPRITES_NPC[NPC_SWORD_KNIGHT].special_attack_left[i].h=NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].special_height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].dead_sprites;i++){
        SPRITES_NPC[NPC_SWORD_KNIGHT].dead.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SWORD_KNIGHT].dead[i].x=i*NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].dead_width;
        SPRITES_NPC[NPC_SWORD_KNIGHT].dead[i].y=NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].height*4;
        SPRITES_NPC[NPC_SWORD_KNIGHT].dead[i].w=NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].dead_width;
        SPRITES_NPC[NPC_SWORD_KNIGHT].dead[i].h=NPC_SPRITE_COUNTS[NPC_SWORD_KNIGHT].dead_height;
    }

    //Ball knight
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].walk_sprites;i++){
        SPRITES_NPC[NPC_BALL_KNIGHT].walk_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_BALL_KNIGHT].walk_right[i].x=i*NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].width;
        SPRITES_NPC[NPC_BALL_KNIGHT].walk_right[i].y=0;
        SPRITES_NPC[NPC_BALL_KNIGHT].walk_right[i].w=NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].width;
        SPRITES_NPC[NPC_BALL_KNIGHT].walk_right[i].h=NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].height;

        SPRITES_NPC[NPC_BALL_KNIGHT].walk_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_BALL_KNIGHT].walk_left[i].x=i*NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].width;
        SPRITES_NPC[NPC_BALL_KNIGHT].walk_left[i].y=NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].height;
        SPRITES_NPC[NPC_BALL_KNIGHT].walk_left[i].w=NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].width;
        SPRITES_NPC[NPC_BALL_KNIGHT].walk_left[i].h=NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].jump_sprites;i++){
        SPRITES_NPC[NPC_BALL_KNIGHT].jump_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_BALL_KNIGHT].jump_right[i].x=i*NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].width;
        SPRITES_NPC[NPC_BALL_KNIGHT].jump_right[i].y=0;
        SPRITES_NPC[NPC_BALL_KNIGHT].jump_right[i].w=NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].width;
        SPRITES_NPC[NPC_BALL_KNIGHT].jump_right[i].h=NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].height;

        SPRITES_NPC[NPC_BALL_KNIGHT].jump_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_BALL_KNIGHT].jump_left[i].x=i*NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].width;
        SPRITES_NPC[NPC_BALL_KNIGHT].jump_left[i].y=NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].height;
        SPRITES_NPC[NPC_BALL_KNIGHT].jump_left[i].w=NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].width;
        SPRITES_NPC[NPC_BALL_KNIGHT].jump_left[i].h=NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].swim_sprites;i++){
        SPRITES_NPC[NPC_BALL_KNIGHT].swim_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_BALL_KNIGHT].swim_right[i].x=i*NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].width;
        SPRITES_NPC[NPC_BALL_KNIGHT].swim_right[i].y=0;
        SPRITES_NPC[NPC_BALL_KNIGHT].swim_right[i].w=NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].width;
        SPRITES_NPC[NPC_BALL_KNIGHT].swim_right[i].h=NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].height;

        SPRITES_NPC[NPC_BALL_KNIGHT].swim_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_BALL_KNIGHT].swim_left[i].x=i*NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].width;
        SPRITES_NPC[NPC_BALL_KNIGHT].swim_left[i].y=NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].height;
        SPRITES_NPC[NPC_BALL_KNIGHT].swim_left[i].w=NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].width;
        SPRITES_NPC[NPC_BALL_KNIGHT].swim_left[i].h=NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].special_attack_sprites;i++){
        SPRITES_NPC[NPC_BALL_KNIGHT].special_attack_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_BALL_KNIGHT].special_attack_right[i].x=i*NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].special_width;
        SPRITES_NPC[NPC_BALL_KNIGHT].special_attack_right[i].y=NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].height*2;
        SPRITES_NPC[NPC_BALL_KNIGHT].special_attack_right[i].w=NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].special_width;
        SPRITES_NPC[NPC_BALL_KNIGHT].special_attack_right[i].h=NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].special_height;

        SPRITES_NPC[NPC_BALL_KNIGHT].special_attack_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_BALL_KNIGHT].special_attack_left[i].x=i*NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].special_width;
        SPRITES_NPC[NPC_BALL_KNIGHT].special_attack_left[i].y=NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].height*3;
        SPRITES_NPC[NPC_BALL_KNIGHT].special_attack_left[i].w=NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].special_width;
        SPRITES_NPC[NPC_BALL_KNIGHT].special_attack_left[i].h=NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].special_height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].dead_sprites;i++){
        SPRITES_NPC[NPC_BALL_KNIGHT].dead.push_back(SDL_Rect());
        SPRITES_NPC[NPC_BALL_KNIGHT].dead[i].x=i*NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].dead_width;
        SPRITES_NPC[NPC_BALL_KNIGHT].dead[i].y=NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].height*4;
        SPRITES_NPC[NPC_BALL_KNIGHT].dead[i].w=NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].dead_width;
        SPRITES_NPC[NPC_BALL_KNIGHT].dead[i].h=NPC_SPRITE_COUNTS[NPC_BALL_KNIGHT].dead_height;
    }

    //Archer
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_ARCHER].walk_sprites;i++){
        SPRITES_NPC[NPC_ARCHER].walk_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_ARCHER].walk_right[i].x=i*NPC_SPRITE_COUNTS[NPC_ARCHER].width;
        SPRITES_NPC[NPC_ARCHER].walk_right[i].y=0;
        SPRITES_NPC[NPC_ARCHER].walk_right[i].w=NPC_SPRITE_COUNTS[NPC_ARCHER].width;
        SPRITES_NPC[NPC_ARCHER].walk_right[i].h=NPC_SPRITE_COUNTS[NPC_ARCHER].height;

        SPRITES_NPC[NPC_ARCHER].walk_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_ARCHER].walk_left[i].x=i*NPC_SPRITE_COUNTS[NPC_ARCHER].width;
        SPRITES_NPC[NPC_ARCHER].walk_left[i].y=NPC_SPRITE_COUNTS[NPC_ARCHER].height*2;
        SPRITES_NPC[NPC_ARCHER].walk_left[i].w=NPC_SPRITE_COUNTS[NPC_ARCHER].width;
        SPRITES_NPC[NPC_ARCHER].walk_left[i].h=NPC_SPRITE_COUNTS[NPC_ARCHER].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_ARCHER].special_attack_sprites;i++){
        SPRITES_NPC[NPC_ARCHER].special_attack_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_ARCHER].special_attack_right[i].x=i*NPC_SPRITE_COUNTS[NPC_ARCHER].special_width;
        SPRITES_NPC[NPC_ARCHER].special_attack_right[i].y=NPC_SPRITE_COUNTS[NPC_ARCHER].height;
        SPRITES_NPC[NPC_ARCHER].special_attack_right[i].w=NPC_SPRITE_COUNTS[NPC_ARCHER].special_width;
        SPRITES_NPC[NPC_ARCHER].special_attack_right[i].h=NPC_SPRITE_COUNTS[NPC_ARCHER].special_height;

        SPRITES_NPC[NPC_ARCHER].special_attack_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_ARCHER].special_attack_left[i].x=i*NPC_SPRITE_COUNTS[NPC_ARCHER].special_width;
        SPRITES_NPC[NPC_ARCHER].special_attack_left[i].y=NPC_SPRITE_COUNTS[NPC_ARCHER].height*3;
        SPRITES_NPC[NPC_ARCHER].special_attack_left[i].w=NPC_SPRITE_COUNTS[NPC_ARCHER].special_width;
        SPRITES_NPC[NPC_ARCHER].special_attack_left[i].h=NPC_SPRITE_COUNTS[NPC_ARCHER].special_height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_ARCHER].dead_sprites;i++){
        SPRITES_NPC[NPC_ARCHER].dead.push_back(SDL_Rect());
        SPRITES_NPC[NPC_ARCHER].dead[i].x=i*NPC_SPRITE_COUNTS[NPC_ARCHER].dead_width;
        SPRITES_NPC[NPC_ARCHER].dead[i].y=NPC_SPRITE_COUNTS[NPC_ARCHER].height*4;
        SPRITES_NPC[NPC_ARCHER].dead[i].w=NPC_SPRITE_COUNTS[NPC_ARCHER].dead_width;
        SPRITES_NPC[NPC_ARCHER].dead[i].h=NPC_SPRITE_COUNTS[NPC_ARCHER].dead_height;
    }

    //Scarecrow
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_SCARECROW].walk_sprites;i++){
        SPRITES_NPC[NPC_SCARECROW].walk_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SCARECROW].walk_right[i].x=i*NPC_SPRITE_COUNTS[NPC_SCARECROW].width;
        SPRITES_NPC[NPC_SCARECROW].walk_right[i].y=0;
        SPRITES_NPC[NPC_SCARECROW].walk_right[i].w=NPC_SPRITE_COUNTS[NPC_SCARECROW].width;
        SPRITES_NPC[NPC_SCARECROW].walk_right[i].h=NPC_SPRITE_COUNTS[NPC_SCARECROW].height;

        SPRITES_NPC[NPC_SCARECROW].walk_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SCARECROW].walk_left[i].x=i*NPC_SPRITE_COUNTS[NPC_SCARECROW].width;
        SPRITES_NPC[NPC_SCARECROW].walk_left[i].y=0;
        SPRITES_NPC[NPC_SCARECROW].walk_left[i].w=NPC_SPRITE_COUNTS[NPC_SCARECROW].width;
        SPRITES_NPC[NPC_SCARECROW].walk_left[i].h=NPC_SPRITE_COUNTS[NPC_SCARECROW].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_SCARECROW].special_attack_sprites;i++){
        SPRITES_NPC[NPC_SCARECROW].special_attack_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SCARECROW].special_attack_right[i].x=i*NPC_SPRITE_COUNTS[NPC_SCARECROW].special_width;
        SPRITES_NPC[NPC_SCARECROW].special_attack_right[i].y=NPC_SPRITE_COUNTS[NPC_SCARECROW].height;
        SPRITES_NPC[NPC_SCARECROW].special_attack_right[i].w=NPC_SPRITE_COUNTS[NPC_SCARECROW].special_width;
        SPRITES_NPC[NPC_SCARECROW].special_attack_right[i].h=NPC_SPRITE_COUNTS[NPC_SCARECROW].special_height;

        SPRITES_NPC[NPC_SCARECROW].special_attack_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SCARECROW].special_attack_left[i].x=i*NPC_SPRITE_COUNTS[NPC_SCARECROW].special_width;
        SPRITES_NPC[NPC_SCARECROW].special_attack_left[i].y=NPC_SPRITE_COUNTS[NPC_SCARECROW].height+NPC_SPRITE_COUNTS[NPC_SCARECROW].special_height;
        SPRITES_NPC[NPC_SCARECROW].special_attack_left[i].w=NPC_SPRITE_COUNTS[NPC_SCARECROW].special_width;
        SPRITES_NPC[NPC_SCARECROW].special_attack_left[i].h=NPC_SPRITE_COUNTS[NPC_SCARECROW].special_height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_SCARECROW].dead_sprites;i++){
        SPRITES_NPC[NPC_SCARECROW].dead.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SCARECROW].dead[i].x=i*NPC_SPRITE_COUNTS[NPC_SCARECROW].dead_width;
        SPRITES_NPC[NPC_SCARECROW].dead[i].y=NPC_SPRITE_COUNTS[NPC_SCARECROW].height+NPC_SPRITE_COUNTS[NPC_SCARECROW].special_height*2;
        SPRITES_NPC[NPC_SCARECROW].dead[i].w=NPC_SPRITE_COUNTS[NPC_SCARECROW].dead_width;
        SPRITES_NPC[NPC_SCARECROW].dead[i].h=NPC_SPRITE_COUNTS[NPC_SCARECROW].dead_height;
    }

    //Pumpkin
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_PUMPKIN].walk_sprites;i++){
        SPRITES_NPC[NPC_PUMPKIN].walk_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_PUMPKIN].walk_right[i].x=i*NPC_SPRITE_COUNTS[NPC_PUMPKIN].width;
        SPRITES_NPC[NPC_PUMPKIN].walk_right[i].y=0;
        SPRITES_NPC[NPC_PUMPKIN].walk_right[i].w=NPC_SPRITE_COUNTS[NPC_PUMPKIN].width;
        SPRITES_NPC[NPC_PUMPKIN].walk_right[i].h=NPC_SPRITE_COUNTS[NPC_PUMPKIN].height;

        SPRITES_NPC[NPC_PUMPKIN].walk_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_PUMPKIN].walk_left[i].x=i*NPC_SPRITE_COUNTS[NPC_PUMPKIN].width;
        SPRITES_NPC[NPC_PUMPKIN].walk_left[i].y=NPC_SPRITE_COUNTS[NPC_PUMPKIN].height;
        SPRITES_NPC[NPC_PUMPKIN].walk_left[i].w=NPC_SPRITE_COUNTS[NPC_PUMPKIN].width;
        SPRITES_NPC[NPC_PUMPKIN].walk_left[i].h=NPC_SPRITE_COUNTS[NPC_PUMPKIN].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_PUMPKIN].jump_sprites;i++){
        SPRITES_NPC[NPC_PUMPKIN].jump_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_PUMPKIN].jump_right[i].x=i*NPC_SPRITE_COUNTS[NPC_PUMPKIN].width;
        SPRITES_NPC[NPC_PUMPKIN].jump_right[i].y=NPC_SPRITE_COUNTS[NPC_PUMPKIN].height*2;
        SPRITES_NPC[NPC_PUMPKIN].jump_right[i].w=NPC_SPRITE_COUNTS[NPC_PUMPKIN].width;
        SPRITES_NPC[NPC_PUMPKIN].jump_right[i].h=NPC_SPRITE_COUNTS[NPC_PUMPKIN].height;

        SPRITES_NPC[NPC_PUMPKIN].jump_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_PUMPKIN].jump_left[i].x=i*NPC_SPRITE_COUNTS[NPC_PUMPKIN].width;
        SPRITES_NPC[NPC_PUMPKIN].jump_left[i].y=NPC_SPRITE_COUNTS[NPC_PUMPKIN].height*3;
        SPRITES_NPC[NPC_PUMPKIN].jump_left[i].w=NPC_SPRITE_COUNTS[NPC_PUMPKIN].width;
        SPRITES_NPC[NPC_PUMPKIN].jump_left[i].h=NPC_SPRITE_COUNTS[NPC_PUMPKIN].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_PUMPKIN].swim_sprites;i++){
        SPRITES_NPC[NPC_PUMPKIN].swim_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_PUMPKIN].swim_right[i].x=i*NPC_SPRITE_COUNTS[NPC_PUMPKIN].width;
        SPRITES_NPC[NPC_PUMPKIN].swim_right[i].y=0;
        SPRITES_NPC[NPC_PUMPKIN].swim_right[i].w=NPC_SPRITE_COUNTS[NPC_PUMPKIN].width;
        SPRITES_NPC[NPC_PUMPKIN].swim_right[i].h=NPC_SPRITE_COUNTS[NPC_PUMPKIN].height;

        SPRITES_NPC[NPC_PUMPKIN].swim_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_PUMPKIN].swim_left[i].x=i*NPC_SPRITE_COUNTS[NPC_PUMPKIN].width;
        SPRITES_NPC[NPC_PUMPKIN].swim_left[i].y=NPC_SPRITE_COUNTS[NPC_PUMPKIN].height;
        SPRITES_NPC[NPC_PUMPKIN].swim_left[i].w=NPC_SPRITE_COUNTS[NPC_PUMPKIN].width;
        SPRITES_NPC[NPC_PUMPKIN].swim_left[i].h=NPC_SPRITE_COUNTS[NPC_PUMPKIN].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_PUMPKIN].dead_sprites;i++){
        SPRITES_NPC[NPC_PUMPKIN].dead.push_back(SDL_Rect());
        SPRITES_NPC[NPC_PUMPKIN].dead[i].x=i*NPC_SPRITE_COUNTS[NPC_PUMPKIN].dead_width;
        SPRITES_NPC[NPC_PUMPKIN].dead[i].y=NPC_SPRITE_COUNTS[NPC_PUMPKIN].height*4;
        SPRITES_NPC[NPC_PUMPKIN].dead[i].w=NPC_SPRITE_COUNTS[NPC_PUMPKIN].dead_width;
        SPRITES_NPC[NPC_PUMPKIN].dead[i].h=NPC_SPRITE_COUNTS[NPC_PUMPKIN].dead_height;
    }

    //Zombie cow
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_ZOMBIE_COW].walk_sprites;i++){
        SPRITES_NPC[NPC_ZOMBIE_COW].walk_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_ZOMBIE_COW].walk_right[i].x=i*NPC_SPRITE_COUNTS[NPC_ZOMBIE_COW].width;
        SPRITES_NPC[NPC_ZOMBIE_COW].walk_right[i].y=0;
        SPRITES_NPC[NPC_ZOMBIE_COW].walk_right[i].w=NPC_SPRITE_COUNTS[NPC_ZOMBIE_COW].width;
        SPRITES_NPC[NPC_ZOMBIE_COW].walk_right[i].h=NPC_SPRITE_COUNTS[NPC_ZOMBIE_COW].height;

        SPRITES_NPC[NPC_ZOMBIE_COW].walk_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_ZOMBIE_COW].walk_left[i].x=i*NPC_SPRITE_COUNTS[NPC_ZOMBIE_COW].width;
        SPRITES_NPC[NPC_ZOMBIE_COW].walk_left[i].y=NPC_SPRITE_COUNTS[NPC_ZOMBIE_COW].height;
        SPRITES_NPC[NPC_ZOMBIE_COW].walk_left[i].w=NPC_SPRITE_COUNTS[NPC_ZOMBIE_COW].width;
        SPRITES_NPC[NPC_ZOMBIE_COW].walk_left[i].h=NPC_SPRITE_COUNTS[NPC_ZOMBIE_COW].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_ZOMBIE_COW].jump_sprites;i++){
        SPRITES_NPC[NPC_ZOMBIE_COW].jump_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_ZOMBIE_COW].jump_right[i].x=i*NPC_SPRITE_COUNTS[NPC_ZOMBIE_COW].width;
        SPRITES_NPC[NPC_ZOMBIE_COW].jump_right[i].y=0;
        SPRITES_NPC[NPC_ZOMBIE_COW].jump_right[i].w=NPC_SPRITE_COUNTS[NPC_ZOMBIE_COW].width;
        SPRITES_NPC[NPC_ZOMBIE_COW].jump_right[i].h=NPC_SPRITE_COUNTS[NPC_ZOMBIE_COW].height;

        SPRITES_NPC[NPC_ZOMBIE_COW].jump_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_ZOMBIE_COW].jump_left[i].x=i*NPC_SPRITE_COUNTS[NPC_ZOMBIE_COW].width;
        SPRITES_NPC[NPC_ZOMBIE_COW].jump_left[i].y=NPC_SPRITE_COUNTS[NPC_ZOMBIE_COW].height;
        SPRITES_NPC[NPC_ZOMBIE_COW].jump_left[i].w=NPC_SPRITE_COUNTS[NPC_ZOMBIE_COW].width;
        SPRITES_NPC[NPC_ZOMBIE_COW].jump_left[i].h=NPC_SPRITE_COUNTS[NPC_ZOMBIE_COW].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_ZOMBIE_COW].swim_sprites;i++){
        SPRITES_NPC[NPC_ZOMBIE_COW].swim_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_ZOMBIE_COW].swim_right[i].x=i*NPC_SPRITE_COUNTS[NPC_ZOMBIE_COW].width;
        SPRITES_NPC[NPC_ZOMBIE_COW].swim_right[i].y=0;
        SPRITES_NPC[NPC_ZOMBIE_COW].swim_right[i].w=NPC_SPRITE_COUNTS[NPC_ZOMBIE_COW].width;
        SPRITES_NPC[NPC_ZOMBIE_COW].swim_right[i].h=NPC_SPRITE_COUNTS[NPC_ZOMBIE_COW].height;

        SPRITES_NPC[NPC_ZOMBIE_COW].swim_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_ZOMBIE_COW].swim_left[i].x=i*NPC_SPRITE_COUNTS[NPC_ZOMBIE_COW].width;
        SPRITES_NPC[NPC_ZOMBIE_COW].swim_left[i].y=NPC_SPRITE_COUNTS[NPC_ZOMBIE_COW].height;
        SPRITES_NPC[NPC_ZOMBIE_COW].swim_left[i].w=NPC_SPRITE_COUNTS[NPC_ZOMBIE_COW].width;
        SPRITES_NPC[NPC_ZOMBIE_COW].swim_left[i].h=NPC_SPRITE_COUNTS[NPC_ZOMBIE_COW].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_ZOMBIE_COW].dead_sprites;i++){
        SPRITES_NPC[NPC_ZOMBIE_COW].dead.push_back(SDL_Rect());
        SPRITES_NPC[NPC_ZOMBIE_COW].dead[i].x=i*NPC_SPRITE_COUNTS[NPC_ZOMBIE_COW].dead_width;
        SPRITES_NPC[NPC_ZOMBIE_COW].dead[i].y=NPC_SPRITE_COUNTS[NPC_ZOMBIE_COW].height*2;
        SPRITES_NPC[NPC_ZOMBIE_COW].dead[i].w=NPC_SPRITE_COUNTS[NPC_ZOMBIE_COW].dead_width;
        SPRITES_NPC[NPC_ZOMBIE_COW].dead[i].h=NPC_SPRITE_COUNTS[NPC_ZOMBIE_COW].dead_height;
    }

    //Mummy cow
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_MUMMY_COW].walk_sprites;i++){
        SPRITES_NPC[NPC_MUMMY_COW].walk_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_MUMMY_COW].walk_right[i].x=i*NPC_SPRITE_COUNTS[NPC_MUMMY_COW].width;
        SPRITES_NPC[NPC_MUMMY_COW].walk_right[i].y=0;
        SPRITES_NPC[NPC_MUMMY_COW].walk_right[i].w=NPC_SPRITE_COUNTS[NPC_MUMMY_COW].width;
        SPRITES_NPC[NPC_MUMMY_COW].walk_right[i].h=NPC_SPRITE_COUNTS[NPC_MUMMY_COW].height;

        SPRITES_NPC[NPC_MUMMY_COW].walk_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_MUMMY_COW].walk_left[i].x=i*NPC_SPRITE_COUNTS[NPC_MUMMY_COW].width;
        SPRITES_NPC[NPC_MUMMY_COW].walk_left[i].y=NPC_SPRITE_COUNTS[NPC_MUMMY_COW].height;
        SPRITES_NPC[NPC_MUMMY_COW].walk_left[i].w=NPC_SPRITE_COUNTS[NPC_MUMMY_COW].width;
        SPRITES_NPC[NPC_MUMMY_COW].walk_left[i].h=NPC_SPRITE_COUNTS[NPC_MUMMY_COW].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_MUMMY_COW].jump_sprites;i++){
        SPRITES_NPC[NPC_MUMMY_COW].jump_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_MUMMY_COW].jump_right[i].x=i*NPC_SPRITE_COUNTS[NPC_MUMMY_COW].width;
        SPRITES_NPC[NPC_MUMMY_COW].jump_right[i].y=0;
        SPRITES_NPC[NPC_MUMMY_COW].jump_right[i].w=NPC_SPRITE_COUNTS[NPC_MUMMY_COW].width;
        SPRITES_NPC[NPC_MUMMY_COW].jump_right[i].h=NPC_SPRITE_COUNTS[NPC_MUMMY_COW].height;

        SPRITES_NPC[NPC_MUMMY_COW].jump_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_MUMMY_COW].jump_left[i].x=i*NPC_SPRITE_COUNTS[NPC_MUMMY_COW].width;
        SPRITES_NPC[NPC_MUMMY_COW].jump_left[i].y=NPC_SPRITE_COUNTS[NPC_MUMMY_COW].height;
        SPRITES_NPC[NPC_MUMMY_COW].jump_left[i].w=NPC_SPRITE_COUNTS[NPC_MUMMY_COW].width;
        SPRITES_NPC[NPC_MUMMY_COW].jump_left[i].h=NPC_SPRITE_COUNTS[NPC_MUMMY_COW].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_MUMMY_COW].swim_sprites;i++){
        SPRITES_NPC[NPC_MUMMY_COW].swim_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_MUMMY_COW].swim_right[i].x=i*NPC_SPRITE_COUNTS[NPC_MUMMY_COW].width;
        SPRITES_NPC[NPC_MUMMY_COW].swim_right[i].y=0;
        SPRITES_NPC[NPC_MUMMY_COW].swim_right[i].w=NPC_SPRITE_COUNTS[NPC_MUMMY_COW].width;
        SPRITES_NPC[NPC_MUMMY_COW].swim_right[i].h=NPC_SPRITE_COUNTS[NPC_MUMMY_COW].height;

        SPRITES_NPC[NPC_MUMMY_COW].swim_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_MUMMY_COW].swim_left[i].x=i*NPC_SPRITE_COUNTS[NPC_MUMMY_COW].width;
        SPRITES_NPC[NPC_MUMMY_COW].swim_left[i].y=NPC_SPRITE_COUNTS[NPC_MUMMY_COW].height;
        SPRITES_NPC[NPC_MUMMY_COW].swim_left[i].w=NPC_SPRITE_COUNTS[NPC_MUMMY_COW].width;
        SPRITES_NPC[NPC_MUMMY_COW].swim_left[i].h=NPC_SPRITE_COUNTS[NPC_MUMMY_COW].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_MUMMY_COW].dead_sprites;i++){
        SPRITES_NPC[NPC_MUMMY_COW].dead.push_back(SDL_Rect());
        SPRITES_NPC[NPC_MUMMY_COW].dead[i].x=i*NPC_SPRITE_COUNTS[NPC_MUMMY_COW].dead_width;
        SPRITES_NPC[NPC_MUMMY_COW].dead[i].y=NPC_SPRITE_COUNTS[NPC_MUMMY_COW].height*2;
        SPRITES_NPC[NPC_MUMMY_COW].dead[i].w=NPC_SPRITE_COUNTS[NPC_MUMMY_COW].dead_width;
        SPRITES_NPC[NPC_MUMMY_COW].dead[i].h=NPC_SPRITE_COUNTS[NPC_MUMMY_COW].dead_height;
    }

    //Ghost
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_GHOST].fly_sprites;i++){
        SPRITES_NPC[NPC_GHOST].fly_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_GHOST].fly_right[i].x=i*NPC_SPRITE_COUNTS[NPC_GHOST].width;
        SPRITES_NPC[NPC_GHOST].fly_right[i].y=NPC_SPRITE_COUNTS[NPC_GHOST].height*2;
        SPRITES_NPC[NPC_GHOST].fly_right[i].w=NPC_SPRITE_COUNTS[NPC_GHOST].width;
        SPRITES_NPC[NPC_GHOST].fly_right[i].h=NPC_SPRITE_COUNTS[NPC_GHOST].height;

        SPRITES_NPC[NPC_GHOST].fly_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_GHOST].fly_left[i].x=i*NPC_SPRITE_COUNTS[NPC_GHOST].width;
        SPRITES_NPC[NPC_GHOST].fly_left[i].y=NPC_SPRITE_COUNTS[NPC_GHOST].height*3;
        SPRITES_NPC[NPC_GHOST].fly_left[i].w=NPC_SPRITE_COUNTS[NPC_GHOST].width;
        SPRITES_NPC[NPC_GHOST].fly_left[i].h=NPC_SPRITE_COUNTS[NPC_GHOST].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_GHOST].special_attack_sprites;i++){
        SPRITES_NPC[NPC_GHOST].special_attack_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_GHOST].special_attack_right[i].x=i*NPC_SPRITE_COUNTS[NPC_GHOST].special_width;
        SPRITES_NPC[NPC_GHOST].special_attack_right[i].y=0;
        SPRITES_NPC[NPC_GHOST].special_attack_right[i].w=NPC_SPRITE_COUNTS[NPC_GHOST].special_width;
        SPRITES_NPC[NPC_GHOST].special_attack_right[i].h=NPC_SPRITE_COUNTS[NPC_GHOST].special_height;

        SPRITES_NPC[NPC_GHOST].special_attack_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_GHOST].special_attack_left[i].x=i*NPC_SPRITE_COUNTS[NPC_GHOST].special_width;
        SPRITES_NPC[NPC_GHOST].special_attack_left[i].y=NPC_SPRITE_COUNTS[NPC_GHOST].height;
        SPRITES_NPC[NPC_GHOST].special_attack_left[i].w=NPC_SPRITE_COUNTS[NPC_GHOST].special_width;
        SPRITES_NPC[NPC_GHOST].special_attack_left[i].h=NPC_SPRITE_COUNTS[NPC_GHOST].special_height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_GHOST].dead_sprites;i++){
        SPRITES_NPC[NPC_GHOST].dead.push_back(SDL_Rect());
        SPRITES_NPC[NPC_GHOST].dead[i].x=i*NPC_SPRITE_COUNTS[NPC_GHOST].dead_width;
        SPRITES_NPC[NPC_GHOST].dead[i].y=NPC_SPRITE_COUNTS[NPC_GHOST].height*4;
        SPRITES_NPC[NPC_GHOST].dead[i].w=NPC_SPRITE_COUNTS[NPC_GHOST].dead_width;
        SPRITES_NPC[NPC_GHOST].dead[i].h=NPC_SPRITE_COUNTS[NPC_GHOST].dead_height;
    }

    //Spider
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_SPIDER].fly_sprites;i++){
        SPRITES_NPC[NPC_SPIDER].fly_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SPIDER].fly_right[i].x=i*NPC_SPRITE_COUNTS[NPC_SPIDER].width;
        SPRITES_NPC[NPC_SPIDER].fly_right[i].y=0;
        SPRITES_NPC[NPC_SPIDER].fly_right[i].w=NPC_SPRITE_COUNTS[NPC_SPIDER].width;
        SPRITES_NPC[NPC_SPIDER].fly_right[i].h=NPC_SPRITE_COUNTS[NPC_SPIDER].height;

        SPRITES_NPC[NPC_SPIDER].fly_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SPIDER].fly_left[i].x=i*NPC_SPRITE_COUNTS[NPC_SPIDER].width;
        SPRITES_NPC[NPC_SPIDER].fly_left[i].y=NPC_SPRITE_COUNTS[NPC_SPIDER].height;
        SPRITES_NPC[NPC_SPIDER].fly_left[i].w=NPC_SPRITE_COUNTS[NPC_SPIDER].width;
        SPRITES_NPC[NPC_SPIDER].fly_left[i].h=NPC_SPRITE_COUNTS[NPC_SPIDER].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_SPIDER].dead_sprites;i++){
        SPRITES_NPC[NPC_SPIDER].dead.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SPIDER].dead[i].x=i*NPC_SPRITE_COUNTS[NPC_SPIDER].dead_width;
        SPRITES_NPC[NPC_SPIDER].dead[i].y=NPC_SPRITE_COUNTS[NPC_SPIDER].height*2;
        SPRITES_NPC[NPC_SPIDER].dead[i].w=NPC_SPRITE_COUNTS[NPC_SPIDER].dead_width;
        SPRITES_NPC[NPC_SPIDER].dead[i].h=NPC_SPRITE_COUNTS[NPC_SPIDER].dead_height;
    }

    //Cat
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_CAT].walk_sprites;i++){
        SPRITES_NPC[NPC_CAT].walk_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_CAT].walk_right[i].x=i*NPC_SPRITE_COUNTS[NPC_CAT].width;
        SPRITES_NPC[NPC_CAT].walk_right[i].y=0;
        SPRITES_NPC[NPC_CAT].walk_right[i].w=NPC_SPRITE_COUNTS[NPC_CAT].width;
        SPRITES_NPC[NPC_CAT].walk_right[i].h=NPC_SPRITE_COUNTS[NPC_CAT].height;

        SPRITES_NPC[NPC_CAT].walk_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_CAT].walk_left[i].x=i*NPC_SPRITE_COUNTS[NPC_CAT].width;
        SPRITES_NPC[NPC_CAT].walk_left[i].y=NPC_SPRITE_COUNTS[NPC_CAT].height;
        SPRITES_NPC[NPC_CAT].walk_left[i].w=NPC_SPRITE_COUNTS[NPC_CAT].width;
        SPRITES_NPC[NPC_CAT].walk_left[i].h=NPC_SPRITE_COUNTS[NPC_CAT].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_CAT].jump_sprites;i++){
        SPRITES_NPC[NPC_CAT].jump_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_CAT].jump_right[i].x=i*NPC_SPRITE_COUNTS[NPC_CAT].width;
        SPRITES_NPC[NPC_CAT].jump_right[i].y=NPC_SPRITE_COUNTS[NPC_CAT].height*2;
        SPRITES_NPC[NPC_CAT].jump_right[i].w=NPC_SPRITE_COUNTS[NPC_CAT].width;
        SPRITES_NPC[NPC_CAT].jump_right[i].h=NPC_SPRITE_COUNTS[NPC_CAT].height;

        SPRITES_NPC[NPC_CAT].jump_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_CAT].jump_left[i].x=i*NPC_SPRITE_COUNTS[NPC_CAT].width;
        SPRITES_NPC[NPC_CAT].jump_left[i].y=NPC_SPRITE_COUNTS[NPC_CAT].height*3;
        SPRITES_NPC[NPC_CAT].jump_left[i].w=NPC_SPRITE_COUNTS[NPC_CAT].width;
        SPRITES_NPC[NPC_CAT].jump_left[i].h=NPC_SPRITE_COUNTS[NPC_CAT].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_CAT].swim_sprites;i++){
        SPRITES_NPC[NPC_CAT].swim_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_CAT].swim_right[i].x=i*NPC_SPRITE_COUNTS[NPC_CAT].width;
        SPRITES_NPC[NPC_CAT].swim_right[i].y=0;
        SPRITES_NPC[NPC_CAT].swim_right[i].w=NPC_SPRITE_COUNTS[NPC_CAT].width;
        SPRITES_NPC[NPC_CAT].swim_right[i].h=NPC_SPRITE_COUNTS[NPC_CAT].height;

        SPRITES_NPC[NPC_CAT].swim_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_CAT].swim_left[i].x=i*NPC_SPRITE_COUNTS[NPC_CAT].width;
        SPRITES_NPC[NPC_CAT].swim_left[i].y=NPC_SPRITE_COUNTS[NPC_CAT].height;
        SPRITES_NPC[NPC_CAT].swim_left[i].w=NPC_SPRITE_COUNTS[NPC_CAT].width;
        SPRITES_NPC[NPC_CAT].swim_left[i].h=NPC_SPRITE_COUNTS[NPC_CAT].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_CAT].special_attack_sprites;i++){
        SPRITES_NPC[NPC_CAT].special_attack_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_CAT].special_attack_right[i].x=i*NPC_SPRITE_COUNTS[NPC_CAT].special_width;
        SPRITES_NPC[NPC_CAT].special_attack_right[i].y=NPC_SPRITE_COUNTS[NPC_CAT].height*4;
        SPRITES_NPC[NPC_CAT].special_attack_right[i].w=NPC_SPRITE_COUNTS[NPC_CAT].special_width;
        SPRITES_NPC[NPC_CAT].special_attack_right[i].h=NPC_SPRITE_COUNTS[NPC_CAT].special_height;

        SPRITES_NPC[NPC_CAT].special_attack_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_CAT].special_attack_left[i].x=i*NPC_SPRITE_COUNTS[NPC_CAT].special_width;
        SPRITES_NPC[NPC_CAT].special_attack_left[i].y=NPC_SPRITE_COUNTS[NPC_CAT].height*4+NPC_SPRITE_COUNTS[NPC_CAT].special_height;
        SPRITES_NPC[NPC_CAT].special_attack_left[i].w=NPC_SPRITE_COUNTS[NPC_CAT].special_width;
        SPRITES_NPC[NPC_CAT].special_attack_left[i].h=NPC_SPRITE_COUNTS[NPC_CAT].special_height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_CAT].dead_sprites;i++){
        SPRITES_NPC[NPC_CAT].dead.push_back(SDL_Rect());
        SPRITES_NPC[NPC_CAT].dead[i].x=i*NPC_SPRITE_COUNTS[NPC_CAT].dead_width;
        SPRITES_NPC[NPC_CAT].dead[i].y=NPC_SPRITE_COUNTS[NPC_CAT].height*4+NPC_SPRITE_COUNTS[NPC_CAT].special_height*2;
        SPRITES_NPC[NPC_CAT].dead[i].w=NPC_SPRITE_COUNTS[NPC_CAT].dead_width;
        SPRITES_NPC[NPC_CAT].dead[i].h=NPC_SPRITE_COUNTS[NPC_CAT].dead_height;
    }

    //Spider walking
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_SPIDER_WALKING].walk_sprites;i++){
        SPRITES_NPC[NPC_SPIDER_WALKING].walk_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SPIDER_WALKING].walk_right[i].x=i*NPC_SPRITE_COUNTS[NPC_SPIDER_WALKING].width;
        SPRITES_NPC[NPC_SPIDER_WALKING].walk_right[i].y=0;
        SPRITES_NPC[NPC_SPIDER_WALKING].walk_right[i].w=NPC_SPRITE_COUNTS[NPC_SPIDER_WALKING].width;
        SPRITES_NPC[NPC_SPIDER_WALKING].walk_right[i].h=NPC_SPRITE_COUNTS[NPC_SPIDER_WALKING].height;

        SPRITES_NPC[NPC_SPIDER_WALKING].walk_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SPIDER_WALKING].walk_left[i].x=i*NPC_SPRITE_COUNTS[NPC_SPIDER_WALKING].width;
        SPRITES_NPC[NPC_SPIDER_WALKING].walk_left[i].y=NPC_SPRITE_COUNTS[NPC_SPIDER_WALKING].height;
        SPRITES_NPC[NPC_SPIDER_WALKING].walk_left[i].w=NPC_SPRITE_COUNTS[NPC_SPIDER_WALKING].width;
        SPRITES_NPC[NPC_SPIDER_WALKING].walk_left[i].h=NPC_SPRITE_COUNTS[NPC_SPIDER_WALKING].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_SPIDER_WALKING].jump_sprites;i++){
        SPRITES_NPC[NPC_SPIDER_WALKING].jump_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SPIDER_WALKING].jump_right[i].x=i*NPC_SPRITE_COUNTS[NPC_SPIDER_WALKING].width;
        SPRITES_NPC[NPC_SPIDER_WALKING].jump_right[i].y=0;
        SPRITES_NPC[NPC_SPIDER_WALKING].jump_right[i].w=NPC_SPRITE_COUNTS[NPC_SPIDER_WALKING].width;
        SPRITES_NPC[NPC_SPIDER_WALKING].jump_right[i].h=NPC_SPRITE_COUNTS[NPC_SPIDER_WALKING].height;

        SPRITES_NPC[NPC_SPIDER_WALKING].jump_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SPIDER_WALKING].jump_left[i].x=i*NPC_SPRITE_COUNTS[NPC_SPIDER_WALKING].width;
        SPRITES_NPC[NPC_SPIDER_WALKING].jump_left[i].y=NPC_SPRITE_COUNTS[NPC_SPIDER_WALKING].height;
        SPRITES_NPC[NPC_SPIDER_WALKING].jump_left[i].w=NPC_SPRITE_COUNTS[NPC_SPIDER_WALKING].width;
        SPRITES_NPC[NPC_SPIDER_WALKING].jump_left[i].h=NPC_SPRITE_COUNTS[NPC_SPIDER_WALKING].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_SPIDER_WALKING].swim_sprites;i++){
        SPRITES_NPC[NPC_SPIDER_WALKING].swim_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SPIDER_WALKING].swim_right[i].x=i*NPC_SPRITE_COUNTS[NPC_SPIDER_WALKING].width;
        SPRITES_NPC[NPC_SPIDER_WALKING].swim_right[i].y=0;
        SPRITES_NPC[NPC_SPIDER_WALKING].swim_right[i].w=NPC_SPRITE_COUNTS[NPC_SPIDER_WALKING].width;
        SPRITES_NPC[NPC_SPIDER_WALKING].swim_right[i].h=NPC_SPRITE_COUNTS[NPC_SPIDER_WALKING].height;

        SPRITES_NPC[NPC_SPIDER_WALKING].swim_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SPIDER_WALKING].swim_left[i].x=i*NPC_SPRITE_COUNTS[NPC_SPIDER_WALKING].width;
        SPRITES_NPC[NPC_SPIDER_WALKING].swim_left[i].y=NPC_SPRITE_COUNTS[NPC_SPIDER_WALKING].height;
        SPRITES_NPC[NPC_SPIDER_WALKING].swim_left[i].w=NPC_SPRITE_COUNTS[NPC_SPIDER_WALKING].width;
        SPRITES_NPC[NPC_SPIDER_WALKING].swim_left[i].h=NPC_SPRITE_COUNTS[NPC_SPIDER_WALKING].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_SPIDER_WALKING].dead_sprites;i++){
        SPRITES_NPC[NPC_SPIDER_WALKING].dead.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SPIDER_WALKING].dead[i].x=i*NPC_SPRITE_COUNTS[NPC_SPIDER_WALKING].dead_width;
        SPRITES_NPC[NPC_SPIDER_WALKING].dead[i].y=NPC_SPRITE_COUNTS[NPC_SPIDER_WALKING].height*2;
        SPRITES_NPC[NPC_SPIDER_WALKING].dead[i].w=NPC_SPRITE_COUNTS[NPC_SPIDER_WALKING].dead_width;
        SPRITES_NPC[NPC_SPIDER_WALKING].dead[i].h=NPC_SPRITE_COUNTS[NPC_SPIDER_WALKING].dead_height;
    }

    //Ram
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_RAM].walk_sprites;i++){
        SPRITES_NPC[NPC_RAM].walk_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_RAM].walk_right[i].x=i*NPC_SPRITE_COUNTS[NPC_RAM].width;
        SPRITES_NPC[NPC_RAM].walk_right[i].y=0;
        SPRITES_NPC[NPC_RAM].walk_right[i].w=NPC_SPRITE_COUNTS[NPC_RAM].width;
        SPRITES_NPC[NPC_RAM].walk_right[i].h=NPC_SPRITE_COUNTS[NPC_RAM].height;

        SPRITES_NPC[NPC_RAM].walk_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_RAM].walk_left[i].x=i*NPC_SPRITE_COUNTS[NPC_RAM].width;
        SPRITES_NPC[NPC_RAM].walk_left[i].y=NPC_SPRITE_COUNTS[NPC_RAM].height;
        SPRITES_NPC[NPC_RAM].walk_left[i].w=NPC_SPRITE_COUNTS[NPC_RAM].width;
        SPRITES_NPC[NPC_RAM].walk_left[i].h=NPC_SPRITE_COUNTS[NPC_RAM].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_RAM].jump_sprites;i++){
        SPRITES_NPC[NPC_RAM].jump_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_RAM].jump_right[i].x=i*NPC_SPRITE_COUNTS[NPC_RAM].width;
        SPRITES_NPC[NPC_RAM].jump_right[i].y=0;
        SPRITES_NPC[NPC_RAM].jump_right[i].w=NPC_SPRITE_COUNTS[NPC_RAM].width;
        SPRITES_NPC[NPC_RAM].jump_right[i].h=NPC_SPRITE_COUNTS[NPC_RAM].height;

        SPRITES_NPC[NPC_RAM].jump_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_RAM].jump_left[i].x=i*NPC_SPRITE_COUNTS[NPC_RAM].width;
        SPRITES_NPC[NPC_RAM].jump_left[i].y=NPC_SPRITE_COUNTS[NPC_RAM].height;
        SPRITES_NPC[NPC_RAM].jump_left[i].w=NPC_SPRITE_COUNTS[NPC_RAM].width;
        SPRITES_NPC[NPC_RAM].jump_left[i].h=NPC_SPRITE_COUNTS[NPC_RAM].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_RAM].swim_sprites;i++){
        SPRITES_NPC[NPC_RAM].swim_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_RAM].swim_right[i].x=i*NPC_SPRITE_COUNTS[NPC_RAM].width;
        SPRITES_NPC[NPC_RAM].swim_right[i].y=0;
        SPRITES_NPC[NPC_RAM].swim_right[i].w=NPC_SPRITE_COUNTS[NPC_RAM].width;
        SPRITES_NPC[NPC_RAM].swim_right[i].h=NPC_SPRITE_COUNTS[NPC_RAM].height;

        SPRITES_NPC[NPC_RAM].swim_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_RAM].swim_left[i].x=i*NPC_SPRITE_COUNTS[NPC_RAM].width;
        SPRITES_NPC[NPC_RAM].swim_left[i].y=NPC_SPRITE_COUNTS[NPC_RAM].height;
        SPRITES_NPC[NPC_RAM].swim_left[i].w=NPC_SPRITE_COUNTS[NPC_RAM].width;
        SPRITES_NPC[NPC_RAM].swim_left[i].h=NPC_SPRITE_COUNTS[NPC_RAM].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_RAM].special_attack_sprites;i++){
        SPRITES_NPC[NPC_RAM].special_attack_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_RAM].special_attack_right[i].x=i*NPC_SPRITE_COUNTS[NPC_RAM].special_width;
        SPRITES_NPC[NPC_RAM].special_attack_right[i].y=0;
        SPRITES_NPC[NPC_RAM].special_attack_right[i].w=NPC_SPRITE_COUNTS[NPC_RAM].special_width;
        SPRITES_NPC[NPC_RAM].special_attack_right[i].h=NPC_SPRITE_COUNTS[NPC_RAM].special_height;

        SPRITES_NPC[NPC_RAM].special_attack_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_RAM].special_attack_left[i].x=i*NPC_SPRITE_COUNTS[NPC_RAM].special_width;
        SPRITES_NPC[NPC_RAM].special_attack_left[i].y=NPC_SPRITE_COUNTS[NPC_RAM].height;
        SPRITES_NPC[NPC_RAM].special_attack_left[i].w=NPC_SPRITE_COUNTS[NPC_RAM].special_width;
        SPRITES_NPC[NPC_RAM].special_attack_left[i].h=NPC_SPRITE_COUNTS[NPC_RAM].special_height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_RAM].dead_sprites;i++){
        SPRITES_NPC[NPC_RAM].dead.push_back(SDL_Rect());
        SPRITES_NPC[NPC_RAM].dead[i].x=i*NPC_SPRITE_COUNTS[NPC_RAM].dead_width;
        SPRITES_NPC[NPC_RAM].dead[i].y=NPC_SPRITE_COUNTS[NPC_RAM].height*2;
        SPRITES_NPC[NPC_RAM].dead[i].w=NPC_SPRITE_COUNTS[NPC_RAM].dead_width;
        SPRITES_NPC[NPC_RAM].dead[i].h=NPC_SPRITE_COUNTS[NPC_RAM].dead_height;
    }

    //Eagle
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_EAGLE].fly_sprites;i++){
        SPRITES_NPC[NPC_EAGLE].fly_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_EAGLE].fly_right[i].x=i*NPC_SPRITE_COUNTS[NPC_EAGLE].width;
        SPRITES_NPC[NPC_EAGLE].fly_right[i].y=0;
        SPRITES_NPC[NPC_EAGLE].fly_right[i].w=NPC_SPRITE_COUNTS[NPC_EAGLE].width;
        SPRITES_NPC[NPC_EAGLE].fly_right[i].h=NPC_SPRITE_COUNTS[NPC_EAGLE].height;

        SPRITES_NPC[NPC_EAGLE].fly_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_EAGLE].fly_left[i].x=i*NPC_SPRITE_COUNTS[NPC_EAGLE].width;
        SPRITES_NPC[NPC_EAGLE].fly_left[i].y=NPC_SPRITE_COUNTS[NPC_EAGLE].height;
        SPRITES_NPC[NPC_EAGLE].fly_left[i].w=NPC_SPRITE_COUNTS[NPC_EAGLE].width;
        SPRITES_NPC[NPC_EAGLE].fly_left[i].h=NPC_SPRITE_COUNTS[NPC_EAGLE].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_EAGLE].special_attack_sprites;i++){
        SPRITES_NPC[NPC_EAGLE].special_attack_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_EAGLE].special_attack_right[i].x=i*NPC_SPRITE_COUNTS[NPC_EAGLE].special_width;
        SPRITES_NPC[NPC_EAGLE].special_attack_right[i].y=NPC_SPRITE_COUNTS[NPC_EAGLE].height*2;
        SPRITES_NPC[NPC_EAGLE].special_attack_right[i].w=NPC_SPRITE_COUNTS[NPC_EAGLE].special_width;
        SPRITES_NPC[NPC_EAGLE].special_attack_right[i].h=NPC_SPRITE_COUNTS[NPC_EAGLE].special_height;

        SPRITES_NPC[NPC_EAGLE].special_attack_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_EAGLE].special_attack_left[i].x=i*NPC_SPRITE_COUNTS[NPC_EAGLE].special_width;
        SPRITES_NPC[NPC_EAGLE].special_attack_left[i].y=NPC_SPRITE_COUNTS[NPC_EAGLE].height*2+NPC_SPRITE_COUNTS[NPC_EAGLE].special_height;
        SPRITES_NPC[NPC_EAGLE].special_attack_left[i].w=NPC_SPRITE_COUNTS[NPC_EAGLE].special_width;
        SPRITES_NPC[NPC_EAGLE].special_attack_left[i].h=NPC_SPRITE_COUNTS[NPC_EAGLE].special_height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_EAGLE].dead_sprites;i++){
        SPRITES_NPC[NPC_EAGLE].dead.push_back(SDL_Rect());
        SPRITES_NPC[NPC_EAGLE].dead[i].x=i*NPC_SPRITE_COUNTS[NPC_EAGLE].dead_width;
        SPRITES_NPC[NPC_EAGLE].dead[i].y=NPC_SPRITE_COUNTS[NPC_EAGLE].height*2+NPC_SPRITE_COUNTS[NPC_EAGLE].special_height*2;
        SPRITES_NPC[NPC_EAGLE].dead[i].w=NPC_SPRITE_COUNTS[NPC_EAGLE].dead_width;
        SPRITES_NPC[NPC_EAGLE].dead[i].h=NPC_SPRITE_COUNTS[NPC_EAGLE].dead_height;
    }

    //Snowman
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_SNOWMAN].walk_sprites;i++){
        SPRITES_NPC[NPC_SNOWMAN].walk_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SNOWMAN].walk_right[i].x=i*NPC_SPRITE_COUNTS[NPC_SNOWMAN].width;
        SPRITES_NPC[NPC_SNOWMAN].walk_right[i].y=0;
        SPRITES_NPC[NPC_SNOWMAN].walk_right[i].w=NPC_SPRITE_COUNTS[NPC_SNOWMAN].width;
        SPRITES_NPC[NPC_SNOWMAN].walk_right[i].h=NPC_SPRITE_COUNTS[NPC_SNOWMAN].height;

        SPRITES_NPC[NPC_SNOWMAN].walk_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SNOWMAN].walk_left[i].x=i*NPC_SPRITE_COUNTS[NPC_SNOWMAN].width;
        SPRITES_NPC[NPC_SNOWMAN].walk_left[i].y=NPC_SPRITE_COUNTS[NPC_SNOWMAN].height;
        SPRITES_NPC[NPC_SNOWMAN].walk_left[i].w=NPC_SPRITE_COUNTS[NPC_SNOWMAN].width;
        SPRITES_NPC[NPC_SNOWMAN].walk_left[i].h=NPC_SPRITE_COUNTS[NPC_SNOWMAN].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_SNOWMAN].jump_sprites;i++){
        SPRITES_NPC[NPC_SNOWMAN].jump_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SNOWMAN].jump_right[i].x=i*NPC_SPRITE_COUNTS[NPC_SNOWMAN].width;
        SPRITES_NPC[NPC_SNOWMAN].jump_right[i].y=0;
        SPRITES_NPC[NPC_SNOWMAN].jump_right[i].w=NPC_SPRITE_COUNTS[NPC_SNOWMAN].width;
        SPRITES_NPC[NPC_SNOWMAN].jump_right[i].h=NPC_SPRITE_COUNTS[NPC_SNOWMAN].height;

        SPRITES_NPC[NPC_SNOWMAN].jump_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SNOWMAN].jump_left[i].x=i*NPC_SPRITE_COUNTS[NPC_SNOWMAN].width;
        SPRITES_NPC[NPC_SNOWMAN].jump_left[i].y=NPC_SPRITE_COUNTS[NPC_SNOWMAN].height;
        SPRITES_NPC[NPC_SNOWMAN].jump_left[i].w=NPC_SPRITE_COUNTS[NPC_SNOWMAN].width;
        SPRITES_NPC[NPC_SNOWMAN].jump_left[i].h=NPC_SPRITE_COUNTS[NPC_SNOWMAN].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_SNOWMAN].swim_sprites;i++){
        SPRITES_NPC[NPC_SNOWMAN].swim_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SNOWMAN].swim_right[i].x=i*NPC_SPRITE_COUNTS[NPC_SNOWMAN].width;
        SPRITES_NPC[NPC_SNOWMAN].swim_right[i].y=0;
        SPRITES_NPC[NPC_SNOWMAN].swim_right[i].w=NPC_SPRITE_COUNTS[NPC_SNOWMAN].width;
        SPRITES_NPC[NPC_SNOWMAN].swim_right[i].h=NPC_SPRITE_COUNTS[NPC_SNOWMAN].height;

        SPRITES_NPC[NPC_SNOWMAN].swim_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SNOWMAN].swim_left[i].x=i*NPC_SPRITE_COUNTS[NPC_SNOWMAN].width;
        SPRITES_NPC[NPC_SNOWMAN].swim_left[i].y=NPC_SPRITE_COUNTS[NPC_SNOWMAN].height;
        SPRITES_NPC[NPC_SNOWMAN].swim_left[i].w=NPC_SPRITE_COUNTS[NPC_SNOWMAN].width;
        SPRITES_NPC[NPC_SNOWMAN].swim_left[i].h=NPC_SPRITE_COUNTS[NPC_SNOWMAN].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_SNOWMAN].special_attack_sprites;i++){
        SPRITES_NPC[NPC_SNOWMAN].special_attack_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SNOWMAN].special_attack_right[i].x=i*NPC_SPRITE_COUNTS[NPC_SNOWMAN].special_width;
        SPRITES_NPC[NPC_SNOWMAN].special_attack_right[i].y=NPC_SPRITE_COUNTS[NPC_SNOWMAN].height*2;
        SPRITES_NPC[NPC_SNOWMAN].special_attack_right[i].w=NPC_SPRITE_COUNTS[NPC_SNOWMAN].special_width;
        SPRITES_NPC[NPC_SNOWMAN].special_attack_right[i].h=NPC_SPRITE_COUNTS[NPC_SNOWMAN].special_height;

        SPRITES_NPC[NPC_SNOWMAN].special_attack_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SNOWMAN].special_attack_left[i].x=i*NPC_SPRITE_COUNTS[NPC_SNOWMAN].special_width;
        SPRITES_NPC[NPC_SNOWMAN].special_attack_left[i].y=NPC_SPRITE_COUNTS[NPC_SNOWMAN].height*3;
        SPRITES_NPC[NPC_SNOWMAN].special_attack_left[i].w=NPC_SPRITE_COUNTS[NPC_SNOWMAN].special_width;
        SPRITES_NPC[NPC_SNOWMAN].special_attack_left[i].h=NPC_SPRITE_COUNTS[NPC_SNOWMAN].special_height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_SNOWMAN].dead_sprites;i++){
        SPRITES_NPC[NPC_SNOWMAN].dead.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SNOWMAN].dead[i].x=i*NPC_SPRITE_COUNTS[NPC_SNOWMAN].dead_width;
        SPRITES_NPC[NPC_SNOWMAN].dead[i].y=NPC_SPRITE_COUNTS[NPC_SNOWMAN].height*4;
        SPRITES_NPC[NPC_SNOWMAN].dead[i].w=NPC_SPRITE_COUNTS[NPC_SNOWMAN].dead_width;
        SPRITES_NPC[NPC_SNOWMAN].dead[i].h=NPC_SPRITE_COUNTS[NPC_SNOWMAN].dead_height;
    }

    //Fireball
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_FIREBALL].fly_sprites;i++){
        SPRITES_NPC[NPC_FIREBALL].fly_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_FIREBALL].fly_right[i].x=i*NPC_SPRITE_COUNTS[NPC_FIREBALL].width;
        SPRITES_NPC[NPC_FIREBALL].fly_right[i].y=0;
        SPRITES_NPC[NPC_FIREBALL].fly_right[i].w=NPC_SPRITE_COUNTS[NPC_FIREBALL].width;
        SPRITES_NPC[NPC_FIREBALL].fly_right[i].h=NPC_SPRITE_COUNTS[NPC_FIREBALL].height;

        SPRITES_NPC[NPC_FIREBALL].fly_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_FIREBALL].fly_left[i].x=i*NPC_SPRITE_COUNTS[NPC_FIREBALL].width;
        SPRITES_NPC[NPC_FIREBALL].fly_left[i].y=NPC_SPRITE_COUNTS[NPC_FIREBALL].height;
        SPRITES_NPC[NPC_FIREBALL].fly_left[i].w=NPC_SPRITE_COUNTS[NPC_FIREBALL].width;
        SPRITES_NPC[NPC_FIREBALL].fly_left[i].h=NPC_SPRITE_COUNTS[NPC_FIREBALL].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_FIREBALL].dead_sprites;i++){
        SPRITES_NPC[NPC_FIREBALL].dead.push_back(SDL_Rect());
        SPRITES_NPC[NPC_FIREBALL].dead[i].x=i*NPC_SPRITE_COUNTS[NPC_FIREBALL].dead_width;
        SPRITES_NPC[NPC_FIREBALL].dead[i].y=NPC_SPRITE_COUNTS[NPC_FIREBALL].height*2;
        SPRITES_NPC[NPC_FIREBALL].dead[i].w=NPC_SPRITE_COUNTS[NPC_FIREBALL].dead_width;
        SPRITES_NPC[NPC_FIREBALL].dead[i].h=NPC_SPRITE_COUNTS[NPC_FIREBALL].dead_height;
    }

    //Dragon whelp
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_DRAGON_WHELP].fly_sprites;i++){
        SPRITES_NPC[NPC_DRAGON_WHELP].fly_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_DRAGON_WHELP].fly_right[i].x=i*NPC_SPRITE_COUNTS[NPC_DRAGON_WHELP].width;
        SPRITES_NPC[NPC_DRAGON_WHELP].fly_right[i].y=0;
        SPRITES_NPC[NPC_DRAGON_WHELP].fly_right[i].w=NPC_SPRITE_COUNTS[NPC_DRAGON_WHELP].width;
        SPRITES_NPC[NPC_DRAGON_WHELP].fly_right[i].h=NPC_SPRITE_COUNTS[NPC_DRAGON_WHELP].height;

        SPRITES_NPC[NPC_DRAGON_WHELP].fly_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_DRAGON_WHELP].fly_left[i].x=i*NPC_SPRITE_COUNTS[NPC_DRAGON_WHELP].width;
        SPRITES_NPC[NPC_DRAGON_WHELP].fly_left[i].y=NPC_SPRITE_COUNTS[NPC_DRAGON_WHELP].height;
        SPRITES_NPC[NPC_DRAGON_WHELP].fly_left[i].w=NPC_SPRITE_COUNTS[NPC_DRAGON_WHELP].width;
        SPRITES_NPC[NPC_DRAGON_WHELP].fly_left[i].h=NPC_SPRITE_COUNTS[NPC_DRAGON_WHELP].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_DRAGON_WHELP].special_attack_sprites;i++){
        SPRITES_NPC[NPC_DRAGON_WHELP].special_attack_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_DRAGON_WHELP].special_attack_right[i].x=i*NPC_SPRITE_COUNTS[NPC_DRAGON_WHELP].special_width;
        SPRITES_NPC[NPC_DRAGON_WHELP].special_attack_right[i].y=NPC_SPRITE_COUNTS[NPC_DRAGON_WHELP].height*2;
        SPRITES_NPC[NPC_DRAGON_WHELP].special_attack_right[i].w=NPC_SPRITE_COUNTS[NPC_DRAGON_WHELP].special_width;
        SPRITES_NPC[NPC_DRAGON_WHELP].special_attack_right[i].h=NPC_SPRITE_COUNTS[NPC_DRAGON_WHELP].special_height;

        SPRITES_NPC[NPC_DRAGON_WHELP].special_attack_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_DRAGON_WHELP].special_attack_left[i].x=i*NPC_SPRITE_COUNTS[NPC_DRAGON_WHELP].special_width;
        SPRITES_NPC[NPC_DRAGON_WHELP].special_attack_left[i].y=NPC_SPRITE_COUNTS[NPC_DRAGON_WHELP].height*3;
        SPRITES_NPC[NPC_DRAGON_WHELP].special_attack_left[i].w=NPC_SPRITE_COUNTS[NPC_DRAGON_WHELP].special_width;
        SPRITES_NPC[NPC_DRAGON_WHELP].special_attack_left[i].h=NPC_SPRITE_COUNTS[NPC_DRAGON_WHELP].special_height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_DRAGON_WHELP].dead_sprites;i++){
        SPRITES_NPC[NPC_DRAGON_WHELP].dead.push_back(SDL_Rect());
        SPRITES_NPC[NPC_DRAGON_WHELP].dead[i].x=i*NPC_SPRITE_COUNTS[NPC_DRAGON_WHELP].dead_width;
        SPRITES_NPC[NPC_DRAGON_WHELP].dead[i].y=NPC_SPRITE_COUNTS[NPC_DRAGON_WHELP].height*4;
        SPRITES_NPC[NPC_DRAGON_WHELP].dead[i].w=NPC_SPRITE_COUNTS[NPC_DRAGON_WHELP].dead_width;
        SPRITES_NPC[NPC_DRAGON_WHELP].dead[i].h=NPC_SPRITE_COUNTS[NPC_DRAGON_WHELP].dead_height;
    }

    //Fireball shooter left
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_LEFT].walk_sprites;i++){
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_LEFT].walk_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_LEFT].walk_right[i].x=i*NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_LEFT].width;
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_LEFT].walk_right[i].y=0;
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_LEFT].walk_right[i].w=NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_LEFT].width;
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_LEFT].walk_right[i].h=NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_LEFT].height;

        SPRITES_NPC[NPC_FIREBALL_SHOOTER_LEFT].walk_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_LEFT].walk_left[i].x=i*NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_LEFT].width;
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_LEFT].walk_left[i].y=0;
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_LEFT].walk_left[i].w=NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_LEFT].width;
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_LEFT].walk_left[i].h=NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_LEFT].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_LEFT].special_attack_sprites;i++){
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_LEFT].special_attack_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_LEFT].special_attack_right[i].x=0;
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_LEFT].special_attack_right[i].y=0;
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_LEFT].special_attack_right[i].w=NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_LEFT].special_width;
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_LEFT].special_attack_right[i].h=NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_LEFT].special_height;

        SPRITES_NPC[NPC_FIREBALL_SHOOTER_LEFT].special_attack_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_LEFT].special_attack_left[i].x=0;
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_LEFT].special_attack_left[i].y=0;
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_LEFT].special_attack_left[i].w=NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_LEFT].special_width;
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_LEFT].special_attack_left[i].h=NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_LEFT].special_height;
    }

    //Fireball shooter right
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_RIGHT].walk_sprites;i++){
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_RIGHT].walk_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_RIGHT].walk_right[i].x=i*NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_RIGHT].width;
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_RIGHT].walk_right[i].y=0;
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_RIGHT].walk_right[i].w=NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_RIGHT].width;
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_RIGHT].walk_right[i].h=NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_RIGHT].height;

        SPRITES_NPC[NPC_FIREBALL_SHOOTER_RIGHT].walk_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_RIGHT].walk_left[i].x=i*NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_RIGHT].width;
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_RIGHT].walk_left[i].y=0;
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_RIGHT].walk_left[i].w=NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_RIGHT].width;
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_RIGHT].walk_left[i].h=NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_RIGHT].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_RIGHT].special_attack_sprites;i++){
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_RIGHT].special_attack_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_RIGHT].special_attack_right[i].x=0;
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_RIGHT].special_attack_right[i].y=0;
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_RIGHT].special_attack_right[i].w=NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_RIGHT].special_width;
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_RIGHT].special_attack_right[i].h=NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_RIGHT].special_height;

        SPRITES_NPC[NPC_FIREBALL_SHOOTER_RIGHT].special_attack_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_RIGHT].special_attack_left[i].x=0;
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_RIGHT].special_attack_left[i].y=0;
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_RIGHT].special_attack_left[i].w=NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_RIGHT].special_width;
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_RIGHT].special_attack_left[i].h=NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_RIGHT].special_height;
    }

    //Fireball shooter up
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_UP].walk_sprites;i++){
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_UP].walk_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_UP].walk_right[i].x=i*NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_UP].width;
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_UP].walk_right[i].y=0;
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_UP].walk_right[i].w=NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_UP].width;
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_UP].walk_right[i].h=NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_UP].height;

        SPRITES_NPC[NPC_FIREBALL_SHOOTER_UP].walk_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_UP].walk_left[i].x=i*NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_UP].width;
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_UP].walk_left[i].y=0;
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_UP].walk_left[i].w=NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_UP].width;
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_UP].walk_left[i].h=NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_UP].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_UP].special_attack_sprites;i++){
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_UP].special_attack_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_UP].special_attack_right[i].x=0;
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_UP].special_attack_right[i].y=0;
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_UP].special_attack_right[i].w=NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_UP].special_width;
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_UP].special_attack_right[i].h=NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_UP].special_height;

        SPRITES_NPC[NPC_FIREBALL_SHOOTER_UP].special_attack_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_UP].special_attack_left[i].x=0;
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_UP].special_attack_left[i].y=0;
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_UP].special_attack_left[i].w=NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_UP].special_width;
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_UP].special_attack_left[i].h=NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_UP].special_height;
    }

    //Fireball shooter down
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_DOWN].walk_sprites;i++){
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_DOWN].walk_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_DOWN].walk_right[i].x=i*NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_DOWN].width;
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_DOWN].walk_right[i].y=0;
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_DOWN].walk_right[i].w=NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_DOWN].width;
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_DOWN].walk_right[i].h=NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_DOWN].height;

        SPRITES_NPC[NPC_FIREBALL_SHOOTER_DOWN].walk_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_DOWN].walk_left[i].x=i*NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_DOWN].width;
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_DOWN].walk_left[i].y=0;
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_DOWN].walk_left[i].w=NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_DOWN].width;
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_DOWN].walk_left[i].h=NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_DOWN].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_DOWN].special_attack_sprites;i++){
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_DOWN].special_attack_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_DOWN].special_attack_right[i].x=0;
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_DOWN].special_attack_right[i].y=0;
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_DOWN].special_attack_right[i].w=NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_DOWN].special_width;
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_DOWN].special_attack_right[i].h=NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_DOWN].special_height;

        SPRITES_NPC[NPC_FIREBALL_SHOOTER_DOWN].special_attack_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_DOWN].special_attack_left[i].x=0;
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_DOWN].special_attack_left[i].y=0;
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_DOWN].special_attack_left[i].w=NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_DOWN].special_width;
        SPRITES_NPC[NPC_FIREBALL_SHOOTER_DOWN].special_attack_left[i].h=NPC_SPRITE_COUNTS[NPC_FIREBALL_SHOOTER_DOWN].special_height;
    }

    //Dust devil
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_DUST_DEVIL].fly_sprites;i++){
        SPRITES_NPC[NPC_DUST_DEVIL].fly_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_DUST_DEVIL].fly_right[i].x=i*NPC_SPRITE_COUNTS[NPC_DUST_DEVIL].width;
        SPRITES_NPC[NPC_DUST_DEVIL].fly_right[i].y=0;
        SPRITES_NPC[NPC_DUST_DEVIL].fly_right[i].w=NPC_SPRITE_COUNTS[NPC_DUST_DEVIL].width;
        SPRITES_NPC[NPC_DUST_DEVIL].fly_right[i].h=NPC_SPRITE_COUNTS[NPC_DUST_DEVIL].height;

        SPRITES_NPC[NPC_DUST_DEVIL].fly_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_DUST_DEVIL].fly_left[i].x=i*NPC_SPRITE_COUNTS[NPC_DUST_DEVIL].width;
        SPRITES_NPC[NPC_DUST_DEVIL].fly_left[i].y=NPC_SPRITE_COUNTS[NPC_DUST_DEVIL].height;
        SPRITES_NPC[NPC_DUST_DEVIL].fly_left[i].w=NPC_SPRITE_COUNTS[NPC_DUST_DEVIL].width;
        SPRITES_NPC[NPC_DUST_DEVIL].fly_left[i].h=NPC_SPRITE_COUNTS[NPC_DUST_DEVIL].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_DUST_DEVIL].dead_sprites;i++){
        SPRITES_NPC[NPC_DUST_DEVIL].dead.push_back(SDL_Rect());
        SPRITES_NPC[NPC_DUST_DEVIL].dead[i].x=i*NPC_SPRITE_COUNTS[NPC_DUST_DEVIL].dead_width;
        SPRITES_NPC[NPC_DUST_DEVIL].dead[i].y=NPC_SPRITE_COUNTS[NPC_DUST_DEVIL].height*2;
        SPRITES_NPC[NPC_DUST_DEVIL].dead[i].w=NPC_SPRITE_COUNTS[NPC_DUST_DEVIL].dead_width;
        SPRITES_NPC[NPC_DUST_DEVIL].dead[i].h=NPC_SPRITE_COUNTS[NPC_DUST_DEVIL].dead_height;
    }

    //Scorpion
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_SCORPION].walk_sprites;i++){
        SPRITES_NPC[NPC_SCORPION].walk_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SCORPION].walk_right[i].x=i*NPC_SPRITE_COUNTS[NPC_SCORPION].width;
        SPRITES_NPC[NPC_SCORPION].walk_right[i].y=0;
        SPRITES_NPC[NPC_SCORPION].walk_right[i].w=NPC_SPRITE_COUNTS[NPC_SCORPION].width;
        SPRITES_NPC[NPC_SCORPION].walk_right[i].h=NPC_SPRITE_COUNTS[NPC_SCORPION].height;

        SPRITES_NPC[NPC_SCORPION].walk_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SCORPION].walk_left[i].x=i*NPC_SPRITE_COUNTS[NPC_SCORPION].width;
        SPRITES_NPC[NPC_SCORPION].walk_left[i].y=NPC_SPRITE_COUNTS[NPC_SCORPION].height;
        SPRITES_NPC[NPC_SCORPION].walk_left[i].w=NPC_SPRITE_COUNTS[NPC_SCORPION].width;
        SPRITES_NPC[NPC_SCORPION].walk_left[i].h=NPC_SPRITE_COUNTS[NPC_SCORPION].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_SCORPION].jump_sprites;i++){
        SPRITES_NPC[NPC_SCORPION].jump_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SCORPION].jump_right[i].x=i*NPC_SPRITE_COUNTS[NPC_SCORPION].width;
        SPRITES_NPC[NPC_SCORPION].jump_right[i].y=0;
        SPRITES_NPC[NPC_SCORPION].jump_right[i].w=NPC_SPRITE_COUNTS[NPC_SCORPION].width;
        SPRITES_NPC[NPC_SCORPION].jump_right[i].h=NPC_SPRITE_COUNTS[NPC_SCORPION].height;

        SPRITES_NPC[NPC_SCORPION].jump_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SCORPION].jump_left[i].x=i*NPC_SPRITE_COUNTS[NPC_SCORPION].width;
        SPRITES_NPC[NPC_SCORPION].jump_left[i].y=NPC_SPRITE_COUNTS[NPC_SCORPION].height;
        SPRITES_NPC[NPC_SCORPION].jump_left[i].w=NPC_SPRITE_COUNTS[NPC_SCORPION].width;
        SPRITES_NPC[NPC_SCORPION].jump_left[i].h=NPC_SPRITE_COUNTS[NPC_SCORPION].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_SCORPION].swim_sprites;i++){
        SPRITES_NPC[NPC_SCORPION].swim_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SCORPION].swim_right[i].x=i*NPC_SPRITE_COUNTS[NPC_SCORPION].width;
        SPRITES_NPC[NPC_SCORPION].swim_right[i].y=0;
        SPRITES_NPC[NPC_SCORPION].swim_right[i].w=NPC_SPRITE_COUNTS[NPC_SCORPION].width;
        SPRITES_NPC[NPC_SCORPION].swim_right[i].h=NPC_SPRITE_COUNTS[NPC_SCORPION].height;

        SPRITES_NPC[NPC_SCORPION].swim_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SCORPION].swim_left[i].x=i*NPC_SPRITE_COUNTS[NPC_SCORPION].width;
        SPRITES_NPC[NPC_SCORPION].swim_left[i].y=NPC_SPRITE_COUNTS[NPC_SCORPION].height;
        SPRITES_NPC[NPC_SCORPION].swim_left[i].w=NPC_SPRITE_COUNTS[NPC_SCORPION].width;
        SPRITES_NPC[NPC_SCORPION].swim_left[i].h=NPC_SPRITE_COUNTS[NPC_SCORPION].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_SCORPION].special_attack_sprites;i++){
        SPRITES_NPC[NPC_SCORPION].special_attack_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SCORPION].special_attack_right[i].x=i*NPC_SPRITE_COUNTS[NPC_SCORPION].special_width;
        SPRITES_NPC[NPC_SCORPION].special_attack_right[i].y=NPC_SPRITE_COUNTS[NPC_SCORPION].height*2;
        SPRITES_NPC[NPC_SCORPION].special_attack_right[i].w=NPC_SPRITE_COUNTS[NPC_SCORPION].special_width;
        SPRITES_NPC[NPC_SCORPION].special_attack_right[i].h=NPC_SPRITE_COUNTS[NPC_SCORPION].special_height;

        SPRITES_NPC[NPC_SCORPION].special_attack_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SCORPION].special_attack_left[i].x=i*NPC_SPRITE_COUNTS[NPC_SCORPION].special_width;
        SPRITES_NPC[NPC_SCORPION].special_attack_left[i].y=NPC_SPRITE_COUNTS[NPC_SCORPION].height*3;
        SPRITES_NPC[NPC_SCORPION].special_attack_left[i].w=NPC_SPRITE_COUNTS[NPC_SCORPION].special_width;
        SPRITES_NPC[NPC_SCORPION].special_attack_left[i].h=NPC_SPRITE_COUNTS[NPC_SCORPION].special_height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_SCORPION].dead_sprites;i++){
        SPRITES_NPC[NPC_SCORPION].dead.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SCORPION].dead[i].x=i*NPC_SPRITE_COUNTS[NPC_SCORPION].dead_width;
        SPRITES_NPC[NPC_SCORPION].dead[i].y=NPC_SPRITE_COUNTS[NPC_SCORPION].height*4;
        SPRITES_NPC[NPC_SCORPION].dead[i].w=NPC_SPRITE_COUNTS[NPC_SCORPION].dead_width;
        SPRITES_NPC[NPC_SCORPION].dead[i].h=NPC_SPRITE_COUNTS[NPC_SCORPION].dead_height;
    }

    //Sphinx left
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_SPHINX_LEFT].walk_sprites;i++){
        SPRITES_NPC[NPC_SPHINX_LEFT].walk_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SPHINX_LEFT].walk_right[i].x=i*NPC_SPRITE_COUNTS[NPC_SPHINX_LEFT].width;
        SPRITES_NPC[NPC_SPHINX_LEFT].walk_right[i].y=0;
        SPRITES_NPC[NPC_SPHINX_LEFT].walk_right[i].w=NPC_SPRITE_COUNTS[NPC_SPHINX_LEFT].width;
        SPRITES_NPC[NPC_SPHINX_LEFT].walk_right[i].h=NPC_SPRITE_COUNTS[NPC_SPHINX_LEFT].height;

        SPRITES_NPC[NPC_SPHINX_LEFT].walk_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SPHINX_LEFT].walk_left[i].x=i*NPC_SPRITE_COUNTS[NPC_SPHINX_LEFT].width;
        SPRITES_NPC[NPC_SPHINX_LEFT].walk_left[i].y=NPC_SPRITE_COUNTS[NPC_SPHINX_LEFT].height;
        SPRITES_NPC[NPC_SPHINX_LEFT].walk_left[i].w=NPC_SPRITE_COUNTS[NPC_SPHINX_LEFT].width;
        SPRITES_NPC[NPC_SPHINX_LEFT].walk_left[i].h=NPC_SPRITE_COUNTS[NPC_SPHINX_LEFT].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_SPHINX_LEFT].special_attack_sprites;i++){
        SPRITES_NPC[NPC_SPHINX_LEFT].special_attack_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SPHINX_LEFT].special_attack_right[i].x=i*NPC_SPRITE_COUNTS[NPC_SPHINX_LEFT].special_width;
        SPRITES_NPC[NPC_SPHINX_LEFT].special_attack_right[i].y=NPC_SPRITE_COUNTS[NPC_SPHINX_LEFT].height*2;
        SPRITES_NPC[NPC_SPHINX_LEFT].special_attack_right[i].w=NPC_SPRITE_COUNTS[NPC_SPHINX_LEFT].special_width;
        SPRITES_NPC[NPC_SPHINX_LEFT].special_attack_right[i].h=NPC_SPRITE_COUNTS[NPC_SPHINX_LEFT].special_height;

        SPRITES_NPC[NPC_SPHINX_LEFT].special_attack_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SPHINX_LEFT].special_attack_left[i].x=i*NPC_SPRITE_COUNTS[NPC_SPHINX_LEFT].special_width;
        SPRITES_NPC[NPC_SPHINX_LEFT].special_attack_left[i].y=NPC_SPRITE_COUNTS[NPC_SPHINX_LEFT].height*3;
        SPRITES_NPC[NPC_SPHINX_LEFT].special_attack_left[i].w=NPC_SPRITE_COUNTS[NPC_SPHINX_LEFT].special_width;
        SPRITES_NPC[NPC_SPHINX_LEFT].special_attack_left[i].h=NPC_SPRITE_COUNTS[NPC_SPHINX_LEFT].special_height;
    }

    //Sphinx right
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_SPHINX_RIGHT].walk_sprites;i++){
        SPRITES_NPC[NPC_SPHINX_RIGHT].walk_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SPHINX_RIGHT].walk_right[i].x=i*NPC_SPRITE_COUNTS[NPC_SPHINX_RIGHT].width;
        SPRITES_NPC[NPC_SPHINX_RIGHT].walk_right[i].y=0;
        SPRITES_NPC[NPC_SPHINX_RIGHT].walk_right[i].w=NPC_SPRITE_COUNTS[NPC_SPHINX_RIGHT].width;
        SPRITES_NPC[NPC_SPHINX_RIGHT].walk_right[i].h=NPC_SPRITE_COUNTS[NPC_SPHINX_RIGHT].height;

        SPRITES_NPC[NPC_SPHINX_RIGHT].walk_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SPHINX_RIGHT].walk_left[i].x=i*NPC_SPRITE_COUNTS[NPC_SPHINX_RIGHT].width;
        SPRITES_NPC[NPC_SPHINX_RIGHT].walk_left[i].y=NPC_SPRITE_COUNTS[NPC_SPHINX_RIGHT].height;
        SPRITES_NPC[NPC_SPHINX_RIGHT].walk_left[i].w=NPC_SPRITE_COUNTS[NPC_SPHINX_RIGHT].width;
        SPRITES_NPC[NPC_SPHINX_RIGHT].walk_left[i].h=NPC_SPRITE_COUNTS[NPC_SPHINX_RIGHT].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_SPHINX_RIGHT].special_attack_sprites;i++){
        SPRITES_NPC[NPC_SPHINX_RIGHT].special_attack_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SPHINX_RIGHT].special_attack_right[i].x=i*NPC_SPRITE_COUNTS[NPC_SPHINX_RIGHT].special_width;
        SPRITES_NPC[NPC_SPHINX_RIGHT].special_attack_right[i].y=NPC_SPRITE_COUNTS[NPC_SPHINX_RIGHT].height*2;
        SPRITES_NPC[NPC_SPHINX_RIGHT].special_attack_right[i].w=NPC_SPRITE_COUNTS[NPC_SPHINX_RIGHT].special_width;
        SPRITES_NPC[NPC_SPHINX_RIGHT].special_attack_right[i].h=NPC_SPRITE_COUNTS[NPC_SPHINX_RIGHT].special_height;

        SPRITES_NPC[NPC_SPHINX_RIGHT].special_attack_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SPHINX_RIGHT].special_attack_left[i].x=i*NPC_SPRITE_COUNTS[NPC_SPHINX_RIGHT].special_width;
        SPRITES_NPC[NPC_SPHINX_RIGHT].special_attack_left[i].y=NPC_SPRITE_COUNTS[NPC_SPHINX_RIGHT].height*3;
        SPRITES_NPC[NPC_SPHINX_RIGHT].special_attack_left[i].w=NPC_SPRITE_COUNTS[NPC_SPHINX_RIGHT].special_width;
        SPRITES_NPC[NPC_SPHINX_RIGHT].special_attack_left[i].h=NPC_SPRITE_COUNTS[NPC_SPHINX_RIGHT].special_height;
    }

    //Shark
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_SHARK].swim_sprites;i++){
        SPRITES_NPC[NPC_SHARK].swim_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SHARK].swim_right[i].x=i*NPC_SPRITE_COUNTS[NPC_SHARK].width;
        SPRITES_NPC[NPC_SHARK].swim_right[i].y=0;
        SPRITES_NPC[NPC_SHARK].swim_right[i].w=NPC_SPRITE_COUNTS[NPC_SHARK].width;
        SPRITES_NPC[NPC_SHARK].swim_right[i].h=NPC_SPRITE_COUNTS[NPC_SHARK].height;

        SPRITES_NPC[NPC_SHARK].swim_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SHARK].swim_left[i].x=i*NPC_SPRITE_COUNTS[NPC_SHARK].width;
        SPRITES_NPC[NPC_SHARK].swim_left[i].y=NPC_SPRITE_COUNTS[NPC_SHARK].height;
        SPRITES_NPC[NPC_SHARK].swim_left[i].w=NPC_SPRITE_COUNTS[NPC_SHARK].width;
        SPRITES_NPC[NPC_SHARK].swim_left[i].h=NPC_SPRITE_COUNTS[NPC_SHARK].height;
    }

    //Henchmouse
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].walk_sprites;i++){
        SPRITES_NPC[NPC_HENCHMOUSE].walk_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_HENCHMOUSE].walk_right[i].x=i*NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].width;
        SPRITES_NPC[NPC_HENCHMOUSE].walk_right[i].y=0;
        SPRITES_NPC[NPC_HENCHMOUSE].walk_right[i].w=NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].width;
        SPRITES_NPC[NPC_HENCHMOUSE].walk_right[i].h=NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].height;

        SPRITES_NPC[NPC_HENCHMOUSE].walk_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_HENCHMOUSE].walk_left[i].x=i*NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].width;
        SPRITES_NPC[NPC_HENCHMOUSE].walk_left[i].y=NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].height;
        SPRITES_NPC[NPC_HENCHMOUSE].walk_left[i].w=NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].width;
        SPRITES_NPC[NPC_HENCHMOUSE].walk_left[i].h=NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].jump_sprites;i++){
        SPRITES_NPC[NPC_HENCHMOUSE].jump_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_HENCHMOUSE].jump_right[i].x=i*NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].width;
        SPRITES_NPC[NPC_HENCHMOUSE].jump_right[i].y=0;
        SPRITES_NPC[NPC_HENCHMOUSE].jump_right[i].w=NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].width;
        SPRITES_NPC[NPC_HENCHMOUSE].jump_right[i].h=NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].height;

        SPRITES_NPC[NPC_HENCHMOUSE].jump_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_HENCHMOUSE].jump_left[i].x=i*NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].width;
        SPRITES_NPC[NPC_HENCHMOUSE].jump_left[i].y=NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].height;
        SPRITES_NPC[NPC_HENCHMOUSE].jump_left[i].w=NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].width;
        SPRITES_NPC[NPC_HENCHMOUSE].jump_left[i].h=NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].swim_sprites;i++){
        SPRITES_NPC[NPC_HENCHMOUSE].swim_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_HENCHMOUSE].swim_right[i].x=i*NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].width;
        SPRITES_NPC[NPC_HENCHMOUSE].swim_right[i].y=0;
        SPRITES_NPC[NPC_HENCHMOUSE].swim_right[i].w=NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].width;
        SPRITES_NPC[NPC_HENCHMOUSE].swim_right[i].h=NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].height;

        SPRITES_NPC[NPC_HENCHMOUSE].swim_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_HENCHMOUSE].swim_left[i].x=i*NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].width;
        SPRITES_NPC[NPC_HENCHMOUSE].swim_left[i].y=NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].height;
        SPRITES_NPC[NPC_HENCHMOUSE].swim_left[i].w=NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].width;
        SPRITES_NPC[NPC_HENCHMOUSE].swim_left[i].h=NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].special_attack_sprites;i++){
        SPRITES_NPC[NPC_HENCHMOUSE].special_attack_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_HENCHMOUSE].special_attack_right[i].x=i*NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].special_width;
        SPRITES_NPC[NPC_HENCHMOUSE].special_attack_right[i].y=NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].height*2;
        SPRITES_NPC[NPC_HENCHMOUSE].special_attack_right[i].w=NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].special_width;
        SPRITES_NPC[NPC_HENCHMOUSE].special_attack_right[i].h=NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].special_height;

        SPRITES_NPC[NPC_HENCHMOUSE].special_attack_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_HENCHMOUSE].special_attack_left[i].x=i*NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].special_width;
        SPRITES_NPC[NPC_HENCHMOUSE].special_attack_left[i].y=NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].height*3;
        SPRITES_NPC[NPC_HENCHMOUSE].special_attack_left[i].w=NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].special_width;
        SPRITES_NPC[NPC_HENCHMOUSE].special_attack_left[i].h=NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].special_height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].dead_sprites;i++){
        SPRITES_NPC[NPC_HENCHMOUSE].dead.push_back(SDL_Rect());
        SPRITES_NPC[NPC_HENCHMOUSE].dead[i].x=i*NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].dead_width;
        SPRITES_NPC[NPC_HENCHMOUSE].dead[i].y=NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].height*4;
        SPRITES_NPC[NPC_HENCHMOUSE].dead[i].w=NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].dead_width;
        SPRITES_NPC[NPC_HENCHMOUSE].dead[i].h=NPC_SPRITE_COUNTS[NPC_HENCHMOUSE].dead_height;
    }

    //Mechsuit mouse
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_MECHSUIT_MOUSE].fly_sprites;i++){
        SPRITES_NPC[NPC_MECHSUIT_MOUSE].fly_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_MECHSUIT_MOUSE].fly_right[i].x=i*NPC_SPRITE_COUNTS[NPC_MECHSUIT_MOUSE].width;
        SPRITES_NPC[NPC_MECHSUIT_MOUSE].fly_right[i].y=0;
        SPRITES_NPC[NPC_MECHSUIT_MOUSE].fly_right[i].w=NPC_SPRITE_COUNTS[NPC_MECHSUIT_MOUSE].width;
        SPRITES_NPC[NPC_MECHSUIT_MOUSE].fly_right[i].h=NPC_SPRITE_COUNTS[NPC_MECHSUIT_MOUSE].height;

        SPRITES_NPC[NPC_MECHSUIT_MOUSE].fly_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_MECHSUIT_MOUSE].fly_left[i].x=i*NPC_SPRITE_COUNTS[NPC_MECHSUIT_MOUSE].width;
        SPRITES_NPC[NPC_MECHSUIT_MOUSE].fly_left[i].y=NPC_SPRITE_COUNTS[NPC_MECHSUIT_MOUSE].height;
        SPRITES_NPC[NPC_MECHSUIT_MOUSE].fly_left[i].w=NPC_SPRITE_COUNTS[NPC_MECHSUIT_MOUSE].width;
        SPRITES_NPC[NPC_MECHSUIT_MOUSE].fly_left[i].h=NPC_SPRITE_COUNTS[NPC_MECHSUIT_MOUSE].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_MECHSUIT_MOUSE].special_attack_sprites;i++){
        SPRITES_NPC[NPC_MECHSUIT_MOUSE].special_attack_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_MECHSUIT_MOUSE].special_attack_right[i].x=i*NPC_SPRITE_COUNTS[NPC_MECHSUIT_MOUSE].special_width;
        SPRITES_NPC[NPC_MECHSUIT_MOUSE].special_attack_right[i].y=0;
        SPRITES_NPC[NPC_MECHSUIT_MOUSE].special_attack_right[i].w=NPC_SPRITE_COUNTS[NPC_MECHSUIT_MOUSE].special_width;
        SPRITES_NPC[NPC_MECHSUIT_MOUSE].special_attack_right[i].h=NPC_SPRITE_COUNTS[NPC_MECHSUIT_MOUSE].special_height;

        SPRITES_NPC[NPC_MECHSUIT_MOUSE].special_attack_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_MECHSUIT_MOUSE].special_attack_left[i].x=i*NPC_SPRITE_COUNTS[NPC_MECHSUIT_MOUSE].special_width;
        SPRITES_NPC[NPC_MECHSUIT_MOUSE].special_attack_left[i].y=NPC_SPRITE_COUNTS[NPC_MECHSUIT_MOUSE].height;
        SPRITES_NPC[NPC_MECHSUIT_MOUSE].special_attack_left[i].w=NPC_SPRITE_COUNTS[NPC_MECHSUIT_MOUSE].special_width;
        SPRITES_NPC[NPC_MECHSUIT_MOUSE].special_attack_left[i].h=NPC_SPRITE_COUNTS[NPC_MECHSUIT_MOUSE].special_height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_MECHSUIT_MOUSE].dead_sprites;i++){
        SPRITES_NPC[NPC_MECHSUIT_MOUSE].dead.push_back(SDL_Rect());
        SPRITES_NPC[NPC_MECHSUIT_MOUSE].dead[i].x=i*NPC_SPRITE_COUNTS[NPC_MECHSUIT_MOUSE].dead_width;
        SPRITES_NPC[NPC_MECHSUIT_MOUSE].dead[i].y=NPC_SPRITE_COUNTS[NPC_MECHSUIT_MOUSE].height*2;
        SPRITES_NPC[NPC_MECHSUIT_MOUSE].dead[i].w=NPC_SPRITE_COUNTS[NPC_MECHSUIT_MOUSE].dead_width;
        SPRITES_NPC[NPC_MECHSUIT_MOUSE].dead[i].h=NPC_SPRITE_COUNTS[NPC_MECHSUIT_MOUSE].dead_height;
    }

    //Sally
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_SALLY].walk_sprites;i++){
        SPRITES_NPC[NPC_SALLY].walk_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SALLY].walk_right[i].x=i*NPC_SPRITE_COUNTS[NPC_SALLY].width;
        SPRITES_NPC[NPC_SALLY].walk_right[i].y=0;
        SPRITES_NPC[NPC_SALLY].walk_right[i].w=NPC_SPRITE_COUNTS[NPC_SALLY].width;
        SPRITES_NPC[NPC_SALLY].walk_right[i].h=NPC_SPRITE_COUNTS[NPC_SALLY].height;

        SPRITES_NPC[NPC_SALLY].walk_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SALLY].walk_left[i].x=i*NPC_SPRITE_COUNTS[NPC_SALLY].width;
        SPRITES_NPC[NPC_SALLY].walk_left[i].y=NPC_SPRITE_COUNTS[NPC_SALLY].height;
        SPRITES_NPC[NPC_SALLY].walk_left[i].w=NPC_SPRITE_COUNTS[NPC_SALLY].width;
        SPRITES_NPC[NPC_SALLY].walk_left[i].h=NPC_SPRITE_COUNTS[NPC_SALLY].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_SALLY].jump_sprites;i++){
        SPRITES_NPC[NPC_SALLY].jump_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SALLY].jump_right[i].x=i*NPC_SPRITE_COUNTS[NPC_SALLY].width;
        SPRITES_NPC[NPC_SALLY].jump_right[i].y=0;
        SPRITES_NPC[NPC_SALLY].jump_right[i].w=NPC_SPRITE_COUNTS[NPC_SALLY].width;
        SPRITES_NPC[NPC_SALLY].jump_right[i].h=NPC_SPRITE_COUNTS[NPC_SALLY].height;

        SPRITES_NPC[NPC_SALLY].jump_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SALLY].jump_left[i].x=i*NPC_SPRITE_COUNTS[NPC_SALLY].width;
        SPRITES_NPC[NPC_SALLY].jump_left[i].y=NPC_SPRITE_COUNTS[NPC_SALLY].height;
        SPRITES_NPC[NPC_SALLY].jump_left[i].w=NPC_SPRITE_COUNTS[NPC_SALLY].width;
        SPRITES_NPC[NPC_SALLY].jump_left[i].h=NPC_SPRITE_COUNTS[NPC_SALLY].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_SALLY].swim_sprites;i++){
        SPRITES_NPC[NPC_SALLY].swim_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SALLY].swim_right[i].x=i*NPC_SPRITE_COUNTS[NPC_SALLY].width;
        SPRITES_NPC[NPC_SALLY].swim_right[i].y=NPC_SPRITE_COUNTS[NPC_SALLY].height*2;
        SPRITES_NPC[NPC_SALLY].swim_right[i].w=NPC_SPRITE_COUNTS[NPC_SALLY].width;
        SPRITES_NPC[NPC_SALLY].swim_right[i].h=NPC_SPRITE_COUNTS[NPC_SALLY].height;

        SPRITES_NPC[NPC_SALLY].swim_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SALLY].swim_left[i].x=i*NPC_SPRITE_COUNTS[NPC_SALLY].width;
        SPRITES_NPC[NPC_SALLY].swim_left[i].y=NPC_SPRITE_COUNTS[NPC_SALLY].height*3;
        SPRITES_NPC[NPC_SALLY].swim_left[i].w=NPC_SPRITE_COUNTS[NPC_SALLY].width;
        SPRITES_NPC[NPC_SALLY].swim_left[i].h=NPC_SPRITE_COUNTS[NPC_SALLY].height;
    }

    //Bouncing cow
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_BOUNCING_COW].walk_sprites;i++){
        SPRITES_NPC[NPC_BOUNCING_COW].walk_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_BOUNCING_COW].walk_right[i].x=i*NPC_SPRITE_COUNTS[NPC_BOUNCING_COW].width;
        SPRITES_NPC[NPC_BOUNCING_COW].walk_right[i].y=0;
        SPRITES_NPC[NPC_BOUNCING_COW].walk_right[i].w=NPC_SPRITE_COUNTS[NPC_BOUNCING_COW].width;
        SPRITES_NPC[NPC_BOUNCING_COW].walk_right[i].h=NPC_SPRITE_COUNTS[NPC_BOUNCING_COW].height;

        SPRITES_NPC[NPC_BOUNCING_COW].walk_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_BOUNCING_COW].walk_left[i].x=i*NPC_SPRITE_COUNTS[NPC_BOUNCING_COW].width;
        SPRITES_NPC[NPC_BOUNCING_COW].walk_left[i].y=NPC_SPRITE_COUNTS[NPC_BOUNCING_COW].height;
        SPRITES_NPC[NPC_BOUNCING_COW].walk_left[i].w=NPC_SPRITE_COUNTS[NPC_BOUNCING_COW].width;
        SPRITES_NPC[NPC_BOUNCING_COW].walk_left[i].h=NPC_SPRITE_COUNTS[NPC_BOUNCING_COW].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_BOUNCING_COW].jump_sprites;i++){
        SPRITES_NPC[NPC_BOUNCING_COW].jump_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_BOUNCING_COW].jump_right[i].x=i*NPC_SPRITE_COUNTS[NPC_BOUNCING_COW].width;
        SPRITES_NPC[NPC_BOUNCING_COW].jump_right[i].y=0;
        SPRITES_NPC[NPC_BOUNCING_COW].jump_right[i].w=NPC_SPRITE_COUNTS[NPC_BOUNCING_COW].width;
        SPRITES_NPC[NPC_BOUNCING_COW].jump_right[i].h=NPC_SPRITE_COUNTS[NPC_BOUNCING_COW].height;

        SPRITES_NPC[NPC_BOUNCING_COW].jump_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_BOUNCING_COW].jump_left[i].x=i*NPC_SPRITE_COUNTS[NPC_BOUNCING_COW].width;
        SPRITES_NPC[NPC_BOUNCING_COW].jump_left[i].y=NPC_SPRITE_COUNTS[NPC_BOUNCING_COW].height;
        SPRITES_NPC[NPC_BOUNCING_COW].jump_left[i].w=NPC_SPRITE_COUNTS[NPC_BOUNCING_COW].width;
        SPRITES_NPC[NPC_BOUNCING_COW].jump_left[i].h=NPC_SPRITE_COUNTS[NPC_BOUNCING_COW].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_BOUNCING_COW].swim_sprites;i++){
        SPRITES_NPC[NPC_BOUNCING_COW].swim_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_BOUNCING_COW].swim_right[i].x=i*NPC_SPRITE_COUNTS[NPC_BOUNCING_COW].width;
        SPRITES_NPC[NPC_BOUNCING_COW].swim_right[i].y=0;
        SPRITES_NPC[NPC_BOUNCING_COW].swim_right[i].w=NPC_SPRITE_COUNTS[NPC_BOUNCING_COW].width;
        SPRITES_NPC[NPC_BOUNCING_COW].swim_right[i].h=NPC_SPRITE_COUNTS[NPC_BOUNCING_COW].height;

        SPRITES_NPC[NPC_BOUNCING_COW].swim_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_BOUNCING_COW].swim_left[i].x=i*NPC_SPRITE_COUNTS[NPC_BOUNCING_COW].width;
        SPRITES_NPC[NPC_BOUNCING_COW].swim_left[i].y=NPC_SPRITE_COUNTS[NPC_BOUNCING_COW].height;
        SPRITES_NPC[NPC_BOUNCING_COW].swim_left[i].w=NPC_SPRITE_COUNTS[NPC_BOUNCING_COW].width;
        SPRITES_NPC[NPC_BOUNCING_COW].swim_left[i].h=NPC_SPRITE_COUNTS[NPC_BOUNCING_COW].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_BOUNCING_COW].dead_sprites;i++){
        SPRITES_NPC[NPC_BOUNCING_COW].dead.push_back(SDL_Rect());
        SPRITES_NPC[NPC_BOUNCING_COW].dead[i].x=i*NPC_SPRITE_COUNTS[NPC_BOUNCING_COW].dead_width;
        SPRITES_NPC[NPC_BOUNCING_COW].dead[i].y=NPC_SPRITE_COUNTS[NPC_BOUNCING_COW].height*4;
        SPRITES_NPC[NPC_BOUNCING_COW].dead[i].w=NPC_SPRITE_COUNTS[NPC_BOUNCING_COW].dead_width;
        SPRITES_NPC[NPC_BOUNCING_COW].dead[i].h=NPC_SPRITE_COUNTS[NPC_BOUNCING_COW].dead_height;
    }

    //Bouncing ball
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_BOUNCING_BALL].walk_sprites;i++){
        SPRITES_NPC[NPC_BOUNCING_BALL].walk_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_BOUNCING_BALL].walk_right[i].x=i*NPC_SPRITE_COUNTS[NPC_BOUNCING_BALL].width;
        SPRITES_NPC[NPC_BOUNCING_BALL].walk_right[i].y=0;
        SPRITES_NPC[NPC_BOUNCING_BALL].walk_right[i].w=NPC_SPRITE_COUNTS[NPC_BOUNCING_BALL].width;
        SPRITES_NPC[NPC_BOUNCING_BALL].walk_right[i].h=NPC_SPRITE_COUNTS[NPC_BOUNCING_BALL].height;

        SPRITES_NPC[NPC_BOUNCING_BALL].walk_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_BOUNCING_BALL].walk_left[i].x=i*NPC_SPRITE_COUNTS[NPC_BOUNCING_BALL].width;
        SPRITES_NPC[NPC_BOUNCING_BALL].walk_left[i].y=NPC_SPRITE_COUNTS[NPC_BOUNCING_BALL].height;
        SPRITES_NPC[NPC_BOUNCING_BALL].walk_left[i].w=NPC_SPRITE_COUNTS[NPC_BOUNCING_BALL].width;
        SPRITES_NPC[NPC_BOUNCING_BALL].walk_left[i].h=NPC_SPRITE_COUNTS[NPC_BOUNCING_BALL].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_BOUNCING_BALL].jump_sprites;i++){
        SPRITES_NPC[NPC_BOUNCING_BALL].jump_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_BOUNCING_BALL].jump_right[i].x=i*NPC_SPRITE_COUNTS[NPC_BOUNCING_BALL].width;
        SPRITES_NPC[NPC_BOUNCING_BALL].jump_right[i].y=0;
        SPRITES_NPC[NPC_BOUNCING_BALL].jump_right[i].w=NPC_SPRITE_COUNTS[NPC_BOUNCING_BALL].width;
        SPRITES_NPC[NPC_BOUNCING_BALL].jump_right[i].h=NPC_SPRITE_COUNTS[NPC_BOUNCING_BALL].height;

        SPRITES_NPC[NPC_BOUNCING_BALL].jump_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_BOUNCING_BALL].jump_left[i].x=i*NPC_SPRITE_COUNTS[NPC_BOUNCING_BALL].width;
        SPRITES_NPC[NPC_BOUNCING_BALL].jump_left[i].y=NPC_SPRITE_COUNTS[NPC_BOUNCING_BALL].height;
        SPRITES_NPC[NPC_BOUNCING_BALL].jump_left[i].w=NPC_SPRITE_COUNTS[NPC_BOUNCING_BALL].width;
        SPRITES_NPC[NPC_BOUNCING_BALL].jump_left[i].h=NPC_SPRITE_COUNTS[NPC_BOUNCING_BALL].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_BOUNCING_BALL].swim_sprites;i++){
        SPRITES_NPC[NPC_BOUNCING_BALL].swim_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_BOUNCING_BALL].swim_right[i].x=i*NPC_SPRITE_COUNTS[NPC_BOUNCING_BALL].width;
        SPRITES_NPC[NPC_BOUNCING_BALL].swim_right[i].y=0;
        SPRITES_NPC[NPC_BOUNCING_BALL].swim_right[i].w=NPC_SPRITE_COUNTS[NPC_BOUNCING_BALL].width;
        SPRITES_NPC[NPC_BOUNCING_BALL].swim_right[i].h=NPC_SPRITE_COUNTS[NPC_BOUNCING_BALL].height;

        SPRITES_NPC[NPC_BOUNCING_BALL].swim_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_BOUNCING_BALL].swim_left[i].x=i*NPC_SPRITE_COUNTS[NPC_BOUNCING_BALL].width;
        SPRITES_NPC[NPC_BOUNCING_BALL].swim_left[i].y=NPC_SPRITE_COUNTS[NPC_BOUNCING_BALL].height;
        SPRITES_NPC[NPC_BOUNCING_BALL].swim_left[i].w=NPC_SPRITE_COUNTS[NPC_BOUNCING_BALL].width;
        SPRITES_NPC[NPC_BOUNCING_BALL].swim_left[i].h=NPC_SPRITE_COUNTS[NPC_BOUNCING_BALL].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_BOUNCING_BALL].dead_sprites;i++){
        SPRITES_NPC[NPC_BOUNCING_BALL].dead.push_back(SDL_Rect());
        SPRITES_NPC[NPC_BOUNCING_BALL].dead[i].x=i*NPC_SPRITE_COUNTS[NPC_BOUNCING_BALL].dead_width;
        SPRITES_NPC[NPC_BOUNCING_BALL].dead[i].y=0;
        SPRITES_NPC[NPC_BOUNCING_BALL].dead[i].w=NPC_SPRITE_COUNTS[NPC_BOUNCING_BALL].dead_width;
        SPRITES_NPC[NPC_BOUNCING_BALL].dead[i].h=NPC_SPRITE_COUNTS[NPC_BOUNCING_BALL].dead_height;
    }

    //Sea turtle
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_SEA_TURTLE].swim_sprites;i++){
        SPRITES_NPC[NPC_SEA_TURTLE].swim_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SEA_TURTLE].swim_right[i].x=i*NPC_SPRITE_COUNTS[NPC_SEA_TURTLE].width;
        SPRITES_NPC[NPC_SEA_TURTLE].swim_right[i].y=0;
        SPRITES_NPC[NPC_SEA_TURTLE].swim_right[i].w=NPC_SPRITE_COUNTS[NPC_SEA_TURTLE].width;
        SPRITES_NPC[NPC_SEA_TURTLE].swim_right[i].h=NPC_SPRITE_COUNTS[NPC_SEA_TURTLE].height;

        SPRITES_NPC[NPC_SEA_TURTLE].swim_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_SEA_TURTLE].swim_left[i].x=i*NPC_SPRITE_COUNTS[NPC_SEA_TURTLE].width;
        SPRITES_NPC[NPC_SEA_TURTLE].swim_left[i].y=NPC_SPRITE_COUNTS[NPC_SEA_TURTLE].height;
        SPRITES_NPC[NPC_SEA_TURTLE].swim_left[i].w=NPC_SPRITE_COUNTS[NPC_SEA_TURTLE].width;
        SPRITES_NPC[NPC_SEA_TURTLE].swim_left[i].h=NPC_SPRITE_COUNTS[NPC_SEA_TURTLE].height;
    }

    //Starfish
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_STARFISH].swim_sprites;i++){
        SPRITES_NPC[NPC_STARFISH].swim_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_STARFISH].swim_right[i].x=i*NPC_SPRITE_COUNTS[NPC_STARFISH].width;
        SPRITES_NPC[NPC_STARFISH].swim_right[i].y=0;
        SPRITES_NPC[NPC_STARFISH].swim_right[i].w=NPC_SPRITE_COUNTS[NPC_STARFISH].width;
        SPRITES_NPC[NPC_STARFISH].swim_right[i].h=NPC_SPRITE_COUNTS[NPC_STARFISH].height;

        SPRITES_NPC[NPC_STARFISH].swim_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_STARFISH].swim_left[i].x=i*NPC_SPRITE_COUNTS[NPC_STARFISH].width;
        SPRITES_NPC[NPC_STARFISH].swim_left[i].y=NPC_SPRITE_COUNTS[NPC_STARFISH].height;
        SPRITES_NPC[NPC_STARFISH].swim_left[i].w=NPC_SPRITE_COUNTS[NPC_STARFISH].width;
        SPRITES_NPC[NPC_STARFISH].swim_left[i].h=NPC_SPRITE_COUNTS[NPC_STARFISH].height;
    }

    //Boss Castle
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_BOSS_CASTLE].fly_sprites;i++){
        SPRITES_NPC[NPC_BOSS_CASTLE].fly_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_BOSS_CASTLE].fly_right[i].x=i*NPC_SPRITE_COUNTS[NPC_BOSS_CASTLE].width;
        SPRITES_NPC[NPC_BOSS_CASTLE].fly_right[i].y=0;
        SPRITES_NPC[NPC_BOSS_CASTLE].fly_right[i].w=NPC_SPRITE_COUNTS[NPC_BOSS_CASTLE].width;
        SPRITES_NPC[NPC_BOSS_CASTLE].fly_right[i].h=NPC_SPRITE_COUNTS[NPC_BOSS_CASTLE].height;

        SPRITES_NPC[NPC_BOSS_CASTLE].fly_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_BOSS_CASTLE].fly_left[i].x=i*NPC_SPRITE_COUNTS[NPC_BOSS_CASTLE].width;
        SPRITES_NPC[NPC_BOSS_CASTLE].fly_left[i].y=NPC_SPRITE_COUNTS[NPC_BOSS_CASTLE].height;
        SPRITES_NPC[NPC_BOSS_CASTLE].fly_left[i].w=NPC_SPRITE_COUNTS[NPC_BOSS_CASTLE].width;
        SPRITES_NPC[NPC_BOSS_CASTLE].fly_left[i].h=NPC_SPRITE_COUNTS[NPC_BOSS_CASTLE].height;
    }

    //Boss Haunted House
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_BOSS_HAUNTED_HOUSE].fly_sprites;i++){
        SPRITES_NPC[NPC_BOSS_HAUNTED_HOUSE].fly_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_BOSS_HAUNTED_HOUSE].fly_right[i].x=i*NPC_SPRITE_COUNTS[NPC_BOSS_HAUNTED_HOUSE].width;
        SPRITES_NPC[NPC_BOSS_HAUNTED_HOUSE].fly_right[i].y=0;
        SPRITES_NPC[NPC_BOSS_HAUNTED_HOUSE].fly_right[i].w=NPC_SPRITE_COUNTS[NPC_BOSS_HAUNTED_HOUSE].width;
        SPRITES_NPC[NPC_BOSS_HAUNTED_HOUSE].fly_right[i].h=NPC_SPRITE_COUNTS[NPC_BOSS_HAUNTED_HOUSE].height;

        SPRITES_NPC[NPC_BOSS_HAUNTED_HOUSE].fly_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_BOSS_HAUNTED_HOUSE].fly_left[i].x=i*NPC_SPRITE_COUNTS[NPC_BOSS_HAUNTED_HOUSE].width;
        SPRITES_NPC[NPC_BOSS_HAUNTED_HOUSE].fly_left[i].y=NPC_SPRITE_COUNTS[NPC_BOSS_HAUNTED_HOUSE].height;
        SPRITES_NPC[NPC_BOSS_HAUNTED_HOUSE].fly_left[i].w=NPC_SPRITE_COUNTS[NPC_BOSS_HAUNTED_HOUSE].width;
        SPRITES_NPC[NPC_BOSS_HAUNTED_HOUSE].fly_left[i].h=NPC_SPRITE_COUNTS[NPC_BOSS_HAUNTED_HOUSE].height;
    }

    //Boss Volcano
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_BOSS_VOLCANO].walk_sprites;i++){
        SPRITES_NPC[NPC_BOSS_VOLCANO].walk_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_BOSS_VOLCANO].walk_right[i].x=i*NPC_SPRITE_COUNTS[NPC_BOSS_VOLCANO].width;
        SPRITES_NPC[NPC_BOSS_VOLCANO].walk_right[i].y=0;
        SPRITES_NPC[NPC_BOSS_VOLCANO].walk_right[i].w=NPC_SPRITE_COUNTS[NPC_BOSS_VOLCANO].width;
        SPRITES_NPC[NPC_BOSS_VOLCANO].walk_right[i].h=NPC_SPRITE_COUNTS[NPC_BOSS_VOLCANO].height;

        SPRITES_NPC[NPC_BOSS_VOLCANO].walk_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_BOSS_VOLCANO].walk_left[i].x=i*NPC_SPRITE_COUNTS[NPC_BOSS_VOLCANO].width;
        SPRITES_NPC[NPC_BOSS_VOLCANO].walk_left[i].y=NPC_SPRITE_COUNTS[NPC_BOSS_VOLCANO].height;
        SPRITES_NPC[NPC_BOSS_VOLCANO].walk_left[i].w=NPC_SPRITE_COUNTS[NPC_BOSS_VOLCANO].width;
        SPRITES_NPC[NPC_BOSS_VOLCANO].walk_left[i].h=NPC_SPRITE_COUNTS[NPC_BOSS_VOLCANO].height;
    }
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_BOSS_VOLCANO].jump_sprites;i++){
        SPRITES_NPC[NPC_BOSS_VOLCANO].jump_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_BOSS_VOLCANO].jump_right[i].x=i*NPC_SPRITE_COUNTS[NPC_BOSS_VOLCANO].width;
        SPRITES_NPC[NPC_BOSS_VOLCANO].jump_right[i].y=0;
        SPRITES_NPC[NPC_BOSS_VOLCANO].jump_right[i].w=NPC_SPRITE_COUNTS[NPC_BOSS_VOLCANO].width;
        SPRITES_NPC[NPC_BOSS_VOLCANO].jump_right[i].h=NPC_SPRITE_COUNTS[NPC_BOSS_VOLCANO].height;

        SPRITES_NPC[NPC_BOSS_VOLCANO].jump_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_BOSS_VOLCANO].jump_left[i].x=i*NPC_SPRITE_COUNTS[NPC_BOSS_VOLCANO].width;
        SPRITES_NPC[NPC_BOSS_VOLCANO].jump_left[i].y=NPC_SPRITE_COUNTS[NPC_BOSS_VOLCANO].height;
        SPRITES_NPC[NPC_BOSS_VOLCANO].jump_left[i].w=NPC_SPRITE_COUNTS[NPC_BOSS_VOLCANO].width;
        SPRITES_NPC[NPC_BOSS_VOLCANO].jump_left[i].h=NPC_SPRITE_COUNTS[NPC_BOSS_VOLCANO].height;
    }

    //Boss Pyramid
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_BOSS_PYRAMID].fly_sprites;i++){
        SPRITES_NPC[NPC_BOSS_PYRAMID].fly_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_BOSS_PYRAMID].fly_right[i].x=i*NPC_SPRITE_COUNTS[NPC_BOSS_PYRAMID].width;
        SPRITES_NPC[NPC_BOSS_PYRAMID].fly_right[i].y=0;
        SPRITES_NPC[NPC_BOSS_PYRAMID].fly_right[i].w=NPC_SPRITE_COUNTS[NPC_BOSS_PYRAMID].width;
        SPRITES_NPC[NPC_BOSS_PYRAMID].fly_right[i].h=NPC_SPRITE_COUNTS[NPC_BOSS_PYRAMID].height;

        SPRITES_NPC[NPC_BOSS_PYRAMID].fly_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_BOSS_PYRAMID].fly_left[i].x=i*NPC_SPRITE_COUNTS[NPC_BOSS_PYRAMID].width;
        SPRITES_NPC[NPC_BOSS_PYRAMID].fly_left[i].y=NPC_SPRITE_COUNTS[NPC_BOSS_PYRAMID].height;
        SPRITES_NPC[NPC_BOSS_PYRAMID].fly_left[i].w=NPC_SPRITE_COUNTS[NPC_BOSS_PYRAMID].width;
        SPRITES_NPC[NPC_BOSS_PYRAMID].fly_left[i].h=NPC_SPRITE_COUNTS[NPC_BOSS_PYRAMID].height;
    }

    //Boss Lighthouse
    for(short i=0;i<NPC_SPRITE_COUNTS[NPC_BOSS_LIGHTHOUSE].fly_sprites;i++){
        SPRITES_NPC[NPC_BOSS_LIGHTHOUSE].fly_right.push_back(SDL_Rect());
        SPRITES_NPC[NPC_BOSS_LIGHTHOUSE].fly_right[i].x=i*NPC_SPRITE_COUNTS[NPC_BOSS_LIGHTHOUSE].width;
        SPRITES_NPC[NPC_BOSS_LIGHTHOUSE].fly_right[i].y=0;
        SPRITES_NPC[NPC_BOSS_LIGHTHOUSE].fly_right[i].w=NPC_SPRITE_COUNTS[NPC_BOSS_LIGHTHOUSE].width;
        SPRITES_NPC[NPC_BOSS_LIGHTHOUSE].fly_right[i].h=NPC_SPRITE_COUNTS[NPC_BOSS_LIGHTHOUSE].height;

        SPRITES_NPC[NPC_BOSS_LIGHTHOUSE].fly_left.push_back(SDL_Rect());
        SPRITES_NPC[NPC_BOSS_LIGHTHOUSE].fly_left[i].x=i*NPC_SPRITE_COUNTS[NPC_BOSS_LIGHTHOUSE].width;
        SPRITES_NPC[NPC_BOSS_LIGHTHOUSE].fly_left[i].y=NPC_SPRITE_COUNTS[NPC_BOSS_LIGHTHOUSE].height;
        SPRITES_NPC[NPC_BOSS_LIGHTHOUSE].fly_left[i].w=NPC_SPRITE_COUNTS[NPC_BOSS_LIGHTHOUSE].width;
        SPRITES_NPC[NPC_BOSS_LIGHTHOUSE].fly_left[i].h=NPC_SPRITE_COUNTS[NPC_BOSS_LIGHTHOUSE].height;
    }
}

void create_sprite_sheets_traps(){
    SPRITES_TRAP.clear();

    for(short i=0;i<TRAP_END;i++){
        SPRITES_TRAP.push_back(trap_sprite_data());
    }

    //Fire
    for(short i=0;i<TRAP_SPRITE_COUNTS[TRAP_FIRE].idle_active_sprites;i++){
        SPRITES_TRAP[TRAP_FIRE].idle_active.push_back(SDL_Rect());
        SPRITES_TRAP[TRAP_FIRE].idle_active[i].x=i*TRAP_SPRITE_COUNTS[TRAP_FIRE].width;
        SPRITES_TRAP[TRAP_FIRE].idle_active[i].y=0;
        SPRITES_TRAP[TRAP_FIRE].idle_active[i].w=TRAP_SPRITE_COUNTS[TRAP_FIRE].width;
        SPRITES_TRAP[TRAP_FIRE].idle_active[i].h=TRAP_SPRITE_COUNTS[TRAP_FIRE].height;
    }
    for(short i=0;i<TRAP_SPRITE_COUNTS[TRAP_FIRE].idle_inactive_sprites;i++){
        SPRITES_TRAP[TRAP_FIRE].idle_inactive.push_back(SDL_Rect());
        SPRITES_TRAP[TRAP_FIRE].idle_inactive[i].x=80;
        SPRITES_TRAP[TRAP_FIRE].idle_inactive[i].y=0;
        SPRITES_TRAP[TRAP_FIRE].idle_inactive[i].w=TRAP_SPRITE_COUNTS[TRAP_FIRE].width;
        SPRITES_TRAP[TRAP_FIRE].idle_inactive[i].h=TRAP_SPRITE_COUNTS[TRAP_FIRE].height;
    }

    //Spikes
    for(short i=0;i<TRAP_SPRITE_COUNTS[TRAP_SPIKES].idle_active_sprites;i++){
        SPRITES_TRAP[TRAP_SPIKES].idle_active.push_back(SDL_Rect());
        SPRITES_TRAP[TRAP_SPIKES].idle_active[i].x=i*TRAP_SPRITE_COUNTS[TRAP_SPIKES].width;
        SPRITES_TRAP[TRAP_SPIKES].idle_active[i].y=0;
        SPRITES_TRAP[TRAP_SPIKES].idle_active[i].w=TRAP_SPRITE_COUNTS[TRAP_SPIKES].width;
        SPRITES_TRAP[TRAP_SPIKES].idle_active[i].h=TRAP_SPRITE_COUNTS[TRAP_SPIKES].height;
    }

    //Icicle
    for(short i=0;i<TRAP_SPRITE_COUNTS[TRAP_ICICLE].idle_active_sprites;i++){
        SPRITES_TRAP[TRAP_ICICLE].idle_active.push_back(SDL_Rect());
        SPRITES_TRAP[TRAP_ICICLE].idle_active[i].x=i*TRAP_SPRITE_COUNTS[TRAP_ICICLE].width;
        SPRITES_TRAP[TRAP_ICICLE].idle_active[i].y=0;
        SPRITES_TRAP[TRAP_ICICLE].idle_active[i].w=TRAP_SPRITE_COUNTS[TRAP_ICICLE].width;
        SPRITES_TRAP[TRAP_ICICLE].idle_active[i].h=TRAP_SPRITE_COUNTS[TRAP_ICICLE].height;
    }

    //Branch
    for(short i=0;i<TRAP_SPRITE_COUNTS[TRAP_BRANCH].move_sprites;i++){
        SPRITES_TRAP[TRAP_BRANCH].move.push_back(SDL_Rect());
        SPRITES_TRAP[TRAP_BRANCH].move[i].x=i*TRAP_SPRITE_COUNTS[TRAP_BRANCH].width;
        SPRITES_TRAP[TRAP_BRANCH].move[i].y=0;
        SPRITES_TRAP[TRAP_BRANCH].move[i].w=TRAP_SPRITE_COUNTS[TRAP_BRANCH].width;
        SPRITES_TRAP[TRAP_BRANCH].move[i].h=TRAP_SPRITE_COUNTS[TRAP_BRANCH].height;
    }
    for(short i=0;i<TRAP_SPRITE_COUNTS[TRAP_BRANCH].idle_inactive_sprites;i++){
        SPRITES_TRAP[TRAP_BRANCH].idle_inactive.push_back(SDL_Rect());
        SPRITES_TRAP[TRAP_BRANCH].idle_inactive[i].x=i*TRAP_SPRITE_COUNTS[TRAP_BRANCH].width;
        SPRITES_TRAP[TRAP_BRANCH].idle_inactive[i].y=0;
        SPRITES_TRAP[TRAP_BRANCH].idle_inactive[i].w=TRAP_SPRITE_COUNTS[TRAP_BRANCH].width;
        SPRITES_TRAP[TRAP_BRANCH].idle_inactive[i].h=TRAP_SPRITE_COUNTS[TRAP_BRANCH].height;
    }

    //Electric
    for(short i=0;i<TRAP_SPRITE_COUNTS[TRAP_ELECTRIC].idle_active_sprites;i++){
        SPRITES_TRAP[TRAP_ELECTRIC].idle_active.push_back(SDL_Rect());
        SPRITES_TRAP[TRAP_ELECTRIC].idle_active[i].x=i*TRAP_SPRITE_COUNTS[TRAP_ELECTRIC].width;
        SPRITES_TRAP[TRAP_ELECTRIC].idle_active[i].y=0;
        SPRITES_TRAP[TRAP_ELECTRIC].idle_active[i].w=TRAP_SPRITE_COUNTS[TRAP_ELECTRIC].width;
        SPRITES_TRAP[TRAP_ELECTRIC].idle_active[i].h=TRAP_SPRITE_COUNTS[TRAP_ELECTRIC].height;
    }

    //Stone slab
    for(short i=0;i<TRAP_SPRITE_COUNTS[TRAP_STONESLAB].move_sprites;i++){
        SPRITES_TRAP[TRAP_STONESLAB].move.push_back(SDL_Rect());
        SPRITES_TRAP[TRAP_STONESLAB].move[i].x=i*TRAP_SPRITE_COUNTS[TRAP_STONESLAB].width;
        SPRITES_TRAP[TRAP_STONESLAB].move[i].y=0;
        SPRITES_TRAP[TRAP_STONESLAB].move[i].w=TRAP_SPRITE_COUNTS[TRAP_STONESLAB].width;
        SPRITES_TRAP[TRAP_STONESLAB].move[i].h=TRAP_SPRITE_COUNTS[TRAP_STONESLAB].height;
    }
    for(short i=0;i<TRAP_SPRITE_COUNTS[TRAP_STONESLAB].idle_inactive_sprites;i++){
        SPRITES_TRAP[TRAP_STONESLAB].idle_inactive.push_back(SDL_Rect());
        SPRITES_TRAP[TRAP_STONESLAB].idle_inactive[i].x=i*TRAP_SPRITE_COUNTS[TRAP_STONESLAB].width;
        SPRITES_TRAP[TRAP_STONESLAB].idle_inactive[i].y=0;
        SPRITES_TRAP[TRAP_STONESLAB].idle_inactive[i].w=TRAP_SPRITE_COUNTS[TRAP_STONESLAB].width;
        SPRITES_TRAP[TRAP_STONESLAB].idle_inactive[i].h=TRAP_SPRITE_COUNTS[TRAP_STONESLAB].height;
    }

    //Buzz saw.
    for(short i=0;i<TRAP_SPRITE_COUNTS[TRAP_BUZZSAW].move_sprites;i++){
        SPRITES_TRAP[TRAP_BUZZSAW].move.push_back(SDL_Rect());
        SPRITES_TRAP[TRAP_BUZZSAW].move[i].x=i*TRAP_SPRITE_COUNTS[TRAP_BUZZSAW].width;
        SPRITES_TRAP[TRAP_BUZZSAW].move[i].y=0;
        SPRITES_TRAP[TRAP_BUZZSAW].move[i].w=TRAP_SPRITE_COUNTS[TRAP_BUZZSAW].width;
        SPRITES_TRAP[TRAP_BUZZSAW].move[i].h=TRAP_SPRITE_COUNTS[TRAP_BUZZSAW].height;
    }

    //Spikes Up
    for(short i=0;i<TRAP_SPRITE_COUNTS[TRAP_SPIKES_UP].idle_active_sprites;i++){
        SPRITES_TRAP[TRAP_SPIKES_UP].idle_active.push_back(SDL_Rect());
        SPRITES_TRAP[TRAP_SPIKES_UP].idle_active[i].x=i*TRAP_SPRITE_COUNTS[TRAP_SPIKES_UP].width;
        SPRITES_TRAP[TRAP_SPIKES_UP].idle_active[i].y=0;
        SPRITES_TRAP[TRAP_SPIKES_UP].idle_active[i].w=TRAP_SPRITE_COUNTS[TRAP_SPIKES_UP].width;
        SPRITES_TRAP[TRAP_SPIKES_UP].idle_active[i].h=TRAP_SPRITE_COUNTS[TRAP_SPIKES_UP].height;
    }

    //Spikes Down
    for(short i=0;i<TRAP_SPRITE_COUNTS[TRAP_SPIKES_DOWN].idle_active_sprites;i++){
        SPRITES_TRAP[TRAP_SPIKES_DOWN].idle_active.push_back(SDL_Rect());
        SPRITES_TRAP[TRAP_SPIKES_DOWN].idle_active[i].x=i*TRAP_SPRITE_COUNTS[TRAP_SPIKES_DOWN].width;
        SPRITES_TRAP[TRAP_SPIKES_DOWN].idle_active[i].y=0;
        SPRITES_TRAP[TRAP_SPIKES_DOWN].idle_active[i].w=TRAP_SPRITE_COUNTS[TRAP_SPIKES_DOWN].width;
        SPRITES_TRAP[TRAP_SPIKES_DOWN].idle_active[i].h=TRAP_SPRITE_COUNTS[TRAP_SPIKES_DOWN].height;
    }

    //Spikes Right
    for(short i=0;i<TRAP_SPRITE_COUNTS[TRAP_SPIKES_RIGHT].idle_active_sprites;i++){
        SPRITES_TRAP[TRAP_SPIKES_RIGHT].idle_active.push_back(SDL_Rect());
        SPRITES_TRAP[TRAP_SPIKES_RIGHT].idle_active[i].x=i*TRAP_SPRITE_COUNTS[TRAP_SPIKES_RIGHT].width;
        SPRITES_TRAP[TRAP_SPIKES_RIGHT].idle_active[i].y=0;
        SPRITES_TRAP[TRAP_SPIKES_RIGHT].idle_active[i].w=TRAP_SPRITE_COUNTS[TRAP_SPIKES_RIGHT].width;
        SPRITES_TRAP[TRAP_SPIKES_RIGHT].idle_active[i].h=TRAP_SPRITE_COUNTS[TRAP_SPIKES_RIGHT].height;
    }

    //Spikes Left
    for(short i=0;i<TRAP_SPRITE_COUNTS[TRAP_SPIKES_LEFT].idle_active_sprites;i++){
        SPRITES_TRAP[TRAP_SPIKES_LEFT].idle_active.push_back(SDL_Rect());
        SPRITES_TRAP[TRAP_SPIKES_LEFT].idle_active[i].x=i*TRAP_SPRITE_COUNTS[TRAP_SPIKES_LEFT].width;
        SPRITES_TRAP[TRAP_SPIKES_LEFT].idle_active[i].y=0;
        SPRITES_TRAP[TRAP_SPIKES_LEFT].idle_active[i].w=TRAP_SPRITE_COUNTS[TRAP_SPIKES_LEFT].width;
        SPRITES_TRAP[TRAP_SPIKES_LEFT].idle_active[i].h=TRAP_SPRITE_COUNTS[TRAP_SPIKES_LEFT].height;
    }

    //Boss Castle
    for(short i=0;i<TRAP_SPRITE_COUNTS[TRAP_BOSS_CASTLE].move_sprites;i++){
        SPRITES_TRAP[TRAP_BOSS_CASTLE].move.push_back(SDL_Rect());
        SPRITES_TRAP[TRAP_BOSS_CASTLE].move[i].x=i*TRAP_SPRITE_COUNTS[TRAP_BOSS_CASTLE].width;
        SPRITES_TRAP[TRAP_BOSS_CASTLE].move[i].y=0;
        SPRITES_TRAP[TRAP_BOSS_CASTLE].move[i].w=TRAP_SPRITE_COUNTS[TRAP_BOSS_CASTLE].width;
        SPRITES_TRAP[TRAP_BOSS_CASTLE].move[i].h=TRAP_SPRITE_COUNTS[TRAP_BOSS_CASTLE].height;
    }
    for(short i=0;i<TRAP_SPRITE_COUNTS[TRAP_BOSS_CASTLE].idle_active_sprites;i++){
        SPRITES_TRAP[TRAP_BOSS_CASTLE].idle_active.push_back(SDL_Rect());
        SPRITES_TRAP[TRAP_BOSS_CASTLE].idle_active[i].x=i*TRAP_SPRITE_COUNTS[TRAP_BOSS_CASTLE].width;
        SPRITES_TRAP[TRAP_BOSS_CASTLE].idle_active[i].y=0;
        SPRITES_TRAP[TRAP_BOSS_CASTLE].idle_active[i].w=TRAP_SPRITE_COUNTS[TRAP_BOSS_CASTLE].width;
        SPRITES_TRAP[TRAP_BOSS_CASTLE].idle_active[i].h=TRAP_SPRITE_COUNTS[TRAP_BOSS_CASTLE].height;
    }

    //Boss Pyramid
    for(short i=0;i<TRAP_SPRITE_COUNTS[TRAP_BOSS_PYRAMID].move_sprites;i++){
        SPRITES_TRAP[TRAP_BOSS_PYRAMID].move.push_back(SDL_Rect());
        SPRITES_TRAP[TRAP_BOSS_PYRAMID].move[i].x=i*TRAP_SPRITE_COUNTS[TRAP_BOSS_PYRAMID].width;
        SPRITES_TRAP[TRAP_BOSS_PYRAMID].move[i].y=0;
        SPRITES_TRAP[TRAP_BOSS_PYRAMID].move[i].w=TRAP_SPRITE_COUNTS[TRAP_BOSS_PYRAMID].width;
        SPRITES_TRAP[TRAP_BOSS_PYRAMID].move[i].h=TRAP_SPRITE_COUNTS[TRAP_BOSS_PYRAMID].height;
    }
    for(short i=0;i<TRAP_SPRITE_COUNTS[TRAP_BOSS_PYRAMID].idle_active_sprites;i++){
        SPRITES_TRAP[TRAP_BOSS_PYRAMID].idle_active.push_back(SDL_Rect());
        SPRITES_TRAP[TRAP_BOSS_PYRAMID].idle_active[i].x=i*TRAP_SPRITE_COUNTS[TRAP_BOSS_PYRAMID].width;
        SPRITES_TRAP[TRAP_BOSS_PYRAMID].idle_active[i].y=0;
        SPRITES_TRAP[TRAP_BOSS_PYRAMID].idle_active[i].w=TRAP_SPRITE_COUNTS[TRAP_BOSS_PYRAMID].width;
        SPRITES_TRAP[TRAP_BOSS_PYRAMID].idle_active[i].h=TRAP_SPRITE_COUNTS[TRAP_BOSS_PYRAMID].height;
    }

    //Seaweed
    for(short i=0;i<TRAP_SPRITE_COUNTS[TRAP_SEAWEED].idle_active_sprites;i++){
        SPRITES_TRAP[TRAP_SEAWEED].idle_active.push_back(SDL_Rect());
        SPRITES_TRAP[TRAP_SEAWEED].idle_active[i].x=i*TRAP_SPRITE_COUNTS[TRAP_SEAWEED].width;
        SPRITES_TRAP[TRAP_SEAWEED].idle_active[i].y=0;
        SPRITES_TRAP[TRAP_SEAWEED].idle_active[i].w=TRAP_SPRITE_COUNTS[TRAP_SEAWEED].width;
        SPRITES_TRAP[TRAP_SEAWEED].idle_active[i].h=TRAP_SPRITE_COUNTS[TRAP_SEAWEED].height;
    }
}

void create_sprite_sheets_player(){
    for(short i=0;i<PLAYER_WORLDMAP_IDLE_SPRITES;i++){
        sprites_player_worldmap_idle_right[i].x=i*PLAYER_WORLDMAP_W;
        sprites_player_worldmap_idle_right[i].y=0;
        sprites_player_worldmap_idle_right[i].w=PLAYER_WORLDMAP_W;
        sprites_player_worldmap_idle_right[i].h=PLAYER_WORLDMAP_H;
    }

    for(short i=0;i<PLAYER_WORLDMAP_IDLE_SPRITES;i++){
        sprites_player_worldmap_idle_left[i].x=i*PLAYER_WORLDMAP_W;
        sprites_player_worldmap_idle_left[i].y=PLAYER_WORLDMAP_H;
        sprites_player_worldmap_idle_left[i].w=PLAYER_WORLDMAP_W;
        sprites_player_worldmap_idle_left[i].h=PLAYER_WORLDMAP_H;
    }

    for(short i=0;i<PLAYER_WORLDMAP_IDLE_SPRITES;i++){
        sprites_player_worldmap_idle_up[i].x=i*PLAYER_WORLDMAP_W;
        sprites_player_worldmap_idle_up[i].y=PLAYER_WORLDMAP_H*2;
        sprites_player_worldmap_idle_up[i].w=PLAYER_WORLDMAP_W;
        sprites_player_worldmap_idle_up[i].h=PLAYER_WORLDMAP_H;
    }

    for(short i=0;i<PLAYER_WORLDMAP_IDLE_SPRITES;i++){
        sprites_player_worldmap_idle_down[i].x=i*PLAYER_WORLDMAP_W;
        sprites_player_worldmap_idle_down[i].y=PLAYER_WORLDMAP_H*3;
        sprites_player_worldmap_idle_down[i].w=PLAYER_WORLDMAP_W;
        sprites_player_worldmap_idle_down[i].h=PLAYER_WORLDMAP_H;
    }

    for(short i=0;i<PLAYER_WORLDMAP_WALK_SPRITES;i++){
        sprites_player_worldmap_walk_right[i].x=i*PLAYER_WORLDMAP_W;
        sprites_player_worldmap_walk_right[i].y=PLAYER_WORLDMAP_H*4;
        sprites_player_worldmap_walk_right[i].w=PLAYER_WORLDMAP_W;
        sprites_player_worldmap_walk_right[i].h=PLAYER_WORLDMAP_H;
    }

    for(short i=0;i<PLAYER_WORLDMAP_WALK_SPRITES;i++){
        sprites_player_worldmap_walk_left[i].x=i*PLAYER_WORLDMAP_W;
        sprites_player_worldmap_walk_left[i].y=PLAYER_WORLDMAP_H*5;
        sprites_player_worldmap_walk_left[i].w=PLAYER_WORLDMAP_W;
        sprites_player_worldmap_walk_left[i].h=PLAYER_WORLDMAP_H;
    }

    for(short i=0;i<PLAYER_WORLDMAP_WALK_SPRITES;i++){
        sprites_player_worldmap_walk_up[i].x=i*PLAYER_WORLDMAP_W;
        sprites_player_worldmap_walk_up[i].y=PLAYER_WORLDMAP_H*6;
        sprites_player_worldmap_walk_up[i].w=PLAYER_WORLDMAP_W;
        sprites_player_worldmap_walk_up[i].h=PLAYER_WORLDMAP_H;
    }

    for(short i=0;i<PLAYER_WORLDMAP_WALK_SPRITES;i++){
        sprites_player_worldmap_walk_down[i].x=i*PLAYER_WORLDMAP_W;
        sprites_player_worldmap_walk_down[i].y=PLAYER_WORLDMAP_H*7;
        sprites_player_worldmap_walk_down[i].w=PLAYER_WORLDMAP_W;
        sprites_player_worldmap_walk_down[i].h=PLAYER_WORLDMAP_H;
    }

    for(short i=0;i<PLAYER_IDLE_SPRITES;i++){
        sprites_player_idle_right[i].x=i*PLAYER_W;
        sprites_player_idle_right[i].y=0;
        sprites_player_idle_right[i].w=PLAYER_W;
        sprites_player_idle_right[i].h=PLAYER_H;
    }

    for(short i=0;i<PLAYER_IDLE_SPRITES;i++){
        sprites_player_idle_left[i].x=i*PLAYER_W;
        sprites_player_idle_left[i].y=PLAYER_H;
        sprites_player_idle_left[i].w=PLAYER_W;
        sprites_player_idle_left[i].h=PLAYER_H;
    }

    for(short i=0;i<PLAYER_WALK_SPRITES;i++){
        sprites_player_right[i].x=i*PLAYER_W;
        sprites_player_right[i].y=PLAYER_H*2;
        sprites_player_right[i].w=PLAYER_W;
        sprites_player_right[i].h=PLAYER_H;
    }

    for(short i=0;i<PLAYER_WALK_SPRITES;i++){
        sprites_player_left[i].x=i*PLAYER_W;
        sprites_player_left[i].y=PLAYER_H*3;
        sprites_player_left[i].w=PLAYER_W;
        sprites_player_left[i].h=PLAYER_H;
    }

    for(short i=0;i<PLAYER_JUMP_SPRITES;i++){
        sprites_player_jump_right[i].x=i*PLAYER_W;
        sprites_player_jump_right[i].y=PLAYER_H*4;
        sprites_player_jump_right[i].w=PLAYER_W;
        sprites_player_jump_right[i].h=PLAYER_H;
    }

    for(short i=0;i<PLAYER_JUMP_SPRITES;i++){
        sprites_player_jump_left[i].x=i*PLAYER_W;
        sprites_player_jump_left[i].y=PLAYER_H*5;
        sprites_player_jump_left[i].w=PLAYER_W;
        sprites_player_jump_left[i].h=PLAYER_H;
    }

    for(short i=0;i<PLAYER_SWIM_SPRITES;i++){
        sprites_player_swim_up_right[i].x=i*PLAYER_W;
        sprites_player_swim_up_right[i].y=PLAYER_H*6;
        sprites_player_swim_up_right[i].w=PLAYER_W;
        sprites_player_swim_up_right[i].h=PLAYER_H;
    }

    for(short i=0;i<PLAYER_SWIM_SPRITES;i++){
        sprites_player_swim_up_left[i].x=i*PLAYER_W;
        sprites_player_swim_up_left[i].y=PLAYER_H*7;
        sprites_player_swim_up_left[i].w=PLAYER_W;
        sprites_player_swim_up_left[i].h=PLAYER_H;
    }

    for(short i=0;i<PLAYER_SWIM_SPRITES;i++){
        sprites_player_swim_right[i].x=i*PLAYER_W;
        sprites_player_swim_right[i].y=PLAYER_H*8;
        sprites_player_swim_right[i].w=PLAYER_W;
        sprites_player_swim_right[i].h=PLAYER_H;
    }

    for(short i=0;i<PLAYER_SWIM_SPRITES;i++){
        sprites_player_swim_left[i].x=i*PLAYER_W;
        sprites_player_swim_left[i].y=PLAYER_H*9;
        sprites_player_swim_left[i].w=PLAYER_W;
        sprites_player_swim_left[i].h=PLAYER_H;
    }

    sprites_player_shoot_right[0].x=0;
    sprites_player_shoot_right[0].y=PLAYER_H*10;
    sprites_player_shoot_right[0].w=PLAYER_SHOOT_W;
    sprites_player_shoot_right[0].h=PLAYER_H;

    sprites_player_shoot_left[0].x=PLAYER_SHOOT_W;
    sprites_player_shoot_left[0].y=PLAYER_H*10;
    sprites_player_shoot_left[0].w=PLAYER_SHOOT_W;
    sprites_player_shoot_left[0].h=PLAYER_H;

    sprites_player_shoot_up_right[0].x=PLAYER_SHOOT_W*2;
    sprites_player_shoot_up_right[0].y=PLAYER_H*10;
    sprites_player_shoot_up_right[0].w=PLAYER_SHOOT_W;
    sprites_player_shoot_up_right[0].h=PLAYER_H;

    sprites_player_shoot_up_left[0].x=PLAYER_SHOOT_W*3;
    sprites_player_shoot_up_left[0].y=PLAYER_H*10;
    sprites_player_shoot_up_left[0].w=PLAYER_SHOOT_W;
    sprites_player_shoot_up_left[0].h=PLAYER_H;

    sprites_player_shoot_up_right_air[0].x=PLAYER_SHOOT_W*4;
    sprites_player_shoot_up_right_air[0].y=PLAYER_H*10;
    sprites_player_shoot_up_right_air[0].w=PLAYER_SHOOT_W;
    sprites_player_shoot_up_right_air[0].h=PLAYER_H;

    sprites_player_shoot_up_left_air[0].x=PLAYER_SHOOT_W*5;
    sprites_player_shoot_up_left_air[0].y=PLAYER_H*10;
    sprites_player_shoot_up_left_air[0].w=PLAYER_SHOOT_W;
    sprites_player_shoot_up_left_air[0].h=PLAYER_H;

    sprites_player_shoot_down_right_air[0].x=PLAYER_SHOOT_W*6;
    sprites_player_shoot_down_right_air[0].y=PLAYER_H*10;
    sprites_player_shoot_down_right_air[0].w=PLAYER_SHOOT_W;
    sprites_player_shoot_down_right_air[0].h=PLAYER_H;

    sprites_player_shoot_down_left_air[0].x=PLAYER_SHOOT_W*7;
    sprites_player_shoot_down_left_air[0].y=PLAYER_H*10;
    sprites_player_shoot_down_left_air[0].w=PLAYER_SHOOT_W;
    sprites_player_shoot_down_left_air[0].h=PLAYER_H;

    sprites_player_shoot_right_air[0].x=PLAYER_SHOOT_W*8;
    sprites_player_shoot_right_air[0].y=PLAYER_H*10;
    sprites_player_shoot_right_air[0].w=PLAYER_SHOOT_W;
    sprites_player_shoot_right_air[0].h=PLAYER_H;

    sprites_player_shoot_left_air[0].x=PLAYER_SHOOT_W*9;
    sprites_player_shoot_left_air[0].y=PLAYER_H*10;
    sprites_player_shoot_left_air[0].w=PLAYER_SHOOT_W;
    sprites_player_shoot_left_air[0].h=PLAYER_H;

    sprites_player_shoot_climb_right[0].x=PLAYER_SHOOT_W*10;
    sprites_player_shoot_climb_right[0].y=PLAYER_H*10;
    sprites_player_shoot_climb_right[0].w=PLAYER_SHOOT_W;
    sprites_player_shoot_climb_right[0].h=PLAYER_H;

    sprites_player_shoot_climb_left[0].x=PLAYER_SHOOT_W*11;
    sprites_player_shoot_climb_left[0].y=PLAYER_H*10;
    sprites_player_shoot_climb_left[0].w=PLAYER_SHOOT_W;
    sprites_player_shoot_climb_left[0].h=PLAYER_H;

    sprites_player_shoot_climb_up_right[0].x=PLAYER_SHOOT_W*12;
    sprites_player_shoot_climb_up_right[0].y=PLAYER_H*10;
    sprites_player_shoot_climb_up_right[0].w=PLAYER_SHOOT_W;
    sprites_player_shoot_climb_up_right[0].h=PLAYER_H;

    sprites_player_shoot_climb_up_left[0].x=PLAYER_SHOOT_W*13;
    sprites_player_shoot_climb_up_left[0].y=PLAYER_H*10;
    sprites_player_shoot_climb_up_left[0].w=PLAYER_SHOOT_W;
    sprites_player_shoot_climb_up_left[0].h=PLAYER_H;

    sprites_player_shoot_climb_down_right[0].x=PLAYER_SHOOT_W*14;
    sprites_player_shoot_climb_down_right[0].y=PLAYER_H*10;
    sprites_player_shoot_climb_down_right[0].w=PLAYER_SHOOT_W;
    sprites_player_shoot_climb_down_right[0].h=PLAYER_H;

    sprites_player_shoot_climb_down_left[0].x=PLAYER_SHOOT_W*15;
    sprites_player_shoot_climb_down_left[0].y=PLAYER_H*10;
    sprites_player_shoot_climb_down_left[0].w=PLAYER_SHOOT_W;
    sprites_player_shoot_climb_down_left[0].h=PLAYER_H;

    for(short i=0;i<PLAYER_CLIMB_SPRITES;i++){
        sprites_player_climb_up_right[i].x=i*PLAYER_W;
        sprites_player_climb_up_right[i].y=PLAYER_H*11;
        sprites_player_climb_up_right[i].w=PLAYER_W;
        sprites_player_climb_up_right[i].h=PLAYER_H;
    }

    for(short i=0;i<PLAYER_CLIMB_SPRITES;i++){
        sprites_player_climb_up_left[i].x=i*PLAYER_W;
        sprites_player_climb_up_left[i].y=PLAYER_H*12;
        sprites_player_climb_up_left[i].w=PLAYER_W;
        sprites_player_climb_up_left[i].h=PLAYER_H;
    }

    for(short i=0;i<PLAYER_CLIMB_SPRITES;i++){
        sprites_player_climb_down_right[i].x=i*PLAYER_W;
        sprites_player_climb_down_right[i].y=PLAYER_H*13;
        sprites_player_climb_down_right[i].w=PLAYER_W;
        sprites_player_climb_down_right[i].h=PLAYER_H;
    }

    for(short i=0;i<PLAYER_CLIMB_SPRITES;i++){
        sprites_player_climb_down_left[i].x=i*PLAYER_W;
        sprites_player_climb_down_left[i].y=PLAYER_H*14;
        sprites_player_climb_down_left[i].w=PLAYER_W;
        sprites_player_climb_down_left[i].h=PLAYER_H;
    }

    for(short i=0;i<PLAYER_LOOK_SPRITES;i++){
        sprites_player_look_right[i].x=i*PLAYER_W;
        sprites_player_look_right[i].y=PLAYER_H*15;
        sprites_player_look_right[i].w=PLAYER_W;
        sprites_player_look_right[i].h=PLAYER_H;
    }

    for(short i=0;i<PLAYER_LOOK_SPRITES;i++){
        sprites_player_look_right_up[i].x=26+i*PLAYER_W;
        sprites_player_look_right_up[i].y=PLAYER_H*15;
        sprites_player_look_right_up[i].w=PLAYER_W;
        sprites_player_look_right_up[i].h=PLAYER_H;
    }

    for(short i=0;i<PLAYER_LOOK_SPRITES;i++){
        sprites_player_look_right_down[i].x=52+i*PLAYER_W;
        sprites_player_look_right_down[i].y=PLAYER_H*15;
        sprites_player_look_right_down[i].w=PLAYER_W;
        sprites_player_look_right_down[i].h=PLAYER_H;
    }

    for(short i=0;i<PLAYER_LOOK_SPRITES;i++){
        sprites_player_look_left[i].x=i*PLAYER_W;
        sprites_player_look_left[i].y=PLAYER_H*16;
        sprites_player_look_left[i].w=PLAYER_W;
        sprites_player_look_left[i].h=PLAYER_H;
    }

    for(short i=0;i<PLAYER_LOOK_SPRITES;i++){
        sprites_player_look_left_up[i].x=26+i*PLAYER_W;
        sprites_player_look_left_up[i].y=PLAYER_H*16;
        sprites_player_look_left_up[i].w=PLAYER_W;
        sprites_player_look_left_up[i].h=PLAYER_H;
    }

    for(short i=0;i<PLAYER_LOOK_SPRITES;i++){
        sprites_player_look_left_down[i].x=52+i*PLAYER_W;
        sprites_player_look_left_down[i].y=PLAYER_H*16;
        sprites_player_look_left_down[i].w=PLAYER_W;
        sprites_player_look_left_down[i].h=PLAYER_H;
    }

    for(short i=0;i<PLAYER_LOOK_SPRITES;i++){
        sprites_player_look_right_climb_up[i].x=78+i*PLAYER_W;
        sprites_player_look_right_climb_up[i].y=PLAYER_H*15;
        sprites_player_look_right_climb_up[i].w=PLAYER_W;
        sprites_player_look_right_climb_up[i].h=PLAYER_H;
    }

    for(short i=0;i<PLAYER_LOOK_SPRITES;i++){
        sprites_player_look_right_climb_down[i].x=104+i*PLAYER_W;
        sprites_player_look_right_climb_down[i].y=PLAYER_H*15;
        sprites_player_look_right_climb_down[i].w=PLAYER_W;
        sprites_player_look_right_climb_down[i].h=PLAYER_H;
    }

    for(short i=0;i<PLAYER_LOOK_SPRITES;i++){
        sprites_player_look_left_climb_up[i].x=78+i*PLAYER_W;
        sprites_player_look_left_climb_up[i].y=PLAYER_H*16;
        sprites_player_look_left_climb_up[i].w=PLAYER_W;
        sprites_player_look_left_climb_up[i].h=PLAYER_H;
    }

    for(short i=0;i<PLAYER_LOOK_SPRITES;i++){
        sprites_player_look_left_climb_down[i].x=104+i*PLAYER_W;
        sprites_player_look_left_climb_down[i].y=PLAYER_H*16;
        sprites_player_look_left_climb_down[i].w=PLAYER_W;
        sprites_player_look_left_climb_down[i].h=PLAYER_H;
    }

    for(short i=0;i<PLAYER_DEATH_SPRITES;i++){
        sprites_player_death[i].x=i*36;
        sprites_player_death[i].y=PLAYER_H*17;
        sprites_player_death[i].w=36;
        sprites_player_death[i].h=PLAYER_H;
    }

    for(short i=0;i<PLAYER_CROUCH_SPRITES;i++){
        sprites_player_crouch_right[i].x=i*PLAYER_W;
        sprites_player_crouch_right[i].y=PLAYER_H*18;
        sprites_player_crouch_right[i].w=PLAYER_W;
        sprites_player_crouch_right[i].h=23;
    }

    for(short i=0;i<PLAYER_CROUCH_SPRITES;i++){
        sprites_player_crouch_left[i].x=i*PLAYER_W;
        sprites_player_crouch_left[i].y=PLAYER_H*18+23;
        sprites_player_crouch_left[i].w=PLAYER_W;
        sprites_player_crouch_left[i].h=23;
    }
}

void create_sprite_sheets_tiles(){
    //Water tile sprite sheet.
    for(short i=0;i<WATER_SPRITES;i++){
        sprites_water_tiles[i].x=TILE_SIZE+TILE_SIZE*i;
        sprites_water_tiles[i].y=0;
        sprites_water_tiles[i].w=TILE_SIZE;
        sprites_water_tiles[i].h=TILE_SIZE;
    }

    //Terrain tiles:

    //Water.
    sprites_tiles[1].x=TILE_SIZE;
    sprites_tiles[1].y=0;
    sprites_tiles[1].w=TILE_SIZE;
    sprites_tiles[1].h=TILE_SIZE;

    sprites_tiles[2].x=0;
    sprites_tiles[2].y=0;
    sprites_tiles[2].w=TILE_SIZE;
    sprites_tiles[2].h=TILE_SIZE;

    //Ground.
    for(short i=3,n=0;i<=5;i++,n++){
        sprites_tiles[i].x=n*TILE_SIZE;
        sprites_tiles[i].y=0;
        sprites_tiles[i].w=TILE_SIZE;
        sprites_tiles[i].h=TILE_SIZE;
    }
    for(short i=6,n=0;i<=8;i++,n++){
        sprites_tiles[i].x=n*TILE_SIZE;
        sprites_tiles[i].y=TILE_SIZE;
        sprites_tiles[i].w=TILE_SIZE;
        sprites_tiles[i].h=TILE_SIZE;
    }
    for(short i=9,n=0;i<=11;i++,n++){
        sprites_tiles[i].x=n*TILE_SIZE;
        sprites_tiles[i].y=TILE_SIZE*2;
        sprites_tiles[i].w=TILE_SIZE;
        sprites_tiles[i].h=TILE_SIZE;
    }
    for(short i=12,n=0;i<=14;i++,n++){
        sprites_tiles[i].x=n*TILE_SIZE;
        sprites_tiles[i].y=TILE_SIZE*3;
        sprites_tiles[i].w=TILE_SIZE;
        sprites_tiles[i].h=TILE_SIZE;
    }

    //Slopes.
    for(short i=15,n=0;i<=16;i++,n++){
        sprites_tiles[i].x=3*TILE_SIZE+n*TILE_SIZE;
        sprites_tiles[i].y=0;
        sprites_tiles[i].w=TILE_SIZE;
        sprites_tiles[i].h=TILE_SIZE;
    }
    for(short i=17,n=0;i<=18;i++,n++){
        sprites_tiles[i].x=3*TILE_SIZE+n*TILE_SIZE;
        sprites_tiles[i].y=TILE_SIZE;
        sprites_tiles[i].w=TILE_SIZE;
        sprites_tiles[i].h=TILE_SIZE;
    }

    //Climbable.
    for(short i=19,n=0;i<=22;i++,n++){
        sprites_tiles[i].x=3*TILE_SIZE+n*TILE_SIZE;
        sprites_tiles[i].y=TILE_SIZE*2;
        sprites_tiles[i].w=TILE_SIZE;
        sprites_tiles[i].h=TILE_SIZE;
    }

    //Other.
    for(short i=23,n=0;i<=29;i++,n++){
        sprites_tiles[i].x=n*TILE_SIZE;
        sprites_tiles[i].y=TILE_SIZE*4;
        sprites_tiles[i].w=TILE_SIZE;
        sprites_tiles[i].h=TILE_SIZE;
    }
    for(short i=30,n=0;i<=37;i++,n++){
        sprites_tiles[i].x=n*TILE_SIZE;
        sprites_tiles[i].y=TILE_SIZE*5;
        sprites_tiles[i].w=TILE_SIZE;
        sprites_tiles[i].h=TILE_SIZE;
    }
    for(short i=38,n=0;i<=45;i++,n++){
        sprites_tiles[i].x=n*TILE_SIZE;
        sprites_tiles[i].y=TILE_SIZE*6;
        sprites_tiles[i].w=TILE_SIZE;
        sprites_tiles[i].h=TILE_SIZE;
    }
    for(short i=46,n=0;i<=53;i++,n++){
        sprites_tiles[i].x=n*TILE_SIZE;
        sprites_tiles[i].y=TILE_SIZE*7;
        sprites_tiles[i].w=TILE_SIZE;
        sprites_tiles[i].h=TILE_SIZE;
    }
    for(short i=54,n=0;i<=61;i++,n++){
        sprites_tiles[i].x=n*TILE_SIZE;
        sprites_tiles[i].y=TILE_SIZE*8;
        sprites_tiles[i].w=TILE_SIZE;
        sprites_tiles[i].h=TILE_SIZE;
    }

    //Clutter tiles.
    for(short i=62,n=0;i<=66;i++,n++){
        sprites_tiles[i].x=TILE_SIZE*7;
        sprites_tiles[i].y=TILE_SIZE*n;
        sprites_tiles[i].w=TILE_SIZE;
        sprites_tiles[i].h=TILE_SIZE;
    }

    //Other ground edges.
    for(short i=67,n=0;i<=68;i++,n++){
        sprites_tiles[i].x=TILE_SIZE*5+TILE_SIZE*n;
        sprites_tiles[i].y=0;
        sprites_tiles[i].w=TILE_SIZE;
        sprites_tiles[i].h=TILE_SIZE;
    }
    for(short i=69,n=0;i<=70;i++,n++){
        sprites_tiles[i].x=TILE_SIZE*5+TILE_SIZE*n;
        sprites_tiles[i].y=TILE_SIZE;
        sprites_tiles[i].w=TILE_SIZE;
        sprites_tiles[i].h=TILE_SIZE;
    }
    for(short i=71,n=0;i<=73;i++,n++){
        sprites_tiles[i].x=TILE_SIZE*3+TILE_SIZE*n;
        sprites_tiles[i].y=TILE_SIZE*3;
        sprites_tiles[i].w=TILE_SIZE;
        sprites_tiles[i].h=TILE_SIZE;
    }

    //Other
    for(short i=74,n=0;i<=84;i++,n++){
        sprites_tiles[i].x=TILE_SIZE*8+TILE_SIZE*n;
        sprites_tiles[i].y=0;
        sprites_tiles[i].w=TILE_SIZE;
        sprites_tiles[i].h=TILE_SIZE;
    }
    for(short i=85,n=0;i<=95;i++,n++){
        sprites_tiles[i].x=TILE_SIZE*8+TILE_SIZE*n;
        sprites_tiles[i].y=TILE_SIZE;
        sprites_tiles[i].w=TILE_SIZE;
        sprites_tiles[i].h=TILE_SIZE;
    }
    for(short i=96,n=0;i<=106;i++,n++){
        sprites_tiles[i].x=TILE_SIZE*8+TILE_SIZE*n;
        sprites_tiles[i].y=TILE_SIZE*2;
        sprites_tiles[i].w=TILE_SIZE;
        sprites_tiles[i].h=TILE_SIZE;
    }
    for(short i=107,n=0;i<=117;i++,n++){
        sprites_tiles[i].x=TILE_SIZE*8+TILE_SIZE*n;
        sprites_tiles[i].y=TILE_SIZE*3;
        sprites_tiles[i].w=TILE_SIZE;
        sprites_tiles[i].h=TILE_SIZE;
    }
    for(short i=118,n=0;i<=128;i++,n++){
        sprites_tiles[i].x=TILE_SIZE*8+TILE_SIZE*n;
        sprites_tiles[i].y=TILE_SIZE*4;
        sprites_tiles[i].w=TILE_SIZE;
        sprites_tiles[i].h=TILE_SIZE;
    }
    for(short i=129,n=0;i<=139;i++,n++){
        sprites_tiles[i].x=TILE_SIZE*8+TILE_SIZE*n;
        sprites_tiles[i].y=TILE_SIZE*5;
        sprites_tiles[i].w=TILE_SIZE;
        sprites_tiles[i].h=TILE_SIZE;
    }
    for(short i=140,n=0;i<=150;i++,n++){
        sprites_tiles[i].x=TILE_SIZE*8+TILE_SIZE*n;
        sprites_tiles[i].y=TILE_SIZE*6;
        sprites_tiles[i].w=TILE_SIZE;
        sprites_tiles[i].h=TILE_SIZE;
    }
    for(short i=151,n=0;i<=161;i++,n++){
        sprites_tiles[i].x=TILE_SIZE*8+TILE_SIZE*n;
        sprites_tiles[i].y=TILE_SIZE*7;
        sprites_tiles[i].w=TILE_SIZE;
        sprites_tiles[i].h=TILE_SIZE;
    }
    for(short i=162,n=0;i<=172;i++,n++){
        sprites_tiles[i].x=TILE_SIZE*8+TILE_SIZE*n;
        sprites_tiles[i].y=TILE_SIZE*8;
        sprites_tiles[i].w=TILE_SIZE;
        sprites_tiles[i].h=TILE_SIZE;
    }

    //Background tiles.
    for(int column=0;column<8;column++){
        for(int i=1+9*column,n=0;i<10+9*column;i++,n++){
            sprites_tiles_background[i].x=TILE_SIZE*column;
            sprites_tiles_background[i].y=n*TILE_SIZE;
            sprites_tiles_background[i].w=TILE_SIZE;
            sprites_tiles_background[i].h=TILE_SIZE;
        }
    }

    //World map tiles:

    //Water.
    worldmap_sprites_tiles[1].x=TILE_SIZE;
    worldmap_sprites_tiles[1].y=0;
    worldmap_sprites_tiles[1].w=TILE_SIZE;
    worldmap_sprites_tiles[1].h=TILE_SIZE;

    worldmap_sprites_tiles[2].x=0;
    worldmap_sprites_tiles[2].y=0;
    worldmap_sprites_tiles[2].w=TILE_SIZE;
    worldmap_sprites_tiles[2].h=TILE_SIZE;

    for(int i=3;i<=11;i++){
        worldmap_sprites_tiles[i].x=0;
        worldmap_sprites_tiles[i].y=(i-3)*TILE_SIZE;
        worldmap_sprites_tiles[i].w=TILE_SIZE;
        worldmap_sprites_tiles[i].h=TILE_SIZE;
    }

    for(int i=12,n=0;i<=20;i++,n++){
        worldmap_sprites_tiles[i].x=TILE_SIZE;
        worldmap_sprites_tiles[i].y=n*TILE_SIZE;
        worldmap_sprites_tiles[i].w=TILE_SIZE;
        worldmap_sprites_tiles[i].h=TILE_SIZE;
    }

    for(int i=21,n=0;i<=29;i++,n++){
        worldmap_sprites_tiles[i].x=TILE_SIZE*2;
        worldmap_sprites_tiles[i].y=n*TILE_SIZE;
        worldmap_sprites_tiles[i].w=TILE_SIZE;
        worldmap_sprites_tiles[i].h=TILE_SIZE;
    }

    for(int i=30,n=0;i<=38;i++,n++){
        worldmap_sprites_tiles[i].x=TILE_SIZE*3;
        worldmap_sprites_tiles[i].y=n*TILE_SIZE;
        worldmap_sprites_tiles[i].w=TILE_SIZE;
        worldmap_sprites_tiles[i].h=TILE_SIZE;
    }

    for(int i=39,n=0;i<=47;i++,n++){
        worldmap_sprites_tiles[i].x=TILE_SIZE*4;
        worldmap_sprites_tiles[i].y=n*TILE_SIZE;
        worldmap_sprites_tiles[i].w=TILE_SIZE;
        worldmap_sprites_tiles[i].h=TILE_SIZE;
    }

    for(int i=48,n=0;i<=56;i++,n++){
        worldmap_sprites_tiles[i].x=TILE_SIZE*5;
        worldmap_sprites_tiles[i].y=n*TILE_SIZE;
        worldmap_sprites_tiles[i].w=TILE_SIZE;
        worldmap_sprites_tiles[i].h=TILE_SIZE;
    }

    for(int i=57,n=0;i<=65;i++,n++){
        worldmap_sprites_tiles[i].x=TILE_SIZE*6;
        worldmap_sprites_tiles[i].y=n*TILE_SIZE;
        worldmap_sprites_tiles[i].w=TILE_SIZE;
        worldmap_sprites_tiles[i].h=TILE_SIZE;
    }

    for(int i=66,n=0;i<=74;i++,n++){
        worldmap_sprites_tiles[i].x=TILE_SIZE*7;
        worldmap_sprites_tiles[i].y=n*TILE_SIZE;
        worldmap_sprites_tiles[i].w=TILE_SIZE;
        worldmap_sprites_tiles[i].h=TILE_SIZE;
    }

    for(int i=75,n=0;i<=83;i++,n++){
        worldmap_sprites_tiles[i].x=TILE_SIZE*8;
        worldmap_sprites_tiles[i].y=n*TILE_SIZE;
        worldmap_sprites_tiles[i].w=TILE_SIZE;
        worldmap_sprites_tiles[i].h=TILE_SIZE;
    }

    for(int i=84,n=0;i<=92;i++,n++){
        worldmap_sprites_tiles[i].x=TILE_SIZE*9;
        worldmap_sprites_tiles[i].y=n*TILE_SIZE;
        worldmap_sprites_tiles[i].w=TILE_SIZE;
        worldmap_sprites_tiles[i].h=TILE_SIZE;
    }

    for(int i=93,n=0;i<=101;i++,n++){
        worldmap_sprites_tiles[i].x=TILE_SIZE*10;
        worldmap_sprites_tiles[i].y=n*TILE_SIZE;
        worldmap_sprites_tiles[i].w=TILE_SIZE;
        worldmap_sprites_tiles[i].h=TILE_SIZE;
    }

    for(int i=102,n=0;i<=110;i++,n++){
        worldmap_sprites_tiles[i].x=TILE_SIZE*11;
        worldmap_sprites_tiles[i].y=n*TILE_SIZE;
        worldmap_sprites_tiles[i].w=TILE_SIZE;
        worldmap_sprites_tiles[i].h=TILE_SIZE;
    }

    for(int i=111,n=0;i<=119;i++,n++){
        worldmap_sprites_tiles[i].x=TILE_SIZE*12;
        worldmap_sprites_tiles[i].y=n*TILE_SIZE;
        worldmap_sprites_tiles[i].w=TILE_SIZE;
        worldmap_sprites_tiles[i].h=TILE_SIZE;
    }

    for(int i=120,n=0;i<=128;i++,n++){
        worldmap_sprites_tiles[i].x=TILE_SIZE*13;
        worldmap_sprites_tiles[i].y=n*TILE_SIZE;
        worldmap_sprites_tiles[i].w=TILE_SIZE;
        worldmap_sprites_tiles[i].h=TILE_SIZE;
    }

    for(int i=129,n=0;i<=137;i++,n++){
        worldmap_sprites_tiles[i].x=TILE_SIZE*14;
        worldmap_sprites_tiles[i].y=n*TILE_SIZE;
        worldmap_sprites_tiles[i].w=TILE_SIZE;
        worldmap_sprites_tiles[i].h=TILE_SIZE;
    }

    for(int i=138,n=0;i<=146;i++,n++){
        worldmap_sprites_tiles[i].x=TILE_SIZE*15;
        worldmap_sprites_tiles[i].y=n*TILE_SIZE;
        worldmap_sprites_tiles[i].w=TILE_SIZE;
        worldmap_sprites_tiles[i].h=TILE_SIZE;
    }

    for(int i=147,n=0;i<=155;i++,n++){
        worldmap_sprites_tiles[i].x=TILE_SIZE*16;
        worldmap_sprites_tiles[i].y=n*TILE_SIZE;
        worldmap_sprites_tiles[i].w=TILE_SIZE;
        worldmap_sprites_tiles[i].h=TILE_SIZE;
    }

    for(int i=156,n=0;i<=164;i++,n++){
        worldmap_sprites_tiles[i].x=TILE_SIZE*17;
        worldmap_sprites_tiles[i].y=n*TILE_SIZE;
        worldmap_sprites_tiles[i].w=TILE_SIZE;
        worldmap_sprites_tiles[i].h=TILE_SIZE;
    }

    for(int i=165,n=0;i<=173;i++,n++){
        worldmap_sprites_tiles[i].x=TILE_SIZE*18;
        worldmap_sprites_tiles[i].y=n*TILE_SIZE;
        worldmap_sprites_tiles[i].w=TILE_SIZE;
        worldmap_sprites_tiles[i].h=TILE_SIZE;
    }
}

void create_sprite_sheets_items(){
    //Here we set the items' sprite sheet arrays.

    for(short i=0;i<ITEM_SPRITES_LEAF;i++){
        sprites_item_leaf[i].x=i*ITEM_W;
        sprites_item_leaf[i].y=0;
        sprites_item_leaf[i].w=ITEM_W;
        sprites_item_leaf[i].h=ITEM_H;
    }

    for(short i=0;i<ITEM_SPRITES_CHEESE;i++){
        sprites_item_cheese[i].x=i*64;
        sprites_item_cheese[i].y=ITEM_H;
        sprites_item_cheese[i].w=64;
        sprites_item_cheese[i].h=64;
    }

    for(short i=0;i<ITEM_SPRITES_AMMO;i++){
        sprites_item_ammo[i].x=i*32;
        sprites_item_ammo[i].y=80;
        sprites_item_ammo[i].w=32;
        sprites_item_ammo[i].h=32;
    }

    for(short i=0;i<ITEM_SPRITES_SPAWNPOINT;i++){
        sprites_item_spawnpoint[i].x=0;
        sprites_item_spawnpoint[i].y=0;
        sprites_item_spawnpoint[i].w=32;
        sprites_item_spawnpoint[i].h=64;
    }

    for(short i=0;i<ITEM_SPRITES_CHECKPOINT;i++){
        sprites_item_checkpoint[i].x=i*32;
        sprites_item_checkpoint[i].y=96;
        sprites_item_checkpoint[i].w=32;
        sprites_item_checkpoint[i].h=96;
    }

    for(short i=0;i<ITEM_SPRITES_ENDPOINT;i++){
        sprites_item_endpoint[i].x=64;
        sprites_item_endpoint[i].y=0;
        sprites_item_endpoint[i].w=32;
        sprites_item_endpoint[i].h=96;
    }

    for(short i=0;i<ITEM_SPRITES_ENDPOINT_SECRET;i++){
        sprites_item_endpoint_secret[i].x=96;
        sprites_item_endpoint_secret[i].y=0;
        sprites_item_endpoint_secret[i].w=32;
        sprites_item_endpoint_secret[i].h=96;
    }

    for(short i=0;i<ITEM_SPRITES_SWIMMING_GEAR;i++){
        sprites_item_swimming_gear[i].x=i*32;
        sprites_item_swimming_gear[i].y=80+32;
        sprites_item_swimming_gear[i].w=32;
        sprites_item_swimming_gear[i].h=32;
    }

    for(short i=0;i<ITEM_SPRITES_KEY_RED;i++){
        sprites_item_key_red[i].x=i*32;
        sprites_item_key_red[i].y=80+32*2;
        sprites_item_key_red[i].w=32;
        sprites_item_key_red[i].h=32;
    }

    for(short i=0;i<ITEM_SPRITES_KEY_BLUE;i++){
        sprites_item_key_blue[i].x=i*32;
        sprites_item_key_blue[i].y=80+32*3;
        sprites_item_key_blue[i].w=32;
        sprites_item_key_blue[i].h=32;
    }

    for(short i=0;i<ITEM_SPRITES_KEY_GREEN;i++){
        sprites_item_key_green[i].x=i*32;
        sprites_item_key_green[i].y=80+32*4;
        sprites_item_key_green[i].w=32;
        sprites_item_key_green[i].h=32;
    }

    for(short i=0;i<ITEM_SPRITES_KEY_YELLOW;i++){
        sprites_item_key_yellow[i].x=i*32;
        sprites_item_key_yellow[i].y=80+32*5;
        sprites_item_key_yellow[i].w=32;
        sprites_item_key_yellow[i].h=32;
    }

    for(short i=0;i<ITEM_SPRITES_KEY_ORANGE;i++){
        sprites_item_key_orange[i].x=i*32;
        sprites_item_key_orange[i].y=80+32*6;
        sprites_item_key_orange[i].w=32;
        sprites_item_key_orange[i].h=32;
    }

    for(short i=0;i<ITEM_SPRITES_KEY_PURPLE;i++){
        sprites_item_key_purple[i].x=i*32;
        sprites_item_key_purple[i].y=80+32*7;
        sprites_item_key_purple[i].w=32;
        sprites_item_key_purple[i].h=32;
    }

    for(short i=0;i<ITEM_SPRITES_TOWEL;i++){
        sprites_item_towel[i].x=i*32;
        sprites_item_towel[i].y=80+32*8;
        sprites_item_towel[i].w=32;
        sprites_item_towel[i].h=32;
    }

    for(short i=0;i<ITEM_SPRITES_KEY_GRAY;i++){
        sprites_item_key_gray[i].x=i*32;
        sprites_item_key_gray[i].y=80+32*11+48+16;
        sprites_item_key_gray[i].w=32;
        sprites_item_key_gray[i].h=32;
    }

    for(short i=0;i<ITEM_SPRITES_KEY_BROWN;i++){
        sprites_item_key_brown[i].x=i*32;
        sprites_item_key_brown[i].y=80+32*11+48+16+32;
        sprites_item_key_brown[i].w=32;
        sprites_item_key_brown[i].h=32;
    }

    for(short i=0;i<ITEM_SPRITES_KEY_BLACK;i++){
        sprites_item_key_black[i].x=i*32;
        sprites_item_key_black[i].y=80+32*11+48+16+32*2;
        sprites_item_key_black[i].w=32;
        sprites_item_key_black[i].h=32;
    }

    for(short i=0;i<ITEM_SPRITES_KEY_PINK;i++){
        sprites_item_key_pink[i].x=i*32;
        sprites_item_key_pink[i].y=80+32*11+48+16+32*3;
        sprites_item_key_pink[i].w=32;
        sprites_item_key_pink[i].h=32;
    }

    for(short i=0;i<ITEM_SPRITES_KEY_CYAN;i++){
        sprites_item_key_cyan[i].x=i*32;
        sprites_item_key_cyan[i].y=80+32*11+48+16+32*4;
        sprites_item_key_cyan[i].w=32;
        sprites_item_key_cyan[i].h=32;
    }

    for(short i=0;i<ITEM_SPRITES_SINK;i++){
        sprites_item_sink[i].x=i*32;
        sprites_item_sink[i].y=80+32*9;
        sprites_item_sink[i].w=32;
        sprites_item_sink[i].h=32;
    }

    for(short i=0;i<ITEM_SPRITES_J_BALLOON;i++){
        sprites_item_j_balloon[i].x=i*32;
        sprites_item_j_balloon[i].y=80+32*10;
        sprites_item_j_balloon[i].w=32;
        sprites_item_j_balloon[i].h=32;
    }

    for(short i=0;i<ITEM_SPRITES_AMMO_BARREL;i++){
        sprites_item_ammo_barrel[i].x=i*34;
        sprites_item_ammo_barrel[i].y=80+32*11;
        sprites_item_ammo_barrel[i].w=34;
        sprites_item_ammo_barrel[i].h=48;
    }

    for(short i=0;i<ITEM_SPRITES_CANDY;i++){
        sprites_item_candy[i].x=i*16;
        sprites_item_candy[i].y=80+32*11+48;
        sprites_item_candy[i].w=16;
        sprites_item_candy[i].h=16;
    }

    for(short i=0;i<ITEM_SPRITES_CANDY;i++){
        sprites_item_candy_black[i].x=32+i*16;
        sprites_item_candy_black[i].y=80+32*11+48;
        sprites_item_candy_black[i].w=16;
        sprites_item_candy_black[i].h=16;
    }

    for(short i=0;i<ITEM_SPRITES_SUIT_DEADLY_WATER;i++){
        sprites_item_suit_deadly_water[i].x=i*32;
        sprites_item_suit_deadly_water[i].y=80+32*11+48+16+32*5;
        sprites_item_suit_deadly_water[i].w=32;
        sprites_item_suit_deadly_water[i].h=32;
    }

    for(short i=0;i<ITEM_SPRITES_SUIT_SHARP;i++){
        sprites_item_suit_sharp[i].x=i*32;
        sprites_item_suit_sharp[i].y=80+32*11+48+16+32*6;
        sprites_item_suit_sharp[i].w=32;
        sprites_item_suit_sharp[i].h=32;
    }

    for(short i=0;i<ITEM_SPRITES_SUIT_BANANA;i++){
        sprites_item_suit_banana[i].x=i*32;
        sprites_item_suit_banana[i].y=80+32*11+48+16+32*7;
        sprites_item_suit_banana[i].w=32;
        sprites_item_suit_banana[i].h=32;
    }

    for(short i=0;i<ITEM_SPRITES_SHOT_HOMING;i++){
        sprites_item_shot_homing[i].x=i*32;
        sprites_item_shot_homing[i].y=80+32*11+48+16+32*8;
        sprites_item_shot_homing[i].w=32;
        sprites_item_shot_homing[i].h=32;
    }

    for(short i=0;i<ITEM_SPRITES_TRANSLATOR;i++){
        sprites_item_translator[i].x=i*32;
        sprites_item_translator[i].y=80+32*11+48+16+32*9;
        sprites_item_translator[i].w=32;
        sprites_item_translator[i].h=32;
    }

    for(short i=0;i<ITEM_SPRITES_J_WING;i++){
        sprites_item_j_wing[i].x=i*32;
        sprites_item_j_wing[i].y=80+32*11+48+16+32*10;
        sprites_item_j_wing[i].w=32;
        sprites_item_j_wing[i].h=32;
    }
}

void create_sprite_sheets_shots(){
    SPRITES_SHOT.clear();

    for(short i=0;i<SHOT_TYPES_COUNT;i++){
        SPRITES_SHOT.push_back(shot_sprite_data());
    }

    //Player shot
    for(short i=0;i<SHOT_SPRITE_COUNTS[SHOT_PLAYER].sprites;i++){
        SPRITES_SHOT[SHOT_PLAYER].move.push_back(SDL_Rect());
        SPRITES_SHOT[SHOT_PLAYER].move[i].x=i*SHOT_SPRITE_COUNTS[SHOT_PLAYER].width;
        SPRITES_SHOT[SHOT_PLAYER].move[i].y=0;
        SPRITES_SHOT[SHOT_PLAYER].move[i].w=SHOT_SPRITE_COUNTS[SHOT_PLAYER].width;
        SPRITES_SHOT[SHOT_PLAYER].move[i].h=SHOT_SPRITE_COUNTS[SHOT_PLAYER].height;
    }

    //Player shot splode
    for(short i=0;i<SHOT_SPRITE_COUNTS[SHOT_PLAYER_SPLODE].sprites;i++){
        SPRITES_SHOT[SHOT_PLAYER_SPLODE].move.push_back(SDL_Rect());
        SPRITES_SHOT[SHOT_PLAYER_SPLODE].move[i].x=i*SHOT_SPRITE_COUNTS[SHOT_PLAYER_SPLODE].width;
        SPRITES_SHOT[SHOT_PLAYER_SPLODE].move[i].y=0;
        SPRITES_SHOT[SHOT_PLAYER_SPLODE].move[i].w=SHOT_SPRITE_COUNTS[SHOT_PLAYER_SPLODE].width;
        SPRITES_SHOT[SHOT_PLAYER_SPLODE].move[i].h=SHOT_SPRITE_COUNTS[SHOT_PLAYER_SPLODE].height;
    }

    //Player shot splosion
    for(short i=0;i<SHOT_SPRITE_COUNTS[SHOT_PLAYER_SPLOSION].sprites;i++){
        SPRITES_SHOT[SHOT_PLAYER_SPLOSION].move.push_back(SDL_Rect());
        SPRITES_SHOT[SHOT_PLAYER_SPLOSION].move[i].x=i*SHOT_SPRITE_COUNTS[SHOT_PLAYER_SPLOSION].width;
        SPRITES_SHOT[SHOT_PLAYER_SPLOSION].move[i].y=0;
        SPRITES_SHOT[SHOT_PLAYER_SPLOSION].move[i].w=SHOT_SPRITE_COUNTS[SHOT_PLAYER_SPLOSION].width;
        SPRITES_SHOT[SHOT_PLAYER_SPLOSION].move[i].h=SHOT_SPRITE_COUNTS[SHOT_PLAYER_SPLOSION].height;
    }

    //Player shot homing
    for(short i=0;i<SHOT_SPRITE_COUNTS[SHOT_PLAYER_HOMING].sprites;i++){
        SPRITES_SHOT[SHOT_PLAYER_HOMING].move.push_back(SDL_Rect());
        SPRITES_SHOT[SHOT_PLAYER_HOMING].move[i].x=i*SHOT_SPRITE_COUNTS[SHOT_PLAYER_HOMING].width;
        SPRITES_SHOT[SHOT_PLAYER_HOMING].move[i].y=0;
        SPRITES_SHOT[SHOT_PLAYER_HOMING].move[i].w=SHOT_SPRITE_COUNTS[SHOT_PLAYER_HOMING].width;
        SPRITES_SHOT[SHOT_PLAYER_HOMING].move[i].h=SHOT_SPRITE_COUNTS[SHOT_PLAYER_HOMING].height;
    }

    //Fruit
    for(short i=0;i<SHOT_SPRITE_COUNTS[SHOT_FRUIT].sprites;i++){
        SPRITES_SHOT[SHOT_FRUIT].move.push_back(SDL_Rect());
        SPRITES_SHOT[SHOT_FRUIT].move[i].x=i*SHOT_SPRITE_COUNTS[SHOT_FRUIT].width;
        SPRITES_SHOT[SHOT_FRUIT].move[i].y=0;
        SPRITES_SHOT[SHOT_FRUIT].move[i].w=SHOT_SPRITE_COUNTS[SHOT_FRUIT].width;
        SPRITES_SHOT[SHOT_FRUIT].move[i].h=SHOT_SPRITE_COUNTS[SHOT_FRUIT].height;
    }

    //Laser
    for(short i=0;i<SHOT_SPRITE_COUNTS[SHOT_LASER].sprites;i++){
        SPRITES_SHOT[SHOT_LASER].move.push_back(SDL_Rect());
        SPRITES_SHOT[SHOT_LASER].move[i].x=i*SHOT_SPRITE_COUNTS[SHOT_LASER].width;
        SPRITES_SHOT[SHOT_LASER].move[i].y=0;
        SPRITES_SHOT[SHOT_LASER].move[i].w=SHOT_SPRITE_COUNTS[SHOT_LASER].width;
        SPRITES_SHOT[SHOT_LASER].move[i].h=SHOT_SPRITE_COUNTS[SHOT_LASER].height;
    }

    //Ball
    for(short i=0;i<SHOT_SPRITE_COUNTS[SHOT_BALL].sprites;i++){
        SPRITES_SHOT[SHOT_BALL].move.push_back(SDL_Rect());
        SPRITES_SHOT[SHOT_BALL].move[i].x=i*SHOT_SPRITE_COUNTS[SHOT_BALL].width;
        SPRITES_SHOT[SHOT_BALL].move[i].y=0;
        SPRITES_SHOT[SHOT_BALL].move[i].w=SHOT_SPRITE_COUNTS[SHOT_BALL].width;
        SPRITES_SHOT[SHOT_BALL].move[i].h=SHOT_SPRITE_COUNTS[SHOT_BALL].height;
    }

    //Arrow
    for(short i=0;i<SHOT_SPRITE_COUNTS[SHOT_ARROW].sprites;i++){
        SPRITES_SHOT[SHOT_ARROW].move.push_back(SDL_Rect());
        SPRITES_SHOT[SHOT_ARROW].move[i].x=i*SHOT_SPRITE_COUNTS[SHOT_ARROW].width;
        SPRITES_SHOT[SHOT_ARROW].move[i].y=0;
        SPRITES_SHOT[SHOT_ARROW].move[i].w=SHOT_SPRITE_COUNTS[SHOT_ARROW].width;
        SPRITES_SHOT[SHOT_ARROW].move[i].h=SHOT_SPRITE_COUNTS[SHOT_ARROW].height;
    }

    //Snowball
    for(short i=0;i<SHOT_SPRITE_COUNTS[SHOT_SNOWBALL].sprites;i++){
        SPRITES_SHOT[SHOT_SNOWBALL].move.push_back(SDL_Rect());
        SPRITES_SHOT[SHOT_SNOWBALL].move[i].x=i*SHOT_SPRITE_COUNTS[SHOT_SNOWBALL].width;
        SPRITES_SHOT[SHOT_SNOWBALL].move[i].y=0;
        SPRITES_SHOT[SHOT_SNOWBALL].move[i].w=SHOT_SPRITE_COUNTS[SHOT_SNOWBALL].width;
        SPRITES_SHOT[SHOT_SNOWBALL].move[i].h=SHOT_SPRITE_COUNTS[SHOT_SNOWBALL].height;
    }

    //Fireball
    for(short i=0;i<SHOT_SPRITE_COUNTS[SHOT_FIREBALL].sprites;i++){
        SPRITES_SHOT[SHOT_FIREBALL].move.push_back(SDL_Rect());
        SPRITES_SHOT[SHOT_FIREBALL].move[i].x=i*SHOT_SPRITE_COUNTS[SHOT_FIREBALL].width;
        SPRITES_SHOT[SHOT_FIREBALL].move[i].y=0;
        SPRITES_SHOT[SHOT_FIREBALL].move[i].w=SHOT_SPRITE_COUNTS[SHOT_FIREBALL].width;
        SPRITES_SHOT[SHOT_FIREBALL].move[i].h=SHOT_SPRITE_COUNTS[SHOT_FIREBALL].height;
    }

    //Poison
    for(short i=0;i<SHOT_SPRITE_COUNTS[SHOT_POISON].sprites;i++){
        SPRITES_SHOT[SHOT_POISON].move.push_back(SDL_Rect());
        SPRITES_SHOT[SHOT_POISON].move[i].x=i*SHOT_SPRITE_COUNTS[SHOT_POISON].width;
        SPRITES_SHOT[SHOT_POISON].move[i].y=0;
        SPRITES_SHOT[SHOT_POISON].move[i].w=SHOT_SPRITE_COUNTS[SHOT_POISON].width;
        SPRITES_SHOT[SHOT_POISON].move[i].h=SHOT_SPRITE_COUNTS[SHOT_POISON].height;
    }

    //Homing missile
    for(short i=0;i<SHOT_SPRITE_COUNTS[SHOT_HOMING_MISSILE].sprites;i++){
        SPRITES_SHOT[SHOT_HOMING_MISSILE].move.push_back(SDL_Rect());
        SPRITES_SHOT[SHOT_HOMING_MISSILE].move[i].x=i*SHOT_SPRITE_COUNTS[SHOT_HOMING_MISSILE].width;
        SPRITES_SHOT[SHOT_HOMING_MISSILE].move[i].y=0;
        SPRITES_SHOT[SHOT_HOMING_MISSILE].move[i].w=SHOT_SPRITE_COUNTS[SHOT_HOMING_MISSILE].width;
        SPRITES_SHOT[SHOT_HOMING_MISSILE].move[i].h=SHOT_SPRITE_COUNTS[SHOT_HOMING_MISSILE].height;
    }

    //Boss Defeated
    for(short i=0;i<SHOT_SPRITE_COUNTS[SHOT_BOSS_DEFEATED].sprites;i++){
        SPRITES_SHOT[SHOT_BOSS_DEFEATED].move.push_back(SDL_Rect());
        SPRITES_SHOT[SHOT_BOSS_DEFEATED].move[i].x=i*SHOT_SPRITE_COUNTS[SHOT_BOSS_DEFEATED].width;
        SPRITES_SHOT[SHOT_BOSS_DEFEATED].move[i].y=0;
        SPRITES_SHOT[SHOT_BOSS_DEFEATED].move[i].w=SHOT_SPRITE_COUNTS[SHOT_BOSS_DEFEATED].width;
        SPRITES_SHOT[SHOT_BOSS_DEFEATED].move[i].h=SHOT_SPRITE_COUNTS[SHOT_BOSS_DEFEATED].height;
    }

    //Boss Haunted House
    for(short i=0;i<SHOT_SPRITE_COUNTS[SHOT_BOSS_HAUNTED_HOUSE].sprites;i++){
        SPRITES_SHOT[SHOT_BOSS_HAUNTED_HOUSE].move.push_back(SDL_Rect());
        SPRITES_SHOT[SHOT_BOSS_HAUNTED_HOUSE].move[i].x=i*SHOT_SPRITE_COUNTS[SHOT_BOSS_HAUNTED_HOUSE].width;
        SPRITES_SHOT[SHOT_BOSS_HAUNTED_HOUSE].move[i].y=0;
        SPRITES_SHOT[SHOT_BOSS_HAUNTED_HOUSE].move[i].w=SHOT_SPRITE_COUNTS[SHOT_BOSS_HAUNTED_HOUSE].width;
        SPRITES_SHOT[SHOT_BOSS_HAUNTED_HOUSE].move[i].h=SHOT_SPRITE_COUNTS[SHOT_BOSS_HAUNTED_HOUSE].height;
    }

    //Boss Volcano
    for(short i=0;i<SHOT_SPRITE_COUNTS[SHOT_BOSS_VOLCANO].sprites;i++){
        SPRITES_SHOT[SHOT_BOSS_VOLCANO].move.push_back(SDL_Rect());
        SPRITES_SHOT[SHOT_BOSS_VOLCANO].move[i].x=i*SHOT_SPRITE_COUNTS[SHOT_BOSS_VOLCANO].width;
        SPRITES_SHOT[SHOT_BOSS_VOLCANO].move[i].y=0;
        SPRITES_SHOT[SHOT_BOSS_VOLCANO].move[i].w=SHOT_SPRITE_COUNTS[SHOT_BOSS_VOLCANO].width;
        SPRITES_SHOT[SHOT_BOSS_VOLCANO].move[i].h=SHOT_SPRITE_COUNTS[SHOT_BOSS_VOLCANO].height;
    }

    //Boss Pyramid
    for(short i=0;i<SHOT_SPRITE_COUNTS[SHOT_BOSS_PYRAMID].sprites;i++){
        SPRITES_SHOT[SHOT_BOSS_PYRAMID].move.push_back(SDL_Rect());
        SPRITES_SHOT[SHOT_BOSS_PYRAMID].move[i].x=i*SHOT_SPRITE_COUNTS[SHOT_BOSS_PYRAMID].width;
        SPRITES_SHOT[SHOT_BOSS_PYRAMID].move[i].y=0;
        SPRITES_SHOT[SHOT_BOSS_PYRAMID].move[i].w=SHOT_SPRITE_COUNTS[SHOT_BOSS_PYRAMID].width;
        SPRITES_SHOT[SHOT_BOSS_PYRAMID].move[i].h=SHOT_SPRITE_COUNTS[SHOT_BOSS_PYRAMID].height;
    }

    //Boss Lighthouse
    for(short i=0;i<SHOT_SPRITE_COUNTS[SHOT_BOSS_LIGHTHOUSE].sprites;i++){
        SPRITES_SHOT[SHOT_BOSS_LIGHTHOUSE].move.push_back(SDL_Rect());
        SPRITES_SHOT[SHOT_BOSS_LIGHTHOUSE].move[i].x=i*SHOT_SPRITE_COUNTS[SHOT_BOSS_LIGHTHOUSE].width;
        SPRITES_SHOT[SHOT_BOSS_LIGHTHOUSE].move[i].y=0;
        SPRITES_SHOT[SHOT_BOSS_LIGHTHOUSE].move[i].w=SHOT_SPRITE_COUNTS[SHOT_BOSS_LIGHTHOUSE].width;
        SPRITES_SHOT[SHOT_BOSS_LIGHTHOUSE].move[i].h=SHOT_SPRITE_COUNTS[SHOT_BOSS_LIGHTHOUSE].height;
    }
}

void create_sprite_sheets_moving_platforms(){
    for(short i=0;i<MOVING_PLATFORM_SPRITES;i++){
        sprites_moving_platform_left[i].x=i*58;
        sprites_moving_platform_left[i].y=0;
        sprites_moving_platform_left[i].w=58;
        sprites_moving_platform_left[i].h=10;
    }

    for(short i=0;i<MOVING_PLATFORM_SPRITES;i++){
        sprites_moving_platform_right[i].x=i*58;
        sprites_moving_platform_right[i].y=10;
        sprites_moving_platform_right[i].w=58;
        sprites_moving_platform_right[i].h=10;
    }

    for(short i=0;i<MOVING_PLATFORM_SPRITES;i++){
        sprites_moving_platform_up[i].x=i*58;
        sprites_moving_platform_up[i].y=20;
        sprites_moving_platform_up[i].w=58;
        sprites_moving_platform_up[i].h=15;
    }

    for(short i=0;i<MOVING_PLATFORM_SPRITES;i++){
        sprites_moving_platform_down[i].x=i*58;
        sprites_moving_platform_down[i].y=35;
        sprites_moving_platform_down[i].w=58;
        sprites_moving_platform_down[i].h=15;
    }

    for(short i=0;i<MOVING_PLATFORM_IDLE_SPRITES;i++){
        sprites_moving_platform_idle[i].x=i*58;
        sprites_moving_platform_idle[i].y=50;
        sprites_moving_platform_idle[i].w=58;
        sprites_moving_platform_idle[i].h=12;
    }
}

void create_sprite_sheets_triggers(){
    sprites_triggers.clear();

    for(short i=0;i<TRIGGER_COUNT+1;i++){
        sprites_triggers.push_back(trigger_data());
    }

    for(short n=0;n<TRIGGER_COUNT;n++){
        for(short i=0;i<TRIGGER_SPRITES;i++){
            sprites_triggers[n].change_states.push_back(SDL_Rect());
            sprites_triggers[n].change_states[i].x=i*32;
            sprites_triggers[n].change_states[i].y=n*32;
            sprites_triggers[n].change_states[i].w=32;
            sprites_triggers[n].change_states[i].h=32;
        }
    }
}

void create_sprite_sheets_doors(){
    for(short n=0;n<NUMBER_OF_DOORS;n++){
        for(short i=0;i<DOOR_SPRITES;i++){
            sprites_doors[i][n].x=i*32;
            sprites_doors[i][n].y=n*96;
            sprites_doors[i][n].w=32;
            sprites_doors[i][n].h=96;
        }
    }

    for(short n=0;n<NUMBER_OF_KEY_DOORS;n++){
        for(short i=0;i<KEY_DOOR_SPRITES;i++){
            sprites_key_doors[i][n].x=288+n*32;
            sprites_key_doors[i][n].y=i*96;
            sprites_key_doors[i][n].w=32;
            sprites_key_doors[i][n].h=96;
        }
    }

    for(short i=0;i<DOOR_STANDARD_SPRITES;i++){
        sprites_door_standard[i].x=0;
        sprites_door_standard[i].y=i*96;
        sprites_door_standard[i].w=32;
        sprites_door_standard[i].h=96;
    }
}

void create_sprite_sheets_signs(){
    for(short i=0;i<SIGN_COUNT;i++){
        sprites_signs[i].x=i*32;
        sprites_signs[i].y=0;
        sprites_signs[i].w=32;
        sprites_signs[i].h=32;
    }
}

void create_sprite_sheets_springs(){
    for(short i=0;i<SPRING_SPRITES;i++){
        sprites_spring[i].x=i*SPRING_W;
        sprites_spring[i].y=0;
        sprites_spring[i].w=SPRING_W;
        sprites_spring[i].h=SPRING_H;
    }
}

void create_sprite_sheets_boosters(){
    for(short i=0;i<BOOSTER_SPRITES;i++){
        sprites_booster[i].x=i*BOOSTER_W;
        sprites_booster[i].y=0;
        sprites_booster[i].w=BOOSTER_W;
        sprites_booster[i].h=BOOSTER_H;
    }
}

void create_sprite_sheets_effect_water_bubble(){
    for(short i=0;i<4;i++){
        sprites_effect_water_bubble[i].x=384+32*i;
        sprites_effect_water_bubble[i].y=0;
        sprites_effect_water_bubble[i].w=13;
        sprites_effect_water_bubble[i].h=13;
    }
}

void create_sprite_sheets_effect_water_splash(){
    sprites_effect_water_splash[0].x=32*9;
    sprites_effect_water_splash[0].y=0;
    sprites_effect_water_splash[0].w=32;
    sprites_effect_water_splash[0].h=32;

    sprites_effect_water_splash[1].x=32*10;
    sprites_effect_water_splash[1].y=0;
    sprites_effect_water_splash[1].w=32;
    sprites_effect_water_splash[1].h=32;

    sprites_effect_water_splash[2].x=32*11;
    sprites_effect_water_splash[2].y=0;
    sprites_effect_water_splash[2].w=32;
    sprites_effect_water_splash[2].h=32;
}

void create_sprite_sheets_interface(){
    sprites_shot_types.clear();

    for(int i=0;i<3;i++){
        sprites_shot_types.push_back(SDL_Rect());
        sprites_shot_types[i].x=i*32;
        sprites_shot_types[i].y=0;
        sprites_shot_types[i].w=32;
        sprites_shot_types[i].h=32;
    }

    sprites_moon.clear();

    for(int i=MOON_PHASE_BEGIN;i<MOON_PHASE_END;i++){
        sprites_moon.push_back(SDL_Rect());
        sprites_moon[i].x=0;
        sprites_moon[i].y=i*76;
        sprites_moon[i].w=78;
        sprites_moon[i].h=76;
    }
}

void create_sprite_sheets_powerups(){
    sprites_powerup_jump.clear();

    for(int i=0;i<POWERUP_SPRITES_JUMP;i++){
        sprites_powerup_jump.push_back(SDL_Rect());
        sprites_powerup_jump[i].x=i*42;
        sprites_powerup_jump[i].y=0;
        sprites_powerup_jump[i].w=42;
        sprites_powerup_jump[i].h=46;
    }
}

void create_sprite_sheets_survival(){
    sprites_survival_spawn.clear();

    for(int i=0;i<SURVIVAL_SPRITES_SPAWN;i++){
        sprites_survival_spawn.push_back(SDL_Rect());
        sprites_survival_spawn[i].x=i*32;
        sprites_survival_spawn[i].y=0;
        sprites_survival_spawn[i].w=32;
        sprites_survival_spawn[i].h=64;
    }
}

void generate_sprite_sheets(){
    create_sprite_sheets_player();
    create_sprite_sheets_tiles();
    create_sprite_sheets_items();
    create_sprite_sheets_npcs();
    create_sprite_sheets_traps();
    create_sprite_sheets_shots();
    create_sprite_sheets_moving_platforms();
    create_sprite_sheets_triggers();
    create_sprite_sheets_doors();
    create_sprite_sheets_signs();
    create_sprite_sheets_springs();
    create_sprite_sheets_boosters();
    create_sprite_sheets_effect_water_bubble();
    create_sprite_sheets_effect_water_splash();
    create_sprite_sheets_interface();
    create_sprite_sheets_powerups();
    create_sprite_sheets_survival();
}
