/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "npc.h"
#include "world.h"
#include "collision.h"
#include "ai_keys.h"

using namespace std;

void Npc::handle_ai(int index){
    //Reset AI keystates.
    for(short i=0;i<256;i++){
        ai_keystates[i]=false;
    }

    if(exists){
        //Recharge the jump counter.
        if(jump_counter>0){
            jump_counter--;
        }

        //Recharge the use counter.
        if(use_counter>0){
            use_counter--;
        }

        ai_boss_active_check(index);

        //If the npc is not currently using a special attack.
        if(!special_attack_in_progress){

            //As long as a special attack is not in progress, we continue recharging the special attack.
            if(special_attack_counter>0){
                special_attack_counter--;
            }

            int player_index=ai_get_nearest_player();

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

            //If the npc is set to wander randomly about,
            //and does not need to follow or flee the player.
            if(wander && !ai_player_followable(player_index) && (!flee_player || !ai_player_fleeable(player_index))){
                //Increment the wander counter.
                wander_counter++;

                //If the wander counter meets the wander frequency, or if wander_notice_edges is true and an edge has been encountered,
                //or if wander_notice_walls is true and a wall has been encountered,
                //change the wander direction.
                if(wander_counter>=wander_frequency || (wander_notice_edges && ai_notice_edge()) || (wander_notice_walls && ai_notice_wall())){
                    wander_counter=0;
                    wander_frequency=random_range(wander_frequency_min,wander_frequency_max);
                    short random=random_range(0,99);

                    if(!wander_up_and_down_only){
                        //Wander left.
                        if(random<(short)(wander_weight/2)){
                            wander_direction=AIK_LEFT;
                        }
                        //Wander right.
                        else if(random>=(short)(wander_weight/2) && random<wander_weight){
                            wander_direction=AIK_RIGHT;
                        }
                        else{
                            //If the npc cannot pause while wandering.
                            if(!wander_pause){
                                wander_direction=AIK_RIGHT;
                            }
                            //If the npc can pause while wandering.
                            else if(wander_pause){
                                wander_direction=-1;
                            }
                        }
                    }

                    //If the npc can wander up or down.
                    if(wander_up_and_down || wander_up_and_down_only){
                        random=random_range(0,99);

                        //Wander up.
                        if(random<(short)(wander_weight/2)){
                            wander_direction_up_down=AIK_UP;
                        }
                        //Wander down.
                        else if(random>=(short)(wander_weight/2) && random<wander_weight){
                            wander_direction_up_down=AIK_DOWN;
                        }
                        //Wander neither up or down.
                        else{
                            //If the npc can pause while wandering.
                            if(wander_pause){
                                wander_direction_up_down=-1;
                            }
                            //If the npc cannot pause while wandering.
                            else{
                                wander_direction_up_down=AIK_UP;
                            }
                        }
                    }
                }

                //As long as the wander direction is not -1 (which means not to move at all),
                //press the movement key corresponding to the current wander direction.
                if(wander_direction!=-1){
                    ai_keystates[wander_direction]=true;
                }
                if(wander_direction_up_down!=-1){
                    ai_keystates[wander_direction_up_down]=true;
                }
            }

            //If the npc is set to follow the player.
            else if(ai_player_followable(player_index)){
                ai_follow_player(player_index);
            }

            //If the npc is set to flee the player.
            else if(flee_player){
                if(ai_player_fleeable(player_index)){
                    ai_flee_player(player_index);
                }
            }

            if(wander_direction!=-1){
                ai_make_attempts();
            }

            //If the special attack is recharged.
            if(special_attack_range_x!=-1 && special_attack_counter==0 &&
               (!special_attack_only_when_alarm_off || (special_attack_only_when_alarm_off && !level.alarm)) &&
               (!special_attack_only_when_player_facing || (special_attack_only_when_player_facing && !ai_player_back_turned(player_index))) &&
               (!special_attack_only_when_facing_player || (special_attack_only_when_facing_player && ai_facing_player(player_index)))){
                //If the npc can use its special attack regardless of state.
                if(special_attack_anytime){
                    ai_initiate_special_attack(player_index);
                }

                //If the npc can use its special attack on sight.
                else if(special_attack_on_sight){
                    //If the player is in range.
                    if(special_attack_below_only && collision_check(x-TILE_SIZE*special_attack_range_x,y+h,w+(TILE_SIZE*special_attack_range_x)*2,TILE_SIZE*special_attack_range_y,PLAYER_X,PLAYER_Y,PLAYER_WIDTH,PLAYER_HEIGHT)){
                        bool walls_between=false;

                        double check_x=x;
                        double check_y=y;

                        double temp_angle=ai_get_target_direction(PLAYER_X,PLAYER_Y,PLAYER_WIDTH,PLAYER_HEIGHT);

                        int tries=0;

                        while(!walls_between && !ai_int_past_goal(check_x,check_y,PLAYER_X,PLAYER_Y)){
                            int int_x=(int)((int)check_x/TILE_SIZE);
                            int int_y=(int)((int)check_y/TILE_SIZE);

                            if(int_x>=0 && int_x<=(level.level_x/TILE_SIZE)-1 && int_y>=0 && int_y<=(level.level_y/TILE_SIZE)-1){
                                if(level.tile_array[int_x][int_y].solidity==TILE_SOLIDITY_SOLID || level.tile_array[int_x][int_y].special==TILE_SPECIAL_WATER){
                                    walls_between=true;
                                }
                            }

                            check_x+=(double)TILE_SIZE*(cos(temp_angle*(ENGINE_MATH_PI/180)));
                            check_y+=(double)TILE_SIZE*-(sin(temp_angle*(ENGINE_MATH_PI/180)));

                            if(++tries>250){
                                break;
                            }
                        }

                        if(!walls_between){
                            ai_initiate_special_attack(player_index);
                        }
                    }
                    else if(special_attack_above_only && collision_check(x-TILE_SIZE*special_attack_range_x,y-TILE_SIZE*special_attack_range_y,w+(TILE_SIZE*special_attack_range_x)*2,TILE_SIZE*special_attack_range_y,PLAYER_X,PLAYER_Y,PLAYER_WIDTH,PLAYER_HEIGHT)){
                        ai_initiate_special_attack(player_index);
                    }
                    else if(!special_attack_below_only && !special_attack_above_only && collision_check(x-TILE_SIZE*special_attack_range_x,y-TILE_SIZE*special_attack_range_y,w+(TILE_SIZE*special_attack_range_x)*2,h+(TILE_SIZE*special_attack_range_y)*2,PLAYER_X,PLAYER_Y,PLAYER_WIDTH,PLAYER_HEIGHT)){
                        ai_initiate_special_attack(player_index);
                    }
                }
            }
        }
        //If the npc is currently using a special attack.
        else{
            ai_handle_special_attack();
        }
    }
}

