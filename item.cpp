/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "item.h"
#include "random_number_generator.h"
#include "world.h"
#include "collision.h"
#include "render.h"
#include "mirror.h"
#include "distance.h"

using namespace std;

Item::Item(double get_x,double get_y,bool get_IN_AIR,short get_type,int get_goal_level_to_load,bool get_goal_secret,int min_move,int max_move,int min_jump,int max_jump,bool can_start_falling,short get_counter_vacuum,int get_score_bonus){
    int random=random_range(0,1);
    if(random==0){
        move_state=LEFT;
    }
    else{
        move_state=RIGHT;
    }

    run_speed=0.0;
    max_speed=0.1*random_range(min_move,max_move);
    air_drag=0.125;
    air_drag_divisor=256.0;
    air_accel=1.0;
    air_decel=0.5;

    SWIMMING=false;

    max_swim_speed=6.0;
    max_buoyancy=2.0;
    float_speed=0.0;
    swim_acceleration=0.1;

    IN_AIR=get_IN_AIR;

    counter_cannot_be_vacuumed=get_counter_vacuum;

    score_bonus=get_score_bonus;

    if(can_start_falling){
        random=random_range(0,99);
        if(random<75){
            random=-1;
        }
    }
    else{
        random=-1;
    }
    air_velocity=(0.1*random_range(min_jump,max_jump))*random;

    gravity_max=10.0;
    gravity=0.5;

    type=get_type;

    exists=true;

    checkpoint_reached=false;
    checkpoint_unreached=false;

    goal_level_to_load=get_goal_level_to_load;

    goal_secret=get_goal_secret;

    x=get_x;
    y=get_y;

    color=COLOR_WHITE;

    if(type<=ITEM_NONE){
        w=32;
        h=32;
    }
    else if(type==ITEM_LEAF){
        w=ITEM_W;
        h=ITEM_H;
    }
    else if(type==ITEM_CHEESE){
        w=64;
        h=64;
    }
    else if(type==ITEM_AMMO){
        w=32;
        h=32;
    }
    else if(type>=ITEM_SPAWNPOINT && type<=ITEM_ENDPOINT){
        w=32;
        h=96;
    }
    else if(type>=ITEM_SWIMMING_GEAR && type<=ITEM_SINK){
        w=32;
        h=32;
    }
    else if(type==ITEM_J_BALLOON){
        w=32;
        h=32;
    }
    else if(type==ITEM_AMMO_BARREL){
        w=34;
        h=48;
    }
    else if(type==ITEM_CANDY){
        w=16;
        h=16;
        set_color();
    }
    else if(type>=ITEM_KEY_GRAY && type<=ITEM_J_WING){
        w=32;
        h=32;
    }

    frame=0;
    frame_counter=0;
}

void Item::set_color(){
    short random;

    if(player.option_holiday_cheer && holiday==HOLIDAY_NEW_YEARS_DAY){
        random=random_range(0,3);

        if(random==0){
            color=COLOR_CHAMPAGNE;
        }
        else if(random==1){
            color=COLOR_WINE;
        }
        else if(random==2){
            color=COLOR_WHITE;
        }
        else if(random==3){
            color=COLOR_BROWN;
        }
    }
    else if(player.option_holiday_cheer && holiday==HOLIDAY_VALENTINES){
        random=random_range(0,4);

        if(random==0){
            color=COLOR_PINK;
        }
        else if(random==1){
            color=COLOR_ROSE;
        }
        else if(random==2){
            color=COLOR_SHOCKING_PINK;
        }
        else if(random==3){
            color=COLOR_RED;
        }
        else if(random==4){
            color=COLOR_WHITE;
        }
    }
    else if(player.option_holiday_cheer && holiday==HOLIDAY_ST_PATRICKS){
        random=random_range(0,3);

        if(random==0){
            color=COLOR_GREEN;
        }
        else if(random==1){
            color=COLOR_JUNGLE;
        }
        else if(random==2){
            color=COLOR_SPRING;
        }
        else if(random==3){
            color=COLOR_GOLD;
        }
    }
    else if(player.option_holiday_cheer && holiday==HOLIDAY_EASTER){
        random=random_range(0,6);

        if(random==0){
            color=COLOR_WHITE;
        }
        else if(random==1){
            color=COLOR_YELLOW_PASTEL;
        }
        else if(random==2){
            color=COLOR_ORANGE_PASTEL;
        }
        else if(random==3){
            color=COLOR_PINK;
        }
        else if(random==4){
            color=COLOR_PURPLE_PASTEL;
        }
        else if(random==5){
            color=COLOR_SKY_BLUE;
        }
        else if(random==6){
            color=COLOR_SPRING;
        }
    }
    else if(player.option_holiday_cheer && holiday==HOLIDAY_INDEPENDENCE){
        random=random_range(0,2);

        if(random==0){
            color=COLOR_RED;
        }
        else if(random==1){
            color=COLOR_WHITE;
        }
        else if(random==2){
            color=COLOR_BLUE;
        }
    }
    else if(player.option_holiday_cheer && holiday==HOLIDAY_HALLOWEEN){
        random=random_range(0,1);

        if(random==0){
            color=COLOR_PUMPKIN;
        }
        else if(random==1){
            color=COLOR_BLACK;
        }
    }
    else if(player.option_holiday_cheer && holiday==HOLIDAY_THANKSGIVING){
        random=random_range(0,3);

        if(random==0){
            color=random_range(COLOR_BROWN,COLOR_CHAMPAGNE);
        }
        else if(random==1){
            color=COLOR_YELLOW;
        }
        else if(random==2){
            color=COLOR_PUMPKIN;
        }
        else if(random==3){
            color=COLOR_WINE;
        }
    }
    else if(player.option_holiday_cheer && holiday==HOLIDAY_CHRISTMAS){
        random=random_range(0,2);

        if(random==0){
            color=COLOR_RED;
        }
        else if(random==1){
            color=COLOR_DARK_GREEN;
        }
        else if(random==2){
            color=COLOR_WHITE;
        }
    }
    else if(player.option_holiday_cheer && holiday==HOLIDAY_NEW_YEARS_EVE){
        random=random_range(0,5);

        if(random==0){
            color=COLOR_CHAMPAGNE;
        }
        else if(random==1){
            color=COLOR_RED;
        }
        else if(random==2){
            color=COLOR_WHITE;
        }
        else if(random==3){
            color=COLOR_BLUE;
        }
        else if(random==4){
            color=COLOR_GREEN;
        }
        else if(random==5){
            color=COLOR_YELLOW;
        }
    }
    else{
        random=random_range(0,11);

        if(random==0){
            color=COLOR_RED;
        }
        else if(random==1){
            color=COLOR_VIOLET;
        }
        else if(random==2){
            color=COLOR_BLUE;
        }
        else if(random==3){
            color=COLOR_PUMPKIN;
        }
        else if(random==4){
            color=COLOR_SHOCKING_PINK;
        }
        else if(random==5){
            color=COLOR_INDIGO;
        }
        else if(random==6){
            color=COLOR_YELLOW;
        }
        else if(random==7){
            color=COLOR_GREEN;
        }
        else if(random==8){
            color=COLOR_BLACK;
        }
        else if(random==9){
            color=COLOR_ORANGE;
        }
        else if(random==10){
            color=COLOR_DARK_RED;
        }
        else if(random==11){
            color=COLOR_ROSE;
        }
    }
}

