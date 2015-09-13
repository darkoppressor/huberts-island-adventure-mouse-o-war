/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef timer_h
#define timer_h

#include <SDL.h>

class Timer{
    private:
    //The clock time when the timer started.
    Uint32 start_ticks;

    //The ticks stored when the timer was paused.
    Uint32 paused_ticks;

    //Timer status.
    bool paused;
    bool started;

    public:
    //Initializes variables.
    Timer();

    //The various clock actions.
    void start();
    void stop();
    void pause();
    void unpause();

    //Get the timer's time.
    Uint32 get_ticks();

    //Checks the status of the timer.
    bool is_started();
    bool is_paused();
};

#endif
