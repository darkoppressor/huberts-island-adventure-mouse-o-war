/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "information.h"
#include "render.h"
#include "world.h"
#include "collision.h"
#include "special_info_strings.h"
#include "holidays.h"

using namespace std;

Information::Information(short get_x,short get_y,string get_tooltip_text,string get_text,image_data* get_info_image,short get_special_info,SDL_Rect* get_info_sprite_sheet,short get_text_color,short get_font_type){
    x=get_x;
    y=get_y;

    text=get_text;
    tooltip_text=get_tooltip_text;

    info_image=get_info_image;
    info_sprite_sheet=get_info_sprite_sheet;

    text_color=get_text_color;
    if(text_color==COLOR_RAINBOW){
        text_color=return_gui_color(holiday,3);
    }

    special_info=get_special_info;

    font_type=get_font_type;

    if(text.length()>0){
        set_dimensions();
    }
}

void Information::set_dimensions(){
    //If the information has text.
    if(text.length()>0){
        short current_width=11;
        w=11;
        h=24;

        for(short i=0;i<text.length();i++){
            if(text[i]!='\xA'){
                current_width+=11;
            }
            //If we hit a new line.
            else{
                h+=24;
                if(current_width>w){
                    w=current_width;
                }
                current_width=11;
            }

            if(i==text.length()-1){
                if(current_width>w){
                    w=current_width;
                }
            }
        }
    }
    //If the information has an image.
    else if(info_image!=0){
        if(info_sprite_sheet!=0){
            w=info_sprite_sheet->w;
            h=info_sprite_sheet->h;
        }
        else{
            w=info_image->w;
            h=info_image->h;
        }
    }
}

void Information::set_special_string(){
    text.clear();

    special_info_return_data special_info_data=set_special_info_string(special_info);

    text=special_info_data.text;
    short command=special_info_data.command;

    short options_keys_width=600;

    //If the special info is a key bind.
    if(special_info>=SPECIAL_INFO_KEY_TOGGLE_MAIN_MENU && special_info<=SPECIAL_INFO_KEY_CHANGE_SHOT){
        //If this key is currently being set.
        if(player.current_command_to_set()==command){
            //If no input already bound message needs to be relayed.
            if(player.return_input_already_bound()==-1){
                ss.clear();ss.str("");ss<<"Press any key";text=ss.str();
            }
            //If we need to relay an input already bound message.
            else{
                text=player.command_bound_input(player.return_input_already_bound(),player.input_already_bound_player);
                text+=" is already bound.";
            }
        }
        //If this key is not being set.
        else{
            text=player.command_bound_input(command,-1);
        }

        set_dimensions();
        x=options_keys_width-w-15;
    }
    else if(special_info>=SPECIAL_INFO_KEY_1_LEFT && special_info<=SPECIAL_INFO_KEY_1_CHANGE_SHOT){
        //If this key is currently being set.
        if(player.current_command_to_set()==command){
            //If no input already bound message needs to be relayed.
            if(player.return_input_already_bound()==-1){
                ss.clear();ss.str("");ss<<"Press any key";text=ss.str();
            }
            //If we need to relay an input already bound message.
            else{
                text=player.command_bound_input(player.return_input_already_bound(),player.input_already_bound_player);
                text+=" is already bound.";
            }
        }
        //If this key is not being set.
        else{
            text=player.command_bound_input(command,0);
        }

        set_dimensions();
        x=options_keys_width-w-15;
    }
    else if(special_info>=SPECIAL_INFO_KEY_2_LEFT && special_info<=SPECIAL_INFO_KEY_2_CHANGE_SHOT){
        //If this key is currently being set.
        if(player.current_command_to_set()==command){
            //If no input already bound message needs to be relayed.
            if(player.return_input_already_bound()==-1){
                ss.clear();ss.str("");ss<<"Press any key";text=ss.str();
            }
            //If we need to relay an input already bound message.
            else{
                text=player.command_bound_input(player.return_input_already_bound(),player.input_already_bound_player);
                text+=" is already bound.";
            }
        }
        //If this key is not being set.
        else{
            text=player.command_bound_input(command,1);
        }

        set_dimensions();
        x=options_keys_width-w-15;
    }
    else if(special_info>=SPECIAL_INFO_KEY_3_LEFT && special_info<=SPECIAL_INFO_KEY_3_CHANGE_SHOT){
        //If this key is currently being set.
        if(player.current_command_to_set()==command){
            //If no input already bound message needs to be relayed.
            if(player.return_input_already_bound()==-1){
                ss.clear();ss.str("");ss<<"Press any key";text=ss.str();
            }
            //If we need to relay an input already bound message.
            else{
                text=player.command_bound_input(player.return_input_already_bound(),player.input_already_bound_player);
                text+=" is already bound.";
            }
        }
        //If this key is not being set.
        else{
            text=player.command_bound_input(command,2);
        }

        set_dimensions();
        x=options_keys_width-w-15;
    }
}

