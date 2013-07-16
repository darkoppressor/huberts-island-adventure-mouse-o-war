/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "sound.h"
#include "holidays.h"
#include "world.h"

using namespace std;

void Sound::load_sounds_global(){
    camera_unlock.load_sound("data/sounds/camera_unlock.wav");
    camera_lock.load_sound("data/sounds/camera_lock.wav");
    camera_move.load_sound("data/sounds/camera_move.wav");
    achievement_earned.load_sound("data/sounds/achievement_earned.wav");
    pause.load_sound("data/sounds/pause.wav");
    unpause.load_sound("data/sounds/unpause.wav");
    map_open.load_sound("data/sounds/map_open.wav");
    map_close.load_sound("data/sounds/map_close.wav");
    inventory_open.load_sound("data/sounds/inventory_open.wav");
    inventory_close.load_sound("data/sounds/inventory_close.wav");
    game_start_friday_13.load_sound("data/sounds/game_start_friday_13.ogg");
    game_start_full_moon.load_sound("data/sounds/game_start_full_moon.ogg");
    survival_escape.load_sound("data/sounds/survival_escape.ogg");

    item_collect_leaf.load_sound("data/sounds/item_collect_leaf.wav");
    item_collect_cheese.load_sound("data/sounds/item_collect_cheese.wav");
    item_collect_ammo.load_sound("data/sounds/item_collect_ammo.wav");
    item_checkpoint.load_sound("data/sounds/item_checkpoint.wav");
    item_collect_key.load_sound("data/sounds/item_collect_key.wav");
    item_collect_swimming_gear.load_sound("data/sounds/item_collect_swimming_gear.wav");
    item_collect_ammo_barrel.load_sound("data/sounds/item_collect_ammo_barrel.wav");
    item_collect_candy.load_sound("data/sounds/item_collect_candy.wav");
    item_collect_j_balloon.load_sound("data/sounds/item_collect_j_balloon.wav");
    item_collect_sink.load_sound("data/sounds/item_collect_sink.wav");
    item_collect_towel.load_sound("data/sounds/item_collect_towel.wav");
    item_collect_suit_deadly_water.load_sound("data/sounds/item_collect_suit_deadly_water.wav");
    item_collect_suit_sharp.load_sound("data/sounds/item_collect_suit_sharp.wav");
    item_collect_suit_banana.load_sound("data/sounds/item_collect_suit_banana.wav");
    item_collect_shot_homing.load_sound("data/sounds/item_collect_shot_homing.wav");
    item_collect_translator.load_sound("data/sounds/item_collect_translator.wav");
    item_collect_j_wing.load_sound("data/sounds/item_collect_j_wing.wav");

    inventory_grab.load_sound("data/sounds/inventory_grab.wav");
    inventory_drop.load_sound("data/sounds/inventory_drop.wav");
    inventory_trade.load_sound("data/sounds/inventory_trade.wav");

    trigger_click.load_sound("data/sounds/trigger_click.wav");
    booster.load_sound("data/sounds/booster.wav");
    spring.load_sound("data/sounds/spring.wav");
    door_open.load_sound("data/sounds/door_open.wav");
    door_close.load_sound("data/sounds/door_close.wav");
    door_unlock.load_sound("data/sounds/door_unlock.wav");
    door_standard_open.load_sound("data/sounds/door_standard_open.wav");
    weather_machine_on.load_sound("data/sounds/weather_machine_on.wav");
    weather_machine_off.load_sound("data/sounds/weather_machine_off.wav");
    party_ball_explode.load_sound("data/sounds/party_ball_explode.wav");
    goal_reached.load_sound("data/sounds/goal_reached.ogg");
    balloonin.load_sound("data/sounds/balloonin.ogg");
    read_sign.load_sound("data/sounds/read_sign.wav");
    reflect_banana.load_sound("data/sounds/reflect_banana.wav");

    trap_spikes.load_sound("data/sounds/trap_spikes.wav");
    trap_electric.load_sound("data/sounds/trap_electric.wav");
    trap_branch.load_sound("data/sounds/trap_branch.wav");
    trap_stoneslab.load_sound("data/sounds/trap_stoneslab.wav");

    splash_in.load_sound("data/sounds/splash_in.wav");
    splash_out.load_sound("data/sounds/splash_out.wav");
    swim.load_sound("data/sounds/swim.wav");
    bubble_form.load_sound("data/sounds/bubble_form.wav");
    bubble_pop.load_sound("data/sounds/bubble_pop.wav");
    firework_explode_1.load_sound("data/sounds/firework_explode_1.wav");
    firework_explode_2.load_sound("data/sounds/firework_explode_2.wav");
    firework_explode_3.load_sound("data/sounds/firework_explode_3.wav");
    firework_shoot_1.load_sound("data/sounds/firework_shoot_1.wav");
    firework_shoot_2.load_sound("data/sounds/firework_shoot_2.wav");
    firework_shoot_3.load_sound("data/sounds/firework_shoot_3.wav");

    npc_die.load_sound("data/sounds/npc_die.wav");
    boss_hurt.load_sound("data/sounds/boss_hurt.wav");
    boss_die.load_sound("data/sounds/boss_die.wav");
    npc_cow_moo.load_sound("data/sounds/npc_cow_moo.wav");
    npc_cow_moo2.load_sound("data/sounds/npc_cow_moo2.wav");
    npc_bee_dive.load_sound("data/sounds/npc_bee_dive.wav");
    npc_cat_pounce.load_sound("data/sounds/npc_cat_pounce.wav");
    npc_eagle_dive.load_sound("data/sounds/npc_eagle_dive.wav");
    npc_jump.load_sound("data/sounds/npc_jump.wav");
    npc_ram_charge.load_sound("data/sounds/npc_ram_charge.wav");
    npc_scarecrow_whack.load_sound("data/sounds/npc_scarecrow_whack.wav");
    npc_sword_slash.load_sound("data/sounds/npc_sword_slash.wav");
    npc_ghost_cow_moo.load_sound("data/sounds/npc_ghost_cow_moo.wav");
    npc_mummy_cow_moo.load_sound("data/sounds/npc_mummy_cow_moo.wav");
    npc_zombie_cow_moo.load_sound("data/sounds/npc_zombie_cow_moo.wav");

    shot_arrow.load_sound("data/sounds/shot_arrow.wav");
    shot_ball.load_sound("data/sounds/shot_ball.wav");
    shot_boss_haunted_house.load_sound("data/sounds/shot_boss_haunted_house.wav");
    shot_boss_lighthouse.load_sound("data/sounds/shot_boss_lighthouse.wav");
    shot_boss_pyramid.load_sound("data/sounds/shot_boss_pyramid.wav");
    shot_boss_volcano.load_sound("data/sounds/shot_boss_volcano.wav");
    shot_fireball.load_sound("data/sounds/shot_fireball.wav");
    shot_fruit.load_sound("data/sounds/shot_fruit.wav");
    shot_homing_missile.load_sound("data/sounds/shot_homing_missile.wav");
    shot_laser.load_sound("data/sounds/shot_laser.wav");
    shot_poison.load_sound("data/sounds/shot_poison.wav");
    shot_snowball.load_sound("data/sounds/shot_snowball.wav");

    shot_impact_arrow.load_sound("data/sounds/shot_impact_arrow.wav");
    shot_impact_ball.load_sound("data/sounds/shot_impact_ball.wav");
    shot_impact_boss_haunted_house.load_sound("data/sounds/shot_impact_boss_haunted_house.wav");
    shot_impact_boss_lighthouse.load_sound("data/sounds/shot_impact_boss_lighthouse.wav");
    shot_impact_boss_pyramid.load_sound("data/sounds/shot_impact_boss_pyramid.wav");
    shot_impact_boss_volcano.load_sound("data/sounds/shot_impact_boss_volcano.wav");
    shot_impact_fireball.load_sound("data/sounds/shot_impact_fireball.wav");
    shot_impact_fruit.load_sound("data/sounds/shot_impact_fruit.wav");
    shot_impact_homing_missile.load_sound("data/sounds/shot_impact_homing_missile.wav");
    shot_impact_laser.load_sound("data/sounds/shot_impact_laser.wav");
    shot_impact_poison.load_sound("data/sounds/shot_impact_poison.wav");
    shot_impact_snowball.load_sound("data/sounds/shot_impact_snowball.wav");

    player_shoot.load_sound("data/sounds/player_shoot.wav");
    player_shot_impact.load_sound("data/sounds/player_shot_impact.wav");
    player_jump.load_sound("data/sounds/player_jump.wav");
    player_jump_j_balloon.load_sound("data/sounds/player_jump_j_balloon.wav");
    player_fear.load_sound("data/sounds/player_fear.wav");
    player_footstep.load_sound("data/sounds/player_footstep.wav");
    player_footstep2.load_sound("data/sounds/player_footstep2.wav");
    player_footstep_water.load_sound("data/sounds/player_footstep_water.wav");
    player_footstep2_water.load_sound("data/sounds/player_footstep2_water.wav");
    player_death.load_sound("data/sounds/player_death.wav");
    player_death2.load_sound("data/sounds/player_death2.wav");
    player_change_shot.load_sound("data/sounds/player_change_shot.wav");
    player_start_slide.load_sound("data/sounds/player_start_slide.wav");
    player_look_start.load_sound("data/sounds/player_look_start.wav");
    player_look_stop.load_sound("data/sounds/player_look_stop.wav");
    player_no_ammo.load_sound("data/sounds/player_no_ammo.wav");
    player_bubble_form.load_sound("data/sounds/player_bubble_form.wav");
    player_bubble_pop.load_sound("data/sounds/player_bubble_pop.wav");
    player_respawn.load_sound("data/sounds/player_respawn.wav");

    character_sketch_footstep.load_sound("data/sounds/character_sketch_footstep.wav");
    character_sketch_footstep2.load_sound("data/sounds/character_sketch_footstep2.wav");
    character_sketch_jump.load_sound("data/sounds/character_sketch_jump.wav");
    character_sketch_start_slide.load_sound("data/sounds/character_sketch_start_slide.wav");
    character_slime_o_footstep.load_sound("data/sounds/character_slime_o_footstep.wav");
    character_slime_o_footstep2.load_sound("data/sounds/character_slime_o_footstep2.wav");
    character_slime_o_jump.load_sound("data/sounds/character_slime_o_jump.wav");
    character_slime_o_start_slide.load_sound("data/sounds/character_slime_o_start_slide.wav");
    character_penny_footstep.load_sound("data/sounds/character_penny_footstep.wav");
    character_penny_footstep2.load_sound("data/sounds/character_penny_footstep2.wav");
    character_penny_jump.load_sound("data/sounds/character_penny_jump.wav");
    character_penny_start_slide.load_sound("data/sounds/character_penny_start_slide.wav");

    string holiday_name=return_holiday_name(holiday);

    for(int i=0;i<12;i++){
        button_mouse_over.push_back(sound_data());
        msg="data/sounds/buttons/";
        msg+=holiday_name;
        msg+="/mouse_over/";
        ss.clear();ss.str("");ss<<i+1;msg+=ss.str();
        msg+=".ogg";
        button_mouse_over[i].load_sound(msg.c_str());
    }
    for(int i=0;i<12;i++){
        button_event_fire.push_back(sound_data());
        msg="data/sounds/buttons/";
        msg+=holiday_name;
        msg+="/event_fire/";
        ss.clear();ss.str("");ss<<i+1;msg+=ss.str();
        msg+=".ogg";
        button_event_fire[i].load_sound(msg.c_str());
    }
}

