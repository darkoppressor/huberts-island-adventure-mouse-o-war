/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef message_log_h
#define message_log_h

#include <string>

void update_error_log(std::string message);

void update_survival_log(std::string message,bool multiplayer);

#endif
