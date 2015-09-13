/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "player_mp.h"
#include "world.h"
#include "collision.h"
#include "render.h"

using namespace std;

void Player_Mp::worldmap_ai(){
    //Reset AI keystates.
    for(short i=0;i<256;i++){
        ai_keystates[i]=false;
    }
    ai_key_events.clear();

    int player_index=which_mp_player-1;

    double PLAYER_X=player.x;
    double PLAYER_Y=player.y;
    double PLAYER_WIDTH=player.w;
    double PLAYER_HEIGHT=player.h;
    if(player_index>=0){
        PLAYER_X=mp_players[player_index].x;
        PLAYER_Y=mp_players[player_index].y;
        PLAYER_WIDTH=mp_players[player_index].w;
        PLAYER_HEIGHT=mp_players[player_index].h;
    }

    worldmap_ai_follow_player(player_index);

    while(ai_key_events.size()>0){
        handle_command_event(ai_key_events[ai_key_events.size()-1]);

        ai_key_events.pop_back();
    }
}

void Player_Mp::worldmap_ai_follow_player(int player_index){
    double PLAYER_X=player.x;
    double PLAYER_Y=player.y;
    double PLAYER_WIDTH=player.w;
    double PLAYER_HEIGHT=player.h;
    if(player_index>=0){
        PLAYER_X=mp_players[player_index].x;
        PLAYER_Y=mp_players[player_index].y;
        PLAYER_WIDTH=mp_players[player_index].w;
        PLAYER_HEIGHT=mp_players[player_index].h;
    }

    worldmap_ai_move_to_target(PLAYER_X,PLAYER_Y,PLAYER_WIDTH,PLAYER_HEIGHT);
}

void Player_Mp::worldmap_ai_move_to_target(double target_x,double target_y,double target_w,double target_h){
    if(!collision_check(target_x,target_y,target_w,target_h,x,y,w,h)){
        if(fabs(x-target_x)>4){
            if(x>target_x){
                ai_keystates[COMMAND_LEFT]=true;
            }
            else if(x<target_x){
                ai_keystates[COMMAND_RIGHT]=true;
            }
        }
    }

    if(!collision_check(target_x,target_y,target_w,target_h,x,y,w,h)){
        if(fabs(y-target_y)>4){
            if(y>target_y){
                ai_keystates[COMMAND_UP]=true;
            }
            else if(y<target_y){
                ai_keystates[COMMAND_DOWN]=true;
            }
        }
    }
}

void Player_Mp::worldmap_handle_input_states(){
    //************************//
    // Handle direction keys: //
    //************************//

    //If the left directional key is pressed.
    if(command_state(COMMAND_LEFT)){
        //Set their move state to match the directional key.
        move_state=LEFT;

        //Set their facing variable to match the directional key.
        facing=LEFT;
    }

    //If the up directional key is pressed.
    if(command_state(COMMAND_UP)){
        //If the move_state is changing, reset the animation frames.
        move_state=UP;

        facing=UP;
    }

    //If the right directional key is pressed.
    if(command_state(COMMAND_RIGHT)){
        //If the move_state is changing, reset the animation frames.
        //Set their move state to match the directional key.
        move_state=RIGHT;

        //Set their facing variable to match the directional key.
        facing=RIGHT;
    }

    //If the down directional key is pressed.
    if(command_state(COMMAND_DOWN)){
        //If the move_state is changing, reset the animation frames.
        move_state=DOWN;

        facing=DOWN;
    }

    //*******************************************************//
    // Handle multiple direction keys being pressed at once: //
    //*******************************************************//

    //Priorities:
    //Movement: Left, Up, Right, Down.

    if(command_state(COMMAND_LEFT) && command_state(COMMAND_UP)){
        move_state=LEFT_UP;
        facing=LEFT_UP;
    }
    if(command_state(COMMAND_RIGHT) && command_state(COMMAND_UP)){
        move_state=RIGHT_UP;
        facing=RIGHT_UP;
    }
    if(command_state(COMMAND_RIGHT) && command_state(COMMAND_DOWN)){
        move_state=RIGHT_DOWN;
        facing=RIGHT_DOWN;
    }
    if(command_state(COMMAND_LEFT) && command_state(COMMAND_DOWN)){
        move_state=LEFT_DOWN;
        facing=LEFT_DOWN;
    }
    if(command_state(COMMAND_LEFT) && command_state(COMMAND_RIGHT)){
        move_state=LEFT;
        facing=LEFT;
    }
    if(command_state(COMMAND_UP) && command_state(COMMAND_DOWN)){
        move_state=UP;
        facing=UP;
    }
    if(command_state(COMMAND_LEFT) && command_state(COMMAND_UP) && command_state(COMMAND_RIGHT)){
        move_state=LEFT_UP;
        facing=LEFT_UP;
    }
    if(command_state(COMMAND_LEFT) && command_state(COMMAND_DOWN) && command_state(COMMAND_RIGHT)){
        move_state=LEFT_DOWN;
        facing=LEFT_DOWN;
    }
    if(command_state(COMMAND_LEFT) && command_state(COMMAND_UP) && command_state(COMMAND_DOWN)){
        move_state=LEFT_UP;
        facing=LEFT_UP;
    }
    if(command_state(COMMAND_UP) && command_state(COMMAND_RIGHT) && command_state(COMMAND_DOWN)){
        move_state=RIGHT_UP;
        facing=RIGHT_UP;
    }
    if(command_state(COMMAND_LEFT) && command_state(COMMAND_UP) && command_state(COMMAND_RIGHT) && command_state(COMMAND_DOWN)){
        move_state=LEFT_UP;
        facing=LEFT_UP;
    }

    //If no directional keys are pressed, the player is not moving.
    if(!command_state(COMMAND_LEFT) && !command_state(COMMAND_UP) && !command_state(COMMAND_RIGHT) && !command_state(COMMAND_DOWN)){
        frame=0;
        frame_counter=0;
        move_state=0;
    }
}

