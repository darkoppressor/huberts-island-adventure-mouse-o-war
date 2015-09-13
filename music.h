/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef music_h
#define music_h

#include <vector>
#include <string>

#include <SDL_mixer.h>

class Music{
private:

    struct music_data{
        Mix_Chunk* track;
        bool playing;
        int channel;
        double volume;
        double fade_speed;
    };

    std::vector<music_data> tracks;

public:

    friend int get_free_channel();

    void set_track_volumes();

    //Handle the fading in and out of tracks.
    void fadein_tracks();
    void fadeout_tracks();

    //Turn off the currently playing track, if any, and start playing track.
    void play_track(int track_ident,double fade_speed=0.01);

    //Turn off the currently playing track, if any.
    void stop_track(double fade_speed=0.01);

    void restart_track(int i);

    //Prepare all tracks.
    //Does not load anything.
    void prepare_tracks();

    //Load a single track.
    void load_track(std::string track_path,int track_ident);

    //Unload a single track.
    void unload_track(int track_ident);

    //Unload all tracks.
    void unload_tracks();

    //Returns true if the passed track is currently the playing track.
    //Returns false if the passed track is not playing.
    bool track_is_playing(int track_ident);

    //Reloads the menu track.
    void reinitialize_menu_track();
};

#endif