void Npc::ai_boss_active_check(int index){
    //If this npc is a boss.
    if(BOSS){
        //If the boss is not active.
        if(player.boss_index==-1){
            for(int i=-1;i<(int)mp_players.size();i++){
                double PLAYER_X=player.x;
                double PLAYER_Y=player.y;
                bool PLAYER_DYING=player.DYING;
                bool PLAYER_BUBBLE_MODE=player.bubble_mode;
                if(i>=0){
                    PLAYER_X=mp_players[i].x;
                    PLAYER_Y=mp_players[i].y;
                    PLAYER_DYING=mp_players[i].DYING;
                    PLAYER_BUBBLE_MODE=mp_players[i].bubble_mode;
                }

                //If the player is in range.
                if(!PLAYER_DYING && !PLAYER_BUBBLE_MODE &&
                   (fabs(PLAYER_X-x)<BOSS_ACTIVATE_RANGE && fabs(PLAYER_Y-y)<BOSS_ACTIVATE_RANGE)){
                    never_move=false;

                    invulnerable=false;

                    special_attack_range_x=65;
                    special_attack_range_y=65;

                    //Activate any associated traps.
                    for(int n=0;n<vector_traps.size();n++){
                        if(vector_traps[n].BOSS){
                            vector_traps[n].moving=true;
                        }
                    }

                    player.boss_begin(index);

                    break;
                }
            }
        }
        //If the boss is active.
        else{
            int players_far_from_boss=0;
            int total_players=0;

            for(int i=-1;i<(int)mp_players.size();i++){
                double PLAYER_X=player.x;
                double PLAYER_Y=player.y;
                bool PLAYER_DYING=player.DYING;
                bool PLAYER_BUBBLE_MODE=player.bubble_mode;
                if(i>=0){
                    PLAYER_X=mp_players[i].x;
                    PLAYER_Y=mp_players[i].y;
                    PLAYER_DYING=mp_players[i].DYING;
                    PLAYER_BUBBLE_MODE=mp_players[i].bubble_mode;
                }

                if(!PLAYER_DYING && !PLAYER_BUBBLE_MODE){
                    total_players++;
                }

                //If the player leaves the stay-active range.
                if(!PLAYER_DYING && !PLAYER_BUBBLE_MODE &&
                   !(fabs(PLAYER_X-x)<BOSS_DEACTIVATE_RANGE && fabs(PLAYER_Y-y)<BOSS_DEACTIVATE_RANGE)){
                    players_far_from_boss++;
                }
            }

            //If all players are far from the boss.
            if(players_far_from_boss>=total_players){
                never_move=true;

                invulnerable=true;

                ai_end_special_attack();

                special_attack_range_x=-1;
                special_attack_range_y=-1;

                //Deactivate any associated traps.
                for(int n=0;n<vector_traps.size();n++){
                    if(vector_traps[n].BOSS){
                        vector_traps[n].moving=false;
                    }
                }

                player.boss_end();
            }
        }
    }
}

void Npc::ai_attempt_use(){
    if(use_counter==0 && use &&
       (!use_only_while_alarm_off || (use_only_while_alarm_off && !level.alarm))){
        //Reset the use recharge counter.
        use_counter=use_frequency;

        ai_keystates[AIK_USE]=true;
    }
}

void Npc::ai_attempt_jump(){
    if(jump_counter==0){
        //If the npc is set to jump and only jump while wandering.
        if(jump && jump_only){
            //Reset the jump recharge counter.
            jump_counter=jump_frequency;

            ai_keystates[AIK_JUMP]=true;
        }
        //If the npc is set to jump.
        else if(jump && !jump_only){
            short random_num=random_range(0,99);
            if(random_num<50){
                //Reset the jump recharge counter.
                jump_counter=jump_frequency;

                ai_keystates[AIK_JUMP]=true;
            }
        }
    }
}

void Npc::ai_make_attempts(){
    if(random_range(0,99)<jump_percent){
        ai_attempt_jump();
    }

    ai_attempt_use();
}

