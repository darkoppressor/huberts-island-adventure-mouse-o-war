/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "button_events.h"
#include "world.h"
#include "quit.h"
#include "options.h"

#ifdef GAME_OS_WINDOWS
#include <windows.h>
#endif

using namespace std;

//****************//
// Open a window: //
//****************//

void button_event_open_window_main_menu(Window* parent_window,int special_data){
    //Close windows.
    window_manager.close_windows(0);

    window_manager.configure_main_menu();

    vector_windows[WINDOW_MAIN_MENU].turn_on();

    window_manager.set_main_menu_current_button();
}

void button_event_open_window_inventory(Window* parent_window,int special_data){
    //Close windows.
    window_manager.close_windows(0);

    window_inventory[0].turn_on();
}

void button_event_open_window_create_profile(Window* parent_window,int special_data){
    //Close windows.
    window_manager.close_windows(0);

    vector_windows[WINDOW_CREATE_PROFILE].turn_on();
}

void button_event_open_window_change_profile(Window* parent_window,int special_data){
    //Close windows.
    window_manager.close_windows(0);

    set_change_profile_buttons();

    //If there is a profile.
    if(player.name!="\x1F"){
        vector_windows[WINDOW_CHANGE_PROFILE].turn_on();
    }
    else{
        vector_windows[WINDOW_CREATE_PROFILE].turn_on();
    }
}

void button_event_open_window_rewards(Window* parent_window,int special_data){
    //Close windows.
    window_manager.close_windows(0);

    vector_windows[WINDOW_REWARDS].turn_on();
}

void button_event_open_window_stats(Window* parent_window,int special_data){
    //Close windows.
    window_manager.close_windows(0);

    vector_windows[WINDOW_STATS].turn_on();

    player.current_button=3;
}

void button_event_open_window_stats_2(Window* parent_window,int special_data){
    //Close windows.
    window_manager.close_windows(0);

    vector_windows[WINDOW_STATS_2].turn_on();

    player.current_button=3;
}

void button_event_open_window_achievements(Window* parent_window,int special_data){
    //Close windows.
    window_manager.close_windows(0);

    vector_windows[WINDOW_ACHIEVEMENTS].turn_on();

    player.current_button=3;
}

void button_event_open_window_achievements_2(Window* parent_window,int special_data){
    //Close windows.
    window_manager.close_windows(0);

    vector_windows[WINDOW_ACHIEVEMENTS_2].turn_on();

    player.current_button=3;
}

void button_event_open_window_achievements_3(Window* parent_window,int special_data){
    //Close windows.
    window_manager.close_windows(0);

    vector_windows[WINDOW_ACHIEVEMENTS_3].turn_on();

    player.current_button=3;
}

void button_event_open_window_achievements_4(Window* parent_window,int special_data){
    //Close windows.
    window_manager.close_windows(0);

    vector_windows[WINDOW_ACHIEVEMENTS_4].turn_on();

    player.current_button=3;
}

void button_event_open_window_achievements_5(Window* parent_window,int special_data){
    //Close windows.
    window_manager.close_windows(0);

    vector_windows[WINDOW_ACHIEVEMENTS_5].turn_on();

    player.current_button=3;
}

void button_event_open_window_achievements_6(Window* parent_window,int special_data){
    //Close windows.
    window_manager.close_windows(0);

    vector_windows[WINDOW_ACHIEVEMENTS_6].turn_on();

    player.current_button=3;
}

void button_event_open_window_achievements_7(Window* parent_window,int special_data){
    //Close windows.
    window_manager.close_windows(0);

    vector_windows[WINDOW_ACHIEVEMENTS_7].turn_on();

    player.current_button=3;
}

void button_event_open_window_achievements_8(Window* parent_window,int special_data){
    //Close windows.
    window_manager.close_windows(0);

    vector_windows[WINDOW_ACHIEVEMENTS_8].turn_on();

    player.current_button=3;
}

void button_event_open_window_level_info(Window* parent_window,int special_data){
    //Close windows.
    window_manager.close_windows(0);

    if(special_data==-1){
        if(player.game_in_progress){
            int info_window=-1;
            for(int i=0,n=3;i<16;i++,n++){
                if(n==16){
                    n=21;
                }

                if(player.current_level==n){
                    info_window=i;
                }
            }

            if(info_window!=-1){
                special_data=info_window;
            }
            else{
                special_data=0;
            }
        }
        else{
            special_data=0;
        }
    }

    windows_level_info[special_data].load_stats();
    windows_level_info[special_data].turn_on();

    player.current_button=4;
}

void button_event_open_window_options(Window* parent_window,int special_data){
    //Close windows.
    window_manager.close_windows(0);

    vector_windows[WINDOW_OPTIONS].turn_on();
}

void button_event_open_window_options_keys(Window* parent_window,int special_data){
    //Close windows.
    window_manager.close_windows(0);

    vector_windows[WINDOW_OPTIONS_KEYS].turn_on();
}

