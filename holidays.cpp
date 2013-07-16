/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "holidays.h"
#include "enumerations.h"
#include "world.h"

using namespace std;

void play_game_start_sound(){
    //Determine the date and time.
    time_t now;
    struct tm *tm_now;
    now=time(NULL);
    tm_now=localtime(&now);

    bool friday_13=false;
    bool full_moon=false;

    //If it is Friday the 13th.
    if(tm_now->tm_mday==13 && tm_now->tm_wday==5){
        friday_13=true;
    }
    //If it is a full moon.
    if(player.return_moon_phase()==MOON_PHASE_FULL){
        full_moon=true;
    }

    player.main_menu_special="";
    player.main_menu_special_color=COLOR_RAINBOW;

    if(holiday!=HOLIDAY_NONE){
        player.main_menu_special+=return_holiday_special_string(holiday)+"\n\n";
        if(player.main_menu_special_color==COLOR_RAINBOW){
            player.main_menu_special_color=return_holiday_color(holiday);
        }
    }
    if(full_moon){
        player.main_menu_special+="You are lucky!\nFull moon tonight.\n\n";
        if(player.main_menu_special_color==COLOR_RAINBOW){
            player.main_menu_special_color=COLOR_WHITE;
        }
    }
    if(friday_13){
        player.main_menu_special+="Watch out!\nBad things\ncan happen on\nFriday the 13th.\n\n";
        if(player.main_menu_special_color==COLOR_RAINBOW){
            player.main_menu_special_color=COLOR_GRAY;
        }
    }

    if(friday_13 && !full_moon){
        play_positional_sound(sound_system.game_start_friday_13);
    }
    else if(full_moon && !friday_13){
        play_positional_sound(sound_system.game_start_full_moon);
    }
}

string return_holiday_special_string(short holiday_ident){
    if(holiday_ident==HOLIDAY_NEW_YEARS_DAY){
        return "Happy New Year!";
    }
    else if(holiday_ident==HOLIDAY_VALENTINES){
        return "Happy\nValentine's Day!";
    }
    else if(holiday_ident==HOLIDAY_ST_PATRICKS){
        return "Are you\nwearing green?";
    }
    else if(holiday_ident==HOLIDAY_EASTER){
        return "Happy Easter!";
    }
    else if(holiday_ident==HOLIDAY_INDEPENDENCE){
        return "Happy\nIndependence Day!";
    }
    else if(holiday_ident==HOLIDAY_HALLOWEEN){
        return "Happy Halloween!";
    }
    else if(holiday_ident==HOLIDAY_THANKSGIVING){
        return "Happy Thanksgiving!";
    }
    else if(holiday_ident==HOLIDAY_CHRISTMAS){
        return "Christmas\nhas been detected.";
    }
    else if(holiday_ident==HOLIDAY_NEW_YEARS_EVE){
        return "A new year\nis approaching.";
    }

    return "Not a holiday!";
}

short return_holiday_color(short holiday_ident){
    if(holiday_ident==HOLIDAY_NEW_YEARS_DAY){
        return COLOR_WHITE;
    }
    else if(holiday_ident==HOLIDAY_VALENTINES){
        return COLOR_PINK;
    }
    else if(holiday_ident==HOLIDAY_ST_PATRICKS){
        return COLOR_GREEN;
    }
    else if(holiday_ident==HOLIDAY_EASTER){
        return COLOR_YELLOW_PASTEL;
    }
    else if(holiday_ident==HOLIDAY_INDEPENDENCE){
        return COLOR_BLUE;
    }
    else if(holiday_ident==HOLIDAY_HALLOWEEN){
        return COLOR_PUMPKIN;
    }
    else if(holiday_ident==HOLIDAY_THANKSGIVING){
        return COLOR_BROWN;
    }
    else if(holiday_ident==HOLIDAY_CHRISTMAS){
        return COLOR_RED;
    }
    else if(holiday_ident==HOLIDAY_NEW_YEARS_EVE){
        return COLOR_CHAMPAGNE;
    }

    return COLOR_WHITE;
}