void Npc::ai_initiate_special_attack(int player_index){
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

    //Reset AI keystates.
    for(short i=0;i<256;i++){
        ai_keystates[i]=false;
    }

    run_speed=0.0;
    moving_platform_x=0.0;

    //A special attack is now in progress.
    special_attack_in_progress=true;

    //Reset the special attack recharge counter.
    special_attack_counter=special_attack_frequency;

    //Reset the special attack pause counter.
    special_attack_pause_counter=special_attack_pause_length;

    //Reset the special attack give up counter.
    special_attack_give_up_counter=special_attack_give_up_time;

    //Set the home position to the current location.
    if(!home_permanent){
        home_x=x;
        home_y=y;
    }

    if(special_attack_random_target){
        //Set the target position to a random location.
        int random_direction=random_range(0,1);
        int random_x=random_range(12,32);
        int random_y=random_range(12,32);

        if(random_direction){
            special_attack_target_x=x+random_x;
        }
        else{
            special_attack_target_x=x-random_x;
        }

        random_direction=random_range(0,1);

        if(random_direction){
            special_attack_target_y=y+random_y;
        }
        else{
            special_attack_target_y=y-random_y;
        }
    }
    else{
        //Set the target position to the player's current location.
        special_attack_target_x=PLAYER_X;
        special_attack_target_y=PLAYER_Y;
    }

    double angle=ai_get_target_direction(special_attack_target_x,special_attack_target_y,PLAYER_WIDTH,PLAYER_HEIGHT);
    if(angle>=270 || angle<=90){
        facing=RIGHT;
    }
    else{
        facing=LEFT;
    }

    double spec_divide=1.0;
    /**if(!friendly && !counts_as_trap && player.get_upgrade_state("tiny_baddies")){
        spec_divide=2.0;
    }*/

    if(facing==LEFT){
        x-=NPC_SPRITE_COUNTS[type].special_width/spec_divide-w;
    }
    w=NPC_SPRITE_COUNTS[type].special_width;
    h=NPC_SPRITE_COUNTS[type].special_height;

    /**if(!friendly && !counts_as_trap && player.get_upgrade_state("tiny_baddies")){
        w/=2.0;
        h/=2.0;
    }*/

    //Reset the special attack frame.
    frame_special_attack=0;
    frame_counter_special_attack=0;

    if(special_attack_toggles_light){
        light_source.on=!light_source.on;
    }

    if(special_attack_type==SPECIAL_ATTACK_BEE_DIVEBOMB){
        play_positional_sound(sound_system.npc_bee_dive,x,y);
    }
    else if(special_attack_type==SPECIAL_ATTACK_CAT_POUNCE){
        play_positional_sound(sound_system.npc_cat_pounce,x,y);
    }
    else if(special_attack_type==SPECIAL_ATTACK_RAM_CHARGE){
        play_positional_sound(sound_system.npc_ram_charge,x,y);
    }
    else if(special_attack_type==SPECIAL_ATTACK_EAGLE_DIVEBOMB){
        play_positional_sound(sound_system.npc_eagle_dive,x,y);
    }
    else if(special_attack_type==SPECIAL_ATTACK_SCARECROW_WHACK){
        play_positional_sound(sound_system.npc_scarecrow_whack,x,y);
    }
    else if(special_attack_type==SPECIAL_ATTACK_SWORD_KNIGHT_SWORD_SLASH){
        play_positional_sound(sound_system.npc_sword_slash,x,y);
    }
}

void Npc::ai_handle_special_attack(){
    //If the special attack pause is still in effect.
    if(special_attack_pause_counter>0){
        //Decrement the special attack pause counter.
        special_attack_pause_counter--;
    }

    //If the special attack pause is over.
    //At this point, the npc actually uses his special attack.
    //Animation/sound is handled in animation().
    //Special AI conditions are handled right here, as are end conditions for the special attack.
    if(special_attack_pause_counter==0){

        //Count down to giving up on this special attack.
        if(special_attack_give_up_counter>0){
            special_attack_give_up_counter--;

            if(special_attack_give_up_counter==0){
                ai_end_special_attack();
            }
        }

        if(special_attack_type==SPECIAL_ATTACK_BEE_DIVEBOMB){
            ai_special_attack_bee_divebomb();
        }
        else if(special_attack_type==SPECIAL_ATTACK_REPLICATE){
            ai_special_attack_replicate();
        }
        else if(special_attack_type==SPECIAL_ATTACK_GHOST_HIDE){
            ai_special_attack_ghost_hide();
        }
        else if(special_attack_type==SPECIAL_ATTACK_CAT_POUNCE){
            ai_special_attack_cat_pounce();
        }
        else if(special_attack_type==SPECIAL_ATTACK_RAM_CHARGE){
            ai_special_attack_ram_charge();
        }
        else if(special_attack_type==SPECIAL_ATTACK_EAGLE_DIVEBOMB){
            ai_special_attack_eagle_divebomb();
        }
        else if(special_attack_type==SPECIAL_ATTACK_SPHINX_SUCK_LEFT){
            ai_special_attack_sphinx_suck_left();
        }
        else if(special_attack_type==SPECIAL_ATTACK_SPHINX_SUCK_RIGHT){
            ai_special_attack_sphinx_suck_right();
        }
        else if(special_attack_type==SPECIAL_ATTACK_HENCHMOUSE_SOUND_ALARM){
            ai_special_attack_henchmouse_sound_alarm();
        }
    }
}

void Npc::ai_end_special_attack(){
    special_attack_in_progress=false;
    returning_home=false;
    w=NPC_SPRITE_COUNTS[type].width;
    h=NPC_SPRITE_COUNTS[type].height;

    double spec_divide=1.0;
    /**if(!friendly && !counts_as_trap && player.get_upgrade_state("tiny_baddies")){
        w/=2.0;
        h/=2.0;
        spec_divide=2.0;
    }*/

    if(facing==LEFT){
        x+=NPC_SPRITE_COUNTS[type].special_width/spec_divide-w;
    }

    if(special_attack_end_toggles_light){
        light_source.on=!light_source.on;
    }
}

bool Npc::ai_player_in_range(short range_x,short range_y,int player_index){
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

    //If the player is in range.
    if(collision_check(x-TILE_SIZE*range_x,y-TILE_SIZE*range_y,w+(TILE_SIZE*range_x)*2,h+(TILE_SIZE*range_y)*2,PLAYER_X,PLAYER_Y,PLAYER_WIDTH,PLAYER_HEIGHT)){
        return true;
    }

    return false;
}