int Item::get_nearest_player(){
    int nearest_player=-1;
    double nearest_distance=-1.0;

    for(int i=-1;i<(int)mp_players.size();i++){
        double PLAYER_X=player.x;
        double PLAYER_Y=player.y;
        bool PLAYER_DYING=player.DYING;
        bool PLAYER_BUBBLE_MODE=player.bubble_mode;
        if(i>=0){
            PLAYER_X=mp_players[i].x;
            PLAYER_Y=mp_players[i].y;
            PLAYER_DYING=mp_players[i].DYING;
            PLAYER_BUBBLE_MODE=mp_players[i].bubble_mode;
        }

        if(!PLAYER_DYING && !PLAYER_BUBBLE_MODE){
            double x1=x;
            double y1=y;
            double x2=PLAYER_X;
            double y2=PLAYER_Y;
            double this_distance=sqrt(((x2-x1)*(x2-x1))+((y2-y1)*(y2-y1)));

            if(nearest_distance==-1.0 || this_distance<nearest_distance){
                nearest_player=i;
                nearest_distance=this_distance;
            }
        }
    }

    return nearest_player;
}

double Item::get_target_direction(double target_x,double target_y,double target_w,double target_h){
    double target_angle=0.0;
    int rotation_target_x=0;
    int rotation_target_y=0;
    double x2=0.0;
    double y2=0.0;
    double x1=0.0;
    double y1=0.0;
    double x_component=0.0;
    double y_component=0.0;

    rotation_target_x=target_x+target_w/2-player.camera_x;
    rotation_target_y=target_y+target_h/2-player.camera_y;

    x2=(x+w/2-player.camera_x)+(rotation_target_x-(x+w/2-player.camera_x));
    y2=rotation_target_y+((y+h/2-player.camera_y)-rotation_target_y);

    x1=x+w/2-player.camera_x;
    y1=y+h/2-player.camera_y;

    x_component=sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
    y_component=sqrt(((rotation_target_x-x2)*(rotation_target_x-x2))+((rotation_target_y-y2)*(rotation_target_y-y2)));

    if(rotation_target_x<(x+w/2-player.camera_x)){
        x_component*=-1;
    }

    target_angle=atan2(y_component,x_component)*(180/ENGINE_MATH_PI);

    if(rotation_target_y>(y+h/2-player.camera_y)){
        target_angle=180+(180-target_angle);
    }

    return target_angle;
}

