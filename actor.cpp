/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "actor.h"
#include "world.h"
#include "collision.h"
#include "counters.h"

using namespace std;

double Actor::return_x(){
    return x-hitbox_size_modifier;
}
double Actor::return_y(){
    return y-hitbox_size_modifier;
}
double Actor::return_w(){
    return w+hitbox_size_modifier*2;
}
double Actor::return_h(){
    return h+hitbox_size_modifier*2;
}

int Actor::get_extra_jumps_max(){
    if(is_player && player.get_upgrade_state("infinite_jumps")){
        return 1;
    }
    else if(j_wing){
        return 1;
    }
    else{
        return 0;
    }
}

void Actor::after_input(){
    double sound_x=x;
    double sound_y=y;
    double falloff=8.0;
    if(is_player){
        falloff=64.0;
    }

    if(SWIMMING && delta_move_state!=move_state && move_state!=NONE && counter_swim_sound==0 &&
       !(SWIM_CAN_JUMP && (move_state==UP || move_state==LEFT_UP || move_state==RIGHT_UP))){
        counter_swim_sound=SWIM_SOUND_TIME;

        //I'm disabling the swim sound for NPCs for now. It doesn't sound very good.
        if(is_player){
            play_positional_sound(sound_system.swim,sound_x,sound_y,falloff);
        }
    }

    delta_move_state=move_state;
}

void Actor::play_footstep_sound(){
    short random=random_range(0,99);

    sound_data* ptr_footstep=&sound_system.player_footstep;
    sound_data* ptr_footstep2=&sound_system.player_footstep2;

    double sound_x=x;
    double sound_y=y;
    if(is_player){
        if(which_mp_player==-1){
            ptr_footstep=player.ptr_player_footstep;
            ptr_footstep2=player.ptr_player_footstep2;
        }
        else{
            ptr_footstep=mp_players[which_mp_player].ptr_player_footstep;
            ptr_footstep2=mp_players[which_mp_player].ptr_player_footstep2;
        }
    }

    if(random<69){
        if(!water_running){
            play_positional_sound(*ptr_footstep,sound_x,sound_y);
        }
        else{
            play_positional_sound(sound_system.player_footstep_water,sound_x,sound_y);
        }
    }
    else{
        if(!water_running){
            play_positional_sound(*ptr_footstep2,sound_x,sound_y);
        }
        else{
            play_positional_sound(sound_system.player_footstep2_water,sound_x,sound_y);
        }
    }

    if(water_running){
        //As long as the elements of the vector do not exceed the limit.
        if(vector_effect_water_splash.size()<player.option_effect_limit){
            vector_effect_water_splash.push_back(Effect_Water_Splash(x-3,y+h-32+6));
        }
    }
}

void Actor::move_suck(short direction){
    //temp_run_speed holds the value of the "run speed" we will be using.
    double temp_run_speed=SUCK_SPEED;

    //This is the maximum number of pixels that the npc will move before calling handle_events().
    double run_chunk=fabs(temp_run_speed);

    for(double i=fabs(temp_run_speed);i>0;){
        //If we have run_chunk or more pixels left to move,
        //we will move run_chunk pixels, call handle_events(), and loop back up here.

        //Or, if we have less than run_chunk pixels left to move,
        //we will move whatever pixels we have left and call handle_events() once more.
        if(i<run_chunk){
            run_chunk=i;
            i=0;
        }

        //Move.
        if(direction==LEFT){
            x-=run_chunk;
        }
        else if(direction==RIGHT){
            x+=run_chunk;
        }

        //If we still have pixels left to move.
        if(i!=0){
            i-=run_chunk;
        }

        ///handle_events();
    }
}

void Actor::handle_counters(){
    if(counter_swim_sound>0){
        counter_swim_sound--;
    }

    if(underwater){
        if(counter_bubble>0){
            counter_bubble--;
        }
        if(counter_bubble<=0){
            if(!swimming_gear){
                counter_bubble=random_range(BUBBLE_TIME_MIN,BUBBLE_TIME_MAX);
            }
            else{
                counter_bubble=random_range(BUBBLE_SWIM_GEAR_TIME_MIN,BUBBLE_SWIM_GEAR_TIME_MAX);
            }

            //As long as the elements of the vector do not exceed the limit.
            if(vector_effect_water_bubble.size()<player.option_effect_limit){
                vector_effect_water_bubble.push_back(Effect_Water_Bubble(x+8,y));
            }
        }
    }

    if(underwater){
        //If the npc has no swimming gear, he loses oxygen.
        if(!swimming_gear){
            oxygen--;
            if(oxygen<0){
                oxygen=0;
            }
        }
    }
    //If not underwater.
    else{
        //Reset oxygen to the maximum.
        oxygen=oxygen_max_capacity;
    }
}

void Actor::fall_through_cloud(){
    on_cloud=false;

    //If the actor is on a moving platform that is moving down.
    if(moving_platform_y>0.0){
        air_velocity=(4.0/10.0+0.4)*gravity_max;
        if(air_velocity>gravity_max){
            air_velocity=gravity_max;
        }
        y+=moving_platform_y*5.0;
    }
    else if(moving_platform_y<0.0){
        air_velocity=0;
        y+=fabs(moving_platform_y)*4.0;
    }
    else{
        air_velocity=0;
        y+=10;
    }

    IN_AIR=true;

    frame_jump=0;
}

void Actor::set_solid_above(){
    solid_above=false;

    //The current tile location for the actor.
    int actor_current_x=(int)((int)x/TILE_SIZE);
    int actor_current_y=(int)((int)y/TILE_SIZE);

    //Check all tiles in a square around the actor.
    int check_x_start=actor_current_x-4;
    int check_x_end=actor_current_x+4;
    int check_y_start=actor_current_y-4;
    int check_y_end=actor_current_y+4;

    for(int int_x=check_x_start;int_x<check_x_end;int_x++){
        for(int int_y=check_y_start;int_y<check_y_end;int_y++){
            if(int_x>=0 && int_x<=(level.level_x/TILE_SIZE)-1 && int_y>=0 && int_y<=(level.level_y/TILE_SIZE)-1){
                Tile* tile=&level.tile_array[int_x][int_y];

                if(tile->solidity==TILE_SOLIDITY_SOLID && collision_check(x,y-h/2,w,h/2,tile->x,tile->y+TILE_SIZE/2,TILE_SIZE,TILE_SIZE/2)){
                    solid_above=true;

                    break;
                }
            }
        }
    }
}

void Actor::prepare_for_events(){
    //We always start by "forgetting" where the actor's feet are.
    //The rest of handle_events() will determine if the actor's feet are touching air, ground, or both, and handle that.
    touching_air=false;
    touching_ground=false;
    touching_sloped_ground_last_check=touching_sloped_ground;
    touching_sloped_ground=false;
    touched_slope_angle=0.0;

    //We always begin by setting the moving platform modifiers to 0.
    //If the actor is on a moving platform, it will be handled below.
    moving_platform_y=0.0;
    MOVING_PLATFORM_X_THIS_FRAME=false;
    MOVING_PLATFORM_IN_WATER=false;

    //We begin by assuming the actor is not standing on a cloud tile.
    //If the actor is standing on a cloud tile, it will be handled below.
    on_cloud=false;
    cloud_npc=false;

    if(is_player){
        player.riding_cow=false;
    }

    on_climbable_top_tile=false;

    //If the actor is swimming, start out assuming he is submerged underwater.
    if(SWIMMING){
        underwater=true;
    }
    //Otherwise, he is certainly not underwater.
    else{
        underwater=false;
    }

    water_running=false;
}

