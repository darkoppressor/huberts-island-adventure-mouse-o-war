/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "trap.h"
#include "world.h"
#include "profile.h"

using namespace std;

void Trap::special_function_alarm_toggler_toggle(){
    level.alarm=!level.alarm;

    if(level.alarm){
        music.restart_track(MUSIC_ALARM);
        music.play_track(MUSIC_ALARM);
    }
    else{
        music.play_track(MUSIC_LEVEL_NORMAL);
    }
}

void Trap::special_function_weather_machine(){
    if(player.game_mode==GAME_MODE_MP_ADVENTURE){
        Level_Properties lp=player.mp_level_properties[4];
        if(lp.current_sub_level==0){
            lp.current_sub_level=1;
        }
        else if(lp.current_sub_level==1){
            lp.current_sub_level=0;
        }
        player.mp_level_properties[4]=lp;

        lp=player.mp_level_properties[11];
        if(lp.current_sub_level==0){
            lp.current_sub_level=1;
        }
        else if(lp.current_sub_level==1){
            lp.current_sub_level=0;
        }
        player.mp_level_properties[11]=lp;
    }
    else if(player.game_mode==GAME_MODE_SP_ADVENTURE){
        Level_Properties lp=profile.load_level_properties_weather_machine(4);
        if(lp.current_sub_level==0){
            lp.current_sub_level=1;
        }
        else if(lp.current_sub_level==1){
            lp.current_sub_level=0;
        }
        profile.save_level_properties_weather_machine(4,lp);

        lp=profile.load_level_properties_weather_machine(11);
        if(lp.current_sub_level==0){
            lp.current_sub_level=1;
        }
        else if(lp.current_sub_level==1){
            lp.current_sub_level=0;
        }
        profile.save_level_properties_weather_machine(11,lp);
    }

    player.weather_machine=!player.weather_machine;

    //We save inventory here since weather machine status is stored there.
    //We just saved the level properties affected by the weather machine, so ensure the game has the correct
    //weather machine state saved, too.
    profile.save_inventory();

    if(player.weather_machine){
        play_positional_sound(sound_system.weather_machine_on,x,y);
    }
    else{
        play_positional_sound(sound_system.weather_machine_off,x,y);
    }
}

void Trap::special_function_open_shop(){
    if(player.game_mode==GAME_MODE_SP_ADVENTURE){
        window_shop[0].setup();

        if(player.shop_upgrades.size()>0){
            window_manager.close_windows(0);

            window_shop[0].turn_on();

            player.current_button=2;
        }
        else{
            window_message[0].set_message("","There are no toys left to purchase.\nHave a pleasant day!",0);
        }
    }
    else{
        window_message[0].set_message("","Sorry, but the Shoppe-O-Matic is only available in singleplayer.",0);
    }
}

void Trap::special_function_open_upgrades(){
    if(player.game_mode==GAME_MODE_SP_ADVENTURE){
        window_upgrades[0].setup();

        if(player.upgrades.size()>0){
            window_manager.close_windows(0);

            window_upgrades[0].turn_on();

            player.current_button=2;
        }
        else{
            window_message[0].set_message("","You don't have any toys yet.\nPurchase some at the Shoppe-O-Matic with your candy!",0);
        }
    }
    else{
        window_message[0].set_message("","Sorry, but toys are only available in singleplayer.",0);
    }
}
