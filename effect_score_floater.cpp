/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "effect_score_floater.h"
#include "render.h"
#include "mirror.h"
#include "world.h"
#include "random_number_generator.h"
#include "holidays.h"

using namespace std;

Effect_Score_Floater::Effect_Score_Floater(uint64_t get_score,double get_x,double get_y){
    exists=true;

    move_speed=1.0;

    x=get_x;
    y=get_y;

    score=get_score;

    life=150;

    if(random_range(0,99)<50){
        moving_left=true;
    }
    else{
        moving_left=false;
    }

    pixels_moved=0;

    player.existing_effects_score_floater++;
}

void Effect_Score_Floater::move(){
    if(exists){
        //Move the score floater sideways.
        if(pixels_moved>=18){
            pixels_moved=0;
            moving_left=!moving_left;
        }

        if(life%3==0){
            if(moving_left){
                x--;
            }
            else{
                x++;
            }

            pixels_moved++;
        }

        y-=move_speed;

        if(--life<=0){
            exists=false;

            player.existing_effects_score_floater--;
        }
    }
}

void Effect_Score_Floater::render(bool mirrored){
    if(exists){
        ss.clear();ss.str("");ss<<score;msg=ss.str();

        if(x>=player.camera_x-(msg.length()*font.spacing_x) && x<=player.camera_x+player.camera_w && y>=player.camera_y-font.spacing_y && y<=player.camera_y+player.camera_h){
            double render_x=x;
            double render_y=y;
            if(mirrored && touching_mirror(x+MIRROR_DISTANCE_X,y+MIRROR_DISTANCE_Y,msg.length()*font.spacing_x,16)){
                render_x+=MIRROR_DISTANCE_X;
                render_y+=MIRROR_DISTANCE_Y;
            }

            font.show((int)((int)render_x+2-(int)player.camera_x),(int)((int)render_y+2-(int)player.camera_y),msg,COLOR_BLACK);
            font.show((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),msg,return_gui_color(holiday,3));
        }
        else{
            exists=false;

            player.existing_effects_score_floater--;
        }
    }
}
