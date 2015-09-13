/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef sound_data_h
#define sound_data_h

#include <string>

#include <SDL_mixer.h>

class sound_data{
    private:
    public:
    Mix_Chunk* chunk;

    sound_data();

    void load_sound(std::string filename);

    void unload_sound();
};

#endif
