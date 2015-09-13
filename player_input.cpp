/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "player.h"
#include "world.h"
#include "collision.h"
#include "options.h"
#include "quit.h"
#include "button_events.h"

using namespace std;

void Player::prepare_for_input(){
    //Get the SDL keystates and store them in the keystates variable for evaluation.
    keystates=SDL_GetKeyState(NULL);

    crouching_at_frame_start=CROUCHING;

    set_solid_above();
}

void Player::handle_command_event(short command){
    short current_window=window_manager.which_window_open();
    Window* ptr_window=NULL;
    if(current_window==WHICH_WINDOW_INVENTORY){
        ptr_window=&window_inventory[0];
    }
    else if(current_window==WHICH_WINDOW_MAP){
        ptr_window=&window_map[0];
    }
    else if(current_window==WHICH_WINDOW_MESSAGE){
        ptr_window=&window_message[0];
    }
    else if(current_window==WHICH_WINDOW_SETUP_SURVIVAL){
        ptr_window=&window_setup_survival[0];
    }
    else if(current_window==WHICH_WINDOW_SHOP){
        ptr_window=&window_shop[0];
    }
    else if(current_window==WHICH_WINDOW_UPGRADES){
        ptr_window=&window_upgrades[0];
    }
    else if(current_window>=WHICH_WINDOW_LEVEL_INFO && current_window<WHICH_WINDOW_OTHER){
        ptr_window=&windows_level_info[current_window-WHICH_WINDOW_LEVEL_INFO];
    }
    else if(current_window>=WHICH_WINDOW_OTHER){
        ptr_window=&vector_windows[current_window-WHICH_WINDOW_OTHER];
    }

    int button_check=0;
    int last_check=-1;

    switch(command){

    case COMMAND_TOGGLE_MAIN_MENU:
        //If the game is in progress.
        if(game_in_progress && !level.return_title_pause()){
            //Close all windows.
            window_manager.close_windows(&vector_windows[WINDOW_MAIN_MENU]);

            window_manager.configure_main_menu();

            vector_windows[WINDOW_MAIN_MENU].toggle_on();

            window_manager.set_main_menu_current_button();
        }
        break;

    case COMMAND_TOGGLE_INVENTORY:
        if(game_mode==GAME_MODE_SP_ADVENTURE || game_mode==GAME_MODE_MP_ADVENTURE){
            //If the game is in progress.
            if(game_in_progress && !level.return_title_pause()){
                //Close all windows.
                window_manager.close_windows(&window_inventory[0]);

                window_inventory[0].toggle_on();

                if(window_inventory[0].return_on()){
                    play_positional_sound(sound_system.inventory_open);
                }
                else{
                    play_positional_sound(sound_system.inventory_close);
                }
            }
        }
        break;

    case COMMAND_TOGGLE_MAP:
        if(game_mode==GAME_MODE_SP_ADVENTURE){
            //If the game is in progress.
            if(game_in_progress && !level.return_title_pause()){
                //Close all windows except this one.
                window_manager.close_windows(&window_map[0]);

                //Center the map camera on the player.
                window_map[0].set_map_level(current_level,current_sub_level);
                window_map[0].set_camera_coordinates(x/2.0+(w/2.0-window_map[0].map_camera_w)/2.0,y/2.0+(h/2.0-window_map[0].map_camera_h)/2.0);

                window_map[0].toggle_on();

                if(window_map[0].return_on()){
                    play_positional_sound(sound_system.map_open);
                }
                else{
                    play_positional_sound(sound_system.map_close);
                }
            }
        }
        break;

    case COMMAND_SCREENSHOT:
        main_window.screenshot();

        play_positional_sound(sound_system.camera_lock);
        break;

    case COMMAND_LOOK:
        //As long as the game is not paused and is in progress.
        if(!pause && game_in_progress && !game_mode_is_multiplayer()){
            //If the current level is not the world map.
            if(!on_worldmap() && !DYING){
                //If the player presses the look key and isn't already holding any direction key, in the air, or swimming, toggle look mode.
                if(!LOOKING && !IN_AIR && !command_state(COMMAND_LEFT) && !command_state(COMMAND_UP) && !command_state(COMMAND_RIGHT) && !command_state(COMMAND_DOWN)){
                    //Toggle look mode.
                    LOOKING=true;

                    play_positional_sound(sound_system.player_look_start);
                }
                //If the player presses the look key and isn't already in the air or swimming, toggle look mode.
                else if(LOOKING){
                    //Toggle look mode.
                    LOOKING=false;

                    play_positional_sound(sound_system.player_look_stop);
                }
            }
        }
        break;

    case COMMAND_JUMP:
        //If a window is open.
        if(ptr_window!=NULL){
            if(window_message[0].return_on()){
                window_message[0].turn_off();
            }
            else{
                if(current_button!=-1){
                    int recall_current_button=current_button;

                    ptr_window->buttons[recall_current_button].mouse_button_down();
                    ptr_window->buttons[recall_current_button].mouse_button_up(ptr_window);
                    ptr_window->buttons[recall_current_button].reset_clicked();
                }
            }
        }
        //As long as the game is not paused, and is in progress.
        else if(!pause && game_in_progress && !bubble_mode){
            //If the current level is the world map.
            if(on_worldmap()){
                //Enter level.
                worldmap_enter_level=true;
            }
            //If the player is in a level.
            else{
                //As long as the player isn't dying.
                if(!DYING){
                    bool fell_through_cloud=false;

                    //If all the conditions for falling through a cloud are met.
                    if(command_state(COMMAND_JUMP) && command_state(COMMAND_DOWN) && !IN_AIR && !LOOKING && !SHOOTING && !CLIMBING){
                        if(on_cloud){
                            fall_through_cloud();
                            fell_through_cloud=true;
                        }
                    }

                    //If all the conditions for a jump are met.
                    if(!fell_through_cloud && !LOOKING && (cheat_jump || counter_jump_mode>0 || (extra_jumps>0 && ((SWIMMING && SWIM_CAN_JUMP) || !SWIMMING)) ||
                                                           ((!IN_AIR || (IN_AIR && counter_jump_mercy>0)) && !jump_state && ((SWIMMING && SWIM_CAN_JUMP) || !SWIMMING)))){
                        if(extra_jumps>0 && IN_AIR && counter_jump_mercy==0){
                            if(!get_upgrade_state("infinite_jumps")){
                                extra_jumps--;
                            }
                        }

                        counter_jump_mercy=0;

                        if(CLIMBING){
                            CLIMBING=false;
                            climb_jump_timer=climb_jump_delay;
                            CLIMBING_JUMP=true;
                        }

                        jump_state=true;

                        if(!CLIMBING_JUMP){
                            air_velocity=jump_max*-1;
                            /**double p_angle=touched_slope_angle;
                            if((p_angle>=0 && p_angle<=90) || (p_angle>=180 && p_angle<=270)){
                                p_angle+=90;
                            }
                            else if((p_angle>90 && p_angle<180) || (p_angle>270 && p_angle<360)){
                                p_angle-=90;
                            }
                            run_speed+=jump_max*(cos(p_angle*(M_PI/180)));
                            air_velocity=jump_max*-(sin(p_angle*(M_PI/180)));*/
                        }
                        else{
                            air_velocity=climbing_jump_max*-1;
                        }

                        //The player has begun a jump, so set them to be in the air.
                        IN_AIR=true;
                        frame_jump=0;

                        //If the player is under the effect of a J Balloon.
                        if(counter_jump_mode>0){
                            play_positional_sound(sound_system.player_jump_j_balloon,x,y);
                        }
                        else{
                            play_positional_sound(*ptr_player_jump,x,y);
                        }

                        stat_total_jumps++;
                    }
                }
            }
        }
        else if(!pause && game_in_progress && bubble_mode){
            if(collision_check(x,y,w,h,camera_x,camera_y,camera_w,camera_h) && in_open_air()){
                bubble_mode=false;
                air_velocity=-jump_min*1.5;
                IN_AIR=true;
                run_speed=0.0;

                //Prevent the constant bubble popping sound if we are moving the camera around with dev controls.
                if(cam_state==CAM_STICKY){
                    play_positional_sound(sound_system.player_bubble_pop);
                }
            }
        }
        break;

    case COMMAND_SHOOT:
        if(game_beginning_cutscene!=0){
            hud_buttons[0].mouse_button_down();
            hud_buttons[0].mouse_button_up(0);
            hud_buttons[0].reset_clicked();
        }
        else if(ptr_window!=NULL){
            window_manager.close_windows(0);
            if(!game_in_progress){
                vector_windows[WINDOW_MAIN_MENU].turn_on();
            }
        }
        //As long as the game is not paused and is in progress.
        else if(!pause && game_in_progress && !bubble_mode){
            //If the current level is the world map.
            if(on_worldmap()){
                //Enter level.
                worldmap_enter_level=true;
            }
            //If the current level is not the world map.
            else{
                //If the shooting key is pressed and the player has ammo.
                if(!SWIMMING && !LOOKING && !SHOOTING && !DYING){
                    crouch_stop();

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

                    //Pretty straightforward. If shoot_state is non-zero, then the player is holding a directional key, so shoot in the corresponding direction and subtract one ammo.
                    //If the player is not holding any directional keys, shoot in the direction that the player is facing and subtract one ammo.
                    //An exception is shooting downwards. Shooting down is only allowed while the player is in the air.

                    if(shoot_state==UP){
                        if(ammo>=shot_cost){
                            vector_shots.push_back(Shot(x-4,y-13,90,current_shot,false));
                            ammo-=shot_cost;
                        }
                        else{
                            play_positional_sound(sound_system.player_no_ammo,x,y);
                        }

                        //The player is now shooting.
                        SHOOTING=true;
                        shoot_render_direction=UP;
                        //Reset the shooting frame.
                        frame_shoot=0;
                        frame_counter_shoot=0;

                        stat_shots_fired++;
                        special_count_shots_this_level++;

                        //Reset other variables.
                        frame=0;
                        frame_counter=0;
                        shoot_state=0;
                        look_state=0;
                    }
                    else if(shoot_state==DOWN){
                        if(IN_AIR || CLIMBING){
                            if(ammo>=shot_cost){
                                vector_shots.push_back(Shot(x-4,y+h-13,270,current_shot,false));
                                ammo-=shot_cost;
                            }
                            else{
                                play_positional_sound(sound_system.player_no_ammo,x,y);
                            }

                            //The player is now shooting.
                            SHOOTING=true;
                            shoot_render_direction=DOWN;
                            //Reset the shooting frame.
                            frame_shoot=0;
                            frame_counter_shoot=0;

                            stat_shots_fired++;
                            special_count_shots_this_level++;

                            //Reset other variables.
                            frame=0;
                            frame_counter=0;
                            shoot_state=0;
                            look_state=0;
                        }
                    }
                    else if(facing==LEFT || facing==RIGHT){
                        //If the player has any ammo remaining.
                        if(ammo>=shot_cost){
                            if(facing==LEFT){
                                vector_shots.push_back(Shot(x-13,y+12,180,current_shot,false));
                                ammo-=shot_cost;
                            }
                            else if(facing==RIGHT){
                                vector_shots.push_back(Shot(x+w-13,y+12,0,current_shot,false));
                                ammo-=shot_cost;
                            }
                        }
                        else{
                            play_positional_sound(sound_system.player_no_ammo,x,y);
                        }

                        //The player is now shooting.
                        SHOOTING=true;

                        if(facing==LEFT){
                            shoot_render_direction=LEFT;
                        }
                        else if(facing==RIGHT){
                            shoot_render_direction=RIGHT;
                        }

                        //Reset the shooting frame.
                        frame_shoot=0;
                        frame_counter_shoot=0;

                        stat_shots_fired++;
                        special_count_shots_this_level++;

                        //Reset other variables.
                        frame=0;
                        frame_counter=0;
                        shoot_state=0;
                        look_state=0;
                    }
                }
            }
        }


        if(!pause && game_in_progress && game_mode_is_multiplayer() && !on_worldmap() && all_humans_dead()){
            for(int i=0;i<mp_players.size();i++){
                if(!mp_players[i].DYING && !mp_players[i].bubble_mode){
                    mp_players[i].handle_death(mp_players[i].x,mp_players[i].y,mp_players[i].w,mp_players[i].h,true);
                }
            }
        }
        break;

    case COMMAND_CHANGE_CHARACTER:
        if(game_in_progress && !game_mode_is_multiplayer()){
            toggle_character();

            play_positional_sound(sound_system.player_change_shot,x,y);
        }
        break;

    case COMMAND_PAUSE:
        if(!level.return_title_pause() && window_manager.which_window_open()==-1){
            toggle_pause(!pause);

            if(pause){
                play_positional_sound(sound_system.pause);
            }
            else{
                play_positional_sound(sound_system.unpause);
            }
        }
        break;

    case COMMAND_CHANGE_SHOT:
        if(game_in_progress){
            if(!on_worldmap()){
                if(shot_homing){
                    if(current_shot==SHOT_PLAYER){
                        current_shot=SHOT_PLAYER_HOMING;
                    }
                    else{
                        current_shot=SHOT_PLAYER;
                    }
                    play_positional_sound(sound_system.player_change_shot,x,y);
                }
                else{
                    play_positional_sound(sound_system.player_no_ammo,x,y);
                }
            }
        }
        break;

    case COMMAND_UP: case COMMAND_LEFT:
        //If a window is open.
        if(ptr_window!=NULL){
            if(current_button==-1){
                current_button=ptr_window->buttons.size();
            }

            button_check=current_button;
            last_check=-1;
            do{
                if(--button_check<1){
                    button_check=ptr_window->buttons.size()-1;
                }

                if(last_check==current_button){
                    button_check=current_button;
                    break;
                }

                last_check=button_check;
            }while(!ptr_window->buttons[button_check].enabled);
            current_button=button_check;
        }
        else{
            if(command==COMMAND_UP){
                if(!LOOKING){
                    //As long as the game is not paused and is in progress.
                    if(!pause && game_in_progress && !bubble_mode){
                        //If the current level is not the world map.
                        if(!on_worldmap() && !DYING){
                            bool trigger_used=false;

                            for(int i=0;i<vector_triggers.size();i++){
                                if(fabs(vector_triggers[i].x-x)<PROCESS_RANGE && fabs(vector_triggers[i].y-y)<PROCESS_RANGE){
                                    //If the trigger is active, its user type is the player, and it is manually activated.
                                    if(vector_triggers[i].active && !vector_triggers[i].trigger_method){
                                        if(collision_check(x,y,w,h,vector_triggers[i].x,vector_triggers[i].y,vector_triggers[i].w,vector_triggers[i].h)){
                                            vector_triggers[i].use();
                                            play_positional_sound(sound_system.trigger_click,vector_triggers[i].x,vector_triggers[i].y);

                                            if(vector_triggers[i].render_trigger!=0){
                                                stat_levers_pulled++;
                                            }

                                            trigger_used=true;
                                            break;
                                        }
                                    }
                                }
                            }

                            if(!trigger_used){
                                for(int i=0;i<vector_signs.size();i++){
                                    if(fabs(vector_signs[i].x-x)<PROCESS_RANGE && fabs(vector_signs[i].y-y)<PROCESS_RANGE){
                                        if(collision_check(x,y,w,h,vector_signs[i].x,vector_signs[i].y,SIGN_W,SIGN_H)){
                                            short font_type=vector_signs[i].font_type;
                                            if(translator && font_type!=0){
                                                font_type=0;

                                                play_positional_sound(sound_system.item_collect_translator,x,y);
                                            }
                                            else{
                                                play_positional_sound(sound_system.read_sign,x,y);
                                            }

                                            window_message[0].set_message("",vector_signs[i].message,font_type);

                                            break;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        break;

    case COMMAND_DOWN: case COMMAND_RIGHT:
        //If a window is open.
        if(ptr_window!=NULL){
            if(current_button==-1){
                current_button=0;
            }

            button_check=current_button;
            last_check=-1;
            do{
                if(++button_check>ptr_window->buttons.size()-1){
                    button_check=1;
                    if(ptr_window->buttons.size()==1){
                        button_check=0;
                    }
                }

                if(last_check==current_button){
                    button_check=current_button;
                    break;
                }

                last_check=button_check;
            }while(!ptr_window->buttons[button_check].enabled);
            current_button=button_check;
        }
        break;

    }
}

void Player::crouch_start(){
    if(!CROUCHING && !water_running){
        CROUCHING=true;

        y+=PLAYER_CROUCH_H;

        //Set the player's dimensions to the crouching dimensions.
        w=PLAYER_CROUCH_W;
        h=PLAYER_CROUCH_H;
    }
}

void Player::crouch_stop(){
    if(CROUCHING && !solid_above){
        //If the player is holding the down key, crouching will be activated.
        CROUCHING=false;

        y-=PLAYER_CROUCH_H;

        //Set the player's dimensions to the standard dimensions.
        w=PLAYER_W;
        h=PLAYER_H;
    }
}

void Player::handle_input_states(){
    handle_input_states_always();

    handle_input_states_during_play();

    //If the player is alive.
    if(!DYING && !bubble_mode){
        //Begin here not crouching.
        crouch_stop();

        //************************//
        // Handle direction keys: //
        //************************//

        climb_state=NONE;

        //If the left directional key is pressed.
        if(command_state(COMMAND_LEFT)){
            //Set their move state to match the directional key.
            move_state=LEFT;

            //Set their facing variable to match the directional key.
            facing=LEFT;

            //When only one directional key is pressed, shoot_state and move_state are the same.
            shoot_state=move_state;
        }

        //If the up directional key is pressed.
        if(command_state(COMMAND_UP)){
            move_state=UP;

            //When only one directional key is pressed, shoot_state and move_state are the same.
            shoot_state=move_state;

            climb_state=move_state;
        }

        //If the player is swimming and the jump key is pressed, mimic the up direction key's swimming function.
        if(SWIMMING && command_state(COMMAND_JUMP)){
            move_state=UP;
        }

        //If the right directional key is pressed.
        if(command_state(COMMAND_RIGHT)){
            //Set their move state to match the directional key.
            move_state=RIGHT;

            //Set their facing variable to match the directional key.
            facing=RIGHT;

            //When only one directional key is pressed, shoot_state and move_state are the same.
            shoot_state=move_state;
        }

        //If the down directional key is pressed.
        if(command_state(COMMAND_DOWN)){
            move_state=DOWN;

            //When only one directional key is pressed, shoot_state and move_state are the same.
            shoot_state=move_state;

            climb_state=move_state;

            if(!SWIMMING && !CLIMBING && !LOOKING && !SHOOTING){
                crouch_start();
            }
        }

        //*******************************************************//
        // Handle multiple direction keys being pressed at once: //
        //*******************************************************//

        //When swimming, 8 directions of movement are possible.
        if(SWIMMING && !CLIMBING){
            //Priorities:
            //Movement: Left, Up, Right, Down.

            if(command_state(COMMAND_LEFT) && (command_state(COMMAND_UP) || command_state(COMMAND_JUMP))){
                move_state=LEFT_UP;
            }
            if(command_state(COMMAND_RIGHT) && (command_state(COMMAND_UP) || command_state(COMMAND_JUMP))){
                move_state=RIGHT_UP;
            }
            if(command_state(COMMAND_RIGHT) && command_state(COMMAND_DOWN)){
                move_state=RIGHT_DOWN;
            }
            if(command_state(COMMAND_LEFT) && command_state(COMMAND_DOWN)){
                move_state=LEFT_DOWN;
            }
            if(command_state(COMMAND_LEFT) && command_state(COMMAND_RIGHT)){
                move_state=LEFT;
            }
            if((command_state(COMMAND_UP) || command_state(COMMAND_JUMP)) && command_state(COMMAND_DOWN)){
                move_state=UP;
            }
            if(command_state(COMMAND_LEFT) && (command_state(COMMAND_UP) || command_state(COMMAND_JUMP)) && command_state(COMMAND_RIGHT)){
                move_state=LEFT_UP;
            }
            if(command_state(COMMAND_LEFT) && command_state(COMMAND_DOWN) && command_state(COMMAND_RIGHT)){
                move_state=LEFT_DOWN;
            }
            if(command_state(COMMAND_LEFT) && (command_state(COMMAND_UP) || command_state(COMMAND_JUMP)) && command_state(COMMAND_DOWN)){
                move_state=LEFT_UP;
            }
            if((command_state(COMMAND_UP) || command_state(COMMAND_JUMP)) && command_state(COMMAND_RIGHT) && command_state(COMMAND_DOWN)){
                move_state=RIGHT_UP;
            }
            if(command_state(COMMAND_LEFT) && (command_state(COMMAND_UP) || command_state(COMMAND_JUMP)) && command_state(COMMAND_RIGHT) && command_state(COMMAND_DOWN)){
                move_state=LEFT_UP;
            }

            //If no directional keys are pressed, the player is not moving.
            if(!command_state(COMMAND_LEFT) && (!command_state(COMMAND_UP) && !command_state(COMMAND_JUMP)) && !command_state(COMMAND_RIGHT) && !command_state(COMMAND_DOWN)){
                move_state=0;
            }
        }

        //When on land or in the air, 4 directions of movement are possible.
        else if(!SWIMMING && !CLIMBING){
            //Priorities:
            //Movement: Left, Up, Right, Down.
            //Shooting on the ground: Up, Left, Right, Down.
            //Shooting in the air: Up, Down, Left, Right.

            if(command_state(COMMAND_LEFT) && command_state(COMMAND_UP)){
                move_state=LEFT;
                facing=LEFT;
                shoot_state=UP;
                climb_state=UP;
            }
            if(command_state(COMMAND_RIGHT) && command_state(COMMAND_UP)){
                move_state=RIGHT;
                facing=RIGHT;
                shoot_state=UP;
                climb_state=UP;
            }
            if(command_state(COMMAND_RIGHT) && command_state(COMMAND_DOWN)){
                move_state=RIGHT;
                facing=RIGHT;
                if(!IN_AIR){
                    shoot_state=RIGHT;
                }
                if(IN_AIR){
                    shoot_state=DOWN;
                }
                climb_state=DOWN;
            }
            if(command_state(COMMAND_LEFT) && command_state(COMMAND_DOWN)){
                move_state=LEFT;
                facing=LEFT;
                if(!IN_AIR){
                    shoot_state=LEFT;
                }
                if(IN_AIR){
                    shoot_state=DOWN;
                }
                climb_state=DOWN;
            }
            if(command_state(COMMAND_LEFT) && command_state(COMMAND_RIGHT)){
                move_state=LEFT;
                facing=LEFT;
                shoot_state=LEFT;
            }
            if(command_state(COMMAND_UP) && command_state(COMMAND_DOWN)){
                move_state=UP;
                shoot_state=UP;
                climb_state=UP;
            }
            if(command_state(COMMAND_LEFT) && command_state(COMMAND_UP) && command_state(COMMAND_RIGHT)){
                move_state=LEFT;
                facing=LEFT;
                shoot_state=UP;
                climb_state=UP;
            }
            if(command_state(COMMAND_LEFT) && command_state(COMMAND_DOWN) && command_state(COMMAND_RIGHT)){
                move_state=LEFT;
                facing=LEFT;
                if(!IN_AIR){
                    shoot_state=LEFT;
                }
                if(IN_AIR){
                    shoot_state=DOWN;
                }
                climb_state=DOWN;
            }
            if(command_state(COMMAND_LEFT) && command_state(COMMAND_UP) && command_state(COMMAND_DOWN)){
                move_state=LEFT;
                facing=LEFT;
                shoot_state=UP;
                climb_state=UP;
            }
            if(command_state(COMMAND_UP) && command_state(COMMAND_RIGHT) && command_state(COMMAND_DOWN)){
                move_state=RIGHT;
                facing=RIGHT;
                shoot_state=UP;
                climb_state=UP;
            }
            if(command_state(COMMAND_LEFT) && command_state(COMMAND_UP) && command_state(COMMAND_RIGHT) && command_state(COMMAND_DOWN)){
                move_state=LEFT;
                facing=LEFT;
                shoot_state=UP;
                climb_state=UP;
            }

            //If no directional keys are pressed, the player is not moving.
            if(!command_state(COMMAND_LEFT) && !command_state(COMMAND_UP) && !command_state(COMMAND_RIGHT) && !command_state(COMMAND_DOWN)){
                move_state=0;
                shoot_state=0;
                climb_state=0;
                look_state=0;
            }
        }

        //When climbing, only 2 directions of movement are possible.
        else if(CLIMBING && !SWIMMING){
            //Priorities:
            //Movement: Up, Down.

            if(command_state(COMMAND_LEFT) && command_state(COMMAND_UP)){
                move_state=UP;
                facing=LEFT;
                shoot_state=UP;
            }
            if(command_state(COMMAND_RIGHT) && command_state(COMMAND_UP)){
                move_state=UP;
                facing=RIGHT;
                shoot_state=UP;
            }
            if(command_state(COMMAND_RIGHT) && command_state(COMMAND_DOWN)){
                move_state=DOWN;
                facing=RIGHT;
                shoot_state=DOWN;
            }
            if(command_state(COMMAND_LEFT) && command_state(COMMAND_DOWN)){
                move_state=DOWN;
                facing=LEFT;
                shoot_state=DOWN;
            }
            if(command_state(COMMAND_LEFT) && command_state(COMMAND_RIGHT)){
                facing=LEFT;
                shoot_state=LEFT;
            }
            if(command_state(COMMAND_UP) && command_state(COMMAND_DOWN)){
                move_state=UP;
                shoot_state=UP;
            }
            if(command_state(COMMAND_LEFT) && command_state(COMMAND_UP) && command_state(COMMAND_RIGHT)){
                move_state=UP;
                facing=LEFT;
                shoot_state=UP;
            }
            if(command_state(COMMAND_LEFT) && command_state(COMMAND_DOWN) && command_state(COMMAND_RIGHT)){
                move_state=DOWN;
                facing=LEFT;
                shoot_state=DOWN;
            }
            if(command_state(COMMAND_LEFT) && command_state(COMMAND_UP) && command_state(COMMAND_DOWN)){
                move_state=UP;
                facing=LEFT;
                shoot_state=UP;
            }
            if(command_state(COMMAND_UP) && command_state(COMMAND_RIGHT) && command_state(COMMAND_DOWN)){
                move_state=UP;
                facing=RIGHT;
                shoot_state=UP;
            }
            if(command_state(COMMAND_LEFT) && command_state(COMMAND_UP) && command_state(COMMAND_RIGHT) && command_state(COMMAND_DOWN)){
                move_state=UP;
                facing=LEFT;
                shoot_state=UP;
            }

            //If no directional keys are pressed, the player is not moving.
            if(!command_state(COMMAND_LEFT) && !command_state(COMMAND_UP) && !command_state(COMMAND_RIGHT) && !command_state(COMMAND_DOWN)){
                frame=0;
                frame_counter=0;
                move_state=0;
                shoot_state=0;
                look_state=0;
            }
        }

        if(LOOKING){
            //Reset the standard movement variables.
            frame=0;
            frame_counter=0;
            move_state=0;
            climb_state=0;
        }

        //If look mode is on, and the player is not currently in the air or moving, the directional keys affect the look offsets instead of anything else.
        if(LOOKING && move_state==0 && !IN_AIR){
            if(command_state(COMMAND_LEFT)){
                look_state=LEFT;
            }
            if(command_state(COMMAND_UP)){
                look_state=UP;
            }
            if(command_state(COMMAND_RIGHT)){
                look_state=RIGHT;
            }
            if(command_state(COMMAND_DOWN)){
                look_state=DOWN;
            }

            //Handle multiple directional keys being pressed at once.

            if(command_state(COMMAND_LEFT) && command_state(COMMAND_UP)){
                look_state=LEFT_UP;
            }
            if(command_state(COMMAND_RIGHT) && command_state(COMMAND_UP)){
                look_state=RIGHT_UP;
            }
            if(command_state(COMMAND_RIGHT) && command_state(COMMAND_DOWN)){
                look_state=RIGHT_DOWN;
            }
            if(command_state(COMMAND_LEFT) && command_state(COMMAND_DOWN)){
                look_state=LEFT_DOWN;
            }
            if(command_state(COMMAND_LEFT) && command_state(COMMAND_RIGHT)){
                look_state=LEFT;
            }
            if(command_state(COMMAND_UP) && command_state(COMMAND_DOWN)){
                look_state=UP;
            }
            if(command_state(COMMAND_LEFT) && command_state(COMMAND_UP) && command_state(COMMAND_RIGHT)){
                look_state=LEFT_UP;
            }
            if(command_state(COMMAND_LEFT) && command_state(COMMAND_DOWN) && command_state(COMMAND_RIGHT)){
                look_state=LEFT_DOWN;
            }
            if(command_state(COMMAND_LEFT) && command_state(COMMAND_UP) && command_state(COMMAND_DOWN)){
                look_state=LEFT_UP;
            }
            if(command_state(COMMAND_UP) && command_state(COMMAND_RIGHT) && command_state(COMMAND_DOWN)){
                look_state=RIGHT_UP;
            }
            if(command_state(COMMAND_LEFT) && command_state(COMMAND_UP) && command_state(COMMAND_RIGHT) && command_state(COMMAND_DOWN)){
                look_state=LEFT_UP;
            }

            //If no directional keys are pressed, we won't move the look offsets.
            if(!command_state(COMMAND_LEFT) && !command_state(COMMAND_UP) && !command_state(COMMAND_RIGHT) && !command_state(COMMAND_DOWN)){
                look_state=0;
            }
        }

        if(CROUCHING){
            frame=0;
            frame_counter=0;
            if(!IN_AIR){
                move_state=0;
            }
            look_state=0;
        }

        //*****************//
        // Handle jumping: //
        //*****************//

        if(!command_state(COMMAND_JUMP) && jump_state){
            jump_state=false;

            if(!CLIMBING_JUMP && air_velocity<jump_min*-1){
                air_velocity=jump_min*-1;
            }
            else if(CLIMBING_JUMP && air_velocity<climbing_jump_min*-1){
                air_velocity=climbing_jump_min*-1;
            }

            CLIMBING_JUMP=false;
        }

        after_input();
    }
}

void Player::handle_input_states_always(){
    //Toggle fullscreen.
    if((keystates[SDLK_LALT] || keystates[SDLK_RALT]) && keystates[SDLK_RETURN]){
        toggle_fullscreen();

        if(keystates[SDLK_LALT]){
            keystates[SDLK_LALT]=NULL;
        }
        if(keystates[SDLK_RALT]){
            keystates[SDLK_RALT]=NULL;
        }
        keystates[SDLK_RETURN]=NULL;
    }

    //Quit the game.
    if((keystates[SDLK_LALT] || keystates[SDLK_RALT]) && keystates[SDLK_F4]){
        quit_game();

        if(keystates[SDLK_LALT]){
            keystates[SDLK_LALT]=NULL;
        }
        if(keystates[SDLK_RALT]){
            keystates[SDLK_RALT]=NULL;
        }
        keystates[SDLK_F4]=NULL;
    }

    //Toggle main menu.
    if(keystates[SDLK_ESCAPE]){
        //If the escape key is pressed, stop setting the command.
        if(command_to_set!=-1){
            command_to_set=-1;
        }
        //If the game is in progress.
        else if(game_in_progress && command_to_set==-1){
            handle_command_event(COMMAND_TOGGLE_MAIN_MENU);
        }
        //If the game is not in progress and there is not command being set.
        else if(!game_in_progress && command_to_set==-1){
            //Close all windows.
            window_manager.close_windows(0);

            //If a profile exists.
            if(player.name!="\x1F"){
                vector_windows[WINDOW_MAIN_MENU].turn_on();

                window_manager.set_main_menu_current_button();
            }
            //If no profile exists.
            else{
                //Keep the Create Profile window open.
                vector_windows[WINDOW_CREATE_PROFILE].turn_on();
            }
        }

        keystates[SDLK_ESCAPE]=NULL;
    }

    //Toggle dev mode.
    if(keystates[SDLK_d] && keystates[SDLK_e] && keystates[SDLK_v]){
        option_dev=!option_dev;
        options_save();

        keystates[SDLK_d]=NULL;
        keystates[SDLK_e]=NULL;
        keystates[SDLK_v]=NULL;
    }

    if(window_manager.which_window_open()==WHICH_WINDOW_OTHER+WINDOW_SOUND_TEST){
        int music_keys[12];
        music_keys[0]=SDLK_q;
        music_keys[1]=SDLK_w;
        music_keys[2]=SDLK_e;
        music_keys[3]=SDLK_r;
        music_keys[4]=SDLK_t;
        music_keys[5]=SDLK_y;
        music_keys[6]=SDLK_u;
        music_keys[7]=SDLK_i;
        music_keys[8]=SDLK_o;
        music_keys[9]=SDLK_p;
        music_keys[10]=SDLK_LEFTBRACKET;
        music_keys[11]=SDLK_RIGHTBRACKET;

        for(int i=0;i<12;i++){
            if(keystates[music_keys[i]]){
                current_button=i+1;
                vector_windows[WINDOW_SOUND_TEST].buttons[current_button].mouse_button_down();
                vector_windows[WINDOW_SOUND_TEST].buttons[current_button].mouse_button_up(&vector_windows[WINDOW_SOUND_TEST]);
                vector_windows[WINDOW_SOUND_TEST].buttons[current_button].reset_clicked();
                keystates[music_keys[i]]=NULL;
            }
        }
    }
}

void Player::handle_input_states_during_play(){
    //If the player is alive and developer mode is on.
    if(option_dev){
        //******************//
        // Camera controls: //
        //******************//

        //If numpad 0 is pressed, toggle the camera's stickiness and play the appropriate sound.
        if(keystates[SDLK_KP0]){
            if(cam_state==CAM_STICKY){
                cam_state=NONE;
                play_positional_sound(sound_system.camera_unlock);
            }
            else{
                //Reset the look offsets.
                look_offset_x=0;
                look_offset_y=0;
                LOOKING=false;

                camera_trap_x=x;
                if(!on_worldmap()){
                    camera_trap_y=y+h-CAMERA_TRAP_H;
                }
                else{
                    camera_trap_y=y+h-CAMERA_TRAP_WORLDMAP_H;
                }

                cam_state=CAM_STICKY;
                play_positional_sound(sound_system.camera_lock);
            }

            //Once the toggle camera stickiness key has been hit, the player must release it for it to function again.
            keystates[SDLK_KP0]=NULL;
        }

        if(keystates[SDLK_KP7] && cam_state!=CAM_STICKY){
            if(camera_speed==24){
                camera_speed=96;
            }
            else{
                camera_speed=24;
            }

            keystates[SDLK_KP7]=NULL;
        }

        //If the camera is unsticky, check for camera inputs.
        if(cam_state!=CAM_STICKY){
            //Handle camera directional keys being pressed.
            if(keystates[SDLK_KP1]){
                cam_state=LEFT;
            }
            if(keystates[SDLK_KP5]){
                cam_state=UP;
            }
            if(keystates[SDLK_KP3]){
                cam_state=RIGHT;
            }
            if(keystates[SDLK_KP2]){
                cam_state=DOWN;
            }

            //Handle multiple camera directional keys being pressed at once.
            if(keystates[SDLK_KP1] && keystates[SDLK_KP5]){
                cam_state=LEFT_UP;
            }
            if(keystates[SDLK_KP5] && keystates[SDLK_KP3]){
                cam_state=RIGHT_UP;
            }
            if(keystates[SDLK_KP3] && keystates[SDLK_KP2]){
                cam_state=RIGHT_DOWN;
            }
            if(keystates[SDLK_KP2] && keystates[SDLK_KP1]){
                cam_state=LEFT_DOWN;
            }
            if(keystates[SDLK_KP1] && keystates[SDLK_KP3]){
                cam_state=LEFT;
            }
            if(keystates[SDLK_KP5] && keystates[SDLK_KP2]){
                cam_state=UP;
            }
            if(keystates[SDLK_KP1] && keystates[SDLK_KP5] && keystates[SDLK_KP3]){
                cam_state=LEFT_UP;
            }
            if(keystates[SDLK_KP1] && keystates[SDLK_KP2] && keystates[SDLK_KP3]){
                cam_state=LEFT_DOWN;
            }
            if(keystates[SDLK_KP1] && keystates[SDLK_KP5] && keystates[SDLK_KP2]){
                cam_state=LEFT_UP;
            }
            if(keystates[SDLK_KP5] && keystates[SDLK_KP3] && keystates[SDLK_KP2]){
                cam_state=RIGHT_UP;
            }
            if(keystates[SDLK_KP1] && keystates[SDLK_KP5] && keystates[SDLK_KP3] && keystates[SDLK_KP2]){
                cam_state=LEFT_UP;
            }

            //If no camera directional keys are pressed, stop the camera.
            if(!keystates[SDLK_KP1] && !keystates[SDLK_KP5] && !keystates[SDLK_KP3] && !keystates[SDLK_KP2]){
                cam_state=0;
            }
        }
    }

    if(game_in_progress && (keystates[SDLK_RALT] || keystates[SDLK_LALT]) && keystates[SDLK_z]){
        hide_gui=!hide_gui;

        keystates[SDLK_RALT]=NULL;
        keystates[SDLK_LALT]=NULL;
        keystates[SDLK_z]=NULL;
    }

    if(game_in_progress && game_mode==GAME_MODE_SP_ADVENTURE){
        if(keystates[SDLK_b] && keystates[SDLK_a] && keystates[SDLK_t]){
            ammo+=99;
            profile.save_inventory();

            keystates[SDLK_b]=NULL;
            keystates[SDLK_a]=NULL;
            keystates[SDLK_t]=NULL;
        }
    }

    ///******************///
    /// Debug/Test Code. ///
    ///******************///

    //If developer mode is enabled and the dev key is pressed.
    if(option_dev && keystates[SDLK_F1]){
        if(keystates[SDLK_l]){
            levelshot();

            keystates[SDLK_l]=NULL;
        }

        if(keystates[SDLK_q]){
            vector_items.push_back(Item(x+(w-sprites_item_candy[0].w)/2.0,y-sprites_item_candy[0].h,true,random_range(ITEM_CANDY,ITEM_CANDY),0,false,40,80,80,160,false,60));

            for(int i=0;i<mp_players.size();i++){
                vector_items.push_back(Item(mp_players[i].x+(mp_players[i].w-sprites_item_candy[0].w)/2.0,mp_players[i].y-sprites_item_candy[0].h,true,random_range(ITEM_CANDY,ITEM_CANDY),0,false,40,80,80,160,false,60));
            }
        }

        if(keystates[SDLK_a]){
            ammo+=99999;
            for(int i=0;i<mp_players.size();i++){
                mp_players[i].ammo+=99999;
            }

            score+=999999;

            for(short i=ITEM_SWIMMING_GEAR;i<ITEM_END;i++){
                if(i!=ITEM_J_BALLOON && i!=ITEM_AMMO_BARREL && i!=ITEM_CANDY){
                    if(!check_inventory(i)){
                        //Find the next available inventory slot.
                        short next_available_slot=next_available_inventory_slot();

                        //If there is a free inventory slot.
                        if(next_available_slot!=-1){
                            inventory.push_back(inventory_item());

                            inventory[inventory.size()-1].type=i;

                            inventory[inventory.size()-1].slot=next_available_slot;

                            inventory[inventory.size()-1].name=name_inventory_item(i);

                            //Create an inventory item notification slider.
                            sliders.push_back(Slider(i,false));

                            if(i==ITEM_SWIMMING_GEAR){
                                oxygen=oxygen_max_capacity;
                                for(int mps=0;mps<mp_players.size();mps++){
                                    mp_players[mps].oxygen=mp_players[mps].oxygen_max_capacity;
                                }
                            }
                        }
                    }
                }
            }

            check_special_items();
            for(int mps=0;mps<mp_players.size();mps++){
                mp_players[mps].check_special_items();
            }

            keystates[SDLK_a]=NULL;
        }

        short npc_type=0;

        if(keystates[SDLK_c]){
            npc_type=NPC_COW;
            keystates[SDLK_c]=NULL;
        }
        if(keystates[SDLK_b]){
            npc_type=NPC_BOUNCING_BALL;
            keystates[SDLK_b]=NULL;
        }
        if(keystates[SDLK_w]){
            npc_type=NPC_SALLY;
            keystates[SDLK_w]=NULL;
        }
        if(keystates[SDLK_v]){
            npc_type=NPC_MECHSUIT_MOUSE;
            keystates[SDLK_v]=NULL;
        }
        if(keystates[SDLK_r]){
            npc_type=NPC_REPLICATOR;
            keystates[SDLK_r]=NULL;
        }

        if(npc_type!=0){
            for(short i=0;i<1;i++){
                vector_npcs.push_back(Npc(x,y,npc_type));
            }
        }

        if(keystates[SDLK_g]){
            invulnerable=!invulnerable;
            for(int i=0;i<mp_players.size();i++){
                mp_players[i].invulnerable=!mp_players[i].invulnerable;
            }

            keystates[SDLK_g]=NULL;
        }

        if(keystates[SDLK_t]){
            show_tracers=!show_tracers;

            keystates[SDLK_t]=NULL;
        }

        if(keystates[SDLK_p]){
            show_paths=!show_paths;

            keystates[SDLK_p]=NULL;
        }

        if(keystates[SDLK_j]){
            cheat_jump=!cheat_jump;

            keystates[SDLK_j]=NULL;
        }

        if(keystates[SDLK_n]){
            cheat_noclip=!cheat_noclip;
            IN_AIR=true;
            air_velocity=0;

            keystates[SDLK_n]=NULL;
        }

        if(keystates[SDLK_e] && game_mode==GAME_MODE_SP_ADVENTURE && !on_worldmap()){
            //Make sure these aren't 0, since if they were, the game would mistakenly think the player
            //beat the level without shooting or killing.
            special_count_shots_this_level=1;
            special_count_kills_this_level=1;

            player.boss_end();

            profile.save_level_data();
            previous_level=current_level;
            previous_sub_level=current_sub_level;
            current_level=current_worldmap;
            current_checkpoint=-1;
            load_data();
            level.load_level();

            keystates[SDLK_e]=NULL;

            //Skip the rest of this function, as we are now on a different level.
            return;
        }

        /**if(keystates[SDLK_r] && game_mode==GAME_MODE_SP_ADVENTURE && on_worldmap()){
            world_x[current_level]=x;
            world_y[current_level]=y;
            profile.save_level_data();
            previous_level=current_level;
            previous_sub_level=current_sub_level;
            current_level=fabs(100);
            current_sub_level=0;
            //Until current_checkpoint is not equal to -1, the player will respawn at the spawn point.
            player.current_checkpoint=-1;
            if(on_worldmap()){
                current_worldmap=current_level;
            }
            load_data();
            level.generate_level();

            keystates[SDLK_r]=NULL;

            //Skip the rest of this function, as we are now on a different level.
            return;
        }*/

        if(keystates[SDLK_d] && !on_worldmap()){
            handle_death(x,y,w,h,true);
            for(int i=0;i<mp_players.size();i++){
                mp_players[i].handle_death(mp_players[i].x,mp_players[i].y,mp_players[i].w,mp_players[i].h,true);
            }

            keystates[SDLK_d]=NULL;
        }

        if(keystates[SDLK_k]){
            for(int i=0;i<vector_npcs.size();i++){
                vector_npcs[i].handle_death(true);
            }
            for(int i=0;i<vector_traps.size();i++){
                vector_traps[i].active=false;
                vector_traps[i].dangerous=false;
            }

            keystates[SDLK_k]=NULL;
        }

        if(keystates[SDLK_s]){
            if(UPDATE_RATE==DEFAULT_UPDATE_RATE){
                UPDATE_RATE=DEFAULT_UPDATE_RATE/2.4;
            }
            else if(UPDATE_RATE==DEFAULT_UPDATE_RATE/2.4){
                UPDATE_RATE=DEFAULT_UPDATE_RATE/4.0;
            }
            else{
                UPDATE_RATE=DEFAULT_UPDATE_RATE;
            }
            SKIP_TICKS=1000.0/UPDATE_RATE;

            keystates[SDLK_s]=NULL;
        }

        /**if(keystates[SDLK_RIGHT] && !on_worldmap()){
            x+=32.0;
            for(int i=0;i<mp_players.size();i++){
                mp_players[i].x+=32.0;
            }

            keystates[SDLK_RIGHT]=NULL;
        }
        else if(keystates[SDLK_LEFT] && !on_worldmap()){
            x-=32.0;
            for(int i=0;i<mp_players.size();i++){
                mp_players[i].x-=32.0;
            }

            keystates[SDLK_LEFT]=NULL;
        }
        else if(keystates[SDLK_UP] && !on_worldmap()){
            y-=32.0;
            for(int i=0;i<mp_players.size();i++){
                mp_players[i].y-=32.0;
            }

            keystates[SDLK_UP]=NULL;
        }
        else if(keystates[SDLK_DOWN] && !on_worldmap()){
            y+=32.0;
            for(int i=0;i<mp_players.size();i++){
                mp_players[i].y+=32.0;
            }

            keystates[SDLK_DOWN]=NULL;
        }*/
    }

    //If developer mode is enabled.
    if(option_dev){
        int mouse_x=0;
        int mouse_y=0;
        if(!pause && SDL_GetMouseState(&mouse_x,&mouse_y)&SDL_BUTTON(SDL_BUTTON_LEFT)){
            mouse_x*=(double)((double)main_window.SCREEN_WIDTH/(double)main_window.REAL_SCREEN_WIDTH);
            mouse_y*=(double)((double)main_window.SCREEN_HEIGHT/(double)main_window.REAL_SCREEN_HEIGHT);
            x=(int)(mouse_x+camera_x);
            y=(int)(mouse_y+camera_y);
            air_velocity=0;

            for(int i=0;i<mp_players.size();i++){
                mp_players[i].x=(int)(mouse_x+camera_x);
                mp_players[i].y=(int)(mouse_y+camera_y);
                mp_players[i].air_velocity=0;
            }
        }
    }

    ///*************************///
    /// End of Debug/Test Code. ///
    ///*************************///
}

bool Player::command_state(short command){
    Sint16 axis_value=0;
    Uint8 hat_value=SDL_HAT_CENTERED;
    short ball_direction=NONE;
    int ball_delta_x=0;
    int ball_delta_y=0;

    //If this command's bound input is a keyboard key, and the key is being pressed.
    if(keys[command].type==INPUT_TYPE_KEYBOARD && keystates[keys[command].key]){
        return true;
    }

    //Check all available joysticks for input.
    for(int i=0;i<number_of_joysticks;i++){
        //As long as this joystick is opened properly, and is the joystick associated with this command.
        if(SDL_JoystickOpened(i) && joystick[i].joy!=NULL && i==keys[command].which_joystick){
            //If this command's bound input is a joystick button, and the button is being pressed.
            if(keys[command].type==INPUT_TYPE_JOYSTICK_BUTTON && SDL_JoystickGetButton(joystick[i].joy,keys[command].joy_button)){
                return true;
            }

            //If this command's bound input is a joystick axis.
            else if(keys[command].type==INPUT_TYPE_JOYSTICK_AXIS){
                //Get the current value of this command's bound axis.
                axis_value=SDL_JoystickGetAxis(joystick[i].joy,keys[command].joy_axis);

                //If the axis is outside of the neutral zone and held in the correct direction.
                if(!keys[command].joy_axis_direction && axis_value<JOYSTICK_NEUTRAL_NEGATIVE){
                    return true;
                }
                else if(keys[command].joy_axis_direction && axis_value>JOYSTICK_NEUTRAL_POSITIVE){
                    return true;
                }
            }

            //If this command's bound input is a joystick hat.
            else if(keys[command].type==INPUT_TYPE_JOYSTICK_HAT){
                //Get the current value of this command's bound hat.
                hat_value=SDL_JoystickGetHat(joystick[i].joy,keys[command].joy_hat);

                //If the hat is held in the correct direction.
                if(hat_value==keys[command].joy_hat_direction){
                    return true;
                }
            }

            //If this command's bound input is a joystick ball.
            else if(keys[command].type==INPUT_TYPE_JOYSTICK_BALL){
                //Get the current delta movement of this command's bound ball.
                SDL_JoystickGetBall(joystick[i].joy,keys[command].joy_ball,&ball_delta_x,&ball_delta_y);

                //Set the ball's direction using the deltas.
                if(ball_delta_x<0 && ball_delta_y<0){
                    ball_direction=LEFT_UP;
                }
                else if(ball_delta_x<0 && ball_delta_y>0){
                    ball_direction=LEFT_DOWN;
                }
                else if(ball_delta_x>0 && ball_delta_y<0){
                    ball_direction=RIGHT_UP;
                }
                else if(ball_delta_x>0 && ball_delta_y>0){
                    ball_direction=RIGHT_DOWN;
                }
                else if(ball_delta_x<0){
                    ball_direction=LEFT;
                }
                else if(ball_delta_x>0){
                    ball_direction=RIGHT;
                }
                else if(ball_delta_y<0){
                    ball_direction=UP;
                }
                else if(ball_delta_y>0){
                    ball_direction=DOWN;
                }

                //If the ball has moved in the correct direction.
                if(ball_direction==keys[command].joy_ball_direction){
                    return true;
                }
            }
        }
    }

    //If neither the key nor the joystick input bound to this command are active,
    //return false to let the game know this command is not currently being given.
    return false;
}

string Player::command_bound_input(short command,short player_index){
    string string_to_return="None";

    if(player_index==-1){
        //If this command's bound input is a keyboard key.
        if(keys[command].type==INPUT_TYPE_KEYBOARD){
            ss.clear();ss.str("");ss<<SDL_GetKeyName(keys[command].key);string_to_return=ss.str();
        }
        //If this command's bound input is a joystick button.
        else if(keys[command].type==INPUT_TYPE_JOYSTICK_BUTTON){
            ss.clear();ss.str("");ss<<"Joystick ";ss<<(int)keys[command].which_joystick;ss<<" Button ";ss<<(int)keys[command].joy_button;string_to_return=ss.str();
        }
        //If this command's bound input is a joystick axis.
        else if(keys[command].type==INPUT_TYPE_JOYSTICK_AXIS){
            ss.clear();ss.str("");ss<<"Joystick ";ss<<(int)keys[command].which_joystick;ss<<" Axis ";ss<<(int)keys[command].joy_axis;ss<<" ";ss<<keys[command].joy_axis_direction;string_to_return=ss.str();
        }
        //If this command's bound input is a joystick hat.
        else if(keys[command].type==INPUT_TYPE_JOYSTICK_HAT){
            ss.clear();ss.str("");ss<<"Joystick ";ss<<(int)keys[command].which_joystick;ss<<" Hat ";ss<<(int)keys[command].joy_hat;ss<<" ";string_to_return=ss.str();
            if(keys[command].joy_hat_direction==SDL_HAT_LEFT){
                string_to_return+="Left";
            }
            else if(keys[command].joy_hat_direction==SDL_HAT_UP){
                string_to_return+="Up";
            }
            else if(keys[command].joy_hat_direction==SDL_HAT_RIGHT){
                string_to_return+="Right";
            }
            else if(keys[command].joy_hat_direction==SDL_HAT_DOWN){
                string_to_return+="Down";
            }
            else if(keys[command].joy_hat_direction==SDL_HAT_LEFTUP){
                string_to_return+="Left Up";
            }
            else if(keys[command].joy_hat_direction==SDL_HAT_LEFTDOWN){
                string_to_return+="Left Down";
            }
            else if(keys[command].joy_hat_direction==SDL_HAT_RIGHTUP){
                string_to_return+="Right Up";
            }
            else if(keys[command].joy_hat_direction==SDL_HAT_RIGHTDOWN){
                string_to_return+="Right Down";
            }
        }
        //If this command's bound input is a joystick ball.
        else if(keys[command].type==INPUT_TYPE_JOYSTICK_BALL){
            ss.clear();ss.str("");ss<<"Joystick ";ss<<(int)keys[command].which_joystick;ss<<" Ball ";ss<<(int)keys[command].joy_ball;ss<<" ";string_to_return=ss.str();
            if(keys[command].joy_ball_direction==LEFT){
                string_to_return+="Left";
            }
            else if(keys[command].joy_ball_direction==UP){
                string_to_return+="Up";
            }
            else if(keys[command].joy_ball_direction==RIGHT){
                string_to_return+="Right";
            }
            else if(keys[command].joy_ball_direction==DOWN){
                string_to_return+="Down";
            }
            else if(keys[command].joy_ball_direction==LEFT_UP){
                string_to_return+="Left Up";
            }
            else if(keys[command].joy_ball_direction==LEFT_DOWN){
                string_to_return+="Left Down";
            }
            else if(keys[command].joy_ball_direction==RIGHT_UP){
                string_to_return+="Right Up";
            }
            else if(keys[command].joy_ball_direction==RIGHT_DOWN){
                string_to_return+="Right Down";
            }
        }
    }
    else{
        //If this command's bound input is a keyboard key.
        if(mp_keys[player_index][command].type==INPUT_TYPE_KEYBOARD){
            ss.clear();ss.str("");ss<<SDL_GetKeyName(mp_keys[player_index][command].key);string_to_return=ss.str();
        }
        //If this command's bound input is a joystick button.
        else if(mp_keys[player_index][command].type==INPUT_TYPE_JOYSTICK_BUTTON){
            ss.clear();ss.str("");ss<<"Joystick ";ss<<(int)mp_keys[player_index][command].which_joystick;ss<<" Button ";ss<<(int)mp_keys[player_index][command].joy_button;string_to_return=ss.str();
        }
        //If this command's bound input is a joystick axis.
        else if(mp_keys[player_index][command].type==INPUT_TYPE_JOYSTICK_AXIS){
            ss.clear();ss.str("");ss<<"Joystick ";ss<<(int)mp_keys[player_index][command].which_joystick;ss<<" Axis ";ss<<(int)mp_keys[player_index][command].joy_axis;ss<<" ";ss<<mp_keys[player_index][command].joy_axis_direction;string_to_return=ss.str();
        }
        //If this command's bound input is a joystick hat.
        else if(mp_keys[player_index][command].type==INPUT_TYPE_JOYSTICK_HAT){
            ss.clear();ss.str("");ss<<"Joystick ";ss<<(int)mp_keys[player_index][command].which_joystick;ss<<" Hat ";ss<<(int)mp_keys[player_index][command].joy_hat;ss<<" ";string_to_return=ss.str();
            if(mp_keys[player_index][command].joy_hat_direction==SDL_HAT_LEFT){
                string_to_return+="Left";
            }
            else if(mp_keys[player_index][command].joy_hat_direction==SDL_HAT_UP){
                string_to_return+="Up";
            }
            else if(mp_keys[player_index][command].joy_hat_direction==SDL_HAT_RIGHT){
                string_to_return+="Right";
            }
            else if(mp_keys[player_index][command].joy_hat_direction==SDL_HAT_DOWN){
                string_to_return+="Down";
            }
            else if(mp_keys[player_index][command].joy_hat_direction==SDL_HAT_LEFTUP){
                string_to_return+="Left Up";
            }
            else if(mp_keys[player_index][command].joy_hat_direction==SDL_HAT_LEFTDOWN){
                string_to_return+="Left Down";
            }
            else if(mp_keys[player_index][command].joy_hat_direction==SDL_HAT_RIGHTUP){
                string_to_return+="Right Up";
            }
            else if(mp_keys[player_index][command].joy_hat_direction==SDL_HAT_RIGHTDOWN){
                string_to_return+="Right Down";
            }
        }
        //If this command's bound input is a joystick ball.
        else if(mp_keys[player_index][command].type==INPUT_TYPE_JOYSTICK_BALL){
            ss.clear();ss.str("");ss<<"Joystick ";ss<<(int)mp_keys[player_index][command].which_joystick;ss<<" Ball ";ss<<(int)mp_keys[player_index][command].joy_ball;ss<<" ";string_to_return=ss.str();
            if(mp_keys[player_index][command].joy_ball_direction==LEFT){
                string_to_return+="Left";
            }
            else if(mp_keys[player_index][command].joy_ball_direction==UP){
                string_to_return+="Up";
            }
            else if(mp_keys[player_index][command].joy_ball_direction==RIGHT){
                string_to_return+="Right";
            }
            else if(mp_keys[player_index][command].joy_ball_direction==DOWN){
                string_to_return+="Down";
            }
            else if(mp_keys[player_index][command].joy_ball_direction==LEFT_UP){
                string_to_return+="Left Up";
            }
            else if(mp_keys[player_index][command].joy_ball_direction==LEFT_DOWN){
                string_to_return+="Left Down";
            }
            else if(mp_keys[player_index][command].joy_ball_direction==RIGHT_UP){
                string_to_return+="Right Up";
            }
            else if(mp_keys[player_index][command].joy_ball_direction==RIGHT_DOWN){
                string_to_return+="Right Down";
            }
        }
    }

    return string_to_return;
}

void Player::handle_input_events(){
    if(event.type==SDL_QUIT){
        quit_game();
        return;
    }

    bool allow_input_event=true;

    //If a command is being set.
    if(command_to_set!=-1){
        allow_input_event=false;
    }

    //If we are on the sound test window, and the key is a music key.
    if(window_manager.which_window_open()==WHICH_WINDOW_OTHER+WINDOW_SOUND_TEST){
        int music_keys[12];
        music_keys[0]=SDLK_q;
        music_keys[1]=SDLK_w;
        music_keys[2]=SDLK_e;
        music_keys[3]=SDLK_r;
        music_keys[4]=SDLK_t;
        music_keys[5]=SDLK_y;
        music_keys[6]=SDLK_u;
        music_keys[7]=SDLK_i;
        music_keys[8]=SDLK_o;
        music_keys[9]=SDLK_p;
        music_keys[10]=SDLK_LEFTBRACKET;
        music_keys[11]=SDLK_RIGHTBRACKET;

        for(int i=0;i<12;i++){
            if(event.type==SDL_KEYDOWN && event.key.keysym.sym==music_keys[i]){
                allow_input_event=false;
                break;
            }
        }
    }

    check_for_command_set();

    //As long as the player's input events are not being blocked.
    if(allow_input_event){
        //Look through all commands, and see if any should be triggered by this input event.
        for(int i=0;i<keys.size();i++){
            //If the input type is a keyboard keypress, and the event is a keyboard keypress.
            if(keys[i].type==INPUT_TYPE_KEYBOARD && event.type==SDL_KEYDOWN){
                if(event.key.keysym.sym==keys[i].key){
                    handle_command_event(i);
                    return;
                }
            }
            //If the input type is a joystick button press, and the event is a joystick button press, and the event joystick is the command's bound joystick.
            else if(keys[i].type==INPUT_TYPE_JOYSTICK_BUTTON && event.type==SDL_JOYBUTTONDOWN && event.jbutton.which==keys[i].which_joystick){
                if(event.jbutton.button==keys[i].joy_button){
                    handle_command_event(i);
                    return;
                }
            }
            //If the input type is a joystick axis motion, and the event is a joystick axis motion, and the event joystick is the command's bound joystick.
            else if(keys[i].type==INPUT_TYPE_JOYSTICK_AXIS && event.type==SDL_JOYAXISMOTION && event.jaxis.which==keys[i].which_joystick){
                if(event.jaxis.axis==keys[i].joy_axis){
                    if(!keys[i].joy_axis_direction && event.jaxis.value<JOYSTICK_NEUTRAL_NEGATIVE){
                        handle_command_event(i);
                        return;
                    }
                    else if(keys[i].joy_axis_direction && event.jaxis.value>JOYSTICK_NEUTRAL_POSITIVE){
                        handle_command_event(i);
                        return;
                    }
                }
            }
            //If the input type is a joystick hat motion, and the event is a joystick hat motion, and the event joystick is the command's bound joystick.
            else if(keys[i].type==INPUT_TYPE_JOYSTICK_HAT && event.type==SDL_JOYHATMOTION && event.jhat.which==keys[i].which_joystick){
                if(event.jhat.hat==keys[i].joy_hat){
                    if(keys[i].joy_hat_direction==event.jhat.value){
                        handle_command_event(i);
                        return;
                    }
                }
            }
            //If the input type is a joystick ball motion, and the event is a joystick ball motion, and the event joystick is the command's bound joystick.
            else if(keys[i].type==INPUT_TYPE_JOYSTICK_BALL && event.type==SDL_JOYBALLMOTION && event.jball.which==keys[i].which_joystick){
                if(event.jball.ball==keys[i].joy_ball){
                    short ball_direction=NONE;

                    //Set the ball's direction using the deltas.
                    if(event.jball.xrel<0 && event.jball.yrel<0){
                        ball_direction=LEFT_UP;
                    }
                    else if(event.jball.xrel<0 && event.jball.yrel>0){
                        ball_direction=LEFT_DOWN;
                    }
                    else if(event.jball.xrel>0 && event.jball.yrel<0){
                        ball_direction=RIGHT_UP;
                    }
                    else if(event.jball.xrel>0 && event.jball.yrel>0){
                        ball_direction=RIGHT_DOWN;
                    }
                    else if(event.jball.xrel<0){
                        ball_direction=LEFT;
                    }
                    else if(event.jball.xrel>0){
                        ball_direction=RIGHT;
                    }
                    else if(event.jball.yrel<0){
                        ball_direction=UP;
                    }
                    else if(event.jball.yrel>0){
                        ball_direction=DOWN;
                    }

                    //If the ball has moved in the correct direction.
                    if(ball_direction==keys[i].joy_ball_direction){
                        handle_command_event(i);
                        return;
                    }
                }
            }
        }
    }
}