void Actor::handle_npc_platform(int i){
    //If the npc is acting as a platform.
    if(!SWIMMING && vector_npcs[i].act_as_platform){
        //Regardless of the actor's IN_AIR and air_velocity status, one other check needs to be done.
        //If the top row of this npc's pixels are one pixel below the actor, he is standing on this npc.
        if(collision_check(x,y+h,w,1,vector_npcs[i].x,vector_npcs[i].y,vector_npcs[i].w,1)){
            //Thus, we let the game know the actor is standing on some ground.
            touching_ground=true;

            //Set the actor's moving platform variables to the movement status of this npc.

            bool npc_notices_wall=vector_npcs[i].ai_notice_wall();

            //X-axis.
            if(!npc_notices_wall){
                moving_platform_x=vector_npcs[i].run_speed;
                MOVING_PLATFORM_X_THIS_FRAME=true;
            }
            //If the npc is not moving and not running into a wall.
            if(vector_npcs[i].run_speed==0.0 && !npc_notices_wall){
                moving_platform_x=vector_npcs[i].moving_platform_x;
                MOVING_PLATFORM_X_THIS_FRAME=true;
            }

            //Y-axis.
            if((vector_npcs[i].IN_AIR && vector_npcs[i].air_velocity<0.0) || vector_npcs[i].moving_platform_y<0.0){
                if(vector_npcs[i].IN_AIR && vector_npcs[i].air_velocity<0.0){
                    moving_platform_y=vector_npcs[i].air_velocity;
                }
                else if(vector_npcs[i].moving_platform_y<0.0){
                    moving_platform_y=vector_npcs[i].moving_platform_y;
                }
            }
            else if((vector_npcs[i].IN_AIR && vector_npcs[i].air_velocity>=0.0) || vector_npcs[i].moving_platform_y>0.0){
                if(vector_npcs[i].IN_AIR && vector_npcs[i].air_velocity>=0.0){
                    moving_platform_y=vector_npcs[i].air_velocity;
                }
                else if(vector_npcs[i].moving_platform_y>0.0){
                    moving_platform_y=vector_npcs[i].moving_platform_y;
                }
            }

            //The actor is now standing on a cloud tile.
            on_cloud=true;
            //The cloud "tile" is a rideable npc.
            cloud_npc=true;

            if(vector_npcs[i].SWIMMING){
                MOVING_PLATFORM_IN_WATER=true;
            }

            //If the actor is the player and the cow riding timer is not already started.
            if(is_player && !player.timer_riding_cows.is_started() && vector_npcs[i].type==NPC_COW){
                player.timer_riding_cows.start();
                player.riding_cow=true;
            }
        }
    }
}

void Actor::handle_player_platform(int i){
    if(!SWIMMING){
        double PLAYER_X=player.x;
        double PLAYER_Y=player.y;
        double PLAYER_WIDTH=player.w;
        double PLAYER_RUN_SPEED=player.run_speed;
        double PLAYER_MOVING_PLATFORM_X=player.moving_platform_x;
        double PLAYER_MOVING_PLATFORM_Y=player.moving_platform_y;
        double PLAYER_AIR_VELOCITY=player.air_velocity;
        bool PLAYER_IN_AIR=player.IN_AIR;
        bool PLAYER_SWIMMING=player.SWIMMING;
        if(i>=0){
            PLAYER_X=mp_players[i].x;
            PLAYER_Y=mp_players[i].y;
            PLAYER_WIDTH=mp_players[i].w;
            PLAYER_RUN_SPEED=mp_players[i].run_speed;
            PLAYER_MOVING_PLATFORM_X=mp_players[i].moving_platform_x;
            PLAYER_MOVING_PLATFORM_Y=mp_players[i].moving_platform_y;
            PLAYER_AIR_VELOCITY=mp_players[i].air_velocity;
            PLAYER_IN_AIR=mp_players[i].IN_AIR;
            PLAYER_SWIMMING=mp_players[i].SWIMMING;
        }

        //Regardless of the actor's IN_AIR and air_velocity status, one other check needs to be done.
        //If the top row of this npc's pixels are one pixel below the actor, he is standing on this npc.
        if(collision_check(x,y+h,w,1,PLAYER_X,PLAYER_Y,PLAYER_WIDTH,1)){
            //Thus, we let the game know the actor is standing on some ground.
            touching_ground=true;

            //Set the actor's moving platform variables to the movement status of this npc.

            bool player_notices_wall=false;
            if(i<0){
                player_notices_wall=player.notice_wall();
            }
            else{
                player_notices_wall=mp_players[i].notice_wall();
            }

            //X-axis.
            if(!player_notices_wall){
                moving_platform_x=PLAYER_RUN_SPEED;
                MOVING_PLATFORM_X_THIS_FRAME=true;
            }
            //If the npc is not moving and not running into a wall.
            if(PLAYER_RUN_SPEED==0.0 && !player_notices_wall){
                moving_platform_x=PLAYER_MOVING_PLATFORM_X;
                MOVING_PLATFORM_X_THIS_FRAME=true;
            }

            //Y-axis.
            if((PLAYER_IN_AIR && PLAYER_AIR_VELOCITY<0.0) || PLAYER_MOVING_PLATFORM_Y<0.0){
                if(PLAYER_IN_AIR && PLAYER_AIR_VELOCITY<0.0){
                    moving_platform_y=PLAYER_AIR_VELOCITY;
                }
                else if(PLAYER_MOVING_PLATFORM_Y<0.0){
                    moving_platform_y=PLAYER_MOVING_PLATFORM_Y;
                }
            }
            else if((PLAYER_IN_AIR && PLAYER_AIR_VELOCITY>=0.0) || PLAYER_MOVING_PLATFORM_Y>0.0){
                if(PLAYER_IN_AIR && PLAYER_AIR_VELOCITY>=0.0){
                    moving_platform_y=PLAYER_AIR_VELOCITY;
                }
                else if(PLAYER_MOVING_PLATFORM_Y>0.0){
                    moving_platform_y=PLAYER_MOVING_PLATFORM_Y;
                }
            }

            //The actor is now standing on a cloud tile.
            on_cloud=true;
            //The cloud "tile" is a rideable npc.
            cloud_npc=true;

            if(PLAYER_SWIMMING){
                MOVING_PLATFORM_IN_WATER=true;
            }
        }
    }
}

void Actor::handle_collision_moving_platforms(short solidity,bool counts_as_trap){
    for(int i=0;i<vector_moving_platforms.size();i++){
        if(fabs(vector_moving_platforms[i].x-x)<PROCESS_RANGE && fabs(vector_moving_platforms[i].y-y)<PROCESS_RANGE){
            if(!counts_as_trap){
                handle_collision_solid(vector_moving_platforms[i].x,vector_moving_platforms[i].y,vector_moving_platforms[i].w,vector_moving_platforms[i].h,solidity,NPC_END);

                //Regardless of the actor's IN_AIR and air_velocity status, one other check needs to be done.
                //If the top row of this npc's pixels are one pixel below the actor, he is standing on this npc.
                if(collision_check(x,y+h,w,1,vector_moving_platforms[i].x,vector_moving_platforms[i].y,vector_moving_platforms[i].w,1)){
                    //Thus, we let the game know the actor is standing on some ground.
                    touching_ground=true;

                    //Set the actor's moving platform variables to the movement status of this platform.

                    //X-axis.
                    if(vector_moving_platforms[i].move_state==LEFT || vector_moving_platforms[i].move_state==LEFT_DOWN || vector_moving_platforms[i].move_state==LEFT_UP){
                        moving_platform_x=-vector_moving_platforms[i].move_speed;
                        MOVING_PLATFORM_X_THIS_FRAME=true;
                    }
                    else if(vector_moving_platforms[i].move_state==RIGHT || vector_moving_platforms[i].move_state==RIGHT_DOWN || vector_moving_platforms[i].move_state==RIGHT_UP){
                        moving_platform_x=vector_moving_platforms[i].move_speed;
                        MOVING_PLATFORM_X_THIS_FRAME=true;
                    }

                    //Y-axis.
                    if(vector_moving_platforms[i].move_state==UP || vector_moving_platforms[i].move_state==LEFT_UP || vector_moving_platforms[i].move_state==RIGHT_UP){
                        moving_platform_y=-vector_moving_platforms[i].move_speed;
                    }
                    else if(vector_moving_platforms[i].move_state==DOWN || vector_moving_platforms[i].move_state==LEFT_DOWN || vector_moving_platforms[i].move_state==RIGHT_DOWN){
                        moving_platform_y=vector_moving_platforms[i].move_speed;
                    }

                    //The actor is now standing on a cloud tile.
                    on_cloud=true;
                }
            }
        }
    }
}

