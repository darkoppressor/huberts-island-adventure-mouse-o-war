/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "spawner.h"
#include "world.h"
#include "collision.h"

using namespace std;

Spawner::Spawner(short get_spawner_type,short get_type,double get_x,double get_y,bool get_disallow_doubles,bool get_items_stay){
    spawner_type=get_spawner_type;
    type=get_type;
    x=get_x;
    y=get_y;
    disallow_doubles=get_disallow_doubles;
    items_stay=get_items_stay;
}

bool Spawner::allow_spawn(){
    if(disallow_doubles){
        bool allow=true;

        if(spawner_type==SPAWN_ITEM){
            for(int i=0;i<vector_items.size();i++){
                if(vector_items[i].exists && type==vector_items[i].type &&
                   collision_check(x,y,ITEM_W,ITEM_H,vector_items[i].x,vector_items[i].y,vector_items[i].w,vector_items[i].h)){
                    allow=false;
                    break;
                }
            }
        }
        else if(spawner_type==SPAWN_NPC){
            for(int i=0;i<vector_npcs.size();i++){
                if(vector_npcs[i].exists && type==vector_npcs[i].type &&
                   collision_check(x,y,ITEM_W,ITEM_H,vector_npcs[i].x,vector_npcs[i].y,vector_npcs[i].w,vector_npcs[i].h)){
                    allow=false;
                    break;
                }
            }
        }

        return allow;
    }
    else{
        return true;
    }
}

void Spawner::spawn_object(short object_type){
    if(spawner_type==object_type){
        if(object_type==SPAWN_ITEM){
            if(fabs(x-player.cam_focused_x())>=SPAWN_RANGE || fabs(y-player.cam_focused_y())>=SPAWN_RANGE){
                if(random_range(0,99)<level.survival_spawn_items_chance()){
                    if(allow_spawn()){
                        vector_items.push_back(Item(x,y,!items_stay,type,0,false));
                    }
                }
            }
        }
        else if(object_type==SPAWN_NPC){
            if(fabs(x-player.cam_focused_x())>=SPAWN_RANGE || fabs(y-player.cam_focused_y())>=SPAWN_RANGE){
                if(random_range(0,99)<level.survival_spawn_npcs_chance()){
                    if(allow_spawn()){
                        vector_npcs.push_back(Npc(x,y,type,false));
                        vector_npcs[vector_npcs.size()-1].ethereal_to_npcs=true;
                    }
                }
            }
        }
    }
}
