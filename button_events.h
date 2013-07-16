/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef button_events_h
#define button_events_h

#include "window.h"

//Open the corresponding window.
void button_event_open_window_main_menu(Window* parent_window,int special_data);
void button_event_open_window_inventory(Window* parent_window,int special_data);
void button_event_open_window_create_profile(Window* parent_window,int special_data);
void button_event_open_window_change_profile(Window* parent_window,int special_data);
void button_event_open_window_rewards(Window* parent_window,int special_data);
void button_event_open_window_stats(Window* parent_window,int special_data);
void button_event_open_window_stats_2(Window* parent_window,int special_data);
void button_event_open_window_achievements(Window* parent_window,int special_data);
void button_event_open_window_achievements_2(Window* parent_window,int special_data);
void button_event_open_window_achievements_3(Window* parent_window,int special_data);
void button_event_open_window_achievements_4(Window* parent_window,int special_data);
void button_event_open_window_achievements_5(Window* parent_window,int special_data);
void button_event_open_window_achievements_6(Window* parent_window,int special_data);
void button_event_open_window_achievements_7(Window* parent_window,int special_data);
void button_event_open_window_achievements_8(Window* parent_window,int special_data);
void button_event_open_window_level_info(Window* parent_window,int special_data);
void button_event_open_window_options(Window* parent_window,int special_data);
void button_event_open_window_options_keys(Window* parent_window,int special_data);
void button_event_open_window_options_keys_1(Window* parent_window,int special_data);
void button_event_open_window_options_keys_2(Window* parent_window,int special_data);
void button_event_open_window_options_keys_3(Window* parent_window,int special_data);
void button_event_open_window_about(Window* parent_window,int special_data);
void button_event_open_window_credits(Window* parent_window,int special_data);
void button_event_open_window_help(Window* parent_window,int special_data);
void button_event_open_window_help_2(Window* parent_window,int special_data);
void button_event_open_window_help_3(Window* parent_window,int special_data);
void button_event_open_window_help_4(Window* parent_window,int special_data);
void button_event_open_window_help_5(Window* parent_window,int special_data);
void button_event_open_window_help_6(Window* parent_window,int special_data);
void button_event_open_window_help_7(Window* parent_window,int special_data);
void button_event_open_window_help_8(Window* parent_window,int special_data);
void button_event_open_window_sound_test(Window* parent_window,int special_data);
void button_event_open_window_demo(Window* parent_window,int special_data);
void button_event_open_window_select_game_mode(Window* parent_window,int special_data);
void button_event_open_window_setup_sp_survival(Window* parent_window,int special_data);
void button_event_open_window_setup_mp_survival(Window* parent_window,int special_data);
void button_event_open_window_mp_setup(Window* parent_window,int special_data);
void button_event_open_window_map_other_level(Window* parent_window,int special_data);

//Change the corresponding option.
void button_event_option_fullscreen(Window* parent_window,int special_data);
void button_event_option_fps(Window* parent_window,int special_data);
void button_event_option_mute_global(Window* parent_window,int special_data);
void button_event_option_mute_sound(Window* parent_window,int special_data);
void button_event_option_mute_music(Window* parent_window,int special_data);
void button_event_option_volume_up_global(Window* parent_window,int special_data);
void button_event_option_volume_down_global(Window* parent_window,int special_data);
void button_event_option_volume_up_sound(Window* parent_window,int special_data);
void button_event_option_volume_down_sound(Window* parent_window,int special_data);
void button_event_option_volume_up_music(Window* parent_window,int special_data);
void button_event_option_volume_down_music(Window* parent_window,int special_data);
void button_event_option_holiday_cheer(Window* parent_window,int special_data);
void button_event_option_time_of_day(Window* parent_window,int special_data);
void button_event_option_time_of_day_levels(Window* parent_window,int special_data);
void button_event_option_difficulty(Window* parent_window,int special_data);
void button_event_option_hints(Window* parent_window,int special_data);
void button_event_option_hardware_cursor(Window* parent_window,int special_data);

void button_event_default_keys_keyboard(Window* parent_window,int special_data);
void button_event_default_keys_1_keyboard(Window* parent_window,int special_data);
void button_event_default_keys_2_keyboard(Window* parent_window,int special_data);
void button_event_default_keys_3_keyboard(Window* parent_window,int special_data);

void button_event_default_keys_joystick(int player_index,int joystick_num);
void button_event_default_keys_joystick_0(Window* parent_window,int special_data);
void button_event_default_keys_joystick_1(Window* parent_window,int special_data);
void button_event_default_keys_joystick_2(Window* parent_window,int special_data);
void button_event_default_keys_joystick_3(Window* parent_window,int special_data);