void button_event_open_window_options_keys_1(Window* parent_window,int special_data){
    //Close windows.
    window_manager.close_windows(0);

    vector_windows[WINDOW_OPTIONS_KEYS_MP_1].turn_on();
}

void button_event_open_window_options_keys_2(Window* parent_window,int special_data){
    //Close windows.
    window_manager.close_windows(0);

    vector_windows[WINDOW_OPTIONS_KEYS_MP_2].turn_on();
}

void button_event_open_window_options_keys_3(Window* parent_window,int special_data){
    //Close windows.
    window_manager.close_windows(0);

    vector_windows[WINDOW_OPTIONS_KEYS_MP_3].turn_on();
}

void button_event_open_window_about(Window* parent_window,int special_data){
    //Close windows.
    window_manager.close_windows(0);

    vector_windows[WINDOW_ABOUT].turn_on();
}

void button_event_open_window_credits(Window* parent_window,int special_data){
    //Close windows.
    window_manager.close_windows(0);

    //The credits have been viewed.
    profile.update_achievements(ACHIEVEMENT_VIEW_CREDITS);

    vector_windows[WINDOW_CREDITS].turn_on();
}

void button_event_open_window_help(Window* parent_window,int special_data){
    //Close windows.
    window_manager.close_windows(0);

    vector_windows[WINDOW_HELP].turn_on();

    player.current_button=3;
}

void button_event_open_window_help_2(Window* parent_window,int special_data){
    //Close windows.
    window_manager.close_windows(0);

    vector_windows[WINDOW_HELP_2].turn_on();

    player.current_button=3;
}

void button_event_open_window_help_3(Window* parent_window,int special_data){
    //Close windows.
    window_manager.close_windows(0);

    vector_windows[WINDOW_HELP_3].turn_on();

    player.current_button=3;
}

void button_event_open_window_help_4(Window* parent_window,int special_data){
    //Close windows.
    window_manager.close_windows(0);

    vector_windows[WINDOW_HELP_4].turn_on();

    player.current_button=3;
}

void button_event_open_window_help_5(Window* parent_window,int special_data){
    //Close windows.
    window_manager.close_windows(0);

    vector_windows[WINDOW_HELP_5].turn_on();

    player.current_button=3;
}

void button_event_open_window_help_6(Window* parent_window,int special_data){
    //Close windows.
    window_manager.close_windows(0);

    vector_windows[WINDOW_HELP_6].turn_on();

    player.current_button=3;
}

void button_event_open_window_help_7(Window* parent_window,int special_data){
    //Close windows.
    window_manager.close_windows(0);

    vector_windows[WINDOW_HELP_7].turn_on();

    player.current_button=3;
}

void button_event_open_window_help_8(Window* parent_window,int special_data){
    //Close windows.
    window_manager.close_windows(0);

    vector_windows[WINDOW_HELP_8].turn_on();

    player.current_button=3;
}

void button_event_open_window_sound_test(Window* parent_window,int special_data){
    //Close windows.
    window_manager.close_windows(0);

    vector_windows[WINDOW_SOUND_TEST].turn_on();
}

void button_event_open_window_demo(Window* parent_window,int special_data){
    //Close windows.
    window_manager.close_windows(0);

    demo_mode=false;

    vector_windows[WINDOW_DEMO].turn_on();
}

void button_event_open_window_select_game_mode(Window* parent_window,int special_data){
    //Close windows.
    window_manager.close_windows(0);

    //This prevents us from adding MP players, backing out, and then starting a SP game and having MP players in it.
    player.reset_mp_players();

    vector_windows[WINDOW_SELECT_GAME_MODE].turn_on();
}

void button_event_open_window_setup_sp_survival(Window* parent_window,int special_data){
    //Close windows.
    window_manager.close_windows(0);

    player.survival_seconds_to_escape=window_setup_survival[0].recall_seconds;

    window_setup_survival[0].setup(false);

    window_setup_survival[0].turn_on();

    player.current_button=7;
}

void button_event_open_window_setup_mp_survival(Window* parent_window,int special_data){
    //Close windows.
    window_manager.close_windows(0);

    player.survival_seconds_to_escape=window_setup_survival[0].recall_seconds;

    window_setup_survival[0].setup(true);

    window_setup_survival[0].turn_on();

    player.current_button=7;
}

void button_event_open_window_mp_setup(Window* parent_window,int special_data){
    //Close windows.
    window_manager.close_windows(0);

    profile.load_mp_players();

    window_manager.configure_mp_setup();

    vector_windows[WINDOW_MP_SETUP].turn_on();
}

void button_event_open_window_map_other_level(Window* parent_window,int special_data){
    //Close windows.
    window_manager.close_windows(0);

    short sub_level=profile.load_level_properties_map(special_data).current_sub_level;

    window_map[0].set_map_level(special_data,sub_level);

    if(special_data==player.current_level){
        window_map[0].set_camera_coordinates(player.x/2.0+(player.w/2.0-window_map[0].map_camera_w)/2.0,player.y/2.0+(player.h/2.0-window_map[0].map_camera_h)/2.0);
    }
    else{
        Spawn_Coords sc=profile.return_level_spawn(special_data);

        window_map[0].set_camera_coordinates(sc.x/2.0+(player.w/2.0-window_map[0].map_camera_w)/2.0,sc.y/2.0+(player.h/2.0-window_map[0].map_camera_h)/2.0);
    }

    window_map[0].turn_on();
}

