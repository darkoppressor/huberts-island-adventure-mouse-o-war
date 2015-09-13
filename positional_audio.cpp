/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "positional_audio.h"
#include "world.h"

using namespace std;

int modify_sound_volume(int volume){
    //If either the global mute or the sound effects mute is enabled.
    if(player.option_mute_global || player.option_mute_sound){
        return 0;
    }

    double new_volume=volume;

    new_volume*=player.option_volume_sound;

    new_volume*=player.option_volume_global;

    return (int)new_volume;
}

int modify_music_volume(int volume){
    //If either the global mute or the music mute is enabled.
    if(player.option_mute_global || player.option_mute_music){
        return 0;
    }

    double new_volume=volume;

    new_volume*=player.option_volume_music;

    new_volume*=player.option_volume_global;

    return (int)new_volume;
}

int get_free_channel(){
    int allocated_channels=Mix_AllocateChannels(-1);

    for(int i=0;i<allocated_channels;i++){
        //Check through all music tracks currently loaded.
        bool channel_taken_by_music_track=false;
        for(int n=0;n<music.tracks.size();n++){
            //If this channel is equal to this track's channel, then this channel is taken.
            if(i==music.tracks[n].channel){
                channel_taken_by_music_track=true;
            }
        }

        //If the channel is already playing.
        if(Mix_Playing(i)==1){
        }
        //If the player's reserved death sound channel is setup and this channel is equal to it.
        else if(player.sound_channel_death!=-1 && i==player.sound_channel_death){
        }
        //If the player's reserved balloonin sound channel is setup and this channel is equal to it.
        else if(player.sound_channel_balloonin!=-1 && i==player.sound_channel_balloonin){
        }
        //If this channel is already reserved by a music track.
        else if(channel_taken_by_music_track){
        }
        //All tests were passed, and this channel is currently free, so return it.
        else{
            return i;
        }
    }

    return -1;
}

void play_positional_sound(sound_data sound,double sound_x,double sound_y,double falloff){
    if(sound.chunk!=NULL){
        int volume=128;

        if(sound_x!=-1){
            double distance_x=fabs(player.cam_focused_x()-sound_x);
            double distance_y=fabs(player.cam_focused_y()-sound_y);

            double average=(distance_x+distance_y)/2;
            if(average==0.0){
                average=0.1;
            }

            volume=(int)((falloff/average)*falloff);

            if(volume>128){
                volume=128;
            }
        }

        volume=modify_sound_volume(volume);

        //As long as volume didn't end up 0.
        if(volume!=0){
            int channel=get_free_channel();

            if(channel!=-1){
                Mix_Volume(channel,volume);
                Mix_PlayChannel(channel,sound.chunk,0);
            }
        }
    }
}
