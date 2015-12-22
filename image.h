/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef image_h
#define image_h

#include <vector>

#include "image_data.h"

#include <SDL.h>
#include <string>

class Image{
    private:
    public:
    //Load global images.
    void load_images_global();
    //Unload global images.
    void unload_images_global();

    //Load level-specific images.
    void load_images_level(short level_to_load,short sub_level_to_load,std::string current_level);
    //Load level-specific images.
    void unload_images_level(short level_to_unload);

    void load_images_level_adventure(short level_to_load,short sub_level_to_load);
    void load_images_level_survival(std::string current_level);

    //Global images.
    std::vector<image_data> menu_background_images;
    std::vector<image_data> overlay_snow_images;
    std::vector<image_data> overlay_rain_images;

    image_data sprite_sheet_player;
    image_data sprite_sheet_player_worldmap;
    image_data sprite_sheet_character_slime_o;
    image_data sprite_sheet_character_worldmap_slime_o;
    image_data sprite_sheet_character_sketch;
    image_data sprite_sheet_character_worldmap_sketch;
    image_data sprite_sheet_character_penny;
    image_data sprite_sheet_character_worldmap_penny;
    image_data sprite_sheet_items;
    image_data sprite_sheet_level_items;
    image_data sprite_sheet_sign;
    image_data sprite_sheet_spring;
    image_data sprite_sheet_booster;
    image_data overlay_night;
    image_data moon;
    image_data sun;
    image_data balloon;
    image_data tophat;
    image_data cursor;
    image_data logo_cheese_and_bacon;
    image_data logo_hubert;
    image_data loading_screen_main;
    image_data level_title_background_0;
    image_data level_title_background_1;
    image_data level_title_background_2;
    image_data menu_background_0;
    image_data checkbox_unchecked;
    image_data checkbox_checked;
    image_data ammo_box;
    image_data character_box_hubert;
    image_data character_box_slime_o;
    image_data character_box_sketch;
    image_data character_box_penny;
    image_data shot_types;
    image_data survival_spawn;
    image_data powerup_jump;
    image_data sprite_sheet_npc_slime;
    image_data sprite_sheet_npc_cow;
    image_data sprite_sheet_npc_bee;
    image_data sprite_sheet_shot;
    image_data sprite_sheet_shot_player_homing;
    image_data sprite_sheet_shot_player_splosion;
    image_data sprite_sheet_shot_fruit;
    image_data sprite_sheet_shot_laser;
    image_data sprite_sheet_shot_ball;
    image_data sprite_sheet_shot_arrow;
    image_data sprite_sheet_shot_snowball;
    image_data sprite_sheet_shot_fireball;
    image_data sprite_sheet_shot_poison;
    image_data sprite_sheet_shot_homing_missile;
    image_data sprite_sheet_shot_boss_defeated;
    image_data sprite_sheet_shot_boss_haunted_house;
    image_data sprite_sheet_shot_boss_volcano;
    image_data sprite_sheet_shot_boss_pyramid;
    image_data sprite_sheet_shot_boss_lighthouse;
    image_data sprite_sheet_moving_platform;
    image_data sprite_sheet_triggers;
    image_data sprite_sheet_doors;
    image_data sprite_sheet_door_standard;
    image_data sprite_sheet_trap_fire;
    image_data sprite_sheet_npc_replicator;
    image_data sprite_sheet_npc_fish;
    image_data sprite_sheet_npc_monkey;
    image_data sprite_sheet_npc_laser_spinner;
    image_data sprite_sheet_npc_penguin;
    image_data sprite_sheet_npc_fish_on_ice;
    image_data sprite_sheet_npc_penguin_harmless;
    image_data sprite_sheet_npc_fish_on_ice_harmless;
    image_data sprite_sheet_npc_sword_knight;
    image_data sprite_sheet_npc_ball_knight;
    image_data sprite_sheet_npc_archer;
    image_data sprite_sheet_npc_scarecrow;
    image_data sprite_sheet_npc_pumpkin;
    image_data sprite_sheet_npc_zombie_cow;
    image_data sprite_sheet_npc_mummy_cow;
    image_data sprite_sheet_npc_ghost;
    image_data sprite_sheet_npc_spider;
    image_data sprite_sheet_npc_cat;
    image_data sprite_sheet_npc_spider_walking;
    image_data sprite_sheet_npc_ram;
    image_data sprite_sheet_npc_eagle;
    image_data sprite_sheet_npc_snowman;
    image_data sprite_sheet_npc_fireball;
    image_data sprite_sheet_npc_dragon_whelp;
    image_data sprite_sheet_npc_fireball_shooter_left;
    image_data sprite_sheet_npc_fireball_shooter_right;
    image_data sprite_sheet_npc_fireball_shooter_up;
    image_data sprite_sheet_npc_fireball_shooter_down;
    image_data sprite_sheet_npc_dust_devil;
    image_data sprite_sheet_npc_scorpion;
    image_data sprite_sheet_npc_sphinx;
    image_data sprite_sheet_npc_shark;
    image_data sprite_sheet_npc_henchmouse;
    image_data sprite_sheet_npc_mechsuit_mouse;
    image_data sprite_sheet_npc_sally;
    image_data sprite_sheet_npc_bouncing_ball;
    image_data sprite_sheet_npc_sea_turtle;
    image_data sprite_sheet_npc_starfish;
    image_data sprite_sheet_npc_boss_castle;
    image_data sprite_sheet_npc_boss_haunted_house;
    image_data sprite_sheet_npc_boss_volcano;
    image_data sprite_sheet_npc_boss_pyramid;
    image_data sprite_sheet_npc_boss_lighthouse;
    image_data sprite_sheet_trap_spikes;
    image_data sprite_sheet_trap_icicle;
    image_data sprite_sheet_trap_branch;
    image_data sprite_sheet_trap_electric;
    image_data sprite_sheet_trap_stoneslab;
    image_data sprite_sheet_trap_buzzsaw;
    image_data sprite_sheet_trap_spikes_up;
    image_data sprite_sheet_trap_spikes_down;
    image_data sprite_sheet_trap_spikes_right;
    image_data sprite_sheet_trap_spikes_left;
    image_data sprite_sheet_trap_boss_castle;
    image_data sprite_sheet_trap_boss_pyramid;
    image_data sprite_sheet_trap_seaweed;
    image_data touch_controller_dpad;
    image_data touch_controller_main;
    image_data touch_controller_main_small;
    image_data touch_controller_middle;
    image_data touch_controller_middle_small;
    image_data touch_controller_shoulder_left;
    image_data touch_controller_shoulder_right;

    //Level-specific images.
    std::vector<image_data> background_layer_images;
    image_data background_static;
    image_data tiles;
    image_data tiles_background;
    image_data water_tiles;
};

#endif
