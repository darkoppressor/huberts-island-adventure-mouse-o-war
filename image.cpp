/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "image.h"
#include "world.h"
#include "render.h"
#include "holidays.h"

using namespace std;

void Image::load_images_global(){
    sprite_sheet_player.load_image("data/images/sprite_sheet_player.png");
    sprite_sheet_player_worldmap.load_image("data/images/sprite_sheet_player_worldmap.png");
    sprite_sheet_character_slime_o.load_image("data/images/sprite_sheet_character_slime_o.png");
    sprite_sheet_character_worldmap_slime_o.load_image("data/images/sprite_sheet_character_worldmap_slime_o.png");
    sprite_sheet_character_sketch.load_image("data/images/sprite_sheet_character_sketch.png");
    sprite_sheet_character_worldmap_sketch.load_image("data/images/sprite_sheet_character_worldmap_sketch.png");
    sprite_sheet_character_penny.load_image("data/images/sprite_sheet_character_penny.png");
    sprite_sheet_character_worldmap_penny.load_image("data/images/sprite_sheet_character_worldmap_penny.png");
    sprite_sheet_items.load_image("data/images/sprite_sheet_items.png");
    sprite_sheet_level_items.load_image("data/images/sprite_sheet_level_items.png");
    sprite_sheet_sign.load_image("data/images/sprite_sheet_sign.png");
    sprite_sheet_spring.load_image("data/images/sprite_sheet_spring.png");
    sprite_sheet_booster.load_image("data/images/sprite_sheet_booster.png");
    overlay_night.load_image("data/images/overlay_night.png");
    moon.load_image("data/images/moon.png");
    sun.load_image("data/images/sun.png");
    balloon.load_image("data/images/balloon.png");
    tophat.load_image("data/images/tophat.png");
    cursor.load_image("data/images/cursor.png");
    logo_cheese_and_bacon.load_image("data/images/logo_cheese_and_bacon.png");
    logo_hubert.load_image("data/images/logo_hubert.png");
    loading_screen_main.load_image("data/images/loading_screen_main.png");
    level_title_background_0.load_image("data/images/level_title_background_0.png");
    level_title_background_1.load_image("data/images/level_title_background_1.png");
    level_title_background_2.load_image("data/images/level_title_background_2.png");
    menu_background_0.load_image("data/images/menu_background/"+return_holiday_name(holiday)+"_0.png");
    checkbox_unchecked.load_image("data/images/checkbox_unchecked.png");
    checkbox_checked.load_image("data/images/checkbox_checked.png");
    ammo_box.load_image("data/images/ammo_box.png");
    character_box_hubert.load_image("data/images/character_box_hubert.png");
    character_box_slime_o.load_image("data/images/character_box_slime_o.png");
    character_box_sketch.load_image("data/images/character_box_sketch.png");
    character_box_penny.load_image("data/images/character_box_penny.png");
    shot_types.load_image("data/images/shot_types.png");
    survival_spawn.load_image("data/images/survival_spawn.png");
    powerup_jump.load_image("data/images/powerup_jump.png");
    sprite_sheet_npc_slime.load_image("data/images/sprite_sheet_npc_slime.png");
    sprite_sheet_npc_cow.load_image("data/images/sprite_sheet_npc_cow.png");
    sprite_sheet_npc_bee.load_image("data/images/sprite_sheet_npc_bee.png");
    sprite_sheet_shot.load_image("data/images/sprite_sheet_shot.png");
    sprite_sheet_shot_player_homing.load_image("data/images/sprite_sheet_shot_player_homing.png");
    sprite_sheet_shot_player_splosion.load_image("data/images/sprite_sheet_shot_player_splosion.png");
    sprite_sheet_shot_fruit.load_image("data/images/sprite_sheet_shot_fruit.png");
    sprite_sheet_shot_laser.load_image("data/images/sprite_sheet_shot_laser.png");
    sprite_sheet_shot_ball.load_image("data/images/sprite_sheet_shot_ball.png");
    sprite_sheet_shot_arrow.load_image("data/images/sprite_sheet_shot_arrow.png");
    sprite_sheet_shot_snowball.load_image("data/images/sprite_sheet_shot_snowball.png");
    sprite_sheet_shot_fireball.load_image("data/images/sprite_sheet_shot_fireball.png");
    sprite_sheet_shot_poison.load_image("data/images/sprite_sheet_shot_poison.png");
    sprite_sheet_shot_homing_missile.load_image("data/images/sprite_sheet_shot_homing_missile.png");
    sprite_sheet_shot_boss_defeated.load_image("data/images/sprite_sheet_shot_boss_defeated.png");
    sprite_sheet_shot_boss_haunted_house.load_image("data/images/sprite_sheet_shot_boss_haunted_house.png");
    sprite_sheet_shot_boss_volcano.load_image("data/images/sprite_sheet_shot_boss_volcano.png");
    sprite_sheet_shot_boss_pyramid.load_image("data/images/sprite_sheet_shot_boss_pyramid.png");
    sprite_sheet_shot_boss_lighthouse.load_image("data/images/sprite_sheet_shot_boss_lighthouse.png");
    sprite_sheet_moving_platform.load_image("data/images/sprite_sheet_moving_platform.png");
    sprite_sheet_triggers.load_image("data/images/sprite_sheet_triggers.png");
    sprite_sheet_doors.load_image("data/images/sprite_sheet_doors.png");
    sprite_sheet_door_standard.load_image("data/images/sprite_sheet_door_standard.png");
    sprite_sheet_trap_fire.load_image("data/images/sprite_sheet_trap_fire.png");
    sprite_sheet_npc_replicator.load_image("data/images/sprite_sheet_npc_replicator.png");
    sprite_sheet_npc_fish.load_image("data/images/sprite_sheet_npc_fish.png");
    sprite_sheet_npc_monkey.load_image("data/images/sprite_sheet_npc_monkey.png");
    sprite_sheet_npc_laser_spinner.load_image("data/images/sprite_sheet_npc_laser_spinner.png");
    sprite_sheet_npc_penguin.load_image("data/images/sprite_sheet_npc_penguin.png");
    sprite_sheet_npc_fish_on_ice.load_image("data/images/sprite_sheet_npc_fish_on_ice.png");
    sprite_sheet_npc_penguin_harmless.load_image("data/images/sprite_sheet_npc_penguin_harmless.png");
    sprite_sheet_npc_fish_on_ice_harmless.load_image("data/images/sprite_sheet_npc_fish_on_ice_harmless.png");
    sprite_sheet_npc_sword_knight.load_image("data/images/sprite_sheet_npc_sword_knight.png");
    sprite_sheet_npc_ball_knight.load_image("data/images/sprite_sheet_npc_ball_knight.png");
    sprite_sheet_npc_archer.load_image("data/images/sprite_sheet_npc_archer.png");
    sprite_sheet_npc_scarecrow.load_image("data/images/sprite_sheet_npc_scarecrow.png");
    sprite_sheet_npc_pumpkin.load_image("data/images/sprite_sheet_npc_pumpkin.png");
    sprite_sheet_npc_zombie_cow.load_image("data/images/sprite_sheet_npc_zombie_cow.png");
    sprite_sheet_npc_mummy_cow.load_image("data/images/sprite_sheet_npc_mummy_cow.png");
    sprite_sheet_npc_ghost.load_image("data/images/sprite_sheet_npc_ghost.png");
    sprite_sheet_npc_spider.load_image("data/images/sprite_sheet_npc_spider.png");
    sprite_sheet_npc_cat.load_image("data/images/sprite_sheet_npc_cat.png");
    sprite_sheet_npc_spider_walking.load_image("data/images/sprite_sheet_npc_spider_walking.png");
    sprite_sheet_npc_ram.load_image("data/images/sprite_sheet_npc_ram.png");
    sprite_sheet_npc_eagle.load_image("data/images/sprite_sheet_npc_eagle.png");
    sprite_sheet_npc_snowman.load_image("data/images/sprite_sheet_npc_snowman.png");
    sprite_sheet_npc_fireball.load_image("data/images/sprite_sheet_npc_fireball.png");
    sprite_sheet_npc_dragon_whelp.load_image("data/images/sprite_sheet_npc_dragon_whelp.png");
    sprite_sheet_npc_fireball_shooter_left.load_image("data/images/sprite_sheet_npc_fireball_shooter_left.png");
    sprite_sheet_npc_fireball_shooter_right.load_image("data/images/sprite_sheet_npc_fireball_shooter_right.png");
    sprite_sheet_npc_fireball_shooter_up.load_image("data/images/sprite_sheet_npc_fireball_shooter_up.png");
    sprite_sheet_npc_fireball_shooter_down.load_image("data/images/sprite_sheet_npc_fireball_shooter_down.png");
    sprite_sheet_npc_dust_devil.load_image("data/images/sprite_sheet_npc_dust_devil.png");
    sprite_sheet_npc_scorpion.load_image("data/images/sprite_sheet_npc_scorpion.png");
    sprite_sheet_npc_sphinx.load_image("data/images/sprite_sheet_npc_sphinx.png");
    sprite_sheet_npc_shark.load_image("data/images/sprite_sheet_npc_shark.png");
    sprite_sheet_npc_henchmouse.load_image("data/images/sprite_sheet_npc_henchmouse.png");
    sprite_sheet_npc_mechsuit_mouse.load_image("data/images/sprite_sheet_npc_mechsuit_mouse.png");
    sprite_sheet_npc_sally.load_image("data/images/sprite_sheet_npc_sally.png");
    sprite_sheet_npc_bouncing_ball.load_image("data/images/sprite_sheet_npc_bouncing_ball.png");
    sprite_sheet_npc_sea_turtle.load_image("data/images/sprite_sheet_npc_sea_turtle.png");
    sprite_sheet_npc_starfish.load_image("data/images/sprite_sheet_npc_starfish.png");
    sprite_sheet_npc_boss_castle.load_image("data/images/sprite_sheet_npc_boss_castle.png");
    sprite_sheet_npc_boss_haunted_house.load_image("data/images/sprite_sheet_npc_boss_haunted_house.png");
    sprite_sheet_npc_boss_volcano.load_image("data/images/sprite_sheet_npc_boss_volcano.png");
    sprite_sheet_npc_boss_pyramid.load_image("data/images/sprite_sheet_npc_boss_pyramid.png");
    sprite_sheet_npc_boss_lighthouse.load_image("data/images/sprite_sheet_npc_boss_lighthouse.png");
    sprite_sheet_trap_spikes.load_image("data/images/sprite_sheet_trap_spikes.png");
    sprite_sheet_trap_icicle.load_image("data/images/sprite_sheet_trap_icicle.png");
    sprite_sheet_trap_branch.load_image("data/images/sprite_sheet_trap_branch.png");
    sprite_sheet_trap_electric.load_image("data/images/sprite_sheet_trap_electric.png");
    sprite_sheet_trap_stoneslab.load_image("data/images/sprite_sheet_trap_stoneslab.png");
    sprite_sheet_trap_buzzsaw.load_image("data/images/sprite_sheet_trap_buzzsaw.png");
    sprite_sheet_trap_spikes_up.load_image("data/images/sprite_sheet_trap_spikes_up.png");
    sprite_sheet_trap_spikes_down.load_image("data/images/sprite_sheet_trap_spikes_down.png");
    sprite_sheet_trap_spikes_right.load_image("data/images/sprite_sheet_trap_spikes_right.png");
    sprite_sheet_trap_spikes_left.load_image("data/images/sprite_sheet_trap_spikes_left.png");
    sprite_sheet_trap_boss_castle.load_image("data/images/sprite_sheet_trap_boss_castle.png");
    sprite_sheet_trap_boss_pyramid.load_image("data/images/sprite_sheet_trap_boss_pyramid.png");
    sprite_sheet_trap_seaweed.load_image("data/images/sprite_sheet_trap_seaweed.png");

    for(int i=0;i<player.menu_backgrounds.size();i++){
        menu_background_images.push_back(image_data());

        string holiday_name=return_holiday_name(holiday);
        string file_name="";
        file_name="data/images/menu_background/";
        file_name+=holiday_name;
        file_name+="_";
        ss.clear();ss.str("");ss<<i+1;file_name+=ss.str();
        file_name+=".png";

        menu_background_images[i].load_image(file_name);
    }

    for(int i=0;i<player.overlay_snow.size();i++){
        overlay_snow_images.push_back(image_data());

        string file_name="data/images/overlay_snow_";
        ss.clear();ss.str("");ss<<i+1;file_name+=ss.str();
        file_name+=".png";
        overlay_snow_images[i].load_image(file_name);
    }
    for(int i=0;i<player.overlay_rain.size();i++){
        overlay_rain_images.push_back(image_data());

        string file_name="data/images/overlay_rain_";
        ss.clear();ss.str("");ss<<i+1;file_name+=ss.str();
        file_name+=".png";
        overlay_rain_images[i].load_image(file_name);
    }
}

