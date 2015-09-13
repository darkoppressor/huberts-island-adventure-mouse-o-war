/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "moving_platform.h"
#include "render.h"
#include "world.h"
#include "collision.h"
#include "mirror.h"

using namespace std;

Moving_Platform::Moving_Platform(double get_move_speed,double get_start_x,double get_start_y,double get_end_x,double get_end_y,vector<moving_platform_waypoint> get_waypoints,bool get_active,bool get_round_trip,bool get_movement_type,short get_identifier){
    //Variables:

    move_state=0;

    move_speed=get_move_speed;

    start_point_x=get_start_x;
    start_point_y=get_start_y;

    x=start_point_x;
    y=start_point_y;

    end_point_x=get_end_x;
    end_point_y=get_end_y;

    waypoints=get_waypoints;

    w=48;
    h=10;

    active=get_active;

    round_trip=get_round_trip;

    movement_type=get_movement_type;

    identifier=get_identifier;

    goal=true;
    goal_x=end_point_x;
    goal_y=end_point_y;

    //If there is at least one waypoint, we start out targeting the first one.
    if(waypoints.size()>0){
        target_type=1;
        target_x=waypoints[0].x;
        target_y=waypoints[0].y;
    }
    //Otherwise we target the end point.
    else{
        target_type=2;
        target_x=end_point_x;
        target_y=end_point_y;
    }

    //First waypoint.
    target=0;

    facing=LEFT;

    frame=0;
    frame_counter=0;
    frame_idle=0;
    frame_counter_idle=0;

    short difficulty=player.return_actual_difficulty();

    if(difficulty==DIFFICULTY_EASY){
        move_speed/=2.0;
    }
    else if(difficulty>=DIFFICULTY_HARD){
        move_speed*=1.0+((double)(difficulty-DIFFICULTY_NORMAL)*0.15);

        if(move_speed>10.0){
            move_speed=10.0;
        }
    }
}

void Moving_Platform::move(){
    //If the platform is active.
    if(active){
        //*******************************//
        // Determine correct move_state: //
        //*******************************//

        if(x<target_x && y<target_y){
            move_state=RIGHT_DOWN;
        }
        else if(x>target_x+w && y<target_y){
            move_state=LEFT_DOWN;
        }
        else if(x>target_x+w && y>target_y+h){
            move_state=LEFT_UP;
        }
        else if(x<target_x && y>target_y+h){
            move_state=RIGHT_UP;
        }
        else if(x>target_x+w){
            move_state=LEFT;
            facing=LEFT;
        }
        else if(x<target_x){
            move_state=RIGHT;
            facing=RIGHT;
        }
        else if(y>target_y+h){
            move_state=UP;
            facing=UP;
        }
        else if(y<target_y){
            move_state=DOWN;
            facing=DOWN;
        }

        /**if(x<target_x && y<target_y){
            move_state=RIGHT_DOWN;
        }
        else if(x>target_x+w && y<target_y){
            move_state=LEFT_DOWN;
        }
        else if(x>target_x+w && y>target_y+h){
            move_state=LEFT_UP;
        }
        else if(x<target_x && y>target_y+h){
            move_state=RIGHT_UP;
        }
        else if(x>target_x+w){
            move_state=LEFT;
            facing=LEFT;
        }
        else if(x<target_x){
            move_state=RIGHT;
            facing=RIGHT;
        }
        else if(y>target_y+h){
            move_state=UP;
            facing=UP;
        }
        else if(y<target_y){
            move_state=DOWN;
            facing=DOWN;
        }*/

        //************************************//
        // Handle x-axis and y-axis movement: //
        //************************************//

        //////////////////////////////////////////////////
        //Move the platform according to its move state://
        //////////////////////////////////////////////////

        double run_chunk;
        if(fabs(move_speed)<pixel_safety_y){
            run_chunk=fabs(move_speed);
        }
        else{
            run_chunk=pixel_safety_y;
        }

        for(double i=fabs(move_speed);i>0;){
            //If we have run_chunk or more pixels left to move,
            //we will move run_chunk pixels, call handle_events(), and loop back up here.

            //Or, if we have less than run_chunk pixels left to move,
            //we will move whatever pixels we have left and call handle_events() once more.
            if(i<run_chunk){
                run_chunk=i;
                i=0;
            }

            //Move.
            if(move_state==LEFT){
                x-=run_chunk;
            }
            else if(move_state==RIGHT){
                x+=run_chunk;
            }
            else if(move_state==UP){
                y-=run_chunk;
            }
            else if(move_state==DOWN){
                y+=run_chunk;
            }
            else if(move_state==LEFT_UP){
                x-=run_chunk;
                y-=run_chunk;
            }
            else if(move_state==RIGHT_UP){
                x+=run_chunk;
                y-=run_chunk;
            }
            else if(move_state==RIGHT_DOWN){
                x+=run_chunk;
                y+=run_chunk;
            }
            else if(move_state==LEFT_DOWN){
                x-=run_chunk;
                y+=run_chunk;
            }

            //If we still have pixels left to move.
            if(i!=0){
                i-=run_chunk;
            }

            handle_events();
        }
    }

    //If the platform is inactive.
    else{
        move_state=0;
    }
}