void Information::set_special_image(){
    info_image=0;

    bool achievement=false;

    switch(special_info){

    case SPECIAL_INFO_CHECKBOX_FULLSCREEN:
        if(player.option_fullscreen){
            info_image=&image.checkbox_checked;
        }
        else{
            info_image=&image.checkbox_unchecked;
        }
        break;

    case SPECIAL_INFO_CHECKBOX_FPS:
        if(player.option_fps){
            info_image=&image.checkbox_checked;
        }
        else{
            info_image=&image.checkbox_unchecked;
        }
        break;

    case SPECIAL_INFO_CHECKBOX_HOLIDAY_CHEER:
        if(player.option_holiday_cheer){
            info_image=&image.checkbox_checked;
        }
        else{
            info_image=&image.checkbox_unchecked;
        }
        break;

    case SPECIAL_INFO_CHECKBOX_TIME_OF_DAY:
        if(player.option_time_of_day){
            info_image=&image.checkbox_checked;
        }
        else{
            info_image=&image.checkbox_unchecked;
        }
        break;

    case SPECIAL_INFO_CHECKBOX_TIME_OF_DAY_LEVELS:
        if(player.option_time_of_day_levels){
            info_image=&image.checkbox_checked;
        }
        else{
            info_image=&image.checkbox_unchecked;
        }
        break;

    case SPECIAL_INFO_CHECKBOX_HINTS:
        if(player.option_hints){
            info_image=&image.checkbox_checked;
        }
        else{
            info_image=&image.checkbox_unchecked;
        }
        break;

    case SPECIAL_INFO_CHECKBOX_HARDWARE_CURSOR:
        if(player.option_hardware_cursor){
            info_image=&image.checkbox_checked;
        }
        else{
            info_image=&image.checkbox_unchecked;
        }
        break;

    case SPECIAL_INFO_CHECKBOX_MUTE_GLOBAL:
        if(player.option_mute_global){
            info_image=&image.checkbox_checked;
        }
        else{
            info_image=&image.checkbox_unchecked;
        }
        break;

    case SPECIAL_INFO_CHECKBOX_MUTE_SOUND:
        if(player.option_mute_sound){
            info_image=&image.checkbox_checked;
        }
        else{
            info_image=&image.checkbox_unchecked;
        }
        break;

    case SPECIAL_INFO_CHECKBOX_MUTE_MUSIC:
        if(player.option_mute_music){
            info_image=&image.checkbox_checked;
        }
        else{
            info_image=&image.checkbox_unchecked;
        }
        break;

    case SPECIAL_INFO_MP_PLAYER_0:
        if(player.mp_character==CHARACTER_HUBERT){
            info_image=&image.character_box_hubert;
        }
        else if(player.mp_character==CHARACTER_SLIME_O){
            info_image=&image.character_box_slime_o;
        }
        else if(player.mp_character==CHARACTER_SKETCH){
            info_image=&image.character_box_sketch;
        }
        else if(player.mp_character==CHARACTER_PENNY){
            info_image=&image.character_box_penny;
        }
        break;

    case SPECIAL_INFO_MP_PLAYER_1:
        if(mp_players.size()>=1){
            if(mp_players[0].option_character==CHARACTER_HUBERT){
                info_image=&image.character_box_hubert;
            }
            else if(mp_players[0].option_character==CHARACTER_SLIME_O){
                info_image=&image.character_box_slime_o;
            }
            else if(mp_players[0].option_character==CHARACTER_SKETCH){
                info_image=&image.character_box_sketch;
            }
            else if(mp_players[0].option_character==CHARACTER_PENNY){
                info_image=&image.character_box_penny;
            }
        }
        break;

    case SPECIAL_INFO_MP_PLAYER_2:
        if(mp_players.size()>=2){
            if(mp_players[1].option_character==CHARACTER_HUBERT){
                info_image=&image.character_box_hubert;
            }
            else if(mp_players[1].option_character==CHARACTER_SLIME_O){
                info_image=&image.character_box_slime_o;
            }
            else if(mp_players[1].option_character==CHARACTER_SKETCH){
                info_image=&image.character_box_sketch;
            }
            else if(mp_players[1].option_character==CHARACTER_PENNY){
                info_image=&image.character_box_penny;
            }
        }
        break;

    case SPECIAL_INFO_MP_PLAYER_3:
        if(mp_players.size()>=3){
            if(mp_players[2].option_character==CHARACTER_HUBERT){
                info_image=&image.character_box_hubert;
            }
            else if(mp_players[2].option_character==CHARACTER_SLIME_O){
                info_image=&image.character_box_slime_o;
            }
            else if(mp_players[2].option_character==CHARACTER_SKETCH){
                info_image=&image.character_box_sketch;
            }
            else if(mp_players[2].option_character==CHARACTER_PENNY){
                info_image=&image.character_box_penny;
            }
        }
        break;

    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_COLLECT_ALL_LEAVES_IN_LEVEL:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_COLLECT_ALL_CHEESE_IN_LEVEL:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_PLAY_LEVEL_30_MINUTES:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_PLAY_LEVEL_1_HOUR:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_PLAY_LEVEL_2_HOURS:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_COLLECT_HALF_OF_LEAVES:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_COLLECT_HALF_OF_CHEESE:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_COLLECT_ALL_LEAVES:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_COLLECT_ALL_CHEESE:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_KILL_ENEMY_1:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_KILL_ENEMY_25:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_KILL_ENEMY_50:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_KILL_ENEMY_100:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_KILL_ENEMY_250:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_KILL_ENEMY_500:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_KILL_ENEMY_750:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_KILL_ENEMY_1000:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_ACTIVATE_CHECKPOINT_1:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_ACTIVATE_CHECKPOINT_5:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_ACTIVATE_CHECKPOINT_10:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_ACTIVATE_CHECKPOINT_25:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_ACTIVATE_CHECKPOINT_50:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_ACTIVATE_CHECKPOINT_75:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_ACTIVATE_CHECKPOINT_100:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_COLLECT_AMMO_1:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_COLLECT_AMMO_10:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_COLLECT_AMMO_25:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_COLLECT_AMMO_50:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_COLLECT_AMMO_100:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_COLLECT_AMMO_250:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_COLLECT_AMMO_500:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_SHOOT_1:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_SHOOT_25:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_SHOOT_50:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_SHOOT_100:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_SHOOT_250:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_SHOOT_500:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_SHOOT_750:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_SHOOT_1000:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_DIE_1:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_DIE_5:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_DIE_10:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_DIE_25:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_DIE_50:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_DIE_75:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_DIE_100:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_DIE_TO_ENEMY_1:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_DIE_TO_ENEMY_5:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_DIE_TO_ENEMY_10:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_DIE_TO_ENEMY_25:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_DIE_TO_TRAP_1:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_DIE_TO_TRAP_5:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_DIE_TO_TRAP_10:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_DIE_TO_TRAP_25:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_REPLAY_LEVEL_1:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_REPLAY_LEVEL_5:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_REPLAY_LEVEL_10:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_REPLAY_LEVEL_25:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_REPLAY_LEVEL_50:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_REPLAY_LEVEL_75:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_REPLAY_LEVEL_100:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_JUMP_1:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_JUMP_25:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_JUMP_50:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_JUMP_100:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_JUMP_250:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_JUMP_500:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_JUMP_750:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_JUMP_1000:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_PLAY_1_HOUR:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_PLAY_2_HOURS:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_PLAY_5_HOURS:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_PLAY_10_HOURS:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_TRAVEL_1_HOUR:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_MENUS_1_HOUR:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_PULL_LEVER_1:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_PULL_LEVER_5:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_PULL_LEVER_10:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_PULL_LEVER_25:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_PULL_LEVER_50:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_PULL_LEVER_75:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_PULL_LEVER_100:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_RIDE_COW_1_SECOND:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_RIDE_COW_1_MINUTE:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_RIDE_COW_2_MINUTES:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_RIDE_COW_3_MINUTES:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_RIDE_COW_4_MINUTES:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_RIDE_COW_5_MINUTES:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_WATER_SHOT_1:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_WATER_SHOT_5:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_WATER_SHOT_10:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_WATER_SHOT_25:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_HUBERTS_HOUSE_1_HOUR:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_ENTER_WORLDMAP:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_ENTER_ANY_LEVEL:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_DEFEAT_ALL_BOSSES_BUT_FINAL:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_DEFEAT_ALL_BOSSES:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_BEAT_ANY_LEVEL:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_BEAT_GAME:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_BEAT_LEVEL_NO_KILLS:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_BEAT_LEVEL_NO_SHOTS:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_JOKE_ITEM:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_FIRE_50_SHOTS_IN_LEVEL:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_MOVE_INVENTORY_ITEM_5:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_CHANGE_AN_OPTION:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_VIEW_CREDITS:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_DIE_AFTER_GOAL:
    case SPECIAL_INFO_CHECKBOX_ACHIEVEMENT+ACHIEVEMENT_MAX_DEATH_BOUNCES:
        achievement=true;
        break;

    }

    //If the special info is an achievement.
    if(achievement){
        if(player.achievements[special_info-SPECIAL_INFO_CHECKBOX_ACHIEVEMENT]){
            info_image=&image.checkbox_checked;
        }
        else{
            info_image=&image.checkbox_unchecked;
        }
    }
}

