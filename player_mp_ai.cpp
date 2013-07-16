/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "player_mp.h"
#include "world.h"
#include "collision.h"
#include "ai_keys.h"
#include "distance.h"

using namespace std;

void Player_Mp::ai_decision(int our_index){
    decision_target.x=-1;
    decision_target.y=-1;
    decision_type=AI_DECISION_NONE;

    int player_index=player.cam_focused_index();

    if(counter_decision_cooldown_revive==0){
        bool anyone_dead=false;
        if(player.DYING){
            anyone_dead=true;
        }

        for(int i=0;i<mp_players.size() && !anyone_dead;i++){
            if(this!=&mp_players[i] && mp_players[i].DYING){
                anyone_dead=true;

                break;
            }
        }

        if(anyone_dead){
            counter_decision_cooldown_revive=AI_DECISION_COOLDOWN_REVIVE;

            double nearest_distance=-1.0;
            int nearest_item=-1;

            for(int i=0;i<vector_items.size();i++){
                if(fabs(vector_items[i].x-x)<PROCESS_RANGE && fabs(vector_items[i].y-y)<PROCESS_RANGE){
                    if(vector_items[i].exists &&
                       (vector_items[i].type==ITEM_SPAWNPOINT || vector_items[i].type==ITEM_CHECKPOINT || vector_items[i].type==ITEM_ENDPOINT)){
                        double this_distance=distance_between_points(x,y,vector_items[i].x,vector_items[i].y);

                        if(nearest_distance==-1.0 || this_distance<nearest_distance){
                            nearest_distance=this_distance;
                            nearest_item=i;
                        }
                    }
                }
            }

            if(nearest_item!=-1){
                decision_target.x=(int)((int)vector_items[nearest_item].x/TILE_SIZE);
                decision_target.y=(int)(((int)vector_items[nearest_item].y+(int)vector_items[nearest_item].h/2)/TILE_SIZE);
                decision_type=AI_DECISION_REVIVE;
            }
        }
    }

    //If we failed to find a nearby respawn point.
    if(decision_type!=AI_DECISION_REVIVE){
        //If we are the camera focused player.
        if(player_index==our_index){
            if(player.game_mode==GAME_MODE_MP_ADVENTURE){
                int tile_location_x=(int)((int)x/TILE_SIZE);
                int tile_location_y=(int)((int)y/TILE_SIZE);

                do{
                    if(random_range(0,99)<5){
                        decision_target.x=random_range(tile_location_x-6,tile_location_x+10);
                    }
                    else{
                        decision_target.x=random_range(tile_location_x-2,tile_location_x+10);
                    }

                    if(random_range(0,99)<5){
                        decision_target.y=random_range(tile_location_y-10,tile_location_y+10);
                    }
                    else if(random_range(0,99)<5){
                        decision_target.y=random_range(tile_location_y-2,tile_location_y+2);
                    }
                    else{
                        decision_target.y=random_range(tile_location_y-1,tile_location_y+1);
                    }

                    decision_type=AI_DECISION_WANDER;
                }
                while(!(decision_target.x>=0 && decision_target.x<=(level.level_x/TILE_SIZE)-1 && decision_target.y>=0 && decision_target.y<=(level.level_y/TILE_SIZE)-1));
            }
            else if(player.game_mode==GAME_MODE_MP_SURVIVAL){
                int tile_location_x=(int)((int)x/TILE_SIZE);
                int tile_location_y=(int)((int)y/TILE_SIZE);

                do{
                    decision_target.x=random_range(tile_location_x-10,tile_location_x+10);

                    if(random_range(0,99)<5){
                        decision_target.y=random_range(tile_location_y-10,tile_location_y+10);
                    }
                    else if(random_range(0,99)<5){
                        decision_target.y=random_range(tile_location_y-2,tile_location_y+2);
                    }
                    else{
                        decision_target.y=random_range(tile_location_y-1,tile_location_y+1);
                    }

                    decision_type=AI_DECISION_WANDER;
                }
                while(!(decision_target.x>=0 && decision_target.x<=(level.level_x/TILE_SIZE)-1 && decision_target.y>=0 && decision_target.y<=(level.level_y/TILE_SIZE)-1));
            }
        }
        else{
            double PLAYER_X=player.x;
            double PLAYER_Y=player.y;
            double PLAYER_WIDTH=player.w;
            double PLAYER_HEIGHT=player.h;
            if(player_index>=0){
                PLAYER_X=mp_players[player_index].x;
                PLAYER_Y=mp_players[player_index].y;
                PLAYER_WIDTH=mp_players[player_index].w;
                PLAYER_HEIGHT=mp_players[player_index].h;
            }

            decision_target.x=(int)(((int)PLAYER_X+(int)PLAYER_WIDTH/2)/TILE_SIZE);
            decision_target.y=(int)(((int)PLAYER_Y+(int)PLAYER_HEIGHT-TILE_SIZE/2)/TILE_SIZE);
            decision_type=AI_DECISION_FOLLOW;
        }

        int item_chance=20;
        if(decision_type==AI_DECISION_FOLLOW){
            double PLAYER_MOVING=player.IN_AIR || player.underwater || player.run_speed!=0.0;
            if(player_index>=0){
                PLAYER_MOVING=mp_players[player_index].IN_AIR || mp_players[player_index].underwater || mp_players[player_index].run_speed!=0.0;
            }

            if(!PLAYER_MOVING){
                if(player.game_mode==GAME_MODE_MP_ADVENTURE){
                    item_chance=40;
                }
                else if(player.game_mode==GAME_MODE_MP_SURVIVAL){
                    item_chance=80;
                }
            }
            else{
                if(player.game_mode==GAME_MODE_MP_ADVENTURE){
                    item_chance=5;
                }
                else if(player.game_mode==GAME_MODE_MP_SURVIVAL){
                    item_chance=60;
                }
            }
        }

        if(random_range(0,99)<item_chance){
            double nearest_distance=-1.0;
            int nearest_item=-1;

            for(int i=0;i<vector_items.size();i++){
                if(fabs(vector_items[i].x-x)<PROCESS_RANGE && fabs(vector_items[i].y-y)<PROCESS_RANGE){
                    if(vector_items[i].exists &&
                       !(vector_items[i].type==ITEM_SPAWNPOINT || vector_items[i].type==ITEM_CHECKPOINT || vector_items[i].type==ITEM_ENDPOINT)){
                        double this_distance=distance_between_points(x,y,vector_items[i].x,vector_items[i].y);

                        if(nearest_distance==-1.0 || this_distance<nearest_distance){
                            nearest_distance=this_distance;
                            nearest_item=i;
                        }
                    }
                }
            }

            if(nearest_item!=-1){
                decision_target.x=(int)((int)vector_items[nearest_item].x/TILE_SIZE);
                decision_target.y=(int)(((int)vector_items[nearest_item].y+(int)vector_items[nearest_item].h/2)/TILE_SIZE);
                decision_type=AI_DECISION_ITEM;
            }
        }
    }
}

