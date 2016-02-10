/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef player_h
#define player_h

#include <SDL.h>

#include "timer.h"
#include "sprite_sheets.h"
#include "positional_audio.h"
#include "player_sprites.h"
#include "actor.h"
#include "inventory.h"
#include "input_data.h"
#include "enumerations.h"
#include "tracer.h"
#include "menu_background.h"
#include "overlay.h"
#include "light_data.h"
#include "profile.h"

#include "fov.h"

#include <string>
#include <vector>
#include <sstream>
#include <map>

const unsigned long SURVIVAL_TIME_VERY_SHORT=60;
const unsigned long SURVIVAL_TIME_SHORT=120;
const unsigned long SURVIVAL_TIME_MEDIUM=240;
const unsigned long SURVIVAL_TIME_LONG=480;
const unsigned long SURVIVAL_TIME_VERY_LONG=960;

struct boss_message{
    std::string message_title;
    std::string message;
};

//Incomplete declaration of Npc.
class Npc;

class Player: public Actor{
    private:

    //Current movement state of the look offsets.
    short look_state;

    //Is the player looking around?
    bool LOOKING;

    //The offsets used to tell the camera where we are looking.
    double look_offset_x,look_offset_y;

    //The rate of change for the look offsets.
    short look_speed;

    bool crouching_at_frame_start;

    //Is the player sliding down a slope by crouching?
    bool SLIDING;

    double worldmap_run_speed;

    //The region of the main world map the player is currently in.
    //Only applies if the player is on the main world map.
    short worldmap_region;

    //This timer is used to delay the camera movement sound.
    Timer timer_camera_sound;

    double camera_delta_x;
    double camera_delta_y;

    public:
    //Current movement state of the camera.
    short cam_state;

    //Is the player crouching?
    bool CROUCHING;

    Player();

    void reset();

    void load_data();

    void check_special_items();

    void set_physics();

    void update_character();

    //Pause or unpause the game.
    //If get_pause is true, pause the game.
    //If get_pause is false, unpause the game.
    void toggle_pause(bool get_pause);

    bool mouse_allowed();
    bool keystate(SDL_Scancode button);

    //Prepare to check for input.
    void prepare_for_input();

    //Input here is handled any time the game is in progress, even if it is paused.
    void handle_input_states_during_play();

    //Start or stop crouching.
    void crouch_start();
    void crouch_stop();

    //Get input states.
    //Only processed when the game is unpaused.
    void handle_input_states();

    //Get input events.
    //Always processed. If an event is supposed to only happen when the game is unpaused, in progress, etc.,
    //this will be handled in handle_command_event().
    void handle_input_events();

    //When a command event occurs, it calls this function to actually execute the command.
    void handle_command_event(short command,bool command_is_axis=false);

    void handle_tracers();

    void put_in_bubble();

    //Move the player.
    void move();

    void handle_tiles(int check_x_start,int check_x_end,int check_y_start,int check_y_end);

    void handle_tile_hazard(int int_x,int int_y);

    //Handle events, such as collision checks.
    void handle_events(bool being_pushed_up=false);

    //Handle the player dying.
    void handle_death(double death_x,double death_y,double death_w,double death_h,bool override_invulnerability=false);

    //Animate the sprite.
    void animate();

    //Animate the main menu.
    void update_menu_background();

    //Animate the overlays.
    void animate_overlays();

    //Handle camera movement.
    void set_camera();

    //Update the scrolling background.
    void update_background();

    void update_window_caption(int frame_rate,double ms_per_frame,int logic_frame_rate);

    void render_menu_pretties();

    //Render the background.
    void render_background();

    //Render the foreground.
    void render_foreground();

    //Render the time of day overlay.
    //Returns the opacity of the overlay.
    double render_time_overlay();

    //Render the time of day overlay using tiled lighting.
    void render_time_overlay_fancy();

    //Returns the opacity associated with the current time of day.
    double return_time_opacity();

    //Render the sprite.
    void render(bool mirrored=false);

    //Checks the inventory for a certain type of item.
    //Returns true if the item was detected.
    //Returns false if the item was not detected.
    bool check_inventory(short type);

    //Removes one instance of an item from the inventory.
    void remove_item(short type);

    //Returns the next available inventory slot, if any.
    //If no slot is free, returns -1.
    short next_available_inventory_slot();

    //Returns the name of an inventory item with the passed type.
    std::string name_inventory_item(short item_type);

