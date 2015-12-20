/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef main_h
#define main_h

#include <SDL.h>

//The main game loop.
void game_loop();

//Handle Android/iOS events
int handle_app_events(void* userdata,SDL_Event* event);

int main(int argc, char* args[]);

#endif