void Actor::handle_collision_doors(){
    double check_x=1.0;
    double check_w=2.0;

    for(int i=0;i<vector_doors.size();i++){
        if(fabs(vector_doors[i].x-x)<PROCESS_RANGE && fabs(vector_doors[i].y-y)<PROCESS_RANGE){
            //If the door is closed.
            if(!vector_doors[i].open){
                bool door_unlocked=false;

                //If this actor is the player and it collides with the door at all.
                if(is_player && collision_check(x-check_x,y,w+check_w,h,vector_doors[i].x,vector_doors[i].y,vector_doors[i].w,vector_doors[i].h)){
                    //Check if the door is a key door and should be unlocked.
                    door_unlocked=vector_doors[i].attempt_unlock();
                }

                if(!door_unlocked){
                    handle_collision_solid(vector_doors[i].x,vector_doors[i].y,vector_doors[i].w,vector_doors[i].h,TILE_SOLIDITY_SOLID,NPC_END);
                }
            }
        }
    }
}

void Actor::handle_collision_springs(short solidity){
    for(int i=0;i<vector_springs.size();i++){
        if(fabs(vector_springs[i].x-x)<PROCESS_RANGE && fabs(vector_springs[i].y-y)<PROCESS_RANGE){
            SDL_Rect springy;

            double adjust_x=0.0;
            double adjust_y=0.0;
            double adjust_w=0.0;
            double adjust_h=0.0;

            if(vector_springs[i].direction==RIGHT){
                springy.x=vector_springs[i].x+SPRING_W/2.0;
                springy.y=vector_springs[i].y+2;
                springy.w=SPRING_W/2.0+2;
                springy.h=SPRING_H-4;
                adjust_w=-11;
            }
            else if(vector_springs[i].direction==LEFT){
                springy.x=vector_springs[i].x-2;
                springy.y=vector_springs[i].y+2;
                springy.w=SPRING_W/2.0+2;
                springy.h=SPRING_H-4;
                adjust_x=11;
                adjust_w=-11;
            }
            else if(vector_springs[i].direction==UP){
                springy.x=vector_springs[i].x+2;
                springy.y=vector_springs[i].y-2;
                springy.w=SPRING_W-4;
                springy.h=SPRING_H/2.0+2;
                adjust_y=11;
                adjust_h=-11;
            }
            else if(vector_springs[i].direction==RIGHT_UP){
                springy.x=vector_springs[i].x+2;
                springy.y=vector_springs[i].y-2;
                springy.w=SPRING_W;
                springy.h=SPRING_H;
                adjust_y=5.5;
                adjust_w=-5.5;
                adjust_h=-5.5;
            }
            else if(vector_springs[i].direction==LEFT_UP){
                springy.x=vector_springs[i].x-2;
                springy.y=vector_springs[i].y-2;
                springy.w=SPRING_W;
                springy.h=SPRING_H;
                adjust_x=5.5;
                adjust_y=5.5;
                adjust_w=-5.5;
                adjust_h=-5.5;
            }
            else if(vector_springs[i].direction==DOWN){
                springy.x=vector_springs[i].x+2;
                springy.y=vector_springs[i].y+SPRING_H/2.0;
                springy.w=SPRING_W-4;
                springy.h=SPRING_H/2.0+2;
                adjust_h=-11;
            }
            else if(vector_springs[i].direction==RIGHT_DOWN){
                springy.x=vector_springs[i].x+2;
                springy.y=vector_springs[i].y+2;
                springy.w=SPRING_W;
                springy.h=SPRING_H;
                adjust_w=-5.5;
                adjust_h=-5.5;
            }
            else if(vector_springs[i].direction==LEFT_DOWN){
                springy.x=vector_springs[i].x-2;
                springy.y=vector_springs[i].y+2;
                springy.w=SPRING_W;
                springy.h=SPRING_H;
                adjust_x=5.5;
                adjust_w=-5.5;
                adjust_h=-5.5;
            }

            if(collision_check(x,y,w,h,springy.x+adjust_x,springy.y+adjust_y,springy.w+adjust_w,springy.h+adjust_h)){
                IN_AIR=true;
                frame_jump=0;

                vector_springs[i].frame=1;

                if(vector_springs[i].direction==RIGHT){
                    run_speed=vector_springs[i].speed;
                    x=springy.x+adjust_x+springy.w+adjust_w;
                }
                else if(vector_springs[i].direction==LEFT){
                    run_speed=-vector_springs[i].speed;
                    x=springy.x+adjust_x-w;
                }
                else if(vector_springs[i].direction==UP){
                    air_velocity=-vector_springs[i].speed;
                    y=springy.y+adjust_y-h;
                }
                else if(vector_springs[i].direction==RIGHT_UP){
                    run_speed=vector_springs[i].speed*(cos(45.0*(M_PI/180)));
                    air_velocity=vector_springs[i].speed*-(sin(45.0*(M_PI/180)));
                    x=springy.x+adjust_x+springy.w+adjust_w;
                    y=springy.y+adjust_y-h;
                }
                else if(vector_springs[i].direction==LEFT_UP){
                    run_speed=vector_springs[i].speed*(cos(135.0*(M_PI/180)));
                    air_velocity=vector_springs[i].speed*-(sin(135.0*(M_PI/180)));
                    x=springy.x+adjust_x-w;
                    y=springy.y+adjust_y-h;
                }
                else if(vector_springs[i].direction==DOWN){
                    air_velocity=vector_springs[i].speed;
                    y=springy.y+adjust_y+springy.h+adjust_h;
                }
                else if(vector_springs[i].direction==RIGHT_DOWN){
                    run_speed=vector_springs[i].speed*(cos(45.0*(M_PI/180)));
                    air_velocity=vector_springs[i].speed*(sin(45.0*(M_PI/180)));
                    x=springy.x+adjust_x+springy.w+adjust_w;
                    y=springy.y+adjust_y+springy.h+adjust_h;
                }
                else if(vector_springs[i].direction==LEFT_DOWN){
                    run_speed=vector_springs[i].speed*(cos(135.0*(M_PI/180)));
                    air_velocity=vector_springs[i].speed*(sin(135.0*(M_PI/180)));
                    x=springy.x+adjust_x-w;
                    y=springy.y+adjust_y+springy.h+adjust_h;
                }

                play_positional_sound(sound_system.spring,x,y);
            }
            else{
                handle_collision_solid(vector_springs[i].x+adjust_x,vector_springs[i].y+adjust_y,SPRING_W+adjust_w,SPRING_H+adjust_h,solidity,NPC_END);
            }
        }
    }
}