    //Gain points and let the player know.
    void gain_score(uint64_t score_to_gain,double x_location,double y_location,bool show_floater=true);

    //Lose points and let the player know.
    void lose_score();

    //Returns true if the passed NPC is a main boss.
    //Returns false if it is not.
    bool npc_is_main_boss(short type);

    //Returns a random level with a boss in it.
    int return_random_main_boss_level();

    //Returns the number of bosses either spawned OR defeated.
    int return_main_bosses_unlocked();

    //Returns the number of bosses defeated.
    int return_main_bosses_defeated();

    //Returns the amount an ammo box will give you.
    int return_ammo_box_amount();

    //Returns the amount an ammo barrel will give you.
    int return_ammo_barrel_amount();

    //Returns the amount a piece of candy will give you.
    uint64_t return_candy_score(int score_bonus);

    //Returns the current phase of the moon.
    short return_moon_phase();

    //Returns true if it is night.
    //Returns false if it is day.
    bool is_night();

    //Returns true if it is midnight.
    //Returns false if it is not.
    bool is_midnight();

    //Returns the actual difficulty.
    int return_actual_difficulty();

    //If true, the player is currently on a worldmap.
    //If false, the player is in a level.
    bool on_worldmap();

    //If true, the passed level is a world map.
    //If false, the passed level is not a world map.
    bool is_level_worldmap(short level_to_check);

    void worldmap_handle_input_states();
    void worldmap_move();
    void worldmap_handle_events();
    void worldmap_animate();
    void worldmap_render();

    void end_game();

    //Stops the current in-progress game, if any,
    //and resets the player's variables.
    void stop_game();

    void start_game();

    void beat_level();

    void boss_begin(int get_boss_index);
    void boss_end();

    void play_death_sound(sound_data sound);
    void play_balloonin_sound(sound_data sound);
    void stop_balloonin_sound();

    //Option functions:

    //Toggles the fullscreen option.
    void toggle_fullscreen();

    //Toggles the show fps option.
    void toggle_fps();

    //Toggles the holiday cheer option.
    void toggle_holiday_cheer();

    //Toggles the time of day option.
    void toggle_time_of_day();

    //Toggles the time of day option for levels.
    void toggle_time_of_day_levels();

    //Toggles the difficulty.
    void toggle_difficulty();

    //Toggles the hints option.
    void toggle_hints();

    //Toggles the character.
    void toggle_character();

    //Toggles the hardware cursor.
    void toggle_hardware_cursor();

    //Toggles global mute.
    void toggle_mute_global();

    //Toggles sound effect mute.
    void toggle_mute_sound();

    //Toggles music mute.
    void toggle_mute_music();

    //Increment or decrement the corresponding volume.
    void volume_up_global();
    void volume_down_global();
    void volume_up_sound();
    void volume_down_sound();
    void volume_up_music();
    void volume_down_music();

    //Pass the identifier for the key you want to set.
    //Pass -1 to stop setting any command.
    void begin_setting_command(short command,short player_to_set=-1);

    //Check through all commands to see if the passed input is already bound to something.
    //Returns true if the input is already bound.
    //Returns false if the input is not bound.
    bool check_key(Input_Data checked_key);

    //Handle setting command input events.
    void check_for_command_set();

    //Returns the current value of command_to_set.
    short current_command_to_set();

    //Returns whether or not the passed command is currently being given.
    bool command_state(short command);

    void reset_gui_axis_nav_command(short command);
    void reset_gui_axis_nav();

    //Returns a string with the name of the input bound to the passed command.
    std::string command_bound_input(short command,short player_index);

    //Returns the current value of input_already_bound.
    short return_input_already_bound();

    //Update the lighting data.
    void update_fov_lighting();

    void set_firework_rate(bool game_beaten_message=false);

    void create_firework();

    image_data* return_character_image();
    image_data* return_character_worldmap_image();
    image_data* return_character_box_image();

    sound_data* return_character_sound_footstep();
    sound_data* return_character_sound_footstep2();
    sound_data* return_character_sound_jump();
    sound_data* return_character_sound_start_slide();

    void survival_end_game(bool won);

    void no_game_in_progress();

    //Returns true if the current game mode is multiplayer.
    //Returns false if the current game mode is singleplayer.
    bool game_mode_is_multiplayer();

    //Returns true if the current game mode is adventure.
    //Returns false if the current game mode is adventure.
    bool game_mode_is_adventure();

    //Returns true if all human-controlled players are dead.
    bool all_humans_dead();