//*******************//
// Change an option: //
//*******************//

void button_event_option_fullscreen(Window* parent_window,int special_data){
    player.toggle_fullscreen();
}

void button_event_option_fps(Window* parent_window,int special_data){
    player.toggle_fps();
}

void button_event_option_mute_global(Window* parent_window,int special_data){
    player.toggle_mute_global();
    music.set_track_volumes();
}

void button_event_option_mute_sound(Window* parent_window,int special_data){
    player.toggle_mute_sound();
}

void button_event_option_mute_music(Window* parent_window,int special_data){
    player.toggle_mute_music();
    music.set_track_volumes();
}

void button_event_option_volume_up_global(Window* parent_window,int special_data){
    player.volume_up_global();
    music.set_track_volumes();
}

void button_event_option_volume_down_global(Window* parent_window,int special_data){
    player.volume_down_global();
    music.set_track_volumes();
}

void button_event_option_volume_up_sound(Window* parent_window,int special_data){
    player.volume_up_sound();
}

void button_event_option_volume_down_sound(Window* parent_window,int special_data){
    player.volume_down_sound();
}

void button_event_option_volume_up_music(Window* parent_window,int special_data){
    player.volume_up_music();
    music.set_track_volumes();
}

void button_event_option_volume_down_music(Window* parent_window,int special_data){
    player.volume_down_music();
    music.set_track_volumes();
}

void button_event_option_holiday_cheer(Window* parent_window,int special_data){
    player.toggle_holiday_cheer();
}

void button_event_option_time_of_day(Window* parent_window,int special_data){
    player.toggle_time_of_day();
}

void button_event_option_time_of_day_levels(Window* parent_window,int special_data){
    player.toggle_time_of_day_levels();
}

void button_event_option_difficulty(Window* parent_window,int special_data){
    player.toggle_difficulty();
}

void button_event_option_hints(Window* parent_window,int special_data){
    player.toggle_hints();
}

void button_event_option_hardware_cursor(Window* parent_window,int special_data){
    player.toggle_hardware_cursor();
}

void button_event_default_keys_keyboard(Window* parent_window,int special_data){
    player.keys.clear();
    player.keys.push_back(Input_Data(SDLK_F10));
    player.keys.push_back(Input_Data(SDLK_i));
    player.keys.push_back(Input_Data(SDLK_m));
    player.keys.push_back(Input_Data(SDLK_F5));
    player.keys.push_back(Input_Data(SDLK_LEFT));
    player.keys.push_back(Input_Data(SDLK_UP));
    player.keys.push_back(Input_Data(SDLK_RIGHT));
    player.keys.push_back(Input_Data(SDLK_DOWN));
    player.keys.push_back(Input_Data(SDLK_l));
    player.keys.push_back(Input_Data(SDLK_RCTRL));
    player.keys.push_back(Input_Data(SDLK_SPACE));
    player.keys.push_back(Input_Data(SDLK_RSHIFT));
    player.keys.push_back(Input_Data(SDLK_PAUSE));
    player.keys.push_back(Input_Data(SDLK_SLASH));

    options_save();
}

void button_event_default_keys_1_keyboard(Window* parent_window,int special_data){
    int player_index=0;

    player.mp_keys[player_index].clear();
    player.mp_keys[player_index].push_back(Input_Data());
    player.mp_keys[player_index].push_back(Input_Data());
    player.mp_keys[player_index].push_back(Input_Data());
    player.mp_keys[player_index].push_back(Input_Data());
    player.mp_keys[player_index].push_back(Input_Data(SDLK_a));
    player.mp_keys[player_index].push_back(Input_Data(SDLK_w));
    player.mp_keys[player_index].push_back(Input_Data(SDLK_d));
    player.mp_keys[player_index].push_back(Input_Data(SDLK_s));
    player.mp_keys[player_index].push_back(Input_Data());
    player.mp_keys[player_index].push_back(Input_Data(SDLK_e));
    player.mp_keys[player_index].push_back(Input_Data(SDLK_c));
    player.mp_keys[player_index].push_back(Input_Data());
    player.mp_keys[player_index].push_back(Input_Data());
    player.mp_keys[player_index].push_back(Input_Data(SDLK_q));

    options_save();
}

