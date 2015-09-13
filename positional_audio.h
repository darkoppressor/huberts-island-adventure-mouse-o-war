/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef positional_audio_h
#define positional_audio_h

#include "sound_data.h"

#include <SDL_mixer.h>

//Modifies volume by applying the appropriate audio option multipliers.
int modify_sound_volume(int volume);

//Modifies volume by applying the appropriate audio option multipliers.
int modify_music_volume(int volume);

int get_free_channel();

//If coordinates are not given, the sound is played at maximum volume.
//falloff determines the rate at which a positional sound falls off.
//The higher falloff is, the more slowly sounds will fade out as the player puts distance between himself and the sound source.
//So a low falloff means the sound will quickly falloff.
void play_positional_sound(sound_data sound,double sound_x=-1.0,double sound_y=-1.0,double falloff=48.0);

#endif