void Player_Mp::ai_update_path(){
    if(!DYING && !bubble_mode && decision_type!=AI_DECISION_NONE){
        ai_path_to_target(decision_target.x,decision_target.y);
    }
}

bool Player_Mp::ai_tile_is_air(Tile* tile){
    if(tile->solidity==TILE_SOLIDITY_PASSABLE &&
       (tile->special==TILE_SPECIAL_NONE || tile->special==TILE_SPECIAL_MIRROR || tile->special==TILE_SPECIAL_ITEM_HOLDER)){
        return true;
    }
    else{
        return false;
    }
}

void Player_Mp::ai(int our_index){
    if(ai_controlled){
        if(counter_path_update>0){
            counter_path_update--;
        }
        if(counter_path_giveup>0){
            counter_path_giveup--;
        }
        if(counter_decision_cooldown_revive>0){
            counter_decision_cooldown_revive--;
        }
        if(counter_path_far>0){
            counter_path_far--;
        }
        if(counter_path_medium>0){
            counter_path_medium--;
        }

        if(counter_path_giveup==0){
            path.clear();
        }

        if(path.size()==0 && counter_path_update==0){
            counter_path_update=AI_PATH_UPDATE_RATE;
            counter_path_giveup=AI_PATH_GIVEUP;

            ai_decision(our_index);

            ai_update_path();
        }

        //Reset AI keystates.
        for(short i=0;i<256;i++){
            ai_keystates[i]=false;
        }
        ai_key_events.clear();

        if(!DYING && !bubble_mode){
            for(int i=path.size()-1;i>=0;i--){
                bool check_for_collide=false;

                Tile* tile=&level.tile_array[path[i].coords.x][path[i].coords.y];
                Tile* tile_below=0;
                if(path[i].coords.y+1<=(level.level_y/TILE_SIZE)-1){
                    tile_below=&level.tile_array[path[i].coords.x][path[i].coords.y+1];
                }

                if(ai_tile_is_air(tile)){
                    if(jump_state && tile_below!=0 && tile_below->solidity!=TILE_SOLIDITY_PASSABLE){
                        if(y+h<path[i].coords.y*TILE_SIZE+TILE_SIZE/2){
                            check_for_collide=true;
                        }
                    }
                    else{
                        check_for_collide=true;
                    }
                }
                else{
                    check_for_collide=true;
                }

                ///Maybe check the mid of the player against the mid of the node here.
                if(check_for_collide && collision_check(x,y,w,h,path[i].coords.x*TILE_SIZE+TILE_SIZE/4,path[i].coords.y*TILE_SIZE+TILE_SIZE/4,TILE_SIZE/2,TILE_SIZE/2)){
                    counter_path_giveup=AI_PATH_GIVEUP;

                    for(int n=i;n<path.size();n++){
                        path.erase(path.begin()+n);
                        n--;
                    }
                }
            }

            ai_move_to_path();

            ai_jump();

            ai_shoot();
        }
        else if(!DYING && bubble_mode){
            double AI_BUBBLE_POP_RANGE=192.0;
            if(player.cam_focused_dying() || (fabs(x-player.cam_focused_x())<AI_BUBBLE_POP_RANGE && fabs(y-player.cam_focused_y())<AI_BUBBLE_POP_RANGE)){
                ai_keystates[COMMAND_JUMP]=true;
                ai_key_events.push_back(COMMAND_JUMP);
            }
        }
    }
}

