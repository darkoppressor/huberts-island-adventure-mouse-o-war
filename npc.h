/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef npc_h
#define npc_h

#include <vector>

#include <SDL.h>

#include "timer.h"
#include "sprite_sheets.h"
#include "positional_audio.h"
#include "image_data.h"
#include "actor.h"
#include "light_data.h"

class Npc: public Actor{
    private:
    //Virtual AI "keystates".
    bool ai_keystates[256];

    public:
    Npc(double get_x,double get_y,short get_type,bool make_traps=true);

    void set_color();

    //Get AI.
    void handle_ai(int index);

    //Handle input.
    void handle_input();

    //Move the NPC.
    void move();

    void handle_tiles(int check_x_start,int check_x_end,int check_y_start,int check_y_end);

    //Handle events, such as collision checks.
    void handle_events(bool being_pushed_up=false);

    //Handle the NPC dying.
    void handle_death(bool override_invulnerability=false);

    //Do the standard death stuff.
    void standard_death();

    //Animate the sprite.
    void animate();

    //Render the sprite.
    void render(bool mirrored=false);

    //If true, the npc moves, checks for collisions, renders, etc.
    //If false, the npc has died.
    bool exists;

    //The current health and max health of the npc.
    //Only used for bosses.
    short health;
    short health_max;

    //These will be set to the relevant sprite numbers for the current NPC type upon this NPC's creation.
    short NPC_WALK_SPRITES;
    short NPC_JUMP_SPRITES;
    short NPC_SWIM_SPRITES;
    short NPC_FLY_SPRITES;
    short NPC_SPECIAL_ATTACK_SPRITES;
    short NPC_DEAD_SPRITES;

    image_data* sprite_sheet_image_npc;
    std::vector<SDL_Rect> vector_sprites_left;
    std::vector<SDL_Rect> vector_sprites_right;

    std::vector<SDL_Rect> vector_sprites_jump_left;
    std::vector<SDL_Rect> vector_sprites_jump_right;

    std::vector<SDL_Rect> vector_sprites_swim_left;
    std::vector<SDL_Rect> vector_sprites_swim_right;

    std::vector<SDL_Rect> vector_sprites_fly_left;
    std::vector<SDL_Rect> vector_sprites_fly_right;

    std::vector<SDL_Rect> vector_sprites_special_attack_left;
    std::vector<SDL_Rect> vector_sprites_special_attack_right;

    std::vector<SDL_Rect> vector_sprites_dead;

    //Current frame of animation.
    short frame_special_attack;
    short frame_counter_special_attack;

    //The type of NPC.
    //See npc_data.h for a full list of NPC types.
    short type;

    //If true, this npc is a boss.
    //If false, it isn't.
    bool BOSS;

    //If true, this npc is a main game boss.
    //If false, it isn't.
    bool MAIN_BOSS;

    //The current phase this npc is in.
    short boss_phase;

    //The color of the npc.
    //Only applies if not COLOR_WHITE.
    short color;

    //AI functions:

    //Activates or deactivates the boss as needed.
    void ai_boss_active_check(int index);

    //Returns true if the npc is next to an edge.
    bool ai_notice_edge();

    //Returns true if the npc is next to a wall.
    bool ai_notice_wall();

    //Returns true if a slope is nearby.
    bool ai_slope_nearby();

    //Use a trigger, if possible.
    void ai_attempt_use();

    //Initiate a jump, if possible.
    void ai_attempt_jump();

    void ai_make_attempts();

    //Initiate the npc's special attack.
    void ai_initiate_special_attack(int player_index);

    //Handle the npc's special attack.
    void ai_handle_special_attack();

    //End the npc's special attack.
    void ai_end_special_attack();

    //Move away from a target location.
    void ai_move_from_target(double target_x,double target_y,double target_w,double target_h);

    //Move towards a target location.
    void ai_move_to_target(double target_x,double target_y,double target_w,double target_h);

    //Checks to see if the player is in range for something.
    //Returns true if the player is in range.
    //Returns false if the player is out of range.
    bool ai_player_in_range(short range_x,short range_y,int player_index);

    //Checks to see if the player's back is turned to this npc.
    //Returns true if the player's back is turned.
    //Returns false if the player's back is not turned.
    bool ai_player_back_turned(int player_index);