void Sound::unload_sounds_global(){
    camera_unlock.unload_sound();
    camera_lock.unload_sound();
    camera_move.unload_sound();
    achievement_earned.unload_sound();
    pause.unload_sound();
    unpause.unload_sound();
    map_open.unload_sound();
    map_close.unload_sound();
    inventory_open.unload_sound();
    inventory_close.unload_sound();
    game_start_friday_13.unload_sound();
    game_start_full_moon.unload_sound();
    survival_escape.unload_sound();

    item_collect_leaf.unload_sound();
    item_collect_cheese.unload_sound();
    item_collect_ammo.unload_sound();
    item_checkpoint.unload_sound();
    item_collect_key.unload_sound();
    item_collect_swimming_gear.unload_sound();
    item_collect_ammo_barrel.unload_sound();
    item_collect_candy.unload_sound();
    item_collect_j_balloon.unload_sound();
    item_collect_sink.unload_sound();
    item_collect_towel.unload_sound();
    item_collect_suit_deadly_water.unload_sound();
    item_collect_suit_sharp.unload_sound();
    item_collect_suit_banana.unload_sound();
    item_collect_shot_homing.unload_sound();
    item_collect_translator.unload_sound();
    item_collect_j_wing.unload_sound();

    inventory_grab.unload_sound();
    inventory_drop.unload_sound();
    inventory_trade.unload_sound();

    trigger_click.unload_sound();
    booster.unload_sound();
    spring.unload_sound();
    door_open.unload_sound();
    door_close.unload_sound();
    door_unlock.unload_sound();
    door_standard_open.unload_sound();
    weather_machine_on.unload_sound();
    weather_machine_off.unload_sound();
    party_ball_explode.unload_sound();
    goal_reached.unload_sound();
    balloonin.unload_sound();
    read_sign.unload_sound();
    reflect_banana.unload_sound();

    trap_spikes.unload_sound();
    trap_electric.unload_sound();
    trap_branch.unload_sound();
    trap_stoneslab.unload_sound();

    splash_in.unload_sound();
    splash_out.unload_sound();
    swim.unload_sound();
    bubble_form.unload_sound();
    bubble_pop.unload_sound();
    firework_explode_1.unload_sound();
    firework_explode_2.unload_sound();
    firework_explode_3.unload_sound();
    firework_shoot_1.unload_sound();
    firework_shoot_2.unload_sound();
    firework_shoot_3.unload_sound();

    npc_die.unload_sound();
    boss_hurt.unload_sound();
    boss_die.unload_sound();
    npc_cow_moo.unload_sound();
    npc_cow_moo2.unload_sound();
    npc_bee_dive.unload_sound();
    npc_cat_pounce.unload_sound();
    npc_eagle_dive.unload_sound();
    npc_jump.unload_sound();
    npc_ram_charge.unload_sound();
    npc_scarecrow_whack.unload_sound();
    npc_sword_slash.unload_sound();
    npc_ghost_cow_moo.unload_sound();
    npc_mummy_cow_moo.unload_sound();
    npc_zombie_cow_moo.unload_sound();

    shot_arrow.unload_sound();
    shot_ball.unload_sound();
    shot_boss_haunted_house.unload_sound();
    shot_boss_lighthouse.unload_sound();
    shot_boss_pyramid.unload_sound();
    shot_boss_volcano.unload_sound();
    shot_fireball.unload_sound();
    shot_fruit.unload_sound();
    shot_homing_missile.unload_sound();
    shot_laser.unload_sound();
    shot_poison.unload_sound();
    shot_snowball.unload_sound();

    shot_impact_arrow.unload_sound();
    shot_impact_ball.unload_sound();
    shot_impact_boss_haunted_house.unload_sound();
    shot_impact_boss_lighthouse.unload_sound();
    shot_impact_boss_pyramid.unload_sound();
    shot_impact_boss_volcano.unload_sound();
    shot_impact_fireball.unload_sound();
    shot_impact_fruit.unload_sound();
    shot_impact_homing_missile.unload_sound();
    shot_impact_laser.unload_sound();
    shot_impact_poison.unload_sound();
    shot_impact_snowball.unload_sound();

    player_shoot.unload_sound();
    player_shot_impact.unload_sound();
    player_jump.unload_sound();
    player_jump_j_balloon.unload_sound();
    player_fear.unload_sound();
    player_footstep.unload_sound();
    player_footstep2.unload_sound();
    player_footstep_water.unload_sound();
    player_footstep2_water.unload_sound();
    player_death.unload_sound();
    player_death2.unload_sound();
    player_change_shot.unload_sound();
    player_start_slide.unload_sound();
    player_look_start.unload_sound();
    player_look_stop.unload_sound();
    player_no_ammo.unload_sound();
    player_bubble_form.unload_sound();
    player_bubble_pop.unload_sound();
    player_respawn.unload_sound();

    character_sketch_footstep.unload_sound();
    character_sketch_footstep2.unload_sound();
    character_sketch_jump.unload_sound();
    character_sketch_start_slide.unload_sound();
    character_slime_o_footstep.unload_sound();
    character_slime_o_footstep2.unload_sound();
    character_slime_o_jump.unload_sound();
    character_slime_o_start_slide.unload_sound();
    character_penny_footstep.unload_sound();
    character_penny_footstep2.unload_sound();
    character_penny_jump.unload_sound();
    character_penny_start_slide.unload_sound();

    for(int i=0;i<12;i++){
        button_mouse_over[i].unload_sound();
    }
    for(int i=0;i<12;i++){
        button_event_fire[i].unload_sound();
    }
}