int Player_Mp::ai_tile_on_list(vector<Path_Calc>* path_list,int tile_x,int tile_y){
    for(int i=0;i<path_list->size();i++){
        if(tile_x==path_list->at(i).coords.x && tile_y==path_list->at(i).coords.y){
            return i;
        }
    }

    return -1;
}

bool Player_Mp::ai_tile_valid(int int_x,int int_y,int target_x,int target_y){
    if(int_x==target_x && int_y==target_y){
        return true;
    }

    Tile* tile=&level.tile_array[int_x][int_y];

    if(tile->slope!=180){
        return true;
    }

    if(tile->solidity!=TILE_SOLIDITY_SOLID && tile->special!=TILE_SPECIAL_HAZARD){
        if(tile->special!=TILE_SPECIAL_WATER || (tile->special==TILE_SPECIAL_WATER && (!player.deadly_water || (player.deadly_water && suit_deadly_water)) && (swimming_gear || underwater))){
            return true;
        }
    }

    return false;
}

int Player_Mp::ai_get_gravity_cost(int int_x,int int_y){
    double fake_vel=-jump_max;
    double fake_move=0.0;
    while(fake_vel<0.0){
        fake_move+=fake_vel;

        fake_vel+=gravity;
    }

    int max_tile_height=(int)floor(fake_move/(double)TILE_SIZE);

    int tile_height=0;

    for(int check_y=int_y;check_y<=int_y+5;check_y++){
        if(int_x>=0 && int_x<=(level.level_x/TILE_SIZE)-1 && check_y>=0 && check_y<=(level.level_y/TILE_SIZE)-1){
            Tile* tile=&level.tile_array[int_x][check_y];

            if(tile->solidity==TILE_SOLIDITY_SOLID || tile->solidity==TILE_SOLIDITY_CLOUD || tile->special==TILE_SPECIAL_CLIMBABLE_TOP ||
               tile->special==TILE_SPECIAL_WATER){
                break;
            }

            tile_height++;
        }
        else{
            break;
        }
    }

    return tile_height*tile_height;
}