    //Checks to see if the npc is facing the player.
    //Returns true if the npc is facing the player.
    //Returns false if the npc is not facing the player.
    bool ai_facing_player(int player_index);

    //Returns true if the player satisfies all criteria for following.
    //Otherwise, returns false.
    bool ai_player_followable(int player_index);

    //Returns true if the player satisfies all criteria for fleeing.
    //Otherwise, returns false.
    bool ai_player_fleeable(int player_index);

    void ai_follow_player(int player_index);

    void ai_flee_player(int player_index);

    //Returns the current direction the target is in in relation to the npc, as an angle.
    double ai_get_target_direction(double target_x,double target_y,double target_w,double target_h);

    //Used in checking a line between the npc and its target.
    bool ai_int_past_goal(double check_x,double check_y,double goal_x,double goal_y);

    //Returns the index for the nearest player.
    int ai_get_nearest_player();

    //Used by replicators.
    bool water_touching_this_tile(int int_x,int int_y);

    //Special attacks:
    void ai_special_attack_replicate();
    void ai_special_attack_bee_divebomb();
    void ai_special_attack_ghost_hide();
    void ai_special_attack_cat_pounce();
    void ai_special_attack_ram_charge();
    void ai_special_attack_eagle_divebomb();
    void ai_special_attack_sphinx_suck_left();
    void ai_special_attack_sphinx_suck_right();
    void ai_special_attack_henchmouse_sound_alarm();

    //AI variables:

    //This is used to keep track of how recently this npc was targeted by a player AI.
    short counter_targeted;

    //If true, this npc is friendly (harmless) to players.
    //If false, this npc is not friendly to players.
    bool friendly;

    //If true, can be duplicated on level load for difficulty stuff.
    //If false, cannot.
    bool can_be_duplicated;

    //If true, this npc counts as a trap instead of an npc for stat purposes.
    //If false, this npc counts as an npc like normal.
    bool counts_as_trap;

    //If true, the npc acts like a platform to the player.
    //If false, the npc does not.
    bool act_as_platform;

    //If true, the npc can pass through solid objects.
    //If false, the npc is blocked by solid objects like normal.
    bool ethereal;

    //If true, this npc can pass through other npcs, and them through it.
    //Aside from that, it is normal.
    //If false, ignore this.
    bool ethereal_to_npcs;

    //If true, the npc never moves, whether alive or dead.
    //If false, the npc can move normally.
    bool never_move;

    //If true, touching the enemy will kill the player.
    //If false, the player will not interact with the enemy.
    bool deadly_to_touch;

    //If true, the npc is deadly while special attacking.
    //If false, the npc is not deadly while special attacking.
    bool deadly_while_special_attacking;

    //If true, the npc has footstep and related sounds.
    //If false, the npc does not.
    bool has_footsteps;

    //How frequently to play the footstep sound.
    short footstep_sound_frequency;
    short footstep_special_attack_sound_frequency;

    //The coordinates of the npc's home position.
    //Currently only used for some special attacks.
    //When a special attack that involves movement begins, the npc sets its home position to its current location.
    //After the special attack, the npc will return to this position.
    double home_x;
    double home_y;

    //If true, the home coordinates are set on spawn and stay the same forever.
    //If false, the home coordinates are always reset.
    bool home_permanent;

    //If true, the npc is returning to its home position.
    bool returning_home;

    //If true, the npc will wander about.
    //If false, the npc will stand still.
    bool wander;

    //If true, the npc will stop and change directions when encountering the edge of the ground.
    //If false, the npc will ignore such things and walk about blindly.
    bool wander_notice_edges;

    //If true, the npc will stop and change directions when encountering a wall.
    //If false, the npc will ignore such things and walk about blindly.
    bool wander_notice_walls;

    //If true, ignore cloud tiles when checking for a wall.
    //If false, notice cloud tiles normally.
    bool notice_wall_ignore_cloud;

    //If true, a wandering npc will sometimes pause and wait before moving again.
    bool wander_pause;

    //How frequently to change the npc's wander direction, in frames.
    short wander_frequency;

