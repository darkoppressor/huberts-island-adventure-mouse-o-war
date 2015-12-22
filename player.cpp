/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "player.h"
#include "render.h"
#include "world.h"
#include "collision.h"
#include "options.h"
#include "quit.h"
#include "mirror.h"
#include "score.h"
#include "counters.h"
#include "sign_compare.h"
#include "holidays.h"
#include "time_string.h"
#include "message_log.h"
#include "version.h"
#include "bubble_mode.h"

#include <cmath>
#include <ctime>

using namespace std;

Player::Player(){
    timer_update_achievements.start();

    // I've disabled this to prevent periodic saving
    ///timer_save_data.start();

    touch_controls=false;

    reset();
}

void Player::reset(){
    inventory.clear();
    dragged_item.clear();

    option_fps=false;
    option_dev=false;

    #ifdef GAME_OS_ANDROID
        option_touch_controls=true;
    #else
        option_touch_controls=false;
    #endif

    option_effect_limit=5000;
    option_volume_global=1.0;
    option_mute_global=false;
    option_volume_sound=0.5;
    option_mute_sound=false;
    option_volume_music=0.15;
    option_mute_music=false;
    option_holiday_cheer=true;
    option_time_of_day=false;
    option_time_of_day_levels=false;
    option_difficulty=DIFFICULTY_NORMAL;
    option_hardware_cursor=false;
    option_hints=true;
    option_character=CHARACTER_HUBERT;

    option_fullscreen_mode="windowed";
    option_screen_width=DEFAULT_SCREEN_WIDTH;
    option_screen_height=DEFAULT_SCREEN_HEIGHT;
    option_fullscreen=false;
    option_display_number=-1;
    option_lighting_tile_size=24;
    option_npc_fade_adventure=30;
    option_npc_fade_survival=15;

    invulnerable=false;
    cheat_jump=false;
    cheat_noclip=false;
    show_tracers=false;
    show_paths=false;
    hide_gui=false;

    stat_enemies_stunned=0;
    stat_triggers_shot=0;
    stat_checkpoints_activated=0;
    stat_ammo_picked_up=0;
    stat_shots_fired=0;
    stat_deaths_enemies=0;
    stat_deaths_traps=0;
    stat_deaths_drowning=0;
    stat_levels_replayed=0;
    stat_total_jumps=0;
    stat_seconds_playing=0;
    stat_seconds_traveling=0;
    stat_seconds_menus=0;
    stat_levers_pulled=0;
    stat_seconds_riding_cows=0;
    stat_farthest_fall=0;
    stat_water_shot=0;
    stat_bosses_defeated=0;
    stat_items_moved=0;

    for(int i=ACHIEVEMENT_BEGIN;i<ACHIEVEMENT_END;i++){
        achievements[i]=false;
    }

    special_count_shots_this_level=0;
    special_count_kills_this_level=0;

    //Note that commands must be setup here in the same order as command identifiers are setup in the enum in enumerations.h.
    keys.clear();
    keys.push_back(Input_Data(SDL_SCANCODE_F10));
    keys.push_back(Input_Data(SDL_SCANCODE_I));
    keys.push_back(Input_Data(SDL_SCANCODE_M));
    keys.push_back(Input_Data(SDL_SCANCODE_F5));
    keys.push_back(Input_Data(SDL_SCANCODE_LEFT));
    keys.push_back(Input_Data(SDL_SCANCODE_UP));
    keys.push_back(Input_Data(SDL_SCANCODE_RIGHT));
    keys.push_back(Input_Data(SDL_SCANCODE_DOWN));
    keys.push_back(Input_Data(SDL_SCANCODE_L));
    keys.push_back(Input_Data(SDL_SCANCODE_RCTRL));
    keys.push_back(Input_Data(SDL_SCANCODE_SPACE));
    keys.push_back(Input_Data(SDL_SCANCODE_RSHIFT));
    keys.push_back(Input_Data(SDL_SCANCODE_PAUSE));
    keys.push_back(Input_Data(SDL_SCANCODE_SLASH));

    mp_keys.clear();
    for(int i=0;i<3;i++){
        mp_keys.push_back(vector<Input_Data>());

        mp_keys[mp_keys.size()-1].push_back(Input_Data());
        mp_keys[mp_keys.size()-1][mp_keys[mp_keys.size()-1].size()-1].set_joy_button(-1,6);
        mp_keys[mp_keys.size()-1].push_back(Input_Data());
        mp_keys[mp_keys.size()-1][mp_keys[mp_keys.size()-1].size()-1].set_joy_button(-1,4);
        mp_keys[mp_keys.size()-1].push_back(Input_Data());
        mp_keys[mp_keys.size()-1][mp_keys[mp_keys.size()-1].size()-1].set_joy_button(-1,9);
        mp_keys[mp_keys.size()-1].push_back(Input_Data());
        mp_keys[mp_keys.size()-1][mp_keys[mp_keys.size()-1].size()-1].set_joy_axis(-1,2,0);
        mp_keys[mp_keys.size()-1].push_back(Input_Data());
        mp_keys[mp_keys.size()-1][mp_keys[mp_keys.size()-1].size()-1].set_joy_axis(i,0,0);
        mp_keys[mp_keys.size()-1].push_back(Input_Data());
        mp_keys[mp_keys.size()-1][mp_keys[mp_keys.size()-1].size()-1].set_joy_axis(i,1,0);
        mp_keys[mp_keys.size()-1].push_back(Input_Data());
        mp_keys[mp_keys.size()-1][mp_keys[mp_keys.size()-1].size()-1].set_joy_axis(i,0,1);
        mp_keys[mp_keys.size()-1].push_back(Input_Data());
        mp_keys[mp_keys.size()-1][mp_keys[mp_keys.size()-1].size()-1].set_joy_axis(i,1,1);
        mp_keys[mp_keys.size()-1].push_back(Input_Data());
        mp_keys[mp_keys.size()-1][mp_keys[mp_keys.size()-1].size()-1].set_joy_button(-1,5);
        mp_keys[mp_keys.size()-1].push_back(Input_Data());
        mp_keys[mp_keys.size()-1][mp_keys[mp_keys.size()-1].size()-1].set_joy_button(i,0);
        mp_keys[mp_keys.size()-1].push_back(Input_Data());
        mp_keys[mp_keys.size()-1][mp_keys[mp_keys.size()-1].size()-1].set_joy_button(i,2);
        mp_keys[mp_keys.size()-1].push_back(Input_Data());
        mp_keys[mp_keys.size()-1][mp_keys[mp_keys.size()-1].size()-1].set_joy_button(-1,1);
        mp_keys[mp_keys.size()-1].push_back(Input_Data());
        mp_keys[mp_keys.size()-1][mp_keys[mp_keys.size()-1].size()-1].set_joy_button(-1,7);
        mp_keys[mp_keys.size()-1].push_back(Input_Data());
        mp_keys[mp_keys.size()-1][mp_keys[mp_keys.size()-1].size()-1].set_joy_button(i,3);
    }

    input_already_bound=-1;
    input_already_bound_player=-1;
    command_to_set=-1;
    command_to_set_player=-1;

    game_in_progress=false;

    name="\x1F";

    current_shot=SHOT_PLAYER;

    ammo=STARTING_AMMO;

    score=0;

    main_levels_beaten=0;

    weather_machine=true;

    leaves=0;
    leaves_max=0;

    cheese=0;
    cheese_max=0;

    current_level=0;

    current_sub_level=0;

    non_number_level="";

    persistent_level_data=true;

    level_beaten=false;

    //This starts out at the program's running as -1 to let the game know that no level has been loaded yet.
    previous_level=-1;
    previous_sub_level=-1;

    //Until current_checkpoint is not equal to -1, the player will respawn at the spawn point.
    current_checkpoint=-1;

    seconds_total_current_level=0;
    seconds_best_current_level=4294967295;

    worldmap_enter_level=false;

    x=0;
    y=0;
    facing=0;

    current_worldmap=0;

    for(int i=0;i<3;i++){
        world_x[i]=-1;
        world_y[i]=-1;
    }

    bosses.clear();
    boss_messages.clear();
    for(int i=0;i<=LAST_LEVEL;i++){
        bosses.push_back(BOSS_STATE_NOT_SPAWNED);
        boss_messages.push_back(boss_message());
        if(i==5){
            boss_messages[boss_messages.size()-1].message_title="Boss unlocked: "+level.return_level_name(i);
            boss_messages[boss_messages.size()-1].message="Off in the distance, towards the keep, came a roar!\nHubert was startled, but ready for more!";
        }
        else if(i==7){
            boss_messages[boss_messages.size()-1].message_title="Boss unlocked: "+level.return_level_name(i);
            boss_messages[boss_messages.size()-1].message="From the direction of the old haunted manor\nCame quite the moan, in a very loud manner!";
        }
        else if(i==10){
            boss_messages[boss_messages.size()-1].message_title="Boss unlocked: "+level.return_level_name(i);
            boss_messages[boss_messages.size()-1].message="A great clash arose from the mountain of fire!\nHubert's situation grows ever more dire!";
        }
        else if(i==12){
            boss_messages[boss_messages.size()-1].message_title="Boss unlocked: "+level.return_level_name(i);
            boss_messages[boss_messages.size()-1].message="From the old tomb came a scream and a rumble!\nHubert set off, without even a grumble.";
        }
        else if(i==14){
            boss_messages[boss_messages.size()-1].message_title="Boss unlocked: "+level.return_level_name(i);
            boss_messages[boss_messages.size()-1].message="At last! His lieutenants defeated and smashed,\nThe pirate-king Dakkar is prepared for a clash!";
        }
    }

    w=PLAYER_W;
    h=PLAYER_H;

    //Set the camera's initial location and its dimensions.
    camera_x=0.0;
    camera_y=0.0;
    camera_w=DEFAULT_SCREEN_WIDTH;
    camera_h=DEFAULT_SCREEN_HEIGHT;

    camera_speed=24;

    camera_trap_x=0.0;
    camera_trap_y=0.0;

    camera_lag_x=9000;
    camera_lag_y=20;

    menu_background_counter_x=0;
    menu_background_counter_y=0;

    menu_background_counter_max_x=0;
    menu_background_counter_max_y=0;

    menu_background_x=NONE;
    menu_background_y=NONE;

    menu_speed_x=5.0;
    menu_speed_y=5.0;

    menu_title_counter=0;
    menu_title_font=false;

    overlay_snow_counter_x=0;
    overlay_snow_counter_max_x=0;
    overlay_snow_x=NONE;
    overlay_snow_speed_x=1.0/random_range(1,5);
    overlay_snow_speed_y=-1.0;

    overlay_rain_counter_x=0;
    overlay_rain_counter_max_x=0;
    overlay_rain_x=NONE;
    overlay_rain_speed_x=6.0/random_range(1,5);
    overlay_rain_speed_y=-6.0;

    game_beginning_cutscene=0;

    main_menu_special="";
    main_menu_special_color=COLOR_WHITE;

    need_to_reinit=false;

    ptr_player_image=return_character_image();
    ptr_player_worldmap_image=return_character_worldmap_image();

    ptr_player_footstep=return_character_sound_footstep();
    ptr_player_footstep2=return_character_sound_footstep2();
    ptr_player_jump=return_character_sound_jump();
    ptr_player_start_slide=return_character_sound_start_slide();
    ptr_player_worldmap_footstep=return_character_sound_footstep();
    ptr_player_worldmap_footstep2=return_character_sound_footstep2();

    new_game_plus=0;

    game_mode=GAME_MODE_SP_ADVENTURE;

    survival_seconds_to_escape=SURVIVAL_TIME_MEDIUM;

    survival_score=0;
    survival_escape=false;
    survival_complete=false;

    mp_adventure_started=false;
    mp_adventure_ending=false;

    new_game_plus_notification=false;

    reset_mp_players();

    mp_level_data_items_collected.clear();
    mp_level_data_doors_opened.clear();
    mp_level_properties.clear();
    for(int i=0;i<=LAST_LEVEL;i++){
        mp_level_data_items_collected.push_back(vector<bool>());
        mp_level_data_doors_opened.push_back(vector<bool>());

        mp_level_properties.push_back(Level_Properties());
        mp_level_properties[mp_level_properties.size()-1].current_sub_level=0;
        mp_level_properties[mp_level_properties.size()-1].level_beaten=false;
    }
}

void Player::end_game(){
    stop_balloonin_sound();

    no_game_in_progress();
}

void Player::stop_game(){
    //If a game is in progress.
    if(game_in_progress){
        level.unload_level();
    }

    reset();
    tooltip_levelname.on=false;
    tooltip_hint.on=false;

    music.restart_track(MUSIC_MENU);
    music.play_track(MUSIC_MENU);

    load_data();
}

void Player::start_game(){
    //If a game is not already in progress.
    if(!game_in_progress){
        //A game is now in progress.
        game_in_progress=true;

        if(game_mode==GAME_MODE_SP_ADVENTURE){
        }
        else if(game_mode==GAME_MODE_MP_ADVENTURE){
            current_level=0;
            current_sub_level=0;

            option_character=mp_character;

            inventory.clear();
            bosses.clear();
            for(int i=0;i<=LAST_LEVEL;i++){
                bosses.push_back(BOSS_STATE_NOT_SPAWNED);
            }
            ammo=STARTING_AMMO;
            score=0;
            main_levels_beaten=0;
            weather_machine=true;
            current_shot=SHOT_PLAYER;
            current_worldmap=0;
            new_game_plus=0;

            for(int i=0;i<mp_players.size();i++){
                mp_players[i].ammo=STARTING_AMMO;
            }
        }
        else if(game_mode==GAME_MODE_SP_SURVIVAL || game_mode==GAME_MODE_MP_SURVIVAL){
            current_level=-2;
            current_sub_level=0;

            level_beaten=false;

            option_character=mp_character;

            current_shot=SHOT_PLAYER;

            ammo=0;

            for(int i=0;i<mp_players.size();i++){
                mp_players[i].ammo=0;
            }

            survival_score=0;
            survival_escape=false;
            survival_complete=false;

            new_game_plus=0;
        }

        //Load and prepare the player data.
        load_data();

        for(int i=0;i<mp_players.size();i++){
            mp_players[i].load_data();
        }

        //Load the initial level data.
        level.load_level();
    }
}

void Player::beat_level(){
    bool game_beaten_message=false;

    //If this level has already been beaten.
    if(level_beaten){
        //We have just replayed this level.
        stat_levels_replayed++;
    }
    //If this level has been beaten for the first time,
    //and it is a main level.
    else if(!level_beaten && current_level>=3 && current_level<=14){
        main_levels_beaten++;

        if(main_levels_beaten%2==0 && return_main_bosses_unlocked()<4){
            //Determine the level for the new boss.
            int new_boss_level=0;
            while(true){
                new_boss_level=return_random_main_boss_level();
                //All bosses are already unlocked.
                if(new_boss_level==-1){
                    break;
                }
                else if(bosses[new_boss_level]==BOSS_STATE_NOT_SPAWNED){
                    bosses[new_boss_level]=BOSS_STATE_SPAWNED;

                    //Play story message.
                    window_message[0].set_message(boss_messages[new_boss_level].message_title,boss_messages[new_boss_level].message);

                    break;
                }
            }
        }

        profile.save_profile_global_data();
    }

    //The level is now beaten for sure.
    level_beaten=true;

    //A level has been beaten.
    profile.update_achievements(ACHIEVEMENT_BEAT_ANY_LEVEL);

    //If the level just beaten was the Lighthouse.
    if(current_level==14){
        profile.update_achievements(ACHIEVEMENT_BEAT_GAME);

        //Play ending message.
        msg="";
        string msg_title="The End?";
        if(bosses[5]==BOSS_STATE_DEFEATED){
            msg+="The king of the castle, beaten and gone,\nWas just one more victim of great Hubert's brawn!\n\n";
        }
        else if(bosses[5]==BOSS_STATE_SPAWNED){
            msg+="The king of the castle, still snug in his nest,\nWas still in the way of brave Hubert's grand quest!\n\n";
        }
        if(bosses[7]==BOSS_STATE_DEFEATED){
            msg+="The spooky ghost pirate was finally defeated,\nBut Hubert was not even slightly conceited.\n\n";
        }
        else if(bosses[7]==BOSS_STATE_SPAWNED){
            msg+="A spooky ghost pirate was haunting the house!\nHubert would need to defeat that darn mouse!\n\n";
        }
        if(bosses[10]==BOSS_STATE_DEFEATED){
            msg+="The beast in the volcano was no longer there!\nHubert had done it, he'd done it with flair!\n\n";
        }
        else if(bosses[10]==BOSS_STATE_SPAWNED){
            msg+="Now what is that hiding up in the caldera?\nSome sort of evil, horrid chimera!\n\n";
        }
        if(bosses[12]==BOSS_STATE_DEFEATED){
            msg+="One more pirate defeated, the pyramid saved,\nHubert stood proud, with another trial braved!\n\n";
        }
        else if(bosses[12]==BOSS_STATE_SPAWNED){
            msg+="A pirate mouse threatened from deep in the tomb,\nWould this evil creature be poor Hubert's doom?\n\n";
        }
        if(bosses[14]==BOSS_STATE_DEFEATED){
            msg+="The foul pirate captain, the mouse named Dakkar,\nWas defeated at last, he'd no longer yell 'arr!'\n\n";
        }
        else if(bosses[14]==BOSS_STATE_SPAWNED){
            msg+="The foul pirate captain, the mouse named Dakkar,\nWas still out there waiting, undefeated so far!\n\n";
        }
        else if(bosses[14]==BOSS_STATE_NOT_SPAWNED){
            if(new_game_plus==0){
                msg="Hubert had made it to lighthouse from valley,\nBut he still hadn't rescued the poor kidnapped Sally!\n\n";
            }
            else{
                msg="Hubert had made it from valley to lighthouse,\nBut had not defeated the foul pirate mouse!\n\n";
            }
        }
        if(achievements[ACHIEVEMENT_COLLECT_ALL_LEAVES] && achievements[ACHIEVEMENT_COLLECT_ALL_CHEESE]){
            msg+="Hubert collected all of the leaves,\nAnd he also collected all of the cheese.\n\n";
        }
        else if(achievements[ACHIEVEMENT_COLLECT_ALL_LEAVES] && !achievements[ACHIEVEMENT_COLLECT_ALL_CHEESE]){
            msg+="Hubert collected all of the leaves,\nBut he'd yet to find all of the pieces of cheese.\n\n";
        }
        else if(!achievements[ACHIEVEMENT_COLLECT_ALL_LEAVES] && achievements[ACHIEVEMENT_COLLECT_ALL_CHEESE]){
            msg+="Hubert collected all of the cheese,\nBut he hadn't yet managed to find all the leaves.\n\n";
        }
        else{
            msg+="Hubert had yet to find all of the leaves,\nAnd he also still needed to find all the cheese.\n\n";
        }
        if(bosses[14]==BOSS_STATE_DEFEATED && achievements[ACHIEVEMENT_COLLECT_ALL_LEAVES] && achievements[ACHIEVEMENT_COLLECT_ALL_CHEESE]){
            msg+="Perhaps this is finally the end of our tale!\nMighty Hubert retired to rest in the dale.\nWith his main squeeze Sally, stacks of leaves and cheese,\nHubert was happy, content as could be!";
            msg_title="The End";
        }
        else{
            msg+="And that is the end of our story, or is it?\nBrave Hubert's quest was not yet finished!";
        }
        window_message[0].set_message(msg_title,msg);
        game_beaten_message=true;
    }

    //If the time taken to beat this level was less than the previous record.
    if(seconds_best_current_level==4294967295 || level.seconds_in_current_level<seconds_best_current_level){
        seconds_best_current_level=level.seconds_in_current_level;
    }

    //Check for new achievements in this level before leaving it.
    profile.update_achievements();

    profile.save_level_data();
    previous_level=current_level;
    previous_sub_level=current_sub_level;
    current_level=next_level;
    current_checkpoint=-1;
    if(on_worldmap()){
        current_worldmap=current_level;
    }
    load_data();

    for(int i=0;i<mp_players.size();i++){
        mp_players[i].load_data();
    }

    if(game_beaten_message){
        set_firework_rate(true);

        //If the final boss is defeated, we just beat the game.
        if(bosses[14]==BOSS_STATE_DEFEATED){
            new_game_plus++;

            if(game_mode==GAME_MODE_SP_ADVENTURE){
                //Convert ammo to score.
                gain_score(ammo*2,-1.0,-1.0,false);

                //Reset player ammo.
                ammo=STARTING_AMMO;

                //Reset the main levels beaten.
                main_levels_beaten=0;

                //Reset the level beaten status of all levels.
                profile.reset_level_beaten();

                //Move player to starting world map location.
                profile.reset_world_map_data();

                //Reset all ammo barrels.
                profile.reset_ammo_barrels();

                //Reset weather machine.
                weather_machine=true;
                profile.reset_weather_machine();

                //Reset bosses.
                bosses.clear();
                for(int i=0;i<=LAST_LEVEL;i++){
                    bosses.push_back(BOSS_STATE_NOT_SPAWNED);
                }

                profile.save_inventory();
                profile.save_boss_states();

                new_game_plus_notification=true;
            }
            else if(game_mode==GAME_MODE_MP_ADVENTURE){
                mp_adventure_ending=true;
            }
        }
    }

    level.load_level();
}

void Player::boss_begin(int get_boss_index){
    boss_index=get_boss_index;
    music.restart_track(MUSIC_BOSS);
    music.restart_track(MUSIC_BOSS_FAST);
    music.play_track(MUSIC_BOSS);
}

void Player::boss_end(){
    if(boss_index!=-1){
        boss_index=-1;
        music.play_track(MUSIC_LEVEL_NORMAL);
    }
}

