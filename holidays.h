/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef holidays_h
#define holidays_h

#include <string>

void play_game_start_sound();

std::string return_holiday_special_string(short holiday_ident);

short return_holiday_color(short holiday_ident);

std::string return_holiday_name(short holiday_ident);

void determine_holiday();

short return_gui_color(short holiday_ident,short color_number);

#endif
