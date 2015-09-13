/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "effect_water_splash.h"
#include "world.h"
#include "render.h"
#include "mirror.h"

using namespace std;

Effect_Water_Splash::Effect_Water_Splash(double get_x,double get_y){
    exists=true;

    dissipate=false;

    x=get_x;
    y=get_y;
    w=32;
    h=32;

    frame=0;
    frame_counter=0;

    player.existing_effects_water_splash++;
}

void Effect_Water_Splash::animate(){
    if(exists){
        //Increment the frame counter.
        frame_counter++;

        //Handle animation.
        if(frame_counter>=3){
            frame_counter=0;

            //Now increment the frame.
            if(!dissipate){
                frame++;
            }
            else{
                frame--;
            }

            if(frame>2){
                frame=2;
                dissipate=true;
            }

            if(frame<0){
                exists=false;
                player.existing_effects_water_splash--;
            }
        }
    }
}

void Effect_Water_Splash::render(bool mirrored){
    if(exists){
        //Render:
        if(x>=player.camera_x-w && x<=player.camera_x+player.camera_w && y>=player.camera_y-h && y<=player.camera_y+player.camera_h){
            double render_x=x;
            double render_y=y;
            if(mirrored && touching_mirror(x+MIRROR_DISTANCE_X,y+MIRROR_DISTANCE_Y,w,h)){
                render_x+=MIRROR_DISTANCE_X;
                render_y+=MIRROR_DISTANCE_Y;
            }

            double opacity=0.25;
            if(player.deadly_water){
                opacity=0.65;
            }

            render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),image.water_tiles,&sprites_effect_water_splash[frame],opacity);
        }
    }
}
