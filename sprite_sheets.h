/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef sprite_sheets_h
#define sprite_sheets_h

#include <vector>

#include "player_sprites.h"
#include "tile_sprites.h"
#include "item_sprites.h"
#include "npc_data.h"
#include "trap_data.h"
#include "powerup_sprites.h"

#include <SDL.h>

//Spritesheets:

//Player:
extern SDL_Rect sprites_player_worldmap_idle_up[];
extern SDL_Rect sprites_player_worldmap_idle_down[];
extern SDL_Rect sprites_player_worldmap_idle_left[];
extern SDL_Rect sprites_player_worldmap_idle_right[];

extern SDL_Rect sprites_player_worldmap_walk_up[];
extern SDL_Rect sprites_player_worldmap_walk_down[];
extern SDL_Rect sprites_player_worldmap_walk_left[];
extern SDL_Rect sprites_player_worldmap_walk_right[];

extern SDL_Rect sprites_player_idle_right[];
extern SDL_Rect sprites_player_idle_left[];

extern SDL_Rect sprites_player_right[];
extern SDL_Rect sprites_player_left[];

extern SDL_Rect sprites_player_jump_right[];
extern SDL_Rect sprites_player_jump_left[];

extern SDL_Rect sprites_player_swim_up_right[];
extern SDL_Rect sprites_player_swim_up_left[];
extern SDL_Rect sprites_player_swim_right[];
extern SDL_Rect sprites_player_swim_left[];

extern SDL_Rect sprites_player_shoot_right[];
extern SDL_Rect sprites_player_shoot_left[];
extern SDL_Rect sprites_player_shoot_right_air[];
extern SDL_Rect sprites_player_shoot_left_air[];
extern SDL_Rect sprites_player_shoot_up_right[];
extern SDL_Rect sprites_player_shoot_up_left[];
extern SDL_Rect sprites_player_shoot_up_right_air[];
extern SDL_Rect sprites_player_shoot_up_left_air[];
extern SDL_Rect sprites_player_shoot_down_right_air[];
extern SDL_Rect sprites_player_shoot_down_left_air[];
extern SDL_Rect sprites_player_shoot_climb_right[];
extern SDL_Rect sprites_player_shoot_climb_left[];
extern SDL_Rect sprites_player_shoot_climb_up_right[];
extern SDL_Rect sprites_player_shoot_climb_up_left[];
extern SDL_Rect sprites_player_shoot_climb_down_right[];
extern SDL_Rect sprites_player_shoot_climb_down_left[];

extern SDL_Rect sprites_player_climb_up_left[];
extern SDL_Rect sprites_player_climb_up_right[];
extern SDL_Rect sprites_player_climb_down_left[];
extern SDL_Rect sprites_player_climb_down_right[];

extern SDL_Rect sprites_player_look_right[];
extern SDL_Rect sprites_player_look_right_up[];
extern SDL_Rect sprites_player_look_right_down[];
extern SDL_Rect sprites_player_look_right_climb_up[];
extern SDL_Rect sprites_player_look_right_climb_down[];

extern SDL_Rect sprites_player_look_left[];
extern SDL_Rect sprites_player_look_left_up[];
extern SDL_Rect sprites_player_look_left_down[];
extern SDL_Rect sprites_player_look_left_climb_up[];
extern SDL_Rect sprites_player_look_left_climb_down[];

extern SDL_Rect sprites_player_death[];

extern SDL_Rect sprites_player_crouch_right[];
extern SDL_Rect sprites_player_crouch_left[];

//Powerups:
extern std::vector<SDL_Rect> sprites_powerup_jump;

//Tiles:
extern SDL_Rect sprites_tiles[];
extern SDL_Rect sprites_water_tiles[];
extern SDL_Rect worldmap_sprites_tiles[];

//Background Tiles:
extern SDL_Rect sprites_tiles_background[];

