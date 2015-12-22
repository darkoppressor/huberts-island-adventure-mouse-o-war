/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef update_h
#define update_h

#include <SDL.h>

void input();

bool poll_event(SDL_Event* event_storage);
bool poll_event_touch(SDL_Event* event_storage);
void finger_down(const SDL_Event& event_storage);

void movement();

void events();

void animation();

void camera(int frame_rate,double ms_per_frame,int logic_frame_rate);

void render(int frame_rate,double ms_per_frame,int logic_frame_rate);

//If level_loading is -1, we are loading the game. Otherwise, we are loading the passed level.
void render_loading_screen(double percentage,int level_loading=-1);

#endif