    //Returns true if all players are dead or in bubble mode.
    //Returns false if some players are alive.
    bool all_players_dead();

    //Returns true if all players are in a state for ending the level.
    bool all_players_gone();

    //Return the coordinates/dimensions of the camera-focused player.
    double cam_focused_x();
    double cam_focused_y();
    double cam_focused_w();
    double cam_focused_h();

    int cam_focused_index();
    bool cam_focused_dying();

    void update_camera_focus();

    short get_player_color(int i);

    void mp_reset(double new_x,double new_y);

    //Reset the mp players stuff.
    void reset_mp_players();

    void mp_load_level_data();
    void mp_save_level_data();

    void mp_load_level_properties();
    void mp_save_level_properties();

    //Returns the NPC fade time in frames.
    short get_npc_fade();

    std::string get_upgrade_name(std::string upgrade);
    std::string get_upgrade_description(std::string upgrade);
    uint64_t get_upgrade_cost(std::string upgrade);

    void remove_shop_upgrade(std::string upgrade);
    void gain_upgrade(std::string upgrade);
    bool has_upgrade(std::string upgrade);
    bool get_upgrade_state(std::string upgrade);
    void toggle_upgrade_state(std::string upgrade);

    //Prints an image of the currently loaded level.
    void levelshot();

    template<class Number_Type>
    inline std::string num_to_string(Number_Type number){
        std::string message="";

        std::stringstream strstream("");
        strstream.clear();strstream.str("");strstream<<number;message=strstream.str();

        return message;
    }

    template<class Number_Type>
    inline std::string num_to_roman_numeral(Number_Type number){
        std::string message="";

        int num=(int)number;

        if(num>=4000){
            int x=(num-num%4000)/1000;
            message="("+num_to_roman_numeral(x)+")";
            num%=4000;
        }

        const std::string roman[13]={"M","CM","D","CD","C","XC","L","XL","X","IX","V","IV","I"};
        const int decimal[13]={1000,900,500,400,100,90,50,40,10,9,5,4,1};

        for(int i=0;i<13;i++){
            while(num>=decimal[i]){
                num-=decimal[i];
                message+=roman[i];
            }
        }

        return message;
    }

    //The id of a command that a checked input is already bound to.
    //When command_to_set is not -1 (currently being set), the player might
    //try to bind an input that is already bound to some other command.
    //If this happens, that command's id is stored in input_already_bound.
    //The corresponding Information object will use this information to let the player
    //know that this input is already bound to something.
    //This will be -1 whenever it is not being used.
    short input_already_bound;

    //The player index of the already bound input.
    short input_already_bound_player;

    //If command_to_set is -1, no key is being set.
    //If anything other than -1, the player has clicked on a command in the key config window, and the next key hit should be set to that command.
    short command_to_set;

    //The player index of the command being set.
    short command_to_set_player;

    //Is the player currently trying to enter a level on the world map?
    bool worldmap_enter_level;

    //Is the player currently trying to return to the world map?
    bool returning_to_worldmap;

    //The world map the player is currently on.
    short current_worldmap;

    //The world map coordinates of the player.
    double world_x[3];
    double world_y[3];

    //The boss state for each level.
    std::vector<short> bosses;

    //The boss unlock messages.
    std::vector<boss_message> boss_messages;

    //The coordinates and dimensions of the camera.
    double camera_x,camera_y;
    double camera_w,camera_h;

    //The speed at which the camera moves when unstuck.
    short camera_speed;

    //The camera trap.
    double camera_trap_x;
    double camera_trap_y;

    //The maximum number of pixels the trap can move per frame.
    int camera_lag_x;
    int camera_lag_y;

    //Animation frames and frame counters.
    short frame_water;
    short frame_counter_water;
    short frame_powerup;
    short frame_counter_powerup;

    bool balloon_scale_direction_x;
    double balloon_scale_x;
    bool balloon_scale_direction_y;
    double balloon_scale_y;

    //Menu background stuff
    short menu_background_counter_x;
    short menu_background_counter_y;

    short menu_background_counter_max_x;
    short menu_background_counter_max_y;

    short menu_background_x;
    short menu_background_y;

    double menu_speed_x;
    double menu_speed_y;

    //Menu title stuff
    short menu_title_counter;
    bool menu_title_font;

    std::vector<Menu_Background> menu_backgrounds;

    std::vector<Overlay> overlay_snow;
    std::vector<Overlay> overlay_rain;

