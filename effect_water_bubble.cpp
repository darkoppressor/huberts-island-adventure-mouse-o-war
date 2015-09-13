/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "effect_water_bubble.h"
#include "world.h"
#include "render.h"
#include "random_number_generator.h"
#include "collision.h"
#include "mirror.h"

using namespace std;

Effect_Water_Bubble::Effect_Water_Bubble(double get_x,double get_y){
    exists=true;

    move_speed=1;

    if(random_range(0,99)<50){
        moving_left=true;
    }
    else{
        moving_left=false;
    }

    pixels_moved=0;

    time_to_move=0;

    pop=false;

    frame_counter=0;
    frame=0;

    x=get_x;
    y=get_y;
    w=13;
    h=13;

    player.existing_effects_water_bubble++;

    play_positional_sound(sound_system.bubble_form,x,y,32.0);
}

void Effect_Water_Bubble::move(){
    //If the bubble exists but has not yet been popped.
    if(exists && !pop){
        //Random chance of the bubble popping.
        if(random_range(0,200)==42){
            pop=true;
            return;
        }

        //Move the bubble sideways.
        if(pixels_moved>=8){
            pixels_moved=0;
            moving_left=!moving_left;
        }

        if(++time_to_move>=12){
            time_to_move=0;

            if(moving_left){
                x--;
            }
            else{
                x++;
            }

            pixels_moved++;
        }

        //Translate the bubble based on its move speed.
        y-=move_speed;

        handle_events();
    }
}

void Effect_Water_Bubble::handle_events(){
    //If the bubble exists but has not yet been popped.
    if(exists && !pop){
        //The current tile location for the effect.
        int effect_current_x=(int)((int)x/TILE_SIZE);
        int effect_current_y=(int)((int)y/TILE_SIZE);

        //Check all tiles in a 1x1 square around the effect.
        int check_x_start=effect_current_x-1;
        int check_x_end=effect_current_x+1;
        int check_y_start=effect_current_y-1;
        int check_y_end=effect_current_y+1;

        //Check for collisions with tiles.

        for(int int_y=check_y_start;int_y<check_y_end;int_y++){
            for(int int_x=check_x_start;int_x<check_x_end;int_x++){
                //As long as the current tile is within the level's boundaries.
                if(int_x>=0 && int_x<=(level.level_x/TILE_SIZE)-1 && int_y>=0 && int_y<=(level.level_y/TILE_SIZE)-1){

                    //If the tile is not a water tile.
                    if(level.tile_array[int_x][int_y].special!=TILE_SPECIAL_WATER){
                        if(collision_check(x,y,w,h,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE)){
                            //Pop the bubble.
                            pop=true;
                        }
                    }
                }
            }
        }

        //If the bubble hits a level boundary, pop it.
        if(x<0){
            pop=true;
        }
        if(x+w>level.level_x){
            pop=true;
        }
        if(y<0){
            pop=true;
        }
        if(y+h>level.level_y){
            pop=true;
        }
    }
}

void Effect_Water_Bubble::animate(){
    if(exists && pop){
        //Increment the frame counter.
        frame_counter++;

        //Handle animation.
        if(frame_counter>=3){
            frame_counter=0;

            //Now increment the frame.
            frame++;

            if(frame>3){
                exists=false;
                player.existing_effects_water_bubble--;
                play_positional_sound(sound_system.bubble_pop,x,y,16.0);
            }
        }
    }
}

void Effect_Water_Bubble::render(bool mirrored){
    if(exists){
        //Render:
        if(x>=player.camera_x-w && x<=player.camera_x+player.camera_w && y>=player.camera_y-h && y<=player.camera_y+player.camera_h){
            double render_x=x;
            double render_y=y;
            if(mirrored && touching_mirror(x+MIRROR_DISTANCE_X,y+MIRROR_DISTANCE_Y,w,h)){
                render_x+=MIRROR_DISTANCE_X;
                render_y+=MIRROR_DISTANCE_Y;
            }

            render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),image.water_tiles,&sprites_effect_water_bubble[frame]);
        }
    }
}