void Actor::handle_collision_boosters(){
    for(int i=0;i<vector_boosters.size();i++){
        if(fabs(vector_boosters[i].x-x)<PROCESS_RANGE && fabs(vector_boosters[i].y-y)<PROCESS_RANGE){
            if(collision_check(return_x(),return_y(),return_w(),return_h(),vector_boosters[i].x+8,vector_boosters[i].y+8,BOOSTER_W-16,BOOSTER_H-16)){
                if(vector_boosters[i].direction==RIGHT){
                    run_speed+=vector_boosters[i].speed;
                    x=vector_boosters[i].x+BOOSTER_W-8;
                }
                else if(vector_boosters[i].direction==LEFT){
                    run_speed-=vector_boosters[i].speed;
                    x=vector_boosters[i].x-w+8;
                }

                play_positional_sound(sound_system.booster,x,y);
            }
        }
    }
}

collision_data_trap Actor::handle_collision_traps(short solidity){
    collision_data_trap box;
    box.x=-1.0;
    box.y=-1.0;
    box.w=-1.0;
    box.h=-1.0;

    for(int i=0;i<vector_traps.size();i++){
        if(fabs(vector_traps[i].x-x)<PROCESS_RANGE && fabs(vector_traps[i].y-y)<PROCESS_RANGE){
            if(vector_traps[i].dangerous){
                if((!vector_traps[i].is_sharp() || (vector_traps[i].is_sharp() && !suit_sharp)) && collision_check(x,y,w,h,vector_traps[i].return_x()+vector_traps[i].return_danger_x(),vector_traps[i].return_y()+vector_traps[i].return_danger_y(),vector_traps[i].return_danger_w(),vector_traps[i].return_danger_h())){
                    box.x=vector_traps[i].return_x()+vector_traps[i].return_danger_x();
                    box.y=vector_traps[i].return_y()+vector_traps[i].return_danger_y();
                    box.w=vector_traps[i].return_danger_w();
                    box.h=vector_traps[i].return_danger_h();
                    return box;
                }
                else{
                    if(vector_traps[i].solid){
                        handle_collision_solid(vector_traps[i].return_x(),vector_traps[i].return_y(),vector_traps[i].return_w(),vector_traps[i].return_h(),solidity,NPC_END);
                    }
                }
            }
        }
    }

    return box;
}

bool Actor::push_actor_up(Actor* actor,double actor_x,double actor_w,bool being_pushed_up,short passed_type){
    bool need_to_handle_events=false;
    bool moving_upwards=false;

    if(being_pushed_up || (IN_AIR && air_velocity<0)){
        moving_upwards=true;
    }

    //If this npc jumps or swims into the bottom of the npc and the npc is falling or swimming down.
    if(moving_upwards && actor->air_velocity>=0){
        //Check to see if there is a y-axis collision.
        //We are checking only the bottom row of the pixels for the npc against only the top row of the pixels for this npc.
        if(collision_check(x,y,w,h/2,actor_x,actor->y+actor->h/2,actor_w,actor->h/2)){
            //We set the npc's y value to the y value of this npc - the npc's height, so the npc will now be right on top of this npc.
            actor->y=y-actor->h;
            //If the npc was in the air, they will no longer be.
            actor->IN_AIR=false;
            //The npc has been moved, so handle collision.
            need_to_handle_events=true;

            actor->pushed_into_ceiling();

            if(actor->is_player && passed_type==NPC_COW){
                if(!player.timer_riding_cows.is_started()){
                    player.timer_riding_cows.start();
                }
                player.riding_cow=true;
            }
        }
    }

    return need_to_handle_events;
}

void Actor::pushed_into_ceiling(){
    //The current tile location for the actor.
    int actor_current_x=(int)((int)x/TILE_SIZE);
    int actor_current_y=(int)((int)y/TILE_SIZE);

    //Check all tiles in a square around the actor.
    int check_x_start=actor_current_x-2;
    int check_x_end=actor_current_x+2;
    int check_y_start=actor_current_y-2;
    int check_y_end=actor_current_y+2;

    for(int int_y=check_y_start;int_y<check_y_end;int_y++){
        for(int int_x=check_x_start;int_x<check_x_end;int_x++){
            //As long as the current tile is within the level's boundaries.
            if(int_x>=0 && int_x<=(level.level_x/TILE_SIZE)-1 && int_y>=0 && int_y<=(level.level_y/TILE_SIZE)-1){
                if(level.tile_array[int_x][int_y].solidity==TILE_SOLIDITY_SOLID &&
                   collision_check(x,y,w,h/2,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE)){
                    y=level.tile_array[int_x][int_y].y+TILE_SIZE;
                    air_velocity=0;
                }
            }
        }
    }
}

