/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef item_h
#define item_h

#include "item_sprites.h"
#include "sprite_sheets.h"
#include "timer.h"

#include <SDL.h>

class Item{
    private:
    //The direction the item is moving.
    short move_state;

    //The speed at which the item moves.
    double run_speed;

    double max_speed;
    double air_accel;
    double air_decel;
    double air_drag;
    double air_drag_divisor;

    bool SWIMMING;

    double max_swim_speed;
    double float_speed;
    double max_buoyancy;
    double swim_acceleration;
    Timer timer_time_in_water;

    //Simply tells us whether or not the item is currently in the air. If so, the item's y coordinate will be incremented by air_velocity.
    //Also, the movement code, event code, etc. only runs when the item is in the air. Once it lands on the ground somewhere, it no longer moves or handles events.
    bool IN_AIR;

    //The velocity at which the item is traveling while in the air.
    double air_velocity;

    //The maximum speed of gravity.
    double gravity_max;
    //The rate of acceleration due to gravity. Capped by gravity_max.
    double gravity;

    public:
    //The contructor takes starting coordinates and a movement direction.
    Item(double get_x,double get_y,bool get_IN_AIR,short get_type,int get_goal_level_to_load,bool get_goal_secret,int min_move=10,int max_move=20,int min_jump=20,int max_jump=40,bool can_start_falling=false,short get_counter_vacuum=0,int get_score_bonus=0);

    void set_color();

    //Returns the index for the nearest player.
    int get_nearest_player();

    //Returns the current direction the target is in in relation to the item, as an angle.
    double get_target_direction(double target_x,double target_y,double target_w,double target_h);

    //Move the item.
    void move();

    //Handle collision checks.
    void handle_events();

    //Animate the sprite.
    void animate();

    SDL_Rect* get_texture_clip(bool survival_spawn);

    //Render the sprite.
    void render(bool mirrored=false,bool survival_spawn=false);

    //Negative numbers: levels.
    //So -1 is level 1, -2 is level 2, etc.
    short type;

    //If exists==true, the item moves, checks for collisions, renders, etc.
    //If it is false, the item has either been collected or fallen out of the level.
    bool exists;

    //The current coordinates of the item.
    double x,y;

    //The dimensions of the item. These are used for collision detection.
    double w,h;

    //Current frame of animation.
    short frame;
    short frame_counter;

    //The color of the item.
    //Only applies if not COLOR_WHITE.
    short color;

    //These two variables let the game know if the checkpoint was just reached or deactivated (a different checkpoint was reached).
    //Used for animation.
    bool checkpoint_reached;
    bool checkpoint_unreached;

    //The level to load when this goal item is reached.
    //Only used if the item is a goal item, obviously.
    int goal_level_to_load;

    //If true, the goal is a secret goal.
    //If false, the goal is a standard goal.
    bool goal_secret;

    //If non-zero, the item cannot be vacuumed.
    short counter_cannot_be_vacuumed;

    //A percentage bonus to the score this item is worth.
    int score_bonus;
};

#endif
