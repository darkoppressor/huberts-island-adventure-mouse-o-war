/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "npc.h"
#include "world.h"
#include "collision.h"
#include "ai_keys.h"

using namespace std;

bool Npc::water_touching_this_tile(int int_x,int int_y){
    if(level.tile_array[int_x][int_y].special==TILE_SPECIAL_WATER){
        return true;
    }
    if(int_x>0 && level.tile_array[int_x-1][int_y].special==TILE_SPECIAL_WATER){
        return true;
    }
    if(int_x<(level.level_x/TILE_SIZE)-1 && level.tile_array[int_x+1][int_y].special==TILE_SPECIAL_WATER){
        return true;
    }
    if(int_y>0 && level.tile_array[int_x][int_y-1].special==TILE_SPECIAL_WATER){
        return true;
    }
    if(int_y<(level.level_y/TILE_SIZE)-1 && level.tile_array[int_x][int_y+1].special==TILE_SPECIAL_WATER){
        return true;
    }
    if(int_x>0 && int_y>0 && level.tile_array[int_x-1][int_y-1].special==TILE_SPECIAL_WATER){
        return true;
    }
    if(int_x>0 && int_y<(level.level_y/TILE_SIZE)-1 && level.tile_array[int_x-1][int_y+1].special==TILE_SPECIAL_WATER){
        return true;
    }
    if(int_x<(level.level_x/TILE_SIZE)-1 && int_y<(level.level_y/TILE_SIZE)-1 && level.tile_array[int_x+1][int_y+1].special==TILE_SPECIAL_WATER){
        return true;
    }
    if(int_x<(level.level_x/TILE_SIZE)-1 && int_y>0 && level.tile_array[int_x+1][int_y-1].special==TILE_SPECIAL_WATER){
        return true;
    }

    return false;
}

void Npc::ai_special_attack_bee_divebomb(){
    //If the target is reached, or the npc is giving up, begin returning home.
    if(ai_notice_wall() || collision_check(x,y,w,h,special_attack_target_x+PLAYER_W/2-2,special_attack_target_y+PLAYER_H/2-2,4,4)){
        returning_home=true;
    }

    //If the npc is not yet returning home, continue special attack movement.
    if(!returning_home){
        ai_move_to_target(special_attack_target_x+PLAYER_W/2-2,special_attack_target_y+PLAYER_H/2+6,2,2);
    }

    //If the npc is returning home.
    if(returning_home){
        //Check to see if home has been reached.
        //If home has been reached, end the home return and the special attack.
        if(collision_check(x,y,w,h,home_x+w/2-2,home_y+h/2-2,4,4)){
            home_x=0;
            home_y=0;
            ai_end_special_attack();
        }
        //If home has not been reached, continue returning home.
        else{
            ai_move_to_target(home_x+w/2-2,home_y+h/2-2,4,4);
        }
    }
}