void button_event_default_keys_2_keyboard(Window* parent_window,int special_data){
    int player_index=1;

    player.mp_keys[player_index].clear();
    player.mp_keys[player_index].push_back(Input_Data());
    player.mp_keys[player_index].push_back(Input_Data());
    player.mp_keys[player_index].push_back(Input_Data());
    player.mp_keys[player_index].push_back(Input_Data());
    player.mp_keys[player_index].push_back(Input_Data(SDLK_h));
    player.mp_keys[player_index].push_back(Input_Data(SDLK_u));
    player.mp_keys[player_index].push_back(Input_Data(SDLK_k));
    player.mp_keys[player_index].push_back(Input_Data(SDLK_j));
    player.mp_keys[player_index].push_back(Input_Data());
    player.mp_keys[player_index].push_back(Input_Data(SDLK_g));
    player.mp_keys[player_index].push_back(Input_Data(SDLK_b));
    player.mp_keys[player_index].push_back(Input_Data());
    player.mp_keys[player_index].push_back(Input_Data());
    player.mp_keys[player_index].push_back(Input_Data(SDLK_y));

    options_save();
}

void button_event_default_keys_3_keyboard(Window* parent_window,int special_data){
    int player_index=2;

    player.mp_keys[player_index].clear();
    player.mp_keys[player_index].push_back(Input_Data());
    player.mp_keys[player_index].push_back(Input_Data());
    player.mp_keys[player_index].push_back(Input_Data());
    player.mp_keys[player_index].push_back(Input_Data());
    player.mp_keys[player_index].push_back(Input_Data(SDLK_KP1));
    player.mp_keys[player_index].push_back(Input_Data(SDLK_KP5));
    player.mp_keys[player_index].push_back(Input_Data(SDLK_KP3));
    player.mp_keys[player_index].push_back(Input_Data(SDLK_KP2));
    player.mp_keys[player_index].push_back(Input_Data());
    player.mp_keys[player_index].push_back(Input_Data(SDLK_KP4));
    player.mp_keys[player_index].push_back(Input_Data(SDLK_KP0));
    player.mp_keys[player_index].push_back(Input_Data());
    player.mp_keys[player_index].push_back(Input_Data());
    player.mp_keys[player_index].push_back(Input_Data(SDLK_KP6));

    options_save();
}

void button_event_default_keys_joystick(int player_index,int joystick_num){
    if(player_index==-1){
        player.keys.clear();
        player.keys.push_back(Input_Data());
        player.keys[player.keys.size()-1].set_joy_button(joystick_num,6);
        player.keys.push_back(Input_Data());
        player.keys[player.keys.size()-1].set_joy_button(joystick_num,4);
        player.keys.push_back(Input_Data());
        player.keys[player.keys.size()-1].set_joy_button(joystick_num,9);
        player.keys.push_back(Input_Data());
        player.keys[player.keys.size()-1].set_joy_axis(joystick_num,2,0);
        player.keys.push_back(Input_Data());
        player.keys[player.keys.size()-1].set_joy_axis(joystick_num,0,0);
        player.keys.push_back(Input_Data());
        player.keys[player.keys.size()-1].set_joy_axis(joystick_num,1,0);
        player.keys.push_back(Input_Data());
        player.keys[player.keys.size()-1].set_joy_axis(joystick_num,0,1);
        player.keys.push_back(Input_Data());
        player.keys[player.keys.size()-1].set_joy_axis(joystick_num,1,1);
        player.keys.push_back(Input_Data());
        player.keys[player.keys.size()-1].set_joy_button(joystick_num,5);
        player.keys.push_back(Input_Data());
        player.keys[player.keys.size()-1].set_joy_button(joystick_num,0);
        player.keys.push_back(Input_Data());
        player.keys[player.keys.size()-1].set_joy_button(joystick_num,2);
        player.keys.push_back(Input_Data());
        player.keys[player.keys.size()-1].set_joy_button(joystick_num,1);
        player.keys.push_back(Input_Data());
        player.keys[player.keys.size()-1].set_joy_button(joystick_num,7);
        player.keys.push_back(Input_Data());
        player.keys[player.keys.size()-1].set_joy_button(joystick_num,3);
    }
    else{
        player.mp_keys[player_index].clear();
        player.mp_keys[player_index].push_back(Input_Data());
        player.mp_keys[player_index][player.mp_keys[player_index].size()-1].set_joy_button(-1,6);
        player.mp_keys[player_index].push_back(Input_Data());
        player.mp_keys[player_index][player.mp_keys[player_index].size()-1].set_joy_button(-1,4);
        player.mp_keys[player_index].push_back(Input_Data());
        player.mp_keys[player_index][player.mp_keys[player_index].size()-1].set_joy_button(-1,9);
        player.mp_keys[player_index].push_back(Input_Data());
        player.mp_keys[player_index][player.mp_keys[player_index].size()-1].set_joy_axis(-1,2,0);
        player.mp_keys[player_index].push_back(Input_Data());
        player.mp_keys[player_index][player.mp_keys[player_index].size()-1].set_joy_axis(joystick_num,0,0);
        player.mp_keys[player_index].push_back(Input_Data());
        player.mp_keys[player_index][player.mp_keys[player_index].size()-1].set_joy_axis(joystick_num,1,0);
        player.mp_keys[player_index].push_back(Input_Data());
        player.mp_keys[player_index][player.mp_keys[player_index].size()-1].set_joy_axis(joystick_num,0,1);
        player.mp_keys[player_index].push_back(Input_Data());
        player.mp_keys[player_index][player.mp_keys[player_index].size()-1].set_joy_axis(joystick_num,1,1);
        player.mp_keys[player_index].push_back(Input_Data());
        player.mp_keys[player_index][player.mp_keys[player_index].size()-1].set_joy_button(-1,5);
        player.mp_keys[player_index].push_back(Input_Data());
        player.mp_keys[player_index][player.mp_keys[player_index].size()-1].set_joy_button(joystick_num,0);
        player.mp_keys[player_index].push_back(Input_Data());
        player.mp_keys[player_index][player.mp_keys[player_index].size()-1].set_joy_button(joystick_num,2);
        player.mp_keys[player_index].push_back(Input_Data());
        player.mp_keys[player_index][player.mp_keys[player_index].size()-1].set_joy_button(-1,1);
        player.mp_keys[player_index].push_back(Input_Data());
        player.mp_keys[player_index][player.mp_keys[player_index].size()-1].set_joy_button(-1,7);
        player.mp_keys[player_index].push_back(Input_Data());
        player.mp_keys[player_index][player.mp_keys[player_index].size()-1].set_joy_button(joystick_num,3);
    }

    options_save();
}

