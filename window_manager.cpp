/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "window_manager.h"
#include "world.h"

using namespace std;

Window_Manager::Window_Manager(){
    allow_button_sound=false;

    return_to_world_map_sp_adventure=-1;
    return_to_world_map_mp_adventure=-1;

    which_mp_game_mode=GAME_MODE_MP_ADVENTURE;
}

void Window_Manager::close_windows(Window* window_to_keep){
    //Clear the profile creation string.
    creating_profile.clear();

    //No button is focused now.
    player.current_button=-1;

    //As long as the inventory window is not to be kept, close it.
    if(window_to_keep!=&window_inventory[0]){
        window_inventory[0].turn_off();
    }

    //As long as the map window is not to be kept, close it.
    if(window_to_keep!=&window_map[0]){
        window_map[0].turn_off();
    }

    //As long as the message window is not to be kept, close it.
    if(window_to_keep!=&window_message[0]){
        window_message[0].turn_off();
    }

    //As long as the setup windows are not to be kept, close them.
    if(window_to_keep!=&window_setup_survival[0]){
        window_setup_survival[0].turn_off();
    }

    if(window_to_keep!=&window_shop[0]){
        window_shop[0].turn_off();
    }

    if(window_to_keep!=&window_upgrades[0]){
        window_upgrades[0].turn_off();
    }

    for(int i=0;i<windows_level_info.size();i++){
        //As long as the current window is not to be kept, close it.
        if(window_to_keep!=&windows_level_info[i]){
            windows_level_info[i].turn_off();
        }
    }

    for(int i=0;i<vector_windows.size();i++){
        //As long as the current window is not to be kept, close it.
        if(window_to_keep!=&vector_windows[i]){
            vector_windows[i].turn_off();
        }
    }
}

short Window_Manager::which_window_open(){
    if(window_inventory[0].return_on()){
        return WHICH_WINDOW_INVENTORY;
    }

    if(window_map[0].return_on()){
        return WHICH_WINDOW_MAP;
    }

    if(window_message[0].return_on()){
        return WHICH_WINDOW_MESSAGE;
    }

    if(window_setup_survival[0].return_on()){
        return WHICH_WINDOW_SETUP_SURVIVAL;
    }

    if(window_shop[0].return_on()){
        return WHICH_WINDOW_SHOP;
    }

    if(window_upgrades[0].return_on()){
        return WHICH_WINDOW_UPGRADES;
    }

    for(int i=0;i<16;i++){
        if(windows_level_info[i].return_on()){
            return WHICH_WINDOW_LEVEL_INFO+i;
        }
    }

    for(int i=WINDOW_BEGIN;i<WINDOW_END;i++){
        if(vector_windows.size()>=i+1 && vector_windows[i].return_on()){
            return WHICH_WINDOW_OTHER+i;
        }
    }

    return -1;
}