bool Npc::ai_player_back_turned(int player_index){
    double PLAYER_X=player.x;
    double PLAYER_WIDTH=player.w;
    short PLAYER_FACING=player.facing;
    bool PLAYER_BUBBLE_MODE=player.bubble_mode;
    if(player_index>=0){
        PLAYER_X=mp_players[player_index].x;
        PLAYER_WIDTH=mp_players[player_index].w;
        PLAYER_FACING=mp_players[player_index].facing;
        PLAYER_BUBBLE_MODE=mp_players[player_index].bubble_mode;
    }

    if(!PLAYER_BUBBLE_MODE){
        //If the player is to the left of the npc and is facing left.
        if(((!player_back_turned_when_above && PLAYER_X<x+w) || (player_back_turned_when_above && PLAYER_X+PLAYER_WIDTH<x)) && (PLAYER_FACING==LEFT || PLAYER_FACING==LEFT_UP || PLAYER_FACING==LEFT_DOWN)){
            return true;
        }
        //If the player is to the right of the npc and is facing right.
        else if(((!player_back_turned_when_above && PLAYER_X+PLAYER_WIDTH>x) || (player_back_turned_when_above && PLAYER_X>x+w)) && (PLAYER_FACING==RIGHT || PLAYER_FACING==RIGHT_UP || PLAYER_FACING==RIGHT_DOWN)){
            return true;
        }
    }
    else{
        return true;
    }

    return false;
}

bool Npc::ai_facing_player(int player_index){
    double PLAYER_X=player.x;
    double PLAYER_WIDTH=player.w;
    if(player_index>=0){
        PLAYER_X=mp_players[player_index].x;
        PLAYER_WIDTH=mp_players[player_index].w;
    }

    double player_point=PLAYER_X+PLAYER_WIDTH/2.0;
    double npc_point=x+w/2.0;

    //If the npc is to the left of the player and is facing left.
    if(npc_point<=player_point && (facing==LEFT || facing==LEFT_UP || facing==LEFT_DOWN)){
        return false;
    }
    //If the npc is to the right of the player and is facing right.
    else if(npc_point>=player_point && (facing==RIGHT || facing==RIGHT_UP || facing==RIGHT_DOWN)){
        return false;
    }

    return true;
}

bool Npc::ai_player_followable(int player_index){
    bool PLAYER_DYING=player.DYING;
    bool PLAYER_BUBBLE_MODE=player.bubble_mode;
    bool PLAYER_SWIMMING=player.SWIMMING;
    if(player_index>=0){
        PLAYER_DYING=mp_players[player_index].DYING;
        PLAYER_BUBBLE_MODE=mp_players[player_index].bubble_mode;
        PLAYER_SWIMMING=mp_players[player_index].SWIMMING;
    }

    if(!PLAYER_DYING && !PLAYER_BUBBLE_MODE && ai_player_in_range(follow_player_range_x,follow_player_range_y,player_index)){
        if(!follow_player_only_when_angry || (follow_player_only_when_angry && angry)){
            if(follow_player && follow_player_always_in_air && IN_AIR){
                return true;
            }

            if((follow_player || (follow_player_during_alarm && level.alarm)) && ((follow_player_during_alarm && level.alarm) ||
               ((!follow_player_only_in_water || (follow_player_only_in_water && PLAYER_SWIMMING)) &&
               (!follow_player_only_in_air || (follow_player_only_in_air && IN_AIR)) &&
               (!follow_player_only_when_back_turned || (follow_player_only_when_back_turned && ai_player_back_turned(player_index)))))){
                return true;
            }
        }
    }

    return false;
}

bool Npc::ai_player_fleeable(int player_index){
    if(ai_player_in_range(flee_player_range,flee_player_range,player_index)){
        return true;
    }

    return false;
}

void Npc::ai_follow_player(int player_index){
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

    //Initiate a special attack, if possible.
    if(!special_attack_in_progress && (special_attack_when_chasing || special_attack_anytime)){
        if(special_attack_counter==0){
            short random=random_range(0,99);

            if(random<50){
                ai_move_to_target(PLAYER_X+PLAYER_WIDTH/2.0-2,PLAYER_Y+PLAYER_HEIGHT/2.0+6,2,2);

                ai_make_attempts();
            }
            else{
                ai_initiate_special_attack(player_index);
            }
        }
        else{
            ai_move_to_target(PLAYER_X+PLAYER_WIDTH/2.0-2,PLAYER_Y+PLAYER_HEIGHT/2.0+6,2,2);

            ai_make_attempts();
        }
    }
    //Move towards the player.
    else{
        ai_move_to_target(PLAYER_X+PLAYER_WIDTH/2.0-2,PLAYER_Y+PLAYER_HEIGHT/2.0+6,2,2);

        ai_make_attempts();
    }
}

void Npc::ai_flee_player(int player_index){
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

    //Initiate a special attack, if possible.
    if(!special_attack_in_progress && (special_attack_when_fleeing || special_attack_anytime)){
        if(special_attack_counter==0){
            short random=random_range(0,99);

            if(random<50){
                ai_move_from_target(PLAYER_X,PLAYER_Y,PLAYER_WIDTH,PLAYER_HEIGHT);

                ai_make_attempts();
            }
            else{
                ai_initiate_special_attack(player_index);
            }
        }
        else{
            ai_move_from_target(PLAYER_X,PLAYER_Y,PLAYER_WIDTH,PLAYER_HEIGHT);

            ai_make_attempts();
        }
    }
    //Move away from the player.
    else{
        ai_move_from_target(PLAYER_X,PLAYER_Y,PLAYER_WIDTH,PLAYER_HEIGHT);

        ai_make_attempts();
    }
}