void Item::move(){
    if(exists){
        bool events_handled=false;

        if(counter_cannot_be_vacuumed>0){
            counter_cannot_be_vacuumed--;
        }

        int player_index=get_nearest_player();
        bool PLAYER_DYING=player.DYING;
        bool PLAYER_BUBBLE_MODE=player.bubble_mode;
        double PLAYER_X=player.x;
        double PLAYER_Y=player.y;
        double PLAYER_W=player.w;
        double PLAYER_H=player.h;
        if(player_index>=0){
            PLAYER_DYING=mp_players[player_index].DYING;
            PLAYER_BUBBLE_MODE=mp_players[player_index].bubble_mode;
            PLAYER_X=mp_players[player_index].x;
            PLAYER_Y=mp_players[player_index].y;
            PLAYER_W=mp_players[player_index].w;
            PLAYER_H=mp_players[player_index].h;
        }

        int candy_vac_range=CANDY_VACUUM_RANGE;
        if(player.get_upgrade_state("candy_vac")){
            candy_vac_range*=2;
        }

        //If the shot is within range of the player's candy vacuum ability
        if(!PLAYER_DYING && !PLAYER_BUBBLE_MODE && counter_cannot_be_vacuumed==0 && type==ITEM_CANDY && fabs(PLAYER_X-x)<candy_vac_range && fabs(PLAYER_Y-y)<candy_vac_range){
            //If the item is not moving, the vacuum should set it falling again.
            if(!IN_AIR){
                IN_AIR=true;
            }

            //The vacuum always stops the item's x-axis movement and resets its y-axis speeds.
            run_speed=0.0;
            air_velocity=0;
            float_speed=0.0;

            SWIMMING=false;

            double suck_speed=CANDY_VACUUM_SPEED;
            double distance_to_player=distance_between_points(x,y,PLAYER_X,PLAYER_Y);
            if(distance_to_player==0.0){
                distance_to_player=0.001;
            }
            double proximity_boost=suck_speed/(distance_to_player/(TILE_SIZE/2.0));
            if(distance_to_player>(double)candy_vac_range*0.75){
                proximity_boost=0.0;
            }
            if(proximity_boost>24.0){
                proximity_boost=24.0;
            }
            suck_speed+=proximity_boost;

            double suck_chunk;
            if(fabs(suck_speed)<pixel_safety_x){
                suck_chunk=fabs(suck_speed);
            }
            else{
                suck_chunk=pixel_safety_x;
            }

            double angle=get_target_direction(PLAYER_X,PLAYER_Y,PLAYER_W,PLAYER_H);

            for(double i=fabs(suck_speed);i>0;){
                //If we have suck_chunk or more pixels left to move,
                //we will move suck_chunk pixels, call handle_events(), and loop back up here.

                //Or, if we have less than suck_chunk pixels left to move,
                //we will move whatever pixels we have left and call handle_events() once more.
                if(i<suck_chunk){
                    suck_chunk=i;
                    i=0;
                }

                //Move.
                x+=suck_chunk*(cos(angle*(ENGINE_MATH_PI/180)));
                y+=suck_chunk*-(sin(angle*(ENGINE_MATH_PI/180)));

                //If we still have pixels left to move.
                if(i!=0){
                    i-=suck_chunk;
                }

                //We set this to true even though they weren't, so events won't be handled this frame.
                events_handled=true;
            }
        }
        else{
            if(IN_AIR || SWIMMING){
                double run_chunk;
                if(fabs(run_speed)<pixel_safety_x){
                    run_chunk=fabs(run_speed);
                }
                else{
                    run_chunk=pixel_safety_x;
                }

                for(double i=fabs(run_speed);i>0;){
                    //If we have run_chunk or more pixels left to move,
                    //we will move run_chunk pixels, call handle_events(), and loop back up here.

                    //Or, if we have less than run_chunk pixels left to move,
                    //we will move whatever pixels we have left and call handle_events() once more.
                    if(i<run_chunk){
                        run_chunk=i;
                        i=0;
                    }

                    //Move.
                    if(run_speed<0.0){
                        run_chunk*=-1;
                    }
                    x+=run_chunk;
                    if(run_speed<0.0){
                        run_chunk*=-1;
                    }

                    //If we still have pixels left to move.
                    if(i!=0){
                        i-=run_chunk;
                    }

                    handle_events();
                    events_handled=true;
                }

                if(!SWIMMING){
                    //Handle acceleration.
                    if(run_speed<max_speed && run_speed>max_speed*-1){
                        if(move_state==LEFT && run_speed<=0.0){
                            run_speed-=air_accel;
                        }
                        else if(move_state==RIGHT && run_speed>=0.0){
                            run_speed+=air_accel;
                        }
                        if(run_speed>max_speed){
                            run_speed=max_speed;
                        }
                        else if(run_speed<max_speed*-1){
                            run_speed=max_speed*-1;
                        }
                    }
                    //Handle deceleration.
                    if(move_state==LEFT && run_speed>0.0){
                        if(run_speed-air_decel<0.0){
                            run_speed=0.0-air_decel;
                        }
                        else{
                            run_speed-=air_decel;
                        }
                    }
                    else if(move_state==RIGHT && run_speed<0.0){
                        if(run_speed+air_decel>0.0){
                            run_speed=air_decel;
                        }
                        else{
                            run_speed+=air_decel;
                        }
                    }
                    //Handle air drag.
                    if(air_velocity<0.0 && air_drag!=0.0){
                        if(run_speed<0.0){
                            run_speed+=(fabs(run_speed)/air_drag)/air_drag_divisor;
                        }
                        else if(run_speed>0.0){
                            run_speed-=(fabs(run_speed)/air_drag)/air_drag_divisor;
                        }
                    }
                }

                if(!SWIMMING){
                    double air_chunk;
                    if(fabs(air_velocity)<pixel_safety_y){
                        air_chunk=fabs(air_velocity);
                    }
                    else{
                        air_chunk=pixel_safety_y;
                    }

                    for(double i=fabs(air_velocity);i>0;){
                        //If we have air_chunk or more pixels left to move,
                        //we will move air_chunk pixels, call handle_events(), and loop back up here.

                        //Or, if we have less than air_chunk pixels left to move,
                        //we will move whatever pixels we have left and call handle_events() once more.
                        if(i<air_chunk){
                            air_chunk=i;
                            i=0;
                        }

                        //Move.
                        if(air_velocity<0){
                            y-=air_chunk;
                        }
                        else if(air_velocity>0){
                            y+=air_chunk;
                        }

                        //If we still have pixels left to move.
                        if(i!=0){
                            i-=air_chunk;
                        }

                        handle_events();
                        events_handled=true;
                    }

                    //Then, we handle gravity, which will affect the next tick.

                    if(air_velocity<gravity_max){
                        air_velocity+=gravity;
                    }
                    if(air_velocity>gravity_max){
                        air_velocity=gravity_max;
                    }
                }
                else{
                    double float_chunk;
                    if(fabs(float_speed)<pixel_safety_y){
                        float_chunk=fabs(float_speed);
                    }
                    else{
                        float_chunk=pixel_safety_y;
                    }

                    for(double i=fabs(float_speed);i>0;){
                        //If we have float_chunk or more pixels left to move,
                        //we will move float_chunk pixels, call handle_events(), and loop back up here.

                        //Or, if we have less than float_chunk pixels left to move,
                        //we will move whatever pixels we have left and call handle_events() once more.
                        if(i<float_chunk){
                            float_chunk=i;
                            i=0;
                        }

                        //Move.
                        if(float_speed<0){
                            float_chunk*=-1;
                        }

                        y+=float_chunk;

                        if(float_speed<0){
                            float_chunk*=-1;
                        }

                        //If we still have pixels left to move.
                        if(i!=0){
                            i-=float_chunk;
                        }

                        handle_events();
                        events_handled=true;
                    }
                }

                //*******************************//
                // Handle swimming acceleration: //
                //*******************************//

                //If the player is swimming but isn't currently moving on the y-axis.
                if(SWIMMING && move_state!=UP && move_state!=LEFT_UP && move_state!=RIGHT_UP && move_state!=DOWN && move_state!=LEFT_DOWN && move_state!=RIGHT_DOWN){
                    //Float upwards.
                    if(float_speed>-max_buoyancy){
                        //If the time in water is long enough.
                        if(timer_time_in_water.get_ticks()>=500){
                            //Stop the timer so the player's buoyancy acceleration will slow to the standard rate.
                            timer_time_in_water.stop();
                        }

                        //If the timer is not running, buoyancy acceleration is normal.
                        if(!timer_time_in_water.is_started()){
                            float_speed-=swim_acceleration/4;
                        }
                        //If the timer is running, buoyancy acceleration is increased.
                        else if(timer_time_in_water.is_started()){
                            float_speed-=swim_acceleration*4;
                        }

                        if(float_speed<-max_buoyancy){
                            float_speed=-max_buoyancy;
                        }
                        if(float_speed<-max_swim_speed){
                            float_speed=-max_swim_speed;
                        }
                    }
                }
                //Slowly stop swimming on the x-axis.
                if(SWIMMING && move_state!=LEFT && move_state!=LEFT_DOWN && move_state!=LEFT_UP && move_state!=RIGHT && move_state!=RIGHT_DOWN && move_state!=RIGHT_UP){
                    if(run_speed>0){
                        run_speed-=swim_acceleration;
                        if(run_speed<0){
                            run_speed=0;
                        }
                    }
                    else if(run_speed<0){
                        run_speed+=swim_acceleration;
                        if(run_speed>0){
                            run_speed=0;
                        }
                    }
                }
                //If the player is swimming and is moving.
                //Don't allow the player to swim beyond the maximum swim speed.
                if(SWIMMING && move_state!=0){
                    //If the player is moving to the left.
                    if(move_state==LEFT || move_state==LEFT_DOWN || move_state==LEFT_UP){
                        run_speed-=swim_acceleration;
                        if(run_speed<-max_swim_speed){
                            run_speed=-max_swim_speed;
                        }
                    }
                    //If the player is moving to the right.
                    else if(move_state==RIGHT || move_state==RIGHT_DOWN || move_state==RIGHT_UP){
                        run_speed+=swim_acceleration;
                        if(run_speed>max_swim_speed){
                            run_speed=max_swim_speed;
                        }
                    }

                    //If the player is moving up.
                    if(move_state==UP || move_state==LEFT_UP || move_state==RIGHT_UP){
                        float_speed-=swim_acceleration;
                        if(float_speed<-max_swim_speed){
                            float_speed=-max_swim_speed;
                        }
                    }
                    //If the player is moving down.
                    else if(move_state==DOWN || move_state==LEFT_DOWN || move_state==RIGHT_DOWN){
                        float_speed+=swim_acceleration;
                        if(float_speed>max_swim_speed){
                            float_speed=max_swim_speed;
                        }
                    }
                }
            }
        }

        if(!events_handled){
            handle_events();
        }
    }
}

