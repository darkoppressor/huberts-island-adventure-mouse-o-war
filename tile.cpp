/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "tile.h"
#include "render.h"
#include "world.h"
#include "collision.h"

using namespace std;

Tile::Tile(){
    x=0;
    y=0;
    solidity=TILE_SOLIDITY_PASSABLE;
    sprite=0;
    special=TILE_SPECIAL_NONE;
    foreground=false;
    slope=180;

    /**for(int i=0;i<TILE_SIZE;i++){
        height_mask[i]=0;
    }*/

    light_source.on=false;
    light_source.x=0;
    light_source.y=0;
    light_source.color=color_name_to_doubles(COLOR_WHITE);
    light_source.radius=6*(TILE_SIZE/LIGHTING_TILE_SIZE);
    light_source.dimness=0.0;
    light_source.falloff=0.1875/(TILE_SIZE/LIGHTING_TILE_SIZE);

    glow_direction=true;
    glow_rate=(double)random_range(24,128);
    glow_counter=0;
}

double Tile::return_x(){
    return x-TILE_SIZEITBOX_HAZARD;
}
double Tile::return_y(){
    return y-TILE_SIZEITBOX_HAZARD;
}
double Tile::return_w(){
    return TILE_SIZE+TILE_SIZEITBOX_HAZARD*2;
}
double Tile::return_h(){
    return TILE_SIZE+TILE_SIZEITBOX_HAZARD*2;
}

void Tile::set_height_mask(){
    /**if(slope==180){
        for(int i=0;i<TILE_SIZE;i++){
            height_mask[i]=0;
        }
    }
    else if(slope==45){
        for(int i=0;i<TILE_SIZE;i++){
            height_mask[i]=TILE_SIZE-1-i;
        }
    }
    else if(slope==135){
        for(int i=0;i<TILE_SIZE;i++){
            height_mask[i]=i;
        }
    }*/
}

short Tile::get_height_mask(short x_position){
    if(slope==180){
        return 0;
    }
    else if(slope==45){
        return TILE_SIZE-1-x_position;
    }
    else if(slope==135){
        return x_position;
    }
}

void Tile::render(){
    //If the tile is in camera bounds, render the tile.
    if(x>=player.camera_x-TILE_SIZE && x<=player.camera_x+player.camera_w && y>=player.camera_y-TILE_SIZE && y<=player.camera_y+player.camera_h){
        //If sprite is not zero (0 means no sprite).
        if(sprite!=0){
            //If the player is in on the world map.
            if(player.on_worldmap()){
                double scale_x=1.0;
                double scale_y=1.0;

                //If this is DevWorld.
                if(player.current_level==2){
                    scale_x=level.devworld_tiles.tiles[x/TILE_SIZE][y/TILE_SIZE].scale_x;
                    scale_y=level.devworld_tiles.tiles[x/TILE_SIZE][y/TILE_SIZE].scale_y;
                }

                //If the tile is a water tile.
                if(sprite==1 || sprite==2){
                    if(sprite==2){
                        render_sprite((int)((int)x-(int)player.camera_x),(int)((int)y-(int)player.camera_y),image.water_tiles,&worldmap_sprites_tiles[sprite],0.75,scale_x,scale_y);
                    }
                    else{
                        render_sprite((int)((int)x-(int)player.camera_x),(int)((int)y-(int)player.camera_y),image.water_tiles,&sprites_water_tiles[player.frame_water],0.75,scale_x,scale_y);
                    }
                }
                else{
                    render_sprite((int)((int)x-(int)player.camera_x),(int)((int)y-(int)player.camera_y),image.tiles,&worldmap_sprites_tiles[sprite],1.0,scale_x,scale_y);
                }
            }
            //If the player is in a level.
            else{
                if(special==TILE_SPECIAL_WATER){
                    double water_opacity=0.35;
                    if(player.deadly_water && !player.suit_deadly_water){
                        water_opacity=0.75;
                    }

                    if(sprite==2){
                        render_sprite((int)((int)x-(int)player.camera_x),(int)((int)y-(int)player.camera_y),image.water_tiles,&sprites_tiles[sprite],water_opacity);
                    }
                    else{
                        render_sprite((int)((int)x-(int)player.camera_x),(int)((int)y-(int)player.camera_y),image.water_tiles,&sprites_water_tiles[player.frame_water],water_opacity);
                    }
                }
                else{
                    double opacity=1.0;
                    if(foreground && player.get_upgrade_state("xray_specs")){
                        opacity=0.75;
                    }

                    render_sprite((int)((int)x-(int)player.camera_x),(int)((int)y-(int)player.camera_y),image.tiles,&sprites_tiles[sprite],opacity);
                }
            }

            /**if(special==TILE_SPECIAL_HAZARD){
                render_rectangle((int)(return_x()-player.camera_x),(int)(return_y()-player.camera_y),return_w(),return_h(),0.5,COLOR_RED);
            }*/
        }
    }
}

void Tile::process_glow(){
    if(light_source.on){
        double glow_rate_divisor=(double)random_range(16,64);

        if(glow_direction){
            light_source.dimness+=1.0/glow_rate/glow_rate_divisor;
            light_source.falloff+=1.0/glow_rate/glow_rate_divisor;
            if(light_source.falloff>0.2){
                light_source.falloff=0.2;
            }
            if(light_source.dimness>0.1){
                light_source.dimness=0.1;
            }
        }
        else{
            light_source.falloff-=1.0/glow_rate/glow_rate_divisor;
            light_source.dimness-=1.0/glow_rate/glow_rate_divisor;
            if(light_source.falloff<0.1){
                light_source.falloff=0.1;
            }
            if(light_source.dimness<0.0){
                light_source.dimness=0.0;
            }
        }

        if(++glow_counter>glow_rate){
            glow_counter=0;
            glow_rate=(double)random_range(24,128);
            glow_direction=!glow_direction;
        }
    }
}