void Npc::ai_move_from_target(double target_x,double target_y,double target_w,double target_h){
    if(x+w/2.0>target_x+target_w/2.0){
        ai_keystates[AIK_RIGHT]=true;
    }
    else if(x+w/2.0<target_x+target_w/2.0){
        ai_keystates[AIK_LEFT]=true;
    }

    if(y+h/2.0>target_y+target_h/2.0){
        ai_keystates[AIK_DOWN]=true;
    }
    else if(y+h/2.0<target_y+target_h/2.0){
        ai_keystates[AIK_UP]=true;
    }
}

void Npc::ai_move_to_target(double target_x,double target_y,double target_w,double target_h){
    double this_width_box_x=(x+w/2.0)-2;
    double this_width_box_y=(x+w/2.0)-2;
    double this_width_box_w=2;
    double this_width_box_h=2;

    double this_height_box_x=(y+h/2.0)-2;
    double this_height_box_y=(y+h/2.0)-2;
    double this_height_box_w=2;
    double this_height_box_h=2;

    double target_width_box_x=(target_x+target_w/2.0)-2;
    double target_width_box_y=(target_x+target_w/2.0)-2;
    double target_width_box_w=2;
    double target_width_box_h=2;

    double target_height_box_x=(target_y+target_h/2.0)-2;
    double target_height_box_y=(target_y+target_h/2.0)-2;
    double target_height_box_w=2;
    double target_height_box_h=2;

    if(!collision_check(this_width_box_x,this_width_box_y,this_width_box_w,this_width_box_h,target_width_box_x,target_width_box_y,target_width_box_w,target_width_box_h)){
        if(x+w/2.0>target_x+target_w/2.0){
            ai_keystates[AIK_LEFT]=true;
        }
        else if(x+w/2.0<target_x+target_w/2.0){
            ai_keystates[AIK_RIGHT]=true;
        }
    }
    if(!collision_check(this_height_box_x,this_height_box_y,this_height_box_w,this_height_box_h,target_height_box_x,target_height_box_y,target_height_box_w,target_height_box_h)){
        if(y+h/2.0>target_y+target_h/2.0){
            ai_keystates[AIK_UP]=true;
        }
        else if(y+h/2.0<target_y+target_h/2.0){
            ai_keystates[AIK_DOWN]=true;
        }
    }
}

double Npc::ai_get_target_direction(double target_x,double target_y,double target_w,double target_h){
    double target_angle=0.0;
    int rotation_target_x=0;
    int rotation_target_y=0;
    double x2=0.0;
    double y2=0.0;
    double x1=0.0;
    double y1=0.0;
    double x_component=0.0;
    double y_component=0.0;

    rotation_target_x=target_x+target_w/2-player.camera_x;
    rotation_target_y=target_y+target_h/2-player.camera_y;

    x2=(x+w/2-player.camera_x)+(rotation_target_x-(x+w/2-player.camera_x));
    y2=rotation_target_y+((y+h/2-player.camera_y)-rotation_target_y);

    x1=x+w/2-player.camera_x;
    y1=y+h/2-player.camera_y;

    x_component=sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
    y_component=sqrt(((rotation_target_x-x2)*(rotation_target_x-x2))+((rotation_target_y-y2)*(rotation_target_y-y2)));

    if(rotation_target_x<(x+w/2-player.camera_x)){
        x_component*=-1;
    }

    target_angle=atan2(y_component,x_component)*(180/ENGINE_MATH_PI);

    if(rotation_target_y>(y+h/2-player.camera_y)){
        target_angle=180+(180-target_angle);
    }

    return target_angle;
}

bool Npc::ai_int_past_goal(double check_x,double check_y,double goal_x,double goal_y){
    bool x_past=false;
    bool y_past=false;

    if(x>=goal_x && check_x<=goal_x){
        x_past=true;
    }
    else if(x<goal_x && check_x>=goal_x){
        x_past=true;
    }

    if(y>=goal_y && check_y<=goal_y){
        y_past=true;
    }
    else if(y<goal_y && check_y>=goal_y){
        y_past=true;
    }

    if(x_past && y_past){
        return true;
    }
    else{
        return false;
    }
}

int Npc::ai_get_nearest_player(){
    int nearest_player=-1;
    double nearest_distance=-1.0;

    for(int i=-1;i<(int)mp_players.size();i++){
        double PLAYER_X=player.x;
        double PLAYER_Y=player.y;
        bool PLAYER_DYING=player.DYING;
        bool PLAYER_BUBBLE_MODE=player.bubble_mode;
        if(i>=0){
            PLAYER_X=mp_players[i].x;
            PLAYER_Y=mp_players[i].y;
            PLAYER_DYING=mp_players[i].DYING;
            PLAYER_BUBBLE_MODE=mp_players[i].bubble_mode;
        }

        if(!PLAYER_DYING && !PLAYER_BUBBLE_MODE){
            double x1=x;
            double y1=y;
            double x2=PLAYER_X;
            double y2=PLAYER_Y;
            double this_distance=sqrt(((x2-x1)*(x2-x1))+((y2-y1)*(y2-y1)));

            if(nearest_distance==-1.0 || this_distance<nearest_distance){
                nearest_player=i;
                nearest_distance=this_distance;
            }
        }
    }

    return nearest_player;
}

bool Npc::ai_slope_nearby(){
    //The npc's current tile position.
    int npc_current_x=(int)((int)x/TILE_SIZE);
    int npc_current_y=(int)((int)y/TILE_SIZE);

    //Check all tiles in a square around the npc.
    int check_x_start=npc_current_x-3;
    int check_x_end=npc_current_x+3;
    int check_y_start=npc_current_y-3;
    int check_y_end=npc_current_y+3;

    for(int int_y=check_y_start;int_y<check_y_end;int_y++){
        for(int int_x=check_x_start;int_x<check_x_end;int_x++){
            //As long as the current tile is within the level's boundaries.
            if(int_x>=0 && int_x<=(level.level_x/TILE_SIZE)-1 && int_y>=0 && int_y<=(level.level_y/TILE_SIZE)-1){
                if(level.tile_array[int_x][int_y].slope!=180){
                    return true;
                }
            }
        }
    }

    return false;
}