void Information::handle_input(int mouse_x,int mouse_y,short x_offset,short y_offset){
    if(tooltip_text.length()>0 && collision_check(mouse_x,mouse_y,2,2,x_offset+x,y_offset+y,w,h)){
        //Setup the information's tooltip.
        tooltip.setup(tooltip_text,mouse_x,mouse_y);
    }
}

void Information::render(short x_offset,short y_offset){
    //If there is special info, it needs to be processed before being displayed.
    if(special_info!=NO_SPECIAL_INFO){
        //Setup the special string, if any.
        set_special_string();
        //Setup the special image, if any.
        set_special_image();
        set_dimensions();
    }

    //If the information has text.
    if(text.length()>0){
        //Display the information's text.
        BitmapFont* ptr_font=NULL;
        if(font_type==0){
            ptr_font=&font;
        }
        else if(font_type==1){
            ptr_font=&font_sga;
        }
        ptr_font->show(x_offset+x+2,y_offset+y+6+2,text,COLOR_BLACK,1.0);
        ptr_font->show(x_offset+x,y_offset+y+6,text,text_color,1.0);
    }
    //If the information has an image.
    else if(info_image!=0){
        if(info_sprite_sheet!=0){
            render_sprite(x_offset+x,y_offset+y,*info_image,info_sprite_sheet);
        }
        else{
            render_texture(x_offset+x,y_offset+y,*info_image);
        }
    }
    //If the information has special non-text and non-image info.
    else{
        double volume_percentage=0.0;
        double volume_bar_width=0.0;

        switch(special_info){

        case SPECIAL_INFO_VOLUME_GLOBAL:
            volume_percentage=(double)player.option_volume_global/1.0;
            volume_bar_width=200.0*volume_percentage;

            //Render the background.
            render_rectangle(x_offset+x-5,y_offset+y-5,210,20,1.0,return_gui_color(holiday,2));

            //Render the bar.
            render_rectangle(x_offset+x+2,y_offset+y+2,volume_bar_width,10,1.0,COLOR_BLACK);
            render_rectangle(x_offset+x,y_offset+y,volume_bar_width,10,1.0,return_gui_color(holiday,3));
            break;

        case SPECIAL_INFO_VOLUME_SOUND:
            volume_percentage=(double)player.option_volume_sound/0.5;
            volume_bar_width=200.0*volume_percentage;

            //Render the background.
            render_rectangle(x_offset+x-5,y_offset+y-5,210,20,1.0,return_gui_color(holiday,2));

            //Render the bar.
            render_rectangle(x_offset+x+2,y_offset+y+2,volume_bar_width,10,1.0,COLOR_BLACK);
            render_rectangle(x_offset+x,y_offset+y,volume_bar_width,10,1.0,return_gui_color(holiday,3));
            break;

        case SPECIAL_INFO_VOLUME_MUSIC:
            volume_percentage=(double)player.option_volume_music/0.15;
            volume_bar_width=200.0*volume_percentage;

            //Render the background.
            render_rectangle(x_offset+x-5,y_offset+y-5,210,20,1.0,return_gui_color(holiday,2));

            //Render the bar.
            render_rectangle(x_offset+x+2,y_offset+y+2,volume_bar_width,10,1.0,COLOR_BLACK);
            render_rectangle(x_offset+x,y_offset+y,volume_bar_width,10,1.0,return_gui_color(holiday,3));
            break;

        }
    }
}
