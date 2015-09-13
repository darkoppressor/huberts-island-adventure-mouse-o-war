/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "music.h"
#include "world.h"
#include "holidays.h"

using namespace std;

void Music::set_track_volumes(){
    for(int i=0;i<tracks.size();i++){
        //Set the track's channel volume.
        Mix_Volume(tracks[i].channel,modify_music_volume((int)(tracks[i].volume*128)));
    }
}

void Music::fadein_tracks(){
    for(int i=0;i<tracks.size();i++){
        //If the track is playing and the track's volume is not already maxed out, fade it in.
        if(tracks[i].playing && tracks[i].volume<1.0){
            tracks[i].volume+=tracks[i].fade_speed;
            if(tracks[i].volume>1.0){
                tracks[i].volume=1.0;
            }

            //Set the track's channel volume.
            Mix_Volume(tracks[i].channel,modify_music_volume((int)(tracks[i].volume*128)));
        }
    }
}

void Music::fadeout_tracks(){
    for(int i=0;i<tracks.size();i++){
        //If the track is not playing and the track's volume is not already off, fade it out.
        if(!tracks[i].playing && tracks[i].volume>0.0){
            tracks[i].volume-=tracks[i].fade_speed;
            if(tracks[i].volume<0.0){
                tracks[i].volume=0.0;
            }

            //Set the track's channel volume.
            Mix_Volume(tracks[i].channel,modify_music_volume((int)(tracks[i].volume*128)));
        }
    }
}

void Music::play_track(int track_ident,double fade_speed){
    //Set all tracks to fade out.
    for(int i=0;i<tracks.size();i++){
        //If this track is currently playing.
        if(tracks[i].track!=NULL && tracks[i].playing){
            tracks[i].playing=false;
            if(fade_speed>0.0){
                tracks[i].fade_speed=fade_speed;
            }
        }
    }

    if(tracks[track_ident].track!=NULL){
        //Set the new track to fade in.
        tracks[track_ident].playing=true;

        if(fade_speed>0.0){
            tracks[track_ident].fade_speed=fade_speed;
        }
    }
}

void Music::stop_track(double fade_speed){
    //Set all tracks to fade out.
    for(int i=0;i<tracks.size();i++){
        //If this track is currently playing.
        if(tracks[i].track!=NULL && tracks[i].playing){
            tracks[i].playing=false;
            if(fade_speed>0.0){
                tracks[i].fade_speed=fade_speed;
            }
        }
    }
}

void Music::restart_track(int i){
    if(tracks[i].track!=NULL){
        //Halt the track's channel.
        Mix_HaltChannel(tracks[i].channel);

        //Set the track's channel volume.
        Mix_Volume(tracks[i].channel,modify_music_volume((int)(tracks[i].volume*128)));

        //Start "playing" the music track.
        Mix_PlayChannel(tracks[i].channel,tracks[i].track,-1);
    }
}

void Music::prepare_tracks(){
    for(int i=MUSIC_BEGIN;i<MUSIC_END;i++){
        tracks.push_back(music_data());
        tracks[tracks.size()-1].track=NULL;
        tracks[tracks.size()-1].playing=false;
        tracks[tracks.size()-1].channel=get_free_channel();
        tracks[tracks.size()-1].volume=0.0;
        tracks[tracks.size()-1].fade_speed=0.01;
    }
}

void Music::load_track(string track_path,int track_ident){
    unload_track(track_ident);

    //Load the new song.
    tracks[track_ident].track=Mix_LoadWAV(track_path.c_str());
    tracks[track_ident].playing=false;
    tracks[track_ident].volume=0.0;
    tracks[track_ident].fade_speed=0.01;

    restart_track(track_ident);
}

void Music::unload_track(int track_ident){
    //If this track has a song loaded.
    if(tracks[track_ident].track!=NULL){
        //Unload it.
        Mix_HaltChannel(tracks[track_ident].channel);
        Mix_FreeChunk(tracks[track_ident].track);
        tracks[track_ident].track=NULL;
        tracks[track_ident].playing=false;
    }
}

void Music::unload_tracks(){
    //Free all songs from memory.
    for(int i=0;i<tracks.size();i++){
        //If this track has a song loaded.
        if(tracks[i].track!=NULL){
            Mix_HaltChannel(tracks[i].channel);
            Mix_FreeChunk(tracks[i].track);
            tracks[i].track=NULL;
            tracks[i].playing=false;
        }
    }

    tracks.clear();
}

bool Music::track_is_playing(int track_ident){
    if(tracks[track_ident].playing){
        return true;
    }
    else{
        return false;
    }
}

void Music::reinitialize_menu_track(){
    unload_track(MUSIC_MENU);

    string holiday_name=return_holiday_name(holiday);
    msg="data/music/menu/";
    msg+=holiday_name;
    msg+=".ogg";

    load_track(msg,MUSIC_MENU);
}
