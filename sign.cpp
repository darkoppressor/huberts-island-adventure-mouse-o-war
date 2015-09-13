/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "sign.h"
#include "world.h"
#include "render.h"
#include "item_sprites.h"
#include "mirror.h"
#include "enumerations.h"

using namespace std;

Sign::Sign(double get_x,double get_y,string get_message,short get_type,short get_font_type){
    x=get_x;
    y=get_y;

    message=get_message;

    type=get_type;
    font_type=get_font_type;
}

void Sign::render(bool mirrored){
    if(x>=player.camera_x-SIGN_W && x<=player.camera_x+player.camera_w && y>=player.camera_y-SIGN_H && y<=player.camera_y+player.camera_h){
        double render_x=x;
        double render_y=y;
        if(mirrored && touching_mirror(x+MIRROR_DISTANCE_X,y+MIRROR_DISTANCE_Y,SIGN_W,SIGN_H)){
            render_x+=MIRROR_DISTANCE_X;
            render_y+=MIRROR_DISTANCE_Y;
        }

        render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),image.sprite_sheet_sign,&sprites_signs[type]);
    }
}