    //Overlay stuff
    short overlay_snow_counter_x;
    short overlay_snow_counter_max_x;
    short overlay_snow_x;
    double overlay_snow_speed_x;
    double overlay_snow_speed_y;

    short overlay_rain_counter_x;
    short overlay_rain_counter_max_x;
    short overlay_rain_x;
    double overlay_rain_speed_x;
    double overlay_rain_speed_y;

    double opacity_overlay_alarm;
    bool fade_direction_overlay_alarm;

    //Manages tracers.
    Tracer tracer_manager;

    //The player's profile name.
    std::string name;

    //The currently selected shot type.
    short current_shot;

    //Options:
    bool option_fps;
    bool option_dev;
    bool option_touch_controls;
    int option_effect_limit;
    double option_volume_global;
    bool option_mute_global;
    double option_volume_sound;
    bool option_mute_sound;
    double option_volume_music;
    bool option_mute_music;
    bool option_holiday_cheer;
    bool option_time_of_day;
    bool option_time_of_day_levels;
    int option_difficulty;
    bool option_hardware_cursor;
    bool option_hints;
    short option_character;

    bool touch_controls;

    //Global options:
    std::string option_fullscreen_mode;
    int option_screen_width;
    int option_screen_height;
    bool option_fullscreen;
    int option_display_number;
    int option_lighting_tile_size;
    short option_npc_fade_adventure;
    short option_npc_fade_survival;

    bool cheat_jump;
    bool cheat_noclip;

    bool show_tracers;
    bool show_paths;
    bool hide_gui;

    //Inputs.
    std::vector<Input_Data> keys;

    //Player_Mp inputs.
    std::vector< std::vector<Input_Data> > mp_keys;

    std::string gui_axis_nav_last_direction;

    //Stats:
    unsigned long stat_enemies_stunned;
    unsigned long stat_triggers_shot;
    unsigned long stat_checkpoints_activated;
    unsigned long stat_ammo_picked_up;
    unsigned long stat_shots_fired;
    unsigned long stat_deaths_enemies;
    unsigned long stat_deaths_traps;
    unsigned long stat_deaths_drowning;
    unsigned long stat_levels_replayed;
    unsigned long stat_total_jumps;
    unsigned long stat_seconds_playing;
    unsigned long stat_seconds_traveling;
    unsigned long stat_seconds_menus;
    unsigned long stat_levers_pulled;
    unsigned long stat_seconds_riding_cows;
    double stat_farthest_fall;
    unsigned long stat_water_shot;
    unsigned long stat_bosses_defeated;
    unsigned long stat_items_moved;

    //Achievements:
    bool achievements[ACHIEVEMENT_END];

    //Special counts (for achievements):
    unsigned long special_count_shots_this_level;
    unsigned long special_count_kills_this_level;
    //Used to save the height at the beginning of a fall.
    double special_count_height;

    //Stats timers.
    Timer timer_playing;
    Timer timer_traveling;
    Timer timer_menus;
    Timer timer_riding_cows;

    //Achievements timer.
    Timer timer_update_achievements;

    //This timer keeps track of saving most data.
    Timer timer_save_data;

    //The current number of elements in each effects vector for which exists==true.
    int existing_effects_water_bubble;
    int existing_effects_water_splash;
    int existing_effects_score_floater;
    int existing_effects_firework;

    int existing_shots;

    //The level the player is currently in.
    short current_level;

    //The current sub level of the current level, if any.
    short current_sub_level;
    //Should the level data (items, etc.) in this level be loaded/saved?
    bool persistent_level_data;
    //Has the current level been beaten before?
    bool level_beaten;

    //image.unload_images_level() uses this variable to unload the previous level's images from memory.
    short previous_level;
    short previous_sub_level;

    //The last checkpoint reached by the player.
    short current_checkpoint;

    //The player's current number of shots.
    uint64_t ammo;

    //The player's score.
    uint64_t score;

    short main_levels_beaten;

    //If true, the weather machine is on.
    //If false, the weather machine is off.
    bool weather_machine;

    //The total and best times for the current level.
    unsigned long seconds_total_current_level;
    unsigned long seconds_best_current_level;

    //The current and maximum count for items for the player's profile.
    //These numbers refer to the entire game.
    unsigned long leaves,leaves_max;
    unsigned long cheese,cheese_max;

    //The number of items collected in the current level.
    unsigned long current_level_leaves,current_level_cheese;

    std::vector<inventory_item> inventory;

    //The currently dragged item.
    std::vector<dragged_inventory_item> dragged_item;

