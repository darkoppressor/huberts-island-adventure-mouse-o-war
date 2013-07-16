/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef shot_h
#define shot_h

#include "shot_data.h"
#include "image_data.h"
#include "light_data.h"

class Shot{
    private:
    //The size modifier of the hitbox.
    short hitbox_size_modifier;

    //The speed at which the shot moves.
    double move_speed;

    //The direction the shot is moving.
    double angle;

    //If true, this shot is affected by gravity.
    //If false, this shot is not affected by gravity.
    bool gravity_affects;

    //The maximum speed of gravity.
    double gravity_max;
    //The rate of acceleration due to gravity. Capped by gravity_max.
    double gravity;
    //The velocity at which the shot is traveling while in the air.
    double air_velocity;

    //If true, the shot will be rendered rotated to face its facing angle.
    //If false, the shot will be rendered with no rotation.
    bool render_rotated;

    //If render_rotated is true, this angle is used for rendering the rotated shot.
    double render_angle;

    public:
    //The contructor takes starting coordinates and a movement direction.
    Shot(double get_x,double get_y,double get_angle,short get_type,bool get_counts_as_trap);

    //Return the corresponding coordinate or dimension, adjusted for the object's hitbox size.
    double return_x();
    double return_y();
    double return_w();
    double return_h();

    bool get_phasing();

    bool moving_up();
    bool moving_down();
    bool moving_left();
    bool moving_right();

    //Returns the index for the nearest player.
    int get_nearest_player();

    //Returns the current direction the target is in in relation to the shot, as an angle.
    double get_target_direction(double target_x,double target_y,double target_w,double target_h);

    //Move due to a sucking force.
    void move_suck(short direction);

    //Move the shot.
    void move();

    //Handle collision checks.
    void handle_events();

    //Animate the sprite.
    void animate();

    //Render the sprite.
    void render(bool mirrored=false);

    void dissipate();

    //If true, homes in on the player.
    //If false, ignore this.
    bool home_on_player;

    //If true, homes in on the nearest npc.
    //If false, ignore this.
    bool home_on_npcs;

    //If true, this shot is not killed by deadly water.
    bool deadly_water_proof;

    //The type of shot.
    short type;

    //If true, this shot is asscoiated with a boss.
    //If false, it isn't.
    bool BOSS;

    //Whether or not the shot is effective against the player and/or npcs.
    bool kills_player;
    bool kills_npc;

    //If true, does damage.
    //If false, does no damage.
    bool does_damage;

    //If true, this is one of the player's shots.
    //If false, this is not a player shot.
    bool player_shot;

    //If true, this shot counts as a trap's shot for stat purposes.
    //If false, this shot counts as an npc's shot.
    bool counts_as_trap;

    //If true, the shot moves, checks for collisions, renders, etc.
    //If false, the shot has hit a wall, enemy, etc. and is no longer being used.
    bool exists;

    //If true, finish the dissipation of the shot and then stop existing.
    bool dissipating;

    //If true, this shot is an explosion.
    bool explosion;

    //If true, this shot has used a trigger.
    //If false, this shot has not yet used a trigger.
    //This is only used by an explosion shot.
    bool trigger_used;

    //If true, the actor is being sucked in the corresponding direction.
    //If false, the actor is not being sucked.
    bool sucked_left;
    bool sucked_right;

    //Keeps track of how long (in frames) this shot has been sucked in the given direction.
    short counter_sucked_left;
    short counter_sucked_right;

    //The percentage chance that the shot will home in a given frame (if it is a homing shot).
    short homing_chance;

    //The percentage chance that the shot will stop homing entirely.
    short homing_stop_chance;

    //The current coordinates of the shot.
    double x,y;

    //The dimensions of the shot. These are used for collision detection.
    double w,h;

    //These will be set to the relevant sprite numbers for the current shot type upon this shot's creation.
    short SHOT_SPRITES;

    image_data* sprite_sheet_image_shot;

    std::vector<SDL_Rect> vector_sprites;

    short frame,frame_counter,frame_counter_limit;

    //The object's light source data.
    Light_Source light_source;
};

#endif