int Player_Mp::ai_get_tile_cost(int int_x,int int_y){
    Tile* tile=&level.tile_array[int_x][int_y];

    if(tile->special==TILE_SPECIAL_CLIMBABLE || tile->special==TILE_SPECIAL_CLIMBABLE_TOP){
        return AI_PATH_COST_TILE_ROPE;
    }
    else if(tile->special==TILE_SPECIAL_WATER){
        return AI_PATH_COST_TILE_WATER;
    }
    else{
        return AI_PATH_COST_TILE_NORMAL;
    }
}

void Player_Mp::ai_add_open_tiles(Path_Calc* parent,int target_x,int target_y,vector<Path_Calc>* open_list,vector<Path_Calc>* closed_list,vector<Path_Calc>* new_open_tiles){
    for(int int_x=parent->coords.x-1;int_x<=parent->coords.x+1;int_x++){
        for(int int_y=parent->coords.y-1;int_y<=parent->coords.y+1;int_y++){
            if(int_x!=parent->coords.x || int_y!=parent->coords.y){
                if(int_x>=0 && int_x<=(level.level_x/TILE_SIZE)-1 && int_y>=0 && int_y<=(level.level_y/TILE_SIZE)-1){
                    int on_open_list=ai_tile_on_list(open_list,int_x,int_y);

                    if(on_open_list==-1 && ai_tile_on_list(closed_list,int_x,int_y)==-1 && ai_tile_valid(int_x,int_y,target_x,target_y)){
                        bool orth=false;
                        if(int_x==parent->coords.x || int_y==parent->coords.y){
                            orth=true;
                        }

                        bool slope=false;
                        if(level.tile_array[int_x][int_y].slope!=180){
                            slope=true;
                        }

                        int cost_g=0;
                        if(slope){
                            cost_g=AI_PATH_COST_SLOPE;
                        }
                        else if(orth){
                            cost_g=AI_PATH_COST_ORTH;
                        }
                        else{
                            cost_g=AI_PATH_COST_DIAG;
                        }
                        cost_g+=parent->cost_g;
                        cost_g*=ai_get_tile_cost(int_x,int_y);
                        if(ai_tile_is_air(&level.tile_array[int_x][int_y]) && y>=int_y*TILE_SIZE+TILE_SIZE/2){
                            cost_g*=ai_get_gravity_cost(int_x,int_y);
                        }

                        int cost_h=((int)fabs(target_x-int_x)+(int)fabs(target_y-int_y))*10;

                        new_open_tiles->push_back(Path_Calc(int_x,int_y,parent->coords.x,parent->coords.y,cost_g,cost_g+cost_h));
                    }
                    else if(on_open_list!=-1){
                        bool orth=false;
                        if(int_x==parent->coords.x || int_y==parent->coords.y){
                            orth=true;
                        }

                        bool slope=false;
                        if(level.tile_array[int_x][int_y].slope!=180){
                            slope=true;
                        }

                        int cost_g=0;
                        if(slope){
                            cost_g=AI_PATH_COST_SLOPE;
                        }
                        else if(orth){
                            cost_g=AI_PATH_COST_ORTH;
                        }
                        else{
                            cost_g=AI_PATH_COST_DIAG;
                        }
                        cost_g+=parent->cost_g;
                        cost_g*=ai_get_tile_cost(int_x,int_y);
                        if(ai_tile_is_air(&level.tile_array[int_x][int_y]) && y>=int_y*TILE_SIZE+TILE_SIZE/2){
                            cost_g*=ai_get_gravity_cost(int_x,int_y);
                        }

                        if(cost_g<open_list->at(on_open_list).cost_g){
                            open_list->at(on_open_list).parent.x=parent->coords.x;
                            open_list->at(on_open_list).parent.y=parent->coords.y;

                            int cost_h=((int)fabs(target_x-int_x)+(int)fabs(target_y-int_y))*10;

                            open_list->at(on_open_list).cost_g=cost_g;
                            open_list->at(on_open_list).cost_f=cost_g+cost_h;
                        }
                    }
                }
            }
        }
    }
}