void Actor::handle_collision_solid(double passed_x,double passed_y,double passed_w,double passed_h,short solidity,short passed_type){
    ///double check_x=1.0;
    ///double check_w=2.0;
    double check_x=0.0;
    double check_w=0.0;

    if(solidity==TILE_SOLIDITY_SOLID){
        //|----|
        //xxxxxx
        //xxxxxx
        //|----|
        if(collision_check(x-check_x,y+(h/4),w+check_w,h/2+h/8,passed_x,passed_y,passed_w,passed_h)){
            //If the left of the actor is left of the tile's left side, we know the actor is hitting the left side of the tile.
            if(x<passed_x){
                //We set the actor's x value to the x value of the tile - the actor's width, so the actor will now be on the left side of the tile.
                x=passed_x-w-check_x;
            }
            //If the right side of the actor is right of the tile's right side, we know the actor is hitting the right side of the tile.
            else if(x+w>passed_x+passed_w){
                //We set the actor's x value to the right side of the tile.
                x=passed_x+passed_w+check_x;
            }
            //The actor has bumped into a wall, so stop moving.
            run_speed=0.0;
            moving_platform_x=0.0;
        }

        //Now we check to see if there is a y-axis collision. The section of the actor that collides with the solid tile matters.
        //We first check for a collision between the top of the actor and the bottom of the tile.
        //|xxxx|
        //|----|
        //|----|
        //|----|
        //We also check that the actor is in the air and moving upwards.
        else if(((IN_AIR && air_velocity<0) || SWIMMING || FLYING || moving_platform_y!=0.0) && collision_check(x+4,y,w-8,h,passed_x,passed_y+passed_h/2,passed_w,passed_h/2)){
            //We set the actor's y value to the bottom of the tile.
            y=passed_y+passed_h;
            //We set the air velocity to 0, so that the actor immediately begins falling once they bump their head on a tile.
            air_velocity=0;
            //The actor has bumped into a wall, so stop moving.
            if(SWIMMING){
                float_speed=0.0;
            }
        }

        //Next, we check for a collision between the bottom of the actor and the top of the tile.
        //|----|
        //|----|
        //|----|
        //xxxxxx
        //We only do this if the actor is falling, so that they don't get warped to the top of the tile if jumping into the side of it.
        else if(!SWIMMING && air_velocity>=0 && collision_check(x+4,y+h-(h/12),w-8,h/12,passed_x,passed_y,passed_w,passed_h/12)){
            //We set the actor's y value to the y value of the tile - the actor's height, so the actor will now be right on top of the tile.
            y=passed_y-h;
            //We set IN_AIR to false, so that if the actor was in the air, they will no longer be.
            IN_AIR=false;
        }
        else if(SWIMMING && collision_check(x,y+h-(h/4),w,(h/4)+1,passed_x,passed_y,passed_w,passed_h)){
            //We set the actor's y value to the y value of the tile - the actor's height, so the actor will now be right on top of the tile.
            y=passed_y-h-1;
            //The actor has bumped into a wall, so stop moving.
            float_speed=0.0;
        }

        //If the top row of this tile's pixels are one pixel below the actor, he is standing on this tile.
        if(collision_check(x,y+h,w,1,passed_x,passed_y,passed_w,1)){
            //Thus, we let the game know the actor is standing on some ground.
            touching_ground=true;

            if(is_player && passed_type==NPC_COW){
                if(!player.timer_riding_cows.is_started()){
                    player.timer_riding_cows.start();
                }
                player.riding_cow=true;
            }
        }
    }
    else if(solidity==TILE_SOLIDITY_CLOUD){
        //If the tile is a cloud tile, and the actor is in the air and falling.
        if(air_velocity>=0){
            //First, we check to see if there is a x-axis collision.
            //|----|
            //xxxxxx
            //xxxxxx
            //|----|
            //If there is an x-axis collision while the actor is falling, don't check for a y-axis collision this time.
            //This prevents a falling actor from warping back to the top of this tile, which would be caused by the below if() statement alone.
            //This if() statement just exists to create another condition for the below if() statement.
            if(collision_check(x-check_x,y+(h/4),w+check_w,h/2+h/8,passed_x,passed_y,passed_w,passed_h)){
            }

            //Now, if there was no x-axis collision, we check to see if there is a y-axis collision.
            //For y-axis hit detection we check this part of the actor's rectangle:
            //|----|
            //|----|
            //|----|
            //xxxxxx
            //As shown above, we are checking only the bottom row of the pixels for the actor against only the top row of the pixels for the tile.
            else if(collision_check(x+4,y+h-(h/12),w-8,h/12,passed_x,passed_y,passed_w,passed_h/12)){
                //We set the actor's y value to the y value of the tile - the actor's height, so the actor will now be right on top of the tile.
                y=passed_y-h;
                //We set IN_AIR to false, since the actor was in the air, so now they will no longer be.
                IN_AIR=false;
            }
        }

        //If the top row of this tile's pixels are one pixel below the actor, he is standing on this tile.
        if(collision_check(x,y+h,w,1,passed_x,passed_y,passed_w,1)){
            //Thus, we let the game know the actor is standing on some ground.
            touching_ground=true;
            //The actor is now standing on a cloud tile.
            on_cloud=true;

            if(is_player && passed_type==NPC_COW){
                if(!player.timer_riding_cows.is_started()){
                    player.timer_riding_cows.start();
                }
                player.riding_cow=true;
            }
        }
    }
}

