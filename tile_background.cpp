/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "tile_background.h"
#include "render.h"
#include "world.h"
#include "collision.h"

using namespace std;

Tile_Background::Tile_Background(){
    //Set the tile's variables:

    sprite=0;

    x=0;
    y=0;
}

void Tile_Background::render(){
    //If sprite is not zero (0 means no sprite), and the tile is in camera bounds, render the tile.
    if(sprite!=0 && x>=player.camera_x-TILE_SIZE && x<=player.camera_x+player.camera_w && y>=player.camera_y-TILE_SIZE && y<=player.camera_y+player.camera_h){
        render_sprite((int)((int)x-(int)player.camera_x),(int)((int)y-(int)player.camera_y),image.tiles_background,&sprites_tiles_background[sprite]);
    }
}