void Player_Mp::ai_path_to_target(int target_x,int target_y){
    int current_x=(int)(((int)x+(int)w/2)/TILE_SIZE);
    int current_y=(int)(((int)y+(int)h-TILE_SIZE/2)/TILE_SIZE);

    path.clear();

    if(target_x>=0 && target_x<=(level.level_x/TILE_SIZE)-1 && target_y>=0 && target_y<=(level.level_y/TILE_SIZE)-1 &&
       current_x>=0 && current_x<=(level.level_x/TILE_SIZE)-1 && current_y>=0 && current_y<=(level.level_y/TILE_SIZE)-1 &&
       (current_x!=target_x || current_y!=target_y)){
        vector<Path_Calc> open_list;
        vector<Path_Calc> closed_list;

        int cost_h=((int)fabs(target_x-current_x)+(int)fabs(target_y-current_y))*10;
        open_list.push_back(Path_Calc(current_x,current_y,-1,-1,0,cost_h));

        int path_range=AI_PATH_RANGE_FAR;
        if(counter_path_far>0){
            path_range=AI_PATH_RANGE_MEDIUM;

            if(counter_path_medium>0){
                path_range=AI_PATH_RANGE_SHORT;
            }
        }

        for(int tile_checks=0;tile_checks<path_range;tile_checks++){
            Path_Calc* lowest_f_cost=0;
            int lowest_f_cost_index=-1;
            for(int i=0;i<open_list.size();i++){
                if(lowest_f_cost==0 || (open_list[i].coords.x==target_x && open_list[i].coords.y==target_y) || open_list[i].cost_f<=lowest_f_cost->cost_f){
                    lowest_f_cost=&open_list[i];
                    lowest_f_cost_index=i;
                }
            }

            if(lowest_f_cost!=0){
                //We've pathed to the target, so construct our final path.
                if(lowest_f_cost->coords.x==target_x && lowest_f_cost->coords.y==target_y){
                    path.push_back(Path(target_x,target_y));
                    int path_tile=ai_tile_on_list(&closed_list,lowest_f_cost->parent.x,lowest_f_cost->parent.y);

                    do{
                        path.push_back(Path(closed_list[path_tile].coords.x,closed_list[path_tile].coords.y));

                        path_tile=ai_tile_on_list(&closed_list,closed_list[path_tile].parent.x,closed_list[path_tile].parent.y);
                    }
                    while(path_tile!=-1);

                    break;
                }
                else{
                    vector<Path_Calc> new_open_tiles;
                    ai_add_open_tiles(lowest_f_cost,target_x,target_y,&open_list,&closed_list,&new_open_tiles);

                    closed_list.push_back(*lowest_f_cost);
                    open_list.erase(open_list.begin()+lowest_f_cost_index);

                    for(int i=0;i<new_open_tiles.size();i++){
                        open_list.push_back(new_open_tiles[i]);
                    }
                }
            }
            //No lowest f cost found in the open list, so we've failed to find a path.
            else{
                break;
            }
        }

        //If we failed to find a path.
        if(path.size()==0){
            if(counter_path_far==0){
                counter_path_far=AI_PATH_COOLDOWN_FAR;
            }
            else{
                counter_path_far=AI_PATH_COOLDOWN_FAR;
                counter_path_medium=AI_PATH_COOLDOWN_MEDIUM;
            }
        }
    }
}

