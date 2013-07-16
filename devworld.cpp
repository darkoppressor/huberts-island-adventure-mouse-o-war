/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "devworld.h"
#include "world.h"
#include "render.h"
#include "enumerations.h"

using namespace std;

void DevWorld::clear_tiles(){
    tiles.clear();
}

void DevWorld::setup(int level_x,int level_y){
    tiles.clear();
    tiles.resize(level_x/TILE_SIZE,vector<devworld_data>(level_y/TILE_SIZE));

    for(int x=0;x<tiles.size();x++){
        for(int y=0;y<tiles[x].size();y++){
            tiles[x][y].scale_x=0.1*random_range(5,20);
            tiles[x][y].scale_y=0.1*random_range(5,20);
            tiles[x][y].scale_x_increasing=random_range(0,1);
            tiles[x][y].scale_y_increasing=random_range(0,1);
        }
    }
}

void DevWorld::process(){
    for(int x=0;x<tiles.size();x++){
        for(int y=0;y<tiles[x].size();y++){
            if(tiles[x][y].scale_x_increasing){
                tiles[x][y].scale_x+=0.005;
                if(tiles[x][y].scale_x>=2.0){
                    tiles[x][y].scale_x=2.0;
                    tiles[x][y].scale_x_increasing=false;
                }
            }
            else if(!tiles[x][y].scale_x_increasing){
                tiles[x][y].scale_x-=0.005;
                if(tiles[x][y].scale_x<=0.5){
                    tiles[x][y].scale_x=0.5;
                    tiles[x][y].scale_x_increasing=true;
                }
            }

            if(tiles[x][y].scale_y_increasing){
                tiles[x][y].scale_y+=0.005;
                if(tiles[x][y].scale_y>=2.0){
                    tiles[x][y].scale_y=2.0;
                    tiles[x][y].scale_y_increasing=false;
                }
            }
            else if(!tiles[x][y].scale_y_increasing){
                tiles[x][y].scale_y-=0.005;
                if(tiles[x][y].scale_y<=0.5){
                    tiles[x][y].scale_y=0.5;
                    tiles[x][y].scale_y_increasing=true;
                }
            }
        }
    }
}