void button_event_default_keys_joystick_0(Window* parent_window,int special_data){
    button_event_default_keys_joystick(special_data,0);
}
void button_event_default_keys_joystick_1(Window* parent_window,int special_data){
    button_event_default_keys_joystick(special_data,1);
}
void button_event_default_keys_joystick_2(Window* parent_window,int special_data){
    button_event_default_keys_joystick(special_data,2);
}
void button_event_default_keys_joystick_3(Window* parent_window,int special_data){
    button_event_default_keys_joystick(special_data,3);
}

//************************************//
// Change the key bound to a command: //
//************************************//

void button_event_key_toggle_main_menu(Window* parent_window,int special_data){
    player.begin_setting_command(COMMAND_TOGGLE_MAIN_MENU,-1);
}
void button_event_key_toggle_inventory(Window* parent_window,int special_data){
    player.begin_setting_command(COMMAND_TOGGLE_INVENTORY,-1);
}
void button_event_key_toggle_map(Window* parent_window,int special_data){
    player.begin_setting_command(COMMAND_TOGGLE_MAP,-1);
}
void button_event_key_screenshot(Window* parent_window,int special_data){
    player.begin_setting_command(COMMAND_SCREENSHOT,-1);
}
void button_event_key_left(Window* parent_window,int special_data){
    player.begin_setting_command(COMMAND_LEFT,-1);
}
void button_event_key_up(Window* parent_window,int special_data){
    player.begin_setting_command(COMMAND_UP,-1);
}
void button_event_key_right(Window* parent_window,int special_data){
    player.begin_setting_command(COMMAND_RIGHT,-1);
}
void button_event_key_down(Window* parent_window,int special_data){
    player.begin_setting_command(COMMAND_DOWN,-1);
}
void button_event_key_look(Window* parent_window,int special_data){
    player.begin_setting_command(COMMAND_LOOK,-1);
}
void button_event_key_jump(Window* parent_window,int special_data){
    player.begin_setting_command(COMMAND_JUMP,-1);
}
void button_event_key_shoot(Window* parent_window,int special_data){
    player.begin_setting_command(COMMAND_SHOOT,-1);
}
void button_event_key_change_character(Window* parent_window,int special_data){
    player.begin_setting_command(COMMAND_CHANGE_CHARACTER,-1);
}
void button_event_key_pause(Window* parent_window,int special_data){
    player.begin_setting_command(COMMAND_PAUSE,-1);
}
void button_event_key_change_shot(Window* parent_window,int special_data){
    player.begin_setting_command(COMMAND_CHANGE_SHOT,-1);
}

//**************************************//
// Change the key bound to a command 1: //
//**************************************//

void button_event_key_1_left(Window* parent_window,int special_data){
    player.begin_setting_command(COMMAND_LEFT,0);
}
void button_event_key_1_up(Window* parent_window,int special_data){
    player.begin_setting_command(COMMAND_UP,0);
}
void button_event_key_1_right(Window* parent_window,int special_data){
    player.begin_setting_command(COMMAND_RIGHT,0);
}
void button_event_key_1_down(Window* parent_window,int special_data){
    player.begin_setting_command(COMMAND_DOWN,0);
}
void button_event_key_1_jump(Window* parent_window,int special_data){
    player.begin_setting_command(COMMAND_JUMP,0);
}
void button_event_key_1_shoot(Window* parent_window,int special_data){
    player.begin_setting_command(COMMAND_SHOOT,0);
}
void button_event_key_1_change_shot(Window* parent_window,int special_data){
    player.begin_setting_command(COMMAND_CHANGE_SHOT,0);
}

//**************************************//
// Change the key bound to a command 2: //
//**************************************//