void Item::handle_events(){
    //If the item exists, handle its collision detection.
    if(exists){
        //First, we check for collisions with tiles, but only if the item is still in the air.
        if(IN_AIR || SWIMMING){
            //The current tile location for the item.
            int item_current_x=(int)((int)x/TILE_SIZE);
            int item_current_y=(int)((int)y/TILE_SIZE);

            int check_x_start=item_current_x-4;
            int check_x_end=item_current_x+4;
            int check_y_start=item_current_y-4;
            int check_y_end=item_current_y+4;

            //First, we check for collisions with tiles.

            for(int int_y=check_y_start;int_y<check_y_end;int_y++){
                for(int int_x=check_x_start;int_x<check_x_end;int_x++){
                    if(int_x>=0 && int_x<=(level.level_x/TILE_SIZE)-1 && int_y>=0 && int_y<=(level.level_y/TILE_SIZE)-1){
                        //If the tile is a non-water non-solid tile.
                        if(SWIMMING && level.tile_array[int_x][int_y].solidity==TILE_SOLIDITY_PASSABLE && level.tile_array[int_x][int_y].special!=TILE_SPECIAL_WATER){
                            if(collision_check(x,y,w,h,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE)){
                                SWIMMING=false;
                                IN_AIR=false;
                                return;
                            }
                        }

                        //If the tile is a water tile.
                        if(!SWIMMING && level.tile_array[int_x][int_y].special==TILE_SPECIAL_WATER){
                            if(collision_check(x,y,w,h,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE)){
                                float_speed=max_buoyancy;
                                //Move the actor just below the top of this water tile, so he doesn't "rehit" the air above the water.
                                y=level.tile_array[int_x][int_y].y+0.1;

                                //Start timing how long the actor has been in the water.
                                timer_time_in_water.start();

                                SWIMMING=true;
                                air_velocity=0;

                                ///I can add a check here later for the actor being within the camera bounds.
                                ///This will ensure that npcs not on camera won't make performance-eating effects.
                                //As long as the elements of the vector do not exceed the limit.
                                if(vector_effect_water_splash.size()<player.option_effect_limit){
                                    vector_effect_water_splash.push_back(Effect_Water_Splash(x-3,y-26+2));
                                    play_positional_sound(sound_system.splash_in,x,y);
                                }
                            }
                        }

                        //If the tile is a solid tile.
                        if(level.tile_array[int_x][int_y].solidity==TILE_SOLIDITY_SOLID){
                            //First, we check for an x-axis collision. For x-axis collision, we are checking this part of the item's rectangle:
                            //|----|
                            //xxxxxx
                            //xxxxxx
                            //|----|
                            if(collision_check(x,y+(h/4),w,h/2,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE)){
                                //If the left of the item is left of the tile's left side, we know the item is hitting the left side of the tile.
                                if(x<level.tile_array[int_x][int_y].x){
                                    //We set the item's x value to the x value of the tile - the item's width, so the item will now be on the left side of the tile.
                                    x=level.tile_array[int_x][int_y].x-w;

                                    //Since the item just bumped into the left side of the tile, it must be moving right, so start it moving left.
                                    move_state=LEFT;

                                    run_speed*=-1.0;
                                }
                                //If the right side of the item is right of the tile's right side, we know the item is hitting the right side of the tile.
                                else if(x+w>level.tile_array[int_x][int_y].x+TILE_SIZE){
                                    //We set the item's x value to the right side of the tile.
                                    x=level.tile_array[int_x][int_y].x+TILE_SIZE;

                                    //Since the item just bumped into the right side of the tile, it must be moving left, so start it moving right.
                                    move_state=RIGHT;

                                    run_speed*=-1.0;
                                }
                            }

                            //Now we check to see if there is a y-axis collision. The section of the item that collides with the solid tile matters.
                            //We first check for a collision between the top of the item and the bottom of the tile.
                            //xxxxxx
                            //|----|
                            //|----|
                            //|----|
                            //We also check that the item is moving upwards.
                            if((air_velocity<0.0 || float_speed<0.0) && collision_check(x,y,w,h,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y+TILE_SIZE/2,TILE_SIZE,TILE_SIZE/2)){
                                //We set the item's y value to the bottom of the tile.
                                y=level.tile_array[int_x][int_y].y+TILE_SIZE;
                                //We set the air velocity to 0, so that the item immediately begins falling once it bumps its top on a tile.
                                air_velocity=0.0;
                                float_speed=0.0;
                            }

                            //Next, we check for a collision between the bottom of the item and the top of the tile.
                            //|----|
                            //|----|
                            //|----|
                            //xxxxxx
                            //We only do this if the item is falling, so that it doesn't get warped to the top of the tile if jumping into the side of it.
                            if((air_velocity>=0.0 || float_speed>=0.0) && collision_check(x,y+h-(h/2),w,h/2,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE/2)){
                                //We set the item's y value to the y value of the tile - the item's height, so the item will now be right on top of the tile.
                                y=level.tile_array[int_x][int_y].y-h;
                                //We set IN_AIR to false, so that if the item was in the air, it will no longer be.
                                IN_AIR=false;
                                air_velocity=0.0;
                                float_speed=0.0;
                            }
                        }

                        //If the tile is a solid-only-on-top tile, and the item is falling.
                        if((level.tile_array[int_x][int_y].solidity==TILE_SOLIDITY_CLOUD || level.tile_array[int_x][int_y].special==TILE_SPECIAL_CLIMBABLE_TOP) && air_velocity>=0){
                            //First, we check to see if there is a x-axis collision.
                            //|----|
                            //xxxxxx
                            //xxxxxx
                            //|----|
                            //If there is an x-axis collision while the item is falling, don't check for a y-axis collision this time.
                            //This prevents a falling item from warping back to the top of this tile, which would be caused by the below if() statement alone.
                            //This if() statement just exists to create another condition for the below if() statement.
                            if(collision_check(x,y+(h/4),w,h/2,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE)){
                            }

                            //Now, if there was no x-axis collision, we check to see if there is a y-axis collision.
                            //For y-axis hit detection we check this part of the item's rectangle:
                            //|----|
                            //|----|
                            //|----|
                            //xxxxxx
                            //As shown above, we are checking only the bottom row of the pixels for the item against only the top row of the pixels for the tile.
                            if(collision_check(x,y+h-(h/2),w,h/2,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE/2)){
                                //We set the item's y value to the y value of the tile - the item's height, so the item will now be right on top of the tile.
                                y=level.tile_array[int_x][int_y].y-h;
                                //We set IN_AIR to false, since the item was in the air, so now it will no longer be.
                                IN_AIR=false;
                                air_velocity=0;
                            }
                        }
                    }
                }
            }

            //Keep the item in the level map's bounds.
            //If an item hits a side of the level, it should reverse its direction on the x-axis.
            if(x<0){
                x=0;
                //Reverse the item's direction.
                move_state=RIGHT;
            }
            if(x+w>level.level_x){
                x=level.level_x-w;
                //Reverse the item's direction.
                move_state=LEFT;
            }
            if(y<0){
                y=0;
                //We set the air velocity to 0, so that the item immediately begins falling once it bumps its top on the top of the level.
                air_velocity=0;
            }
            if(y+h>level.level_y){
                y=level.level_y-h;
                IN_AIR=false;
                air_velocity=0;
            }
        }
    }
}