void Player::load_data(){
    UPDATE_RATE=DEFAULT_UPDATE_RATE;
    SKIP_TICKS=1000.0/UPDATE_RATE;

    light_source.on=true;
    light_source.x=0;
    light_source.y=0;
    light_source.color=color_name_to_doubles(COLOR_WHITE);
    light_source.radius=15*(TILE_SIZE/LIGHTING_TILE_SIZE);
    light_source.dimness=0.0;
    light_source.falloff=0.035/(TILE_SIZE/LIGHTING_TILE_SIZE);

    fadeout=false;
    fade_opacity=0.0;

    FLYING=false;
    is_player=true;
    which_mp_player=-1;
    DYING=false;
    counter_bubble=0;
    death_direction=false;
    death_speed=2.5;
    death_bounces=0;
    sound_channel_death=-1;
    sound_channel_balloonin=-1;
    pause=false;
    special_count_height=100000;
    sucked_left=false;
    sucked_right=false;
    undead=false;
    counter_jump_mercy=0;
    counter_jump_mode=0;
    counter_level_end=0;
    counter_create_tracer=0;
    goal_crossed=false;
    next_level=0;
    hitbox_size_modifier=-4;
    riding_cow=false;
    worldmap_run_speed=5.0;
    delta_move_state=NONE;
    counter_swim_sound=0;
    boss_index=-1;
    worldmap_region=WORLDMAP_REGION_NORMAL;

    bubble_mode=false;
    bubble_move_x=0.0;
    bubble_move_y=0.0;

    camera_focused=-1;

    set_physics();

    speedometer=0.0;

    //Shooting:
    shoot_state=NONE;
    SHOOTING=false;
    shoot_render_direction=NONE;

    //Camera:
    cam_state=CAM_STICKY;

    //Looking:
    look_state=NONE;
    LOOKING=false;
    look_offset_x=0;
    look_offset_y=0;
    look_speed=10;

    CROUCHING=false;
    crouching_at_frame_start=false;
    SLIDING=false;

    night_mode=true;
    deadly_water=false;

    //Start the player off with his feet on neither air nor ground.
    //handle_events() will determine where the player's feet are.
    touching_air=false;
    touching_ground=false;
    touching_sloped_ground=false;
    touched_slope_angle=0.0;
    touching_sloped_ground_last_check=false;
    on_cloud=false;
    cloud_npc=false;

    //Climbing:
    CLIMBING=false;
    on_climbable_top_tile=false;
    climb_jump_timer=0;
    climb_jump_delay=20;
    climb_speed=3.5;
    climbing_jump_max=10.0;
    climbing_jump_min=8.0;
    CLIMBING_JUMP=false;
    climb_state=NONE;

    //Moving Platform:
    moving_platform_x=0.0;
    moving_platform_y=0.0;
    MOVING_PLATFORM_X_THIS_FRAME=false;
    MOVING_PLATFORM_IN_WATER=false;

    //Vector counters:
    existing_effects_water_bubble=0;
    existing_effects_water_splash=0;
    existing_effects_score_floater=0;
    existing_effects_firework=0;
    existing_shots=0;

    //Dimensions of the player.
    if(on_worldmap()){
        w=PLAYER_WORLDMAP_W;
        h=PLAYER_WORLDMAP_H;
    }
    else{
        w=PLAYER_W;
        h=PLAYER_H;
    }

    current_level_leaves=0;
    current_level_cheese=0;

    camera_delta_x=0.0;
    camera_delta_y=0.0;

    //Animation:
    frame_idle=0;
    frame_counter_idle=0;
    frame=0;
    frame_counter=0;
    frame_swim=0;
    frame_counter_swim=0;
    frame_jump=0;
    frame_counter_jump=0;
    frame_shoot=0;
    frame_counter_shoot=0;
    frame_climb=1;
    frame_counter_climb=0;
    frame_fly=0;
    frame_counter_fly=0;
    frame_water=0;
    frame_counter_water=0;
    frame_death=0;
    frame_counter_death=0;
    frame_powerup=0;
    frame_counter_powerup=0;

    balloon_scale_direction_x=false;
    balloon_scale_x=1.0;
    balloon_scale_direction_y=true;
    balloon_scale_y=1.0;

    opacity_overlay_alarm=0.0;
    fade_direction_overlay_alarm=false;

    set_firework_rate();

    update_character();
}

void Player::check_special_items(){
    if(check_inventory(ITEM_SWIMMING_GEAR)){
        swimming_gear=true;
    }
    if(check_inventory(ITEM_SUIT_DEADLY_WATER)){
        suit_deadly_water=true;
    }
    if(check_inventory(ITEM_SUIT_SHARP)){
        suit_sharp=true;
    }
    if(check_inventory(ITEM_SUIT_BANANA)){
        suit_banana=true;
    }
    if(check_inventory(ITEM_SHOT_HOMING)){
        shot_homing=true;
    }
    if(check_inventory(ITEM_TRANSLATOR)){
        translator=true;
    }
    if(check_inventory(ITEM_J_WING)){
        j_wing=true;
    }
}

void Player::set_physics(){
    //Movement:
    move_state=NONE;
    run_speed=0.0;
    max_speed=6.0;
    acceleration=0.275;
    deceleration=0.5;
    air_drag=0.3;
    air_drag_divisor=64.0;
    friction=acceleration;
    air_accel=acceleration;
    air_decel=deceleration;

    //Gravity:
    air_velocity=0;
    IN_AIR=false;
    gravity_max=10.0;
    gravity=0.5;

    //Jumping:
    jump_state=false;
    jump_max=10.0;
    jump_min=5.0;
    extra_jumps=0;

    ///Sonic Physics
    /**max_speed=6.0;
    acceleration=0.046875;
    deceleration=0.5;
    air_drag=0.125;
    air_drag_divisor=256.0;
    friction=acceleration;
    air_accel=acceleration*2;
    air_decel=air_accel;

    air_velocity=0;
    IN_AIR=false;
    gravity_max=16.0;
    gravity=0.21875;

    jump_state=false;
    jump_max=6.5;
    jump_min=4.0;
    extra_jumps=0;*/
    ///

    //Swimming:
    SWIMMING=false;
    underwater=false;
    water_running=false;
    SWIM_CAN_JUMP=false;
    swimming_gear=false;
    oxygen_max_capacity=75;
    oxygen=oxygen_max_capacity;
    max_swim_speed=6.5;
    max_buoyancy=2.0;
    float_speed=0.0;
    swim_acceleration=0.1375;
    swim_deceleration=0.375;
    swim_friction=swim_acceleration;

    //Other Upgrades:
    suit_deadly_water=false;
    suit_sharp=false;
    suit_banana=false;
    shot_homing=false;
    translator=false;
    j_wing=false;
}

void Player::update_character(){
    if(ptr_player_image!=return_character_image()){
        ptr_player_image=return_character_image();
    }
    if(ptr_player_worldmap_image!=return_character_worldmap_image()){
        ptr_player_worldmap_image=return_character_worldmap_image();
    }

    if(ptr_player_footstep!=return_character_sound_footstep()){
        ptr_player_footstep=return_character_sound_footstep();
    }
    if(ptr_player_footstep2!=return_character_sound_footstep2()){
        ptr_player_footstep2=return_character_sound_footstep2();
    }
    if(ptr_player_jump!=return_character_sound_jump()){
        ptr_player_jump=return_character_sound_jump();
    }
    if(ptr_player_start_slide!=return_character_sound_start_slide()){
        ptr_player_start_slide=return_character_sound_start_slide();
    }
    if(ptr_player_worldmap_footstep!=return_character_sound_footstep()){
        ptr_player_worldmap_footstep=return_character_sound_footstep();
    }
    if(ptr_player_worldmap_footstep2!=return_character_sound_footstep2()){
        ptr_player_worldmap_footstep2=return_character_sound_footstep2();
    }

    if(option_character==CHARACTER_HUBERT){
        set_physics();
    }
    else if(option_character==CHARACTER_SLIME_O){
        max_speed=5.0;
        deceleration=0.6;
        friction*=2.0;
        air_decel=deceleration;

        gravity_max=11.0;
        gravity=0.55;

        jump_max=12.0;
        jump_min=6.5;

        max_swim_speed=6.0;
        max_buoyancy=6.0;
        swim_deceleration=0.475;
        swim_friction=swim_acceleration*2.0;
    }
    else if(option_character==CHARACTER_SKETCH){
        max_speed=6.5;
        acceleration=0.375;
        deceleration=0.45;
        friction=acceleration*0.75;
        air_accel=acceleration;
        air_decel=deceleration;

        gravity_max=6.5;
        gravity=0.2;

        jump_max=6.5;
        jump_min=5.0;

        max_swim_speed=7.0;
        max_buoyancy=0.75;
        swim_acceleration=0.3375;
        swim_friction=swim_acceleration;
    }
    else if(option_character==CHARACTER_PENNY){
        max_speed=6.5;
        acceleration=0.375;
        deceleration=0.5;
        air_drag=0.125;
        air_drag_divisor=256.0;
        friction=acceleration;
        air_accel=acceleration;
        air_decel=deceleration;

        gravity=0.55;
        jump_max=10.0;
        jump_min=5.0;

        max_swim_speed=7.0;
        swim_acceleration=0.3375;
        swim_friction=swim_acceleration;
    }

    check_special_items();
}

void Player::toggle_pause(bool get_pause){
    if(game_in_progress){
        //If we are pausing the game.
        if(get_pause){
            pause=true;

            if(game_mode==GAME_MODE_SP_ADVENTURE){
                //If the menus timer is not already started.
                if(!timer_menus.is_started()){
                    timer_menus.start();
                }
                if(timer_playing.is_started()){
                    timer_playing.pause();
                }
                if(timer_traveling.is_started()){
                    timer_traveling.pause();
                }
                if(timer_riding_cows.is_started()){
                    timer_riding_cows.pause();
                }
            }
            else if(game_mode==GAME_MODE_SP_SURVIVAL || game_mode==GAME_MODE_MP_SURVIVAL){
                if(level.timer_survival_escape_counter.is_started()){
                    level.timer_survival_escape_counter.pause();
                }
                if(level.timer_survival_spawn_items.is_started()){
                    level.timer_survival_spawn_items.pause();
                }
                if(level.timer_survival_spawn_npcs.is_started()){
                    level.timer_survival_spawn_npcs.pause();
                }
            }
        }
        //If we are unpausing the game.
        else{
            pause=false;

            if(game_mode==GAME_MODE_SP_ADVENTURE){
                //If the playing timer is not already started.
                if(timer_playing.is_paused()){
                    timer_playing.unpause();
                }
                else{
                    timer_playing.start();
                }
                //If the traveling timer is not already started and the current map is the world map.
                if(on_worldmap()){
                    if(timer_traveling.is_paused()){
                        timer_traveling.unpause();
                    }
                    else{
                        timer_traveling.start();
                    }
                }
                if(timer_riding_cows.is_paused()){
                    timer_riding_cows.unpause();
                }

                timer_menus.stop();
            }
            else if(game_mode==GAME_MODE_SP_SURVIVAL || game_mode==GAME_MODE_MP_SURVIVAL){
                if(level.timer_survival_escape_counter.is_paused()){
                    level.timer_survival_escape_counter.unpause();
                }
                if(level.timer_survival_spawn_items.is_paused()){
                    level.timer_survival_spawn_items.unpause();
                }
                if(level.timer_survival_spawn_npcs.is_paused()){
                    level.timer_survival_spawn_npcs.unpause();
                }
            }
        }
    }
}

void Player::handle_tracers(){
    if(option_dev){
        if(counter_create_tracer>0){
            counter_create_tracer--;
        }
        if(counter_create_tracer<=0){
            counter_create_tracer=UPDATE_RATE/4.0;
            tracer_manager.create_tracer(x+w/2,y+h/2);
        }
    }
}

void Player::put_in_bubble(){
    bubble_move_x=run_speed;
    if(IN_AIR){
        bubble_move_y=air_velocity;
    }
    else{
        bubble_move_y=0.0;
    }
    mp_reset(x,y);
    bubble_mode=true;

    //Prevent the constant bubble forming sound if we are moving the camera around with dev controls.
    if(cam_state==CAM_STICKY){
        play_positional_sound(sound_system.player_bubble_form,x,y);
    }
}