//Items:
extern SDL_Rect sprites_item_leaf[];
extern SDL_Rect sprites_item_cheese[];
extern SDL_Rect sprites_item_ammo[];
extern SDL_Rect sprites_item_spawnpoint[];
extern SDL_Rect sprites_item_checkpoint[];
extern SDL_Rect sprites_item_endpoint[];
extern SDL_Rect sprites_item_endpoint_secret[];
extern SDL_Rect sprites_item_swimming_gear[];
extern SDL_Rect sprites_item_key_red[];
extern SDL_Rect sprites_item_key_blue[];
extern SDL_Rect sprites_item_key_green[];
extern SDL_Rect sprites_item_key_yellow[];
extern SDL_Rect sprites_item_key_orange[];
extern SDL_Rect sprites_item_key_purple[];
extern SDL_Rect sprites_item_towel[];
extern SDL_Rect sprites_item_key_gray[];
extern SDL_Rect sprites_item_key_brown[];
extern SDL_Rect sprites_item_key_black[];
extern SDL_Rect sprites_item_key_pink[];
extern SDL_Rect sprites_item_key_cyan[];
extern SDL_Rect sprites_item_sink[];
extern SDL_Rect sprites_item_j_balloon[];
extern SDL_Rect sprites_item_ammo_barrel[];
extern SDL_Rect sprites_item_candy[];
extern SDL_Rect sprites_item_candy_black[];
extern SDL_Rect sprites_item_suit_deadly_water[];
extern SDL_Rect sprites_item_suit_sharp[];
extern SDL_Rect sprites_item_suit_banana[];
extern SDL_Rect sprites_item_shot_homing[];
extern SDL_Rect sprites_item_translator[];
extern SDL_Rect sprites_item_j_wing[];

//NPCs:
struct npc_sprite_data{
    std::vector<SDL_Rect> walk_right;
    std::vector<SDL_Rect> walk_left;

    std::vector<SDL_Rect> jump_right;
    std::vector<SDL_Rect> jump_left;

    std::vector<SDL_Rect> swim_right;
    std::vector<SDL_Rect> swim_left;

    std::vector<SDL_Rect> fly_right;
    std::vector<SDL_Rect> fly_left;

    std::vector<SDL_Rect> special_attack_right;
    std::vector<SDL_Rect> special_attack_left;

    std::vector<SDL_Rect> dead;
};

extern std::vector<npc_sprite_data> SPRITES_NPC;

//Traps:
struct trap_sprite_data{
    std::vector<SDL_Rect> move;

    std::vector<SDL_Rect> idle_active;
    std::vector<SDL_Rect> idle_inactive;
};

extern std::vector<trap_sprite_data> SPRITES_TRAP;

//Shots:
struct shot_sprite_data{
    std::vector<SDL_Rect> move;
};

extern std::vector<shot_sprite_data> SPRITES_SHOT;

//Moving Platforms:
extern SDL_Rect sprites_moving_platform_left[];
extern SDL_Rect sprites_moving_platform_right[];
extern SDL_Rect sprites_moving_platform_up[];
extern SDL_Rect sprites_moving_platform_down[];
extern SDL_Rect sprites_moving_platform_idle[];

//Triggers:
struct trigger_data{
    std::vector<SDL_Rect> change_states;
};
extern std::vector<trigger_data> sprites_triggers;

//Doors:
extern SDL_Rect sprites_doors[][NUMBER_OF_DOORS];
extern SDL_Rect sprites_key_doors[][NUMBER_OF_KEY_DOORS];
extern SDL_Rect sprites_door_standard[];

//Signs:
extern SDL_Rect sprites_signs[];

//Springs:
extern SDL_Rect sprites_spring[];

//Boosters:
extern SDL_Rect sprites_booster[];

//Effects:

//Water Bubble Effect:
extern SDL_Rect sprites_effect_water_bubble[];

//Water Splash Effect:
extern SDL_Rect sprites_effect_water_splash[];

//Survival
extern std::vector<SDL_Rect> sprites_survival_spawn;

//Interface
extern std::vector<SDL_Rect> sprites_shot_types;
extern std::vector<SDL_Rect> sprites_moon;

void generate_sprite_sheets();

void create_sprite_sheets_npcs();
void create_sprite_sheets_traps();
void create_sprite_sheets_player();
void create_sprite_sheets_tiles();
void create_sprite_sheets_items();
void create_sprite_sheets_shots();
void create_sprite_sheets_moving_platforms();
void create_sprite_sheets_triggers();
void create_sprite_sheets_doors();
void create_sprite_sheets_signs();
void create_sprite_sheets_springs();
void create_sprite_sheets_boosters();
void create_sprite_sheets_effect_water_bubble();
void create_sprite_sheets_effect_water_splash();
void create_sprite_sheets_interface();
void create_sprite_sheets_powerups();
void create_sprite_sheets_survival();

#endif