    //The direction on the x-axis the player moves when dying.
    bool death_direction;

    //The speed the player moves on the x-axis when dying.
    double death_speed;

    //The number of bounces the player has made since dying.
    short death_bounces;

    //The reserved channel for the player's death sound.
    int sound_channel_death;

    //The reserved channel for the player's J-Balloon mode.
    int sound_channel_balloonin;

    //If true, the game is paused.
    //If false, the game is not paused.
    bool pause;

    //If true, a game is currently in progress.
    //If false, the game is on the main menu.
    bool game_in_progress;

    bool night_mode;

    bool deadly_water;

    //Resets every time the player enters the air.
    //Counts down to 0.
    //When non-zero, the player can still jump.
    short counter_jump_mercy;

    //When non-zero, the player can jump infinitely.
    //Works exactly like cheat_jump, except is not a cheat. Is activated by grabbing a J-Balloon.
    short counter_jump_mode;

    //Used to count down to a level's end, after the goal is crossed.
    short counter_level_end;

    //Counts down to 0.
    //When it hits zero, it resets and creates a new tracer.
    short counter_create_tracer;

    //If true, the level's goal has been crossed.
    //If false, it has not.
    bool goal_crossed;

    //If true, we are fading to black.
    //If false, we aren't.
    bool fadeout;

    //The opacity of the fadeout.
    double fade_opacity;

    //When the player crosses a goal, this is set to that goal item's goal_level_to_load.
    int next_level;

    //Used during handle_events().
    //If true, the player is riding a cow this frame.
    //If false, the player is not riding a cow.
    bool riding_cow;

    //If non-zero, the beginning cutscene is playing.
    short game_beginning_cutscene;

    //The currently focused button.
    //Negative numbers mean no button is focused.
    int current_button;

    //Index in the npcs vector for the boss.
    //If -1, no boss battle is in progress.
    //If anything else, a boss battle is in progress.
    int boss_index;

    //A special string used for special fun times!
    std::string main_menu_special;

    short main_menu_special_color;

    //If true, we need to reinitialize everything.
    //If false, we don't.
    bool need_to_reinit;

    //Field of view lighting variables.
    fov_settings_type fov_settings_lighting;

    //The lighting data.
    std::vector< std::vector<Light_Data> > light_data;

    //The player's light source data.
    Light_Source light_source;

    //The number of fireworks to be fired per second.
    short fireworks_rate;

    //Pointer to the current character image.
    image_data* ptr_player_image;

    //Pointer to the current character worldmap image.
    image_data* ptr_player_worldmap_image;

    //Pointers to the current character sounds.
    sound_data* ptr_player_footstep;
    sound_data* ptr_player_footstep2;
    sound_data* ptr_player_jump;
    sound_data* ptr_player_start_slide;
    sound_data* ptr_player_worldmap_footstep;
    sound_data* ptr_player_worldmap_footstep2;

    //If non-zero, this is the amount of pluses in new game plus mode.
    int new_game_plus;

    //The current game mode.
    short game_mode;

    //The current level, not part of the main level files.
    std::string non_number_level;

    //The seconds that must be survived in survival mode before escape becomes possible.
    unsigned long survival_seconds_to_escape;

    //The score within a game of survival.
    uint64_t survival_score;

    //If true, the player can escape in survival mode.
    bool survival_escape;

    //If true, a game of survival is complete.
    bool survival_complete;

    //If true, the player is in bubble mode.
    bool bubble_mode;

    //Movement speed in bubble mode.
    double bubble_move_x;
    double bubble_move_y;

    //The index of the camera-focused player.
    int camera_focused;

    std::vector< std::vector<bool> > mp_level_data_items_collected;
    std::vector< std::vector<bool> > mp_level_data_doors_opened;

    std::vector<Level_Properties> mp_level_properties;

    //If true, a MP Adventure has been started.
    //Used to determine whether the Play button should show the intro story.
    bool mp_adventure_started;

    //We are ending a MP Adventure.
    bool mp_adventure_ending;

    //The amount of players for MP modes.
    int mp_player_count;

    //Characters in MP that are not in use by another player.
    std::vector<short> mp_available_characters;

    //Whether or not each player will be AI controlled in MP.
    std::vector<bool> mp_ai;

    //The character for the player that has been selected for a MP mode.
    short mp_character;

    std::vector<std::string> shop_upgrades;
    std::map<std::string,bool> upgrades;

    bool new_game_plus_notification;
};

#endif
