/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef world_h
#define world_h

#include <string>
#include <sstream>
#include <vector>

#include "string_stuff.h"
#include "level.h"
#include "font.h"
#include "tile.h"
#include "player.h"
#include "item.h"
#include "npc.h"
#include "game_window.h"
#include "random_number_generator.h"
#include "image.h"
#include "effect_water_splash.h"
#include "effect_water_bubble.h"
#include "effect_score_floater.h"
#include "effect_firework.h"
#include "shot.h"
#include "moving_platform.h"
#include "trigger.h"
#include "door.h"
#include "trap.h"
#include "window.h"
#include "window_inventory.h"
#include "window_map.h"
#include "tooltip.h"
#include "profile.h"
#include "window_manager.h"
#include "enumerations.h"
#include "music.h"
#include "slider.h"
#include "window_message.h"
#include "sign.h"
#include "button.h"
#include "window_level_info.h"
#include "spring.h"
#include "booster.h"
#include "sound.h"
#include "player_mp.h"
#include "spawner.h"
#include "window_setup_survival.h"
#include "window_shop.h"
#include "window_upgrades.h"

extern Game_Window main_window;

extern std::vector<Window> vector_windows;

extern std::vector<Window_Inventory> window_inventory;

extern std::vector<Window_Map> window_map;

extern std::vector<Window_Message> window_message;

extern std::vector<Window_Level_Info> windows_level_info;

extern std::vector<Window_Setup_Survival> window_setup_survival;

extern std::vector<Window_Shop> window_shop;

extern std::vector<Window_Upgrades> window_upgrades;

extern Window_Manager window_manager;

extern std::vector<Button> hud_buttons;

extern std::vector<Slider> sliders;

extern Tooltip tooltip;
extern Tooltip tooltip_levelname;
extern Tooltip tooltip_hint;

extern SDL_Event event;

struct joy_stick{
    SDL_Joystick* joy;
};
//Vector containing all of the joysticks.
extern std::vector<joy_stick> joystick;

//The number of joysticks connected to the system when the joysticks were loaded up.
//I don't know if SDL_NumJoysticks() might change during runtime if the player disconnects one or more joysticks (or connects one or more new ones).
//Thus, it is saved here right after the joysticks are loaded, and this number is used when unloading them in quit().
extern int number_of_joysticks;

extern BitmapFont font;
extern BitmapFont font_sga;
extern BitmapFont font_large;
extern BitmapFont font_large2;

extern Random_Number_Generator rng;

extern short option_save_location;

const double DEFAULT_UPDATE_RATE=60.0;

const int PROCESS_RANGE=2400;
const int BOSS_ACTIVATE_RANGE=500;
const int BOSS_DEACTIVATE_RANGE=2000;
const int CANDY_VACUUM_RANGE=128;
const double CANDY_VACUUM_SPEED=6.0;
const int SPAWN_RANGE=800;

const short AI_NPC_TARGET_COOLDOWN=90;
const short AI_PATH_UPDATE_RATE=30;
const short AI_PATH_GIVEUP=120;
const short AI_DECISION_COOLDOWN_REVIVE=360;
const short AI_PATH_COOLDOWN_FAR=240;
const short AI_PATH_COOLDOWN_MEDIUM=120;

const int AI_PATH_RANGE_SHORT=256;
const int AI_PATH_RANGE_MEDIUM=384;
const int AI_PATH_RANGE_FAR=512;

const int AI_PATH_COST_ORTH=10;
const int AI_PATH_COST_DIAG=14;
const int AI_PATH_COST_SLOPE=6;

const int AI_PATH_COST_TILE_WATER=1;
const int AI_PATH_COST_TILE_ROPE=3;
const int AI_PATH_COST_TILE_NORMAL=9;

const uint64_t STARTING_AMMO=5;

//The number of pixels an object can move before it needs to run handle_events() to avoid clipping through stuff.
const double pixel_safety_x=2.0;
const double pixel_safety_y=2.0;

const int LAST_LEVEL=27;

extern int LIGHTING_TILE_SIZE;

extern short holiday;

//If true, quit_game() does not actually quit, but instead loads the nag screen.
extern bool demo_mode;

extern double UPDATE_RATE;
extern double SKIP_TICKS;

extern std::stringstream ss;
extern std::string msg;

extern Level level;

extern Player player;

extern std::vector<Player_Mp> mp_players;

extern std::vector<std::string> default_shop_upgrades;

extern Profile profile;

extern std::string creating_profile;

extern std::vector<Item> vector_items;
extern std::vector<Npc> vector_npcs;
extern std::vector<Shot> vector_shots;
extern std::vector<Moving_Platform> vector_moving_platforms;
extern std::vector<Trigger> vector_triggers;
extern std::vector<Door> vector_doors;
extern std::vector<Trap> vector_traps;
extern std::vector<Sign> vector_signs;
extern std::vector<Spring> vector_springs;
extern std::vector<Booster> vector_boosters;
extern std::vector<Effect_Water_Splash> vector_effect_water_splash;
extern std::vector<Effect_Water_Bubble> vector_effect_water_bubble;
extern std::vector<Effect_Score_Floater> vector_effect_score_floater;
extern std::vector<Effect_Firework> vector_effect_firework;
extern std::vector<Spawner> vector_spawners;

extern Image image;

extern Music music;

extern Sound sound_system;

void load_world();

void unload_world();

void set_default_shop_upgrades();

void set_change_profile_buttons();

void create_hud_buttons();

void create_windows();

#endif
