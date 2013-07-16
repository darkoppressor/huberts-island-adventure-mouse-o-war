/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef sound_h
#define sound_h

#include "sound_data.h"

#include <vector>
#include <string>

class Sound{
    private:
    public:
    void load_sounds_global();
    void unload_sounds_global();

    //Global sounds.
    sound_data camera_unlock;
    sound_data camera_lock;
    sound_data camera_move;
    sound_data achievement_earned;
    sound_data pause;
    sound_data unpause;
    sound_data map_open;
    sound_data map_close;
    sound_data inventory_open;
    sound_data inventory_close;
    sound_data game_start_friday_13;
    sound_data game_start_full_moon;
    sound_data survival_escape;

    sound_data item_collect_leaf;
    sound_data item_collect_cheese;
    sound_data item_collect_ammo;
    sound_data item_checkpoint;
    sound_data item_collect_key;
    sound_data item_collect_swimming_gear;
    sound_data item_collect_ammo_barrel;
    sound_data item_collect_candy;
    sound_data item_collect_j_balloon;
    sound_data item_collect_sink;
    sound_data item_collect_towel;
    sound_data item_collect_suit_deadly_water;
    sound_data item_collect_suit_sharp;
    sound_data item_collect_suit_banana;
    sound_data item_collect_shot_homing;
    sound_data item_collect_translator;
    sound_data item_collect_j_wing;

    sound_data inventory_grab;
    sound_data inventory_drop;
    sound_data inventory_trade;

    sound_data trigger_click;
    sound_data booster;
    sound_data spring;
    sound_data door_open;
    sound_data door_close;
    sound_data door_unlock;
    sound_data door_standard_open;
    sound_data weather_machine_on;
    sound_data weather_machine_off;
    sound_data party_ball_explode;
    sound_data goal_reached;
    sound_data balloonin;
    sound_data read_sign;
    sound_data reflect_banana;

    sound_data trap_spikes;
    sound_data trap_electric;
    sound_data trap_branch;
    sound_data trap_stoneslab;

    sound_data splash_in;
    sound_data splash_out;
    sound_data swim;
    sound_data bubble_form;
    sound_data bubble_pop;
    sound_data firework_explode_1;
    sound_data firework_explode_2;
    sound_data firework_explode_3;
    sound_data firework_shoot_1;
    sound_data firework_shoot_2;
    sound_data firework_shoot_3;

    sound_data npc_die;
    sound_data boss_hurt;
    sound_data boss_die;
    sound_data npc_cow_moo;
    sound_data npc_cow_moo2;
    sound_data npc_bee_dive;
    sound_data npc_cat_pounce;
    sound_data npc_eagle_dive;
    sound_data npc_jump;
    sound_data npc_ram_charge;
    sound_data npc_scarecrow_whack;
    sound_data npc_sword_slash;
    sound_data npc_ghost_cow_moo;
    sound_data npc_mummy_cow_moo;
    sound_data npc_zombie_cow_moo;

    sound_data shot_arrow;
    sound_data shot_ball;
    sound_data shot_boss_haunted_house;
    sound_data shot_boss_lighthouse;
    sound_data shot_boss_pyramid;
    sound_data shot_boss_volcano;
    sound_data shot_fireball;
    sound_data shot_fruit;
    sound_data shot_homing_missile;
    sound_data shot_laser;
    sound_data shot_poison;
    sound_data shot_snowball;

    sound_data shot_impact_arrow;
    sound_data shot_impact_ball;
    sound_data shot_impact_boss_haunted_house;
    sound_data shot_impact_boss_lighthouse;
    sound_data shot_impact_boss_pyramid;
    sound_data shot_impact_boss_volcano;
    sound_data shot_impact_fireball;
    sound_data shot_impact_fruit;
    sound_data shot_impact_homing_missile;
    sound_data shot_impact_laser;
    sound_data shot_impact_poison;
    sound_data shot_impact_snowball;

    sound_data player_shoot;
    sound_data player_shot_impact;
    sound_data player_jump;
    sound_data player_jump_j_balloon;
    sound_data player_fear;
    sound_data player_footstep;
    sound_data player_footstep2;
    sound_data player_footstep_water;
    sound_data player_footstep2_water;
    sound_data player_death;
    sound_data player_death2;
    sound_data player_change_shot;
    sound_data player_start_slide;
    sound_data player_look_start;
    sound_data player_look_stop;
    sound_data player_no_ammo;
    sound_data player_bubble_form;
    sound_data player_bubble_pop;
    sound_data player_respawn;

    sound_data character_sketch_footstep;
    sound_data character_sketch_footstep2;
    sound_data character_sketch_jump;
    sound_data character_sketch_start_slide;
    sound_data character_slime_o_footstep;
    sound_data character_slime_o_footstep2;
    sound_data character_slime_o_jump;
    sound_data character_slime_o_start_slide;
    sound_data character_penny_footstep;
    sound_data character_penny_footstep2;
    sound_data character_penny_jump;
    sound_data character_penny_start_slide;

    std::vector<sound_data> button_mouse_over;
    std::vector<sound_data> button_event_fire;
};

#endif