void Actor::handle_collision_tiles(int int_x,int int_y,bool crouching){
    //If the tile is an air tile, let the game know that the actor is touching air.
    if(!SWIMMING && level.tile_array[int_x][int_y].solidity==TILE_SOLIDITY_PASSABLE){
        if(collision_check(x,y+h,w,h,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE)){
            touching_air=true;
        }
    }

    //If the tile is a non-water non-solid tile.
    else if(SWIMMING && level.tile_array[int_x][int_y].solidity==TILE_SOLIDITY_PASSABLE && level.tile_array[int_x][int_y].special!=TILE_SPECIAL_WATER){
        //This means that the actor is currently at the surface of the water.
        if(!IN_AIR && collision_check(x,y+h/4,w,h*(3/4),level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y+TILE_SIZE/2,TILE_SIZE,TILE_SIZE/2)){
            y=level.tile_array[int_x][int_y].y+TILE_SIZE-h/4;

            //The actor is both swimming and touching an air tile, so he has surfaced.
            //Therefore, the actor can now jump off of the surface of the water.
            SWIM_CAN_JUMP=true;
            //Stop the actor's current y-axis swim speed.
            float_speed=0.0;
        }
        //If the actor is just below this air tile.
        if(!IN_AIR && collision_check(x,y-TILE_SIZE+4,w,TILE_SIZE-4,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE)){
            //Since the actor has surfaced, he is not currently underwater even though he is swimming.
            underwater=false;
        }
        if(collision_check(x,y+h*(3/4),w,h/4,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE/2)){
            SWIMMING=false;
            run_speed=0.0;
            moving_platform_x=0.0;
            ///I can add a check here later for the actor being within the camera bounds.
            ///This will ensure that npcs not on camera won't make performance-eating effects.
            ///I am referencing the player object, since there is only one player.
            ///When I add multiplayer to the engine, I will have to check all of the players' cameras.
            //As long as the elements of the vector do not exceed the limit.
            if(vector_effect_water_splash.size()<player.option_effect_limit){
                vector_effect_water_splash.push_back(Effect_Water_Splash(x-3,y-9+2));
                play_positional_sound(sound_system.splash_out,x,y);
            }
        }
    }

    //If the tile is a solid, non-sloping tile, and we were not touching a sloped tile after the last check.
    //Or, if the actor is flying and this is a solid sloping tile, we will treat this as a regular solid tile.
    if((!touching_sloped_ground_last_check && level.tile_array[int_x][int_y].solidity==TILE_SOLIDITY_SOLID && level.tile_array[int_x][int_y].slope==180) || (FLYING && level.tile_array[int_x][int_y].solidity==TILE_SOLIDITY_SOLID && level.tile_array[int_x][int_y].slope!=180)){
        handle_collision_solid(level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE,TILE_SOLIDITY_SOLID,NPC_END);
    }

    //If the tile is a solid, sloping tile, and the actor is not flying.
    else if(!FLYING && level.tile_array[int_x][int_y].solidity==TILE_SOLIDITY_SOLID && level.tile_array[int_x][int_y].slope!=180){
        //If the actor is on a moving platform that is moving on the x-axis,
        //he should be bumped off when he hits a slope.

        /**if(moving_platform_y==0.0 && (moving_platform_x>0.0 || on_cloud) && !IN_AIR && collision_check(x,y,w/2,h,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE)){
            fall_through_cloud();
        }
        else if(moving_platform_y==0.0 && (moving_platform_x<0.0 || on_cloud) && !IN_AIR && collision_check(x+w/2,y,w/2,h,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE)){
            fall_through_cloud();
        }
        else if(moving_platform_y>0.0 && !IN_AIR && collision_check(x,y+h/2,w,h/2,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE)){
            fall_through_cloud();
        }*/

        //Otherwise, handle the slope normally.
        /**else */if(moving_platform_x==0.0 && !on_cloud && moving_platform_y==0.0){
            double x_position=x+w/2-level.tile_array[int_x][int_y].x;
            ///double y_loc=level.tile_array[int_x][int_y].height_mask[(int)x_position];
            double y_loc=level.tile_array[int_x][int_y].get_height_mask((short)x_position);
            double y_goal=level.tile_array[int_x][int_y].y+y_loc-h;

            int temp_x=-1;
            int temp_y=-1;

            //If the middle of the actor is within the tile on the x-axis.
            if(x+w/2>=level.tile_array[int_x][int_y].x && x+w/2<level.tile_array[int_x][int_y].x+TILE_SIZE &&
               y<=level.tile_array[int_x][int_y].y+TILE_SIZE){
                //If the actor is moving downwards and has reached the "ground level" of the slope, or is not moving downwards or in the air at all.
                if((((air_velocity>=0 && IN_AIR) || (moving_platform_y>0.0)) && y>=y_goal) || (!IN_AIR && moving_platform_y==0.0)){
                    //Snap to the slope.
                    y=y_goal;
                    IN_AIR=false;
                    touching_ground=true;
                    touching_sloped_ground=true;
                    touched_slope_angle=level.tile_array[int_x][int_y].slope;
                }
            }

            if((((air_velocity>=0 && IN_AIR) || (moving_platform_y>0.0)) || (!IN_AIR && moving_platform_y==0.0)) && collision_check(x,y,w,h,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE)){
                if(x+w/2>=level.tile_array[int_x][int_y].x+TILE_SIZE){
                    if(level.tile_array[int_x][int_y].slope==45){
                        temp_x=int_x+1;
                        temp_y=int_y-1;
                    }
                    else if(level.tile_array[int_x][int_y].slope==135){
                        temp_x=int_x+1;
                        temp_y=int_y+1;
                    }
                }
                else if(x+w/2<level.tile_array[int_x][int_y].x){
                    if(level.tile_array[int_x][int_y].slope==45){
                        temp_x=int_x-1;
                        temp_y=int_y+1;
                    }
                    else if(level.tile_array[int_x][int_y].slope==135){
                        temp_x=int_x-1;
                        temp_y=int_y-1;
                    }
                }
            }

            if(temp_x!=-1 && temp_y!=-1 &&
               temp_x>=0 && temp_x<=(level.level_x/TILE_SIZE)-1 && temp_y>=0 && temp_y<=(level.level_y/TILE_SIZE)-1 &&
               (level.tile_array[temp_x][temp_y].solidity==TILE_SOLIDITY_SOLID || level.tile_array[temp_x][temp_y].solidity==TILE_SOLIDITY_CLOUD) &&
               level.tile_array[temp_x][temp_y].slope==180){
                y=level.tile_array[temp_x][temp_y].y-h;
                IN_AIR=false;
                touching_ground=true;
            }
        }
    }

    //If the tile is a cloud tile, and the actor is in the air and falling.
    if(level.tile_array[int_x][int_y].solidity==TILE_SOLIDITY_CLOUD){
        handle_collision_solid(level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE,TILE_SOLIDITY_CLOUD,NPC_END);
    }

    //If the tile is a water tile.
    if(level.tile_array[int_x][int_y].special==TILE_SPECIAL_WATER){
        if(is_player && !SWIMMING && fabs(run_speed)>=WATER_RUN_THRESHOLD){
            handle_collision_solid(level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE,TILE_SOLIDITY_SOLID,NPC_END);
            if(touching_ground){
                water_running=true;
            }
        }

        if(collision_check(x,y,w,h/4,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE)){
            //If the actor is not already swimming, start swimming mode.
            if(!SWIMMING && fabs(run_speed)<WATER_RUN_THRESHOLD){
                crouch_stop();

                float_speed=max_buoyancy;
                //Move the actor just below the top of this water tile, so he doesn't "rehit" the air above the water.
                y=level.tile_array[int_x][int_y].y+0.1;

                //Start timing how long the actor has been in the water.
                timer_time_in_water.start();

                SWIMMING=true;
                CLIMBING=false;
                climb_jump_timer=climb_jump_delay;
                IN_AIR=false;
                air_velocity=0;

                ///I can add a check here later for the actor being within the camera bounds.
                ///This will ensure that npcs not on camera won't make performance-eating effects.
                //As long as the elements of the vector do not exceed the limit.
                if(vector_effect_water_splash.size()<player.option_effect_limit){
                    vector_effect_water_splash.push_back(Effect_Water_Splash(x-3,y-26+2));
                    play_positional_sound(sound_system.splash_in,x,y);
                }
            }

            if(SWIMMING){
                //The actor is in water, and thus he can't jump.
                SWIM_CAN_JUMP=false;
                IN_AIR=false;
            }
        }
    }

    //*******************************************//
    // Handle climbable and climbable top tiles: //
    //*******************************************//

    //If the tile is a climbable top tile, and the actor is in the air and falling.
    if(level.tile_array[int_x][int_y].special==TILE_SPECIAL_CLIMBABLE_TOP && IN_AIR && air_velocity>=0){
        //First, we check to see if there is a x-axis collision.
        //|----|
        //xxxxxx
        //xxxxxx
        //|----|
        //If there is an x-axis collision while the actor is falling, don't check for a y-axis collision this time.
        //This prevents a falling actor from warping back to the top of this tile, which would be caused by the below if() statement alone.
        //This if() statement just exists to create another condition for the below if() statement.
        if(collision_check(x,y+(h/4),w,h/2+h/8,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE)){
        }

        //Now, if there was no x-axis collision, we check to see if there is a y-axis collision.
        //For y-axis hit detection we check this part of the actor's rectangle:
        //|----|
        //|----|
        //|----|
        //xxxxxx
        //As shown above, we are checking only the bottom row of the pixels for the actor against only the top row of the pixels for the tile.
        else if(collision_check(x,y+h-(h/12),w,h/12,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE/12)){
            //We set the actor's y value to the y value of the tile - the actor's height, so the actor will now be right on top of the tile.
            y=level.tile_array[int_x][int_y].y-h;
            //We set IN_AIR to false, since the actor was in the air, so now they will no longer be.
            IN_AIR=false;
        }
    }

    //Regardless of the actor's IN_AIR and air_velocity status, one other check needs to be done for this tile.
    if(level.tile_array[int_x][int_y].special==TILE_SPECIAL_CLIMBABLE_TOP){
        //If the top row of this tile's pixels are one pixel below the actor, he is standing on this tile.
        if(collision_check(x,y+h,w,1,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,1)){
            //Thus, we let the game know the actor is standing on some ground.
            touching_ground=true;
            //The actor is now standing on a climbable top tile.
            on_climbable_top_tile=true;
        }
    }

    //If the tile is a climbable tile and the actor is not already climbing.
    if(level.tile_array[int_x][int_y].special==TILE_SPECIAL_CLIMBABLE && climb_jump_timer==0 && !crouching){
        if((climb_state==UP || climb_state==DOWN) && collision_check(x,y,w,h/8,level.tile_array[int_x][int_y].x+w/2-w/8,level.tile_array[int_x][int_y].y,TILE_SIZE/4,TILE_SIZE)){
            IN_AIR=false;
            CLIMBING=true;
            x=level.tile_array[int_x][int_y].x+3;
            run_speed=0.0;
            moving_platform_x=0.0;
        }
    }
    else if(level.tile_array[int_x][int_y].special==TILE_SPECIAL_CLIMBABLE_TOP && climb_jump_timer==0){
        if(climb_state==DOWN && on_climbable_top_tile && collision_check(x,y,w,h+h/12,level.tile_array[int_x][int_y].x+w/2-w/8,level.tile_array[int_x][int_y].y,TILE_SIZE/4,TILE_SIZE)){
            IN_AIR=false;
            CLIMBING=true;
            x=level.tile_array[int_x][int_y].x+3;
            run_speed=0.0;
            moving_platform_x=0.0;
        }
    }
}