void Window_Manager::configure_main_menu(){
    for(int i=0;i<main_menu_buttons_no_game.size();i++){
        vector_windows[WINDOW_MAIN_MENU].buttons[main_menu_buttons_no_game[i]].set_invisibility(true);
        vector_windows[WINDOW_MAIN_MENU].buttons[main_menu_buttons_no_game[i]].set_enabled(false);
    }

    for(int i=0;i<main_menu_buttons_sp_adventure.size();i++){
        vector_windows[WINDOW_MAIN_MENU].buttons[main_menu_buttons_sp_adventure[i]].set_invisibility(true);
        vector_windows[WINDOW_MAIN_MENU].buttons[main_menu_buttons_sp_adventure[i]].set_enabled(false);
    }

    for(int i=0;i<main_menu_buttons_mp_adventure.size();i++){
        vector_windows[WINDOW_MAIN_MENU].buttons[main_menu_buttons_mp_adventure[i]].set_invisibility(true);
        vector_windows[WINDOW_MAIN_MENU].buttons[main_menu_buttons_mp_adventure[i]].set_enabled(false);
    }

    for(int i=0;i<main_menu_buttons_survival.size();i++){
        vector_windows[WINDOW_MAIN_MENU].buttons[main_menu_buttons_survival[i]].set_invisibility(true);
        vector_windows[WINDOW_MAIN_MENU].buttons[main_menu_buttons_survival[i]].set_enabled(false);
    }

    if(!player.game_in_progress){
        for(int i=0;i<main_menu_buttons_no_game.size();i++){
            //Don't enable the change profile button, because the profile system is being removed
            if(vector_windows[WINDOW_MAIN_MENU].buttons[main_menu_buttons_no_game[i]].text!="Change Profile"){
                vector_windows[WINDOW_MAIN_MENU].buttons[main_menu_buttons_no_game[i]].set_invisibility(false);
                vector_windows[WINDOW_MAIN_MENU].buttons[main_menu_buttons_no_game[i]].set_enabled(true);
            }
        }
    }
    else{
        if(player.game_mode==GAME_MODE_SP_ADVENTURE){
            for(int i=0;i<main_menu_buttons_sp_adventure.size();i++){
                //Don't enable the change profile button, because the profile system is being removed
                if(vector_windows[WINDOW_MAIN_MENU].buttons[main_menu_buttons_sp_adventure[i]].text!="Change Profile"){
                    vector_windows[WINDOW_MAIN_MENU].buttons[main_menu_buttons_sp_adventure[i]].set_invisibility(false);
                    vector_windows[WINDOW_MAIN_MENU].buttons[main_menu_buttons_sp_adventure[i]].set_enabled(true);
                }
            }

            if(player.on_worldmap()){
                vector_windows[WINDOW_MAIN_MENU].buttons[return_to_world_map_sp_adventure].set_invisibility(true);
                vector_windows[WINDOW_MAIN_MENU].buttons[return_to_world_map_sp_adventure].set_enabled(false);
            }
        }
        else if(player.game_mode==GAME_MODE_MP_ADVENTURE){
            for(int i=0;i<main_menu_buttons_mp_adventure.size();i++){
                //Don't enable the change profile button, because the profile system is being removed
                if(vector_windows[WINDOW_MAIN_MENU].buttons[main_menu_buttons_mp_adventure[i]].text!="Change Profile"){
                    vector_windows[WINDOW_MAIN_MENU].buttons[main_menu_buttons_mp_adventure[i]].set_invisibility(false);
                    vector_windows[WINDOW_MAIN_MENU].buttons[main_menu_buttons_mp_adventure[i]].set_enabled(true);
                }
            }

            if(player.on_worldmap()){
                vector_windows[WINDOW_MAIN_MENU].buttons[return_to_world_map_mp_adventure].set_invisibility(true);
                vector_windows[WINDOW_MAIN_MENU].buttons[return_to_world_map_mp_adventure].set_enabled(false);
            }
        }
        else if(player.game_mode==GAME_MODE_SP_SURVIVAL || player.game_mode==GAME_MODE_MP_SURVIVAL){
            for(int i=0;i<main_menu_buttons_survival.size();i++){
                //Don't enable the change profile button, because the profile system is being removed
                if(vector_windows[WINDOW_MAIN_MENU].buttons[main_menu_buttons_survival[i]].text!="Change Profile"){
                    vector_windows[WINDOW_MAIN_MENU].buttons[main_menu_buttons_survival[i]].set_invisibility(false);
                    vector_windows[WINDOW_MAIN_MENU].buttons[main_menu_buttons_survival[i]].set_enabled(true);
                }
            }
        }
    }
}

void Window_Manager::set_main_menu_current_button(){
    if(!player.game_in_progress){
        player.current_button=main_menu_buttons_no_game[1];
    }
    else{
        if(player.game_mode==GAME_MODE_SP_ADVENTURE){
            player.current_button=main_menu_buttons_sp_adventure[2];
        }
        else if(player.game_mode==GAME_MODE_MP_ADVENTURE){
            player.current_button=main_menu_buttons_mp_adventure[2];
        }
        else if(player.game_mode==GAME_MODE_SP_SURVIVAL || player.game_mode==GAME_MODE_MP_SURVIVAL){
            player.current_button=main_menu_buttons_survival[1];
        }
    }
}

void Window_Manager::configure_mp_setup(){
    for(int i=3;i<=6;i++){
        if(i-2>player.mp_player_count){
            vector_windows[WINDOW_MP_SETUP].buttons[i].set_invisibility(true);
            vector_windows[WINDOW_MP_SETUP].buttons[i].set_enabled(false);
        }
        else{
            vector_windows[WINDOW_MP_SETUP].buttons[i].set_invisibility(false);
            vector_windows[WINDOW_MP_SETUP].buttons[i].set_enabled(true);
        }
    }

    for(int i=7;i<=9;i++){
        if(i-5>player.mp_player_count){
            vector_windows[WINDOW_MP_SETUP].buttons[i].set_invisibility(true);
            vector_windows[WINDOW_MP_SETUP].buttons[i].set_enabled(false);
        }
        else{
            vector_windows[WINDOW_MP_SETUP].buttons[i].set_invisibility(false);
            vector_windows[WINDOW_MP_SETUP].buttons[i].set_enabled(true);
        }
    }
}