    //The minimum and maximum amount of time that the npc will wait before changing wander_direction again.
    //Whenever wander_direction changes, wander_frequency is set to a new random value between wander_frequency_min and wander_frequency_max.
    short wander_frequency_min,wander_frequency_max;

    //Counter variable that keeps track of how many frames it has been since the last wander direction change.
    //When this reaches wander_frequency, the wander direction changes.
    short wander_counter;

    //The current direction that the npc is wandering on the x axis.
    short wander_direction;

    //The current direction that the npc is wandering on the y axis.
    short wander_direction_up_down;

    //The percentage chance that the npc will wander in a direction (versus pausing, using special attack, etc.) when choosing a new wander state.
    short wander_weight;

    //If true, the npc can wander up and down in addition to left to right.
    //If false, the npc only wanders left and right.
    bool wander_up_and_down;

    //If true, the npc can wander up and down, but NOT left and right.
    //If false, ignore this.
    bool wander_up_and_down_only;

    //If true, the npc will jump while moving.
    //If false, the npc will not jump while moving.
    bool jump;

    //If true, and wander_jump is true, will ONLY jump while moving, and won't move otherwise.
    //If false, the npc can move normally.
    bool jump_only;

    //How often (in frames) the npc can jump.
    short jump_frequency;

    //Counter variable that keeps track of how many frames it has been since the last jump.
    //When reset, it is set to jump_frequency and begins counting down to 0.
    //At 0, the npc can jump again.
    short jump_counter;

    //The percentage chance the npc will attempt jump when doing a jump check.
    short jump_percent;

    //If true, the npc will use triggers.
    //If false, the npc will not use triggers.
    bool use;

    //How often (in frames) the npc can use a trigger.
    short use_frequency;

    //Counter variable that keeps track of how many frames it has been since the last use.
    //When reset, it is set to use_frequency and begins counting down to 0.
    //At 0, the npc can use again.
    short use_counter;

    //If true, only use triggers while the alarm is off.
    //If false, ignore this check.
    bool use_only_while_alarm_off;

    //If true, overrides wander whenever the player comes within follow_player_range of the npc.
    //The npc will then follow the player until a) the player dies, b) the npc dies, c) the player leaves the npc's follow range.
    //If false, the npc's movement will be unaffected by the player.
    bool follow_player;

    //If the player comes within this range and follow_player is true, the npc will begin following the player.
    //These ranges are in tiles.
    short follow_player_range_x;
    short follow_player_range_y;

    //If true, only follow the player if the player is swimming.
    //If false, follow the player regardless of his status.
    bool follow_player_only_in_water;

    //If true, only follow the player if the npc is in the air.
    //If false, ignore this check.
    bool follow_player_only_in_air;

    //If true, only follow the player if the player's back is turned to this npc.
    //If false, ignore this check.
    bool follow_player_only_when_back_turned;

    //If true, the player's back is still considered turned when he is above the npc.
    //If false, the player's back is only turned if it is REALLY turned.
    bool player_back_turned_when_above;

    //If true, ignore all other follow bools when this npc is in the air.
    //If false, ignore this.
    bool follow_player_always_in_air;

    //If true, follow the player during an alarm.
    //If false, ignore this check.
    bool follow_player_during_alarm;

    //If true, the npc must be angry to follow the player.
    //If false, ignore this.
    //This is only used for replicators right now.
    bool follow_player_only_when_angry;

    //Identical to follow_player, except the npc will instead seek to distance itself from the player.
    bool flee_player;

    //Identical to follow_player_range, but used for fleeing.
    //This range is in tiles.
    short flee_player_range;

    //The type of special attack.
    short special_attack_type;

    //If true, the npc will periodically use its special attack while chasing.
    //If false, the npc will not use a special attack while chasing.
    bool special_attack_when_chasing;

    //If true, the npc will periodically use its special attack while chasing.
    //If false, the npc will not use a special attack while chasing.
    bool special_attack_when_fleeing;

    //Similar to special_attack_when_chasing, but does not require follow_player to be true.
    //If true, the npc will use its special attack if it sees the player and the player is in range.
    bool special_attack_on_sight;

    //If true, only allow a special attack if the player is facing the npc.
    //If false, ignore this check.
    bool special_attack_only_when_player_facing;

    //If true, only allow a special attack if the npc is facing the player.
    //If false, ignore this check.
    bool special_attack_only_when_facing_player;