void Player::move(){
    bool events_handled=false;
    bool started_frame_on_ground=false;

    if(goal_crossed && counter_level_end==0){
        stop_balloonin_sound();

        boss_end();

        beat_level();

        //Skip the rest of this function, as we are now on a different level.
        return;
    }

    if(game_mode==GAME_MODE_MP_SURVIVAL){
        if(all_players_gone()){
            survival_end_game(false);

            //Play the final death sound.
            play_positional_sound(sound_system.player_death2);

            return;
        }
    }
    else if(game_mode==GAME_MODE_MP_ADVENTURE){
        if(all_players_gone()){
            boss_end();

            //If the goal has already been crossed, the player gets credit for beating
            //the level even if he dies.
            if(goal_crossed){
                beat_level();

                //Skip the rest of this function, as we are now on a different level.
                return;
            }
            else{
                //Reset needed variables.
                load_data();

                for(int i=0;i<mp_players.size();i++){
                    mp_players[i].load_data();
                }

                //Reload the level.
                profile.save_level_data();
                previous_level=current_level;
                previous_sub_level=current_sub_level;
                level.load_level();

                //Play the final death sound.
                play_positional_sound(sound_system.player_death2,x,y);

                //Skip the rest of this function, as we are now on a different level.
                return;
            }
        }
    }

    if(!bubble_mode){
        //If the player is dying and has left the screen,
        //complete the death.
        if(!game_mode_is_multiplayer() && DYING && !collision_check(x,y,w,h,camera_x,camera_y,camera_w,camera_h)){
            //If the goal has already been crossed, the player gets credit for beating
            //the level even if he dies.
            if(goal_crossed){
                beat_level();

                //Skip the rest of this function, as we are now on a different level.
                return;
            }
            else{
                if(game_mode==GAME_MODE_SP_ADVENTURE){
                    //Reset needed variables.
                    load_data();

                    //Reload the level.
                    profile.save_level_data();
                    previous_level=current_level;
                    previous_sub_level=current_sub_level;
                    level.load_level();

                    //Play the final death sound.
                    play_positional_sound(sound_system.player_death2,x,y);

                    //Skip the rest of this function, as we are now on a different level.
                    return;
                }
                else if(game_mode==GAME_MODE_SP_SURVIVAL){
                    survival_end_game(false);

                    //Play the final death sound.
                    play_positional_sound(sound_system.player_death2,x,y);

                    //Skip the rest of this function, as we are now on a different level.
                    return;
                }
            }
        }

        if(game_mode_is_multiplayer()){
            if(!DYING && which_mp_player!=cam_focused_index() && !collision_check(x,y,w,h,camera_x,camera_y,camera_w,camera_h)){
                put_in_bubble();

                return;
            }
        }

        speedometer=run_speed+moving_platform_x;
        if(sucked_left && !sucked_right){
            speedometer-=SUCK_SPEED;
        }
        else if(!sucked_left && sucked_right){
            speedometer+=SUCK_SPEED;
        }
        speedometer=fabs(speedometer);

        if(!IN_AIR){
            started_frame_on_ground=true;
        }

        if(crouching_at_frame_start && !CROUCHING){
            pushed_into_ceiling();
        }

        bool was_sliding=SLIDING;
        SLIDING=false;

        if(touching_sloped_ground && command_state(COMMAND_DOWN) && !LOOKING){
            if(!was_sliding){
                play_positional_sound(*ptr_player_start_slide,x,y);
            }

            crouch_start();
            SLIDING=true;
            if(touched_slope_angle==45){
                move_state=LEFT;
            }
            else if(touched_slope_angle==135){
                move_state=RIGHT;
            }
        }

        if(option_hints && !LOOKING && !DYING){
            bool hint=false;

            for(int i=0;i<vector_signs.size();i++){
                if(fabs(vector_signs[i].x-x)<PROCESS_RANGE && fabs(vector_signs[i].y-y)<PROCESS_RANGE){
                    if(collision_check(x,y,w,h,vector_signs[i].x,vector_signs[i].y,SIGN_W,SIGN_H)){
                        hint=true;

                        string tooltip_text="Press 'up' to read signs.";
                        tooltip_hint.setup(tooltip_text,(camera_w-tooltip_text.length()*12)/2.0,40);

                        break;
                    }
                }
            }

            if(!hint){
                for(int i=0;i<vector_triggers.size();i++){
                    if(fabs(vector_triggers[i].x-x)<PROCESS_RANGE && fabs(vector_triggers[i].y-y)<PROCESS_RANGE){
                        if(!vector_triggers[i].trigger_method && collision_check(x,y,w,h,vector_triggers[i].x,vector_triggers[i].y,vector_triggers[i].w,vector_triggers[i].h)){
                            if(vector_triggers[i].render_trigger!=0){
                                hint=true;

                                string tooltip_text="Press 'up' to use levers.";
                                tooltip_hint.setup(tooltip_text,(camera_w-tooltip_text.length()*12)/2.0,40);

                                break;
                            }
                            else if(vector_triggers[i].targets_trap(TRAP_OPEN_SHOP)){
                                hint=true;

                                string tooltip_text="Press 'up' to use the Shoppe-O-Matic.";
                                tooltip_hint.setup(tooltip_text,(camera_w-tooltip_text.length()*12)/2.0,40);

                                break;
                            }
                            else if(vector_triggers[i].targets_trap(TRAP_OPEN_UPGRADES)){
                                hint=true;

                                string tooltip_text="Press 'up' to configure your purchased toys.";
                                tooltip_hint.setup(tooltip_text,(camera_w-tooltip_text.length()*12)/2.0,40);

                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    if(counter_jump_mercy>0){
        counter_jump_mercy--;
    }
    if(counter_jump_mode>0){
        counter_jump_mode--;
    }
    if(counter_level_end>0){
        counter_level_end--;

        if(counter_level_end<=4.35*UPDATE_RATE){
            fadeout=true;
            fade_opacity+=0.01;
            if(fade_opacity>1.0){
                fade_opacity=1.0;
            }
        }
    }
    if(!DYING){
        handle_counters();
    }

    if(!bubble_mode){
        if(!CLIMBING){
            if(sucked_left && !sucked_right){
                move_suck(LEFT);
                handle_events();
                events_handled=true;
            }
            else if(!sucked_left && sucked_right){
                move_suck(RIGHT);
                handle_events();
                events_handled=true;
            }
        }

        //If the player is alive.
        if(!DYING){
            if(climb_jump_timer>0){
                climb_jump_timer--;
            }

            if(!CLIMBING){
                //*************************//
                // Handle x-axis movement: //
                //*************************//

                //////////////////////////////////////////////////
                //Move the player according to their move state://
                //////////////////////////////////////////////////

                double run_chunk;
                if(fabs(run_speed)<pixel_safety_x){
                    run_chunk=fabs(run_speed);
                }
                else{
                    run_chunk=pixel_safety_x;
                }

                for(double i=fabs(run_speed);i>0;){
                    //If we have run_chunk or more pixels left to move,
                    //we will move run_chunk pixels, call handle_events(), and loop back up here.

                    //Or, if we have less than run_chunk pixels left to move,
                    //we will move whatever pixels we have left and call handle_events() once more.
                    if(i<run_chunk){
                        run_chunk=i;
                        i=0;
                    }

                    //Move.
                    if(run_speed<0.0){
                        run_chunk*=-1;
                    }
                    x+=run_chunk;
                    if(run_speed<0.0){
                        run_chunk*=-1;
                    }

                    //If we still have pixels left to move.
                    if(i!=0){
                        i-=run_chunk;
                    }

                    handle_events();
                    //If the player is now climbing.
                    if(CLIMBING){
                        //Stop processing movement for this frame.
                        return;
                    }
                    events_handled=true;
                }

                if(!SWIMMING){
                    //Handle acceleration.
                    double accel_to_use=acceleration;
                    if(IN_AIR){
                        accel_to_use=air_accel;
                    }
                    double slide_bonus=0.0;
                    if(SLIDING){
                        slide_bonus=accel_to_use;
                    }
                    if(run_speed<max_speed && run_speed>max_speed*-1){
                        if(move_state==LEFT && run_speed<=0.0){
                            run_speed-=accel_to_use+slide_bonus;
                        }
                        else if(move_state==RIGHT && run_speed>=0.0){
                            run_speed+=accel_to_use+slide_bonus;
                        }
                        if(run_speed>max_speed){
                            run_speed=max_speed;
                        }
                        else if(run_speed<max_speed*-1){
                            run_speed=max_speed*-1;
                        }
                    }
                    //Handle deceleration.
                    double decel_to_use=deceleration;
                    if(IN_AIR){
                        decel_to_use=air_decel;
                    }
                    slide_bonus=0.0;
                    if(SLIDING){
                        slide_bonus=decel_to_use;
                    }
                    if(move_state==LEFT && run_speed>0.0){
                        if(run_speed-decel_to_use-slide_bonus<0.0){
                            run_speed=0.0-decel_to_use-slide_bonus;
                        }
                        else{
                            run_speed-=decel_to_use+slide_bonus;
                        }
                    }
                    else if(move_state==RIGHT && run_speed<0.0){
                        if(run_speed+decel_to_use+slide_bonus>0.0){
                            run_speed=decel_to_use+slide_bonus;
                        }
                        else{
                            run_speed+=decel_to_use+slide_bonus;
                        }
                    }
                    if(!IN_AIR){
                        //Handle friction.
                        if(move_state!=LEFT && move_state!=RIGHT && run_speed!=0.0){
                            if(run_speed<0.0){
                                run_speed+=friction;
                                if(run_speed>0.0){
                                    run_speed=0.0;
                                }
                            }
                            else if(run_speed>0.0){
                                run_speed-=friction;
                                if(run_speed<0.0){
                                    run_speed=0.0;
                                }
                            }
                        }
                        if(!MOVING_PLATFORM_X_THIS_FRAME && moving_platform_x!=0.0){
                            if(moving_platform_x<0.0){
                                moving_platform_x+=friction;
                                if(moving_platform_x>0.0){
                                    moving_platform_x=0.0;
                                }
                            }
                            else if(moving_platform_x>0.0){
                                moving_platform_x-=friction;
                                if(moving_platform_x<0.0){
                                    moving_platform_x=0.0;
                                }
                            }
                        }
                    }
                    else{
                        //Handle air drag.
                        if(air_velocity<0.0 && air_velocity>jump_min*-1 && air_drag!=0.0){
                            if(run_speed<0.0){
                                run_speed+=(fabs(run_speed)/air_drag)/air_drag_divisor;
                            }
                            else if(run_speed>0.0){
                                run_speed-=(fabs(run_speed)/air_drag)/air_drag_divisor;
                            }
                            if(moving_platform_x<0.0){
                                moving_platform_x+=(fabs(moving_platform_x)/air_drag)/air_drag_divisor;
                            }
                            else if(moving_platform_x>0.0){
                                moving_platform_x-=(fabs(moving_platform_x)/air_drag)/air_drag_divisor;
                            }
                        }
                    }
                }

                //////////////////////////////////////////////////////////////////
                //Move the player according to their moving platform x modifier://
                //////////////////////////////////////////////////////////////////

                if(fabs(moving_platform_x)<pixel_safety_x){
                    run_chunk=fabs(moving_platform_x);
                }
                else{
                    run_chunk=pixel_safety_x;
                }

                for(double i=fabs(moving_platform_x);i>0;){
                    //If we have run_chunk or more pixels left to move,
                    //we will move run_chunk pixels, call handle_events(), and loop back up here.

                    //Or, if we have less than run_chunk pixels left to move,
                    //we will move whatever pixels we have left and call handle_events() once more.
                    if(i<run_chunk){
                        run_chunk=i;
                        i=0;
                    }

                    //Move.
                    if(moving_platform_x<0.0){
                        run_chunk*=-1;
                    }
                    x+=run_chunk;
                    if(moving_platform_x<0.0){
                        run_chunk*=-1;
                    }

                    //If we still have pixels left to move.
                    if(i!=0){
                        i-=run_chunk;
                    }

                    handle_events();
                    //If the player is now climbing.
                    if(CLIMBING){
                        //Stop processing movement for this frame.
                        return;
                    }
                    events_handled=true;
                }

                //*************************//
                // Handle y-axis movement: //
                //*************************//

                //////////////////////////////////////////////////////
                //Move the player according to their swimming state://
                //////////////////////////////////////////////////////

                //If the player is swimming.
                if(SWIMMING){
                    double float_chunk;
                    if(fabs(float_speed)<pixel_safety_y){
                        float_chunk=fabs(float_speed);
                    }
                    else{
                        float_chunk=pixel_safety_y;
                    }

                    for(double i=fabs(float_speed);i>0;){
                        //If we have float_chunk or more pixels left to move,
                        //we will move float_chunk pixels, call handle_events(), and loop back up here.

                        //Or, if we have less than float_chunk pixels left to move,
                        //we will move whatever pixels we have left and call handle_events() once more.
                        if(i<float_chunk){
                            float_chunk=i;
                            i=0;
                        }

                        //Move.
                        if(float_speed<0){
                            float_chunk*=-1;
                        }

                        y+=float_chunk;

                        if(float_speed<0){
                            float_chunk*=-1;
                        }

                        //If we still have pixels left to move.
                        if(i!=0){
                            i-=float_chunk;
                        }

                        handle_events();
                        //If the player is now climbing.
                        if(CLIMBING){
                            //Stop processing movement for this frame.
                            return;
                        }
                        events_handled=true;
                    }
                }

                ////////////////////////////////////////////////////
                //Move the player according to their air_velocity://
                ////////////////////////////////////////////////////

                //If the player is in the air.
                if(IN_AIR){
                    double air_chunk;
                    if(fabs(air_velocity)<pixel_safety_y){
                        air_chunk=fabs(air_velocity);
                    }
                    else{
                        air_chunk=pixel_safety_y;
                    }

                    //First, translate the player based on his air velocity.
                    for(double i=fabs(air_velocity);i>0;){
                        //If we have air_chunk or more pixels left to move,
                        //we will move air_chunk pixels, call handle_events(), and loop back up here.

                        //Or, if we have less than air_chunk pixels left to move,
                        //we will move whatever pixels we have left and call handle_events() once more.
                        if(i<air_chunk){
                            air_chunk=i;
                            i=0;
                        }

                        //Move.
                        if(air_velocity<0){
                            y-=air_chunk;
                        }
                        else if(air_velocity>0){
                            y+=air_chunk;
                        }

                        //If we still have pixels left to move.
                        if(i!=0){
                            i-=air_chunk;
                        }

                        handle_events();
                        //If the player is now climbing.
                        if(CLIMBING){
                            //Stop processing movement for this frame.
                            return;
                        }
                        events_handled=true;
                    }

                    //Then, we handle gravity, which will affect the next tick.

                    //If the player is going to start falling this frame.
                    if(IN_AIR && air_velocity<=0 && air_velocity+gravity>=0){
                        //Save the current absolute height of the player.
                        special_count_height=y;
                    }

                    //As long as air_velocity hasn't exceeded gravity_max, the maximum speed an object can fall, add gravity to air_velocity.
                    if(air_velocity<gravity_max){
                        air_velocity+=gravity;
                    }
                    if(air_velocity>gravity_max){
                        air_velocity=gravity_max;
                    }
                }

                //*******************************//
                // Handle swimming acceleration: //
                //*******************************//

                //If the player is swimming but isn't currently moving on the y-axis.
                if(SWIMMING && move_state!=UP && move_state!=LEFT_UP && move_state!=RIGHT_UP && move_state!=DOWN && move_state!=LEFT_DOWN && move_state!=RIGHT_DOWN){
                    //Float upwards.
                    if(float_speed>-max_buoyancy){
                        //If the time in water is long enough.
                        if(timer_time_in_water.get_ticks()>=500){
                            //Stop the timer so the player's buoyancy acceleration will slow to the standard rate.
                            timer_time_in_water.stop();
                        }

                        //If the timer is not running, buoyancy acceleration is normal.
                        if(!timer_time_in_water.is_started()){
                            float_speed-=swim_acceleration/4;
                        }
                        //If the timer is running, buoyancy acceleration is increased.
                        else if(timer_time_in_water.is_started()){
                            float_speed-=swim_acceleration*4;
                        }

                        if(float_speed<-max_buoyancy){
                            float_speed=-max_buoyancy;
                        }
                        if(float_speed<-max_swim_speed){
                            float_speed=-max_swim_speed;
                        }
                    }
                }
                //Slowly stop swimming on the x-axis.
                if(SWIMMING && move_state!=LEFT && move_state!=LEFT_DOWN && move_state!=LEFT_UP && move_state!=RIGHT && move_state!=RIGHT_DOWN && move_state!=RIGHT_UP){
                    if(run_speed>0.0){
                        run_speed-=swim_friction;
                        if(run_speed<0.0){
                            run_speed=0.0;
                        }
                    }
                    else if(run_speed<0.0){
                        run_speed+=swim_friction;
                        if(run_speed>0.0){
                            run_speed=0.0;
                        }
                    }
                }
                if(SWIMMING && moving_platform_x!=0.0){
                    if(moving_platform_x<0.0){
                        moving_platform_x+=friction;
                        if(moving_platform_x>0.0){
                            moving_platform_x=0.0;
                        }
                    }
                    else if(moving_platform_x>0.0){
                        moving_platform_x-=friction;
                        if(moving_platform_x<0.0){
                            moving_platform_x=0.0;
                        }
                    }
                }
                //If the player is swimming and is moving.
                //Don't allow the player to swim beyond the maximum swim speed.
                if(SWIMMING && move_state!=0){
                    if(run_speed<max_swim_speed && run_speed>max_swim_speed*-1){
                        if((move_state==LEFT || move_state==LEFT_DOWN || move_state==LEFT_UP) && run_speed<=0.0){
                            run_speed-=swim_acceleration;
                        }
                        else if((move_state==RIGHT || move_state==RIGHT_DOWN || move_state==RIGHT_UP) && run_speed>=0.0){
                            run_speed+=swim_acceleration;
                        }
                        if(run_speed>max_swim_speed){
                            run_speed=max_swim_speed;
                        }
                        else if(run_speed<max_swim_speed*-1){
                            run_speed=max_swim_speed*-1;
                        }
                    }
                    if(float_speed<max_swim_speed && float_speed>max_swim_speed*-1){
                        if((move_state==UP || move_state==LEFT_UP || move_state==RIGHT_UP) && float_speed<=0.0){
                            float_speed-=swim_acceleration;
                        }
                        else if((move_state==DOWN || move_state==LEFT_DOWN || move_state==RIGHT_DOWN) && float_speed>=0.0){
                            float_speed+=swim_acceleration;
                        }
                        if(float_speed>max_swim_speed){
                            float_speed=max_swim_speed;
                        }
                        else if(float_speed<max_swim_speed*-1){
                            float_speed=max_swim_speed*-1;
                        }
                    }
                    if((move_state==LEFT || move_state==LEFT_DOWN || move_state==LEFT_UP) && run_speed>0.0){
                        if(run_speed-swim_deceleration<0.0){
                            run_speed=0.0-swim_deceleration;
                        }
                        else{
                            run_speed-=swim_deceleration;
                        }
                    }
                    else if((move_state==RIGHT || move_state==RIGHT_DOWN || move_state==RIGHT_UP) && run_speed<0.0){
                        if(run_speed+swim_deceleration>0.0){
                            run_speed=swim_deceleration;
                        }
                        else{
                            run_speed+=swim_deceleration;
                        }
                    }
                    if((move_state==UP || move_state==LEFT_UP || move_state==RIGHT_UP) && float_speed>0.0){
                        if(float_speed-swim_deceleration<0.0){
                            float_speed=0.0-swim_deceleration;
                        }
                        else{
                            float_speed-=swim_deceleration;
                        }
                    }
                    else if((move_state==DOWN || move_state==LEFT_DOWN || move_state==RIGHT_DOWN) && float_speed<0.0){
                        if(float_speed+swim_deceleration>0.0){
                            float_speed=swim_deceleration;
                        }
                        else{
                            float_speed+=swim_deceleration;
                        }
                    }
                }
            }

            //***********************************************//
            // Handle y-axis movement for a climbing player: //
            //***********************************************//

            else if(CLIMBING){
                double speed=climb_speed;
                if(move_state==DOWN){
                    speed*=2;
                }

                double run_chunk;
                if(fabs(speed)<pixel_safety_y){
                    run_chunk=fabs(speed);
                }
                else{
                    run_chunk=pixel_safety_y;
                }

                for(double i=fabs(speed);i>0;){
                    //If we have run_chunk or more pixels left to move,
                    //we will move run_chunk pixels, call handle_events(), and loop back up here.

                    //Or, if we have less than run_chunk pixels left to move,
                    //we will move whatever pixels we have left and call handle_events() once more.
                    if(i<run_chunk){
                        run_chunk=i;
                        i=0;
                    }

                    //Move.
                    if(move_state==UP){
                        y-=run_chunk;
                    }
                    else if(move_state==DOWN){
                        y+=run_chunk;
                    }

                    //If we still have pixels left to move.
                    if(i!=0){
                        i-=run_chunk;
                    }

                    handle_events();
                    events_handled=true;
                }
            }

            //*****************//
            // Handle looking: //
            //*****************//

            //If the player is not in look mode, bring the look offsets closer to the player.
            if(!LOOKING){
                //Move the x look offet either 4 times or until it reaches 0, whichever comes first.
                for(short i=0;i<look_speed && look_offset_x!=0;i++){
                    if(look_offset_x<0){
                        look_offset_x++;
                    }
                    else if(look_offset_x>0){
                        look_offset_x--;
                    }
                }

                //Move the y look offet either 4 times or until it reaches 0, whichever comes first.
                for(int i=0;i<look_speed && look_offset_y!=0;i++){
                    if(look_offset_y<0){
                        look_offset_y++;
                    }
                    else if(look_offset_y>0){
                        look_offset_y--;
                    }
                }
            }
        }
        //If the player is dying.
        else{
            if(collision_check(x,y,w,h,0,0,level.level_x,level.level_y)){
                double chunk;
                if(fabs(air_velocity)<pixel_safety_y){
                    chunk=fabs(air_velocity);
                }
                else{
                    chunk=pixel_safety_y;
                }

                //First, translate the player based on his air velocity.
                for(double i=fabs(air_velocity);i>0;){
                    //If we have air_chunk or more pixels left to move,
                    //we will move air_chunk pixels, call handle_events(), and loop back up here.

                    //Or, if we have less than air_chunk pixels left to move,
                    //we will move whatever pixels we have left and call handle_events() once more.
                    if(i<chunk){
                        chunk=i;
                        i=0;
                    }

                    //Move.
                    if(air_velocity<0){
                        y-=chunk;
                    }
                    else if(air_velocity>0){
                        y+=chunk;
                    }

                    //If we still have pixels left to move.
                    if(i!=0){
                        i-=chunk;
                    }

                    handle_events();
                    events_handled=true;
                }

                //Then, we handle gravity, which will affect the next tick.

                //If a jump is in progress, gravity is handled there, in handle_input().
                //Otherwise, we handle gravity normally.
                //As long as air_velocity hasn't exceeded jump_max, the maximum speed an object can fall, add gravity to air_velocity.
                if(air_velocity<gravity_max){
                    air_velocity+=gravity;
                }
                if(air_velocity>gravity_max){
                    air_velocity=gravity_max;
                }

                if(fabs(death_speed)<pixel_safety_x){
                    chunk=fabs(death_speed);
                }
                else{
                    chunk=pixel_safety_x;
                }

                for(double i=fabs(death_speed);i>0;){
                    //If we have run_chunk or more pixels left to move,
                    //we will move run_chunk pixels, call handle_events(), and loop back up here.

                    //Or, if we have less than run_chunk pixels left to move,
                    //we will move whatever pixels we have left and call handle_events() once more.
                    if(i<chunk){
                        chunk=i;
                        i=0;
                    }

                    //Move.
                    if(death_direction){
                        x-=chunk;
                    }
                    else{
                        x+=chunk;
                    }

                    //If we still have pixels left to move.
                    if(i!=0){
                        i-=chunk;
                    }

                    handle_events();
                    events_handled=true;
                }
            }
        }
    }
    //If in bubble mode.
    else{
        double bubble_max_speed=BUBBLE_MAX_SPEED_ON_CAMERA;
        if(!collision_check(x,y,w,h,camera_x,camera_y,camera_w,camera_h)){
            bubble_max_speed=BUBBLE_MAX_SPEED_OFF_CAMERA;
        }

        if(fabs(cam_focused_x()-x)>=bubble_move_x){
            x+=bubble_move_x;
        }
        else{
            x=cam_focused_x();
        }

        if(fabs(cam_focused_y()-y)>=bubble_move_y){
            y+=bubble_move_y;
        }
        else{
            y=cam_focused_y();
        }

        if(fabs(cam_focused_x()-x)!=0.0){
            if(cam_focused_x()<x){
                if(bubble_move_x<=0.0){
                    bubble_move_x-=BUBBLE_ACCEL;
                }
                else{
                    bubble_move_x-=BUBBLE_DECEL;
                }
            }
            else{
                if(bubble_move_x>=0.0){
                    bubble_move_x+=BUBBLE_ACCEL;
                }
                else{
                    bubble_move_x+=BUBBLE_DECEL;
                }
            }
        }
        if(bubble_move_x<bubble_max_speed*-1){
            bubble_move_x=bubble_max_speed*-1;
        }
        if(bubble_move_x>bubble_max_speed){
            bubble_move_x=bubble_max_speed;
        }

        if(fabs(cam_focused_y()-y)!=0.0){
            if(cam_focused_y()<y){
                if(bubble_move_y<=0.0){
                    bubble_move_y-=BUBBLE_ACCEL;
                }
                else{
                    bubble_move_y-=BUBBLE_DECEL;
                }
            }
            else{
                if(bubble_move_y>=0.0){
                    bubble_move_y+=BUBBLE_ACCEL;
                }
                else{
                    bubble_move_y+=BUBBLE_DECEL;
                }
            }
        }
        if(bubble_move_y<bubble_max_speed*-1){
            bubble_move_y=bubble_max_speed*-1;
        }
        if(bubble_move_y>bubble_max_speed){
            bubble_move_y=bubble_max_speed;
        }

        if(!cam_focused_dying() && collision_check(x+w/4.0,y+h/8.0,w/2.0,h/4.0,cam_focused_x()+cam_focused_w()/4.0,cam_focused_y()+cam_focused_h()/8.0,cam_focused_w()/2.0,cam_focused_h()/4.0)){
            bubble_mode=false;
            air_velocity=-jump_min*1.5;
            IN_AIR=true;
            run_speed=0.0;

            y=cam_focused_y();

            //Prevent the constant bubble popping sound if we are moving the camera around with dev controls.
            if(cam_state==CAM_STICKY){
                play_positional_sound(sound_system.player_bubble_pop);
            }
        }
    }

    if(!bubble_mode){
        if(!events_handled){
            handle_events();
        }

        if(started_frame_on_ground && IN_AIR){
            counter_jump_mercy=JUMP_MERCY_TIME;
        }
    }

    sucked_left=false;
    sucked_right=false;
}

void Player::handle_tiles(int check_x_start,int check_x_end,int check_y_start,int check_y_end){
    for(int int_y=check_y_start;int_y<check_y_end;int_y++){
        for(int int_x=check_x_start;int_x<check_x_end;int_x++){
            //As long as the current tile is within the level's boundaries.
            if(int_x>=0 && int_x<=(level.level_x/TILE_SIZE)-1 && int_y>=0 && int_y<=(level.level_y/TILE_SIZE)-1){
                //If the player is not climbing.
                if(!CLIMBING){

                    if(!cheat_noclip){
                        handle_collision_tiles(int_x,int_y,CROUCHING);
                    }

                    handle_tile_hazard(int_x,int_y);
                }
                //If the player is currently climbing.
                else{
                    if(!cheat_noclip){
                        handle_climbing(int_x,int_y);
                    }
                }
            }
        }
    }
}

void Player::handle_tile_hazard(int int_x,int int_y){
    if(level.tile_array[int_x][int_y].special==TILE_SPECIAL_HAZARD){
        if(collision_check(return_x(),return_y(),return_w(),return_h(),level.tile_array[int_x][int_y].return_x(),level.tile_array[int_x][int_y].return_y(),TILE_SIZE,TILE_SIZE)){
            //If the player was not already dying, and is not invulnerable.
            if(!DYING && !invulnerable){
                stat_deaths_traps++;
            }
            handle_death(level.tile_array[int_x][int_y].return_x(),level.tile_array[int_x][int_y].return_y(),TILE_SIZE,TILE_SIZE);
        }
    }
    //If the tile is a water tile and water tiles are deadly.
    else if(deadly_water && !suit_deadly_water && SWIMMING && level.tile_array[int_x][int_y].special==TILE_SPECIAL_WATER){
        if(collision_check(return_x(),return_y(),return_w(),return_h(),level.tile_array[int_x][int_y].return_x(),level.tile_array[int_x][int_y].return_y(),TILE_SIZE,TILE_SIZE)){
            //If the player was not already dying, and is not invulnerable.
            if(!DYING && !invulnerable){
                stat_deaths_traps++;
            }
            handle_death(level.tile_array[int_x][int_y].return_x(),level.tile_array[int_x][int_y].return_y(),TILE_SIZE,TILE_SIZE);
        }
    }
}

void Player::handle_events(bool being_pushed_up){
    prepare_for_events();

    //The current tile location for the actor.
    int actor_current_x=(int)((int)x/TILE_SIZE);
    int actor_current_y=(int)((int)y/TILE_SIZE);

    //Check all tiles in a square around the actor.
    int check_x_start=actor_current_x-4;
    int check_x_end=actor_current_x+4;
    int check_y_start=actor_current_y-4;
    int check_y_end=actor_current_y+4;

    //If the player is alive.
    if(!DYING){
        //**********************************//
        // Check for collisions with items: //
        //**********************************//
        for(int i=0;i<vector_items.size();i++){
            if(fabs(vector_items[i].x-x)<PROCESS_RANGE && fabs(vector_items[i].y-y)<PROCESS_RANGE){
                //Only do collision checks for the item if it exists.
                if(vector_items[i].exists){
                    if(collision_check(x,y,w,h,vector_items[i].x,vector_items[i].y,vector_items[i].w,vector_items[i].h)){
                        if(!(vector_items[i].type==ITEM_SPAWNPOINT || vector_items[i].type==ITEM_CHECKPOINT || vector_items[i].type==ITEM_ENDPOINT)){
                            //The item no longer exists.
                            vector_items[i].exists=false;

                            //If the item is a perma-item.
                            /**if(vector_items[i].type==ITEM_LEAF || vector_items[i].type==ITEM_CHEESE ||
                               (vector_items[i].type>=ITEM_SWIMMING_GEAR && vector_items[i].type<=ITEM_SINK) ||
                               vector_items[i].type==ITEM_AMMO_BARREL ||
                               (vector_items[i].type>=ITEM_KEY_GRAY && vector_items[i].type<=ITEM_KEY_CYAN)){
                                if(persistent_level_data){
                                    need_to_save_level_data=true;
                                }
                            }*/
                        }

                        if(vector_items[i].type==ITEM_LEAF){
                            leaves++;
                            current_level_leaves++;

                            gain_score(SCORES_ITEMS[ITEM_LEAF],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);

                            //Play the leaf item collection sound.
                            play_positional_sound(sound_system.item_collect_leaf,x,y);
                        }

                        else if(vector_items[i].type==ITEM_CHEESE){
                            cheese++;
                            current_level_cheese++;

                            gain_score(SCORES_ITEMS[ITEM_CHEESE],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);

                            //Play the cheese item collection sound.
                            play_positional_sound(sound_system.item_collect_cheese,x,y);
                        }

                        else if(vector_items[i].type==ITEM_AMMO){
                            if(game_mode_is_multiplayer()){
                                int player_count=1+mp_players.size();

                                int ammo_per_player=floor((double)return_ammo_box_amount()/(double)player_count);
                                int difference=return_ammo_box_amount()%player_count;

                                ammo+=ammo_per_player+difference;

                                for(int n=0;n<mp_players.size();n++){
                                    mp_players[n].ammo+=ammo_per_player;
                                }
                            }
                            else{
                                ammo+=return_ammo_box_amount();
                            }

                            ///gain_score(SCORES_ITEMS[ITEM_AMMO],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);

                            stat_ammo_picked_up++;

                            //Play the ammo box collection sound.
                            play_positional_sound(sound_system.item_collect_ammo,x,y);
                        }

                        else if(vector_items[i].type==ITEM_J_BALLOON){
                            counter_jump_mode=JUMP_MODE_TIME;

                            gain_score(SCORES_ITEMS[ITEM_J_BALLOON],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);

                            play_positional_sound(sound_system.item_collect_j_balloon,x,y);
                            play_balloonin_sound(sound_system.balloonin);
                        }

                        else if(vector_items[i].type==ITEM_AMMO_BARREL){
                            if(game_mode_is_multiplayer()){
                                int player_count=1+mp_players.size();

                                int ammo_per_player=floor((double)return_ammo_barrel_amount()/(double)player_count);
                                int difference=return_ammo_barrel_amount()%player_count;

                                ammo+=ammo_per_player+difference;

                                for(int n=0;n<mp_players.size();n++){
                                    mp_players[n].ammo+=ammo_per_player;
                                }
                            }
                            else{
                                ammo+=return_ammo_barrel_amount();
                            }

                            ///gain_score(SCORES_ITEMS[ITEM_AMMO_BARREL],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);

                            play_positional_sound(sound_system.item_collect_ammo_barrel,x,y);
                        }

                        else if(vector_items[i].type==ITEM_CANDY){
                            gain_score(return_candy_score(vector_items[i].score_bonus),vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);

                            play_positional_sound(sound_system.item_collect_candy,x,y);
                        }

                        else if((vector_items[i].type>=ITEM_SWIMMING_GEAR && vector_items[i].type<=ITEM_SINK) ||
                                (vector_items[i].type>=ITEM_KEY_GRAY && vector_items[i].type<=ITEM_J_WING)){
                            //Find the next available inventory slot.
                            short next_available_slot=next_available_inventory_slot();

                            //If there is a free inventory slot.
                            if(next_available_slot!=-1){
                                //The item no longer exists.
                                vector_items[i].exists=false;

                                inventory.push_back(inventory_item());

                                inventory[inventory.size()-1].type=vector_items[i].type;

                                inventory[inventory.size()-1].slot=next_available_slot;

                                inventory[inventory.size()-1].name=name_inventory_item(vector_items[i].type);

                                //Create an inventory item notification slider.
                                sliders.push_back(Slider(vector_items[i].type,false));

                                bool new_special_item=false;

                                if(vector_items[i].type==ITEM_SWIMMING_GEAR){
                                    new_special_item=true;

                                    oxygen=oxygen_max_capacity;
                                    for(int mps=0;mps<mp_players.size();mps++){
                                        mp_players[mps].oxygen=mp_players[mps].oxygen_max_capacity;
                                    }

                                    gain_score(SCORES_ITEMS[ITEM_SWIMMING_GEAR],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);
                                }
                                else if(vector_items[i].type==ITEM_KEY_RED){
                                    gain_score(SCORES_ITEMS[ITEM_KEY_RED],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);
                                }
                                else if(vector_items[i].type==ITEM_KEY_BLUE){
                                    gain_score(SCORES_ITEMS[ITEM_KEY_BLUE],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);
                                }
                                else if(vector_items[i].type==ITEM_KEY_GREEN){
                                    gain_score(SCORES_ITEMS[ITEM_KEY_GREEN],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);
                                }
                                else if(vector_items[i].type==ITEM_KEY_YELLOW){
                                    gain_score(SCORES_ITEMS[ITEM_KEY_YELLOW],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);
                                }
                                else if(vector_items[i].type==ITEM_KEY_ORANGE){
                                    gain_score(SCORES_ITEMS[ITEM_KEY_ORANGE],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);
                                }
                                else if(vector_items[i].type==ITEM_KEY_PURPLE){
                                    gain_score(SCORES_ITEMS[ITEM_KEY_PURPLE],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);
                                }
                                else if(vector_items[i].type==ITEM_TOWEL){
                                    gain_score(SCORES_ITEMS[ITEM_TOWEL],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);
                                }
                                else if(vector_items[i].type==ITEM_KEY_GRAY){
                                    gain_score(SCORES_ITEMS[ITEM_KEY_GRAY],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);
                                }
                                else if(vector_items[i].type==ITEM_KEY_BROWN){
                                    gain_score(SCORES_ITEMS[ITEM_KEY_BROWN],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);
                                }
                                else if(vector_items[i].type==ITEM_KEY_BLACK){
                                    gain_score(SCORES_ITEMS[ITEM_KEY_BLACK],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);
                                }
                                else if(vector_items[i].type==ITEM_KEY_PINK){
                                    gain_score(SCORES_ITEMS[ITEM_KEY_PINK],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);
                                }
                                else if(vector_items[i].type==ITEM_KEY_CYAN){
                                    gain_score(SCORES_ITEMS[ITEM_KEY_CYAN],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);
                                }
                                else if(vector_items[i].type==ITEM_SINK){
                                    profile.update_achievements(ACHIEVEMENT_JOKE_ITEM);

                                    gain_score(SCORES_ITEMS[ITEM_SINK],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);
                                }
                                else if(vector_items[i].type==ITEM_SUIT_DEADLY_WATER){
                                    new_special_item=true;

                                    gain_score(SCORES_ITEMS[ITEM_SUIT_DEADLY_WATER],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);
                                }
                                else if(vector_items[i].type==ITEM_SUIT_SHARP){
                                    new_special_item=true;

                                    gain_score(SCORES_ITEMS[ITEM_SUIT_SHARP],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);
                                }
                                else if(vector_items[i].type==ITEM_SUIT_BANANA){
                                    new_special_item=true;

                                    gain_score(SCORES_ITEMS[ITEM_SUIT_BANANA],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);
                                }
                                else if(vector_items[i].type==ITEM_SHOT_HOMING){
                                    new_special_item=true;

                                    gain_score(SCORES_ITEMS[ITEM_SHOT_HOMING],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);
                                }
                                else if(vector_items[i].type==ITEM_TRANSLATOR){
                                    new_special_item=true;

                                    gain_score(SCORES_ITEMS[ITEM_TRANSLATOR],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);
                                }
                                else if(vector_items[i].type==ITEM_J_WING){
                                    new_special_item=true;

                                    gain_score(SCORES_ITEMS[ITEM_J_WING],vector_items[i].x+vector_items[i].w/2.0,vector_items[i].y);
                                }

                                if(new_special_item){
                                    check_special_items();
                                    for(int mps=0;mps<mp_players.size();mps++){
                                        mp_players[mps].check_special_items();
                                    }
                                }

                                if(vector_items[i].type==ITEM_SWIMMING_GEAR){
                                    //Play the swimming gear collection sound.
                                    play_positional_sound(sound_system.item_collect_swimming_gear,x,y);
                                }
                                else if((vector_items[i].type>=ITEM_KEY_RED && vector_items[i].type<=ITEM_KEY_PURPLE) ||
                                        (vector_items[i].type>=ITEM_KEY_GRAY && vector_items[i].type<=ITEM_KEY_CYAN)){
                                    //Play the key collection sound.
                                    play_positional_sound(sound_system.item_collect_key,x,y);
                                }
                                else if(vector_items[i].type==ITEM_TOWEL){
                                    //Play the towel collection sound.
                                    play_positional_sound(sound_system.item_collect_towel,x,y);
                                }
                                else if(vector_items[i].type==ITEM_SINK){
                                    //Play the sink collection sound.
                                    play_positional_sound(sound_system.item_collect_sink,x,y);
                                }
                                else if(vector_items[i].type==ITEM_SUIT_DEADLY_WATER){
                                    play_positional_sound(sound_system.item_collect_suit_deadly_water,x,y);
                                }
                                else if(vector_items[i].type==ITEM_SUIT_SHARP){
                                    play_positional_sound(sound_system.item_collect_suit_sharp,x,y);
                                }
                                else if(vector_items[i].type==ITEM_SUIT_BANANA){
                                    play_positional_sound(sound_system.item_collect_suit_banana,x,y);
                                }
                                else if(vector_items[i].type==ITEM_SHOT_HOMING){
                                    play_positional_sound(sound_system.item_collect_shot_homing,x,y);
                                }
                                else if(vector_items[i].type==ITEM_TRANSLATOR){
                                    play_positional_sound(sound_system.item_collect_translator,x,y);
                                }
                                else if(vector_items[i].type==ITEM_J_WING){
                                    play_positional_sound(sound_system.item_collect_j_wing,x,y);
                                }
                            }
                        }
                    }
                    if(vector_items[i].type==ITEM_SPAWNPOINT){
                        if(collision_check(x+w/4.0,y,w-w/2.0,h,vector_items[i].x+16,vector_items[i].y+16,1,16)){
                            if(game_mode==GAME_MODE_SP_SURVIVAL || game_mode==GAME_MODE_MP_SURVIVAL ||
                               game_mode==GAME_MODE_MP_ADVENTURE){
                                if(survival_escape && !survival_complete){
                                    survival_end_game(true);

                                    return;
                                }
                                else{
                                    bool player_respawned=false;

                                    for(int n=0;n<mp_players.size();n++){
                                        if(mp_players[n].DYING){
                                            mp_players[n].mp_reset(vector_items[i].x,vector_items[i].y-14);
                                            player_respawned=true;
                                        }
                                    }

                                    if(player_respawned){
                                        play_positional_sound(sound_system.player_respawn);
                                    }
                                }
                            }

                            if(ptr_player_image!=return_character_image()){
                                double recall_run_speed=run_speed;
                                double recall_air_velocity=air_velocity;
                                bool recall_IN_AIR=IN_AIR;

                                update_character();

                                run_speed=recall_run_speed;
                                air_velocity=recall_air_velocity;
                                IN_AIR=recall_IN_AIR;
                            }
                        }
                    }
                    if(vector_items[i].type==ITEM_CHECKPOINT){
                        if(collision_check(x,y,w,h,vector_items[i].x,vector_items[i].y-64*2,32,96+64*2)){
                            //Set the animation states for the old checkpoint.
                            if(current_checkpoint!=-1){
                                vector_items[current_checkpoint].checkpoint_reached=false;
                                vector_items[current_checkpoint].checkpoint_unreached=true;
                            }

                            //Set the animation states for the newly reached checkpoint.
                            vector_items[i].checkpoint_reached=true;
                            vector_items[i].checkpoint_unreached=false;

                            //If the checkpoint is a new one.
                            if(current_checkpoint!=i){
                                play_positional_sound(sound_system.item_checkpoint,x,y);
                                stat_checkpoints_activated++;
                            }

                            current_checkpoint=i;

                            if(game_mode==GAME_MODE_MP_ADVENTURE){
                                bool player_respawned=false;

                                for(int n=0;n<mp_players.size();n++){
                                    if(mp_players[n].DYING){
                                        mp_players[n].mp_reset(vector_items[i].x,vector_items[i].y+50);
                                        player_respawned=true;
                                    }
                                }

                                if(player_respawned){
                                    play_positional_sound(sound_system.player_respawn);
                                }
                            }
                        }
                        if(collision_check(x+w/4.0,y,w-w/2.0,h,vector_items[i].x+16,vector_items[i].y+80,1,16)){
                            if(ptr_player_image!=return_character_image()){
                                double recall_run_speed=run_speed;
                                double recall_air_velocity=air_velocity;
                                bool recall_IN_AIR=IN_AIR;

                                update_character();

                                run_speed=recall_run_speed;
                                air_velocity=recall_air_velocity;
                                IN_AIR=recall_IN_AIR;
                            }
                        }
                    }
                    //If the item is the end point, and the goal has not been crossed yet.
                    if(vector_items[i].type==ITEM_ENDPOINT && !goal_crossed){
                        if(collision_check(x,y,w,h,vector_items[i].x,vector_items[i].y,32,96)){
                            goal_crossed=true;
                            next_level=vector_items[i].goal_level_to_load;
                            UPDATE_RATE=DEFAULT_UPDATE_RATE/2.4;
                            SKIP_TICKS=1000.0/UPDATE_RATE;
                            counter_level_end=8*UPDATE_RATE;

                            if(!vector_items[i].goal_secret){
                                gain_score(SCORE_BEAT_LEVEL,x+w/2.0,y);
                            }
                            else{
                                gain_score(SCORE_BEAT_LEVEL*2,x+w/2.0,y);
                            }

                            if(counter_jump_mode>0){
                                gain_score(SCORE_LEVEL_END_BONUS_J_BALLOON,x+w/2.0,y);
                            }

                            for(int n=0;n<vector_npcs.size();n++){
                                if(!vector_npcs[n].friendly){
                                    vector_npcs[n].handle_death(true);
                                }
                            }
                            for(int n=0;n<vector_shots.size();n++){
                                if(!vector_shots[n].BOSS){
                                    vector_shots[n].dissipate();
                                }
                            }
                            for(int n=0;n<vector_traps.size();n++){
                                if(!vector_traps[n].BOSS){
                                    vector_traps[n].active=false;
                                    vector_traps[n].dangerous=false;
                                }
                            }

                            int random=random_range(0,99);
                            if(random>=0 && random<5){
                                level.explode_party_balls(ITEM_AMMO);
                            }
                            else{
                                level.explode_party_balls(ITEM_CANDY);
                            }

                            if(game_mode==GAME_MODE_MP_ADVENTURE){
                                bool player_respawned=false;

                                for(int n=0;n<mp_players.size();n++){
                                    if(mp_players[n].DYING){
                                        mp_players[n].mp_reset(vector_items[i].x,vector_items[i].y+50);
                                        player_respawned=true;
                                    }
                                }

                                if(player_respawned){
                                    play_positional_sound(sound_system.player_respawn);
                                }
                            }

                            music.stop_track(0.25);
                            play_positional_sound(sound_system.goal_reached);
                        }
                    }
                }
            }
        }

        //*********************************************//
        // Check for collisions with moving platforms: //
        //*********************************************//
        if(!cheat_noclip){
            handle_collision_moving_platforms(TILE_SOLIDITY_CLOUD,false);
        }

        //*********************************//
        // Check for collisions with npcs: //
        //*********************************//
        for(int i=0;i<vector_npcs.size();i++){
            if(fabs(vector_npcs[i].x-x)<PROCESS_RANGE && fabs(vector_npcs[i].y-y)<PROCESS_RANGE){
                if(vector_npcs[i].exists){
                    if(!CLIMBING && !vector_npcs[i].CLIMBING){
                        if(!cheat_noclip){
                            short solidity;
                            if(!SWIMMING && vector_npcs[i].act_as_platform && !vector_npcs[i].ethereal){
                                solidity=TILE_SOLIDITY_CLOUD;
                            }
                            else{
                                solidity=TILE_SOLIDITY_PASSABLE;
                            }
                            handle_collision_solid(vector_npcs[i].x,vector_npcs[i].y,vector_npcs[i].w,vector_npcs[i].h,solidity,vector_npcs[i].type);

                            handle_npc_platform(i);
                        }
                    }

                    //If the player touches any part of the npc, and the npc exists and is deadly to the touch.
                    if(!SLIDING && ((!vector_npcs[i].special_attack_in_progress && vector_npcs[i].deadly_to_touch) || (vector_npcs[i].special_attack_in_progress && vector_npcs[i].deadly_while_special_attacking)) &&
                       collision_check(return_x(),return_y(),return_w(),return_h(),vector_npcs[i].return_x(),vector_npcs[i].return_y(),vector_npcs[i].return_w(),vector_npcs[i].return_h())){
                        //If the player was not already dying, and is not invulnerable.
                        if(!DYING && !invulnerable){
                            if(vector_npcs[i].counts_as_trap){
                                stat_deaths_traps++;
                            }
                            else{
                                stat_deaths_enemies++;
                            }
                        }
                        //The player dies!
                        handle_death(vector_npcs[i].return_x(),vector_npcs[i].return_y(),vector_npcs[i].return_w(),vector_npcs[i].return_h());
                    }
                    else if(SLIDING && !vector_npcs[i].counts_as_trap &&
                            collision_check(return_x(),return_y(),return_w(),return_h(),vector_npcs[i].return_x(),vector_npcs[i].return_y(),vector_npcs[i].return_w(),vector_npcs[i].return_h())){
                        //If the npc is not invulnerable.
                        if(!vector_npcs[i].invulnerable){
                            stat_enemies_stunned++;
                            special_count_kills_this_level++;
                        }

                        vector_npcs[i].handle_death();
                    }
                }
            }
        }

        //******************************************//
        // Check for collisions with other players: //
        //******************************************//
        for(int i=0;i<mp_players.size();i++){
            if(!CLIMBING && !CROUCHING && !mp_players[i].DYING && !mp_players[i].bubble_mode && !mp_players[i].CLIMBING && !mp_players[i].CROUCHING){
                if(!cheat_noclip){
                    short solidity;
                    if(!SWIMMING){
                        solidity=TILE_SOLIDITY_CLOUD;
                    }
                    else{
                        solidity=TILE_SOLIDITY_PASSABLE;
                    }
                    handle_collision_solid(mp_players[i].return_x(),mp_players[i].y,mp_players[i].return_w(),mp_players[i].h,solidity,NPC_END);

                    ///handle_player_platform(i);

                    if(!mp_players[i].SWIMMING){
                        if(push_actor_up(&mp_players[i],mp_players[i].return_x(),mp_players[i].return_w(),being_pushed_up,NPC_END)){
                            mp_players[i].handle_events(true);
                        }
                    }
                }
            }
        }

        //**********************************//
        // Check for collisions with doors: //
        //**********************************//
        if(!cheat_noclip){
            handle_collision_doors();
        }

        //************************************//
        // Check for collisions with springs: //
        //************************************//
        if(!cheat_noclip){
            handle_collision_springs(TILE_SOLIDITY_SOLID);
        }

        //*************************************//
        // Check for collisions with boosters: //
        //*************************************//
        handle_collision_boosters();

        //**********************************//
        // Check for collisions with traps: //
        //**********************************//
        collision_data_trap box_trap=handle_collision_traps(TILE_SOLIDITY_SOLID);
        if(box_trap.w!=-1.0){
            //If the player was not already dying, and is not invulnerable.
            if(!DYING && !invulnerable){
                stat_deaths_traps++;
            }
            handle_death(box_trap.x,box_trap.y,box_trap.w,box_trap.h);
        }

        //**********************************//
        // Check for collisions with tiles: //
        //**********************************//
        handle_tiles(check_x_start,check_x_end,check_y_start,check_y_end);

        //If the player is underwater.
        if(underwater){
            //If the player is out of oxygen, they have drowned.
            if(oxygen<=0){
                //If the player was not already dying, and is not invulnerable.
                if(!DYING && !invulnerable){
                    stat_deaths_drowning++;
                }
                handle_death(x,y,w,h);
            }
        }

        finish_events();
    }
    //If the player is dying.
    else{
        //*********************************//
        // Check for collisions with npcs: //
        //*********************************//
        for(int i=0;i<vector_npcs.size();i++){
            if(fabs(vector_npcs[i].x-x)<PROCESS_RANGE && fabs(vector_npcs[i].y-y)<PROCESS_RANGE){
                //Only do collision checks for the npc if it exists.
                if(vector_npcs[i].exists){
                    //If the player touches any part of the npc, and the npc is deadly to the touch.
                    if(((!vector_npcs[i].special_attack_in_progress && vector_npcs[i].deadly_to_touch) || (vector_npcs[i].special_attack_in_progress && vector_npcs[i].deadly_while_special_attacking)) &&
                       collision_check(return_x(),return_y(),return_w(),return_h(),vector_npcs[i].return_x(),vector_npcs[i].return_y(),vector_npcs[i].return_w(),vector_npcs[i].return_h())){
                        //The player dies!
                        handle_death(vector_npcs[i].return_x(),vector_npcs[i].return_y(),vector_npcs[i].return_w(),vector_npcs[i].return_h());
                    }
                }
            }
        }

        //**********************************//
        // Check for collisions with tiles: //
        //**********************************//
        for(int int_y=check_y_start;int_y<check_y_end;int_y++){
            for(int int_x=check_x_start;int_x<check_x_end;int_x++){
                //As long as the current tile is within the level's boundaries.
                if(int_x>=0 && int_x<=(level.level_x/TILE_SIZE)-1 && int_y>=0 && int_y<=(level.level_y/TILE_SIZE)-1){
                    handle_tile_hazard(int_x,int_y);
                }
            }
        }

        //**********************************//
        // Check for collisions with traps: //
        //**********************************//
        collision_data_trap box_trap=handle_collision_traps(TILE_SOLIDITY_PASSABLE);
        if(box_trap.w!=-1.0){
            handle_death(box_trap.x,box_trap.y,box_trap.w,box_trap.h);
        }

        if(y<0){
            y=0;
            //We set the air velocity to 0, so that the actor immediately begins falling once they bump their head on the top of the level.
            air_velocity=0;

            //The actor has bumped into a wall, so stop moving.
            if(SWIMMING){
                float_speed=0;
            }
        }
    }

    //If the player is not riding a cow.
    if(!riding_cow){
        timer_riding_cows.stop();
    }
}

void Player::handle_death(double death_x,double death_y,double death_w,double death_h,bool override_invulnerability){
    //As long as the player is not invulnerable and not already dying,
    //he begins dying.
    if((override_invulnerability || !invulnerable) && !DYING){
        DYING=true;
        frame_death=0;
        death_bounces=0;

        if(goal_crossed){
            profile.update_achievements(ACHIEVEMENT_DIE_AFTER_GOAL);
        }

        if(!game_mode_is_multiplayer()){
            stop_balloonin_sound();
        }

        //Make sure regular gravity applies when the player dies.
        gravity_max=10.0;
        gravity=0.5;

        /**if(game_beaten_all()){
            lose_score();
        }*/
    }

    //If the player is already dying, increment the death frame.
    if(DYING){
        if(++death_bounces<=6){
            frame_death++;
            if(frame_death>PLAYER_DEATH_SPRITES-1){
                frame_death=0;
            }

            handle_collision_solid(death_x,death_y,death_w,death_h,TILE_SOLIDITY_SOLID,NPC_END);
            if(x+w<=death_x){
                death_direction=true;
            }
            else if(x>=death_x+death_w){
                death_direction=false;
            }
            else{
                death_direction=random_range(0,1);
            }
            air_velocity=-1*(int)random_range(jump_min,jump_max);
            play_death_sound(sound_system.player_death);

            if(death_bounces>=6){
                profile.update_achievements(ACHIEVEMENT_MAX_DEATH_BOUNCES);
            }
        }
    }
}

void Player::set_camera(){
    //If the player is alive.
    if((game_mode_is_multiplayer() && !all_players_dead()) || (!game_mode_is_multiplayer() && !DYING)){
        int trap_width=CAMERA_TRAP_W;
        int trap_height=CAMERA_TRAP_H;
        if(on_worldmap()){
            trap_width=CAMERA_TRAP_WORLDMAP_W;
            trap_height=CAMERA_TRAP_WORLDMAP_H;
        }

        double get_cam_focused_x=cam_focused_x();
        double get_cam_focused_y=cam_focused_y();
        double get_cam_focused_w=cam_focused_w();
        double get_cam_focused_h=cam_focused_h();

        if(get_cam_focused_x+get_cam_focused_w>camera_trap_x+trap_width){
            for(int i=0;i<camera_lag_x;i++){
                camera_trap_x+=1.0;
                if(camera_trap_x>=get_cam_focused_x+get_cam_focused_w-trap_width){
                    camera_trap_x=get_cam_focused_x+get_cam_focused_w-trap_width;
                    break;
                }
            }
        }
        if(get_cam_focused_x<camera_trap_x){
            for(int i=0;i<camera_lag_x;i++){
                camera_trap_x-=1.0;
                if(camera_trap_x<=get_cam_focused_x){
                    camera_trap_x=get_cam_focused_x;
                    break;
                }
            }
        }
        if(get_cam_focused_y+get_cam_focused_h>camera_trap_y+trap_height){
            for(int i=0;i<camera_lag_y;i++){
                camera_trap_y+=1.0;
                if(camera_trap_y>=get_cam_focused_y+get_cam_focused_h-trap_height){
                    camera_trap_y=get_cam_focused_y+get_cam_focused_h-trap_height;
                    break;
                }
            }
        }
        if(get_cam_focused_y<camera_trap_y){
            for(int i=0;i<camera_lag_y;i++){
                camera_trap_y-=1.0;
                if(camera_trap_y<=get_cam_focused_y){
                    camera_trap_y=get_cam_focused_y;
                    break;
                }
            }
        }

        camera_delta_x=camera_x;
        camera_delta_y=camera_y;

        //If the camera is sticky, center the camera on the player.
        if(cam_state==CAM_STICKY){
            //If the camera sound timer is running, stop it.
            if(timer_camera_sound.is_started()){
                timer_camera_sound.stop();
            }

            //If look_state is nonzero, then the look offsets need to be moved.
            if(look_state==LEFT){
                look_offset_x-=look_speed;
            }
            else if(look_state==UP){
                look_offset_y-=look_speed;
            }
            else if(look_state==RIGHT){
                look_offset_x+=look_speed;
            }
            else if(look_state==DOWN){
                look_offset_y+=look_speed;
            }
            else if(look_state==LEFT_UP){
                look_offset_x-=look_speed;
                look_offset_y-=look_speed;
            }
            else if(look_state==LEFT_DOWN){
                look_offset_x-=look_speed;
                look_offset_y+=look_speed;
            }
            else if(look_state==RIGHT_UP){
                look_offset_x+=look_speed;
                look_offset_y-=look_speed;
            }
            else if(look_state==RIGHT_DOWN){
                look_offset_x+=look_speed;
                look_offset_y+=look_speed;
            }

            //If the look offsets go outside the maximum look bounds, bring them back within bounds.
            if(look_offset_x>200){
                look_offset_x=200;
            }
            else if(look_offset_x<-200){
                look_offset_x=-200;
            }
            if(look_offset_y>200){
                look_offset_y=200;
            }
            else if(look_offset_y<-200){
                look_offset_y=-200;
            }

            //Now center the camera on the camera trap.
            camera_x=(camera_trap_x+trap_width/2)-(camera_w/2)+look_offset_x;
            camera_y=(camera_trap_y+trap_height/2)-(camera_h/2)+look_offset_y;
        }

        //Move the camera if it is unsticky:
        if(cam_state!=CAM_STICKY){
            //If the camera sound timer is not started, start it.
            if(!timer_camera_sound.is_started()){
                timer_camera_sound.start();
            }

            //Now check for camera input and move the camera accordingly.
            if(cam_state==LEFT){
                camera_x-=camera_speed;
            }
            if(cam_state==UP){
                camera_y-=camera_speed;
            }
            if(cam_state==RIGHT){
                camera_x+=camera_speed;
            }
            if(cam_state==DOWN){
                camera_y+=camera_speed;
            }
            if(cam_state==LEFT_UP){
                camera_x-=camera_speed;
                camera_y-=camera_speed;
            }
            if(cam_state==RIGHT_UP){
                camera_x+=camera_speed;
                camera_y-=camera_speed;
            }
            if(cam_state==RIGHT_DOWN){
                camera_x+=camera_speed;
                camera_y+=camera_speed;
            }
            if(cam_state==LEFT_DOWN){
                camera_x-=camera_speed;
                camera_y+=camera_speed;
            }

            double cam_sound_speed=0.0;
            if(camera_speed==24){
                cam_sound_speed=500.0;
            }
            else if(camera_speed==96){
                cam_sound_speed=250.0;
            }

            //If 500 milliseconds have passed and the camera is moving.
            if(timer_camera_sound.get_ticks()>=cam_sound_speed && cam_state!=0){
                //Restart the camera sound timer.
                timer_camera_sound.start();

                //Play the camera movement sound.
                play_positional_sound(sound_system.camera_move);
            }
        }

        //If the camera goes past the bounds of the level map, set it back in the bounds.
        if(camera_x<0){
            camera_x=0;
        }
        if(camera_x+camera_w>level.level_x){
            camera_x=level.level_x-camera_w;
        }
        if(camera_y<0){
            camera_y=0;
        }
        if(camera_y+camera_h>level.level_y){
            camera_y=level.level_y-camera_h;
        }

        camera_delta_x=camera_x-camera_delta_x;
        camera_delta_y=camera_y-camera_delta_y;
    }
}

void Player::update_background(){
    //If the player is alive.
    if((game_mode_is_multiplayer() && !all_players_dead()) || (!game_mode_is_multiplayer() && !DYING)){
        for(int i=0;i<level.background_layers.size();i++){
            level.background_layers[i].update(camera_delta_x);
        }
    }
}

void Player::update_window_caption(int frame_rate,double ms_per_frame,int logic_frame_rate){
    string temp="";

    //Set the window caption.
    if(option_dev){
        msg="Hubert's Island Adventure: Mouse o' War (DEV Mode)";
    }
    else{
        msg="Hubert's Island Adventure: Mouse o' War";
    }

    //I commented this out as part of the removal of the profile system
    //If a profile exists.
    /**if(name!="\x1F"){
        msg+=" - ";
        msg+=name;
    }*/

    if(option_fps){
        ss.clear();ss.str("");ss<<"   FPS: ";ss<<frame_rate;msg+=ss.str();

        ss.clear();ss.str("");ss<<" / MSPF: ";ss<<ms_per_frame;msg+=ss.str();

        ss.clear();ss.str("");ss<<" / LUPS: ";ss<<logic_frame_rate;msg+=ss.str();
    }

    if(invulnerable){
        msg+=" Invulnerable!";
    }
    if(show_tracers){
        msg+=" Tracers!";
    }
    if(show_paths){
        msg+=" AI Paths!";
    }
    if(cheat_jump){
        msg+=" Jump mode!";
    }
    if(cheat_noclip){
        msg+=" Noclip!";
    }

    SDL_SetWindowTitle(main_window.screen,msg.c_str());
}

void Player::render_menu_pretties(){
    short moon_phase=return_moon_phase();

    if(!game_in_progress){
        double opacity=render_time_overlay();
        opacity+=opacity/8.0;
        if(opacity>1.0){
            opacity=1.0;
        }

        //Determine the date and time.
        time_t now;
        struct tm *tm_now;
        now=time(NULL);
        tm_now=localtime(&now);

        //Display the sun or moon.
        if(is_night()){
            render_rectangle(main_window.SCREEN_WIDTH-sprites_moon[moon_phase].w-15-15,0,sprites_moon[moon_phase].w+30,sprites_moon[moon_phase].h+30,1.0,COLOR_BLUE_OCEAN);
            render_rectangle(main_window.SCREEN_WIDTH-sprites_moon[moon_phase].w-15-13,2,sprites_moon[moon_phase].w+26,sprites_moon[moon_phase].h+26,opacity,COLOR_BLACK);
            render_sprite(main_window.SCREEN_WIDTH-sprites_moon[moon_phase].w-15,15,image.moon,&sprites_moon[moon_phase]);
        }
        else{
            render_rectangle(main_window.SCREEN_WIDTH-sprites_moon[moon_phase].w-15-15,0,sprites_moon[moon_phase].w+30,sprites_moon[moon_phase].h+30,1.0,COLOR_BLUE);
            render_rectangle(main_window.SCREEN_WIDTH-sprites_moon[moon_phase].w-15-13,2,sprites_moon[moon_phase].w+26,sprites_moon[moon_phase].h+26,1.0,COLOR_SKY_BLUE);
            render_texture(main_window.SCREEN_WIDTH-sprites_moon[moon_phase].w-15,15,image.sun);
        }

        //Display the time/date.
        char buff[BUFSIZ];
        strftime(buff,sizeof buff,"%H:%M:%S",tm_now);
        msg=buff;
        font.show(main_window.SCREEN_WIDTH-msg.length()*font.spacing_x-15+2,15+sprites_moon[moon_phase].h+20+2,msg,COLOR_BLACK);
        font.show(main_window.SCREEN_WIDTH-msg.length()*font.spacing_x-15,15+sprites_moon[moon_phase].h+20,msg,return_gui_color(holiday,3));
        strftime(buff,sizeof buff,"%A",tm_now);
        msg=buff;
        font.show(main_window.SCREEN_WIDTH-msg.length()*font.spacing_x-15+2,15+sprites_moon[moon_phase].h+20+font.spacing_y+2,msg,COLOR_BLACK);
        font.show(main_window.SCREEN_WIDTH-msg.length()*font.spacing_x-15,15+sprites_moon[moon_phase].h+20+font.spacing_y,msg,return_gui_color(holiday,3));
        strftime(buff,sizeof buff,"%B %d",tm_now);
        msg=buff;
        font.show(main_window.SCREEN_WIDTH-msg.length()*font.spacing_x-15+2,15+sprites_moon[moon_phase].h+20+font.spacing_y*2+2,msg,COLOR_BLACK);
        font.show(main_window.SCREEN_WIDTH-msg.length()*font.spacing_x-15,15+sprites_moon[moon_phase].h+20+font.spacing_y*2,msg,return_gui_color(holiday,3));
        strftime(buff,sizeof buff,"%Y",tm_now);
        msg=buff;
        font.show(main_window.SCREEN_WIDTH-msg.length()*font.spacing_x-15+2,15+sprites_moon[moon_phase].h+20+font.spacing_y*3+2,msg,COLOR_BLACK);
        font.show(main_window.SCREEN_WIDTH-msg.length()*font.spacing_x-15,15+sprites_moon[moon_phase].h+20+font.spacing_y*3,msg,return_gui_color(holiday,3));

        msg=main_menu_special;
        font.show(15+2,15+sprites_moon[moon_phase].h+20+2,msg,COLOR_BLACK);
        font.show(15,15+sprites_moon[moon_phase].h+20,msg,main_menu_special_color);
    }

    short colors[6];
    colors[0]=COLOR_RED;
    colors[1]=COLOR_YELLOW_FULL;
    colors[2]=COLOR_BLUE;
    colors[3]=COLOR_GREEN;
    colors[4]=COLOR_ORANGE_FULL;
    colors[5]=COLOR_PURPLE_FULL;

    string colored_message="";
    int msg_color=0;

    //Display the game title.
    if(menu_title_font){
        if(!game_in_progress){
            colored_message="Hubert's Island Adventure:";
            for(int i=0;i<colored_message.length();i++){
                font_large.show((main_window.SCREEN_WIDTH-colored_message.length()*font_large.spacing_x*2.0)/2.0+(i*font_large.spacing_x*2.0)+4,15+4,colored_message.substr(i,1),COLOR_BLACK,1.0,2.0);
                font_large.show((main_window.SCREEN_WIDTH-colored_message.length()*font_large.spacing_x*2.0)/2.0+(i*font_large.spacing_x*2.0),15,colored_message.substr(i,1),colors[msg_color],1.0,2.0);
                if(colored_message.substr(i,1)!=" " && ++msg_color>5){
                    msg_color=0;
                }
            }

            colored_message="Mouse o' War";
            for(int i=0;i<colored_message.length();i++){
                font_large.show((main_window.SCREEN_WIDTH-colored_message.length()*font_large.spacing_x*2.0)/2.0+(i*font_large.spacing_x*2.0)+4,47+4,colored_message.substr(i,1),COLOR_BLACK,1.0,2.0);
                font_large.show((main_window.SCREEN_WIDTH-colored_message.length()*font_large.spacing_x*2.0)/2.0+(i*font_large.spacing_x*2.0),47,colored_message.substr(i,1),colors[msg_color],1.0,2.0);
                if(colored_message.substr(i,1)!=" " && ++msg_color>5){
                    msg_color=0;
                }
            }

            msg_color=0;
            colored_message="Co-Op Edition";
            for(int i=0;i<colored_message.length();i++){
                font.show((main_window.SCREEN_WIDTH-colored_message.length()*font.spacing_x)*0.95+(i*font.spacing_x)+2,543+2,colored_message.substr(i,1),COLOR_BLACK);
                font.show((main_window.SCREEN_WIDTH-colored_message.length()*font.spacing_x)*0.95+(i*font.spacing_x),543,colored_message.substr(i,1),colors[msg_color]);
                if(colored_message.substr(i,1)!=" " && ++msg_color>5){
                    msg_color=0;
                }
            }

            msg_color=0;
            colored_message="co op edition";
            for(int i=0;i<colored_message.length();i++){
                font_sga.show((main_window.SCREEN_WIDTH-colored_message.length()*font.spacing_x)*0.95+(i*font.spacing_x)+2,575+2,colored_message.substr(i,1),COLOR_BLACK);
                font_sga.show((main_window.SCREEN_WIDTH-colored_message.length()*font.spacing_x)*0.95+(i*font.spacing_x),575,colored_message.substr(i,1),colors[msg_color]);
                if(colored_message.substr(i,1)!=" " && ++msg_color>5){
                    msg_color=0;
                }
            }
        }

        if(new_game_plus>0){
            msg_color=0;
            colored_message="New Game + "+num_to_roman_numeral(new_game_plus)+"!";
            for(int i=0;i<colored_message.length();i++){
                font.show((main_window.SCREEN_WIDTH-colored_message.length()*font.spacing_x)*0.95+(i*font.spacing_x)+2,15+sprites_moon[moon_phase].h+20+font.spacing_y*4+2,colored_message.substr(i,1),COLOR_BLACK);
                font.show((main_window.SCREEN_WIDTH-colored_message.length()*font.spacing_x)*0.95+(i*font.spacing_x),15+sprites_moon[moon_phase].h+20+font.spacing_y*4,colored_message.substr(i,1),colors[msg_color]);
                if(colored_message.substr(i,1)!=" " && ++msg_color>5){
                    msg_color=0;
                }
            }
        }
    }
    else{
        if(!game_in_progress){
            colored_message="Hubert's Island Adventure:";
            for(int i=0;i<colored_message.length();i++){
                font_large2.show((main_window.SCREEN_WIDTH-colored_message.length()*font_large.spacing_x*2.0)/2.0+(i*font_large.spacing_x*2.0)+4,15+4,colored_message.substr(i,1),COLOR_BLACK,1.0,2.0);
                font_large2.show((main_window.SCREEN_WIDTH-colored_message.length()*font_large.spacing_x*2.0)/2.0+(i*font_large.spacing_x*2.0),15,colored_message.substr(i,1),colors[msg_color],1.0,2.0);
                if(colored_message.substr(i,1)!=" " && ++msg_color>5){
                    msg_color=0;
                }
            }

            colored_message="Mouse o' War";
            for(int i=0;i<colored_message.length();i++){
                font_large2.show((main_window.SCREEN_WIDTH-colored_message.length()*font_large.spacing_x*2.0)/2.0+(i*font_large.spacing_x*2.0)+4,47+4,colored_message.substr(i,1),COLOR_BLACK,1.0,2.0);
                font_large2.show((main_window.SCREEN_WIDTH-colored_message.length()*font_large.spacing_x*2.0)/2.0+(i*font_large.spacing_x*2.0),47,colored_message.substr(i,1),colors[msg_color],1.0,2.0);
                if(colored_message.substr(i,1)!=" " && ++msg_color>5){
                    msg_color=0;
                }
            }

            msg_color=0;
            msg_color++;
            colored_message="Co-Op Edition";
            for(int i=0;i<colored_message.length();i++){
                font.show((main_window.SCREEN_WIDTH-colored_message.length()*font.spacing_x)*0.95+(i*font.spacing_x)+2,543+2,colored_message.substr(i,1),COLOR_BLACK);
                font.show((main_window.SCREEN_WIDTH-colored_message.length()*font.spacing_x)*0.95+(i*font.spacing_x),543,colored_message.substr(i,1),colors[msg_color]);
                if(colored_message.substr(i,1)!=" " && ++msg_color>5){
                    msg_color=0;
                }
            }

            msg_color=0;
            msg_color++;
            colored_message="co op edition";
            for(int i=0;i<colored_message.length();i++){
                font_sga.show((main_window.SCREEN_WIDTH-colored_message.length()*font.spacing_x)*0.95+(i*font.spacing_x)+2,575+2,colored_message.substr(i,1),COLOR_BLACK);
                font_sga.show((main_window.SCREEN_WIDTH-colored_message.length()*font.spacing_x)*0.95+(i*font.spacing_x),575,colored_message.substr(i,1),colors[msg_color]);
                if(colored_message.substr(i,1)!=" " && ++msg_color>5){
                    msg_color=0;
                }
            }
        }

        if(new_game_plus>0){
            msg_color=0;
            msg_color++;
            colored_message="New Game + "+num_to_roman_numeral(new_game_plus)+"!";
            for(int i=0;i<colored_message.length();i++){
                font.show((main_window.SCREEN_WIDTH-colored_message.length()*font.spacing_x)*0.95+(i*font.spacing_x)+2,15+sprites_moon[moon_phase].h+20+font.spacing_y*4+2,colored_message.substr(i,1),COLOR_BLACK);
                font.show((main_window.SCREEN_WIDTH-colored_message.length()*font.spacing_x)*0.95+(i*font.spacing_x),15+sprites_moon[moon_phase].h+20+font.spacing_y*4,colored_message.substr(i,1),colors[msg_color]);
                if(colored_message.substr(i,1)!=" " && ++msg_color>5){
                    msg_color=0;
                }
            }
        }
    }
}

void Player::render_background(){
    if(game_in_progress){
        render_texture(0,0,image.background_static);

        if(level.background_layers.size()<=1){
            if(!on_worldmap()){
                for(int i=0;i<vector_effect_firework.size();i++){
                    vector_effect_firework[i].render();
                }
            }
        }

        //Render the scrolling background layer(s).
        for(int i=0;i<level.background_layers.size();i++){
            level.background_layers[i].render(i);

            if(i==1){
                if(!on_worldmap()){
                    for(int i=0;i<vector_effect_firework.size();i++){
                        vector_effect_firework[i].render();
                    }
                }
            }
        }
    }
    else if(!game_in_progress && game_beginning_cutscene==0){
        render_texture(0,0,image.menu_background_0);
        for(int i=0;i<menu_backgrounds.size();i++){
            menu_backgrounds[i].render(i);
        }

        for(int i=0;i<vector_effect_firework.size();i++){
            vector_effect_firework[i].render();
        }

        render_foreground();

        render_menu_pretties();
    }
    else if(!game_in_progress && game_beginning_cutscene!=0){
        render_rectangle(0,0,main_window.SCREEN_WIDTH,main_window.SCREEN_HEIGHT,1.0,COLOR_BLACK);

        render_texture((main_window.SCREEN_WIDTH-image.logo_hubert.w)/2.0,15,image.logo_hubert);
    }
}

void Player::render_foreground(){
    if(game_in_progress){
        if(level.alarm){
            render_rectangle(0,0,main_window.SCREEN_WIDTH,main_window.SCREEN_HEIGHT,opacity_overlay_alarm,COLOR_RED);
        }

        if(night_mode){
            render_texture((int)(x-camera_x-DEFAULT_SCREEN_WIDTH+w/2),(int)(y-camera_y-DEFAULT_SCREEN_HEIGHT+h/2),image.overlay_night);
        }

        if(level.return_level_snow(current_level)){
            for(int i=0;i<overlay_snow.size();i++){
                overlay_snow[i].render(i);
            }
        }
        if(level.return_level_rain(current_level)){
            for(int i=0;i<overlay_rain.size();i++){
                overlay_rain[i].render(i);
            }
        }

        if((on_worldmap() && option_time_of_day) || (!on_worldmap() && option_time_of_day_levels)){
            render_time_overlay_fancy();
        }
    }
    else{
        if(option_holiday_cheer && holiday==HOLIDAY_CHRISTMAS){
            for(int i=0;i<overlay_snow.size();i++){
                overlay_snow[i].render(i);
            }
        }
    }
}

void Player::render_time_overlay_fancy(){
    double opacity=return_time_opacity();

    //Can adjust the opacity of unseen tiles here.
    double opacity_actual=opacity;
    if(opacity_actual>1.0){
        opacity_actual=1.0;
    }

    int camera_current_x=(int)((int)camera_x/LIGHTING_TILE_SIZE);
    int camera_current_y=(int)((int)camera_y/LIGHTING_TILE_SIZE);

    int check_x_start=camera_current_x;
    int check_x_end=camera_current_x+(int)((int)camera_w/LIGHTING_TILE_SIZE)+2;
    int check_y_start=camera_current_y;
    int check_y_end=camera_current_y+(int)((int)camera_h/LIGHTING_TILE_SIZE)+2;

    //Keeps track of whether each on-screen lighting tile has had its lighting
    //rectangle rendered yet.
    bool tile_lit[check_x_end-check_x_start][check_y_end-check_y_start];
    for(int int_x=0;int_x<check_x_end-check_x_start;int_x++){
        for(int int_y=0;int_y<check_y_end-check_y_start;int_y++){
            tile_lit[int_x][int_y]=false;
        }
    }

    for(int int_x=check_x_start;int_x<check_x_end;int_x++){
        for(int int_y=check_y_start;int_y<check_y_end;int_y++){
            if(int_x>=0 && int_x<=(level.level_x/LIGHTING_TILE_SIZE)-1 && int_y>=0 && int_y<=(level.level_y/LIGHTING_TILE_SIZE)-1){
                int tile_x=int_x*LIGHTING_TILE_SIZE;
                int tile_y=int_y*LIGHTING_TILE_SIZE;

                //If this tile has not already been lit.
                if(!tile_lit[int_x-check_x_start][int_y-check_y_start]){
                    //If true, we will render a rectangle over this tile and the three others
                    //that form a square.
                    bool tile_square=true;
                    if(int_x<(level.level_x/LIGHTING_TILE_SIZE)-1 && int_y<(level.level_y/LIGHTING_TILE_SIZE)-1 &&
                       int_x<check_x_end-1 && int_y<check_y_end-1){
                        for(int i=0;i<3;i++){
                            int light_check_x=0;
                            int light_check_y=0;

                            if(i==0){
                                light_check_x=int_x+1;
                                light_check_y=int_y;
                            }
                            else if(i==1){
                                light_check_x=int_x;
                                light_check_y=int_y+1;
                            }
                            else if(i==2){
                                light_check_x=int_x+1;
                                light_check_y=int_y+1;
                            }

                            if(tile_lit[light_check_x-check_x_start][light_check_y-check_y_start]){
                                tile_square=false;
                                break;
                            }
                            if(light_data[light_check_x][light_check_y].seen!=light_data[int_x][int_y].seen){
                                tile_square=false;
                                break;
                            }
                            if(light_data[light_check_x][light_check_y].light_intensity!=light_data[int_x][int_y].light_intensity){
                                tile_square=false;
                                break;
                            }
                            if(light_data[light_check_x][light_check_y].dimness!=light_data[int_x][int_y].dimness){
                                tile_square=false;
                                break;
                            }
                            if(light_data[light_check_x][light_check_y].color.red!=light_data[int_x][int_y].color.red){
                                tile_square=false;
                                break;
                            }
                            if(light_data[light_check_x][light_check_y].color.green!=light_data[int_x][int_y].color.green){
                                tile_square=false;
                                break;
                            }
                            if(light_data[light_check_x][light_check_y].color.blue!=light_data[int_x][int_y].color.blue){
                                tile_square=false;
                                break;
                            }
                        }
                    }
                    else{
                        tile_square=false;
                    }

                    if(!light_data[int_x][int_y].seen){
                        if(opacity_actual>0.0){
                            tile_lit[int_x-check_x_start][int_y-check_y_start]=true;

                            int dimension_mod=1;
                            if(tile_square){
                                dimension_mod=2;
                                tile_lit[int_x+1-check_x_start][int_y-check_y_start]=true;
                                tile_lit[int_x-check_x_start][int_y+1-check_y_start]=true;
                                tile_lit[int_x+1-check_x_start][int_y+1-check_y_start]=true;
                            }

                            render_rectangle((int)(tile_x-(int)camera_x),(int)(tile_y-(int)camera_y),LIGHTING_TILE_SIZE*dimension_mod,LIGHTING_TILE_SIZE*dimension_mod,opacity_actual,COLOR_BLACK);
                        }
                    }
                    else{
                        double dimness=light_data[int_x][int_y].dimness;
                        if(dimness>opacity_actual){
                            dimness=opacity_actual;
                        }

                        double light_opacity=0.20-light_data[int_x][int_y].light_intensity/2.0;
                        if(light_opacity<0.0){
                            light_opacity=0.0;
                        }

                        if((light_opacity>0.0 && dimness<opacity_actual) || dimness>0.0){
                            tile_lit[int_x-check_x_start][int_y-check_y_start]=true;

                            int dimension_mod=1;
                            if(tile_square){
                                dimension_mod=2;
                                tile_lit[int_x+1-check_x_start][int_y-check_y_start]=true;
                                tile_lit[int_x-check_x_start][int_y+1-check_y_start]=true;
                                tile_lit[int_x+1-check_x_start][int_y+1-check_y_start]=true;
                            }

                            Light_Data blended=level.blend_light(dimness,color_name_to_doubles(COLOR_BLACK),light_opacity,light_data[int_x][int_y].color);
                            render_rectangle((int)(tile_x-(int)camera_x),(int)(tile_y-(int)camera_y),LIGHTING_TILE_SIZE*dimension_mod,LIGHTING_TILE_SIZE*dimension_mod,blended.light_intensity,blended.color);
                        }
                    }
                }
            }
        }
    }
}

double Player::render_time_overlay(){
    double opacity=return_time_opacity();

    if(opacity>0.0){
        render_rectangle(0,0,main_window.SCREEN_WIDTH,main_window.SCREEN_HEIGHT,opacity,COLOR_BLACK);
    }

    return opacity;
}

double Player::return_time_opacity(){
    //Determine the date and time.
    time_t now;
    struct tm *tm_now;
    now=time(NULL);
    tm_now=localtime(&now);

    double opacity=0.0;

    if(is_night()){
        if(tm_now->tm_hour==18){
            opacity=0.25;
        }
        else if(tm_now->tm_hour==19){
            opacity=0.35;
        }
        else if(tm_now->tm_hour==20){
            opacity=0.45;
        }
        else if(tm_now->tm_hour==21){
            opacity=0.55;
        }
        else if(tm_now->tm_hour==22){
            opacity=0.65;
        }
        else if(tm_now->tm_hour==23 || (tm_now->tm_hour>=0 && tm_now->tm_hour<5)){
            opacity=0.75;
        }
        else if(tm_now->tm_hour==5){
            opacity=0.375;
        }

        if(tm_now->tm_hour>=18 || tm_now->tm_hour<5){
            opacity+=tm_now->tm_min*0.0016925;
        }
        else if(tm_now->tm_hour==5){
            opacity-=tm_now->tm_min/157.333;
        }

        if(opacity>0.75){
            opacity=0.75;
        }
        if(opacity<0.0){
            opacity=0.0;
        }
    }

    return opacity;
}

void Player::animate(){
    //Increment the frame counter.
    frame_counter_idle++;
    frame_counter++;
    frame_counter_swim++;
    frame_counter_jump++;
    frame_counter_shoot++;
    frame_counter_climb++;
    frame_counter_water++;
    frame_counter_powerup++;

    //Handle idle animation.
    if(frame_counter_idle>=15){
        frame_counter_idle=0;

        //Now increment the frame.
        frame_idle++;

        if(frame_idle>PLAYER_IDLE_SPRITES-1){
            frame_idle=0;
        }
    }

    //Handle player animation.
    if(frame_counter>=8-fabs(run_speed)){
        frame_counter=0;

        //Now increment the frame.
        frame++;

        //If the player is walking, play an occasional footstep sound.
        if(!SWIMMING && !IN_AIR && !SHOOTING && !CLIMBING && !DYING && !LOOKING && run_speed!=0.0 && frame%7==0){
            play_footstep_sound();
        }

        if(MOVING_PLATFORM_IN_WATER && fabs(moving_platform_x)>max_speed){
            //As long as the elements of the vector do not exceed the limit.
            if(vector_effect_water_splash.size()<option_effect_limit){
                vector_effect_water_splash.push_back(Effect_Water_Splash(x-3,y+h-32+12));
            }
        }

        if(frame>PLAYER_WALK_SPRITES-1){
            frame=0;
        }
    }

    //The swimming animation increments at different rates depending on whether or not the player is moving.
    short swim_frame_limit=0;
    if(move_state==0){
        swim_frame_limit=8;
    }
    else{
        swim_frame_limit=4;
    }

    //Handle swimming animation.
    if(frame_counter_swim>=swim_frame_limit){
        frame_counter_swim=0;

        //Now increment the frame.
        frame_swim++;

        if(frame_swim>PLAYER_SWIM_SPRITES-1){
            frame_swim=0;
        }
    }

    //Handle jumping animation.
    if((frame_counter_jump>=20 && frame_jump<3) || (frame_counter_jump>=30 && frame_jump==3) || (frame_counter_jump>=5 && frame_jump>=4)){
        frame_counter_jump=0;

        //Now increment the frame.
        frame_jump++;

        //If the player has reached the fear frame,
        //but is fairly close to the ground, skip the frame.
        if(frame_jump==3 && distance_to_ground()<=10){
            frame_jump=4;
        }
        //If the fear frame was not skipped, play the fear sound.
        if(IN_AIR && !SHOOTING && frame_jump==3){
            play_positional_sound(sound_system.player_fear,x,y);
        }

        if(frame_jump>PLAYER_JUMP_SPRITES-1){
            frame_jump=4;
        }
    }

    //Handle shooting animation.
    if(frame_counter_shoot>=15 && SHOOTING){
        frame_shoot++;

        if(frame_shoot>PLAYER_SHOOT_SPRITES-1){
            frame_shoot=0;
            SHOOTING=false;
            shoot_render_direction=0;
        }
    }

    //Handle climb animation.
    if(frame_counter_climb>=10){
        frame_counter_climb=0;

        //Now increment the frame.
        frame_climb++;

        if(frame_climb>PLAYER_CLIMB_SPRITES-1){
            frame_climb=1;
        }
    }

    //Handle water animation.
    if(frame_counter_water>=12){
        frame_counter_water=0;

        frame_water++;

        if(frame_water>WATER_SPRITES-1){
            frame_water=0;
        }
    }

    if(frame_counter_powerup>=10){
        frame_counter_powerup=0;

        frame_powerup++;

        if(frame_powerup>POWERUP_SPRITES_JUMP-1){
            frame_powerup=0;
        }
    }

    if(balloon_scale_direction_x){
        balloon_scale_x+=0.01;
        if(balloon_scale_x>1.2){
            balloon_scale_x=1.2;
            balloon_scale_direction_x=!balloon_scale_direction_x;
        }
    }
    else{
        balloon_scale_x-=0.01;
        if(balloon_scale_x<0.80){
            balloon_scale_x=0.80;
            balloon_scale_direction_x=!balloon_scale_direction_x;
        }
    }
    if(balloon_scale_direction_y){
        balloon_scale_y+=0.01;
        if(balloon_scale_y>1.2){
            balloon_scale_y=1.2;
            balloon_scale_direction_y=!balloon_scale_direction_y;
        }
    }
    else{
        balloon_scale_y-=0.01;
        if(balloon_scale_y<0.80){
            balloon_scale_y=0.80;
            balloon_scale_direction_y=!balloon_scale_direction_y;
        }
    }
}

void Player::update_menu_background(){
    //Update the title.
    menu_title_counter++;
    if(menu_title_counter>=0.5*UPDATE_RATE){
        menu_title_counter=0;
        menu_title_font=!menu_title_font;
    }

    if(!game_in_progress){
        menu_background_counter_x+=random_range(1,2);
        menu_background_counter_y+=random_range(1,2);

        if(menu_background_counter_x>=menu_background_counter_max_x){
            menu_background_counter_x=0;
            menu_background_counter_max_x=random_range(60,210);
            menu_speed_x=5.0/random_range(1,5);

            int random=random_range(0,2);
            if(menu_background_y==NONE){
                random=random_range(1,2);
            }
            if(random==0){
                menu_background_x=NONE;
            }
            else if(random==1){
                menu_background_x=LEFT;
            }
            else if(random==2){
                menu_background_x=RIGHT;
            }
        }
        if(menu_background_counter_y>=menu_background_counter_max_y){
            menu_background_counter_y=0;
            menu_background_counter_max_y=random_range(60,210);
            menu_speed_y=5.0/random_range(1,5);

            int random=random_range(0,2);
            if(menu_background_x==NONE){
                random=random_range(1,2);
            }
            if(random==0){
                menu_background_y=NONE;
            }
            else if(random==1){
                menu_background_y=UP;
            }
            else if(random==2){
                menu_background_y=DOWN;
            }
        }

        double fake_delta_x=0.0;
        double fake_delta_y=0.0;

        if(menu_background_x==LEFT){
            fake_delta_x=-menu_speed_x;
        }
        else if(menu_background_x==RIGHT){
            fake_delta_x=menu_speed_x;
        }
        if(menu_background_y==UP){
            fake_delta_y=-menu_speed_y;
        }
        else if(menu_background_y==DOWN){
            fake_delta_y=menu_speed_y;
        }

        for(int i=0;i<menu_backgrounds.size();i++){
            menu_backgrounds[i].update(fake_delta_x,fake_delta_y);
        }
    }
}

void Player::animate_overlays(){
    if(!fade_direction_overlay_alarm){
        opacity_overlay_alarm+=0.02;
    }
    else{
        opacity_overlay_alarm-=0.02;
    }

    if(opacity_overlay_alarm>=0.7){
        opacity_overlay_alarm=0.7;
        fade_direction_overlay_alarm=!fade_direction_overlay_alarm;
    }
    else if(opacity_overlay_alarm<=0.0){
        opacity_overlay_alarm=0.0;
        fade_direction_overlay_alarm=!fade_direction_overlay_alarm;
    }

    overlay_snow_counter_x+=random_range(1,2);
    if(overlay_snow_counter_x>=overlay_snow_counter_max_x){
        overlay_snow_counter_x=0;
        overlay_snow_counter_max_x=random_range(120,300);
        overlay_snow_speed_x=1.0/random_range(1,5);

        int random=random_range(0,99);
        if(random>=0 && random<20){
            overlay_snow_x=NONE;
        }
        else if(random>=20 && random<60){
            overlay_snow_x=LEFT;
        }
        else if(random>=60 && random<100){
            overlay_snow_x=RIGHT;
        }
    }
    double fake_delta_x_snow=0.0;
    if(overlay_snow_x==LEFT){
        fake_delta_x_snow=-overlay_snow_speed_x;
    }
    else if(overlay_snow_x==RIGHT){
        fake_delta_x_snow=overlay_snow_speed_x;
    }

    overlay_rain_counter_x+=random_range(1,2);
    if(overlay_rain_counter_x>=overlay_rain_counter_max_x){
        overlay_rain_counter_x=0;
        overlay_rain_counter_max_x=random_range(300,360);
        overlay_rain_speed_x=6.0/random_range(3,5);

        int random=random_range(0,99);
        if(random>=0 && random<20){
            overlay_rain_x=NONE;
        }
        else if(random>=20 && random<60){
            overlay_rain_x=LEFT;
        }
        else if(random>=60 && random<100){
            overlay_rain_x=RIGHT;
        }
    }
    double fake_delta_x_rain=0.0;
    if(overlay_rain_x==LEFT){
        fake_delta_x_rain=-overlay_rain_speed_x;
    }
    else if(overlay_rain_x==RIGHT){
        fake_delta_x_rain=overlay_rain_speed_x;
    }

    double snow_x=fake_delta_x_snow+camera_delta_x;
    double snow_y=overlay_snow_speed_y+camera_delta_y;
    double rain_x=fake_delta_x_rain+camera_delta_x;
    double rain_y=overlay_rain_speed_y+camera_delta_y;
    if(DYING){
        snow_x-=camera_delta_x;
        snow_y-=camera_delta_y;
        rain_x-=camera_delta_x;
        rain_y-=camera_delta_y;
    }

    for(int i=0;i<overlay_snow.size();i++){
        overlay_snow[i].update(snow_x,snow_y);
    }
    for(int i=0;i<overlay_rain.size();i++){
        overlay_rain[i].update(rain_x,rain_y);
    }
}

void Player::render(bool mirrored){
    double render_x=x;
    double render_y=y;
    if(mirrored && touching_mirror(x+MIRROR_DISTANCE_X,y+MIRROR_DISTANCE_Y,w,h)){
        render_x+=MIRROR_DISTANCE_X;
        render_y+=MIRROR_DISTANCE_Y;
    }

    //If the player is dying.
    if(DYING){
        render_sprite((int)((int)render_x-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_death[frame_death]);
    }
    //If the player is on the ground.
    else if(!SWIMMING && !IN_AIR && !SHOOTING && !CLIMBING && !LOOKING && !CROUCHING){
        if(run_speed==0.0 && facing==LEFT){
            render_sprite((int)((int)render_x-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_idle_left[frame_idle]);
        }
        else if(run_speed==0.0 && facing==RIGHT){
            render_sprite((int)((int)render_x-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_idle_right[frame_idle]);
        }
        else if(run_speed!=0.0 && facing==LEFT){
            render_sprite((int)((int)render_x-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_left[frame]);
        }
        else if(run_speed!=0.0 && facing==RIGHT){
            render_sprite((int)((int)render_x-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_right[frame]);
        }
    }
    //If the player is crouching.
    else if(CROUCHING){
        if(facing==LEFT || facing==LEFT_UP || facing==LEFT_DOWN){
            render_sprite((int)((int)render_x-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_crouch_left[0]);
        }
        else if(facing==RIGHT || facing==RIGHT_UP || facing==RIGHT_DOWN){
            render_sprite((int)((int)render_x-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_crouch_right[0]);
        }
    }
    //If the player is swimming.
    else if(SWIMMING){
        if(move_state==0 && facing==LEFT){
            render_sprite((int)((int)render_x-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_swim_left[frame_swim]);
        }
        else if(move_state==0 && facing==RIGHT){
            render_sprite((int)((int)render_x-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_swim_right[frame_swim]);
        }
        else if(move_state==UP && facing==RIGHT){
            render_sprite((int)((int)render_x-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_swim_up_right[frame_swim]);
        }
        else if(move_state==UP && facing==LEFT){
            render_sprite((int)((int)render_x-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_swim_up_left[frame_swim]);
        }
        else if(move_state==LEFT || move_state==LEFT_DOWN || move_state==LEFT_UP || (move_state==DOWN && facing==LEFT)){
            render_sprite((int)((int)render_x-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_swim_left[frame_swim]);
        }
        else if(move_state==RIGHT || move_state==RIGHT_DOWN || move_state==RIGHT_UP || (move_state==DOWN && facing==RIGHT)){
            render_sprite((int)((int)render_x-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_swim_right[frame_swim]);
        }
    }
    //If the player is in the air and not shooting.
    else if(IN_AIR && !SHOOTING){
        if(facing==RIGHT){
            render_sprite((int)((int)render_x-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_jump_right[frame_jump]);
        }
        else if(facing==LEFT){
            render_sprite((int)((int)render_x-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_jump_left[frame_jump]);
        }
    }
    //If the player is shooting and not in the air or climbing.
    else if(SHOOTING && !IN_AIR && !CLIMBING){
        if(shoot_render_direction==UP && facing==LEFT){
            render_sprite((int)((int)render_x-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_shoot_up_left[frame_shoot]);
        }
        else if(shoot_render_direction==UP && facing==RIGHT){
            render_sprite((int)((int)render_x-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_shoot_up_right[frame_shoot]);
        }
        else if(shoot_render_direction==DOWN && facing==LEFT){
            render_sprite((int)((int)render_x-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_shoot_down_left_air[frame_shoot]);
        }
        else if(shoot_render_direction==DOWN && facing==RIGHT){
            render_sprite((int)((int)render_x-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_shoot_down_right_air[frame_shoot]);
        }
        else if(shoot_render_direction==RIGHT){
            render_sprite((int)((int)render_x-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_shoot_right[frame_shoot]);
        }
        else if(shoot_render_direction==LEFT){
            render_sprite((int)((int)render_x-4-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_shoot_left[frame_shoot]);
        }
    }
    //If the player is shooting and in the air.
    else if(SHOOTING && IN_AIR){
        if(shoot_render_direction==UP && facing==LEFT){
            render_sprite((int)((int)render_x-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_shoot_up_left_air[frame_shoot]);
        }
        else if(shoot_render_direction==UP && facing==RIGHT){
            render_sprite((int)((int)render_x-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_shoot_up_right_air[frame_shoot]);
        }
        else if(shoot_render_direction==DOWN && facing==LEFT){
            render_sprite((int)((int)render_x-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_shoot_down_left_air[frame_shoot]);
        }
        else if(shoot_render_direction==DOWN && facing==RIGHT){
            render_sprite((int)((int)render_x-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_shoot_down_right_air[frame_shoot]);
        }
        else if(shoot_render_direction==RIGHT){
            render_sprite((int)((int)render_x-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_shoot_right_air[frame_shoot]);
        }
        else if(shoot_render_direction==LEFT){
            render_sprite((int)((int)render_x-4-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_shoot_left_air[frame_shoot]);
        }
    }
    //If the player is shooting and climbing.
    else if(SHOOTING && CLIMBING){
        if(shoot_render_direction==UP && facing==RIGHT){
            render_sprite((int)((int)render_x-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_shoot_climb_up_right[frame_shoot]);
        }
        else if(shoot_render_direction==UP && facing==LEFT){
            render_sprite((int)((int)render_x-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_shoot_climb_up_left[frame_shoot]);
        }
        else if(shoot_render_direction==DOWN && facing==RIGHT){
            render_sprite((int)((int)render_x-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_shoot_climb_down_right[frame_shoot]);
        }
        else if(shoot_render_direction==DOWN && facing==LEFT){
            render_sprite((int)((int)render_x-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_shoot_climb_down_left[frame_shoot]);
        }
        else if(shoot_render_direction==RIGHT){
            render_sprite((int)((int)render_x-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_shoot_climb_right[frame_shoot]);
        }
        else if(shoot_render_direction==LEFT){
            render_sprite((int)((int)render_x-4-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_shoot_climb_left[frame_shoot]);
        }
    }
    //If the player is climbing and not shooting or looking.
    else if(CLIMBING && !SHOOTING && !LOOKING){
        if((move_state!=UP && move_state!=DOWN) && facing==RIGHT){
            render_sprite((int)((int)render_x-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_climb_up_right[0]);
        }
        else if((move_state!=UP && move_state!=DOWN) && facing==LEFT){
            render_sprite((int)((int)render_x-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_climb_up_left[0]);
        }
        else if(move_state==UP && facing==RIGHT){
            render_sprite((int)((int)render_x-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_climb_up_right[frame_climb]);
        }
        else if(move_state==UP && facing==LEFT){
            render_sprite((int)((int)render_x-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_climb_up_left[frame_climb]);
        }
        else if(move_state==DOWN && facing==RIGHT){
            render_sprite((int)((int)render_x-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_climb_down_right[frame_climb]);
        }
        else if(move_state==DOWN && facing==LEFT){
            render_sprite((int)((int)render_x-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_climb_down_left[frame_climb]);
        }
    }
    //If the player is looking and not climbing.
    else if(LOOKING && !CLIMBING){
        if((look_state==0 && facing==RIGHT) || look_state==RIGHT){
            render_sprite((int)((int)render_x-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_look_right[0]);
        }
        else if((look_state==0 && facing==LEFT) || look_state==LEFT){
            render_sprite((int)((int)render_x-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_look_left[0]);
        }
        else if(look_state==RIGHT_UP || (look_state==UP && facing==RIGHT)){
            render_sprite((int)((int)render_x-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_look_right_up[0]);
        }
        else if(look_state==RIGHT_DOWN || (look_state==DOWN && facing==RIGHT)){
            render_sprite((int)((int)render_x-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_look_right_down[0]);
        }
        else if(look_state==LEFT_UP || (look_state==UP && facing==LEFT)){
            render_sprite((int)((int)render_x-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_look_left_up[0]);
        }
        else if(look_state==LEFT_DOWN || (look_state==DOWN && facing==LEFT)){
            render_sprite((int)((int)render_x-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_look_left_down[0]);
        }
    }
    //If the player is both looking and climbing.
    else if(LOOKING && CLIMBING){
        if((look_state==0 && facing==RIGHT) || look_state==RIGHT){
            render_sprite((int)((int)render_x-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_look_right_climb_down[0]);
        }
        else if((look_state==0 && facing==LEFT) || look_state==LEFT){
            render_sprite((int)((int)render_x-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_look_left_climb_down[0]);
        }
        else if(look_state==RIGHT_UP || (look_state==UP && facing==RIGHT)){
            render_sprite((int)((int)render_x-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_look_right_climb_up[0]);
        }
        else if(look_state==RIGHT_DOWN || (look_state==DOWN && facing==RIGHT)){
            render_sprite((int)((int)render_x-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_look_right_climb_down[0]);
        }
        else if(look_state==LEFT_UP || (look_state==UP && facing==LEFT)){
            render_sprite((int)((int)render_x-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_look_left_climb_up[0]);
        }
        else if(look_state==LEFT_DOWN || (look_state==DOWN && facing==LEFT)){
            render_sprite((int)((int)render_x-(int)camera_x),(int)((int)render_y-(int)camera_y),*ptr_player_image,&sprites_player_look_left_climb_down[0]);
        }
    }

    if(counter_jump_mode>0 && !DYING){
        double scale_y=1.0;

        if(CROUCHING){
            scale_y=0.5;
        }

        bool render_wings=true;

        if(counter_jump_mode<240 && counter_jump_mode>=120 && counter_jump_mode%10>=0 && counter_jump_mode%10<=5){
            render_wings=false;
        }
        else if(counter_jump_mode<120 && counter_jump_mode>=60 && counter_jump_mode%10>=6 && counter_jump_mode%10<=10){
            render_wings=false;
        }
        else if(counter_jump_mode<60 && counter_jump_mode>=0 && counter_jump_mode%2==0){
            render_wings=false;
        }

        if(render_wings){
            render_sprite((int)((int)x-8-(int)camera_x),(int)((int)y-(int)camera_y),image.powerup_jump,&sprites_powerup_jump[frame_powerup],1.0,1.0,scale_y);
        }
    }

    if(bubble_mode && !DYING){
        SDL_Rect balloon_rect;
        balloon_rect.x=0;
        balloon_rect.y=0;
        balloon_rect.w=BALLOON_W;
        balloon_rect.h=BALLOON_H;
        int balloon_width=(int)((double)BALLOON_W*balloon_scale_x);
        int balloon_height=(int)((double)BALLOON_H*balloon_scale_y);
        render_sprite((int)((int)x-(balloon_width-w)/2-(int)camera_x),(int)((int)y-(balloon_height-h)/2-(int)camera_y),image.balloon,&balloon_rect,1.0,balloon_scale_x,balloon_scale_y,0.0,get_player_color(which_mp_player));
    }

    ///render_rectangle((int)(camera_trap_x-camera_x),(int)(camera_trap_y-camera_y),CAMERA_TRAP_W,CAMERA_TRAP_H,0.5,COLOR_RED);

    ///render_rectangle((int)(x-camera_x),(int)(y-camera_y),w,h,0.5,COLOR_BLUE);
    ///render_rectangle((int)((int)return_x()-(int)camera_x),(int)((int)return_y()-(int)camera_y),return_w(),return_h(),0.5,COLOR_RED);
    ///render_rectangle((int)(x-camera_x+w/2),(int)(y-camera_y),1,h,0.5,COLOR_GREEN);
}

bool Player::check_inventory(short type){
    if(game_mode==GAME_MODE_SP_ADVENTURE || game_mode==GAME_MODE_MP_ADVENTURE){
        for(int i=0;i<inventory.size();i++){
            if(inventory[i].type==type){
                return true;
            }
        }
    }

    return false;
}

void Player::remove_item(short type){
    for(int i=0;i<inventory.size();i++){
        if(inventory[i].type==type){
            window_inventory[0].drop_dragged_item();
            inventory.erase(inventory.begin()+i);
            break;
        }
    }
}

short Player::next_available_inventory_slot(){
    short next_available_slot=-1;

    //If the inventory is empty, the first slot is empty.
    if(inventory.size()==0){
        next_available_slot=0;
    }
    //Otherwise, find the next empty slot.
    else{
        for(short check_slot=0;next_available_slot==-1 && check_slot<window_inventory[0].INVENTORY_MAX_SIZE;check_slot++){
            for(int i=0;i<inventory.size();i++){
                //If an item is in this slot.
                if(inventory[i].slot==check_slot){
                    break;
                }

                //If we are on the last item in the inventory, and no item was found occupying this slot,
                //this slot is empty and can be used.
                if(i==inventory.size()-1){
                    next_available_slot=check_slot;
                    break;
                }
            }
        }
    }

    return next_available_slot;
}

string Player::name_inventory_item(short item_type){
    if(item_type==ITEM_SWIMMING_GEAR){
        return "Swimming gear.\nDoes exactly what\nit says on the tin.";
    }
    else if(item_type==ITEM_KEY_RED){
        return "A red key.\nWhat could it open?";
    }
    else if(item_type==ITEM_KEY_BLUE){
        return "A blue key.\nWhat could it open?";
    }
    else if(item_type==ITEM_KEY_GREEN){
        return "A green key.\nWhat could it open?";
    }
    else if(item_type==ITEM_KEY_YELLOW){
        return "A yellow key.\nWhat could it open?";
    }
    else if(item_type==ITEM_KEY_ORANGE){
        return "An orange key.\nWhat could it open?";
    }
    else if(item_type==ITEM_KEY_PURPLE){
        return "A purple key.\nWhat could it open?";
    }
    else if(item_type==ITEM_TOWEL){
        return "A towel.\nThe most massively useful\nthing one can have.";
    }
    else if(item_type==ITEM_KEY_GRAY){
        return "A gray key.\nWhat could it open?";
    }
    else if(item_type==ITEM_KEY_BROWN){
        return "A brown key.\nWhat could it open?";
    }
    else if(item_type==ITEM_KEY_BLACK){
        return "A black key.\nWhat could it open?";
    }
    else if(item_type==ITEM_KEY_PINK){
        return "A pink key.\nWhat could it open?";
    }
    else if(item_type==ITEM_KEY_CYAN){
        return "A cyan key.\nWhat could it open?";
    }
    else if(item_type==ITEM_SINK){
        return "A kitchen sink.\nThis game's got everything,\nincluding this!";
    }
    else if(item_type==ITEM_SUIT_DEADLY_WATER){
        return "Hazard Suit.\nProtects the wearer\nfrom dangerous liquids.";
    }
    else if(item_type==ITEM_SUIT_SHARP){
        return "Spiky Suit.\nProtects the wearer\nfrom sharp things.";
    }
    else if(item_type==ITEM_SUIT_BANANA){
        return "Banana Suit.\nReflects bananas.\nLook! A seagull!";
    }
    else if(item_type==ITEM_SHOT_HOMING){
        return "Homing Shot Upgrade.\nAdds a homing setting\nto the stunner.";
    }
    else if(item_type==ITEM_TRANSLATOR){
        return "SGA Translator.\nTranslates signs written\nin the Standard Galactic Alphabet.";
    }
    else if(item_type==ITEM_J_WING){
        return "J-Wing.\nGrants the wearer\na double-jump!";
    }
}

void Player::gain_score(uint64_t score_to_gain,double x_location,double y_location,bool show_floater){
    if(game_mode==GAME_MODE_SP_ADVENTURE || game_mode==GAME_MODE_MP_ADVENTURE){
        //Add the points to the player's score.
        if(score+score_to_gain<=numeric_limits<uint64_t>::max()){
            score+=score_to_gain;
        }
        else{
            score=numeric_limits<uint64_t>::max();
        }
    }
    else if(game_mode==GAME_MODE_SP_SURVIVAL || game_mode==GAME_MODE_MP_SURVIVAL){
        if(survival_score+score_to_gain<=numeric_limits<uint64_t>::max()){
            survival_score+=score_to_gain;
        }
        else{
            survival_score=numeric_limits<uint64_t>::max();
        }
    }

    //As long as the elements of the vector do not exceed the limit.
    if(vector_effect_score_floater.size()<option_effect_limit){
        if(score_to_gain>0 && show_floater){
            vector_effect_score_floater.push_back(Effect_Score_Floater(score_to_gain,x_location,y_location));
        }
    }
}

void Player::lose_score(){
    if(game_mode==GAME_MODE_SP_ADVENTURE || game_mode==GAME_MODE_MP_ADVENTURE){
        if(score>0){
            score--;
        }
    }
    else if(game_mode==GAME_MODE_SP_SURVIVAL || game_mode==GAME_MODE_MP_SURVIVAL){
        if(survival_score>0){
            survival_score--;
        }
    }
}

void Player::play_death_sound(sound_data sound){
    int volume=128;

    volume=modify_sound_volume(volume);

    //If no channel is currently reserved.
    if(sound_channel_death==-1){
        sound_channel_death=get_free_channel();
    }
    //If a channel is already reserved.
    else{
        //Halt the channel before using it again.
        Mix_HaltChannel(sound_channel_death);
    }

    if(sound_channel_death!=-1){
        Mix_Volume(sound_channel_death,volume);
        Mix_PlayChannel(sound_channel_death,sound.chunk,0);
    }
}

void Player::play_balloonin_sound(sound_data sound){
    int volume=128;

    volume=modify_sound_volume(volume);

    //If no channel is currently reserved.
    if(sound_channel_balloonin==-1){
        sound_channel_balloonin=get_free_channel();
    }
    //If a channel is already reserved.
    else{
        //Halt the channel before using it again.
        Mix_HaltChannel(sound_channel_balloonin);
    }

    if(sound_channel_balloonin!=-1){
        music.stop_track(0.25);

        Mix_Volume(sound_channel_balloonin,volume);
        Mix_PlayChannel(sound_channel_balloonin,sound.chunk,0);
    }
}

void Player::stop_balloonin_sound(){
    //If a channel is currently reserved.
    if(sound_channel_balloonin!=-1){
        //Fade the channel out before using it again.
        Mix_FadeOutChannel(sound_channel_balloonin,500);
    }
}

bool Player::npc_is_main_boss(short type){
    if(type==NPC_BOSS_CASTLE || type==NPC_BOSS_HAUNTED_HOUSE || type==NPC_BOSS_VOLCANO ||
       type==NPC_BOSS_PYRAMID || type==NPC_BOSS_LIGHTHOUSE){
        return true;
    }
    else{
        return false;
    }
}

int Player::return_random_main_boss_level(){
    //If all bosses are already unlocked.
    if(return_main_bosses_unlocked()>=4){
        //This should not happen, but in case it does for some reason...
        return -1;
    }

    int random=random_range(0,3);
    if(random==0){
        return 5;
    }
    else if(random==1){
        return 7;
    }
    else if(random==2){
        return 10;
    }
    else if(random==3){
        return 12;
    }
}

int Player::return_main_bosses_unlocked(){
    //The number of bosses that are either unlocked OR defeated already.
    int bosses_unlocked=0;

    if(bosses[5]!=BOSS_STATE_NOT_SPAWNED){
        bosses_unlocked++;
    }
    if(bosses[7]!=BOSS_STATE_NOT_SPAWNED){
        bosses_unlocked++;
    }
    if(bosses[10]!=BOSS_STATE_NOT_SPAWNED){
        bosses_unlocked++;
    }
    if(bosses[12]!=BOSS_STATE_NOT_SPAWNED){
        bosses_unlocked++;
    }
    if(bosses[14]!=BOSS_STATE_NOT_SPAWNED){
        bosses_unlocked++;
    }

    return bosses_unlocked;
}

int Player::return_main_bosses_defeated(){
    //The number of bosses that are defeated already.
    int bosses_defeated=0;

    if(bosses[5]==BOSS_STATE_DEFEATED){
        bosses_defeated++;
    }
    if(bosses[7]==BOSS_STATE_DEFEATED){
        bosses_defeated++;
    }
    if(bosses[10]==BOSS_STATE_DEFEATED){
        bosses_defeated++;
    }
    if(bosses[12]==BOSS_STATE_DEFEATED){
        bosses_defeated++;
    }
    if(bosses[14]==BOSS_STATE_DEFEATED){
        bosses_defeated++;
    }

    return bosses_defeated;
}

int Player::return_ammo_box_amount(){
    int amount=5;

    if(game_mode==GAME_MODE_SP_ADVENTURE || game_mode==GAME_MODE_MP_ADVENTURE){
        short difficulty=return_actual_difficulty();

        if(game_mode==GAME_MODE_MP_ADVENTURE){
            int player_count=1+mp_players.size();
            difficulty-=player_count-1;
        }

        if(difficulty==DIFFICULTY_EASY){
            amount+=3;
        }
        /**else if(difficulty>=DIFFICULTY_HARD){
            amount-=(int)floor((double)(difficulty-DIFFICULTY_NORMAL)/1.5);
            if(amount<2){
                amount=2;
            }
        }*/

        if(get_upgrade_state("ammo_amount")){
            amount+=ceil((double)amount*0.25);
        }

        amount=ceil((double)amount*(1+((double)mp_players.size()*1.25)));
    }

    return amount;
}

int Player::return_ammo_barrel_amount(){
    int amount=100;

    if(game_mode==GAME_MODE_SP_ADVENTURE || game_mode==GAME_MODE_MP_ADVENTURE){
        short difficulty=return_actual_difficulty();

        if(game_mode==GAME_MODE_MP_ADVENTURE){
            int player_count=1+mp_players.size();
            difficulty-=player_count-1;
        }

        if(difficulty==DIFFICULTY_EASY){
            amount+=25;
        }
        /**else if(difficulty>=DIFFICULTY_HARD){
            amount-=(difficulty-DIFFICULTY_NORMAL)*12;
            if(amount<25){
                amount=25;
            }
        }*/

        if(get_upgrade_state("ammo_amount")){
            amount+=ceil((double)amount*0.25);
        }

        amount=ceil((double)amount*(1+((double)mp_players.size()*1.25)));
    }

    return amount;
}

uint64_t Player::return_candy_score(int score_bonus){
    uint64_t amount=SCORES_ITEMS[ITEM_CANDY];

    if(game_mode==GAME_MODE_SP_SURVIVAL || game_mode==GAME_MODE_MP_SURVIVAL){
        amount=SCORE_SURVIVAL_CANDY;
    }

    if(game_mode==GAME_MODE_SP_ADVENTURE || game_mode==GAME_MODE_MP_ADVENTURE){
        amount+=new_game_plus;

        uint64_t base_amount=amount;

        //If the moon is full.
        if(return_moon_phase()==MOON_PHASE_FULL){
            amount+=ceil((double)base_amount*0.25);
        }

        if(get_upgrade_state("candy_amount_1")){
            amount+=ceil((double)base_amount*0.25);
        }
        if(get_upgrade_state("candy_amount_2")){
            amount+=ceil((double)base_amount*0.5);
        }
        if(get_upgrade_state("candy_amount_3")){
            amount+=ceil((double)base_amount*0.75);
        }

        amount+=ceil((double)amount*(double)score_bonus*0.01);
    }

    return amount;
}

int Player::return_actual_difficulty(){
    //Determine the date and time.
    time_t now;
    struct tm *tm_now;
    now=time(NULL);
    tm_now=localtime(&now);

    if(game_mode==GAME_MODE_SP_ADVENTURE){
        int difficulty=option_difficulty;

        //If it is Friday the 13th.
        if(tm_now->tm_mday==13 && tm_now->tm_wday==5){
            difficulty++;
        }

        difficulty+=new_game_plus;

        if(option_difficulty==DIFFICULTY_HARD && difficulty>DIFFICULTY_HARD){
            difficulty+=(int)ceil((double)difficulty*0.25);
        }

        return difficulty;
    }
    else if(game_mode==GAME_MODE_SP_SURVIVAL || game_mode==GAME_MODE_MP_SURVIVAL){
        int difficulty=DIFFICULTY_NORMAL;

        return difficulty;
    }
    else if(game_mode==GAME_MODE_MP_ADVENTURE){
        int difficulty=option_difficulty;

        //If it is Friday the 13th.
        if(tm_now->tm_mday==13 && tm_now->tm_wday==5){
            difficulty++;
        }

        int player_count=1+mp_players.size();
        difficulty+=player_count-1;

        return difficulty;
    }
}

short Player::return_moon_phase(){
    //Determine the date and time.
    time_t now;
    struct tm *tm_now;
    now=time(NULL);
    tm_now=localtime(&now);

    int goldn=(tm_now->tm_year%19)+1;
    int epact=(11*goldn+18)%30;

    if((epact==25 && goldn>11) || epact==24){
        epact++;
    }

    short phase=((((((tm_now->tm_yday+epact)*6)+11)%177)/22)&7);

    //If the phase is not valid for some reason.
    if(!(phase>=MOON_PHASE_BEGIN && phase<MOON_PHASE_END)){
        phase=MOON_PHASE_FULL;
    }

    return phase;
}

bool Player::is_night(){
    //Determine the date and time.
    time_t now;
    struct tm *tm_now;
    now=time(NULL);
    tm_now=localtime(&now);

    if(tm_now->tm_hour>=18 || tm_now->tm_hour<6){
        return true;
    }

    return false;
}

bool Player::is_midnight(){
    //Determine the date and time.
    time_t now;
    struct tm *tm_now;
    now=time(NULL);
    tm_now=localtime(&now);

    if(tm_now->tm_hour==0){
        return true;
    }

    return false;
}

void Player::set_firework_rate(bool game_beaten_message){
    fireworks_rate=0;

    if((game_mode==GAME_MODE_SP_ADVENTURE || game_mode==GAME_MODE_MP_ADVENTURE) &&
       (game_beaten_message || new_game_plus>0)){
        if(game_beaten_message){
            fireworks_rate++;
        }

        fireworks_rate+=new_game_plus;

        if(achievements[ACHIEVEMENT_COLLECT_ALL_LEAVES]){
            fireworks_rate++;
        }
        if(achievements[ACHIEVEMENT_COLLECT_ALL_CHEESE]){
            fireworks_rate++;
        }
    }
    else{
        if(option_holiday_cheer && holiday==HOLIDAY_INDEPENDENCE){
            fireworks_rate++;
        }
    }

    if(fireworks_rate>3){
        fireworks_rate=3;
    }
}

void Player::create_firework(){
    if(vector_effect_firework.size()<option_effect_limit){
        int random=random_range(0,99);
        short color_random;
        if(random>=0 && random<50){
            color_random=COLOR_RAINBOW;
        }
        else if(random>=50 && random<57){
            color_random=COLOR_YELLOW;
        }
        else if(random>=57 && random<64){
            color_random=COLOR_ORANGE;
        }
        else if(random>=64 && random<71){
            color_random=COLOR_RED;
        }
        else if(random>=71 && random<78){
            color_random=COLOR_SHOCKING_PINK;
        }
        else if(random>=78 && random<85){
            color_random=COLOR_VIOLET;
        }
        else if(random>=85 && random<92){
            color_random=COLOR_BLUE;
        }
        else if(random>=92 && random<99){
            color_random=COLOR_GREEN;
        }
        else if(random>=99 && random<100){
            color_random=COLOR_INDIGO;
        }

        int thickness=random_range(450,500);
        double speed_y=0.5*random_range(4,15);

        if(!on_worldmap()){
            thickness=random_range(200,250);
            speed_y=0.5*random_range(10,15);
        }

        vector_effect_firework.push_back(Effect_Firework((double)random_range(200,main_window.SCREEN_WIDTH-200)+camera_x,(double)main_window.SCREEN_HEIGHT+camera_y,random_range(20,40),color_random,0,thickness,0.1*random_range(1,20),speed_y));
    }
}

image_data* Player::return_character_image(){
    image_data* ptr_image=&image.sprite_sheet_player;

    if(option_character==CHARACTER_HUBERT){
        ptr_image=&image.sprite_sheet_player;
    }
    else if(option_character==CHARACTER_SLIME_O){
        ptr_image=&image.sprite_sheet_character_slime_o;
    }
    else if(option_character==CHARACTER_SKETCH){
        ptr_image=&image.sprite_sheet_character_sketch;
    }
    else if(option_character==CHARACTER_PENNY){
        ptr_image=&image.sprite_sheet_character_penny;
    }

    return ptr_image;
}

image_data* Player::return_character_worldmap_image(){
    image_data* ptr_image=&image.sprite_sheet_player_worldmap;

    if(option_character==CHARACTER_HUBERT){
        ptr_image=&image.sprite_sheet_player_worldmap;
    }
    else if(option_character==CHARACTER_SLIME_O){
        ptr_image=&image.sprite_sheet_character_worldmap_slime_o;
    }
    else if(option_character==CHARACTER_SKETCH){
        ptr_image=&image.sprite_sheet_character_worldmap_sketch;
    }
    else if(option_character==CHARACTER_PENNY){
        ptr_image=&image.sprite_sheet_character_worldmap_penny;
    }

    return ptr_image;
}

image_data* Player::return_character_box_image(){
    image_data* ptr_image=&image.character_box_hubert;

    if(option_character==CHARACTER_HUBERT){
        ptr_image=&image.character_box_hubert;
    }
    else if(option_character==CHARACTER_SLIME_O){
        ptr_image=&image.character_box_slime_o;
    }
    else if(option_character==CHARACTER_SKETCH){
        ptr_image=&image.character_box_sketch;
    }
    else if(option_character==CHARACTER_PENNY){
        ptr_image=&image.character_box_penny;
    }

    return ptr_image;
}

sound_data* Player::return_character_sound_footstep(){
    sound_data* ptr_sound=&sound_system.player_footstep;

    if(option_character==CHARACTER_HUBERT){
        ptr_sound=&sound_system.player_footstep;
    }
    else if(option_character==CHARACTER_SKETCH){
        ptr_sound=&sound_system.character_sketch_footstep;
    }
    else if(option_character==CHARACTER_SLIME_O){
        ptr_sound=&sound_system.character_slime_o_footstep;
    }
    else if(option_character==CHARACTER_PENNY){
        ptr_sound=&sound_system.character_penny_footstep;
    }

    return ptr_sound;
}

sound_data* Player::return_character_sound_footstep2(){
    sound_data* ptr_sound=&sound_system.player_footstep2;

    if(option_character==CHARACTER_HUBERT){
        ptr_sound=&sound_system.player_footstep2;
    }
    else if(option_character==CHARACTER_SKETCH){
        ptr_sound=&sound_system.character_sketch_footstep2;
    }
    else if(option_character==CHARACTER_SLIME_O){
        ptr_sound=&sound_system.character_slime_o_footstep2;
    }
    else if(option_character==CHARACTER_PENNY){
        ptr_sound=&sound_system.character_penny_footstep2;
    }

    return ptr_sound;
}

sound_data* Player::return_character_sound_jump(){
    sound_data* ptr_sound=&sound_system.player_jump;

    if(option_character==CHARACTER_HUBERT){
        ptr_sound=&sound_system.player_jump;
    }
    else if(option_character==CHARACTER_SKETCH){
        ptr_sound=&sound_system.character_sketch_jump;
    }
    else if(option_character==CHARACTER_SLIME_O){
        ptr_sound=&sound_system.character_slime_o_jump;
    }
    else if(option_character==CHARACTER_PENNY){
        ptr_sound=&sound_system.character_penny_jump;
    }

    return ptr_sound;
}

sound_data* Player::return_character_sound_start_slide(){
    sound_data* ptr_sound=&sound_system.player_start_slide;

    if(option_character==CHARACTER_HUBERT){
        ptr_sound=&sound_system.player_start_slide;
    }
    else if(option_character==CHARACTER_SKETCH){
        ptr_sound=&sound_system.character_sketch_start_slide;
    }
    else if(option_character==CHARACTER_SLIME_O){
        ptr_sound=&sound_system.character_slime_o_start_slide;
    }
    else if(option_character==CHARACTER_PENNY){
        ptr_sound=&sound_system.character_penny_start_slide;
    }

    return ptr_sound;
}

void Player::survival_end_game(bool won){
    string message_title="You lose!";
    string message_msg="";

    stop_balloonin_sound();

    music.stop_track();

    if(won){
        survival_score*=2;

        message_title="You escaped!";

        play_positional_sound(sound_system.survival_escape);
    }

    gain_score(ammo*2,x+w/2,y,false);

    message_msg="Level: "+non_number_level;
    message_msg+="\nTime survived: ";
    message_msg+=get_time_string_long(level.survival_seconds_played);
    message_msg+="\nScore: ";
    ss.clear();ss.str("");ss<<survival_score;message_msg+=ss.str();
    message_msg+="\nNumber of players: ";
    ss.clear();ss.str("");ss<<1+mp_players.size();message_msg+=ss.str();

    survival_complete=true;

    string current_version="";
    ss.clear();ss.str("");ss<<"Version: ";ss<<Version::MAJOR;ss<<".";ss<<Version::MINOR;ss<<".";ss<<Version::MICRO;current_version=ss.str();

    string log_message="\nProfile: "+name;
    log_message+="\nVersion: "+current_version;
    if(won){
        log_message+="\nStatus: Escaped";
    }
    else{
        log_message+="\nStatus: Killed In Action";
    }
    log_message+="\n"+message_msg;

    update_survival_log(log_message,game_mode_is_multiplayer());

    window_message[0].set_message(message_title,message_msg);
}

void Player::no_game_in_progress(){
    string recall_name=name;

    if(game_in_progress){
        profile.save_level_data();
    }

    options_save();
    profile.save_profile_global_data();
    save_current_profile();
    profile.save_profile_list();

    stop_game();

    sliders.clear();

    name=recall_name;

    options_load();
    profile.load_profile_global_data();

    save_current_profile();

    window_manager.close_windows(0);

    window_manager.configure_main_menu();

    vector_windows[WINDOW_MAIN_MENU].turn_on();

    window_manager.set_main_menu_current_button();

    //Show or hide the hardware mouse cursor.
    if(option_hardware_cursor){
        SDL_ShowCursor(SDL_ENABLE);
    }
    else{
        SDL_ShowCursor(SDL_DISABLE);
    }

    global_options_load();

    play_game_start_sound();
}

bool Player::game_mode_is_multiplayer(){
    bool multiplayer=false;

    if(game_in_progress){
        if(game_mode==GAME_MODE_MP_ADVENTURE || game_mode==GAME_MODE_MP_SURVIVAL || game_mode==GAME_MODE_MP_VERSUS){
            multiplayer=true;
        }
    }

    return multiplayer;
}

bool Player::all_humans_dead(){
    bool all_dead=true;

    if(!DYING){
        all_dead=false;
    }

    for(int i=0;i<mp_players.size();i++){
        if(!mp_players[i].ai_controlled && !mp_players[i].DYING){
            all_dead=false;
            break;
        }
    }

    return all_dead;
}

bool Player::all_players_dead(){
    bool all_dead=true;

    if(!DYING && !bubble_mode){
        all_dead=false;
    }

    for(int i=0;i<mp_players.size();i++){
        if(!mp_players[i].DYING && !mp_players[i].bubble_mode){
            all_dead=false;
            break;
        }
    }

    return all_dead;
}

bool Player::all_players_gone(){
    int players_gone=0;

    if((DYING && !collision_check(x,y,w,h,camera_x,camera_y,camera_w,camera_h)) || bubble_mode){
        players_gone++;
    }

    for(int i=0;i<mp_players.size();i++){
        if((mp_players[i].DYING && !collision_check(mp_players[i].x,mp_players[i].y,mp_players[i].w,mp_players[i].h,camera_x,camera_y,camera_w,camera_h)) || mp_players[i].bubble_mode){
            players_gone++;
        }
    }

    if(players_gone>=1+mp_players.size()){
        return true;
    }
    else{
        return false;
    }
}

double Player::cam_focused_x(){
    if(camera_focused==-1){
        return x;
    }
    else{
        return mp_players[camera_focused].x;
    }
}

double Player::cam_focused_y(){
    if(camera_focused==-1){
        return y;
    }
    else{
        return mp_players[camera_focused].y;
    }
}

double Player::cam_focused_w(){
    if(camera_focused==-1){
        return w;
    }
    else{
        return mp_players[camera_focused].w;
    }
}

double Player::cam_focused_h(){
    if(camera_focused==-1){
        return h;
    }
    else{
        return mp_players[camera_focused].h;
    }
}

int Player::cam_focused_index(){
    return camera_focused;
}

bool Player::cam_focused_dying(){
    if(camera_focused==-1){
        return DYING;
    }
    else{
        return mp_players[camera_focused].DYING;
    }
}

void Player::update_camera_focus(){
    if(game_mode_is_multiplayer()){
        if(camera_focused==-1){
            if(DYING || bubble_mode){
                for(int ai_check=0;ai_check<2;ai_check++){
                    for(int i=0;i<mp_players.size();i++){
                        bool ai_controlled=mp_players[i].ai_controlled;
                        //If this is our second check, ignore AI control status.
                        if(ai_check==1){
                            ai_controlled=false;
                        }

                        if(!mp_players[i].DYING && !mp_players[i].bubble_mode && !ai_controlled){
                            camera_focused=i;
                            return;
                        }
                    }
                }
            }
        }
        else{
            if(mp_players[camera_focused].DYING || mp_players[camera_focused].bubble_mode){
                if(!DYING && !bubble_mode){
                    camera_focused=-1;
                    return;
                }

                for(int ai_check=0;ai_check<2;ai_check++){
                    for(int i=0;i<mp_players.size();i++){
                        bool ai_controlled=mp_players[i].ai_controlled;
                        //If this is our second check, ignore AI control status.
                        if(ai_check==1){
                            ai_controlled=false;
                        }

                        if(!mp_players[i].DYING && !mp_players[i].bubble_mode && !ai_controlled){
                            camera_focused=i;
                            return;
                        }
                    }
                }
            }
        }

        //If an AI controlled player is focused, try to replace them with a human player.
        if(camera_focused!=-1 && mp_players[camera_focused].ai_controlled){
            if(!DYING && !bubble_mode){
                camera_focused=-1;
                return;
            }

            for(int i=0;i<mp_players.size();i++){
                if(!mp_players[i].DYING && !mp_players[i].bubble_mode && !mp_players[i].ai_controlled){
                    camera_focused=i;
                    return;
                }
            }
        }
    }
}

short Player::get_player_color(int i){
    short color=COLOR_RED;

    short PLAYER_CHARACTER=option_character;
    if(i>=0){
        PLAYER_CHARACTER=mp_players[i].option_character;
    }

    if(PLAYER_CHARACTER==CHARACTER_HUBERT){
        color=COLOR_RED;
    }
    else if(PLAYER_CHARACTER==CHARACTER_SKETCH){
        color=COLOR_ORANGE;
    }
    else if(PLAYER_CHARACTER==CHARACTER_SLIME_O){
        color=COLOR_GREEN;
    }
    else if(PLAYER_CHARACTER==CHARACTER_PENNY){
        color=COLOR_PURPLE;
    }

    return color;
}

void Player::mp_reset(double new_x,double new_y){
    bool recall_night_mode=night_mode;
    bool recall_deadly_water=deadly_water;
    int recall_camera_focused=camera_focused;
    short recall_counter_level_end=counter_level_end;
    bool recall_goal_crossed=goal_crossed;
    double recall_update_rate=UPDATE_RATE;
    double recall_skip_ticks=SKIP_TICKS;
    bool recall_fadeout=fadeout;
    double recall_fade_opacity=fade_opacity;
    int recall_next_level=next_level;
    short recall_previous_level=previous_level;
    short recall_previous_sub_level=previous_sub_level;
    int recall_boss_index=boss_index;
    int recall_existing_effects_water_bubble=existing_effects_water_bubble;
    int recall_existing_effects_water_splash=existing_effects_water_splash;
    int recall_existing_effects_score_floater=existing_effects_score_floater;
    int recall_existing_effects_firework=existing_effects_firework;
    int recall_existing_shots=existing_shots;
    unsigned long recall_current_level_leaves=current_level_leaves;
    unsigned long recall_current_level_cheese=current_level_cheese;
    double recall_opacity_overlay_alarm=opacity_overlay_alarm;
    bool recall_fade_direction_overlay_alarm=fade_direction_overlay_alarm;
    int recall_sound_channel_death=sound_channel_death;
    int recall_sound_channel_balloonin=sound_channel_balloonin;

    uint64_t recall_ammo=ammo;
    double recall_bubble_move_x=bubble_move_x;
    double recall_bubble_move_y=bubble_move_y;
    short recall_counter_jump_mode=counter_jump_mode;

    load_data();

    night_mode=recall_night_mode;
    deadly_water=recall_deadly_water;
    camera_focused=recall_camera_focused;
    counter_level_end=recall_counter_level_end;
    goal_crossed=recall_goal_crossed;
    UPDATE_RATE=recall_update_rate;
    SKIP_TICKS=recall_skip_ticks;
    fadeout=recall_fadeout;
    fade_opacity=recall_fade_opacity;
    next_level=recall_next_level;
    previous_level=recall_previous_level;
    previous_sub_level=recall_previous_sub_level;
    boss_index=recall_boss_index;
    existing_effects_water_bubble=recall_existing_effects_water_bubble;
    existing_effects_water_splash=recall_existing_effects_water_splash;
    existing_effects_score_floater=recall_existing_effects_score_floater;
    existing_effects_firework=recall_existing_effects_firework;
    existing_shots=recall_existing_shots;
    current_level_leaves=recall_current_level_leaves;
    current_level_cheese=recall_current_level_cheese;
    opacity_overlay_alarm=recall_opacity_overlay_alarm;
    fade_direction_overlay_alarm=recall_fade_direction_overlay_alarm;
    sound_channel_death=recall_sound_channel_death;
    sound_channel_balloonin=recall_sound_channel_balloonin;

    ammo=recall_ammo;
    bubble_move_x=recall_bubble_move_x;
    bubble_move_y=recall_bubble_move_y;
    counter_jump_mode=recall_counter_jump_mode;

    x=new_x;
    y=new_y;
    if(game_mode!=GAME_MODE_MP_ADVENTURE){
        swimming_gear=true;
    }
    air_velocity=((double)random_range((uint32_t)(jump_min*10.0),(uint32_t)(jump_max*0.75*10.0))/10.0)*-1.0;
    IN_AIR=true;
    frame_jump=0;
    if(random_range(0,99)<50){
        run_speed=((double)random_range((uint32_t)(max_speed/4.0*10.0),(uint32_t)(max_speed/2.0*10.0))/10.0)*-1.0;
    }
    else{
        run_speed=(double)random_range((uint32_t)(max_speed/4.0*10.0),(uint32_t)(max_speed/2.0*10.0))/10.0;
    }
}

void Player::reset_mp_players(){
    mp_players.clear();

    mp_player_count=1;

    mp_character=CHARACTER_HUBERT;

    mp_available_characters.clear();
    for(int i=CHARACTER_BEGIN+1;i<CHARACTER_END;i++){
        mp_available_characters.push_back(i);
    }

    mp_ai.clear();
    for(int i=0;i<3;i++){
        mp_ai.push_back(false);
    }
}

void Player::mp_load_level_data(){
    //If there is no profile.
    if(name=="\x1F"){
    }
    //If there is a profile.
    else{
        if(game_mode==GAME_MODE_MP_ADVENTURE){
            if(game_in_progress && persistent_level_data){
                if(!on_worldmap()){
                    if(mp_level_data_items_collected[current_level].size()==0){
                        for(int i=0;i<vector_items.size();i++){
                            mp_level_data_items_collected[current_level].push_back(false);
                        }
                    }
                    if(mp_level_data_doors_opened[current_level].size()==0){
                        for(int i=0;i<vector_doors.size();i++){
                            mp_level_data_doors_opened[current_level].push_back(false);
                        }
                    }

                    for(int i=0;i<vector_items.size();i++){
                        //If the item is of a collectable type.
                        if(profile.is_item_collectable(vector_items[i].type)){
                            if(mp_level_data_items_collected[current_level][i]){
                                vector_items[i].exists=false;
                                if(vector_items[i].type==ITEM_LEAF){
                                    current_level_leaves++;
                                }
                                else if(vector_items[i].type==ITEM_CHEESE){
                                    current_level_cheese++;
                                }

                                int candy_width=vector_items[i].w/ITEM_W;
                                int candy_height=vector_items[i].h/ITEM_H;

                                for(int x=0;x<candy_width;x++){
                                    for(int y=0;y<candy_height;y++){
                                        vector_items.push_back(Item(vector_items[i].x+x*ITEM_W,vector_items[i].y+y*ITEM_H,false,ITEM_CANDY,0,false));
                                    }
                                }
                            }
                        }
                    }

                    for(int i=0;i<vector_doors.size();i++){
                        if(vector_doors[i].type==DOOR_TYPE_KEY){
                            if(mp_level_data_doors_opened[current_level][i]){
                                vector_doors[i].open=true;
                                vector_doors[i].frame=KEY_DOOR_SPRITES-1;
                            }
                        }
                    }
                }
            }
        }
    }
}

void Player::mp_save_level_data(){
    //If there is no profile.
    if(name=="\x1F"){
    }
    //If there is a profile.
    else{
        if(game_mode==GAME_MODE_MP_ADVENTURE){
            if(game_in_progress && persistent_level_data){
                mp_save_level_properties();

                if(!on_worldmap()){
                    for(int i=0;i<vector_items.size();i++){
                        //If the item is of a collectable type.
                        if(profile.is_item_collectable(vector_items[i].type)){
                            if(!vector_items[i].exists){
                                mp_level_data_items_collected[current_level][i]=true;
                            }
                            else{
                                mp_level_data_items_collected[current_level][i]=false;
                            }
                        }
                    }

                    for(int i=0;i<vector_doors.size();i++){
                        if(vector_doors[i].type==DOOR_TYPE_KEY){
                            if(vector_doors[i].open){
                                mp_level_data_doors_opened[current_level][i]=true;
                            }
                            else{
                                mp_level_data_doors_opened[current_level][i]=false;
                            }
                        }
                    }
                }
            }
        }
    }
}

void Player::mp_load_level_properties(){
    //If there is no profile.
    if(name=="\x1F"){
    }
    //If there is a profile.
    else{
        if(game_mode==GAME_MODE_MP_ADVENTURE){
            current_sub_level=mp_level_properties[current_level].current_sub_level;

            level_beaten=mp_level_properties[current_level].level_beaten;
        }
    }
}

void Player::mp_save_level_properties(){
    //If there is no profile.
    if(name=="\x1F"){
    }
    //If there is a profile.
    else{
        if(game_mode==GAME_MODE_MP_ADVENTURE){
            mp_level_properties[current_level].current_sub_level=current_sub_level;

            mp_level_properties[current_level].level_beaten=level_beaten;
        }
    }
}

short Player::get_npc_fade(){
    if(game_mode==GAME_MODE_SP_ADVENTURE || game_mode==GAME_MODE_MP_ADVENTURE){
        return option_npc_fade_adventure*(short)UPDATE_RATE;
    }
    else if(game_mode==GAME_MODE_SP_SURVIVAL || game_mode==GAME_MODE_MP_SURVIVAL){
        return option_npc_fade_survival*(short)UPDATE_RATE;
    }
    else{
        return 0;
    }
}

string Player::get_upgrade_name(string upgrade){
    if(upgrade=="monkeys"){
        return "Welcome to the Jungle";
    }
    else if(upgrade=="infinite_jumps"){
        return "Infinity Jumps";
    }
    /**else if(upgrade=="tiny_baddies"){
        return "Tiny Baddies";
    }*/
    else if(upgrade=="player_shot_noclip"){
        return "Stunner Phase Inverter";
    }
    else if(upgrade=="xray_specs"){
        return "X-Ray Specs";
    }
    else if(upgrade=="tophats"){
        return "Top Hats";
    }
    else if(upgrade=="candy_amount_1"){
        return "Candy Booster 1";
    }
    else if(upgrade=="candy_amount_2"){
        return "Candy Booster 2";
    }
    else if(upgrade=="candy_amount_3"){
        return "Candy Booster 3";
    }
    else if(upgrade=="candy_drop"){
        return "Candy Drop Rate";
    }
    else if(upgrade=="candy_vac"){
        return "Candy Vac 2.0";
    }
    else if(upgrade=="ammo_amount"){
        return "Ammo Booster";
    }
    else{
        return "Unknown";
    }
}

string Player::get_upgrade_description(string upgrade){
    if(upgrade=="monkeys"){
        return "Turns all baddies into Simians!";
    }
    else if(upgrade=="infinite_jumps"){
        return "Lets you jump infinity times without touching the ground!";
    }
    /**else if(upgrade=="tiny_baddies"){
        return "Shrinks all baddies into cute little versions of themselves!";
    }*/
    else if(upgrade=="player_shot_noclip"){
        return "Causes your stunner shots to pass through stuff!";
    }
    else if(upgrade=="xray_specs"){
        return "Lets you see through things!";
    }
    else if(upgrade=="tophats"){
        return "Classy NPCs wear top hats. Classy!";
    }
    else if(upgrade=="candy_amount_1"){
        return "Boosts the amount of points you gain per candy by 25%!";
    }
    else if(upgrade=="candy_amount_2"){
        return "Boosts the amount of points you gain per candy by 50%!";
    }
    else if(upgrade=="candy_amount_3"){
        return "Boosts the amount of points you gain per candy by 75%!";
    }
    else if(upgrade=="candy_drop"){
        return "Increases the candy drop rate from baddies!";
    }
    else if(upgrade=="candy_vac"){
        return "Doubles the range of your candy vacuum!";
    }
    else if(upgrade=="ammo_amount"){
        return "Boosts the amount of ammo you gain by 25%!";
    }
    else{
        return "None";
    }
}

uint64_t Player::get_upgrade_cost(string upgrade){
    if(upgrade=="monkeys"){
        return 32000;
    }
    else if(upgrade=="infinite_jumps"){
        return 128000;
    }
    /**else if(upgrade=="tiny_baddies"){
        return 38400;
    }*/
    else if(upgrade=="player_shot_noclip"){
        return 89600;
    }
    else if(upgrade=="xray_specs"){
        return 48000;
    }
    else if(upgrade=="tophats"){
        return 25600;
    }
    else if(upgrade=="candy_amount_1"){
        return 38400;
    }
    else if(upgrade=="candy_amount_2"){
        return 89600;
    }
    else if(upgrade=="candy_amount_3"){
        return 160000;
    }
    else if(upgrade=="candy_drop"){
        return 48000;
    }
    else if(upgrade=="candy_vac"){
        return 32000;
    }
    else if(upgrade=="ammo_amount"){
        return 38400;
    }
    else{
        return 0;
    }
}

void Player::remove_shop_upgrade(string upgrade){
    for(int i=0;i<shop_upgrades.size();i++){
        if(shop_upgrades[i]==upgrade){
            shop_upgrades.erase(shop_upgrades.begin()+i);
            i--;
        }
    }
}

void Player::gain_upgrade(string upgrade){
    if(!has_upgrade(upgrade)){
        upgrades.insert(make_pair(upgrade,false));
    }
}

bool Player::has_upgrade(string upgrade){
    for(map<string,bool>::iterator it=upgrades.begin();it!=upgrades.end();it++){
        if(it->first==upgrade){
            return true;
        }
    }

    return false;
}

bool Player::get_upgrade_state(string upgrade){
    if(game_mode==GAME_MODE_SP_ADVENTURE){
        for(map<string,bool>::iterator it=upgrades.begin();it!=upgrades.end();it++){
            if(it->first==upgrade){
                return it->second;
            }
        }
    }

    return false;
}

void Player::toggle_upgrade_state(std::string upgrade){
    for(map<string,bool>::iterator it=upgrades.begin();it!=upgrades.end();it++){
        if(it->first==upgrade){
            it->second=!it->second;

            break;
        }
    }
}
