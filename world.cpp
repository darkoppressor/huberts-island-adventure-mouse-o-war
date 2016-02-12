/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "world.h"

#include "sprite_sheets.h"
#include "button_events.h"
#include "achievement_tooltips.h"
#include "holidays.h"

using namespace std;

//Create the main window.
Game_Window main_window;

vector<Window> vector_windows;

vector<Window_Inventory> window_inventory;

vector<Window_Map> window_map;

vector<Window_Message> window_message;

vector<Window_Level_Info> windows_level_info;

vector<Window_Setup_Survival> window_setup_survival;

vector<Window_Shop> window_shop;

vector<Window_Upgrades> window_upgrades;

Window_Manager window_manager;

vector<Button> hud_buttons;

vector<Slider> sliders;

Tooltip tooltip;
Tooltip tooltip_levelname;
Tooltip tooltip_hint;

Random_Number_Generator rng;

int LIGHTING_TILE_SIZE=32;

short holiday=HOLIDAY_NONE;

bool demo_mode=false;

//We use this to poll for events when getting input.
SDL_Event event;

vector<joy_stick> joystick;

BitmapFont font;
BitmapFont font_sga;
BitmapFont font_large;
BitmapFont font_large2;

short option_save_location=SAVE_LOCATION_HOME;

double UPDATE_RATE=DEFAULT_UPDATE_RATE;
double SKIP_TICKS=1000.0/UPDATE_RATE;

//String stream used to easily render ints and stuff as strings.
stringstream ss("");

//ss passes its information into this string for output.
string msg="";

//All of the world variables are defined here. These include the level object (which stores much of the level's information, and has a function that handles loading of levels),
//and the vectors that hold players, enemies, etc.

//All of the level's information is stored here.
Level level;

Player player;

vector<Player_Mp> mp_players;

vector<string> default_shop_upgrades;

Profile profile;

string creating_profile="";

vector<Item> vector_items;
vector<Npc> vector_npcs;
vector<Shot> vector_shots;
vector<Moving_Platform> vector_moving_platforms;
vector<Trigger> vector_triggers;
vector<Door> vector_doors;
vector<Trap> vector_traps;
vector<Sign> vector_signs;
vector<Spring> vector_springs;
vector<Booster> vector_boosters;
vector<Effect_Water_Splash> vector_effect_water_splash;
vector<Effect_Water_Bubble> vector_effect_water_bubble;
vector<Effect_Score_Floater> vector_effect_score_floater;
vector<Effect_Firework> vector_effect_firework;
vector<Spawner> vector_spawners;

vector<SDL_Event> touch_controller_events;

Image image;

Music music;

Sound sound_system;

Touch_Controller touch_controller;

void load_world(){
    font.build_font("data/images/bitmap_font.png",4096,16,11,24);
    font_sga.build_font("data/images/bitmap_font_sga.png",4096,16,20,24);
    font_large.build_font("data/images/bitmap_font_large.png",4096,16,11,24);
    font_large2.build_font("data/images/bitmap_font_large2.png",4096,16,11,24);

    image.load_images_global();

    sound_system.load_sounds_global();

    prepare_npc_sprite_counts();
    prepare_trap_sprite_counts();
    prepare_shot_sprite_counts();

    generate_sprite_sheets();

    create_windows();
    create_hud_buttons();

    set_default_shop_upgrades();
}

void unload_world(){
    font.unload_font();
    font_sga.unload_font();
    font_large.unload_font();
    font_large2.unload_font();

    image.unload_images_global();

    sound_system.unload_sounds_global();
}

void set_default_shop_upgrades(){
    default_shop_upgrades.clear();

    default_shop_upgrades.push_back("tophats");
    default_shop_upgrades.push_back("monkeys");
    default_shop_upgrades.push_back("candy_vac");
    ///default_shop_upgrades.push_back("tiny_baddies");
    default_shop_upgrades.push_back("ammo_amount");
    default_shop_upgrades.push_back("candy_amount_1");
    default_shop_upgrades.push_back("xray_specs");
    default_shop_upgrades.push_back("candy_drop");
    default_shop_upgrades.push_back("player_shot_noclip");
    default_shop_upgrades.push_back("candy_amount_2");
    default_shop_upgrades.push_back("infinite_jumps");
    default_shop_upgrades.push_back("candy_amount_3");
}