void button_event_key_2_left(Window* parent_window,int special_data){
    player.begin_setting_command(COMMAND_LEFT,1);
}
void button_event_key_2_up(Window* parent_window,int special_data){
    player.begin_setting_command(COMMAND_UP,1);
}
void button_event_key_2_right(Window* parent_window,int special_data){
    player.begin_setting_command(COMMAND_RIGHT,1);
}
void button_event_key_2_down(Window* parent_window,int special_data){
    player.begin_setting_command(COMMAND_DOWN,1);
}
void button_event_key_2_jump(Window* parent_window,int special_data){
    player.begin_setting_command(COMMAND_JUMP,1);
}
void button_event_key_2_shoot(Window* parent_window,int special_data){
    player.begin_setting_command(COMMAND_SHOOT,1);
}
void button_event_key_2_change_shot(Window* parent_window,int special_data){
    player.begin_setting_command(COMMAND_CHANGE_SHOT,1);
}

//**************************************//
// Change the key bound to a command 3: //
//**************************************//

void button_event_key_3_left(Window* parent_window,int special_data){
    player.begin_setting_command(COMMAND_LEFT,2);
}
void button_event_key_3_up(Window* parent_window,int special_data){
    player.begin_setting_command(COMMAND_UP,2);
}
void button_event_key_3_right(Window* parent_window,int special_data){
    player.begin_setting_command(COMMAND_RIGHT,2);
}
void button_event_key_3_down(Window* parent_window,int special_data){
    player.begin_setting_command(COMMAND_DOWN,2);
}
void button_event_key_3_jump(Window* parent_window,int special_data){
    player.begin_setting_command(COMMAND_JUMP,2);
}
void button_event_key_3_shoot(Window* parent_window,int special_data){
    player.begin_setting_command(COMMAND_SHOOT,2);
}
void button_event_key_3_change_shot(Window* parent_window,int special_data){
    player.begin_setting_command(COMMAND_CHANGE_SHOT,2);
}

//**************//
// Other Stuff: //
//**************//

void button_event_create_profile(Window* parent_window,int special_data){
    profile.create_profile();
}

void button_event_select_profile(Window* parent_window,int special_data){
    profile.select_profile(special_data);
}

void button_event_delete_profile(Window* parent_window,int special_data){
    profile.delete_profile(special_data);
}

void button_event_skip_intro(Window* parent_window,int special_data){
    music.unload_track(MUSIC_INTRO1);
    music.unload_track(MUSIC_INTRO2);
    music.unload_track(MUSIC_INTRO3);
    music.unload_track(MUSIC_INTRO4);

    player.game_beginning_cutscene=0;
    window_message[0].turn_off();
    hud_buttons[0].set_invisibility(true);
    hud_buttons[0].set_enabled(false);
    player.start_game();
}

void button_event_start_game(Window* parent_window,int special_data){
    //Close windows.
    window_manager.close_windows(0);

    if(player.game_mode==GAME_MODE_SP_ADVENTURE){
        //If this player has never played before.
        if(!player.achievements[ACHIEVEMENT_ENTER_WORLDMAP]){
            player.game_beginning_cutscene=1;
            hud_buttons[0].set_invisibility(false);
            hud_buttons[0].set_enabled(true);
            window_message[0].set_message("","Act One, Scene One: Our story begins!\nA picnic lunch, with Hubert and friends!\nWith his main squeeze Sally, stacks of leaves and cheese,\nHubert was happy, content as could be.");
            music.load_track("data/music/intro1.ogg",MUSIC_INTRO1);
            music.load_track("data/music/intro2.ogg",MUSIC_INTRO2);
            music.load_track("data/music/intro3.ogg",MUSIC_INTRO3);
            music.load_track("data/music/intro4.ogg",MUSIC_INTRO4);
            music.restart_track(MUSIC_INTRO1);
            music.restart_track(MUSIC_INTRO2);
            music.restart_track(MUSIC_INTRO3);
            music.restart_track(MUSIC_INTRO4);
            music.play_track(MUSIC_INTRO1);
        }
        else{
            //Start/resume the game.
            player.start_game();
        }
    }
    else if(player.game_mode==GAME_MODE_MP_ADVENTURE){
        if(!player.mp_adventure_started){
            player.mp_adventure_started=true;

            player.game_beginning_cutscene=1;
            hud_buttons[0].set_invisibility(false);
            hud_buttons[0].set_enabled(true);
            window_message[0].set_message("","Act One, Scene One: Our story begins!\nA picnic lunch, with Hubert and friends!\nWith his main squeeze Sally, stacks of leaves and cheese,\nHubert was happy, content as could be.");
            music.load_track("data/music/intro1.ogg",MUSIC_INTRO1);
            music.load_track("data/music/intro2.ogg",MUSIC_INTRO2);
            music.load_track("data/music/intro3.ogg",MUSIC_INTRO3);
            music.load_track("data/music/intro4.ogg",MUSIC_INTRO4);
            music.restart_track(MUSIC_INTRO1);
            music.restart_track(MUSIC_INTRO2);
            music.restart_track(MUSIC_INTRO3);
            music.restart_track(MUSIC_INTRO4);
            music.play_track(MUSIC_INTRO1);
        }
        else{
            player.start_game();
        }
    }
    else if(player.game_mode==GAME_MODE_SP_SURVIVAL){
        player.start_game();
    }
    else if(player.game_mode==GAME_MODE_MP_SURVIVAL){
        player.start_game();
    }
}

