/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef player_mp_h
#define player_mp_h

#include "actor.h"
#include "input_data.h"
#include "light_data.h"
#include "path.h"
#include "path_calc.h"
#include "tile.h"

#include <vector>

/*
This is the special player object for multiplayer.

Player 1 is always the Player object.
Any subsequent players will be Player_Mp objects in a vector of Player_Mp objects.
*/

class Player_Mp: public Actor{
private:
    bool crouching_at_frame_start;

    //Is the player sliding down a slope by crouching?
    bool SLIDING;

    double worldmap_run_speed;

public:
    //Is the player crouching?
    bool CROUCHING;

    //Animation frames and frame counters.
    short frame_powerup;
    short frame_counter_powerup;

    bool balloon_scale_direction_x;
    double balloon_scale_x;
    bool balloon_scale_direction_y;
    double balloon_scale_y;

    //The currently selected shot type.
    short current_shot;

    //Options:
    short option_character;

    //Inputs.
    std::vector<Input_Data> keys;

    //The player's current number of shots.
    uint64_t ammo;

    //The direction on the x-axis the player moves when dying.
    bool death_direction;

    //The speed the player moves on the x-axis when dying.
    double death_speed;

    short death_bounces;

    //Resets every time the player enters the air.
    //Counts down to 0.
    //When non-zero, the player can still jump.
    short counter_jump_mercy;

    //When non-zero, the player can jump infinitely.
    //Works exactly like cheat_jump, except is not a cheat. Is activated by grabbing a J-Balloon.
    short counter_jump_mode;

    //The player's light source data.
    Light_Source light_source;

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

    //If true, the player is in bubble mode.
    bool bubble_mode;

    //Movement speed in bubble mode.
    double bubble_move_x;
    double bubble_move_y;

    //Virtual AI "keystates".
    bool ai_keystates[256];
    std::vector<short> ai_key_events;

    //If true, this player is AI controlled.
    bool ai_controlled;

    Coords decision_target;
    short decision_type;
    short counter_decision_cooldown_revive;

    short counter_path_far;
    short counter_path_medium;

    short counter_path_update;
    short counter_path_giveup;

    std::vector<Path> path;

    Player_Mp(std::vector<Input_Data> get_keys,int get_which_mp_player,bool get_ai_controlled);

    void reset();

    void load_data();

    void check_special_items();

    void set_physics();

    void update_character();

    //Start or stop crouching.
    void crouch_start();
    void crouch_stop();

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

    //Render the sprite.
    void render(bool mirrored=false);

    void render_path();

    std::string get_decision_type();

    image_data* return_character_image();
    image_data* return_character_worldmap_image();

    sound_data* return_character_sound_footstep();
    sound_data* return_character_sound_footstep2();
    sound_data* return_character_sound_jump();
    sound_data* return_character_sound_start_slide();

    void ai(int our_index);

    void prepare_for_input();

    void handle_input_states();

    void handle_input_events();

    void handle_command_event(short command);

    bool command_state(short command);

    void mp_reset(double new_x,double new_y);

    void ai_decision(int our_index);
    void ai_update_path();

    bool ai_tile_is_air(Tile* tile);

    int ai_tile_on_list(std::vector<Path_Calc>* path_list,int tile_x,int tile_y);
    bool ai_tile_valid(int int_x,int int_y,int target_x,int target_y);
    int ai_get_gravity_cost(int int_x,int int_y);
    int ai_get_tile_cost(int int_x,int int_y);
    void ai_add_open_tiles(Path_Calc* parent,int target_x,int target_y,std::vector<Path_Calc>* open_list,std::vector<Path_Calc>* closed_list,std::vector<Path_Calc>* new_open_tiles);
    void ai_path_to_target(int target_x,int target_y);

    //Move to the next node on the path.
    void ai_move_to_path();

    void ai_jump();
    void ai_shoot();

    void worldmap_ai();
    void worldmap_ai_follow_player(int player_index);
    void worldmap_ai_move_to_target(double target_x,double target_y,double target_w,double target_h);
    void worldmap_handle_input_states();
    void worldmap_move();
    void worldmap_handle_events();
    void worldmap_animate();
    void worldmap_render();
};

#endif
