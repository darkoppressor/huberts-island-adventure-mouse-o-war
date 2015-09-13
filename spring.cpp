/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "spring.h"
#include "world.h"
#include "render.h"
#include "item_sprites.h"
#include "mirror.h"
#include "enumerations.h"

using namespace std;

Spring::Spring(double get_x,double get_y,double get_speed,short get_direction){
    x=get_x;
    y=get_y;
    speed=get_speed;
    direction=get_direction;

    frame=0;
    frame_counter=0;
}

void Spring::animate(){
    if(frame!=0){
        frame_counter++;

        if(frame_counter>=1){
            frame_counter=0;

            frame++;

            if(frame>SPRING_SPRITES-1){
                frame=0;
            }
        }
    }
}

void Spring::render(bool mirrored){
    if(x>=player.camera_x-SPRING_W && x<=player.camera_x+player.camera_w && y>=player.camera_y-SPRING_H && y<=player.camera_y+player.camera_h){
        double render_x=x;
        double render_y=y;
        if(mirrored && touching_mirror(x+MIRROR_DISTANCE_X,y+MIRROR_DISTANCE_Y,SPRING_W,SPRING_H)){
            render_x+=MIRROR_DISTANCE_X;
            render_y+=MIRROR_DISTANCE_Y;
        }

        double angle=0.0;

        if(direction==LEFT){
            angle=90.0;
        }
        else if(direction==UP){
            angle=0.0;
        }
        else if(direction==RIGHT){
            angle=270.0;
        }
        else if(direction==DOWN){
            angle=180.0;
        }
        else if(direction==LEFT_UP){
            angle=45.0;
        }
        else if(direction==LEFT_DOWN){
            angle=135.0;
        }
        else if(direction==RIGHT_UP){
            angle=315.0;
        }
        else if(direction==RIGHT_DOWN){
            angle=225.0;
        }

        render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),image.sprite_sheet_spring,&sprites_spring[frame],1.0,1.0,1.0,angle);

        /**double adjust_x=0.0;
        double adjust_y=0.0;
        double adjust_w=0.0;
        double adjust_h=0.0;

        if(direction==RIGHT){
            adjust_w=-11;
        }
        else if(direction==LEFT){
            adjust_x=11;
            adjust_w=-11;
        }
        else if(direction==UP){
            adjust_y=11;
            adjust_h=-11;
        }
        else if(direction==RIGHT_UP){
            adjust_y=5.5;
            adjust_w=-5.5;
            adjust_h=-5.5;
        }
        else if(direction==LEFT_UP){
            adjust_x=5.5;
            adjust_y=5.5;
            adjust_w=-5.5;
            adjust_h=-5.5;
        }
        else if(direction==DOWN){
            adjust_h=-11;
        }
        else if(direction==RIGHT_DOWN){
            adjust_w=-5.5;
            adjust_h=-5.5;
        }
        else if(direction==LEFT_DOWN){
            adjust_x=5.5;
            adjust_w=-5.5;
            adjust_h=-5.5;
        }

        render_rectangle((int)(render_x-player.camera_x),(int)(render_y-player.camera_y),SPRING_W,SPRING_H,0.5,COLOR_RED);
        render_rectangle((int)(render_x+adjust_x-player.camera_x),(int)(render_y+adjust_y-player.camera_y),SPRING_W+adjust_w,SPRING_H+adjust_h,0.5,COLOR_GREEN);*/
    }
}