void Player_Mp::ai_move_to_path(){
    if(path.size()>0){
        Path* next_path=&path[path.size()-1];
        double next_coord_x=next_path->coords.x*TILE_SIZE+TILE_SIZE/2;
        double next_coord_y=next_path->coords.y*TILE_SIZE+TILE_SIZE/2;

        if(jump_state && facing==LEFT){
            ai_keystates[COMMAND_LEFT]=true;
        }
        else if(jump_state && facing==RIGHT){
            ai_keystates[COMMAND_RIGHT]=true;
        }
        else if(x>=next_coord_x){
            ai_keystates[COMMAND_LEFT]=true;
        }
        else if(x<next_coord_x){
            ai_keystates[COMMAND_RIGHT]=true;
        }

        if(SWIMMING){
            if(y>=next_coord_y){
                ai_keystates[COMMAND_UP]=true;

                if(SWIM_CAN_JUMP){
                    ai_keystates[COMMAND_JUMP]=true;
                    ai_key_events.push_back(COMMAND_JUMP);
                }
            }
            else if(y<next_coord_y){
                ai_keystates[COMMAND_DOWN]=true;
            }
        }
        else{
            bool lets_climb=false;

            if(touching_sloped_ground && path.size()>1 && y+h<path[path.size()-2].coords.y*TILE_SIZE+TILE_SIZE/2){
                ai_keystates[COMMAND_DOWN]=true;
            }
            else if(on_cloud && (y+h<next_coord_y)){
                ai_keystates[COMMAND_DOWN]=true;
                ai_keystates[COMMAND_JUMP]=true;
                ai_key_events.push_back(COMMAND_JUMP);
            }
            else if(on_climbable_top_tile && path.size()>1 && (level.tile_array[path[path.size()-2].coords.x][path[path.size()-2].coords.y].special==TILE_SPECIAL_CLIMBABLE ||
                                      level.tile_array[path[path.size()-2].coords.x][path[path.size()-2].coords.y].special==TILE_SPECIAL_CLIMBABLE_TOP) &&
                    y+h<path[path.size()-2].coords.y*TILE_SIZE+TILE_SIZE/2){
                lets_climb=true;
                ai_keystates[COMMAND_DOWN]=true;
            }
            else if(y>=next_coord_y && (level.tile_array[next_path->coords.x][next_path->coords.y].special==TILE_SPECIAL_CLIMBABLE ||
                                                         level.tile_array[next_path->coords.x][next_path->coords.y].special==TILE_SPECIAL_CLIMBABLE_TOP)){
                lets_climb=true;
                ai_keystates[COMMAND_UP]=true;
            }
            else if(y>=next_coord_y && ((run_speed<0 && x>=next_coord_x) || (run_speed>0 && x<next_coord_x)) &&
                    (!touching_sloped_ground || (touching_sloped_ground &&
                                                 fabs(x-next_coord_x)<TILE_SIZE))){
                ai_keystates[COMMAND_JUMP]=true;
                ai_key_events.push_back(COMMAND_JUMP);
            }

            if(!lets_climb && CLIMBING){
                ai_keystates[COMMAND_JUMP]=true;
                ai_key_events.push_back(COMMAND_JUMP);
            }
        }
    }
}

void Player_Mp::ai_jump(){
    if(path.size()>0){
        if(jump_state && air_velocity<0.0){
            if(IN_AIR && ((path.size()==1 && y>=path[path.size()-1].coords.y*TILE_SIZE+TILE_SIZE/2) ||
                          (path.size()>1 && y>=path[path.size()-2].coords.y*TILE_SIZE+TILE_SIZE/2))){
                ai_keystates[COMMAND_JUMP]=true;
            }
            else{
                ai_keystates[COMMAND_JUMP]=false;
            }
        }

        if(!jump_state && j_wing && extra_jumps>0 && ((path.size()==1 && y>=path[path.size()-1].coords.y*TILE_SIZE+TILE_SIZE/2) ||
                                     (path.size()>1 && y>=path[path.size()-2].coords.y*TILE_SIZE+TILE_SIZE/2))){
            ai_keystates[COMMAND_JUMP]=true;
            ai_key_events.push_back(COMMAND_JUMP);
        }
    }
    else{
        ai_keystates[COMMAND_JUMP]=false;
    }
}