void button_event_start_game_sp_adventure(Window* parent_window,int special_data){
    player.game_mode=GAME_MODE_SP_ADVENTURE;

    button_event_start_game(parent_window,special_data);
}

void button_event_start_game_mp_adventure(Window* parent_window,int special_data){
    player.game_mode=GAME_MODE_MP_ADVENTURE;

    button_event_start_game(parent_window,special_data);
}

void button_event_start_game_survival(Window* parent_window,int special_data){
    if(window_setup_survival[0].is_multiplayer){
        player.game_mode=GAME_MODE_MP_SURVIVAL;
    }
    else{
        player.game_mode=GAME_MODE_SP_SURVIVAL;
    }

    player.non_number_level=window_setup_survival[0].levels[window_setup_survival[0].level_list_selection];

    button_event_start_game(parent_window,special_data);
}

void button_event_shop_buy(Window* parent_window,int special_data){
    if(player.game_mode==GAME_MODE_SP_ADVENTURE){
        string upgrade=window_shop[0].upgrades[window_shop[0].upgrade_list_selection];
        uint64_t price=player.get_upgrade_cost(upgrade);

        if(price<=player.score){
            player.score-=price;

            player.remove_shop_upgrade(upgrade);
            player.gain_upgrade(upgrade);

            profile.save_inventory();
            profile.save_shop();
            profile.save_upgrades();

            window_shop[0].setup();

            if(player.shop_upgrades.size()==0){
                window_manager.close_windows(0);
            }
        }
    }
}

void button_event_upgrade_toggle(Window* parent_window,int special_data){
    if(player.game_mode==GAME_MODE_SP_ADVENTURE){
        string upgrade=window_upgrades[0].upgrades[window_upgrades[0].upgrade_list_selection];

        player.toggle_upgrade_state(upgrade);

        profile.save_upgrades();

        window_upgrades[0].setup();
    }
}

void button_event_return_to_world_map(Window* parent_window,int special_data){
    //If the game is in progress and the player is not already on the world map.
    if(player.game_in_progress && !player.on_worldmap()){
        //Close windows.
        window_manager.close_windows(0);

        //Make sure these aren't 0, since if they were, the game would mistakenly think the player
        //beat the level without shooting or killing.
        player.special_count_shots_this_level=1;
        player.special_count_kills_this_level=1;

        player.stop_balloonin_sound();

        player.boss_end();

        //Check for new achievements in this level before leaving it.
        profile.update_achievements();

        profile.save_level_data();
        player.previous_level=player.current_level;
        player.previous_sub_level=player.current_sub_level;
        player.current_level=player.current_worldmap;
        player.current_checkpoint=-1;
        player.load_data();

        for(int i=0;i<mp_players.size();i++){
            mp_players[i].load_data();
        }

        level.load_level();
    }
}

void button_event_end_game(Window* parent_window,int special_data){
    player.end_game();
}

void button_event_quit(Window* parent_window,int special_data){
    quit_game();
}

void button_event_buy_game(Window* parent_window,int special_data){
    string buy_url="http://cheeseandbacon.org/huberts-island/buy.php";

    #ifdef GAME_OS_WINDOWS
    ShellExecute(NULL,"open",buy_url.c_str(),NULL,NULL,SW_SHOWNORMAL);
    #endif

    #ifdef GAME_OS_LINUX
    string open_command="xdg-open "+buy_url;
    system(open_command.c_str());
    #endif

    #ifdef GAME_OS_OSX
    string open_command="open "+buy_url;
    system(open_command.c_str());
    #endif
}

void button_event_survival_time_very_short(Window* parent_window,int special_data){
    player.survival_seconds_to_escape=SURVIVAL_TIME_VERY_SHORT;
    window_setup_survival[0].recall_seconds=player.survival_seconds_to_escape;
}
void button_event_survival_time_short(Window* parent_window,int special_data){
    player.survival_seconds_to_escape=SURVIVAL_TIME_SHORT;
    window_setup_survival[0].recall_seconds=player.survival_seconds_to_escape;
}
void button_event_survival_time_medium(Window* parent_window,int special_data){
    player.survival_seconds_to_escape=SURVIVAL_TIME_MEDIUM;
    window_setup_survival[0].recall_seconds=player.survival_seconds_to_escape;
}
void button_event_survival_time_long(Window* parent_window,int special_data){
    player.survival_seconds_to_escape=SURVIVAL_TIME_LONG;
    window_setup_survival[0].recall_seconds=player.survival_seconds_to_escape;
}
void button_event_survival_time_very_long(Window* parent_window,int special_data){
    player.survival_seconds_to_escape=SURVIVAL_TIME_VERY_LONG;
    window_setup_survival[0].recall_seconds=player.survival_seconds_to_escape;
}