void Npc::ai_special_attack_replicate(){
    for(int i=0;i<vector_doors.size();i++){
        if(fabs(vector_doors[i].x-x)<PROCESS_RANGE && fabs(vector_doors[i].y-y)<PROCESS_RANGE){
            if(!vector_doors[i].open && collision_check(x-TILE_SIZE,y-TILE_SIZE,TILE_SIZE*3,TILE_SIZE*3,vector_doors[i].x,vector_doors[i].y,vector_doors[i].w,vector_doors[i].h)){
                vector_doors[i].toggle_open();
                break;
            }
        }
    }

    bool replicate=false;

    //Check for a nearby replicate-able target.

    //Next, we check nearby npcs.
    if(!replicate){
        for(int i=0;i<vector_npcs.size();i++){
            if(this!=&vector_npcs[i]){
                if(fabs(vector_npcs[i].x-x)<PROCESS_RANGE && fabs(vector_npcs[i].y-y)<PROCESS_RANGE){
                    if(vector_npcs[i].exists && vector_npcs[i].type!=type && collision_check(x-TILE_SIZE,y-TILE_SIZE,TILE_SIZE*3,TILE_SIZE*3,vector_npcs[i].x,vector_npcs[i].y,vector_npcs[i].w,vector_npcs[i].h)){
                        vector_npcs[i].handle_death();
                        replicate=true;
                        break;
                    }
                }
            }
        }
    }

    //Next, we check nearby traps.
    if(!replicate){
        for(int i=0;i<vector_traps.size();i++){
            if(fabs(vector_traps[i].x-x)<PROCESS_RANGE && fabs(vector_traps[i].y-y)<PROCESS_RANGE){
                if(vector_traps[i].active && collision_check(x-TILE_SIZE,y-TILE_SIZE,TILE_SIZE*3,TILE_SIZE*3,vector_traps[i].x,vector_traps[i].y,vector_traps[i].w,vector_traps[i].h)){
                    vector_traps[i].active=false;
                    vector_traps[i].dangerous=false;
                    replicate=true;
                    break;
                }
            }
        }
    }

    //Next, we check nearby items.
    if(!replicate){
        for(int i=0;i<vector_items.size();i++){
            if(fabs(vector_items[i].x-x)<PROCESS_RANGE && fabs(vector_items[i].y-y)<PROCESS_RANGE){
                if(vector_items[i].exists && collision_check(x-TILE_SIZE,y-TILE_SIZE,TILE_SIZE*3,TILE_SIZE*3,vector_items[i].x,vector_items[i].y,vector_items[i].w,vector_items[i].h)){
                    vector_items[i].exists=false;
                    replicate=true;
                    break;
                }
            }
        }
    }

    //The current tile location for the actor.
    int actor_current_x=(int)((int)x/TILE_SIZE);
    int actor_current_y=(int)((int)y/TILE_SIZE);

    //Check all tiles in a square around the actor.
    int check_x_start=actor_current_x-2;
    int check_x_end=actor_current_x+2;
    int check_y_start=actor_current_y-2;
    int check_y_end=actor_current_y+2;

    for(int int_y=check_y_start;!replicate && int_y<check_y_end;int_y++){
        for(int int_x=check_x_start;!replicate && int_x<check_x_end;int_x++){
            //As long as the current tile is within the level's boundaries.
            if(int_x>=0 && int_x<=(level.level_x/TILE_SIZE)-1 && int_y>=0 && int_y<=(level.level_y/TILE_SIZE)-1){
                if(level.tile_background_array[int_x][int_y].sprite!=0){
                    level.tile_background_array[int_x][int_y].sprite=0;
                    replicate=true;
                    break;
                }
                if(!water_touching_this_tile(int_x,int_y)){
                    if(level.tile_array[int_x][int_y].sprite!=0 || level.tile_array[int_x][int_y].solidity!=TILE_SOLIDITY_PASSABLE || level.tile_array[int_x][int_y].special!=TILE_SPECIAL_NONE){
                        level.tile_array[int_x][int_y].solidity=TILE_SOLIDITY_PASSABLE;
                        level.tile_array[int_x][int_y].sprite=0;
                        level.tile_array[int_x][int_y].special=TILE_SPECIAL_NONE;
                        level.tile_array[int_x][int_y].foreground=false;
                        level.tile_array[int_x][int_y].slope=180;
                        level.tile_array[int_x][int_y].set_height_mask();
                        replicate=true;
                        break;
                    }
                }
            }
        }
    }

    if(level.amount_replicators<100 && replicate){
        new_npcs.push_back(new_npc_data(x,y,type));
    }

    //The special attack ends either way.
    ai_end_special_attack();
}

void Npc::ai_special_attack_ghost_hide(){
    bool all_players_backs_turned=true;

    for(int i=-1;i<(int)mp_players.size();i++){
        if(!ai_player_back_turned(i)){
            all_players_backs_turned=false;
            break;
        }
    }

    if(all_players_backs_turned){
        ai_end_special_attack();
        play_positional_sound(sound_system.npc_ghost_cow_moo,x,y);
    }
}

void Npc::ai_special_attack_cat_pounce(){
    if(frame_special_attack==NPC_SPECIAL_ATTACK_SPRITES-1){
        ai_end_special_attack();
        ai_keystates[AIK_JUMP]=true;
    }
}

void Npc::ai_special_attack_ram_charge(){
    if(ai_notice_wall()){
        ai_end_special_attack();
        return;
    }

    if(facing==LEFT){
        ai_keystates[AIK_LEFT]=true;
    }
    else if(facing==RIGHT){
        ai_keystates[AIK_RIGHT]=true;
    }
}

void Npc::ai_special_attack_eagle_divebomb(){
    int i=ai_get_nearest_player();
    double PLAYER_X=player.x;
    double PLAYER_Y=player.y;
    double PLAYER_WIDTH=player.w;
    double PLAYER_HEIGHT=player.h;
    if(i>=0){
        PLAYER_X=mp_players[i].x;
        PLAYER_Y=mp_players[i].y;
        PLAYER_WIDTH=mp_players[i].w;
        PLAYER_HEIGHT=mp_players[i].h;
    }

    //If the target is reached, or the npc is giving up, begin returning home.
    if(ai_notice_wall() || collision_check(x,y,w,h,PLAYER_X+PLAYER_WIDTH/2-2,PLAYER_Y+PLAYER_HEIGHT/2-2,4,4)){
        returning_home=true;
    }

    //If the npc is not yet returning home, continue special attack movement.
    if(!returning_home){
        ai_move_to_target(PLAYER_X+PLAYER_WIDTH/2-2,PLAYER_Y+PLAYER_HEIGHT/2-2,4,4);
    }

    //If the npc is returning home.
    if(returning_home){
        //Check to see if home has been reached.
        //If home has been reached, end the home return and the special attack.
        if(collision_check(x,y,w,h,home_x+w/2-2,home_y+h/2-2,4,4)){
            ai_end_special_attack();
        }
        //If home has not been reached, continue returning home.
        else{
            ai_move_to_target(home_x+w/2-2,home_y+h/2-2,4,4);
        }
    }
}

