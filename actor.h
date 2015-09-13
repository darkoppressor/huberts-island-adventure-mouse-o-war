/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef actor_h
#define actor_h

#include <SDL.h>

#include "timer.h"
#include "sprite_sheets.h"
#include "positional_audio.h"

const double WATER_RUN_THRESHOLD=7.1;

struct collision_data_trap{
    double x;
    double y;
    double w;
    double h;
};

class Actor{
    private:
    public:

    //Return the corresponding coordinate or dimension, adjusted for the object's hitbox size.
    double return_x();
    double return_y();
    double return_w();
    double return_h();

    int get_extra_jumps_max();

    void after_input();

    void play_footstep_sound();

    //Move due to a sucking force.
    void move_suck(short direction);

    //Increment counters.
    void handle_counters();

    void set_solid_above();

    void prepare_for_events();

    void handle_npc_platform(int i);

    void handle_player_platform(int i);

    void handle_collision_moving_platforms(short solidity,bool counts_as_trap);

    void handle_collision_doors();

    void handle_collision_springs(short solidity);

    void handle_collision_boosters();

    //Returns the collision data for the trap that killed the player, if any.
    collision_data_trap handle_collision_traps(short solidity);

    bool push_actor_up(Actor* actor,double actor_x,double actor_w,bool being_pushed_up,short passed_type);

    void pushed_into_ceiling();

    void handle_collision_solid(double passed_x,double passed_y,double passed_w,double passed_h,short solidity,short passed_type);

    void handle_collision_tiles(int int_x,int int_y,bool crouching);

    void handle_climbing(int int_x,int int_y);

    void finish_events();

    void fall_through_cloud();

    //Returns true if the actor is touching a solid tile.
    //Returns false if the actor is not touching any solid tiles.
    bool touching_solid_tile(int check_x_start,int check_x_end,int check_y_start,int check_y_end);

    //Returns the distance to the ground in tiles.
    int distance_to_ground();

    //Returns true if the actor is next to a wall.
    //Note that Npc's should use their ai_notice_wall() instead.
    bool notice_wall();

    //If true, the actor is in open air.
    //If false, the actor is inside something solid.
    bool in_open_air();

    //Needed for Player.
    virtual void crouch_stop();

    //The size modifier of the hitbox.
    short hitbox_size_modifier;

    //Current movement state of the actor.
    short move_state;

    //Current direction for shooting purposes. When multiple keys are held, the priorities are different for shooting than for moving.
    short shoot_state;

    //If true, the actor is shooting a shot (or trying to, SHOOTING does not require any ammo to become true).
    //If false, the actor is not currently trying to shoot.
    bool SHOOTING;

    //When the actor shoots, the direction of of the shooting is remembered here for rendering purposes.
    short shoot_render_direction;

    //Current jump state of the actor.
    bool jump_state;

    //Used to determine if the actor can still jump again while in air.
    int extra_jumps;

    //The below two variables are used to determine if the actor has walked off of a ledge, and should begin falling.
    //Tells us if the actor's feet are currently touching air or ground.
    bool touching_air;
    bool touching_ground;

    //Is the actor touching a sloping ground tile?
    bool touching_sloped_ground;

    //If the actor is touching a sloping ground tile, this is its angle.
    double touched_slope_angle;

    //Was the actor touching sloped ground last check?
    bool touching_sloped_ground_last_check;

    bool solid_above;

    //Is the actor underwater?
    bool underwater;

    //Is the actor currently standing on a cloud tile?
    bool on_cloud;

    //If true, the actor is currently standing on a rideable npc.
    //If false, it is not.
    //This is set right along with on_cloud, but is only set to true when the "cloud" is a rideable npc.
    bool cloud_npc;

    //If true, the actor is being sucked in the corresponding direction.
    //If false, the actor is not being sucked.
    bool sucked_left;
    bool sucked_right;

    bool undead;

    //The x-axis speed of the actor.
    double run_speed;

    double acceleration;
    double deceleration;
    double friction;
    double max_speed;
    double air_accel;
    double air_decel;
    double air_drag;
    double air_drag_divisor;

    double speedometer;

    //The climbing speed of the actor.
    double climb_speed;