void Actor::handle_climbing(int int_x,int int_y){
    //If the actor is climbing downwards.
    if(move_state==DOWN){
        //If the actor hits ground.
        if(level.tile_array[int_x][int_y].solidity>TILE_SOLIDITY_PASSABLE){
            if(collision_check(x,y,w,h,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE)){
                y=level.tile_array[int_x][int_y].y-h;
                CLIMBING=false;
                climb_jump_timer=climb_jump_delay;
            }
        }
        //If the actor hits air.
        else if(level.tile_array[int_x][int_y].solidity==TILE_SOLIDITY_PASSABLE && level.tile_array[int_x][int_y].special!=TILE_SPECIAL_CLIMBABLE && level.tile_array[int_x][int_y].special!=TILE_SPECIAL_CLIMBABLE_TOP){
            if(collision_check(x,y,w,h/2,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE)){
                CLIMBING=false;
                climb_jump_timer=climb_jump_delay;
                IN_AIR=true;
                air_velocity=0;
            }
        }
    }
    //If the actor is climbing upwards.
    else if(move_state==UP){
        //If the actor hits ground.
        if(level.tile_array[int_x][int_y].solidity>TILE_SOLIDITY_PASSABLE){
            if(collision_check(x,y,w,h/2,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE)){
                y=level.tile_array[int_x][int_y].y+TILE_SIZE;
            }
        }
        //If the actor hits air.
        else if(level.tile_array[int_x][int_y].solidity==TILE_SOLIDITY_PASSABLE && level.tile_array[int_x][int_y].special!=TILE_SPECIAL_CLIMBABLE && level.tile_array[int_x][int_y].special!=TILE_SPECIAL_CLIMBABLE_TOP){
            if(collision_check(x,y,w,h/4,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y+TILE_SIZE-TILE_SIZE/4,TILE_SIZE,TILE_SIZE/4)){
                y=level.tile_array[int_x][int_y].y+TILE_SIZE;
            }
        }
    }
}

void Actor::finish_events(){
    if(is_player){
        int actor_current_x=(int)((int)x/TILE_SIZE);
        int actor_current_y=(int)((int)y/TILE_SIZE);

        int check_x_start=actor_current_x-4;
        int check_x_end=actor_current_x+4;
        int check_y_start=actor_current_y-4;
        int check_y_end=actor_current_y+4;

        /**bool touching_any_air=false;

        for(int int_x=check_x_start;int_x<check_x_end;int_x++){
            for(int int_y=check_y_start;int_y<check_y_end;int_y++){
                if(int_x>=0 && int_x<=(level.level_x/TILE_SIZE)-1 && int_y>=0 && int_y<=(level.level_y/TILE_SIZE)-1){
                    if(collision_check(x,y,w,h,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE)){
                        if(level.tile_array[int_x][int_y].solidity!=TILE_SOLIDITY_SOLID){
                            touching_any_air=true;
                        }
                    }
                }
            }
        }

        if(!touching_any_air){
            y-=PLAYER_CROUCH_H;
        }*/

        ///
        if(!touching_sloped_ground){
            bool touching_any_solid=false;

            for(int int_x=check_x_start;int_x<check_x_end;int_x++){
                for(int int_y=check_y_start;int_y<check_y_end;int_y++){
                    if(int_x>=0 && int_x<=(level.level_x/TILE_SIZE)-1 && int_y>=0 && int_y<=(level.level_y/TILE_SIZE)-1){
                        if(collision_check(return_x(),return_y(),return_w(),return_h(),level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE)){
                            //If the tile is solid and non-sloping.
                            if(level.tile_array[int_x][int_y].solidity==TILE_SOLIDITY_SOLID && level.tile_array[int_x][int_y].slope==180){
                                touching_any_solid=true;

                                break;
                            }
                        }
                    }
                }
            }

            if(touching_any_solid){
                y-=PLAYER_CROUCH_H;
            }
        }
        ///
    }

    if(SWIMMING || CLIMBING){
        extra_jumps=get_extra_jumps_max();
    }

    //If the player is touching ground.
    if(touching_ground){
        extra_jumps=get_extra_jumps_max();

        //If the actor is the player.
        if(is_player){
            //Set this fall's height.
            double this_fall=player.y-player.special_count_height;

            //If this fall is non-negative and is greater than the current farthest fall stat.
            if(this_fall>0 && player.stat_farthest_fall<this_fall){
                //Set the farthest fall stat to this fall's height.
                player.stat_farthest_fall=this_fall;
                //Reset the special count height variable.
                //Note: The number this variable is always reset to (100,000) is chosen so
                //No player position within any level in the game can ever be a higher number.
                player.special_count_height=100000;
            }
        }
    }

    //If the actor is not already in the air, is not climbing, is touching air and is not touching any ground.
    if(!CLIMBING && !IN_AIR && touching_air && !touching_ground){
        //We set IN_AIR to true, as the actor is now in the air.
        IN_AIR=true;

        //We set the air velocity to 0, and then it increments up to the maximum fall rate in move(). By setting this to 0, the actor starts falling slowly, but
        //still reaches the maximum fall rate fairly quickly.
        air_velocity=0;

        //If the actor is a player.
        if(is_player){
            frame_jump=3;
            //If the player has reached the fear frame,
            //but is fairly close to the ground, skip the frame.
            if(distance_to_ground()<=10){
                frame_jump=4;
            }
            //If the fear frame was not skipped, play the fear sound.
            if(IN_AIR && !SHOOTING && frame_jump==3){
                play_positional_sound(sound_system.player_fear,x,y);
            }
        }
        else{
            frame_jump=0;
        }
    }

    //Keep the actor in the level map's bounds.
    if(x<0){
        x=0;

        //The actor has bumped into a wall, so stop moving.
        run_speed=0.0;
        moving_platform_x=0.0;
    }
    if(x+w>level.level_x){
        x=level.level_x-w;

        //The actor has bumped into a wall, so stop moving.
        run_speed=0.0;
        moving_platform_x=0.0;
    }
    if(y<0){
        y=0;
        //We set the air velocity to 0, so that the actor immediately begins falling once they bump their head on the top of the level.
        air_velocity=0;

        //The actor has bumped into a wall, so stop moving.
        if(SWIMMING){
            float_speed=0;
        }
    }
    if(y+h>level.level_y){
        y=level.level_y-h;
        IN_AIR=false;

        //The actor has bumped into a wall, so stop moving.
        if(SWIMMING){
            float_speed=0;
        }
    }

    if(is_player && which_mp_player==player.cam_focused_index() && !player.survival_complete && !player.goal_crossed && player.counter_jump_mode<=0){
        if(level.alarm){
            if(!music.track_is_playing(MUSIC_ALARM) && !music.track_is_playing(MUSIC_BOSS) &&
               !music.track_is_playing(MUSIC_BOSS_FAST)){
                music.play_track(MUSIC_ALARM);
            }
        }
        else if(underwater){
            if(!music.track_is_playing(MUSIC_BOSS) && !music.track_is_playing(MUSIC_BOSS_FAST) &&
               !music.track_is_playing(MUSIC_LEVEL_WATER)){
                music.play_track(MUSIC_LEVEL_WATER);
            }
        }
        else{
            if(!music.track_is_playing(MUSIC_BOSS) && !music.track_is_playing(MUSIC_BOSS_FAST) &&
               !music.track_is_playing(MUSIC_LEVEL_NORMAL)){
                music.play_track(MUSIC_LEVEL_NORMAL);
            }
        }
    }
}

int Actor::distance_to_ground(){
    int distance=0;

    //The current tile location.
    int int_x=(int)((int)x/TILE_SIZE);
    int int_y=(int)((int)(y+h)/TILE_SIZE);

    while(true){
        //As long as the current tile is within the level's boundaries.
        if(int_x>=0 && int_x<=(level.level_x/TILE_SIZE)-1 && int_y>=0 && int_y<=(level.level_y/TILE_SIZE)-1){
            if(level.tile_array[int_x][int_y].solidity==TILE_SOLIDITY_SOLID || level.tile_array[int_x][int_y].solidity==TILE_SOLIDITY_CLOUD || level.tile_array[int_x][int_y].special==TILE_SPECIAL_CLIMBABLE_TOP || level.tile_array[int_x][int_y].special==TILE_SPECIAL_WATER){
                break;
            }
            else{
                int_y++;
                distance++;
            }
        }

        //If the tile is outside the level's boundaries.
        else{
            break;
        }
    }

    return distance;
}