    //If true, only allow a special attack if the alarm is not currently sounding.
    //If false, ignore this check.
    bool special_attack_only_when_alarm_off;

    //If true, special attack can be used regardless of state.
    //If false, the other special attack variables (special_attack_when_chasing, etc.) will be used.
    bool special_attack_anytime;

    //If the player comes within special_attack_range_x and special_attack_range_y and any/all other special attack conditions are met, the npc will use its special attack.
    //This range is in tiles.
    short special_attack_range_x;
    short special_attack_range_y;

    //If true, only attack below the npc.
    bool special_attack_below_only;

    //If true, only attack above the npc.
    bool special_attack_above_only;

    //The coordinates of the special attack's current target.
    double special_attack_target_x;
    double special_attack_target_y;

    //How long (in frames) the npc will special attack/chase its target before giving up.
    //A negative number means never give up.
    short special_attack_give_up_time;

    //Counter variable that keeps track of how many frames the npc has been attempting a special attack or chasing the player.
    //When reset, it is set to special_attack_give_up_time and begins counting down to 0.
    //At 0, the npc will give up on its current special attack or chasing the player.
    short special_attack_give_up_counter;

    //How often (in frames) the npc can use its special attack.
    //It does not necessarily use the special attack when this is reached, but it becomes able to again.
    short special_attack_frequency;

    //Counter variable that keeps track of how many frames it has been since the last special attack.
    //When reset, it is set to special_attack_frequency and begins counting down to 0.
    //At 0, the npc can use its special attack again.
    short special_attack_counter;

    //When the npc "uses" its special attack, it waits special_attack_pause_length frames before actually using it.
    short special_attack_pause_length;

    //Whenever a special attack is initiated, special_attack_pause_counter is set to special_attack_pause_length and begins counting down to 0.
    //At 0, the npc can actually use its special attack.
    short special_attack_pause_counter;

    //The frame of special attack animation at which to play the special attack sound.
    short special_attack_sound_frame;

    //Set to true when an npc uses its special attack.
    //Set to false when an npc is no longer using its special attack.
    bool special_attack_in_progress;

    //If true, the special attack ends when the special attack animation ends.
    //If false, special end conditions are handled in ai_handle_special_attack().
    bool special_attack_ends_with_animation;

    //If true, the special attack's target will be a random position.
    //If false, the special attack's target will not be random.
    bool special_attack_random_target;

    //The max x-axis speed of the actor while in the air.
    //Only applied if > 0.0.
    double max_speed_in_air;

    //The max x-axis speed of the npc while doing a special attack.
    //Only applied if > 0.0.
    double max_speed_during_special_attack;

    //The max x-axis speed of the npc while the alarm is sounding.
    //Only applied if > 0.0.
    double max_speed_during_alarm;

    //The max swim speed of the npc while at the surface of the water.
    //Only applied if > 0.0.
    double max_swim_speed_at_surface;

    double default_max_speed;
    double default_max_swim_speed;

    //If true, the npc is angered.
    //If false, the npc is passive.
    //Only used for replicators.
    ///Possibly not needed, but we'll see.
    bool angry;

    //If true, the npc can crawl on walls and ceilings.
    //If false, the npc can only walk on the ground.
    //When an npc with crawls==true encounters a wall, they may begin "crawling" up or down it.
    //Crawling is really just flying.
    bool crawls;

    //If true, the NPC drops items upon its death.
    //If false, the NPC drops nothing on death.
    //This only applies to non-bosses.
    bool drops_items;

    bool custom_physics;

    bool wears_tophat;
    bool tophat_when_dead;
    double tophat_adjust_x;
    double tophat_adjust_y;

    //If true, this NPC will never fade out.
    bool starts_dead;

    //Frame speeds.
    short frame_speed_walk;
    short frame_speed_special_attack;
    short frame_speed_death;
    short frame_speed_fly;
    short frame_speed_swim;

    short counter_fade;

    //The object's light source data.
    Light_Source light_source;

    //If true, special attack toggles light.
    //If false, it doesn't.
    bool special_attack_toggles_light;

    //If true, special attack ending toggles light.
    //If false, it doesn't.
    bool special_attack_end_toggles_light;
};

#endif