void Npc::ai_special_attack_sphinx_suck_left(){
    facing=LEFT;

    bool anything_sucked=false;

    double suck_x=x-TILE_SIZE*10;
    double suck_y=y-h*2;
    double suck_w=w+(TILE_SIZE*10);
    double suck_h=h*5;

    if(player.x+player.w<x &&
       collision_check(suck_x,suck_y,suck_w,suck_h,player.x,player.y,player.w,player.h)){
        player.sucked_right=true;
        anything_sucked=true;
    }

    for(int i=0;i<mp_players.size();i++){
        if(mp_players[i].x+mp_players[i].w<x &&
           collision_check(suck_x,suck_y,suck_w,suck_h,mp_players[i].x,mp_players[i].y,mp_players[i].w,mp_players[i].h)){
            mp_players[i].sucked_right=true;
            anything_sucked=true;
        }
    }

    for(int i=0;i<vector_npcs.size();i++){
        if(this!=&vector_npcs[i]){
            if(fabs(vector_npcs[i].x-x)<PROCESS_RANGE && fabs(vector_npcs[i].y-y)<PROCESS_RANGE){
                if(vector_npcs[i].x+vector_npcs[i].w<x &&
                   collision_check(suck_x,suck_y,suck_w,suck_h,vector_npcs[i].x,vector_npcs[i].y,vector_npcs[i].w,vector_npcs[i].h)){
                    vector_npcs[i].sucked_right=true;
                    anything_sucked=true;
                }
            }
        }
    }

    for(int i=0;i<vector_shots.size();i++){
        if(fabs(vector_shots[i].x-x)<PROCESS_RANGE && fabs(vector_shots[i].y-y)<PROCESS_RANGE){
            if(vector_shots[i].x+vector_shots[i].w<x &&
               collision_check(suck_x,suck_y,suck_w,suck_h,vector_shots[i].x,vector_shots[i].y,vector_shots[i].w,vector_shots[i].h)){
                vector_shots[i].sucked_right=true;
                anything_sucked=true;
            }
        }
    }

    if(!anything_sucked){
        ai_end_special_attack();
    }
}

void Npc::ai_special_attack_sphinx_suck_right(){
    facing=RIGHT;

    bool anything_sucked=false;

    double suck_x=x;
    double suck_y=y-h*2;
    double suck_w=w+(TILE_SIZE*10);
    double suck_h=h*5;

    if(player.x>x+w &&
       collision_check(suck_x,suck_y,suck_w,suck_h,player.x,player.y,player.w,player.h)){
        player.sucked_left=true;
        anything_sucked=true;
    }

    for(int i=0;i<mp_players.size();i++){
        if(mp_players[i].x>x+w &&
           collision_check(suck_x,suck_y,suck_w,suck_h,mp_players[i].x,mp_players[i].y,mp_players[i].w,mp_players[i].h)){
            mp_players[i].sucked_left=true;
            anything_sucked=true;
        }
    }

    for(int i=0;i<vector_npcs.size();i++){
        if(this!=&vector_npcs[i]){
            if(fabs(vector_npcs[i].x-x)<PROCESS_RANGE && fabs(vector_npcs[i].y-y)<PROCESS_RANGE){
                if(vector_npcs[i].x>x+w &&
                   collision_check(suck_x,suck_y,suck_w,suck_h,vector_npcs[i].x,vector_npcs[i].y,vector_npcs[i].w,vector_npcs[i].h)){
                    vector_npcs[i].sucked_left=true;
                    anything_sucked=true;
                }
            }
        }
    }

    for(int i=0;i<vector_shots.size();i++){
        if(fabs(vector_shots[i].x-x)<PROCESS_RANGE && fabs(vector_shots[i].y-y)<PROCESS_RANGE){
            if(vector_shots[i].x>x+w &&
               collision_check(suck_x,suck_y,suck_w,suck_h,vector_shots[i].x,vector_shots[i].y,vector_shots[i].w,vector_shots[i].h)){
                vector_shots[i].sucked_left=true;
                anything_sucked=true;
            }
        }
    }

    if(!anything_sucked){
        ai_end_special_attack();
    }
}

void Npc::ai_special_attack_henchmouse_sound_alarm(){
    if(level.alarm){
        ai_end_special_attack();
        return;
    }

    ai_flee_player(ai_get_nearest_player());
}