void Moving_Platform::handle_events(){
    //If the current target point has been reached.
    if(collision_check(x,y,w,h,target_x-w,target_y-h*2,w*3,h*5)){
        //Set the new target.

        //If we were targeting the start point.
        if(target_type==0){
            //Set the end point as the new goal.
            goal=true;
            goal_x=end_point_x;
            goal_y=end_point_y;

            //If there are any waypoints.
            if(waypoints.size()!=0){
                //The new target will be the first waypoint.
                target_type=1;
                target=0;
                target_x=waypoints[0].x;
                target_y=waypoints[0].y;
            }
            //If there are no waypoints.
            else{
                //The new target will be the end point.
                target_type=2;
                target_x=end_point_x;
                target_y=end_point_y;
            }

            //If round_trip is false, the platform should now become inactive.
            if(!round_trip){
                active=false;
            }
        }

        //If we were targeting a waypoint.
        else if(target_type==1){
            //The goal does not change.

            //If the goal is the end point.
            if(goal){
                //Increment the targeted waypoint.
                target++;

                //If this is a valid waypoint.
                if(waypoints.size()>=target+1){
                    //The new target will be the next waypoint.
                    target_x=waypoints[target].x;
                    target_y=waypoints[target].y;
                }
                //If this is not a valid waypoint, we have reached the end of the waypoints.
                else{
                    //The new target will be the end point.
                    target_type=2;
                    target_x=end_point_x;
                    target_y=end_point_y;
                }
            }

            //If the goal is the start point.
            else if(!goal){
                //Decrement the targeted waypoint.
                target--;

                //If this is a valid waypoint.
                if(target>=0){
                    //The new target will be the previous waypoint.
                    target_x=waypoints[target].x;
                    target_y=waypoints[target].y;
                }
                //If this is not a valid waypoint, we have reached the beginning of the waypoints.
                else{
                    //The new target will be the start point.
                    target_type=0;
                    target_x=start_point_x;
                    target_y=start_point_y;
                }
            }
        }

        //If we were targeting the end point.
        else if(target_type==2){
            //Set the start point as the new goal.
            goal=false;
            goal_x=start_point_x;
            goal_y=start_point_y;

            //If there are any waypoints and the movement type is back and forth.
            if(waypoints.size()!=0 && !movement_type){
                //The new target will be the last waypoint.
                target_type=1;
                target=waypoints.size()-1;
                target_x=waypoints[target].x;
                target_y=waypoints[target].y;
            }
            //If there are no waypoints, or the movement type is circular.
            else{
                //The new target will be the start point.
                target_type=0;
                target_x=start_point_x;
                target_y=start_point_y;
            }

            //If round_trip is false, the platform should now become inactive.
            if(!round_trip){
                active=false;
            }
        }
    }

    //Check for collisions with npcs.
    for(int i=0;i<vector_npcs.size();i++){
        if(fabs(vector_npcs[i].x-x)<PROCESS_RANGE && fabs(vector_npcs[i].y-y)<PROCESS_RANGE){
            //Only do collision checks for the npc if it exists.
            if(vector_npcs[i].exists && !vector_npcs[i].counts_as_trap){
                //If the platform moves up into the bottom of the npc, and the npc is falling.
                if((move_state==UP || move_state==LEFT_UP || move_state==RIGHT_UP) && vector_npcs[i].air_velocity>=0){
                    //Check to see if there is a y-axis collision.
                    //We are checking only the bottom row of the pixels for the npc against only the top row of the pixels for the platform.
                    if(collision_check(x,y,w,h/12,vector_npcs[i].x,vector_npcs[i].y+vector_npcs[i].h-(vector_npcs[i].h/12),vector_npcs[i].w,vector_npcs[i].h/12)){
                        //We set the npc's y value to the y value of this platform - the npc's height, so the npc will now be right on top of this platform.
                        vector_npcs[i].y=y-vector_npcs[i].h;
                        //If the npc was in the air, they will no longer be.
                        vector_npcs[i].IN_AIR=false;
                        //The npc has been moved, so handle collision.
                        vector_npcs[i].handle_events();
                    }
                }
                //If the platform moves down with the npc on top of it.
                if((move_state==DOWN || move_state==LEFT_DOWN || move_state==RIGHT_DOWN) && vector_npcs[i].air_velocity>=0 && !vector_npcs[i].SWIMMING){
                    //Check to see if there is a y-axis collision.
                    //We are checking only the bottom row of the pixels for the npc against only the top row of the pixels for the platform.
                    if(collision_check(x,y,w,h/12,vector_npcs[i].x,2+vector_npcs[i].y+vector_npcs[i].h-(vector_npcs[i].h/12),vector_npcs[i].w,2+vector_npcs[i].h/12)){
                        //We set the npc's y value to the y value of this platform - the npc's height, so the npc will now be right on top of this platform.
                        vector_npcs[i].y=y-vector_npcs[i].h;
                        //If the npc was in the air, they will no longer be.
                        vector_npcs[i].IN_AIR=false;
                        //The npc has been moved, so handle collision.
                        vector_npcs[i].handle_events();
                    }
                }
            }
        }
    }

    //Check for a collision with the player.
    if(fabs(player.x-x)<PROCESS_RANGE && fabs(player.y-y)<PROCESS_RANGE){
        if(!player.cheat_noclip){
            //If the platform moves up into the bottom of the player, and the player is falling.
            if((move_state==UP || move_state==LEFT_UP || move_state==RIGHT_UP) && player.air_velocity>=0){
                //Check to see if there is a y-axis collision.
                //We are checking only the bottom row of the pixels for the player against only the top row of the pixels for the platform.
                if(collision_check(x,y,w,h/12,player.x,player.y+player.h-(player.h/12),player.w,player.h/12)){
                    //We set the player's y value to the y value of this platform - the player's height, so the player will now be right on top of this platform.
                    player.y=y-player.h;
                    //If the player was in the air, they will no longer be.
                    player.IN_AIR=false;
                    //The player has been moved, so handle collision.
                    player.handle_events();
                }
            }

            //If the platform moves down with the player on top of it.
            if((move_state==DOWN || move_state==LEFT_DOWN || move_state==RIGHT_DOWN) && player.air_velocity>=0 && !player.SWIMMING){
                //Check to see if there is a y-axis collision.
                //We are checking only the bottom row of the pixels for the player against only the top row of the pixels for the platform.
                if(collision_check(x,y,w,h/12,player.x,2+player.y+player.h-(player.h/12),player.w,2+player.h/12)){
                    //We set the player's y value to the y value of this platform - the player's height, so the player will now be right on top of this platform.
                    player.y=y-player.h;
                    //If the player was in the air, they will no longer be.
                    player.IN_AIR=false;
                    //The player has been moved, so handle collision.
                    player.handle_events();
                }
            }
        }
    }

    for(int i=0;i<mp_players.size();i++){
        if(fabs(mp_players[i].x-x)<PROCESS_RANGE && fabs(mp_players[i].y-y)<PROCESS_RANGE){
            //If the platform moves up into the bottom of the player, and the player is falling.
            if((move_state==UP || move_state==LEFT_UP || move_state==RIGHT_UP) && mp_players[i].air_velocity>=0){
                //Check to see if there is a y-axis collision.
                //We are checking only the bottom row of the pixels for the player against only the top row of the pixels for the platform.
                if(collision_check(x,y,w,h/12,mp_players[i].x,mp_players[i].y+mp_players[i].h-(mp_players[i].h/12),mp_players[i].w,mp_players[i].h/12)){
                    //We set the player's y value to the y value of this platform - the player's height, so the player will now be right on top of this platform.
                    mp_players[i].y=y-mp_players[i].h;
                    //If the player was in the air, they will no longer be.
                    mp_players[i].IN_AIR=false;
                    //The player has been moved, so handle collision.
                    mp_players[i].handle_events();
                }
            }

            //If the platform moves down with the player on top of it.
            if((move_state==DOWN || move_state==LEFT_DOWN || move_state==RIGHT_DOWN) && mp_players[i].air_velocity>=0 && !mp_players[i].SWIMMING){
                //Check to see if there is a y-axis collision.
                //We are checking only the bottom row of the pixels for the player against only the top row of the pixels for the platform.
                if(collision_check(x,y,w,h/12,mp_players[i].x,2+mp_players[i].y+mp_players[i].h-(mp_players[i].h/12),mp_players[i].w,2+mp_players[i].h/12)){
                    //We set the player's y value to the y value of this platform - the player's height, so the player will now be right on top of this platform.
                    mp_players[i].y=y-mp_players[i].h;
                    //If the player was in the air, they will no longer be.
                    mp_players[i].IN_AIR=false;
                    //The player has been moved, so handle collision.
                    mp_players[i].handle_events();
                }
            }
        }
    }
}