void Player_Mp::ai_shoot(){
    short shot_cost=1;
    if(current_shot==SHOT_PLAYER){
        shot_cost=SHOT_COST_PLAYER;
    }
    else if(current_shot==SHOT_PLAYER_SPLODE){
        shot_cost=SHOT_COST_PLAYER_SPLODE;
    }
    else if(current_shot==SHOT_PLAYER_HOMING){
        shot_cost=SHOT_COST_PLAYER_HOMING;
    }

    if(!SWIMMING && !SHOOTING && !DYING && !bubble_mode && ammo>=shot_cost){
        int shoot_range=768;

        for(int i=0;i<vector_npcs.size();i++){
            if(fabs(vector_npcs[i].x-x)<shoot_range && fabs(vector_npcs[i].y-y)<shoot_range){
                if(vector_npcs[i].exists && !vector_npcs[i].counts_as_trap && !vector_npcs[i].invulnerable &&
                   !vector_npcs[i].friendly && vector_npcs[i].counter_targeted==0){
                    int shot_x=(int)((int)x/TILE_SIZE);
                    int shot_y=(int)((int)y/TILE_SIZE);

                    int npc_x=(int)((int)vector_npcs[i].x/TILE_SIZE);
                    int npc_y=(int)((int)vector_npcs[i].y/TILE_SIZE);

                    int dx=abs(npc_x-shot_x);
                    int dy=abs(npc_y-shot_y);

                    int sx=-1;
                    if(shot_x<npc_x){
                        sx=1;
                    }
                    int sy=-1;
                    if(shot_y<npc_y){
                        sy=1;
                    }

                    int err=dx-dy;
                    int err2=0;

                    bool solid_in_way=false;

                    while(true){
                        if(shot_x>=0 && shot_x<=(level.level_x/TILE_SIZE)-1 && shot_y>=0 && shot_y<=(level.level_y/TILE_SIZE)-1){
                            Tile* tile=&level.tile_array[shot_x][shot_y];
                            if(tile->solidity==TILE_SOLIDITY_SOLID){
                                solid_in_way=true;
                                break;
                            }
                        }

                        if(shot_x==npc_x && shot_y==npc_y){
                            break;
                        }

                        err2=err*2;

                        if(err2>-dy){
                            err-=dy;
                            shot_x+=sx;
                        }

                        if(shot_x==npc_x && shot_y==npc_y){
                            if(shot_x>=0 && shot_x<=(level.level_x/TILE_SIZE)-1 && shot_y>=0 && shot_y<=(level.level_y/TILE_SIZE)-1){
                                Tile* tile=&level.tile_array[shot_x][shot_y];
                                if(tile->solidity==TILE_SOLIDITY_SOLID){
                                    solid_in_way=true;
                                }
                            }

                            break;
                        }

                        if(err2<dx){
                            err+=dx;
                            shot_y+=sy;
                        }
                    }

                    if(!solid_in_way){
                        if(fabs(x-vector_npcs[i].x)<TILE_SIZE){
                            if(y<vector_npcs[i].y && IN_AIR){
                                ai_keystates[COMMAND_UP]=false;
                                ai_keystates[COMMAND_LEFT]=false;
                                ai_keystates[COMMAND_RIGHT]=false;
                                vector_npcs[i].counter_targeted=AI_NPC_TARGET_COOLDOWN;

                                ai_keystates[COMMAND_DOWN]=true;
                                ai_key_events.push_back(COMMAND_SHOOT);

                                break;
                            }
                            else if(y>=vector_npcs[i].y){
                                ai_keystates[COMMAND_DOWN]=false;
                                ai_keystates[COMMAND_LEFT]=false;
                                ai_keystates[COMMAND_RIGHT]=false;
                                vector_npcs[i].counter_targeted=AI_NPC_TARGET_COOLDOWN;

                                ai_keystates[COMMAND_UP]=true;
                                ai_key_events.push_back(COMMAND_SHOOT);

                                break;
                            }
                        }
                        else if(fabs(y-vector_npcs[i].y)<TILE_SIZE){
                            if(x<vector_npcs[i].x){
                                ai_keystates[COMMAND_UP]=false;
                                ai_keystates[COMMAND_LEFT]=false;
                                ai_keystates[COMMAND_DOWN]=false;
                                vector_npcs[i].counter_targeted=AI_NPC_TARGET_COOLDOWN;

                                ai_keystates[COMMAND_RIGHT]=true;
                                ai_key_events.push_back(COMMAND_SHOOT);

                                break;
                            }
                            else if(x>=vector_npcs[i].x){
                                ai_keystates[COMMAND_UP]=false;
                                ai_keystates[COMMAND_DOWN]=false;
                                ai_keystates[COMMAND_RIGHT]=false;
                                vector_npcs[i].counter_targeted=AI_NPC_TARGET_COOLDOWN;

                                ai_keystates[COMMAND_LEFT]=true;
                                ai_key_events.push_back(COMMAND_SHOOT);

                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}
