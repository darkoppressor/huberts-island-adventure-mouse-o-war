/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef special_info_strings_h
#define special_info_strings_h

#include <string>

struct special_info_return_data{
    std::string text;
    short command;
};

special_info_return_data set_special_info_string(short special_info);

#endif