void set_change_profile_buttons(){
    short button_sound=0;

    vector_windows[WINDOW_CHANGE_PROFILE].clear_buttons();
    vector_windows[WINDOW_CHANGE_PROFILE].clear_information();

    //Create the close button.
    vector_windows[WINDOW_CHANGE_PROFILE].create_button(360-23,5,"","X",&button_event_close_window,0,0,BUTTON_VISIBLE,-1);

    vector_windows[WINDOW_CHANGE_PROFILE].create_information((360-(16*12))/2,40,"","",0,SPECIAL_INFO_CURRENT_PROFILE);

    for(int i=0;i<profile.profile_list.size();i++){
        vector_windows[WINDOW_CHANGE_PROFILE].create_information(15,105+55*i,"","",0,SPECIAL_INFO_PROFILE_1+i);

        msg="Select Profile";
        button_sound++;
        if(button_sound>11){
            button_sound=0;
        }
        vector_windows[WINDOW_CHANGE_PROFILE].create_button(15,130+55*i,"",msg,&button_event_select_profile,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE,i);

        msg="Delete Profile";
        button_sound++;
        if(button_sound>11){
            button_sound=0;
        }
        vector_windows[WINDOW_CHANGE_PROFILE].create_button(185,130+55*i,"",msg,&button_event_delete_profile,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE,i);
    }

    msg="Create New Profile";
    button_sound=11-1;
    vector_windows[WINDOW_CHANGE_PROFILE].create_button((360-(msg.length()*font.spacing_x))/2,526,"",msg,&button_event_open_window_create_profile,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Back to Main Menu";
    button_sound=12-1;
    vector_windows[WINDOW_CHANGE_PROFILE].create_button((360-(msg.length()*font.spacing_x))/2,563,"",msg,&button_event_open_window_main_menu,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);
}

void create_hud_buttons(){
    short button_sound=0;

    hud_buttons.clear();

    msg="Press Shoot to Skip Intro";
    button_sound=12-1;
    hud_buttons.push_back(Button((main_window.SCREEN_WIDTH-(msg.length()*font.spacing_x))/2,main_window.SCREEN_HEIGHT-35,"",msg,&button_event_skip_intro,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE,-1,BUTTON_DISABLED));
}

void create_windows(){
    short button_sound=0;

    //Used to position stuff in windows more easily.
    short y_position=0;

    //Used to position some help screen stuff.
    int x_position=0;

    short window_width=0;
    short window_height=0;
    short back_button_space=37;

    window_inventory.clear();
    window_inventory.push_back(Window_Inventory((main_window.SCREEN_WIDTH-362)/2,(main_window.SCREEN_HEIGHT-398)/2,362,398,"Inventory"));

    window_map.clear();
    window_map.push_back(Window_Map(0,0,main_window.SCREEN_WIDTH,main_window.SCREEN_HEIGHT,"Map"));

    window_message.clear();
    window_message.push_back(Window_Message());

    window_setup_survival.clear();
    window_setup_survival.push_back(Window_Setup_Survival(0,0,main_window.SCREEN_WIDTH,main_window.SCREEN_HEIGHT,""));

    window_shop.clear();
    window_shop.push_back(Window_Shop(0,0,main_window.SCREEN_WIDTH,main_window.SCREEN_HEIGHT,""));

    window_upgrades.clear();
    window_upgrades.push_back(Window_Upgrades(0,0,main_window.SCREEN_WIDTH,main_window.SCREEN_HEIGHT,""));

    windows_level_info.clear();

    int levels_with_info[16];
    for(int i=0,n=3;i<16;i++,n++){
        if(n==16){
            n=21;
        }
        levels_with_info[i]=n;
    }

    for(int i=0;i<16;i++){
        window_width=800;
        window_height=500;
        msg=level.return_level_name(levels_with_info[i]);
        windows_level_info.push_back(Window_Level_Info((main_window.SCREEN_WIDTH-window_width)/2,(main_window.SCREEN_HEIGHT-window_height)/2,window_width,window_height,msg,i,levels_with_info[i]));

        int previous_window=i-1;
        int next_window=i+1;
        if(previous_window<0){
            previous_window=15;
        }
        if(next_window>15){
            next_window=0;
        }

        y_position=48;
        msg="View Map";
        button_sound=1-1;
        windows_level_info[windows_level_info.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_open_window_map_other_level,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE,levels_with_info[i]);

        y_position+=40;
        windows_level_info[windows_level_info.size()-1].create_information(250,y_position,"The number of leaves Hubert\nhas collected in this level.","",0,SPECIAL_INFO_LEVEL_STAT_LEAVES);

        y_position+=40;
        windows_level_info[windows_level_info.size()-1].create_information(250,y_position,"The number of cheeses Hubert\nhas collected in this level.","",0,SPECIAL_INFO_LEVEL_STAT_CHEESE);

        y_position+=40;
        windows_level_info[windows_level_info.size()-1].create_information(250,y_position,"The time Hubert has\nspent in this level.","",0,SPECIAL_INFO_LEVEL_STAT_TOTAL_TIME);

        y_position+=40;
        windows_level_info[windows_level_info.size()-1].create_information(250,y_position,"Hubert's best time\nbeating this level.","",0,SPECIAL_INFO_LEVEL_STAT_BEST_TIME);

        msg="< ";
        msg+=level.return_level_name(levels_with_info[previous_window]);
        button_sound=10-1;
        windows_level_info[windows_level_info.size()-1].create_button(15,window_height-back_button_space,"",msg,&button_event_open_window_level_info,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE,previous_window);

        msg="Back to Main Menu";
        button_sound=12-1;
        windows_level_info[windows_level_info.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,window_height-back_button_space,"",msg,&button_event_open_window_main_menu,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

        msg=level.return_level_name(levels_with_info[next_window]);
        msg+=" >";
        button_sound=11-1;
        windows_level_info[windows_level_info.size()-1].create_button(window_width-20-msg.length()*font.spacing_x,window_height-back_button_space,"",msg,&button_event_open_window_level_info,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE,next_window);
    }

    vector_windows.clear();

    //************//
    // Main Menu: //
    //************//

    window_width=300;
    window_height=505;
    vector_windows.push_back(Window((main_window.SCREEN_WIDTH-window_width)/2,(main_window.SCREEN_HEIGHT-window_height)/2+40,window_width,window_height,"Main Menu",NO_SPECIAL_INPUT));

    vector_windows[vector_windows.size()-1].create_information((window_width-(16*12))/2,40,"","",0,SPECIAL_INFO_CURRENT_PROFILE);

    // Game not in progress //
    window_manager.main_menu_buttons_no_game.clear();

    msg="Change Profile";
    button_sound=7-1;
    y_position=90;
    //This button now begins disabled as part of the removal of the profile system
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_open_window_change_profile,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE,-1,BUTTON_DISABLED);
    window_manager.main_menu_buttons_no_game.push_back(vector_windows[vector_windows.size()-1].buttons.size()-1);
    msg="Play";
    button_sound=9-1;
    y_position+=45;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_open_window_select_game_mode,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);
    window_manager.main_menu_buttons_no_game.push_back(vector_windows[vector_windows.size()-1].buttons.size()-1);
    msg="Level Information";
    button_sound=7-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_open_window_level_info,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);
    window_manager.main_menu_buttons_no_game.push_back(vector_windows[vector_windows.size()-1].buttons.size()-1);
    msg="Inventory";
    button_sound=8-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_open_window_inventory,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);
    window_manager.main_menu_buttons_no_game.push_back(vector_windows[vector_windows.size()-1].buttons.size()-1);
    ///msg="Rewards";
    ///button_sound=12-1;
    ///y_position+=30;
    ///vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_open_window_rewards,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);
    window_manager.main_menu_buttons_no_game.push_back(vector_windows[vector_windows.size()-1].buttons.size()-1);
    msg="Stats";
    button_sound=10-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_open_window_stats,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);
    window_manager.main_menu_buttons_no_game.push_back(vector_windows[vector_windows.size()-1].buttons.size()-1);
    msg="Achievements";
    button_sound=8-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_open_window_achievements,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);
    window_manager.main_menu_buttons_no_game.push_back(vector_windows[vector_windows.size()-1].buttons.size()-1);
    msg="Options";
    button_sound=10-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_open_window_options,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);
    window_manager.main_menu_buttons_no_game.push_back(vector_windows[vector_windows.size()-1].buttons.size()-1);
    msg="Help";
    button_sound=12-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_open_window_help,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);
    window_manager.main_menu_buttons_no_game.push_back(vector_windows[vector_windows.size()-1].buttons.size()-1);
    msg="About";
    button_sound=8-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_open_window_about,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);
    window_manager.main_menu_buttons_no_game.push_back(vector_windows[vector_windows.size()-1].buttons.size()-1);
    msg="Credits";
    button_sound=7-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_open_window_credits,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);
    window_manager.main_menu_buttons_no_game.push_back(vector_windows[vector_windows.size()-1].buttons.size()-1);
    msg="Quit";
    button_sound=12-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_quit,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);
    window_manager.main_menu_buttons_no_game.push_back(vector_windows[vector_windows.size()-1].buttons.size()-1);

    // Game in progress: SP Adventure //
    window_manager.main_menu_buttons_sp_adventure.clear();

    msg="Change Profile";
    button_sound=7-1;
    y_position=90;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_open_window_change_profile,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE,-1,false);
    window_manager.main_menu_buttons_sp_adventure.push_back(vector_windows[vector_windows.size()-1].buttons.size()-1);
    msg="Return to World Map";
    button_sound=11-1;
    y_position+=45;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_return_to_world_map,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE,-1,false);
    window_manager.return_to_world_map_sp_adventure=vector_windows[vector_windows.size()-1].buttons.size()-1;
    window_manager.main_menu_buttons_sp_adventure.push_back(vector_windows[vector_windows.size()-1].buttons.size()-1);
    msg="Play";
    button_sound=9-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_start_game,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE,-1,false);
    window_manager.main_menu_buttons_sp_adventure.push_back(vector_windows[vector_windows.size()-1].buttons.size()-1);
    msg="Level Information";
    button_sound=7-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_open_window_level_info,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE,-1,false);
    window_manager.main_menu_buttons_sp_adventure.push_back(vector_windows[vector_windows.size()-1].buttons.size()-1);
    msg="Inventory";
    button_sound=8-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_open_window_inventory,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE,-1,false);
    window_manager.main_menu_buttons_sp_adventure.push_back(vector_windows[vector_windows.size()-1].buttons.size()-1);
    msg="Stats";
    button_sound=10-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_open_window_stats,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE,-1,false);
    window_manager.main_menu_buttons_sp_adventure.push_back(vector_windows[vector_windows.size()-1].buttons.size()-1);
    msg="Achievements";
    button_sound=8-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_open_window_achievements,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE,-1,false);
    window_manager.main_menu_buttons_sp_adventure.push_back(vector_windows[vector_windows.size()-1].buttons.size()-1);
    msg="Options";
    button_sound=10-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_open_window_options,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE,-1,false);
    window_manager.main_menu_buttons_sp_adventure.push_back(vector_windows[vector_windows.size()-1].buttons.size()-1);
    msg="Help";
    button_sound=12-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_open_window_help,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE,-1,false);
    window_manager.main_menu_buttons_sp_adventure.push_back(vector_windows[vector_windows.size()-1].buttons.size()-1);
    msg="About";
    button_sound=8-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_open_window_about,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE,-1,false);
    window_manager.main_menu_buttons_sp_adventure.push_back(vector_windows[vector_windows.size()-1].buttons.size()-1);
    msg="Credits";
    button_sound=7-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_open_window_credits,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE,-1,false);
    window_manager.main_menu_buttons_sp_adventure.push_back(vector_windows[vector_windows.size()-1].buttons.size()-1);
    msg="Quit";
    button_sound=8-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_quit,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE,-1,false);
    window_manager.main_menu_buttons_sp_adventure.push_back(vector_windows[vector_windows.size()-1].buttons.size()-1);
    msg="End Game";
    button_sound=12-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_end_game,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE,-1,false);
    window_manager.main_menu_buttons_sp_adventure.push_back(vector_windows[vector_windows.size()-1].buttons.size()-1);

    // Game in progress: SP Survival/MP Survival //
    window_manager.main_menu_buttons_survival.clear();

    msg="Change Profile";
    button_sound=7-1;
    y_position=90;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_open_window_change_profile,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE,-1,false);
    window_manager.main_menu_buttons_survival.push_back(vector_windows[vector_windows.size()-1].buttons.size()-1);
    msg="Play";
    button_sound=11-1;
    y_position+=45;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_start_game,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE,-1,false);
    window_manager.main_menu_buttons_survival.push_back(vector_windows[vector_windows.size()-1].buttons.size()-1);
    msg="Options";
    button_sound=9-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_open_window_options,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE,-1,false);
    window_manager.main_menu_buttons_survival.push_back(vector_windows[vector_windows.size()-1].buttons.size()-1);
    msg="Help";
    button_sound=7-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_open_window_help,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE,-1,false);
    window_manager.main_menu_buttons_survival.push_back(vector_windows[vector_windows.size()-1].buttons.size()-1);
    msg="About";
    button_sound=8-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_open_window_about,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE,-1,false);
    window_manager.main_menu_buttons_survival.push_back(vector_windows[vector_windows.size()-1].buttons.size()-1);
    msg="Credits";
    button_sound=10-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_open_window_credits,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE,-1,false);
    window_manager.main_menu_buttons_survival.push_back(vector_windows[vector_windows.size()-1].buttons.size()-1);
    msg="Quit";
    button_sound=8-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_quit,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE,-1,false);
    window_manager.main_menu_buttons_survival.push_back(vector_windows[vector_windows.size()-1].buttons.size()-1);
    msg="End Game";
    button_sound=12-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_end_game,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE,-1,false);
    window_manager.main_menu_buttons_survival.push_back(vector_windows[vector_windows.size()-1].buttons.size()-1);

    // Game in progress: MP Adventure //
    window_manager.main_menu_buttons_mp_adventure.clear();

    msg="Change Profile";
    button_sound=7-1;
    y_position=90;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_open_window_change_profile,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE,-1,false);
    window_manager.main_menu_buttons_mp_adventure.push_back(vector_windows[vector_windows.size()-1].buttons.size()-1);
    msg="Return to World Map";
    button_sound=11-1;
    y_position+=45;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_return_to_world_map,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE,-1,false);
    window_manager.return_to_world_map_mp_adventure=vector_windows[vector_windows.size()-1].buttons.size()-1;
    window_manager.main_menu_buttons_mp_adventure.push_back(vector_windows[vector_windows.size()-1].buttons.size()-1);
    msg="Play";
    button_sound=9-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_start_game,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE,-1,false);
    window_manager.main_menu_buttons_mp_adventure.push_back(vector_windows[vector_windows.size()-1].buttons.size()-1);
    msg="Inventory";
    button_sound=7-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_open_window_inventory,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE,-1,false);
    window_manager.main_menu_buttons_mp_adventure.push_back(vector_windows[vector_windows.size()-1].buttons.size()-1);
    msg="Options";
    button_sound=8-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_open_window_options,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE,-1,false);
    window_manager.main_menu_buttons_mp_adventure.push_back(vector_windows[vector_windows.size()-1].buttons.size()-1);
    msg="Help";
    button_sound=10-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_open_window_help,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE,-1,false);
    window_manager.main_menu_buttons_mp_adventure.push_back(vector_windows[vector_windows.size()-1].buttons.size()-1);
    msg="About";
    button_sound=8-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_open_window_about,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE,-1,false);
    window_manager.main_menu_buttons_mp_adventure.push_back(vector_windows[vector_windows.size()-1].buttons.size()-1);
    msg="Credits";
    button_sound=12-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_open_window_credits,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE,-1,false);
    window_manager.main_menu_buttons_mp_adventure.push_back(vector_windows[vector_windows.size()-1].buttons.size()-1);
    msg="Quit";
    button_sound=11-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_quit,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE,-1,false);
    window_manager.main_menu_buttons_mp_adventure.push_back(vector_windows[vector_windows.size()-1].buttons.size()-1);
    msg="End Game";
    button_sound=7-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_end_game,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE,-1,false);
    window_manager.main_menu_buttons_mp_adventure.push_back(vector_windows[vector_windows.size()-1].buttons.size()-1);

    //*****************//
    // Create Profile: //
    //*****************//

    window_width=300;
    window_height=400;
    vector_windows.push_back(Window((main_window.SCREEN_WIDTH-window_width)/2,(main_window.SCREEN_HEIGHT-window_height)/2,window_width,window_height,"Create Profile",SPECIAL_INPUT_CREATE_PROFILE));

    vector_windows[vector_windows.size()-1].create_information(15,40,"","",0,SPECIAL_INFO_NO_PROFILE);

    vector_windows[vector_windows.size()-1].create_information(15,65,"","",0,SPECIAL_INFO_PROFILE_TAKEN);

    vector_windows[vector_windows.size()-1].create_information((window_width-16*12)/2,140,"","Enter your name:",0,NO_SPECIAL_INFO);

    vector_windows[vector_windows.size()-1].create_information(15,165,"","",0,SPECIAL_INFO_CREATING_PROFILE);

    msg="Done";
    button_sound=11-1;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,240,"",msg,&button_event_create_profile,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Back to Profile Menu";
    button_sound=12-1;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,window_height-back_button_space,"",msg,&button_event_open_window_change_profile,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    //*****************//
    // Change Profile: //
    //*****************//

    window_width=360;
    window_height=600;
    vector_windows.push_back(Window((main_window.SCREEN_WIDTH-window_width)/2,(main_window.SCREEN_HEIGHT-window_height)/2,window_width,window_height,"Change Profile",NO_SPECIAL_INPUT));

    set_change_profile_buttons();

    //**********//
    // Rewards: //
    //**********//

    window_width=300;
    window_height=400;
    vector_windows.push_back(Window((main_window.SCREEN_WIDTH-window_width)/2,(main_window.SCREEN_HEIGHT-window_height)/2,window_width,window_height,"Rewards",NO_SPECIAL_INPUT));

    //********//
    // Stats: //
    //********//

    window_width=530;
    window_height=530;
    vector_windows.push_back(Window((main_window.SCREEN_WIDTH-window_width)/2,(main_window.SCREEN_HEIGHT-window_height)/2,window_width,window_height,"Stats",NO_SPECIAL_INPUT));

    y_position=40;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"The overall percentage of the game\nthat Hubert has completed.","",0,SPECIAL_INFO_STAT_PERCENT_COMPLETE);

    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"The number of leaves Hubert\nhas collected, and the total number\nof leaves in the game.","",0,SPECIAL_INFO_STAT_LEAVES_COLLECTED);

    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"The number of cheeses Hubert\nhas collected, and the total number\nof cheeses in the game.","",0,SPECIAL_INFO_STAT_CHEESE_COLLECTED);

    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"The number of bosses\nHubert has defeated.","",0,SPECIAL_INFO_STAT_BOSSES_DEFEATED);

    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"The time Hubert has\nspent playing the game.","",0,SPECIAL_INFO_STAT_SECONDS_PLAYING);

    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"The time Hubert has\nspent traveling on the world map.","",0,SPECIAL_INFO_STAT_SECONDS_TRAVELING);

    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"The time Hubert has\nspent in the menus.","",0,SPECIAL_INFO_STAT_SECONDS_MENUS);

    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"The time Hubert has\nspent riding cows.","",0,SPECIAL_INFO_STAT_SECONDS_RIDING_COWS);

    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"Hubert's accuracy\nwith the stunner.","",0,SPECIAL_INFO_STAT_ACCURACY);

    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"The number of times Hubert\nhas fired the stunner.","",0,SPECIAL_INFO_STAT_SHOTS_FIRED);

    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"The number of enemies Hubert has\nhit with the stunner.","",0,SPECIAL_INFO_STAT_ENEMIES_STUNNED);

    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"The number of ammo boxes\nHubert has collected.","",0,SPECIAL_INFO_STAT_AMMO_PICKED_UP);

    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"The number of times\nHubert has shot water.","",0,SPECIAL_INFO_STAT_WATER_SHOT);

    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"The number of levers Hubert has\ntriggered by shooting them.","",0,SPECIAL_INFO_STAT_TRIGGERS_SHOT);

    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"The number of levers\nHubert has pulled.","",0,SPECIAL_INFO_STAT_LEVERS_PULLED);

    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"The number of times\nHubert has jumped.","",0,SPECIAL_INFO_STAT_TOTAL_JUMPS);

    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"The farthest Hubert\nhas ever fallen.","",0,SPECIAL_INFO_STAT_FARTHEST_FALL);

    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"The number of checkpoints\nHubert has reached.","",0,SPECIAL_INFO_STAT_CHECKPOINTS_ACTIVATED);

    msg="< Page 2";
    button_sound=10-1;
    vector_windows[vector_windows.size()-1].create_button(15,window_height-back_button_space,"",msg,&button_event_open_window_stats_2,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Back to Main Menu";
    button_sound=12-1;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,window_height-back_button_space,"",msg,&button_event_open_window_main_menu,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Page 2 >";
    button_sound=11-1;
    vector_windows[vector_windows.size()-1].create_button(window_width-20-msg.length()*font.spacing_x,window_height-back_button_space,"",msg,&button_event_open_window_stats_2,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    //**********//
    // Stats 2: //
    //**********//

    window_width=530;
    window_height=530;
    vector_windows.push_back(Window((main_window.SCREEN_WIDTH-window_width)/2,(main_window.SCREEN_HEIGHT-window_height)/2,window_width,window_height,"Stats",NO_SPECIAL_INPUT));

    y_position=40;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"The total number of times\nHubert has been defeated.","",0,SPECIAL_INFO_STAT_DEATHS_TOTAL);

    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"The number of times Hubert has\nbeen defeated by enemies.","",0,SPECIAL_INFO_STAT_DEATHS_ENEMIES);

    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"The number of times Hubert has\nbeen defeated by traps.","",0,SPECIAL_INFO_STAT_DEATHS_TRAPS);

    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"The number of times\nHubert has drowned.","",0,SPECIAL_INFO_STAT_DEATHS_DROWNING);

    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"The number of levels\nHubert has replayed.","",0,SPECIAL_INFO_STAT_LEVELS_REPLAYED);

    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"The number of times Hubert\nhas moved an item in the inventory.","",0,SPECIAL_INFO_STAT_ITEMS_MOVED);

    msg="< Page 1";
    button_sound=10-1;
    vector_windows[vector_windows.size()-1].create_button(15,window_height-back_button_space,"",msg,&button_event_open_window_stats,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Back to Main Menu";
    button_sound=12-1;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,window_height-back_button_space,"",msg,&button_event_open_window_main_menu,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Page 1 >";
    button_sound=11-1;
    vector_windows[vector_windows.size()-1].create_button(window_width-20-msg.length()*font.spacing_x,window_height-back_button_space,"",msg,&button_event_open_window_stats,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    //***************//
    // Achievements: //
    //***************//

    window_width=530;
    window_height=530;
    vector_windows.push_back(Window((main_window.SCREEN_WIDTH-window_width)/2,(main_window.SCREEN_HEIGHT-window_height)/2,window_width,window_height,"Achievements",NO_SPECIAL_INPUT));

    for(int menu_x=0,achievement=0*15;menu_x<15;menu_x++,achievement++){
        vector_windows[vector_windows.size()-1].create_information(15,40+30*menu_x,achievement_tooltips[achievement],"",0,SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+achievement);
        vector_windows[vector_windows.size()-1].create_information(42,38+30*menu_x,achievement_tooltips[achievement],"",0,SPECIAL_INFO_ACHIEVEMENT+achievement);
    }

    msg="< Page 8";
    button_sound=10-1;
    vector_windows[vector_windows.size()-1].create_button(15,window_height-back_button_space,"",msg,&button_event_open_window_achievements_8,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Back to Main Menu";
    button_sound=12-1;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,window_height-back_button_space,"",msg,&button_event_open_window_main_menu,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Page 2 >";
    button_sound=11-1;
    vector_windows[vector_windows.size()-1].create_button(window_width-20-msg.length()*font.spacing_x,window_height-back_button_space,"",msg,&button_event_open_window_achievements_2,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    //*****************//
    // Achievements 2: //
    //*****************//

    window_width=530;
    window_height=530;
    vector_windows.push_back(Window((main_window.SCREEN_WIDTH-window_width)/2,(main_window.SCREEN_HEIGHT-window_height)/2,window_width,window_height,"Achievements",NO_SPECIAL_INPUT));

    for(int menu_x=0,achievement=1*15;menu_x<15;menu_x++,achievement++){
        vector_windows[vector_windows.size()-1].create_information(15,40+30*menu_x,achievement_tooltips[achievement],"",0,SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+achievement);
        vector_windows[vector_windows.size()-1].create_information(42,38+30*menu_x,achievement_tooltips[achievement],"",0,SPECIAL_INFO_ACHIEVEMENT+achievement);
    }

    msg="< Page 1";
    button_sound=10-1;
    vector_windows[vector_windows.size()-1].create_button(15,window_height-back_button_space,"",msg,&button_event_open_window_achievements,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Back to Main Menu";
    button_sound=12-1;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,window_height-back_button_space,"",msg,&button_event_open_window_main_menu,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Page 3 >";
    button_sound=11-1;
    vector_windows[vector_windows.size()-1].create_button(window_width-20-msg.length()*font.spacing_x,window_height-back_button_space,"",msg,&button_event_open_window_achievements_3,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    //*****************//
    // Achievements 3: //
    //*****************//

    window_width=530;
    window_height=530;
    vector_windows.push_back(Window((main_window.SCREEN_WIDTH-window_width)/2,(main_window.SCREEN_HEIGHT-window_height)/2,window_width,window_height,"Achievements",NO_SPECIAL_INPUT));

    for(int menu_x=0,achievement=2*15;menu_x<15;menu_x++,achievement++){
        vector_windows[vector_windows.size()-1].create_information(15,40+30*menu_x,achievement_tooltips[achievement],"",0,SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+achievement);
        vector_windows[vector_windows.size()-1].create_information(42,38+30*menu_x,achievement_tooltips[achievement],"",0,SPECIAL_INFO_ACHIEVEMENT+achievement);
    }

    msg="< Page 2";
    button_sound=10-1;
    vector_windows[vector_windows.size()-1].create_button(15,window_height-back_button_space,"",msg,&button_event_open_window_achievements_2,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Back to Main Menu";
    button_sound=12-1;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,window_height-back_button_space,"",msg,&button_event_open_window_main_menu,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Page 4 >";
    button_sound=11-1;
    vector_windows[vector_windows.size()-1].create_button(window_width-20-msg.length()*font.spacing_x,window_height-back_button_space,"",msg,&button_event_open_window_achievements_4,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    //*****************//
    // Achievements 4: //
    //*****************//

    window_width=530;
    window_height=530;
    vector_windows.push_back(Window((main_window.SCREEN_WIDTH-window_width)/2,(main_window.SCREEN_HEIGHT-window_height)/2,window_width,window_height,"Achievements",NO_SPECIAL_INPUT));

    for(int menu_x=0,achievement=3*15;menu_x<15;menu_x++,achievement++){
        vector_windows[vector_windows.size()-1].create_information(15,40+30*menu_x,achievement_tooltips[achievement],"",0,SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+achievement);
        vector_windows[vector_windows.size()-1].create_information(42,38+30*menu_x,achievement_tooltips[achievement],"",0,SPECIAL_INFO_ACHIEVEMENT+achievement);
    }

    msg="< Page 3";
    button_sound=10-1;
    vector_windows[vector_windows.size()-1].create_button(15,window_height-back_button_space,"",msg,&button_event_open_window_achievements_3,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Back to Main Menu";
    button_sound=12-1;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,window_height-back_button_space,"",msg,&button_event_open_window_main_menu,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Page 5 >";
    button_sound=11-1;
    vector_windows[vector_windows.size()-1].create_button(window_width-20-msg.length()*font.spacing_x,window_height-back_button_space,"",msg,&button_event_open_window_achievements_5,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    //*****************//
    // Achievements 5: //
    //*****************//

    window_width=530;
    window_height=530;
    vector_windows.push_back(Window((main_window.SCREEN_WIDTH-window_width)/2,(main_window.SCREEN_HEIGHT-window_height)/2,window_width,window_height,"Achievements",NO_SPECIAL_INPUT));

    for(int menu_x=0,achievement=4*15;menu_x<15;menu_x++,achievement++){
        vector_windows[vector_windows.size()-1].create_information(15,40+30*menu_x,achievement_tooltips[achievement],"",0,SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+achievement);
        vector_windows[vector_windows.size()-1].create_information(42,38+30*menu_x,achievement_tooltips[achievement],"",0,SPECIAL_INFO_ACHIEVEMENT+achievement);
    }

    msg="< Page 4";
    button_sound=10-1;
    vector_windows[vector_windows.size()-1].create_button(15,window_height-back_button_space,"",msg,&button_event_open_window_achievements_4,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Back to Main Menu";
    button_sound=12-1;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,window_height-back_button_space,"",msg,&button_event_open_window_main_menu,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Page 6 >";
    button_sound=11-1;
    vector_windows[vector_windows.size()-1].create_button(window_width-20-msg.length()*font.spacing_x,window_height-back_button_space,"",msg,&button_event_open_window_achievements_6,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    //*****************//
    // Achievements 6: //
    //*****************//

    window_width=530;
    window_height=530;
    vector_windows.push_back(Window((main_window.SCREEN_WIDTH-window_width)/2,(main_window.SCREEN_HEIGHT-window_height)/2,window_width,window_height,"Achievements",NO_SPECIAL_INPUT));

    for(int menu_x=0,achievement=5*15;menu_x<15;menu_x++,achievement++){
        vector_windows[vector_windows.size()-1].create_information(15,40+30*menu_x,achievement_tooltips[achievement],"",0,SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+achievement);
        vector_windows[vector_windows.size()-1].create_information(42,38+30*menu_x,achievement_tooltips[achievement],"",0,SPECIAL_INFO_ACHIEVEMENT+achievement);
    }

    msg="< Page 5";
    button_sound=10-1;
    vector_windows[vector_windows.size()-1].create_button(15,window_height-back_button_space,"",msg,&button_event_open_window_achievements_5,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Back to Main Menu";
    button_sound=12-1;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,window_height-back_button_space,"",msg,&button_event_open_window_main_menu,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Page 7 >";
    button_sound=11-1;
    vector_windows[vector_windows.size()-1].create_button(window_width-20-msg.length()*font.spacing_x,window_height-back_button_space,"",msg,&button_event_open_window_achievements_7,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    //*****************//
    // Achievements 7: //
    //*****************//

    window_width=530;
    window_height=530;
    vector_windows.push_back(Window((main_window.SCREEN_WIDTH-window_width)/2,(main_window.SCREEN_HEIGHT-window_height)/2,window_width,window_height,"Achievements",NO_SPECIAL_INPUT));

    for(int menu_x=0,achievement=6*15;menu_x<15;menu_x++,achievement++){
        vector_windows[vector_windows.size()-1].create_information(15,40+30*menu_x,achievement_tooltips[achievement],"",0,SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+achievement);
        vector_windows[vector_windows.size()-1].create_information(42,38+30*menu_x,achievement_tooltips[achievement],"",0,SPECIAL_INFO_ACHIEVEMENT+achievement);
    }

    msg="< Page 6";
    button_sound=10-1;
    vector_windows[vector_windows.size()-1].create_button(15,window_height-back_button_space,"",msg,&button_event_open_window_achievements_6,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Back to Main Menu";
    button_sound=12-1;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,window_height-back_button_space,"",msg,&button_event_open_window_main_menu,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Page 8 >";
    button_sound=11-1;
    vector_windows[vector_windows.size()-1].create_button(window_width-20-msg.length()*font.spacing_x,window_height-back_button_space,"",msg,&button_event_open_window_achievements_8,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    //*****************//
    // Achievements 8: //
    //*****************//

    window_width=530;
    window_height=530;
    vector_windows.push_back(Window((main_window.SCREEN_WIDTH-window_width)/2,(main_window.SCREEN_HEIGHT-window_height)/2,window_width,window_height,"Achievements",NO_SPECIAL_INPUT));

    for(int menu_x=0,achievement=7*15;menu_x<3;menu_x++,achievement++){
        vector_windows[vector_windows.size()-1].create_information(15,40+30*menu_x,achievement_tooltips[achievement],"",0,SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+achievement);
        vector_windows[vector_windows.size()-1].create_information(42,38+30*menu_x,achievement_tooltips[achievement],"",0,SPECIAL_INFO_ACHIEVEMENT+achievement);
    }

    msg="< Page 7";
    button_sound=10-1;
    vector_windows[vector_windows.size()-1].create_button(15,window_height-back_button_space,"",msg,&button_event_open_window_achievements_7,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Back to Main Menu";
    button_sound=12-1;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,window_height-back_button_space,"",msg,&button_event_open_window_main_menu,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Page 1 >";
    button_sound=11-1;
    vector_windows[vector_windows.size()-1].create_button(window_width-20-msg.length()*font.spacing_x,window_height-back_button_space,"",msg,&button_event_open_window_achievements,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    //**********//
    // Options: //
    //**********//

    window_width=600;
    window_height=520;
    vector_windows.push_back(Window((main_window.SCREEN_WIDTH-window_width)/2,(main_window.SCREEN_HEIGHT-window_height)/2,window_width,window_height,"Options",NO_SPECIAL_INPUT));

    msg="Display Options";
    vector_windows[vector_windows.size()-1].create_information(75,35,"",msg,0,NO_SPECIAL_INFO);

    msg="Fullscreen:";
    button_sound=7-1;
    vector_windows[vector_windows.size()-1].create_information(15,65,"Toggle fullscreen mode.",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(200,67,"","",0,SPECIAL_INFO_CHECKBOX_FULLSCREEN);
    vector_windows[vector_windows.size()-1].create_button(200-1,67,"Toggle fullscreen mode.","  ",&button_event_option_fullscreen,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Show FPS:";
    button_sound=11-1;
    vector_windows[vector_windows.size()-1].create_information(15,95,"Toggle FPS display.",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(200,97,"","",0,SPECIAL_INFO_CHECKBOX_FPS);
    vector_windows[vector_windows.size()-1].create_button(200-1,97,"Toggle FPS display.","  ",&button_event_option_fps,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Hardware Cursor:";
    button_sound=9-1;
    vector_windows[vector_windows.size()-1].create_information(15,125,"Try enabling this if\nthe cursor isn't\nvery responsive.",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(200,127,"","",0,SPECIAL_INFO_CHECKBOX_HARDWARE_CURSOR);
    vector_windows[vector_windows.size()-1].create_button(200-1,127,"Try enabling this if\nthe cursor isn't\nvery responsive.","  ",&button_event_option_hardware_cursor,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Audio Options";
    vector_windows[vector_windows.size()-1].create_information(375,35,"",msg,0,NO_SPECIAL_INFO);

    msg="Global Volume";
    vector_windows[vector_windows.size()-1].create_information(315,65,"",msg,0,NO_SPECIAL_INFO);
    msg="Mute:";
    button_sound=7-1;
    vector_windows[vector_windows.size()-1].create_information(315,95,"Mute everything.",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(375,97,"","",0,SPECIAL_INFO_CHECKBOX_MUTE_GLOBAL);
    vector_windows[vector_windows.size()-1].create_button(375-1,97,"Mute everything.","  ",&button_event_option_mute_global,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE);
    msg="-";
    button_sound=8-1;
    vector_windows[vector_windows.size()-1].create_button(315,125,"Decrease the volume of everything.",msg,&button_event_option_volume_down_global,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);
    msg="+";
    button_sound=12-1;
    vector_windows[vector_windows.size()-1].create_button(558,125,"Increase the volume of everything.",msg,&button_event_option_volume_up_global,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);
    vector_windows[vector_windows.size()-1].create_information(345,134,"","",0,SPECIAL_INFO_VOLUME_GLOBAL);

    msg="Sound Effects Volume";
    vector_windows[vector_windows.size()-1].create_information(315,160,"",msg,0,NO_SPECIAL_INFO);
    msg="Mute:";
    button_sound=10-1;
    vector_windows[vector_windows.size()-1].create_information(315,190,"Mute sound effects.",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(375,192,"","",0,SPECIAL_INFO_CHECKBOX_MUTE_SOUND);
    vector_windows[vector_windows.size()-1].create_button(375-1,192,"Mute sound effects.","  ",&button_event_option_mute_sound,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE);
    msg="-";
    button_sound=8-1;
    vector_windows[vector_windows.size()-1].create_button(315,220,"Decrease the volume of sound effects.",msg,&button_event_option_volume_down_sound,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);
    msg="+";
    button_sound=10-1;
    vector_windows[vector_windows.size()-1].create_button(558,220,"Increase the volume of sound effects.",msg,&button_event_option_volume_up_sound,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);
    vector_windows[vector_windows.size()-1].create_information(345,229,"","",0,SPECIAL_INFO_VOLUME_SOUND);

    msg="Music Volume";
    vector_windows[vector_windows.size()-1].create_information(315,255,"",msg,0,NO_SPECIAL_INFO);
    msg="Mute:";
    button_sound=12-1;
    vector_windows[vector_windows.size()-1].create_information(315,285,"Mute the music.",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(375,287,"","",0,SPECIAL_INFO_CHECKBOX_MUTE_MUSIC);
    vector_windows[vector_windows.size()-1].create_button(375-1,287,"Mute the music.","  ",&button_event_option_mute_music,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE);
    msg="-";
    button_sound=8-1;
    vector_windows[vector_windows.size()-1].create_button(315,315,"Decrease the volume of the music.",msg,&button_event_option_volume_down_music,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);
    msg="+";
    button_sound=7-1;
    vector_windows[vector_windows.size()-1].create_button(558,315,"Increase the volume of the music.",msg,&button_event_option_volume_up_music,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);
    vector_windows[vector_windows.size()-1].create_information(345,324,"","",0,SPECIAL_INFO_VOLUME_MUSIC);

    msg="Other Options";
    vector_windows[vector_windows.size()-1].create_information(75,165,"",msg,0,NO_SPECIAL_INFO);

    msg="Holiday Cheer:";
    button_sound=8-1;
    vector_windows[vector_windows.size()-1].create_information(15,195,"Toggle holiday decorations and effects.",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(200,197,"","",0,SPECIAL_INFO_CHECKBOX_HOLIDAY_CHEER);
    vector_windows[vector_windows.size()-1].create_button(200-1,197,"Toggle holiday decorations and effects.","  ",&button_event_option_holiday_cheer,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="World Lighting:";
    button_sound=12-1;
    vector_windows[vector_windows.size()-1].create_information(15,225,"Toggle day/night effects on the world map.",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(200,227,"","",0,SPECIAL_INFO_CHECKBOX_TIME_OF_DAY);
    vector_windows[vector_windows.size()-1].create_button(200-1,227,"Toggle day/night effects on the world map.","  ",&button_event_option_time_of_day,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Level Lighting:";
    button_sound=10-1;
    vector_windows[vector_windows.size()-1].create_information(15,255,"Toggle day/night effects in levels.",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(200,257,"","",0,SPECIAL_INFO_CHECKBOX_TIME_OF_DAY_LEVELS);
    vector_windows[vector_windows.size()-1].create_button(200-1,257,"Toggle day/night effects in levels.","  ",&button_event_option_time_of_day_levels,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Tips:";
    button_sound=8-1;
    vector_windows[vector_windows.size()-1].create_information(15,285,"Toggle tips for new players.",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(200,287,"","",0,SPECIAL_INFO_CHECKBOX_HINTS);
    vector_windows[vector_windows.size()-1].create_button(200-1,287,"Toggle tips for new players.","  ",&button_event_option_hints,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Difficulty: ";
    button_sound=10-1;
    x_position=msg.length()*font.spacing_x;
    vector_windows[vector_windows.size()-1].create_information(15,315,"Toggle game difficulty.",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(15+x_position,317,"","",0,SPECIAL_INFO_DIFFICULTY);
    vector_windows[vector_windows.size()-1].create_button(15+x_position-1,317,"Toggle game difficulty.","      ",&button_event_option_difficulty,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Sound Test";
    button_sound=12-1;
    vector_windows[vector_windows.size()-1].create_button(window_width-20-msg.length()*font.spacing_x,window_height-back_button_space,"",msg,&button_event_open_window_sound_test,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Configure Player 1 Controls";
    button_sound=8-1;
    y_position=357;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_open_window_options_keys,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Configure Player 2 Controls";
    button_sound=6-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_open_window_options_keys_1,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Configure Player 3 Controls";
    button_sound=7-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_open_window_options_keys_2,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Configure Player 4 Controls";
    button_sound=9-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_open_window_options_keys_3,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Save Location";
    button_sound=10-1;
    vector_windows[vector_windows.size()-1].create_button(15,window_height-back_button_space,"",msg,&button_event_show_data_location,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Back to Main Menu";
    button_sound=11-1;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,window_height-back_button_space,"",msg,&button_event_open_window_main_menu,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    //********************//
    // Key Configuration: //
    //********************//

    window_width=600;
    window_height=600;
    vector_windows.push_back(Window((main_window.SCREEN_WIDTH-window_width)/2,(main_window.SCREEN_HEIGHT-window_height)/2,window_width,window_height,"Controls - Player 1",NO_SPECIAL_INPUT));

    msg="Left:";
    button_sound=7-1;
    y_position=30;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(145,y_position,"","",0,SPECIAL_INFO_KEY_LEFT);
    vector_windows[vector_windows.size()-1].create_button(15,y_position,"","                                                   ",&button_event_key_left,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Up:";
    button_sound=11-1;
    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(145,y_position,"","",0,SPECIAL_INFO_KEY_UP);
    vector_windows[vector_windows.size()-1].create_button(15,y_position,"","                                                   ",&button_event_key_up,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Right:";
    button_sound=9-1;
    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(145,y_position,"","",0,SPECIAL_INFO_KEY_RIGHT);
    vector_windows[vector_windows.size()-1].create_button(15,y_position,"","                                                   ",&button_event_key_right,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Down:";
    button_sound=7-1;
    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(145,y_position,"","",0,SPECIAL_INFO_KEY_DOWN);
    vector_windows[vector_windows.size()-1].create_button(15,y_position,"","                                                   ",&button_event_key_down,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Jump:";
    button_sound=8-1;
    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(145,y_position,"","",0,SPECIAL_INFO_KEY_JUMP);
    vector_windows[vector_windows.size()-1].create_button(15,y_position,"","                                                   ",&button_event_key_jump,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Shoot:";
    button_sound=12-1;
    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(145,y_position,"","",0,SPECIAL_INFO_KEY_SHOOT);
    vector_windows[vector_windows.size()-1].create_button(15,y_position,"","                                                   ",&button_event_key_shoot,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Change Shot:";
    button_sound=10-1;
    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(145,y_position,"","",0,SPECIAL_INFO_KEY_CHANGE_SHOT);
    vector_windows[vector_windows.size()-1].create_button(15,y_position,"","                                                   ",&button_event_key_change_shot,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Look:";
    button_sound=8-1;
    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(145,y_position,"","",0,SPECIAL_INFO_KEY_LOOK);
    vector_windows[vector_windows.size()-1].create_button(15,y_position,"","                                                   ",&button_event_key_look,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Change Character:";
    button_sound=10-1;
    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(145,y_position,"","",0,SPECIAL_INFO_KEY_CHANGE_CHARACTER);
    vector_windows[vector_windows.size()-1].create_button(15,y_position,"","                                                   ",&button_event_key_change_character,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Pause:";
    button_sound=12-1;
    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(145,y_position,"","",0,SPECIAL_INFO_KEY_PAUSE);
    vector_windows[vector_windows.size()-1].create_button(15,y_position,"","                                                   ",&button_event_key_pause,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Toggle Main Menu:";
    button_sound=8-1;
    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(145,y_position,"","",0,SPECIAL_INFO_KEY_TOGGLE_MAIN_MENU);
    vector_windows[vector_windows.size()-1].create_button(15,y_position,"","                                                   ",&button_event_key_toggle_main_menu,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Toggle Inventory:";
    button_sound=7-1;
    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(145,y_position,"","",0,SPECIAL_INFO_KEY_TOGGLE_INVENTORY);
    vector_windows[vector_windows.size()-1].create_button(15,y_position,"","                                                   ",&button_event_key_toggle_inventory,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Toggle Map:";
    button_sound=8-1;
    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(145,y_position,"","",0,SPECIAL_INFO_KEY_TOGGLE_MAP);
    vector_windows[vector_windows.size()-1].create_button(15,y_position,"","                                                   ",&button_event_key_toggle_map,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Take Screenshot:";
    button_sound=12-1;
    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(145,y_position,"","",0,SPECIAL_INFO_KEY_SCREENSHOT);
    vector_windows[vector_windows.size()-1].create_button(15,y_position,"","                                                   ",&button_event_key_screenshot,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Restore Defaults\n   (Keyboard)";
    button_sound=10-1;
    y_position+=185;
    vector_windows[vector_windows.size()-1].create_button(10,y_position,"Set controls to some\nnice defaults for\nthe keyboard.",msg,&button_event_default_keys_keyboard,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Restore Defaults (Controller 0)";
    button_sound=11-1;
    y_position=385;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"Set controls to some\nnice defaults for\nthe 360 controller.",msg,&button_event_default_keys_joystick_0,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE,-1);

    msg="Restore Defaults Controller 1)";
    button_sound=9-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"Set controls to some\nnice defaults for\nthe 360 controller.",msg,&button_event_default_keys_joystick_1,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE,-1);

    msg="Restore Defaults (Controller 2)";
    button_sound=8-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"Set controls to some\nnice defaults for\nthe 360 controller.",msg,&button_event_default_keys_joystick_2,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE,-1);

    msg="Restore Defaults (Controller 3)";
    button_sound=6-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"Set controls to some\nnice defaults for\nthe 360 controller.",msg,&button_event_default_keys_joystick_3,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE,-1);

    msg="Back to Options";
    button_sound=12-1;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,window_height-back_button_space,"",msg,&button_event_open_window_options,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    //*************************//
    // Key Configuration MP 1: //
    //*************************//

    window_width=600;
    window_height=425;
    vector_windows.push_back(Window((main_window.SCREEN_WIDTH-window_width)/2,(main_window.SCREEN_HEIGHT-window_height)/2,window_width,window_height,"Controls - Player 2",NO_SPECIAL_INPUT));

    msg="Left:";
    button_sound=7-1;
    y_position=30;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(145,y_position,"","",0,SPECIAL_INFO_KEY_1_LEFT);
    vector_windows[vector_windows.size()-1].create_button(15,y_position,"","                                                   ",&button_event_key_1_left,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Up:";
    button_sound=11-1;
    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(145,y_position,"","",0,SPECIAL_INFO_KEY_1_UP);
    vector_windows[vector_windows.size()-1].create_button(15,y_position,"","                                                   ",&button_event_key_1_up,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Right:";
    button_sound=9-1;
    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(145,y_position,"","",0,SPECIAL_INFO_KEY_1_RIGHT);
    vector_windows[vector_windows.size()-1].create_button(15,y_position,"","                                                   ",&button_event_key_1_right,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Down:";
    button_sound=7-1;
    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(145,y_position,"","",0,SPECIAL_INFO_KEY_1_DOWN);
    vector_windows[vector_windows.size()-1].create_button(15,y_position,"","                                                   ",&button_event_key_1_down,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Jump:";
    button_sound=8-1;
    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(145,y_position,"","",0,SPECIAL_INFO_KEY_1_JUMP);
    vector_windows[vector_windows.size()-1].create_button(15,y_position,"","                                                   ",&button_event_key_1_jump,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Shoot:";
    button_sound=12-1;
    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(145,y_position,"","",0,SPECIAL_INFO_KEY_1_SHOOT);
    vector_windows[vector_windows.size()-1].create_button(15,y_position,"","                                                   ",&button_event_key_1_shoot,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Change Shot:";
    button_sound=10-1;
    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(145,y_position,"","",0,SPECIAL_INFO_KEY_1_CHANGE_SHOT);
    vector_windows[vector_windows.size()-1].create_button(15,y_position,"","                                                   ",&button_event_key_1_change_shot,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Restore Defaults\n   (Keyboard)";
    button_sound=8-1;
    y_position+=185;
    vector_windows[vector_windows.size()-1].create_button(10,y_position,"Set controls to some\nnice defaults for\nthe keyboard.",msg,&button_event_default_keys_1_keyboard,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Restore Defaults (Controller 0)";
    button_sound=11-1;
    y_position=210;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"Set controls to some\nnice defaults for\nthe 360 controller.",msg,&button_event_default_keys_joystick_0,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE,0);

    msg="Restore Defaults Controller 1)";
    button_sound=9-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"Set controls to some\nnice defaults for\nthe 360 controller.",msg,&button_event_default_keys_joystick_1,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE,0);

    msg="Restore Defaults (Controller 2)";
    button_sound=8-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"Set controls to some\nnice defaults for\nthe 360 controller.",msg,&button_event_default_keys_joystick_2,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE,0);

    msg="Restore Defaults (Controller 3)";
    button_sound=6-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"Set controls to some\nnice defaults for\nthe 360 controller.",msg,&button_event_default_keys_joystick_3,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE,0);

    msg="Back to Options";
    button_sound=12-1;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,window_height-back_button_space,"",msg,&button_event_open_window_options,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    //*************************//
    // Key Configuration MP 2: //
    //*************************//

    window_width=600;
    window_height=425;
    vector_windows.push_back(Window((main_window.SCREEN_WIDTH-window_width)/2,(main_window.SCREEN_HEIGHT-window_height)/2,window_width,window_height,"Controls - Player 3",NO_SPECIAL_INPUT));

    msg="Left:";
    button_sound=7-1;
    y_position=30;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(145,y_position,"","",0,SPECIAL_INFO_KEY_2_LEFT);
    vector_windows[vector_windows.size()-1].create_button(15,y_position,"","                                                   ",&button_event_key_2_left,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Up:";
    button_sound=11-1;
    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(145,y_position,"","",0,SPECIAL_INFO_KEY_2_UP);
    vector_windows[vector_windows.size()-1].create_button(15,y_position,"","                                                   ",&button_event_key_2_up,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Right:";
    button_sound=9-1;
    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(145,y_position,"","",0,SPECIAL_INFO_KEY_2_RIGHT);
    vector_windows[vector_windows.size()-1].create_button(15,y_position,"","                                                   ",&button_event_key_2_right,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Down:";
    button_sound=7-1;
    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(145,y_position,"","",0,SPECIAL_INFO_KEY_2_DOWN);
    vector_windows[vector_windows.size()-1].create_button(15,y_position,"","                                                   ",&button_event_key_2_down,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Jump:";
    button_sound=8-1;
    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(145,y_position,"","",0,SPECIAL_INFO_KEY_2_JUMP);
    vector_windows[vector_windows.size()-1].create_button(15,y_position,"","                                                   ",&button_event_key_2_jump,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Shoot:";
    button_sound=12-1;
    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(145,y_position,"","",0,SPECIAL_INFO_KEY_2_SHOOT);
    vector_windows[vector_windows.size()-1].create_button(15,y_position,"","                                                   ",&button_event_key_2_shoot,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Change Shot:";
    button_sound=10-1;
    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(145,y_position,"","",0,SPECIAL_INFO_KEY_2_CHANGE_SHOT);
    vector_windows[vector_windows.size()-1].create_button(15,y_position,"","                                                   ",&button_event_key_2_change_shot,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Restore Defaults\n   (Keyboard)";
    button_sound=8-1;
    y_position+=185;
    vector_windows[vector_windows.size()-1].create_button(10,y_position,"Set controls to some\nnice defaults for\nthe keyboard.",msg,&button_event_default_keys_2_keyboard,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Restore Defaults (Controller 0)";
    button_sound=11-1;
    y_position=210;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"Set controls to some\nnice defaults for\nthe 360 controller.",msg,&button_event_default_keys_joystick_0,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE,1);

    msg="Restore Defaults Controller 1)";
    button_sound=9-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"Set controls to some\nnice defaults for\nthe 360 controller.",msg,&button_event_default_keys_joystick_1,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE,1);

    msg="Restore Defaults (Controller 2)";
    button_sound=8-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"Set controls to some\nnice defaults for\nthe 360 controller.",msg,&button_event_default_keys_joystick_2,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE,1);

    msg="Restore Defaults (Controller 3)";
    button_sound=6-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"Set controls to some\nnice defaults for\nthe 360 controller.",msg,&button_event_default_keys_joystick_3,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE,1);

    msg="Back to Options";
    button_sound=12-1;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,window_height-back_button_space,"",msg,&button_event_open_window_options,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    //*************************//
    // Key Configuration MP 3: //
    //*************************//

    window_width=600;
    window_height=425;
    vector_windows.push_back(Window((main_window.SCREEN_WIDTH-window_width)/2,(main_window.SCREEN_HEIGHT-window_height)/2,window_width,window_height,"Controls - Player 4",NO_SPECIAL_INPUT));

    msg="Left:";
    button_sound=7-1;
    y_position=30;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(145,y_position,"","",0,SPECIAL_INFO_KEY_3_LEFT);
    vector_windows[vector_windows.size()-1].create_button(15,y_position,"","                                                   ",&button_event_key_3_left,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Up:";
    button_sound=11-1;
    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(145,y_position,"","",0,SPECIAL_INFO_KEY_3_UP);
    vector_windows[vector_windows.size()-1].create_button(15,y_position,"","                                                   ",&button_event_key_3_up,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Right:";
    button_sound=9-1;
    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(145,y_position,"","",0,SPECIAL_INFO_KEY_3_RIGHT);
    vector_windows[vector_windows.size()-1].create_button(15,y_position,"","                                                   ",&button_event_key_3_right,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Down:";
    button_sound=7-1;
    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(145,y_position,"","",0,SPECIAL_INFO_KEY_3_DOWN);
    vector_windows[vector_windows.size()-1].create_button(15,y_position,"","                                                   ",&button_event_key_3_down,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Jump:";
    button_sound=8-1;
    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(145,y_position,"","",0,SPECIAL_INFO_KEY_3_JUMP);
    vector_windows[vector_windows.size()-1].create_button(15,y_position,"","                                                   ",&button_event_key_3_jump,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Shoot:";
    button_sound=12-1;
    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(145,y_position,"","",0,SPECIAL_INFO_KEY_3_SHOOT);
    vector_windows[vector_windows.size()-1].create_button(15,y_position,"","                                                   ",&button_event_key_3_shoot,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Change Shot:";
    button_sound=10-1;
    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(145,y_position,"","",0,SPECIAL_INFO_KEY_3_CHANGE_SHOT);
    vector_windows[vector_windows.size()-1].create_button(15,y_position,"","                                                   ",&button_event_key_3_change_shot,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Restore Defaults\n   (Keyboard)";
    button_sound=8-1;
    y_position+=185;
    vector_windows[vector_windows.size()-1].create_button(10,y_position,"Set controls to some\nnice defaults for\nthe keyboard.",msg,&button_event_default_keys_3_keyboard,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Restore Defaults (Controller 0)";
    button_sound=11-1;
    y_position=210;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"Set controls to some\nnice defaults for\nthe 360 controller.",msg,&button_event_default_keys_joystick_0,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE,2);

    msg="Restore Defaults Controller 1)";
    button_sound=9-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"Set controls to some\nnice defaults for\nthe 360 controller.",msg,&button_event_default_keys_joystick_1,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE,2);

    msg="Restore Defaults (Controller 2)";
    button_sound=8-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"Set controls to some\nnice defaults for\nthe 360 controller.",msg,&button_event_default_keys_joystick_2,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE,2);

    msg="Restore Defaults (Controller 3)";
    button_sound=6-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"Set controls to some\nnice defaults for\nthe 360 controller.",msg,&button_event_default_keys_joystick_3,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE,2);

    msg="Back to Options";
    button_sound=12-1;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,window_height-back_button_space,"",msg,&button_event_open_window_options,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    //********//
    // About: //
    //********//

    window_width=800;
    window_height=600;
    vector_windows.push_back(Window((main_window.SCREEN_WIDTH-window_width)/2,(main_window.SCREEN_HEIGHT-window_height)/2,window_width,window_height,"About",NO_SPECIAL_INPUT));

    msg="Hubert's Island Adventure:";
    y_position=40;
    vector_windows[vector_windows.size()-1].create_information((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,0,NO_SPECIAL_INFO);

    msg="Mouse o' War";
    y_position+=20;
    vector_windows[vector_windows.size()-1].create_information((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,0,NO_SPECIAL_INFO);

    y_position+=30;
    vector_windows[vector_windows.size()-1].create_information(300,y_position,"","",0,SPECIAL_INFO_VERSION);

    y_position+=30;
    vector_windows[vector_windows.size()-1].create_information(276,y_position,"","",0,SPECIAL_INFO_BUILD_DATE);

    msg="About Cheese and Bacon Games";
    y_position+=40;
    vector_windows[vector_windows.size()-1].create_information((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,0,NO_SPECIAL_INFO);

    vector_windows[vector_windows.size()-1].create_information(window_width-image.logo_cheese_and_bacon.w-15,45,"","",&image.logo_cheese_and_bacon,NO_SPECIAL_INFO);

    msg="What are we about? Cheese, bacon, or games? We believe that a company\ncan be about all three. Here are some more things we believe in:\n\n";
    msg+="We believe in gameplay over snazzy graphics. We work to create games\nthat will be enjoyable for years to come, not just until the next cool\ntechnology comes out. That is not to say that we dislike fancy\ngraphics, awesome music, etc., but gameplay is our top priority.\n\n";
    msg+="We believe that a game doesn't have enough polish until you can see\nyour reflection in it.\n\n";
    msg+="We believe in creating games that are fun for the whole family!\n\n";
    y_position+=40;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"",msg,0,NO_SPECIAL_INFO);

    msg="Back to Main Menu";
    button_sound=12-1;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,window_height-back_button_space,"",msg,&button_event_open_window_main_menu,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    //**********//
    // Credits: //
    //**********//

    window_width=800;
    window_height=600;
    vector_windows.push_back(Window((main_window.SCREEN_WIDTH-window_width)/2,(main_window.SCREEN_HEIGHT-window_height)/2,window_width,window_height,"Credits",NO_SPECIAL_INPUT));

    msg="Cheese and Bacon Games, LLC";
    vector_windows[vector_windows.size()-1].create_information((window_width-msg.length()*font.spacing_x)/2,40,"",msg,0,NO_SPECIAL_INFO);

    msg="Design:";
    vector_windows[vector_windows.size()-1].create_information(15,90,"",msg,0,NO_SPECIAL_INFO);

    msg="Kevin Wells";
    vector_windows[vector_windows.size()-1].create_information(30,115,"",msg,0,NO_SPECIAL_INFO);

    msg="Programming:";
    vector_windows[vector_windows.size()-1].create_information(15,165,"",msg,0,NO_SPECIAL_INFO);

    msg="Kevin Wells";
    vector_windows[vector_windows.size()-1].create_information(30,190,"",msg,0,NO_SPECIAL_INFO);

    msg="Artwork:";
    vector_windows[vector_windows.size()-1].create_information(15,240,"",msg,0,NO_SPECIAL_INFO);

    msg="Kevin Wells";
    vector_windows[vector_windows.size()-1].create_information(30,265,"",msg,0,NO_SPECIAL_INFO);

    msg="Sound:";
    vector_windows[vector_windows.size()-1].create_information(15,315,"",msg,0,NO_SPECIAL_INFO);

    msg="Kevin Wells";
    vector_windows[vector_windows.size()-1].create_information(30,340,"",msg,0,NO_SPECIAL_INFO);

    msg="Music:";
    vector_windows[vector_windows.size()-1].create_information(15,390,"",msg,0,NO_SPECIAL_INFO);

    msg="Kevin Wells";
    vector_windows[vector_windows.size()-1].create_information(30,415,"",msg,0,NO_SPECIAL_INFO);

    msg="Story:";
    vector_windows[vector_windows.size()-1].create_information(15,465,"",msg,0,NO_SPECIAL_INFO);

    msg="Aneissa Wells";
    vector_windows[vector_windows.size()-1].create_information(30,490,"",msg,0,NO_SPECIAL_INFO);

    msg="Kevin Wells";
    vector_windows[vector_windows.size()-1].create_information(30,515,"",msg,0,NO_SPECIAL_INFO);

    msg="Additional thanks to:";
    vector_windows[vector_windows.size()-1].create_information(275,90,"",msg,0,NO_SPECIAL_INFO);

    msg="Sam Lantinga and the SDL contributors";
    y_position=115;
    vector_windows[vector_windows.size()-1].create_information(290,y_position,"",msg,0,NO_SPECIAL_INFO);

    msg="The GLEW team";
    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(290,y_position,"",msg,0,NO_SPECIAL_INFO);

    msg="The Boost team";
    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(290,y_position,"",msg,0,NO_SPECIAL_INFO);

    msg="The libfov team";
    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(290,y_position,"",msg,0,NO_SPECIAL_INFO);

    msg="The free software community";
    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(290,y_position,"",msg,0,NO_SPECIAL_INFO);

    msg="Tom Hall, architect of the\n    Standard Galactic Alphabet";
    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(290,y_position,"",msg,0,NO_SPECIAL_INFO);

    msg="Special thanks to:";
    y_position+=75;
    vector_windows[vector_windows.size()-1].create_information(275,y_position,"",msg,0,NO_SPECIAL_INFO);

    msg="Aneissa Wells";
    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(290,y_position,"",msg,0,NO_SPECIAL_INFO);

    msg="All of the awesome people\nwho helped test the game";
    y_position+=50;
    vector_windows[vector_windows.size()-1].create_information(290,y_position,"",msg,0,NO_SPECIAL_INFO);

    msg="Back to Main Menu";
    button_sound=12-1;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,window_height-back_button_space,"",msg,&button_event_open_window_main_menu,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    //*********//
    // Help 1: //
    //*********//

    short name_color=return_gui_color(holiday,5);

    window_width=800;
    window_height=600;
    vector_windows.push_back(Window((main_window.SCREEN_WIDTH-window_width)/2,(main_window.SCREEN_HEIGHT-window_height)/2,window_width,window_height,"Help",NO_SPECIAL_INPUT));

    msg="Playable Characters";
    y_position=30;
    vector_windows[vector_windows.size()-1].create_information((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,0,NO_SPECIAL_INFO);

    msg="The Change Character key changes which character you will spawn as.";
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_information((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,0,NO_SPECIAL_INFO);

    y_position=100;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"","",&image.sprite_sheet_player,NO_SPECIAL_INFO,&sprites_player_idle_right[0]);
    msg="Hubert,";
    vector_windows[vector_windows.size()-1].create_information(20+sprites_player_idle_right[0].w,y_position-5,"",msg,0,NO_SPECIAL_INFO,0,name_color);
    msg="        being the eponymous hero of this game,\nis the default character.";
    vector_windows[vector_windows.size()-1].create_information(20+sprites_player_idle_right[0].w,y_position-5,"",msg,0,NO_SPECIAL_INFO);

    y_position+=60;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"","",&image.sprite_sheet_character_slime_o,NO_SPECIAL_INFO,&sprites_player_idle_right[0]);
    msg="Slime-O";
    vector_windows[vector_windows.size()-1].create_information(20+sprites_player_idle_right[0].w,y_position-5,"",msg,0,NO_SPECIAL_INFO,0,name_color);
    msg="        is heavier and slower than Hubert.\nHe is an excellent choice if you are having\na hard time controlling Hubert.";
    vector_windows[vector_windows.size()-1].create_information(20+sprites_player_idle_right[0].w,y_position-5,"",msg,0,NO_SPECIAL_INFO);

    y_position+=90;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"","",&image.sprite_sheet_character_sketch,NO_SPECIAL_INFO,&sprites_player_idle_right[0]);
    msg="Sketch";
    vector_windows[vector_windows.size()-1].create_information(20+sprites_player_idle_right[0].w,y_position-5,"",msg,0,NO_SPECIAL_INFO,0,name_color);
    msg="       is the polar opposite of Slime-O.\nHe is lighter and faster than Hubert.";
    vector_windows[vector_windows.size()-1].create_information(20+sprites_player_idle_right[0].w,y_position-5,"",msg,0,NO_SPECIAL_INFO);

    y_position+=60;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"","",&image.sprite_sheet_character_penny,NO_SPECIAL_INFO,&sprites_player_idle_right[0]);
    msg="Penny";
    vector_windows[vector_windows.size()-1].create_information(20+sprites_player_idle_right[0].w,y_position-5,"",msg,0,NO_SPECIAL_INFO,0,name_color);
    msg="      is fast and agile.\nShe is the quickest of the group, but she takes some\nexperience to control properly.";
    vector_windows[vector_windows.size()-1].create_information(20+sprites_player_idle_right[0].w,y_position-5,"",msg,0,NO_SPECIAL_INFO);

    msg="< Page 8";
    button_sound=10-1;
    vector_windows[vector_windows.size()-1].create_button(15,window_height-back_button_space,"",msg,&button_event_open_window_help_8,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Back to Main Menu";
    button_sound=12-1;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,window_height-back_button_space,"",msg,&button_event_open_window_main_menu,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Page 2 >";
    button_sound=11-1;
    vector_windows[vector_windows.size()-1].create_button(window_width-20-msg.length()*font.spacing_x,window_height-back_button_space,"",msg,&button_event_open_window_help_2,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    //*********//
    // Help 2: //
    //*********//

    window_width=800;
    window_height=600;
    vector_windows.push_back(Window((main_window.SCREEN_WIDTH-window_width)/2,(main_window.SCREEN_HEIGHT-window_height)/2,window_width,window_height,"Help",NO_SPECIAL_INPUT));

    msg="Items";
    y_position=30;
    vector_windows[vector_windows.size()-1].create_information((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,0,NO_SPECIAL_INFO);

    msg="Here is a brief explanation of the game's most common items:";
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_information((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,0,NO_SPECIAL_INFO);

    y_position=100;
    SDL_Rect* ptr_item_rect=&sprites_item_leaf[0];
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"","",&image.sprite_sheet_items,NO_SPECIAL_INFO,ptr_item_rect);
    msg="Leaf.";
    vector_windows[vector_windows.size()-1].create_information(20+ptr_item_rect->w,y_position-5,"",msg,0,NO_SPECIAL_INFO,0,name_color);
    msg="\nLeaves are scattered all across the island.\nWhen Hubert collects them, they stay gone from the level.";
    vector_windows[vector_windows.size()-1].create_information(20+ptr_item_rect->w,y_position-5,"",msg,0,NO_SPECIAL_INFO);

    y_position+=80;
    ptr_item_rect=&sprites_item_cheese[0];
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"","",&image.sprite_sheet_items,NO_SPECIAL_INFO,ptr_item_rect);
    msg="Cheese.";
    vector_windows[vector_windows.size()-1].create_information(20+ptr_item_rect->w,y_position-5,"",msg,0,NO_SPECIAL_INFO,0,name_color);
    msg="\nCheese is rarer than leaves.\nLike leaves, cheese stays gone once collected.";
    vector_windows[vector_windows.size()-1].create_information(20+ptr_item_rect->w,y_position-5,"",msg,0,NO_SPECIAL_INFO);

    y_position+=80;
    ptr_item_rect=&sprites_item_ammo[0];
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"","",&image.sprite_sheet_items,NO_SPECIAL_INFO,ptr_item_rect);
    msg="Ammo.";
    vector_windows[vector_windows.size()-1].create_information(20+ptr_item_rect->w,y_position-5,"",msg,0,NO_SPECIAL_INFO,0,name_color);
    msg="\nGives Hubert some ammo for his stunner.";
    vector_windows[vector_windows.size()-1].create_information(20+ptr_item_rect->w,y_position-5,"",msg,0,NO_SPECIAL_INFO);

    y_position+=80;
    ptr_item_rect=&sprites_item_ammo_barrel[0];
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"","",&image.sprite_sheet_items,NO_SPECIAL_INFO,ptr_item_rect);
    msg="Ammo Barrel.";
    vector_windows[vector_windows.size()-1].create_information(20+ptr_item_rect->w,y_position-5,"",msg,0,NO_SPECIAL_INFO,0,name_color);
    msg="\nGives Hubert a lot of ammo for his stunner.\nLike leaves and cheese, stays gone once collected.";
    vector_windows[vector_windows.size()-1].create_information(20+ptr_item_rect->w,y_position-5,"",msg,0,NO_SPECIAL_INFO);

    y_position+=80;
    ptr_item_rect=&sprites_item_j_balloon[0];
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"","",&image.sprite_sheet_items,NO_SPECIAL_INFO,ptr_item_rect);
    msg="J-Balloon.";
    vector_windows[vector_windows.size()-1].create_information(20+ptr_item_rect->w,y_position-5,"",msg,0,NO_SPECIAL_INFO,0,name_color);
    msg="\nAllows Hubert to jump repeatedly in the air for a short time.";
    vector_windows[vector_windows.size()-1].create_information(20+ptr_item_rect->w,y_position-5,"",msg,0,NO_SPECIAL_INFO);

    y_position+=60;
    ptr_item_rect=&sprites_item_candy[0];
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"","",&image.sprite_sheet_items,NO_SPECIAL_INFO,ptr_item_rect);
    msg="Candy.";
    vector_windows[vector_windows.size()-1].create_information(20+ptr_item_rect->w,y_position-5,"",msg,0,NO_SPECIAL_INFO,0,name_color);
    msg="\nFound on enemies, at the end of levels, and perhaps elsewhere, too.\nIncreases Hubert's score.";
    vector_windows[vector_windows.size()-1].create_information(20+ptr_item_rect->w,y_position-5,"",msg,0,NO_SPECIAL_INFO);

    msg="< Page 1";
    button_sound=10-1;
    vector_windows[vector_windows.size()-1].create_button(15,window_height-back_button_space,"",msg,&button_event_open_window_help,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Back to Main Menu";
    button_sound=12-1;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,window_height-back_button_space,"",msg,&button_event_open_window_main_menu,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Page 3 >";
    button_sound=11-1;
    vector_windows[vector_windows.size()-1].create_button(window_width-20-msg.length()*font.spacing_x,window_height-back_button_space,"",msg,&button_event_open_window_help_3,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    //*********//
    // Help 3: //
    //*********//

    window_width=800;
    window_height=600;
    vector_windows.push_back(Window((main_window.SCREEN_WIDTH-window_width)/2,(main_window.SCREEN_HEIGHT-window_height)/2,window_width,window_height,"Help",NO_SPECIAL_INPUT));

    msg="Cast of Characters";
    y_position=30;
    vector_windows[vector_windows.size()-1].create_information((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,0,NO_SPECIAL_INFO);

    msg="Here is a list of the most common creatures Hubert will encounter:";
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_information((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,0,NO_SPECIAL_INFO);

    y_position=100;
    x_position=15;
    for(int type=NPC_SLIME,i=0;type<=NPC_ARCHER;type++,i++){
        if(type!=NPC_REPLICATOR && type!=NPC_LASER_SPINNER && type!=NPC_PENGUIN_HARMLESS &&
           type!=NPC_FISH_ON_ICE_HARMLESS){
            SDL_Rect* ptr_rect=NULL;
            if(SPRITES_NPC[type].walk_right.size()>0){
                ptr_rect=&SPRITES_NPC[type].walk_right[0];
            }
            else if(SPRITES_NPC[type].fly_right.size()>0){
                ptr_rect=&SPRITES_NPC[type].fly_right[0];
            }
            else if(SPRITES_NPC[type].swim_right.size()>0){
                ptr_rect=&SPRITES_NPC[type].swim_right[0];
            }
            vector_windows[vector_windows.size()-1].create_information(x_position,y_position,"","",return_npc_image(type),NO_SPECIAL_INFO,ptr_rect);
            msg=return_npc_name(type)+".";
            vector_windows[vector_windows.size()-1].create_information(x_position+5+ptr_rect->w,y_position-5,"",msg,0,NO_SPECIAL_INFO,0,name_color);
            msg="\n"+return_npc_description(type);
            vector_windows[vector_windows.size()-1].create_information(x_position+5+ptr_rect->w,y_position-5,"",msg,0,NO_SPECIAL_INFO);
            if(i==4){
                y_position=100;
                x_position=400;
            }
            else{
                y_position+=font.spacing_y*4;
            }
        }
        else{
            i--;
        }
    }

    msg="< Page 2";
    button_sound=10-1;
    vector_windows[vector_windows.size()-1].create_button(15,window_height-back_button_space,"",msg,&button_event_open_window_help_2,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Back to Main Menu";
    button_sound=12-1;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,window_height-back_button_space,"",msg,&button_event_open_window_main_menu,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Page 4 >";
    button_sound=11-1;
    vector_windows[vector_windows.size()-1].create_button(window_width-20-msg.length()*font.spacing_x,window_height-back_button_space,"",msg,&button_event_open_window_help_4,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    //*********//
    // Help 4: //
    //*********//

    window_width=800;
    window_height=600;
    vector_windows.push_back(Window((main_window.SCREEN_WIDTH-window_width)/2,(main_window.SCREEN_HEIGHT-window_height)/2,window_width,window_height,"Help",NO_SPECIAL_INPUT));

    y_position=40;
    x_position=15;
    for(int type=NPC_SCARECROW,i=0;type<=NPC_SNOWMAN;type++,i++){
        if(type!=NPC_SPIDER_WALKING){
            SDL_Rect* ptr_rect=NULL;
            if(SPRITES_NPC[type].walk_right.size()>0){
                ptr_rect=&SPRITES_NPC[type].walk_right[0];
            }
            else if(SPRITES_NPC[type].fly_right.size()>0){
                ptr_rect=&SPRITES_NPC[type].fly_right[0];
            }
            else if(SPRITES_NPC[type].swim_right.size()>0){
                ptr_rect=&SPRITES_NPC[type].swim_right[0];
            }
            vector_windows[vector_windows.size()-1].create_information(x_position,y_position,"","",return_npc_image(type),NO_SPECIAL_INFO,ptr_rect);
            msg=return_npc_name(type)+".";
            vector_windows[vector_windows.size()-1].create_information(x_position+5+ptr_rect->w,y_position-5,"",msg,0,NO_SPECIAL_INFO,0,name_color);
            msg="\n"+return_npc_description(type);
            vector_windows[vector_windows.size()-1].create_information(x_position+5+ptr_rect->w,y_position-5,"",msg,0,NO_SPECIAL_INFO);
            if(i==4){
                y_position=40;
                x_position=400;
            }
            else{
                y_position+=font.spacing_y*4;
            }
        }
        else{
            i--;
        }
    }

    msg="< Page 3";
    button_sound=10-1;
    vector_windows[vector_windows.size()-1].create_button(15,window_height-back_button_space,"",msg,&button_event_open_window_help_3,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Back to Main Menu";
    button_sound=12-1;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,window_height-back_button_space,"",msg,&button_event_open_window_main_menu,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Page 5 >";
    button_sound=11-1;
    vector_windows[vector_windows.size()-1].create_button(window_width-20-msg.length()*font.spacing_x,window_height-back_button_space,"",msg,&button_event_open_window_help_5,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    //*********//
    // Help 5: //
    //*********//

    window_width=800;
    window_height=600;
    vector_windows.push_back(Window((main_window.SCREEN_WIDTH-window_width)/2,(main_window.SCREEN_HEIGHT-window_height)/2,window_width,window_height,"Help",NO_SPECIAL_INPUT));

    y_position=40;
    x_position=15;
    for(int type=NPC_FIREBALL,i=0;type<=NPC_STARFISH;type++,i++){
        if(type!=NPC_FIREBALL_SHOOTER_DOWN && type!=NPC_FIREBALL_SHOOTER_LEFT &&
           type!=NPC_FIREBALL_SHOOTER_RIGHT && type!=NPC_FIREBALL_SHOOTER_UP && type!=NPC_SALLY &&
           type!=NPC_BOUNCING_COW && type!=NPC_BOUNCING_BALL && type!=NPC_SPHINX_LEFT && type!=NPC_SPHINX_RIGHT){
            SDL_Rect* ptr_rect=NULL;
            if(SPRITES_NPC[type].walk_right.size()>0){
                ptr_rect=&SPRITES_NPC[type].walk_right[0];
            }
            else if(SPRITES_NPC[type].fly_right.size()>0){
                ptr_rect=&SPRITES_NPC[type].fly_right[0];
            }
            else if(SPRITES_NPC[type].swim_right.size()>0){
                ptr_rect=&SPRITES_NPC[type].swim_right[0];
            }
            vector_windows[vector_windows.size()-1].create_information(x_position,y_position,"","",return_npc_image(type),NO_SPECIAL_INFO,ptr_rect);
            msg=return_npc_name(type)+".";
            vector_windows[vector_windows.size()-1].create_information(x_position+5+ptr_rect->w,y_position-5,"",msg,0,NO_SPECIAL_INFO,0,name_color);
            msg="\n"+return_npc_description(type);
            vector_windows[vector_windows.size()-1].create_information(x_position+5+ptr_rect->w,y_position-5,"",msg,0,NO_SPECIAL_INFO);
            if(i==4){
                y_position=40;
                x_position=400;
            }
            else{
                y_position+=font.spacing_y*4;
            }
        }
        else{
            i--;
        }
    }

    msg="< Page 4";
    button_sound=10-1;
    vector_windows[vector_windows.size()-1].create_button(15,window_height-back_button_space,"",msg,&button_event_open_window_help_4,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Back to Main Menu";
    button_sound=12-1;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,window_height-back_button_space,"",msg,&button_event_open_window_main_menu,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Page 6 >";
    button_sound=11-1;
    vector_windows[vector_windows.size()-1].create_button(window_width-20-msg.length()*font.spacing_x,window_height-back_button_space,"",msg,&button_event_open_window_help_6,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    //*********//
    // Help 6: //
    //*********//

    window_width=800;
    window_height=600;
    vector_windows.push_back(Window((main_window.SCREEN_WIDTH-window_width)/2,(main_window.SCREEN_HEIGHT-window_height)/2,window_width,window_height,"Help",NO_SPECIAL_INPUT));

    msg="Bosses";
    y_position=30;
    vector_windows[vector_windows.size()-1].create_information((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,0,NO_SPECIAL_INFO);

    msg="While on his quest, Hubert will come across";
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_information((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,0,NO_SPECIAL_INFO);

    msg="several exceptionally tough enemies:";
    y_position+=font.spacing_y;
    vector_windows[vector_windows.size()-1].create_information((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,0,NO_SPECIAL_INFO);

    y_position=160;
    short boss_type=NPC_BOSS_CASTLE;
    SDL_Rect* ptr_boss_rect=&SPRITES_NPC[boss_type].fly_right[0];
    vector_windows[vector_windows.size()-1].create_information(200,y_position,"","",return_npc_image(boss_type),NO_SPECIAL_INFO,ptr_boss_rect);

    y_position=160;
    boss_type=NPC_BOSS_HAUNTED_HOUSE;
    ptr_boss_rect=&SPRITES_NPC[boss_type].fly_left[0];
    vector_windows[vector_windows.size()-1].create_information(window_width-ptr_boss_rect->w-200,y_position,"","",return_npc_image(boss_type),NO_SPECIAL_INFO,ptr_boss_rect);

    y_position=360;
    boss_type=NPC_BOSS_VOLCANO;
    ptr_boss_rect=&SPRITES_NPC[boss_type].walk_right[0];
    vector_windows[vector_windows.size()-1].create_information(200,y_position,"","",return_npc_image(boss_type),NO_SPECIAL_INFO,ptr_boss_rect);

    y_position=360;
    boss_type=NPC_BOSS_PYRAMID;
    ptr_boss_rect=&SPRITES_NPC[boss_type].fly_left[0];
    vector_windows[vector_windows.size()-1].create_information(window_width-ptr_boss_rect->w-200,y_position,"","",return_npc_image(boss_type),NO_SPECIAL_INFO,ptr_boss_rect);

    y_position=255;
    boss_type=NPC_BOSS_LIGHTHOUSE;
    ptr_boss_rect=&SPRITES_NPC[boss_type].fly_right[0];
    vector_windows[vector_windows.size()-1].create_information(355,y_position,"","",return_npc_image(boss_type),NO_SPECIAL_INFO,ptr_boss_rect);

    msg="There are other items and creatures out there! Explore, and have fun!";
    y_position=window_height-back_button_space-80;
    vector_windows[vector_windows.size()-1].create_information((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,0,NO_SPECIAL_INFO);

    msg="That's all! Now go play the game!";
    y_position=window_height-back_button_space-40;
    vector_windows[vector_windows.size()-1].create_information((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,0,NO_SPECIAL_INFO);

    msg="< Page 5";
    button_sound=10-1;
    vector_windows[vector_windows.size()-1].create_button(15,window_height-back_button_space,"",msg,&button_event_open_window_help_5,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Back to Main Menu";
    button_sound=12-1;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,window_height-back_button_space,"",msg,&button_event_open_window_main_menu,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Page 7 >";
    button_sound=11-1;
    vector_windows[vector_windows.size()-1].create_button(window_width-20-msg.length()*font.spacing_x,window_height-back_button_space,"",msg,&button_event_open_window_help_7,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    //*********//
    // Help 7: //
    //*********//

    short alt_color=return_gui_color(holiday,5);

    window_width=800;
    window_height=600;
    vector_windows.push_back(Window((main_window.SCREEN_WIDTH-window_width)/2,(main_window.SCREEN_HEIGHT-window_height)/2,window_width,window_height,"Help",NO_SPECIAL_INPUT));

    msg="Hints";
    y_position=30;
    vector_windows[vector_windows.size()-1].create_information((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,0,NO_SPECIAL_INFO);

    msg="If you seek a key that is Gray,";
    y_position+=40;
    vector_windows[vector_windows.size()-1].create_information((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,0,NO_SPECIAL_INFO);
    msg="Visit the Lowlands on an unfrozen day.";
    y_position+=font.spacing_y;
    vector_windows[vector_windows.size()-1].create_information((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,0,NO_SPECIAL_INFO);

    msg="In the Accursed Acreage you will see,";
    y_position+=40;
    vector_windows[vector_windows.size()-1].create_information((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,0,NO_SPECIAL_INFO,NULL,alt_color);
    msg="The hidden location of the Brown key.";
    y_position+=font.spacing_y;
    vector_windows[vector_windows.size()-1].create_information((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,0,NO_SPECIAL_INFO,NULL,alt_color);

    msg="If you seek a key that is Yellow,";
    y_position+=40;
    vector_windows[vector_windows.size()-1].create_information((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,0,NO_SPECIAL_INFO);
    msg="A dried up Desert'll make you a glad fellow.";
    y_position+=font.spacing_y;
    vector_windows[vector_windows.size()-1].create_information((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,0,NO_SPECIAL_INFO);

    msg="If the Mammoth Mountain you do climb,";
    y_position+=40;
    vector_windows[vector_windows.size()-1].create_information((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,0,NO_SPECIAL_INFO,NULL,alt_color);
    msg="A key that is Red you will find.";
    y_position+=font.spacing_y;
    vector_windows[vector_windows.size()-1].create_information((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,0,NO_SPECIAL_INFO,NULL,alt_color);

    msg="Hubert is such a wonderful guy,";
    y_position+=40;
    vector_windows[vector_windows.size()-1].create_information((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,0,NO_SPECIAL_INFO);
    msg="The Pink key is found in his House way up high.";
    y_position+=font.spacing_y;
    vector_windows[vector_windows.size()-1].create_information((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,0,NO_SPECIAL_INFO);

    msg="If what you are seeking is a Blue key,";
    y_position+=40;
    vector_windows[vector_windows.size()-1].create_information((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,0,NO_SPECIAL_INFO,NULL,alt_color);
    msg="Visit the Lowlands when they are melty.";
    y_position+=font.spacing_y;
    vector_windows[vector_windows.size()-1].create_information((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,0,NO_SPECIAL_INFO,NULL,alt_color);

    msg="The Cyan key is surrounded by doom,";
    y_position+=40;
    vector_windows[vector_windows.size()-1].create_information((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,0,NO_SPECIAL_INFO);
    msg="Located within a Treacherous Tomb.";
    y_position+=font.spacing_y;
    vector_windows[vector_windows.size()-1].create_information((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,0,NO_SPECIAL_INFO);

    msg="< Page 6";
    button_sound=10-1;
    vector_windows[vector_windows.size()-1].create_button(15,window_height-back_button_space,"",msg,&button_event_open_window_help_6,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Back to Main Menu";
    button_sound=12-1;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,window_height-back_button_space,"",msg,&button_event_open_window_main_menu,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Page 8 >";
    button_sound=11-1;
    vector_windows[vector_windows.size()-1].create_button(window_width-20-msg.length()*font.spacing_x,window_height-back_button_space,"",msg,&button_event_open_window_help_8,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    //*********//
    // Help 8: //
    //*********//

    window_width=800;
    window_height=600;
    vector_windows.push_back(Window((main_window.SCREEN_WIDTH-window_width)/2,(main_window.SCREEN_HEIGHT-window_height)/2,window_width,window_height,"Help",NO_SPECIAL_INPUT));

    msg="Hints";
    y_position=30;
    vector_windows[vector_windows.size()-1].create_information((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,0,NO_SPECIAL_INFO);

    msg="If you want the Orange key,";
    y_position+=40;
    vector_windows[vector_windows.size()-1].create_information((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,0,NO_SPECIAL_INFO);
    msg="Enter the Volcano, and then you will see.";
    y_position+=font.spacing_y;
    vector_windows[vector_windows.size()-1].create_information((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,0,NO_SPECIAL_INFO);

    msg="If it was the Green key you wanted,";
    y_position+=40;
    vector_windows[vector_windows.size()-1].create_information((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,0,NO_SPECIAL_INFO,NULL,alt_color);
    msg="You might check out the Halls that are Haunted.";
    y_position+=font.spacing_y;
    vector_windows[vector_windows.size()-1].create_information((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,0,NO_SPECIAL_INFO,NULL,alt_color);

    msg="The Purple key is found up high,";
    y_position+=40;
    vector_windows[vector_windows.size()-1].create_information((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,0,NO_SPECIAL_INFO);
    msg="In a Lighthouse, near the sky.";
    y_position+=font.spacing_y;
    vector_windows[vector_windows.size()-1].create_information((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,0,NO_SPECIAL_INFO);

    msg="A dry, dry Desert, shifting sands;";
    y_position+=40;
    vector_windows[vector_windows.size()-1].create_information((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,0,NO_SPECIAL_INFO,NULL,alt_color);
    msg="Here a Black key will enter your hands.";
    y_position+=font.spacing_y;
    vector_windows[vector_windows.size()-1].create_information((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,0,NO_SPECIAL_INFO,NULL,alt_color);

    msg="Up in the clouds, in the Timeworn Retreat,";
    y_position+=40;
    vector_windows[vector_windows.size()-1].create_information((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,0,NO_SPECIAL_INFO);
    msg="A Towel is waiting, what a nice treat!";
    y_position+=font.spacing_y;
    vector_windows[vector_windows.size()-1].create_information((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,0,NO_SPECIAL_INFO);

    msg="Beyond the Keep, a Faith-testing Moat,";
    y_position+=40;
    vector_windows[vector_windows.size()-1].create_information((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,0,NO_SPECIAL_INFO,NULL,alt_color);
    msg="Take the plunge, and swim like a boat!";
    y_position+=font.spacing_y;
    vector_windows[vector_windows.size()-1].create_information((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,0,NO_SPECIAL_INFO,NULL,alt_color);

    msg="< Page 7";
    button_sound=10-1;
    vector_windows[vector_windows.size()-1].create_button(15,window_height-back_button_space,"",msg,&button_event_open_window_help_7,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Back to Main Menu";
    button_sound=12-1;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,window_height-back_button_space,"",msg,&button_event_open_window_main_menu,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Page 1 >";
    button_sound=11-1;
    vector_windows[vector_windows.size()-1].create_button(window_width-20-msg.length()*font.spacing_x,window_height-back_button_space,"",msg,&button_event_open_window_help,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    //*************//
    // Sound Test: //
    //*************//

    window_width=800;
    window_height=600;
    vector_windows.push_back(Window((main_window.SCREEN_WIDTH-window_width)/2,(main_window.SCREEN_HEIGHT-window_height)/2,window_width,window_height,"Sound Test",NO_SPECIAL_INPUT));

    char actual_keys[12];
    actual_keys[11]='q';
    actual_keys[10]='w';
    actual_keys[9]='e';
    actual_keys[8]='r';
    actual_keys[7]='t';
    actual_keys[6]='y';
    actual_keys[5]='u';
    actual_keys[4]='i';
    actual_keys[3]='o';
    actual_keys[2]='p';
    actual_keys[1]='[';
    actual_keys[0]=']';

    for(int i=12,n=0,letter=67;i>0;i--,n++){
        msg="   \n";
        msg+=" ";
        msg+=actual_keys[i-1];
        msg+=" \n   ";
        button_sound=i-1;
        vector_windows[vector_windows.size()-1].create_button(80+n*20+n*3*font.spacing_x,60,"",msg,&button_event_do_nothing,NULL,&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);
        msg=(char)letter;
        vector_windows[vector_windows.size()-1].create_information(80+n*20+n*3*font.spacing_x+font.spacing_x,140,"",msg,0,NO_SPECIAL_INFO);
        if(++letter>71){
            letter=65;
        }
    }

    msg="Back to Options";
    button_sound=12-1;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,window_height-back_button_space,"",msg,&button_event_open_window_options,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    //*******//
    // Demo: //
    //*******//

    window_width=800;
    window_height=600;
    vector_windows.push_back(Window((main_window.SCREEN_WIDTH-window_width)/2,(main_window.SCREEN_HEIGHT-window_height)/2,window_width,window_height,"Get the full version!",NO_SPECIAL_INPUT,&image.loading_screen_main));

    msg="Features";
    y_position=40;
    vector_windows[vector_windows.size()-1].create_information((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,0,NO_SPECIAL_INFO);

    msg="- 2D sidescrolling platformer action!\n- Tight controls and physics!\n- 12 large, colorful, hand-crafted environments to explore!\n- Bonus levels!\n- 4 playable characters!\n- Myriad traps, monsters, and hazards out to destroy you!\n- Wield a stun gun with 2 firing modes to defeat your enemies!\n- Collect keys and other items to advance through the game!\n- 5 bosses to crush!\n- 3 difficulty settings!\n- Thousands of leaves and plenty of cheese to find!\n- Witness multiple endings!\n- Detailed stats on everything you do!\n- 108 awesome achievements to unlock!\n- Ride-able cows featuring Positional Mooing technology!\n- Tons (and tons!) of secret nooks and crannies to discover!\n- Survival Mode, in which you attempt to... survive!\n- Cooperative multiplayer for both Adventure and Survival mode!";
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_information(20,y_position,"",msg,0,NO_SPECIAL_INFO);

    msg="Buy the full game!";
    button_sound=7-1;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,window_height-back_button_space*2,"",msg,&button_event_buy_game,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Quit";
    button_sound=12-1;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,window_height-back_button_space,"",msg,&button_event_quit,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    //*******************//
    // Select Game Mode: //
    //*******************//

    window_width=300;
    window_height=300;
    vector_windows.push_back(Window((main_window.SCREEN_WIDTH-window_width)/2,(main_window.SCREEN_HEIGHT-window_height)/2,window_width,window_height,"Select Game Mode",NO_SPECIAL_INPUT));

    msg="Singleplayer Adventure";
    y_position=40;
    button_sound=7-1;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_start_game_sp_adventure,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Singleplayer Survival";
    y_position+=30;
    button_sound=9-1;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_open_window_setup_sp_survival,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Multiplayer Survival";
    y_position+=30;
    button_sound=7-1;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_mp_prepare_survival,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Multiplayer Adventure";
    y_position+=30;
    button_sound=8-1;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,y_position,"",msg,&button_event_mp_prepare_adventure,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Back to Main Menu";
    button_sound=12-1;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,window_height-back_button_space,"",msg,&button_event_open_window_main_menu,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    //***********//
    // MP Setup: //
    //***********//

    window_width=335;
    window_height=430;
    vector_windows.push_back(Window((main_window.SCREEN_WIDTH-window_width)/2,(main_window.SCREEN_HEIGHT-window_height)/2,window_width,window_height,"Multiplayer Setup",NO_SPECIAL_INPUT));

    msg="Players: 1";
    y_position=40;
    x_position=(window_width-(msg.length()*font.spacing_x))/2;
    vector_windows[vector_windows.size()-1].create_information(x_position,y_position,"",msg,0,SPECIAL_INFO_MP_PLAYER_COUNT);

    msg=">";
    button_sound=7-1;
    vector_windows[vector_windows.size()-1].create_button(x_position+font.spacing_x*11,y_position,"",msg,&button_event_mp_players_up,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="<";
    button_sound=6-1;
    vector_windows[vector_windows.size()-1].create_button(x_position-font.spacing_x*2,y_position,"",msg,&button_event_mp_players_down,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    y_position+=50;
    vector_windows[vector_windows.size()-1].create_information(40,y_position,"","",0,SPECIAL_INFO_MP_PLAYER_0);
    msg=">";
    button_sound=8-1;
    y_position+=3;
    vector_windows[vector_windows.size()-1].create_button(70,y_position,"",msg,&button_event_mp_character_toggle_0,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"","",0,SPECIAL_INFO_MP_PLAYER_TEXT_0);

    y_position+=30;
    vector_windows[vector_windows.size()-1].create_information(40,y_position,"","",0,SPECIAL_INFO_MP_PLAYER_1);
    msg=">";
    button_sound=11-1;
    y_position+=3;
    vector_windows[vector_windows.size()-1].create_button(70,y_position,"",msg,&button_event_mp_character_toggle_1,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"","",0,SPECIAL_INFO_MP_PLAYER_TEXT_1);

    y_position+=30;
    vector_windows[vector_windows.size()-1].create_information(40,y_position,"","",0,SPECIAL_INFO_MP_PLAYER_2);
    msg=">";
    button_sound=5-1;
    y_position+=3;
    vector_windows[vector_windows.size()-1].create_button(70,y_position,"",msg,&button_event_mp_character_toggle_2,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"","",0,SPECIAL_INFO_MP_PLAYER_TEXT_2);

    y_position+=30;
    vector_windows[vector_windows.size()-1].create_information(40,y_position,"","",0,SPECIAL_INFO_MP_PLAYER_3);
    msg=">";
    button_sound=9-1;
    y_position+=3;
    vector_windows[vector_windows.size()-1].create_button(70,y_position,"",msg,&button_event_mp_character_toggle_3,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"","",0,SPECIAL_INFO_MP_PLAYER_TEXT_3);

    y_position=156;
    msg="^";
    button_sound=3-1;
    vector_windows[vector_windows.size()-1].create_button(200,y_position,"",msg,&button_event_mp_ai_toggle_1,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_information(115,y_position,"","",0,SPECIAL_INFO_MP_AI_1);

    y_position+=33;
    msg="^";
    button_sound=1-1;
    vector_windows[vector_windows.size()-1].create_button(200,y_position,"",msg,&button_event_mp_ai_toggle_2,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_information(115,y_position,"","",0,SPECIAL_INFO_MP_AI_2);

    y_position+=33;
    msg="^";
    button_sound=2-1;
    vector_windows[vector_windows.size()-1].create_button(200,y_position,"",msg,&button_event_mp_ai_toggle_3,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_information(115,y_position,"","",0,SPECIAL_INFO_MP_AI_3);

    msg="Start";
    button_sound=11-1;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,window_height-back_button_space*2,"",msg,&button_event_mp_setup_start,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Back to Game Mode Select";
    button_sound=12-1;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,window_height-back_button_space,"",msg,&button_event_open_window_select_game_mode,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);

    //*********************//
    // Show Data Location: //
    //*********************//

    window_width=800;
    window_height=600;
    vector_windows.push_back(Window(0,0,window_width,window_height,"",NO_SPECIAL_INPUT));

    vector_windows[vector_windows.size()-1].create_information(15,40,"","",0,SPECIAL_INFO_SHOW_DATA_LOCATION);

    msg="OK";
    button_sound=12-1;
    vector_windows[vector_windows.size()-1].create_button((window_width-(msg.length()*font.spacing_x))/2,window_height-back_button_space,"",msg,&button_event_open_window_options,&sound_system.button_mouse_over[button_sound],&sound_system.button_event_fire[button_sound],BUTTON_VISIBLE);
}