bool Npc::ai_notice_edge(){
    //As long as the npc is not in the air and there is no slope nearby.
    if(!IN_AIR && !ai_slope_nearby()){
        //The npc's current tile position.
        int npc_current_x=(int)((int)x/TILE_SIZE);
        int npc_current_y=(int)((int)y/TILE_SIZE);

        //Check all tiles in a square around the npc.
        int check_x_start=npc_current_x-4;
        int check_x_end=npc_current_x+4;
        int check_y_start=npc_current_y-4;
        int check_y_end=npc_current_y+4;

        for(int int_y=check_y_start;int_y<check_y_end;int_y++){
            for(int int_x=check_x_start;int_x<check_x_end;int_x++){
                //As long as the current tile is within the level's boundaries.
                if(int_x>=0 && int_x<=(level.level_x/TILE_SIZE)-1 && int_y>=0 && int_y<=(level.level_y/TILE_SIZE)-1){

                    //If the tile is not a solid or cloud tile.
                    if(level.tile_array[int_x][int_y].solidity!=TILE_SOLIDITY_SOLID && level.tile_array[int_x][int_y].solidity!=TILE_SOLIDITY_CLOUD){
                        //If the tile is just to the npc's left and down and the npc is wandering left.
                        if(collision_check(x-2,y+h,w,TILE_SIZE,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE)){
                            if(level.tile_array[int_x][int_y].x<x && level.tile_array[int_x][int_y].y>y && (move_state==LEFT || move_state==LEFT_UP || move_state==LEFT_DOWN)){
                                return true;
                            }
                        }

                        //If the tile is just to the npc's right and down and the npc is wandering right.
                        if(collision_check(x+2,y+h,w,TILE_SIZE,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE)){
                            if(level.tile_array[int_x][int_y].x>x && level.tile_array[int_x][int_y].y>y && (move_state==RIGHT || move_state==RIGHT_UP || move_state==RIGHT_DOWN)){
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }

    return false;
}

bool Npc::ai_notice_wall(){
    //Then, treat the edges of the level as walls.
    if(x-2<0 && (move_state==LEFT || move_state==LEFT_UP || move_state==LEFT_DOWN)){
        return true;
    }
    else if(x+w+2>level.level_x && (move_state==RIGHT || move_state==RIGHT_UP || move_state==RIGHT_DOWN)){
        return true;
    }

    if(FLYING || SWIMMING){
        if(y-2<0 && (move_state==UP || move_state==LEFT_UP || move_state==RIGHT_UP)){
            return true;
        }
        else if(y+h+2>level.level_y && (move_state==DOWN || move_state==LEFT_DOWN || move_state==RIGHT_DOWN)){
            return true;
        }
    }

    if(SWIMMING && SWIM_CAN_JUMP && !act_as_platform){
        return true;
    }

    if(FLYING){
        //The npc's current tile position.
        int npc_current_x=(int)((int)x/TILE_SIZE);
        int npc_current_y=(int)((int)y/TILE_SIZE);

        //Check all tiles in a square around the npc.
        int check_x_start=npc_current_x-4;
        int check_x_end=npc_current_x+4;
        int check_y_start=npc_current_y-4;
        int check_y_end=npc_current_y+4;

        for(int int_y=check_y_start;int_y<check_y_end;int_y++){
            for(int int_x=check_x_start;int_x<check_x_end;int_x++){
                //As long as the current tile is within the level's boundaries.
                if(int_x>=0 && int_x<=(level.level_x/TILE_SIZE)-1 && int_y>=0 && int_y<=(level.level_y/TILE_SIZE)-1){
                    //If the tile is solid.
                    if(level.tile_array[int_x][int_y].solidity==TILE_SOLIDITY_SOLID || level.tile_array[int_x][int_y].special==TILE_SPECIAL_WATER ||
                       (level.tile_array[int_x][int_y].special==TILE_SPECIAL_CLIMBABLE_TOP && (move_state==DOWN || move_state==LEFT_DOWN || move_state==RIGHT_DOWN)) ||
                       (!notice_wall_ignore_cloud && level.tile_array[int_x][int_y].solidity==TILE_SOLIDITY_CLOUD && (move_state==DOWN || move_state==LEFT_DOWN || move_state==RIGHT_DOWN))){
                        //If the tile is just to the npc's left and the npc is wandering left.
                        if(collision_check(x-2,y,w,h,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE)){
                            if(level.tile_array[int_x][int_y].x<x && (move_state==LEFT || move_state==LEFT_UP || move_state==LEFT_DOWN)){
                                return true;
                            }
                        }
                        //If the tile is just to the npc's right and the npc is wandering right.
                        if(collision_check(x+2,y,w,h,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE)){
                            if(level.tile_array[int_x][int_y].x>x && (move_state==RIGHT || move_state==RIGHT_UP || move_state==RIGHT_DOWN)){
                                return true;
                            }
                        }
                        //If the tile is just below the npc and the npc is wandering down.
                        if(collision_check(x,y+2,w,h,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE)){
                            if(level.tile_array[int_x][int_y].y>y && (move_state==DOWN || move_state==LEFT_DOWN || move_state==RIGHT_DOWN)){
                                return true;
                            }
                        }
                        //If the tile is just above the npc and the npc is wandering up.
                        if(collision_check(x,y-2,w,h,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE)){
                            if(level.tile_array[int_x][int_y].y<y && (move_state==UP || move_state==LEFT_UP || move_state==RIGHT_UP)){
                                return true;
                            }
                        }
                    }
                }
            }
        }

        for(int i=0;i<vector_moving_platforms.size();i++){
            if(fabs(vector_moving_platforms[i].x-x)<PROCESS_RANGE && fabs(vector_moving_platforms[i].y-y)<PROCESS_RANGE){
                //If the object is just below the npc and the npc is wandering down.
                if(collision_check(x,y+2,w,h,vector_moving_platforms[i].x,vector_moving_platforms[i].y,vector_moving_platforms[i].w,vector_moving_platforms[i].h)){
                    if(vector_moving_platforms[i].y>y && (move_state==DOWN || move_state==LEFT_DOWN || move_state==RIGHT_DOWN)){
                        return true;
                    }
                }
                //If the object is just above the npc and the npc is wandering up.
                if(collision_check(x,y-2,w,h,vector_moving_platforms[i].x,vector_moving_platforms[i].y,vector_moving_platforms[i].w,vector_moving_platforms[i].h)){
                    if(vector_moving_platforms[i].y<y && (move_state==UP || move_state==LEFT_UP || move_state==RIGHT_UP)){
                        return true;
                    }
                }
            }
        }

        for(int i=0;i<vector_npcs.size();i++){
            if(this!=&vector_npcs[i] && vector_npcs[i].exists){
                if(fabs(vector_npcs[i].x-x)<PROCESS_RANGE && fabs(vector_npcs[i].y-y)<PROCESS_RANGE){
                    //If the object is just to the npc's left and the npc is wandering left.
                    if(collision_check(x-2,y,w,h,vector_npcs[i].x,vector_npcs[i].y,vector_npcs[i].w,vector_npcs[i].h)){
                        if(vector_npcs[i].x<x && (move_state==LEFT || move_state==LEFT_UP || move_state==LEFT_DOWN)){
                            return true;
                        }
                    }
                    //If the object is just to the npc's right and the npc is wandering right.
                    if(collision_check(x+2,y,w,h,vector_npcs[i].x,vector_npcs[i].y,vector_npcs[i].w,vector_npcs[i].h)){
                        if(vector_npcs[i].x>x && (move_state==RIGHT || move_state==RIGHT_UP || move_state==RIGHT_DOWN)){
                            return true;
                        }
                    }
                    //If the object is just below the npc and the npc is wandering down.
                    if(collision_check(x,y+2,w,h,vector_npcs[i].x,vector_npcs[i].y,vector_npcs[i].w,vector_npcs[i].h)){
                        if(vector_npcs[i].y>y && (move_state==DOWN || move_state==LEFT_DOWN || move_state==RIGHT_DOWN)){
                            return true;
                        }
                    }
                    //If the object is just above the npc and the npc is wandering up.
                    if(collision_check(x,y-2,w,h,vector_npcs[i].x,vector_npcs[i].y,vector_npcs[i].w,vector_npcs[i].h)){
                        if(vector_npcs[i].y<y && (move_state==UP || move_state==LEFT_UP || move_state==RIGHT_UP)){
                            return true;
                        }
                    }
                }
            }
        }
    }
    else if(SWIMMING){
        //The npc's current tile position.
        int npc_current_x=(int)((int)x/TILE_SIZE);
        int npc_current_y=(int)((int)y/TILE_SIZE);

        //Check all tiles in a square around the npc.
        int check_x_start=npc_current_x-4;
        int check_x_end=npc_current_x+4;
        int check_y_start=npc_current_y-4;
        int check_y_end=npc_current_y+4;

        for(int int_y=check_y_start;int_y<check_y_end;int_y++){
            for(int int_x=check_x_start;int_x<check_x_end;int_x++){
                //As long as the current tile is within the level's boundaries.
                if(int_x>=0 && int_x<=(level.level_x/TILE_SIZE)-1 && int_y>=0 && int_y<=(level.level_y/TILE_SIZE)-1){
                    //If the tile is solid.
                    if(level.tile_array[int_x][int_y].solidity==TILE_SOLIDITY_SOLID){
                        //If the tile is just to the npc's left and the npc is wandering left.
                        if(collision_check(x-2,y,w,h,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE)){
                            if(level.tile_array[int_x][int_y].x<x && (move_state==LEFT || move_state==LEFT_UP || move_state==LEFT_DOWN)){
                                return true;
                            }
                        }
                        //If the tile is just to the npc's right and the npc is wandering right.
                        if(collision_check(x+2,y,w,h,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE)){
                            if(level.tile_array[int_x][int_y].x>x && (move_state==RIGHT || move_state==RIGHT_UP || move_state==RIGHT_DOWN)){
                                return true;
                            }
                        }
                        //If the tile is just below the npc and the npc is wandering down.
                        if(collision_check(x,y+2,w,h,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE)){
                            if(level.tile_array[int_x][int_y].y>y && (move_state==DOWN || move_state==LEFT_DOWN || move_state==RIGHT_DOWN)){
                                return true;
                            }
                        }
                        //If the tile is just above the npc and the npc is wandering up.
                        if(collision_check(x,y-2,w,h,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE)){
                            if(level.tile_array[int_x][int_y].y<y && (move_state==UP || move_state==LEFT_UP || move_state==RIGHT_UP)){
                                return true;
                            }
                        }
                    }
                }
            }
        }

        for(int i=0;i<vector_npcs.size();i++){
            if(this!=&vector_npcs[i] && vector_npcs[i].exists){
                if(fabs(vector_npcs[i].x-x)<PROCESS_RANGE && fabs(vector_npcs[i].y-y)<PROCESS_RANGE){
                    //If the object is just to the npc's left and the npc is wandering left.
                    if(collision_check(x-2,y,w,h,vector_npcs[i].x,vector_npcs[i].y,vector_npcs[i].w,vector_npcs[i].h)){
                        if(vector_npcs[i].x<x && (move_state==LEFT || move_state==LEFT_UP || move_state==LEFT_DOWN)){
                            return true;
                        }
                    }
                    //If the object is just to the npc's right and the npc is wandering right.
                    if(collision_check(x+2,y,w,h,vector_npcs[i].x,vector_npcs[i].y,vector_npcs[i].w,vector_npcs[i].h)){
                        if(vector_npcs[i].x>x && (move_state==RIGHT || move_state==RIGHT_UP || move_state==RIGHT_DOWN)){
                            return true;
                        }
                    }
                    //If the object is just below the npc and the npc is wandering down.
                    if(collision_check(x,y+2,w,h,vector_npcs[i].x,vector_npcs[i].y,vector_npcs[i].w,vector_npcs[i].h)){
                        if(vector_npcs[i].y>y && (move_state==DOWN || move_state==LEFT_DOWN || move_state==RIGHT_DOWN)){
                            return true;
                        }
                    }
                    //If the object is just above the npc and the npc is wandering up.
                    if(collision_check(x,y-2,w,h,vector_npcs[i].x,vector_npcs[i].y,vector_npcs[i].w,vector_npcs[i].h)){
                        if(vector_npcs[i].y<y && (move_state==UP || move_state==LEFT_UP || move_state==RIGHT_UP)){
                            return true;
                        }
                    }
                }
            }
        }
    }
    else if(!FLYING && !SWIMMING){
        //The npc's current tile position.
        int npc_current_x=(int)((int)x/TILE_SIZE);
        int npc_current_y=(int)((int)y/TILE_SIZE);

        //Check all tiles in a square around the npc.
        int check_x_start=npc_current_x-4;
        int check_x_end=npc_current_x+4;
        int check_y_start=npc_current_y-4;
        int check_y_end=npc_current_y+4;

        for(int int_y=check_y_start;int_y<check_y_end;int_y++){
            for(int int_x=check_x_start;int_x<check_x_end;int_x++){
                //As long as the current tile is within the level's boundaries.
                if(int_x>=0 && int_x<=(level.level_x/TILE_SIZE)-1 && int_y>=0 && int_y<=(level.level_y/TILE_SIZE)-1){
                    //If the tile is solid.
                    if(level.tile_array[int_x][int_y].solidity==TILE_SOLIDITY_SOLID && level.tile_array[int_x][int_y].slope==180){
                        //If the tile is just to the npc's left and the npc is wandering left.
                        if(collision_check(x-2,y,w,h,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE)){
                            if(level.tile_array[int_x][int_y].x<x && move_state==LEFT){
                                return true;
                            }
                        }
                        //If the tile is just to the npc's right and the npc is wandering right.
                        if(collision_check(x+2,y,w,h,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE)){
                            if(level.tile_array[int_x][int_y].x>x && move_state==RIGHT){
                                return true;
                            }
                        }
                    }
                }
            }
        }

        for(int i=0;i<vector_npcs.size();i++){
            if(this!=&vector_npcs[i] && vector_npcs[i].exists){
                if(fabs(vector_npcs[i].x-x)<PROCESS_RANGE && fabs(vector_npcs[i].y-y)<PROCESS_RANGE){
                    //If the object is just to the npc's left and the npc is wandering left.
                    if(collision_check(x-2,y,w,h,vector_npcs[i].x,vector_npcs[i].y,vector_npcs[i].w,vector_npcs[i].h)){
                        if(vector_npcs[i].x<x && move_state==LEFT){
                            return true;
                        }
                    }
                    //If the object is just to the npc's right and the npc is wandering right.
                    if(collision_check(x+2,y,w,h,vector_npcs[i].x,vector_npcs[i].y,vector_npcs[i].w,vector_npcs[i].h)){
                        if(vector_npcs[i].x>x && move_state==RIGHT){
                            return true;
                        }
                    }
                }
            }
        }

        for(int i=0;i<vector_doors.size();i++){
            if(fabs(vector_doors[i].x-x)<PROCESS_RANGE && fabs(vector_doors[i].y-y)<PROCESS_RANGE){
                //If the object is just to the npc's left and the npc is wandering left.
                if(collision_check(x-2,y,w,h,vector_doors[i].x,vector_doors[i].y,vector_doors[i].w,vector_doors[i].h)){
                    if(vector_doors[i].x<x && move_state==LEFT){
                        return true;
                    }
                }
                //If the object is just to the npc's right and the npc is wandering right.
                if(collision_check(x+2,y,w,h,vector_doors[i].x,vector_doors[i].y,vector_doors[i].w,vector_doors[i].h)){
                    if(vector_doors[i].x>x && move_state==RIGHT){
                        return true;
                    }
                }
            }
        }

        for(int i=0;i<vector_traps.size();i++){
            if(fabs(vector_traps[i].x-x)<PROCESS_RANGE && fabs(vector_traps[i].y-y)<PROCESS_RANGE){
                //If the object is just to the npc's left and the npc is wandering left.
                if(collision_check(x-2,y,w,h,vector_traps[i].x,vector_traps[i].y,vector_traps[i].w,vector_traps[i].h)){
                    if(vector_traps[i].x<x && move_state==LEFT){
                        return true;
                    }
                }
                //If the object is just to the npc's right and the npc is wandering right.
                if(collision_check(x+2,y,w,h,vector_traps[i].x,vector_traps[i].y,vector_traps[i].w,vector_traps[i].h)){
                    if(vector_traps[i].x>x && move_state==RIGHT){
                        return true;
                    }
                }
            }
        }
    }

    return false;
}
