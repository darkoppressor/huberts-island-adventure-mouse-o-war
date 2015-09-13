/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef moving_platform_h
#define moving_platform_h

#include <vector>

#include "moving_platform_waypoint.h"

class Actor;

class Moving_Platform{
    private:
    double start_point_x,start_point_y;

    double end_point_x,end_point_y;

    std::vector<moving_platform_waypoint> waypoints;

    public:
    Moving_Platform(double get_move_speed,double get_start_x,double get_start_y,double get_end_x,double get_end_y,std::vector<moving_platform_waypoint> get_waypoints,bool get_active,bool get_round_trip,bool get_movement_type,short get_identifier);

    //Move the platform.
    void move();

    //Handle collision checks.
    void handle_events();

    //Animate the sprite.
    void animate();

    //Render the sprite.
    void render(bool mirrored=false);

    //The direction the platform is moving.
    short move_state;

    //The speed at which the platform moves.
    double move_speed;

    short identifier;

    //If active is true, the platform moves, checks for collisions, etc.
    bool active;

    //If true, platform continues moving when it reaches its goal.
    //If false, platform becomes inactive when it reaches its goal.
    bool round_trip;

    //False = Back and forth. When end point is reached, platform returns to start point via waypoints. When start point is reached, platform returns to end point via waypoints.
    //True = Circular. When end point is reached, platform returns to start point directly. When start point is reached, platform returns to end point via waypoints.
    bool movement_type;

    //If true, the goal is the end point.
    //If false, the goal is the start point.
    bool goal;

    //The coordinates of the current goal.
    //The goal is the current ultimate destination of the platform.
    double goal_x,goal_y;

    //The current type of target point.
    //0 = Start point.
    //1 = Waypoint.
    //2 = End point.
    short target_type;

    //The current target.
    //Should be 0 if target_type is 0 or 2 (i.e., not a waypoint).
    //If target_type is 1 (i.e., a waypoint), this number represents the waypoint's unique indentifier.
    short target;

    //The current target's coordinates.
    double target_x;
    double target_y;

    //The current coordinates of the platform.
    double x,y;

    //The dimensions of the platform.
    double w,h;

    //Current direction the platform is facing.
    short facing;

    //Animation frame and frame counter.
    short frame,frame_counter;
    short frame_idle,frame_counter_idle;
};

#endif