void Player_Mp::worldmap_move(){
    if(!collision_check(x,y,w,h,player.camera_x,player.camera_y,player.camera_w,player.camera_h)){
        x=player.x;
        y=player.y;
    }
    else{
        //************************************//
        // Handle x-axis and y-axis movement: //
        //************************************//

        double run_chunk;
        if(fabs(worldmap_run_speed)<pixel_safety_y){
            run_chunk=fabs(worldmap_run_speed);
        }
        else{
            run_chunk=pixel_safety_y;
        }

        for(double i=fabs(worldmap_run_speed);i>0;){
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

            worldmap_handle_events();
        }
    }
}

void Player_Mp::worldmap_handle_events(){
    prepare_for_events();

    //The current tile location for the player.
    int player_current_x=(int)((int)x/TILE_SIZE);
    int player_current_y=(int)((int)y/TILE_SIZE);

    int check_x_start=player_current_x-4;
    int check_x_end=player_current_x+4;
    int check_y_start=player_current_y-4;
    int check_y_end=player_current_y+4;

    //First, we check for collisions with tiles.

    for(int int_y=check_y_start;int_y<check_y_end;int_y++){
        for(int int_x=check_x_start;int_x<check_x_end;int_x++){
            //As long as the current tile is within the level's boundaries.
            if(int_x>=0 && int_x<=(level.level_x/TILE_SIZE)-1 && int_y>=0 && int_y<=(level.level_y/TILE_SIZE)-1){
                //If the tile is a solid, non-sloping tile.
                if((level.tile_array[int_x][int_y].solidity==TILE_SOLIDITY_SOLID || (level.tile_array[int_x][int_y].solidity==TILE_SOLIDITY_CLOUD && !command_state(COMMAND_JUMP))) &&
                   level.tile_array[int_x][int_y].slope==180 && !touching_sloped_ground_last_check){
                    //First, we check for an x-axis collision. For x-axis collision, we are checking this part of the player's rectangle:
                    //|----|
                    //xxxxxx
                    //xxxxxx
                    //|----|
                    if(collision_check(x,y+5,w,h-10,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE)){
                        //If the left of the player is left of the tile's left side, we know the player is hitting the left side of the tile.
                        if(x<level.tile_array[int_x][int_y].x){
                            //We set the player's x value to the x value of the tile - the player's width, so the player will now be on the left side of the tile.
                            x=level.tile_array[int_x][int_y].x-w;
                        }
                        //If the right side of the player is right of the tile's right side, we know the player is hitting the right side of the tile.
                        else if(x+w>level.tile_array[int_x][int_y].x+TILE_SIZE){
                            //We set the player's x value to the right side of the tile.
                            x=level.tile_array[int_x][int_y].x+TILE_SIZE;
                        }
                    }
                    //Next, we check for a y-axis collision. For y-axis collision, we are checking this part of the player's rectangle:
                    //|-xx-|
                    //|-xx-|
                    //|-xx-|
                    //|-xx-|
                    else if(collision_check(x+5,y,w-10,h,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE)){
                        //If the top of the player is above the tile's top side, we know the player is hitting the top side of the tile.
                        if(y<level.tile_array[int_x][int_y].y){
                            //We set the player's y value to the y value of the tile - the player's height, so the player will now be on the top side of the tile.
                            y=level.tile_array[int_x][int_y].y-h;
                        }
                        //If the bottom side of the player is below the tile's bottom side, we know the player is hitting the bottom side of the tile.
                        else if(y+h>level.tile_array[int_x][int_y].y+TILE_SIZE){
                            //We set the player's y value to the bottom side of the tile.
                            y=level.tile_array[int_x][int_y].y+TILE_SIZE;
                        }
                    }
                }
                //If the tile is a solid, sloping tile.
                else if(level.tile_array[int_x][int_y].solidity==TILE_SOLIDITY_SOLID && level.tile_array[int_x][int_y].slope!=180){
                    if(collision_check(x,y,w,h,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE)){
                        //Determine the distance from the left edge of this tile to the actor.
                        double x_position=x+w/2-level.tile_array[int_x][int_y].x;

                        short direction=DOWN;
                        double difference_top=0.0;
                        double difference_bottom=0.0;

                        if(level.tile_array[int_x][int_y].slope==45){
                            difference_top=level.tile_array[int_x][int_y].y+TILE_SIZE-h-x_position;
                            difference_bottom=level.tile_array[int_x][int_y].y+TILE_SIZE-x_position;
                            difference_top=y-difference_top;
                            difference_bottom=y-difference_bottom;
                            if(fabs(difference_top)<fabs(difference_bottom)){
                                direction=DOWN;
                            }
                            else if(fabs(difference_top)>fabs(difference_bottom)){
                                direction=UP;
                            }
                        }
                        else if(level.tile_array[int_x][int_y].slope==135){
                            difference_top=level.tile_array[int_x][int_y].y+TILE_SIZE-h-TILE_SIZE+x_position;
                            difference_bottom=level.tile_array[int_x][int_y].y+TILE_SIZE-TILE_SIZE+x_position;
                            difference_top=y-difference_top;
                            difference_bottom=y-difference_bottom;
                            if(fabs(difference_top)<fabs(difference_bottom)){
                                direction=DOWN;
                            }
                            else if(fabs(difference_top)>fabs(difference_bottom)){
                                direction=UP;
                            }
                        }

                        //If the middle of the actor is within the tile on the x-axis.
                        if(x+w/2>=level.tile_array[int_x][int_y].x && x+w/2<level.tile_array[int_x][int_y].x+TILE_SIZE){
                            if((direction==DOWN && move_state!=UP && move_state!=RIGHT_UP && move_state!=LEFT_UP && move_state!=NONE) ||
                               (direction==UP && move_state!=DOWN && move_state!=RIGHT_DOWN && move_state!=LEFT_DOWN && move_state!=NONE)){
                                if(level.tile_array[int_x][int_y].slope==45){
                                    if(direction==DOWN && y>=level.tile_array[int_x][int_y].y+TILE_SIZE-h-x_position){
                                        //Snap to the slope.
                                        y=level.tile_array[int_x][int_y].y+TILE_SIZE-h-x_position;
                                        touching_sloped_ground=true;
                                    }
                                    else if(direction==UP && y<=level.tile_array[int_x][int_y].y+TILE_SIZE-x_position){
                                        //Snap to the slope.
                                        y=level.tile_array[int_x][int_y].y+TILE_SIZE-x_position;
                                        touching_sloped_ground=true;
                                    }
                                }
                                else if(level.tile_array[int_x][int_y].slope==135){
                                    if(direction==DOWN && y>=level.tile_array[int_x][int_y].y+TILE_SIZE-h-TILE_SIZE+x_position){
                                        //Snap to the slope.
                                        y=level.tile_array[int_x][int_y].y+TILE_SIZE-h-TILE_SIZE+x_position;
                                        touching_sloped_ground=true;
                                    }
                                    else if(direction==UP && y<=level.tile_array[int_x][int_y].y+TILE_SIZE-TILE_SIZE+x_position){
                                        //Snap to the slope.
                                        y=level.tile_array[int_x][int_y].y+TILE_SIZE-TILE_SIZE+x_position;
                                        touching_sloped_ground=true;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    //Keep the player in the level map's bounds.
    if(x<0){
        x=0;
    }
    if(x+w>level.level_x){
        x=level.level_x-w;
    }
    if(y<0){
        y=0;
    }
    if(y+h>level.level_y){
        y=level.level_y-h;
    }
}

void Player_Mp::worldmap_animate(){
    //Increment the frame counter.
    frame_counter_idle++;
    frame_counter++;

    //Handle idle animation.
    if(frame_counter_idle>=15){
        frame_counter_idle=0;

        //Now increment the frame.
        frame_idle++;

        if(frame_idle>PLAYER_WORLDMAP_IDLE_SPRITES-1){
            frame_idle=0;
        }
    }

    //Handle player animation.
    if(frame_counter>=2){
        frame_counter=0;

        //Now increment the frame.
        frame++;

        //If the player is walking, play an occasional footstep sound.
        if(frame%6==0){
            short random=random_range(0,99);
            if(random<69){
                play_positional_sound(*ptr_player_worldmap_footstep,x,y,32.0);
            }
            else{
                play_positional_sound(*ptr_player_worldmap_footstep2,x,y,32.0);
            }
        }

        if(frame>PLAYER_WORLDMAP_WALK_SPRITES-1){
            frame=0;
        }
    }
}

void Player_Mp::worldmap_render(){
    if(move_state==0 && (facing==LEFT || facing==LEFT_UP || facing==LEFT_DOWN)){
        render_sprite((int)((int)x-(int)player.camera_x),(int)((int)y-(int)player.camera_y),*ptr_player_worldmap_image,&sprites_player_worldmap_idle_left[frame_idle]);
    }
    else if(move_state==0 && (facing==RIGHT || facing==RIGHT_UP || facing==RIGHT_DOWN)){
        render_sprite((int)((int)x-(int)player.camera_x),(int)((int)y-(int)player.camera_y),*ptr_player_worldmap_image,&sprites_player_worldmap_idle_right[frame_idle]);
    }
    else if(move_state==0 && facing==UP){
        render_sprite((int)((int)x-(int)player.camera_x),(int)((int)y-(int)player.camera_y),*ptr_player_worldmap_image,&sprites_player_worldmap_idle_up[frame_idle]);
    }
    else if(move_state==0 && facing==DOWN){
        render_sprite((int)((int)x-(int)player.camera_x),(int)((int)y-(int)player.camera_y),*ptr_player_worldmap_image,&sprites_player_worldmap_idle_down[frame_idle]);
    }
    else if(move_state!=0 && (facing==LEFT || facing==LEFT_UP || facing==LEFT_DOWN)){
        render_sprite((int)((int)x-(int)player.camera_x),(int)((int)y-(int)player.camera_y),*ptr_player_worldmap_image,&sprites_player_worldmap_walk_left[frame]);
    }
    else if(move_state!=0 && (facing==RIGHT || facing==RIGHT_UP || facing==RIGHT_DOWN)){
        render_sprite((int)((int)x-(int)player.camera_x),(int)((int)y-(int)player.camera_y),*ptr_player_worldmap_image,&sprites_player_worldmap_walk_right[frame]);
    }
    else if(move_state!=0 && facing==UP){
        render_sprite((int)((int)x-(int)player.camera_x),(int)((int)y-(int)player.camera_y),*ptr_player_worldmap_image,&sprites_player_worldmap_walk_up[frame]);
    }
    else if(move_state!=0 && facing==DOWN){
        render_sprite((int)((int)x-(int)player.camera_x),(int)((int)y-(int)player.camera_y),*ptr_player_worldmap_image,&sprites_player_worldmap_walk_down[frame]);
    }

    ///render_rectangle((int)(x-camera_x),(int)(y-camera_y),w,h,0.25,COLOR_RED);
    ///render_rectangle((int)(x+w/2-camera_x),(int)(y-camera_y),1,h,1.0,COLOR_GREEN);
}