void button_event_mp_setup_start(Window* parent_window,int special_data){
    if(window_manager.which_mp_game_mode==GAME_MODE_MP_ADVENTURE){
        button_event_start_game_mp_adventure(parent_window,special_data);
    }
    else if(window_manager.which_mp_game_mode==GAME_MODE_MP_SURVIVAL){
        button_event_open_window_setup_mp_survival(parent_window,special_data);
    }
}

void button_event_mp_prepare_adventure(Window* parent_window,int special_data){
    window_manager.which_mp_game_mode=GAME_MODE_MP_ADVENTURE;

    button_event_open_window_mp_setup(parent_window,special_data);
}
void button_event_mp_prepare_survival(Window* parent_window,int special_data){
    window_manager.which_mp_game_mode=GAME_MODE_MP_SURVIVAL;

    button_event_open_window_mp_setup(parent_window,special_data);
}

void button_event_mp_players_down(Window* parent_window,int special_data){
    if(player.mp_player_count>1){
        int i=player.mp_player_count-2;

        player.mp_available_characters.push_back(mp_players[i].option_character);
        mp_players.erase(mp_players.begin()+i);

        player.mp_player_count--;

        profile.save_mp_players();

        window_manager.configure_mp_setup();
    }
}
void button_event_mp_players_up(Window* parent_window,int special_data){
    if(player.mp_player_count<4){
        int i=player.mp_player_count-1;

        mp_players.push_back(Player_Mp(player.mp_keys[i],i,player.mp_ai[i]));
        mp_players[i].option_character=player.mp_available_characters[0];
        player.mp_available_characters.erase(player.mp_available_characters.begin());

        player.mp_player_count++;

        profile.save_mp_players();

        window_manager.configure_mp_setup();
    }
}

void button_event_mp_character_toggle_0(Window* parent_window,int special_data){
    if(player.mp_available_characters.size()>0){
        short swap_character=player.mp_character;

        player.mp_character=player.mp_available_characters[0];
        player.mp_available_characters.erase(player.mp_available_characters.begin());
        player.mp_available_characters.push_back(swap_character);

        profile.save_mp_players();
    }
}
void button_event_mp_character_toggle_1(Window* parent_window,int special_data){
    if(player.mp_available_characters.size()>0){
        short swap_character=mp_players[0].option_character;

        mp_players[0].option_character=player.mp_available_characters[0];
        player.mp_available_characters.erase(player.mp_available_characters.begin());
        player.mp_available_characters.push_back(swap_character);

        profile.save_mp_players();
    }
}
void button_event_mp_character_toggle_2(Window* parent_window,int special_data){
    if(player.mp_available_characters.size()>0){
        short swap_character=mp_players[1].option_character;

        mp_players[1].option_character=player.mp_available_characters[0];
        player.mp_available_characters.erase(player.mp_available_characters.begin());
        player.mp_available_characters.push_back(swap_character);

        profile.save_mp_players();
    }
}
void button_event_mp_character_toggle_3(Window* parent_window,int special_data){
    if(player.mp_available_characters.size()>0){
        short swap_character=mp_players[2].option_character;

        mp_players[2].option_character=player.mp_available_characters[0];
        player.mp_available_characters.erase(player.mp_available_characters.begin());
        player.mp_available_characters.push_back(swap_character);

        profile.save_mp_players();
    }
}

void button_event_mp_ai_toggle_1(Window* parent_window,int special_data){
    player.mp_ai[0]=!player.mp_ai[0];

    mp_players[0].ai_controlled=player.mp_ai[0];

    profile.save_mp_players();
}
void button_event_mp_ai_toggle_2(Window* parent_window,int special_data){
    player.mp_ai[1]=!player.mp_ai[1];

    mp_players[1].ai_controlled=player.mp_ai[1];

    profile.save_mp_players();
}
void button_event_mp_ai_toggle_3(Window* parent_window,int special_data){
    player.mp_ai[2]=!player.mp_ai[2];

    mp_players[2].ai_controlled=player.mp_ai[2];

    profile.save_mp_players();
}

void button_event_close_window(Window* parent_window,int special_data){
    //No button is focused now.
    player.current_button=-1;

    parent_window->turn_off();

    if(special_data==-1){
        //If a game is not in progress.
        if(!player.game_in_progress && player.game_beginning_cutscene==0){
            //If a profile exists.
            if(player.name!="\x1F"){
                vector_windows[WINDOW_MAIN_MENU].turn_on();

                window_manager.set_main_menu_current_button();
            }
            //If no profile exists.
            else{
                //Keep the Create Profile window open.
                vector_windows[WINDOW_CREATE_PROFILE].turn_on();
            }
        }
    }
}

void button_event_do_nothing(Window* parent_window,int special_data){
}