void Item::animate(){
    //All items aside from the checkpoint.
    if(type!=ITEM_CHECKPOINT){
        frame_counter++;

        if(frame_counter>=20){

            frame_counter=0;

            frame++;

            if(type==ITEM_LEAF && frame>ITEM_SPRITES_LEAF-1){
                frame=0;
            }
            else if(type==ITEM_CHEESE && frame>ITEM_SPRITES_CHEESE-1){
                frame=0;
            }
            else if(type==ITEM_AMMO && frame>ITEM_SPRITES_AMMO-1){
                frame=0;
            }
            else if(type==ITEM_SPAWNPOINT && frame>ITEM_SPRITES_SPAWNPOINT-1 &&
                    (player.game_mode==GAME_MODE_SP_ADVENTURE || player.game_mode==GAME_MODE_MP_ADVENTURE)){
                frame=0;
            }
            else if(type==ITEM_SPAWNPOINT && frame>SURVIVAL_SPRITES_SPAWN-1 &&
                    (player.game_mode==GAME_MODE_SP_SURVIVAL || player.game_mode==GAME_MODE_MP_SURVIVAL)){
                frame=1;
            }
            else if(type==ITEM_ENDPOINT && !goal_secret && frame>ITEM_SPRITES_ENDPOINT-1){
                frame=0;
            }
            else if(type==ITEM_ENDPOINT && goal_secret && frame>ITEM_SPRITES_ENDPOINT_SECRET-1){
                frame=0;
            }
            else if(type==ITEM_SWIMMING_GEAR && frame>ITEM_SPRITES_SWIMMING_GEAR-1){
                frame=0;
            }
            else if(type==ITEM_KEY_RED && frame>ITEM_SPRITES_KEY_RED-1){
                frame=0;
            }
            else if(type==ITEM_KEY_BLUE && frame>ITEM_SPRITES_KEY_BLUE-1){
                frame=0;
            }
            else if(type==ITEM_KEY_GREEN && frame>ITEM_SPRITES_KEY_GREEN-1){
                frame=0;
            }
            else if(type==ITEM_KEY_YELLOW && frame>ITEM_SPRITES_KEY_YELLOW-1){
                frame=0;
            }
            else if(type==ITEM_KEY_ORANGE && frame>ITEM_SPRITES_KEY_ORANGE-1){
                frame=0;
            }
            else if(type==ITEM_KEY_PURPLE && frame>ITEM_SPRITES_KEY_PURPLE-1){
                frame=0;
            }
            else if(type==ITEM_TOWEL && frame>ITEM_SPRITES_TOWEL-1){
                frame=0;
            }
            else if(type==ITEM_KEY_GRAY && frame>ITEM_SPRITES_KEY_GRAY-1){
                frame=0;
            }
            else if(type==ITEM_KEY_BROWN && frame>ITEM_SPRITES_KEY_BROWN-1){
                frame=0;
            }
            else if(type==ITEM_KEY_BLACK && frame>ITEM_SPRITES_KEY_BLACK-1){
                frame=0;
            }
            else if(type==ITEM_KEY_PINK && frame>ITEM_SPRITES_KEY_PINK-1){
                frame=0;
            }
            else if(type==ITEM_KEY_CYAN && frame>ITEM_SPRITES_KEY_CYAN-1){
                frame=0;
            }
            else if(type==ITEM_SINK && frame>ITEM_SPRITES_SINK-1){
                frame=0;
            }
            else if(type==ITEM_J_BALLOON && frame>ITEM_SPRITES_J_BALLOON-1){
                frame=0;
            }
            else if(type==ITEM_AMMO_BARREL && frame>ITEM_SPRITES_AMMO_BARREL-1){
                frame=0;
            }
            else if(type==ITEM_CANDY && frame>ITEM_SPRITES_CANDY-1){
                frame=0;
            }
            else if(type==ITEM_SUIT_DEADLY_WATER && frame>ITEM_SPRITES_SUIT_DEADLY_WATER-1){
                frame=0;
            }
            else if(type==ITEM_SUIT_SHARP && frame>ITEM_SPRITES_SUIT_SHARP-1){
                frame=0;
            }
            else if(type==ITEM_SUIT_BANANA && frame>ITEM_SPRITES_SUIT_BANANA-1){
                frame=0;
            }
            else if(type==ITEM_SHOT_HOMING && frame>ITEM_SPRITES_SHOT_HOMING-1){
                frame=0;
            }
            else if(type==ITEM_TRANSLATOR && frame>ITEM_SPRITES_TRANSLATOR-1){
                frame=0;
            }
            else if(type==ITEM_J_WING && frame>ITEM_SPRITES_J_WING-1){
                frame=0;
            }
        }
    }
    //If the type is checkpoint and it is currently animating.
    else if(type==ITEM_CHECKPOINT && (checkpoint_reached || checkpoint_unreached)){
        //Increment the frame counter.
        frame_counter++;

        //Handle animation.
        if(frame_counter>=1){
            frame_counter=0;

            //Now increment or decrement the frame.
            if(checkpoint_reached){
                frame++;
            }
            else if(checkpoint_unreached){
                frame--;
            }

            if(frame>ITEM_SPRITES_CHECKPOINT-1){
                frame=ITEM_SPRITES_CHECKPOINT-1;
                checkpoint_reached=false;
            }
            else if(frame<0){
                frame=0;
                checkpoint_unreached=false;
            }
        }
    }
}

