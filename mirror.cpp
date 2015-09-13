/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "mirror.h"
#include "world.h"
#include "enumerations.h"
#include "collision.h"

using namespace std;

bool touching_mirror(double x,double y,double w,double h){
    //The current tile location for the actor.
    int actor_current_x=(int)((int)x/TILE_SIZE);
    int actor_current_y=(int)((int)y/TILE_SIZE);

    //Check all tiles in a square around the actor.
    int check_x_start=actor_current_x-4;
    int check_x_end=actor_current_x+4;
    int check_y_start=actor_current_y-4;
    int check_y_end=actor_current_y+4;

    for(int int_y=check_y_start;int_y<check_y_end;int_y++){
        for(int int_x=check_x_start;int_x<check_x_end;int_x++){
            //As long as the current tile is within the level's boundaries.
            if(int_x>=0 && int_x<=(level.level_x/TILE_SIZE)-1 && int_y>=0 && int_y<=(level.level_y/TILE_SIZE)-1){
                if(level.tile_array[int_x][int_y].special==TILE_SPECIAL_MIRROR){
                    if(collision_check(x,y,w,h,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE)){
                        return true;
                    }
                }
            }
        }
    }

    return false;
}