string return_holiday_name(short holiday_ident){
    if(!player.option_holiday_cheer){
        return "normal";
    }

    if(holiday_ident==HOLIDAY_NONE){
        return "normal";
    }
    else if(holiday_ident==HOLIDAY_NEW_YEARS_DAY){
        return "new_years_day";
    }
    else if(holiday_ident==HOLIDAY_VALENTINES){
        return "valentines";
    }
    else if(holiday_ident==HOLIDAY_ST_PATRICKS){
        return "st_patricks";
    }
    else if(holiday_ident==HOLIDAY_EASTER){
        return "easter";
    }
    else if(holiday_ident==HOLIDAY_INDEPENDENCE){
        return "independence";
    }
    else if(holiday_ident==HOLIDAY_HALLOWEEN){
        return "halloween";
    }
    else if(holiday_ident==HOLIDAY_THANKSGIVING){
        return "thanksgiving";
    }
    else if(holiday_ident==HOLIDAY_CHRISTMAS){
        return "christmas";
    }
    else if(holiday_ident==HOLIDAY_NEW_YEARS_EVE){
        return "new_years_eve";
    }

    return "normal";
}

void determine_holiday(){
    //Determine the date and time.
    time_t now;
    struct tm *tm_now;
    now=time(NULL);
    tm_now=localtime(&now);

    holiday=HOLIDAY_NONE;

    if(tm_now->tm_mon==0 && tm_now->tm_mday==1){
        holiday=HOLIDAY_NEW_YEARS_DAY;
    }
    else if(tm_now->tm_mon==1 && tm_now->tm_mday>=8 && tm_now->tm_mday<=14){
        holiday=HOLIDAY_VALENTINES;
    }
    else if(tm_now->tm_mon==2 && tm_now->tm_mday>=11 && tm_now->tm_mday<=17){
        holiday=HOLIDAY_ST_PATRICKS;
    }
    else if(tm_now->tm_mon==3 && tm_now->tm_mday>=24 && tm_now->tm_mday<=30){
        holiday=HOLIDAY_EASTER;
    }
    else if(tm_now->tm_mon==6 && tm_now->tm_mday>=1 && tm_now->tm_mday<=4){
        holiday=HOLIDAY_INDEPENDENCE;
    }
    else if(tm_now->tm_mon==9 && tm_now->tm_mday>=18 && tm_now->tm_mday<=31){
        holiday=HOLIDAY_HALLOWEEN;
    }
    else if(tm_now->tm_mon==10 && tm_now->tm_mday>=17 && tm_now->tm_mday<=30){
        holiday=HOLIDAY_THANKSGIVING;
    }
    else if(tm_now->tm_mon==11 && tm_now->tm_mday>=1 && tm_now->tm_mday<=30){
        holiday=HOLIDAY_CHRISTMAS;
    }
    else if(tm_now->tm_mon==11 && tm_now->tm_mday==31){
        holiday=HOLIDAY_NEW_YEARS_EVE;
    }
}

short return_gui_color(short holiday_ident,short color_number){
    short colors[7];
    colors[0]=COLOR_SEPIA;
    colors[1]=COLOR_COPPER;
    colors[2]=COLOR_BROWN;
    colors[3]=COLOR_WHEAT;
    colors[4]=COLOR_BRONZE;
    colors[5]=COLOR_SANDY_BROWN;
    colors[6]=COLOR_GRASS_GREEN;

    //If the passed color number is invalid.
    if(color_number>6){
        color_number=0;
    }

    if(player.option_holiday_cheer){
        if(holiday_ident==HOLIDAY_NEW_YEARS_DAY){
        }
        else if(holiday_ident==HOLIDAY_VALENTINES){
        }
        else if(holiday_ident==HOLIDAY_ST_PATRICKS){
        }
        else if(holiday_ident==HOLIDAY_EASTER){
        }
        else if(holiday_ident==HOLIDAY_INDEPENDENCE){
            colors[0]=COLOR_DARK_RED;
            colors[1]=COLOR_BLUE_OCEAN;
            colors[2]=COLOR_RED;
            colors[3]=COLOR_WHITE;
            colors[4]=COLOR_BLUE;
            colors[5]=COLOR_DARK_RED;
            colors[6]=COLOR_BLUE_OCEAN;
        }
        else if(holiday_ident==HOLIDAY_HALLOWEEN){
        }
        else if(holiday_ident==HOLIDAY_THANKSGIVING){
        }
        else if(holiday_ident==HOLIDAY_CHRISTMAS){
            colors[0]=COLOR_DARK_RED;
            colors[1]=COLOR_DARK_GREEN;
            colors[2]=COLOR_RED;
            colors[3]=COLOR_WHITE;
            colors[4]=COLOR_GREEN;
            colors[5]=COLOR_DARK_RED;
            colors[6]=COLOR_DARK_GREEN;
        }
        else if(holiday_ident==HOLIDAY_NEW_YEARS_EVE){
        }
    }

    return colors[color_number];
}