SDL_Rect* Item::get_texture_clip(bool survival_spawn){
    if(type==ITEM_LEAF){
        return &sprites_item_leaf[frame];
    }
    else if(type==ITEM_CHEESE){
        return &sprites_item_cheese[frame];
    }
    else if(type==ITEM_AMMO){
        return &sprites_item_ammo[frame];
    }
    else if(type==ITEM_SPAWNPOINT){
        if(survival_spawn){
            if(player.game_mode==GAME_MODE_SP_ADVENTURE || player.game_mode==GAME_MODE_MP_ADVENTURE){
                return &sprites_item_spawnpoint[frame];
            }
            else if(player.game_mode==GAME_MODE_SP_SURVIVAL || player.game_mode==GAME_MODE_MP_SURVIVAL){
                if(player.survival_escape){
                    return &sprites_survival_spawn[frame];
                }
                else{
                    return &sprites_survival_spawn[0];
                }
            }
        }
    }
    else if(type==ITEM_CHECKPOINT){
        return &sprites_item_checkpoint[frame];
    }
    else if(type==ITEM_ENDPOINT){
        if(!goal_secret){
            return &sprites_item_endpoint[frame];
        }
        else{
            return &sprites_item_endpoint_secret[frame];
        }
    }
    else if(type==ITEM_SWIMMING_GEAR){
        return &sprites_item_swimming_gear[frame];
    }
    else if(type==ITEM_KEY_RED){
        return &sprites_item_key_red[frame];
    }
    else if(type==ITEM_KEY_BLUE){
        return &sprites_item_key_blue[frame];
    }
    else if(type==ITEM_KEY_GREEN){
        return &sprites_item_key_green[frame];
    }
    else if(type==ITEM_KEY_YELLOW){
        return &sprites_item_key_yellow[frame];
    }
    else if(type==ITEM_KEY_ORANGE){
        return &sprites_item_key_orange[frame];
    }
    else if(type==ITEM_KEY_PURPLE){
        return &sprites_item_key_purple[frame];
    }
    else if(type==ITEM_TOWEL){
        return &sprites_item_towel[frame];
    }
    else if(type==ITEM_KEY_GRAY){
        return &sprites_item_key_gray[frame];
    }
    else if(type==ITEM_KEY_BROWN){
        return &sprites_item_key_brown[frame];
    }
    else if(type==ITEM_KEY_BLACK){
        return &sprites_item_key_black[frame];
    }
    else if(type==ITEM_KEY_PINK){
        return &sprites_item_key_pink[frame];
    }
    else if(type==ITEM_KEY_CYAN){
        return &sprites_item_key_cyan[frame];
    }
    else if(type==ITEM_SINK){
        return &sprites_item_sink[frame];
    }
    else if(type==ITEM_J_BALLOON){
        return &sprites_item_j_balloon[frame];
    }
    else if(type==ITEM_AMMO_BARREL){
        return &sprites_item_ammo_barrel[frame];
    }
    else if(type==ITEM_CANDY){
        if(color!=COLOR_BLACK){
            return &sprites_item_candy[frame];
        }
        else{
            return &sprites_item_candy_black[frame];
        }
    }
    else if(type==ITEM_SUIT_DEADLY_WATER){
        return &sprites_item_suit_deadly_water[frame];
    }
    else if(type==ITEM_SUIT_SHARP){
        return &sprites_item_suit_sharp[frame];
    }
    else if(type==ITEM_SUIT_BANANA){
        return &sprites_item_suit_banana[frame];
    }
    else if(type==ITEM_SHOT_HOMING){
        return &sprites_item_shot_homing[frame];
    }
    else if(type==ITEM_TRANSLATOR){
        return &sprites_item_translator[frame];
    }
    else if(type==ITEM_J_WING){
        return &sprites_item_j_wing[frame];
    }

    return 0;
}