bool Actor::touching_solid_tile(int check_x_start,int check_x_end,int check_y_start,int check_y_end){
    for(int int_y=check_y_start;int_y<check_y_end;int_y++){
        for(int int_x=check_x_start;int_x<check_x_end;int_x++){
            //As long as the current tile is within the level's boundaries.
            if(int_x>=0 && int_x<=(level.level_x/TILE_SIZE)-1 && int_y>=0 && int_y<=(level.level_y/TILE_SIZE)-1){
                if(level.tile_array[int_x][int_y].solidity==TILE_SOLIDITY_SOLID &&
                   collision_check(x,y,w,h,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE)){
                    return true;
                }
            }
        }
    }

    return false;
}

bool Actor::notice_wall(){
    //Then, treat the edges of the level as walls.
    if(x-2<0 && (move_state==LEFT || move_state==LEFT_UP || move_state==LEFT_DOWN)){
        return true;
    }
    else if(x+w+2>level.level_x && (move_state==RIGHT || move_state==RIGHT_UP || move_state==RIGHT_DOWN)){
        return true;
    }

    if(SWIMMING){
        if(y-2<0 && (move_state==UP || move_state==LEFT_UP || move_state==RIGHT_UP)){
            return true;
        }
        else if(y+h+2>level.level_y && (move_state==DOWN || move_state==LEFT_DOWN || move_state==RIGHT_DOWN)){
            return true;
        }
    }

    if(SWIMMING){
        //The actor's current tile position.
        int current_x=(int)((int)x/TILE_SIZE);
        int current_y=(int)((int)y/TILE_SIZE);

        //Check all tiles in a square around the actor.
        int check_x_start=current_x-4;
        int check_x_end=current_x+4;
        int check_y_start=current_y-4;
        int check_y_end=current_y+4;

        for(int int_y=check_y_start;int_y<check_y_end;int_y++){
            for(int int_x=check_x_start;int_x<check_x_end;int_x++){
                //As long as the current tile is within the level's boundaries.
                if(int_x>=0 && int_x<=(level.level_x/TILE_SIZE)-1 && int_y>=0 && int_y<=(level.level_y/TILE_SIZE)-1){
                    //If the tile is solid.
                    if(level.tile_array[int_x][int_y].solidity==TILE_SOLIDITY_SOLID){
                        //If the tile is just to the actor's left and the actor is wandering left.
                        if(collision_check(x-2,y,w,h,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE)){
                            if(level.tile_array[int_x][int_y].x<x && (move_state==LEFT || move_state==LEFT_UP || move_state==LEFT_DOWN)){
                                return true;
                            }
                        }
                        //If the tile is just to the actor's right and the actor is wandering right.
                        if(collision_check(x+2,y,w,h,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE)){
                            if(level.tile_array[int_x][int_y].x>x && (move_state==RIGHT || move_state==RIGHT_UP || move_state==RIGHT_DOWN)){
                                return true;
                            }
                        }
                        //If the tile is just below the actor and the actor is wandering down.
                        if(collision_check(x,y+2,w,h,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE)){
                            if(level.tile_array[int_x][int_y].y>y && (move_state==DOWN || move_state==LEFT_DOWN || move_state==RIGHT_DOWN)){
                                return true;
                            }
                        }
                        //If the tile is just above the actor and the actor is wandering up.
                        if(collision_check(x,y-2,w,h,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE)){
                            if(level.tile_array[int_x][int_y].y<y && (move_state==UP || move_state==LEFT_UP || move_state==RIGHT_UP)){
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }
    else{
        //The actor's current tile position.
        int current_x=(int)((int)x/TILE_SIZE);
        int current_y=(int)((int)y/TILE_SIZE);

        //Check all tiles in a square around the actor.
        int check_x_start=current_x-4;
        int check_x_end=current_x+4;
        int check_y_start=current_y-4;
        int check_y_end=current_y+4;

        for(int int_y=check_y_start;int_y<check_y_end;int_y++){
            for(int int_x=check_x_start;int_x<check_x_end;int_x++){
                //As long as the current tile is within the level's boundaries.
                if(int_x>=0 && int_x<=(level.level_x/TILE_SIZE)-1 && int_y>=0 && int_y<=(level.level_y/TILE_SIZE)-1){
                    //If the tile is solid.
                    if(level.tile_array[int_x][int_y].solidity==TILE_SOLIDITY_SOLID && level.tile_array[int_x][int_y].slope==180){
                        //If the tile is just to the actor's left and the actor is wandering left.
                        if(collision_check(x-2,y,w,h,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE)){
                            if(level.tile_array[int_x][int_y].x<x && move_state==LEFT){
                                return true;
                            }
                        }
                        //If the tile is just to the actor's right and the actor is wandering right.
                        if(collision_check(x+2,y,w,h,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE)){
                            if(level.tile_array[int_x][int_y].x>x && move_state==RIGHT){
                                return true;
                            }
                        }
                    }
                }
            }
        }

        for(int i=0;i<vector_doors.size();i++){
            if(fabs(vector_doors[i].x-x)<PROCESS_RANGE && fabs(vector_doors[i].y-y)<PROCESS_RANGE){
                if(!vector_doors[i].open){
                    //If the object is just to the actor's left and the actor is wandering left.
                    if(collision_check(x-2,y,w,h,vector_doors[i].x,vector_doors[i].y,vector_doors[i].w,vector_doors[i].h)){
                        if(vector_doors[i].x<x && move_state==LEFT){
                            return true;
                        }
                    }
                    //If the object is just to the actor's right and the actor is wandering right.
                    if(collision_check(x+2,y,w,h,vector_doors[i].x,vector_doors[i].y,vector_doors[i].w,vector_doors[i].h)){
                        if(vector_doors[i].x>x && move_state==RIGHT){
                            return true;
                        }
                    }
                }
            }
        }
    }

    return false;
}

bool Actor::in_open_air(){
    double check_x=1.0;
    double check_w=2.0;

    for(int i=0;i<vector_doors.size();i++){
        if(fabs(vector_doors[i].x-x)<PROCESS_RANGE && fabs(vector_doors[i].y-y)<PROCESS_RANGE){
            if(!vector_doors[i].open){
                if(collision_check(x-check_x,y,w+check_w,h,vector_doors[i].x,vector_doors[i].y,vector_doors[i].w,vector_doors[i].h)){
                    return false;
                }
            }
        }
    }

    int actor_current_x=(int)((int)x/TILE_SIZE);
    int actor_current_y=(int)((int)y/TILE_SIZE);

    int check_x_start=actor_current_x-4;
    int check_x_end=actor_current_x+4;
    int check_y_start=actor_current_y-4;
    int check_y_end=actor_current_y+4;

    for(int int_y=check_y_start;int_y<check_y_end;int_y++){
        for(int int_x=check_x_start;int_x<check_x_end;int_x++){
            if(int_x>=0 && int_x<=(level.level_x/TILE_SIZE)-1 && int_y>=0 && int_y<=(level.level_y/TILE_SIZE)-1){
                if(level.tile_array[int_x][int_y].solidity==TILE_SOLIDITY_SOLID &&
                   collision_check(x,y,w,h,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE)){
                    return false;
                }
            }
        }
    }

    return true;
}

void Actor::crouch_stop(){
}