void Image::unload_images_global(){
    sprite_sheet_player.unload_image();
    sprite_sheet_player_worldmap.unload_image();
    sprite_sheet_character_slime_o.unload_image();
    sprite_sheet_character_worldmap_slime_o.unload_image();
    sprite_sheet_character_sketch.unload_image();
    sprite_sheet_character_worldmap_sketch.unload_image();
    sprite_sheet_character_penny.unload_image();
    sprite_sheet_character_worldmap_penny.unload_image();
    sprite_sheet_items.unload_image();
    sprite_sheet_level_items.unload_image();
    sprite_sheet_sign.unload_image();
    sprite_sheet_spring.unload_image();
    sprite_sheet_booster.unload_image();
    overlay_night.unload_image();
    moon.unload_image();
    sun.unload_image();
    balloon.unload_image();
    tophat.unload_image();
    cursor.unload_image();
    logo_cheese_and_bacon.unload_image();
    logo_hubert.unload_image();
    loading_screen_main.unload_image();
    level_title_background_0.unload_image();
    level_title_background_1.unload_image();
    level_title_background_2.unload_image();
    menu_background_0.unload_image();
    checkbox_unchecked.unload_image();
    checkbox_checked.unload_image();
    ammo_box.unload_image();
    character_box_hubert.unload_image();
    character_box_slime_o.unload_image();
    character_box_sketch.unload_image();
    character_box_penny.unload_image();
    shot_types.unload_image();
    survival_spawn.unload_image();
    powerup_jump.unload_image();
    sprite_sheet_npc_slime.unload_image();
    sprite_sheet_npc_cow.unload_image();
    sprite_sheet_npc_bee.unload_image();
    sprite_sheet_shot.unload_image();
    sprite_sheet_shot_player_homing.unload_image();
    sprite_sheet_shot_player_splosion.unload_image();
    sprite_sheet_shot_fruit.unload_image();
    sprite_sheet_shot_laser.unload_image();
    sprite_sheet_shot_ball.unload_image();
    sprite_sheet_shot_arrow.unload_image();
    sprite_sheet_shot_snowball.unload_image();
    sprite_sheet_shot_fireball.unload_image();
    sprite_sheet_shot_poison.unload_image();
    sprite_sheet_shot_homing_missile.unload_image();
    sprite_sheet_shot_boss_defeated.unload_image();
    sprite_sheet_shot_boss_haunted_house.unload_image();
    sprite_sheet_shot_boss_volcano.unload_image();
    sprite_sheet_shot_boss_pyramid.unload_image();
    sprite_sheet_shot_boss_lighthouse.unload_image();
    sprite_sheet_moving_platform.unload_image();
    sprite_sheet_triggers.unload_image();
    sprite_sheet_doors.unload_image();
    sprite_sheet_door_standard.unload_image();
    sprite_sheet_trap_fire.unload_image();
    sprite_sheet_npc_replicator.unload_image();
    sprite_sheet_npc_fish.unload_image();
    sprite_sheet_npc_monkey.unload_image();
    sprite_sheet_npc_laser_spinner.unload_image();
    sprite_sheet_npc_penguin.unload_image();
    sprite_sheet_npc_fish_on_ice.unload_image();
    sprite_sheet_npc_penguin_harmless.unload_image();
    sprite_sheet_npc_fish_on_ice_harmless.unload_image();
    sprite_sheet_npc_sword_knight.unload_image();
    sprite_sheet_npc_ball_knight.unload_image();
    sprite_sheet_npc_archer.unload_image();
    sprite_sheet_npc_scarecrow.unload_image();
    sprite_sheet_npc_pumpkin.unload_image();
    sprite_sheet_npc_zombie_cow.unload_image();
    sprite_sheet_npc_mummy_cow.unload_image();
    sprite_sheet_npc_ghost.unload_image();
    sprite_sheet_npc_spider.unload_image();
    sprite_sheet_npc_cat.unload_image();
    sprite_sheet_npc_spider_walking.unload_image();
    sprite_sheet_npc_ram.unload_image();
    sprite_sheet_npc_eagle.unload_image();
    sprite_sheet_npc_snowman.unload_image();
    sprite_sheet_npc_fireball.unload_image();
    sprite_sheet_npc_dragon_whelp.unload_image();
    sprite_sheet_npc_fireball_shooter_left.unload_image();
    sprite_sheet_npc_fireball_shooter_right.unload_image();
    sprite_sheet_npc_fireball_shooter_up.unload_image();
    sprite_sheet_npc_fireball_shooter_down.unload_image();
    sprite_sheet_npc_dust_devil.unload_image();
    sprite_sheet_npc_scorpion.unload_image();
    sprite_sheet_npc_sphinx.unload_image();
    sprite_sheet_npc_shark.unload_image();
    sprite_sheet_npc_henchmouse.unload_image();
    sprite_sheet_npc_mechsuit_mouse.unload_image();
    sprite_sheet_npc_sally.unload_image();
    sprite_sheet_npc_bouncing_ball.unload_image();
    sprite_sheet_npc_sea_turtle.unload_image();
    sprite_sheet_npc_starfish.unload_image();
    sprite_sheet_npc_boss_castle.unload_image();
    sprite_sheet_npc_boss_haunted_house.unload_image();
    sprite_sheet_npc_boss_volcano.unload_image();
    sprite_sheet_npc_boss_pyramid.unload_image();
    sprite_sheet_npc_boss_lighthouse.unload_image();
    sprite_sheet_trap_spikes.unload_image();
    sprite_sheet_trap_icicle.unload_image();
    sprite_sheet_trap_branch.unload_image();
    sprite_sheet_trap_electric.unload_image();
    sprite_sheet_trap_stoneslab.unload_image();
    sprite_sheet_trap_buzzsaw.unload_image();
    sprite_sheet_trap_spikes_up.unload_image();
    sprite_sheet_trap_spikes_down.unload_image();
    sprite_sheet_trap_spikes_right.unload_image();
    sprite_sheet_trap_spikes_left.unload_image();
    sprite_sheet_trap_boss_castle.unload_image();
    sprite_sheet_trap_boss_pyramid.unload_image();
    sprite_sheet_trap_seaweed.unload_image();

    for(int i=0;i<menu_background_images.size();i++){
        menu_background_images[i].unload_image();
    }
    menu_background_images.clear();

    for(int i=0;i<overlay_snow_images.size();i++){
        overlay_snow_images[i].unload_image();
    }
    overlay_snow_images.clear();
    for(int i=0;i<overlay_rain_images.size();i++){
        overlay_rain_images[i].unload_image();
    }
    overlay_rain_images.clear();
}