void Item::render(bool mirrored,bool survival_spawn){
    //If the item exists, render it.
    if(exists){
        //If the item is in camera bounds, render the item.
        if(x>=player.camera_x-w && x<=player.camera_x+player.camera_w && y>=player.camera_y-h && y<=player.camera_y+player.camera_h){
            double render_x=x;
            double render_y=y;
            if(mirrored && touching_mirror(x+MIRROR_DISTANCE_X,y+MIRROR_DISTANCE_Y,w,h)){
                render_x+=MIRROR_DISTANCE_X;
                render_y+=MIRROR_DISTANCE_Y;
            }

            //If the player is on the world map.
            if(player.on_worldmap()){
                //If the item is a valid world map item.
                if(type<ITEM_NONE){
                    ///render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),image.sprite_sheet_level_items,&sprites_items[type]);
                }
            }
            //If the player is in a level.
            else{
                if(type==ITEM_LEAF){
                    render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),image.sprite_sheet_items,get_texture_clip(survival_spawn),1.0,1.0,1.0,0.0,color);
                }
                else if(type==ITEM_CHEESE){
                    render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),image.sprite_sheet_items,get_texture_clip(survival_spawn),1.0,1.0,1.0,0.0,color);
                }
                else if(type==ITEM_AMMO){
                    render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),image.sprite_sheet_items,get_texture_clip(survival_spawn),1.0,1.0,1.0,0.0,color);
                }
                else if(type==ITEM_SPAWNPOINT){
                    if(survival_spawn){
                        if(player.game_mode==GAME_MODE_SP_ADVENTURE || player.game_mode==GAME_MODE_MP_ADVENTURE){
                            render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-32-(int)player.camera_y),image.sprite_sheet_level_items,get_texture_clip(survival_spawn),1.0,1.0,1.0,0.0,color);
                        }
                        else if(player.game_mode==GAME_MODE_SP_SURVIVAL || player.game_mode==GAME_MODE_MP_SURVIVAL){
                            if(player.survival_escape){
                                render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-32-(int)player.camera_y),image.survival_spawn,get_texture_clip(survival_spawn),1.0,1.0,1.0,0.0,color);
                            }
                            else{
                                render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-32-(int)player.camera_y),image.survival_spawn,get_texture_clip(survival_spawn),1.0,1.0,1.0,0.0,color);
                            }
                        }
                    }
                }
                else if(type==ITEM_CHECKPOINT){
                    render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),image.sprite_sheet_level_items,get_texture_clip(survival_spawn),1.0,1.0,1.0,0.0,color);
                }
                else if(type==ITEM_ENDPOINT){
                    if(!goal_secret){
                        render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),image.sprite_sheet_level_items,get_texture_clip(survival_spawn),1.0,1.0,1.0,0.0,color);
                    }
                    else{
                        render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),image.sprite_sheet_level_items,get_texture_clip(survival_spawn),1.0,1.0,1.0,0.0,color);
                    }
                }
                else if(type==ITEM_SWIMMING_GEAR){
                    render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),image.sprite_sheet_items,get_texture_clip(survival_spawn),1.0,1.0,1.0,0.0,color);
                }
                else if(type==ITEM_KEY_RED){
                    render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),image.sprite_sheet_items,get_texture_clip(survival_spawn),1.0,1.0,1.0,0.0,color);
                }
                else if(type==ITEM_KEY_BLUE){
                    render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),image.sprite_sheet_items,get_texture_clip(survival_spawn),1.0,1.0,1.0,0.0,color);
                }
                else if(type==ITEM_KEY_GREEN){
                    render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),image.sprite_sheet_items,get_texture_clip(survival_spawn),1.0,1.0,1.0,0.0,color);
                }
                else if(type==ITEM_KEY_YELLOW){
                    render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),image.sprite_sheet_items,get_texture_clip(survival_spawn),1.0,1.0,1.0,0.0,color);
                }
                else if(type==ITEM_KEY_ORANGE){
                    render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),image.sprite_sheet_items,get_texture_clip(survival_spawn),1.0,1.0,1.0,0.0,color);
                }
                else if(type==ITEM_KEY_PURPLE){
                    render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),image.sprite_sheet_items,get_texture_clip(survival_spawn),1.0,1.0,1.0,0.0,color);
                }
                else if(type==ITEM_TOWEL){
                    render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),image.sprite_sheet_items,get_texture_clip(survival_spawn),1.0,1.0,1.0,0.0,color);
                }
                else if(type==ITEM_KEY_GRAY){
                    render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),image.sprite_sheet_items,get_texture_clip(survival_spawn),1.0,1.0,1.0,0.0,color);
                }
                else if(type==ITEM_KEY_BROWN){
                    render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),image.sprite_sheet_items,get_texture_clip(survival_spawn),1.0,1.0,1.0,0.0,color);
                }
                else if(type==ITEM_KEY_BLACK){
                    render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),image.sprite_sheet_items,get_texture_clip(survival_spawn),1.0,1.0,1.0,0.0,color);
                }
                else if(type==ITEM_KEY_PINK){
                    render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),image.sprite_sheet_items,get_texture_clip(survival_spawn),1.0,1.0,1.0,0.0,color);
                }
                else if(type==ITEM_KEY_CYAN){
                    render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),image.sprite_sheet_items,get_texture_clip(survival_spawn),1.0,1.0,1.0,0.0,color);
                }
                else if(type==ITEM_SINK){
                    render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),image.sprite_sheet_items,get_texture_clip(survival_spawn),1.0,1.0,1.0,0.0,color);
                }
                else if(type==ITEM_J_BALLOON){
                    render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),image.sprite_sheet_items,get_texture_clip(survival_spawn),1.0,1.0,1.0,0.0,color);
                }
                else if(type==ITEM_AMMO_BARREL){
                    render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),image.sprite_sheet_items,get_texture_clip(survival_spawn),1.0,1.0,1.0,0.0,color);
                }
                else if(type==ITEM_CANDY){
                    if(color!=COLOR_BLACK){
                        render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),image.sprite_sheet_items,get_texture_clip(survival_spawn),1.0,1.0,1.0,0.0,color);
                    }
                    else{
                        render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),image.sprite_sheet_items,get_texture_clip(survival_spawn));
                    }
                }
                else if(type==ITEM_SUIT_DEADLY_WATER){
                    render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),image.sprite_sheet_items,get_texture_clip(survival_spawn),1.0,1.0,1.0,0.0,color);
                }
                else if(type==ITEM_SUIT_SHARP){
                    render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),image.sprite_sheet_items,get_texture_clip(survival_spawn),1.0,1.0,1.0,0.0,color);
                }
                else if(type==ITEM_SUIT_BANANA){
                    render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),image.sprite_sheet_items,get_texture_clip(survival_spawn),1.0,1.0,1.0,0.0,color);
                }
                else if(type==ITEM_SHOT_HOMING){
                    render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),image.sprite_sheet_items,get_texture_clip(survival_spawn),1.0,1.0,1.0,0.0,color);
                }
                else if(type==ITEM_TRANSLATOR){
                    render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),image.sprite_sheet_items,get_texture_clip(survival_spawn),1.0,1.0,1.0,0.0,color);
                }
                else if(type==ITEM_J_WING){
                    render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),image.sprite_sheet_items,get_texture_clip(survival_spawn),1.0,1.0,1.0,0.0,color);
                }
            }
        }
    }
}
