/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef game_window_h
#define game_window_h

#include <SDL.h>
#include <SDL_mixer.h>

#define GLEW_STATIC
#include <glew.h>

const short DEFAULT_SCREEN_WIDTH=800;
const short DEFAULT_SCREEN_HEIGHT=600;

class Game_Window{
    private:
    public:

    Game_Window();

    bool cleanup_opengl();

    bool initialize_opengl();

    bool init();

    void screenshot();

    void reinitialize();

    short SCREEN_WIDTH;

    short SCREEN_HEIGHT;

    short SCREEN_BPP;

    short REAL_SCREEN_WIDTH;
    short REAL_SCREEN_HEIGHT;

    SDL_Surface* screen;

    SDL_Surface* icon;
    Uint32 icon_colorkey;

    GLuint fbo;
    GLuint fbo_texture;

    //If true, we are using fbos.
    //If false, we are not.
    bool fbo_mode;

    SDL_Cursor* boring_cursor;
};

#endif