void Moving_Platform::animate(){
    //Increment the frame counters.
    frame_counter++;
    frame_counter_idle++;

    if((move_state!=DOWN && frame_counter>=10) || (move_state==DOWN && frame_counter>=2)){
        frame_counter=0;

        //Now increment the frame.
        frame++;

        //
        if(frame>MOVING_PLATFORM_SPRITES-1){
            frame=0;
        }
    }

    if(frame_counter_idle>=20){
        frame_counter_idle=0;

        //Now increment the frame.
        frame_idle++;

        //
        if(frame_idle>MOVING_PLATFORM_IDLE_SPRITES-1){
            frame_idle=0;
        }
    }
}

void Moving_Platform::render(bool mirrored){
    //If the platform is in camera bounds, render the platform.
    if(x>=player.camera_x-w && x<=player.camera_x+player.camera_w && y>=player.camera_y-h && y<=player.camera_y+player.camera_h){
        double render_x=x;
        double render_y=y;
        if(mirrored && touching_mirror(x+MIRROR_DISTANCE_X,y+MIRROR_DISTANCE_Y,w,h)){
            render_x+=MIRROR_DISTANCE_X;
            render_y+=MIRROR_DISTANCE_Y;
        }

        //If the platform is moving.
        if(move_state!=0){
            if(facing==LEFT){
                render_sprite((int)((int)render_x-5-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),image.sprite_sheet_moving_platform,&sprites_moving_platform_left[frame]);
            }
            else if(facing==RIGHT){
                render_sprite((int)((int)render_x-5-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),image.sprite_sheet_moving_platform,&sprites_moving_platform_right[frame]);
            }
            else if(facing==UP){
                render_sprite((int)((int)render_x-5-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),image.sprite_sheet_moving_platform,&sprites_moving_platform_up[frame]);
            }
            else if(facing==DOWN){
                render_sprite((int)((int)render_x-5-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),image.sprite_sheet_moving_platform,&sprites_moving_platform_down[frame]);
            }
        }
        //If the platform is not moving.
        else{
            render_sprite((int)((int)render_x-5-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),image.sprite_sheet_moving_platform,&sprites_moving_platform_idle[frame_idle]);
        }
    }
}