    //The maximum swimming speed in all directions.
    double max_swim_speed;
    //The current y-axis speed of the actor, used only when swimming.
    double float_speed;
    //The maximum speed that the actor will float upwards in water without actively swimming upwards.
    //In other words, if the actor is still in water, and doesn't hit the up or down directional keys, the actor will slowly accelerate upwards until reaching this speed.
    //The actor can continue to accelerate faster on his own by pressing the up directional key, up to max_swim_speed.
    double max_buoyancy;
    //The rate of acceleration in all directions while swimming.
    //Also used in simulating buoyancy (although to a lesser degree).
    double swim_acceleration;
    double swim_deceleration;
    double swim_friction;
    //When the actor hits water, this timer is started.
    //While the timer is running, the actor's buoyancy is increased so they actor floats quickly to the top of the water when he first jumps in.
    Timer timer_time_in_water;
    //If true, the actor is running on water.
    //If false, the actor is not.
    bool water_running;

    double climbing_jump_max;
    double climbing_jump_min;

    bool CLIMBING_JUMP;

    //While non-zero the actor cannot begin climbing.
    //Whenever the actor stops climbing, this is set to climb_jump_delay.
    short climb_jump_timer;

    short climb_jump_delay;

    //The direction the actor is moving (as far as grabbing onto a climbable tile goes).
    short climb_state;

    //This variable defines maximum jump height.
    double jump_max;
    //The minimum height of a jump.
    double jump_min;
    //The maximum speed of gravity.
    double gravity_max;
    //The rate of acceleration due to gravity. Capped by gravity_max.
    double gravity;

    //The current coordinates of the actor.
    double x,y;

    //The dimensions of the actor.
    double w,h;

    //Simply tells us whether or not the actor is currently in the air. If so, the actor's y coordinate will be incremented by air_velocity.
    bool IN_AIR;
    //The velocity at which the actor is traveling while in the air.
    double air_velocity;

    //Tells us whether or not the actor is currently swimming.
    bool SWIMMING;
    //If the actor is swimming, this tells us whether or not the actor can jump (is at the surface of the water).
    bool SWIM_CAN_JUMP;
    //If true, the actor has swimming gear.
    //If false, the actor does not have swimming gear.
    bool swimming_gear;
    //The amount of oxygen the actor currently has. Drops when swimming without swimming gear.
    //When this number reaches 0, the actor dies.
    short oxygen;
    //The maximum amount of oxygen the actor can hold.
    short oxygen_max_capacity;

    bool suit_deadly_water;
    bool suit_sharp;
    bool suit_banana;
    bool shot_homing;
    bool translator;
    bool j_wing;

    //Tells us whether or not the actor is currently climbing.
    bool CLIMBING;

    //Tells us whether or not the actor is currently standing on a climbable top tile.
    bool on_climbable_top_tile;

    //Current moving platform movement modifiers.
    double moving_platform_x;
    //Only used for dropping down from a moving platform.
    double moving_platform_y;
    //If true, the actor was on a moving platform moving on the x-axis this frame.
    //If false, it wasn't.
    bool MOVING_PLATFORM_X_THIS_FRAME;
    //If true, the actor is on a moving platform that is in the water.
    //If false, it isn't.
    bool MOVING_PLATFORM_IN_WATER;

    //Current direction the actor is facing.
    short facing;

    //If true, standard movement is overridden with flying movement.
    //Note: This is currently only used for npcs.
    bool FLYING;

    //If true, this actor is a player.
    //If false, this actor is not a player.
    bool is_player;

    //If 0 or greater, this is a MP player.
    int which_mp_player;

    //If true, the actor cannot be killed.
    //If false, the actor can be killed.
    bool invulnerable;

    bool DYING;

    //Counts down to when the actor can produce another bubble effect.
    short counter_bubble;

    //Animation frames and frame counters.
    short frame_idle;
    short frame_counter_idle;

    short frame;
    short frame_counter;

    short frame_swim;
    short frame_counter_swim;

    short frame_jump;
    short frame_counter_jump;

    short frame_shoot;
    short frame_counter_shoot;

    short frame_climb;
    short frame_counter_climb;

    short frame_fly;
    short frame_counter_fly;

    short frame_death;
    short frame_counter_death;

    //Keeps track of the last frame's move_state.
    //Currently just used for the swimming sound effect.
    short delta_move_state;

    short counter_swim_sound;
};

#endif