void Image::load_images_level(short level_to_load,short sub_level_to_load,string current_level){
    if(player.game_mode==GAME_MODE_SP_ADVENTURE || player.game_mode==GAME_MODE_MP_ADVENTURE){
        load_images_level_adventure(level_to_load,sub_level_to_load);
    }
    else if(player.game_mode==GAME_MODE_SP_SURVIVAL){
        load_images_level_survival(current_level);
    }
    else if(player.game_mode==GAME_MODE_MP_SURVIVAL){
        load_images_level_survival(current_level);
    }
}

void Image::load_images_level_adventure(short level_to_load,short sub_level_to_load){
    //Create a string to hold the current level number.
    string current_level="";
    ss.clear();ss.str("");ss<<level_to_load;current_level=ss.str();

    //Create a string to hold the current sub level number.
    string current_sub_level="";
    ss.clear();ss.str("");ss<<sub_level_to_load;current_sub_level=ss.str();

    string string_level_to_load="";
    //If the current sub level is 0, meaning "load the main level."
    if(sub_level_to_load==0){
        string_level_to_load=current_level;
    }
    //If there is a current sub level to load instead of the main one.
    else{
        string_level_to_load=current_level+"/"+current_sub_level;
    }

    //Load the level-specific images.
    for(short i=0;i<level.background_layers.size();i++){
        background_layer_images.push_back(image_data());

        string image_number="";
        ss.clear();ss.str("");ss<<i;image_number=ss.str();

        background_layer_images[i].load_image("data/levels/"+string_level_to_load+"/background_layer_"+image_number+".png");
    }

    background_static.load_image("data/levels/"+string_level_to_load+"/background_static.png");

    tiles.load_image("data/levels/"+string_level_to_load+"/tiles.png");

    water_tiles.load_image("data/levels/"+string_level_to_load+"/water_tiles.png");

    //The world map does not use these, so they are not loaded if the current level is the world map.
    if(!player.is_level_worldmap(level_to_load)){
        tiles_background.load_image("data/levels/"+string_level_to_load+"/tiles_background.png");
    }
}

void Image::unload_images_level(short level_to_unload){
    //Free up the level-specific images.
    for(short i=0;i<background_layer_images.size();i++){
        background_layer_images[i].unload_image();
    }
    background_layer_images.clear();

    background_static.unload_image();

    tiles.unload_image();

    water_tiles.unload_image();

    //As long as we aren't unloading the world map, unload the non-world map images.
    if(!player.is_level_worldmap(level_to_unload)){
        tiles_background.unload_image();
    }
}

void Image::load_images_level_survival(std::string current_level){
    //Load the level-specific images.
    for(short i=0;i<level.background_layers.size();i++){
        background_layer_images.push_back(image_data());

        string image_number="";
        ss.clear();ss.str("");ss<<i;image_number=ss.str();

        background_layer_images[i].load_image("data/levels/survival/"+current_level+"/background_layer_"+image_number+".png");
    }

    background_static.load_image("data/levels/survival/"+current_level+"/background_static.png");

    tiles.load_image("data/levels/survival/"+current_level+"/tiles.png");

    water_tiles.load_image("data/levels/survival/"+current_level+"/water_tiles.png");

    tiles_background.load_image("data/levels/survival/"+current_level+"/tiles_background.png");
}
