/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "booster.h"
#include "world.h"
#include "render.h"
#include "item_sprites.h"
#include "mirror.h"
#include "enumerations.h"

using namespace std;

Booster::Booster(double get_x,double get_y,double get_speed,short get_direction){
    x=get_x;
    y=get_y;
    speed=get_speed;
    direction=get_direction;

    frame=0;
    frame_counter=0;
}

void Booster::animate(){
    /**if(frame!=0){
        frame_counter++;

        if(frame_counter>=1){
            frame_counter=0;

            frame++;

            if(frame>SPRING_SPRITES-1){
                frame=0;
            }
        }
    }*/
}

void Booster::render(bool mirrored){
    if(x>=player.camera_x-BOOSTER_W && x<=player.camera_x+player.camera_w && y>=player.camera_y-BOOSTER_H && y<=player.camera_y+player.camera_h){
        double render_x=x;
        double render_y=y;
        if(mirrored && touching_mirror(x+MIRROR_DISTANCE_X,y+MIRROR_DISTANCE_Y,BOOSTER_W,BOOSTER_H)){
            render_x+=MIRROR_DISTANCE_X;
            render_y+=MIRROR_DISTANCE_Y;
        }

        bool flip=false;

        if(direction==LEFT){
            flip=true;
        }

        render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),image.sprite_sheet_booster,&sprites_booster[frame],1.0,1.0,1.0,0.0,COLOR_WHITE,flip);
    }
}