//Change key.
void button_event_key_toggle_main_menu(Window* parent_window,int special_data);
void button_event_key_toggle_inventory(Window* parent_window,int special_data);
void button_event_key_toggle_map(Window* parent_window,int special_data);
void button_event_key_screenshot(Window* parent_window,int special_data);
void button_event_key_left(Window* parent_window,int special_data);
void button_event_key_up(Window* parent_window,int special_data);
void button_event_key_right(Window* parent_window,int special_data);
void button_event_key_down(Window* parent_window,int special_data);
void button_event_key_look(Window* parent_window,int special_data);
void button_event_key_jump(Window* parent_window,int special_data);
void button_event_key_shoot(Window* parent_window,int special_data);
void button_event_key_change_character(Window* parent_window,int special_data);
void button_event_key_pause(Window* parent_window,int special_data);
void button_event_key_change_shot(Window* parent_window,int special_data);

//Change keys multiplayer.
void button_event_key_1_left(Window* parent_window,int special_data);
void button_event_key_1_up(Window* parent_window,int special_data);
void button_event_key_1_right(Window* parent_window,int special_data);
void button_event_key_1_down(Window* parent_window,int special_data);
void button_event_key_1_jump(Window* parent_window,int special_data);
void button_event_key_1_shoot(Window* parent_window,int special_data);
void button_event_key_1_change_shot(Window* parent_window,int special_data);

void button_event_key_2_left(Window* parent_window,int special_data);
void button_event_key_2_up(Window* parent_window,int special_data);
void button_event_key_2_right(Window* parent_window,int special_data);
void button_event_key_2_down(Window* parent_window,int special_data);
void button_event_key_2_jump(Window* parent_window,int special_data);
void button_event_key_2_shoot(Window* parent_window,int special_data);
void button_event_key_2_change_shot(Window* parent_window,int special_data);

void button_event_key_3_left(Window* parent_window,int special_data);
void button_event_key_3_up(Window* parent_window,int special_data);
void button_event_key_3_right(Window* parent_window,int special_data);
void button_event_key_3_down(Window* parent_window,int special_data);
void button_event_key_3_jump(Window* parent_window,int special_data);
void button_event_key_3_shoot(Window* parent_window,int special_data);
void button_event_key_3_change_shot(Window* parent_window,int special_data);

//Create profile.
void button_event_create_profile(Window* parent_window,int special_data);

//Select this profile.
void button_event_select_profile(Window* parent_window,int special_data);

//Delete this profile.
void button_event_delete_profile(Window* parent_window,int special_data);

//Skip Intro.
void button_event_skip_intro(Window* parent_window,int special_data);

//Start or resume the game.
void button_event_start_game(Window* parent_window,int special_data);
void button_event_start_game_sp_adventure(Window* parent_window,int special_data);
void button_event_start_game_mp_adventure(Window* parent_window,int special_data);
void button_event_start_game_survival(Window* parent_window,int special_data);

void button_event_shop_buy(Window* parent_window,int special_data);

void button_event_upgrade_toggle(Window* parent_window,int special_data);

//Return to the world map.
void button_event_return_to_world_map(Window* parent_window,int special_data);

//End the current game and return to the main menu.
void button_event_end_game(Window* parent_window,int special_data);

//Quit the game.
void button_event_quit(Window* parent_window,int special_data);

//Open the game buy webpage.
void button_event_buy_game(Window* parent_window,int special_data);

//Setup for SP Survival.
void button_event_survival_time_very_short(Window* parent_window,int special_data);
void button_event_survival_time_short(Window* parent_window,int special_data);
void button_event_survival_time_medium(Window* parent_window,int special_data);
void button_event_survival_time_long(Window* parent_window,int special_data);
void button_event_survival_time_very_long(Window* parent_window,int special_data);

//Setup for MP.
void button_event_mp_setup_start(Window* parent_window,int special_data);

void button_event_mp_prepare_adventure(Window* parent_window,int special_data);
void button_event_mp_prepare_survival(Window* parent_window,int special_data);

void button_event_mp_players_up(Window* parent_window,int special_data);
void button_event_mp_players_down(Window* parent_window,int special_data);

void button_event_mp_character_toggle_0(Window* parent_window,int special_data);
void button_event_mp_character_toggle_1(Window* parent_window,int special_data);
void button_event_mp_character_toggle_2(Window* parent_window,int special_data);
void button_event_mp_character_toggle_3(Window* parent_window,int special_data);

void button_event_mp_ai_toggle_1(Window* parent_window,int special_data);
void button_event_mp_ai_toggle_2(Window* parent_window,int special_data);
void button_event_mp_ai_toggle_3(Window* parent_window,int special_data);

//Close the parent window.
void button_event_close_window(Window* parent_window,int special_data);

//Do nothing.
//This is just a placeholder function.
void button_event_do_nothing(Window* parent_window,int special_data);

#endif
