/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef game_window_h
#define game_window_h

#include "coords.h"
#include "color.h"

#include <SDL.h>
#include <SDL_mixer.h>

#include <stdint.h>

const int DEFAULT_SCREEN_WIDTH=800;
const int DEFAULT_SCREEN_HEIGHT=600;

class Game_Window{
    private:
    public:

    Game_Window();

    bool initialize_video();

    void cleanup_video();

    Engine_Rect get_display_resolution();
    //Returns the resolution dimensions' maximum values for a display
    //If display_number>=0, looks at display number display_number
    //If display_number<0, looks at the display the window is currently on
    Engine_Rect get_display_resolution_max(int display_number=-1);

    bool set_resolution(int* desired_resolution_x,int* desired_resolution_y);

    //Returns true if the position was adjusted based on the display_number option
    //Returns false if the position was not adjusted
    bool set_position(int* desired_resolution_x,int* desired_resolution_y,int* position_x,int* position_y);

    void reload();

    void get_mouse_state(int* mouse_x,int* mouse_y);

    bool pre_initialize();

    bool init();

    void deinitialize();

    bool is_initialized();

    void get_renderer_logical_size(int* width,int* height);
    void get_renderer_viewport(SDL_Rect* rect);
    void get_renderer_scale(float* x,float* y);
    void get_renderer_output_size(int* width,int* height);
    void get_renderer_info(SDL_RendererInfo* info);

    void get_rgba_masks(uint32_t* rmask,uint32_t* gmask,uint32_t* bmask,uint32_t* amask);

    void screenshot();

    void update_display_number();

    void set_sdl_hints();

    SDL_Texture* create_texture_from_surface(SDL_Surface* surface);
    SDL_Texture* create_texture(uint32_t format,int access,int width,int height);

    int set_render_target(SDL_Texture* texture);
    void set_render_draw_blend_mode(SDL_BlendMode blend_mode);
    void set_render_draw_color(const Color& color,double opacity);

    void render_copy_ex(SDL_Texture* texture,const SDL_Rect* srcrect,const SDL_Rect* dstrect,const double angle,const SDL_Point* center,const SDL_RendererFlip flip);
    void render_fill_rect(SDL_Rect* rect);
    void render_draw_line(int x1,int y1,int x2,int y2);

    void clear_renderer(const Color& color);
    void render_present();

    bool pre_initialized;
    bool initialized;

    //The logical resolution of the game window
    int SCREEN_WIDTH;
    int SCREEN_HEIGHT;

    SDL_Window* screen;

    SDL_Renderer* renderer;

    SDL_Surface* icon;
    Uint32 icon_colorkey;

    SDL_Cursor* boring_cursor;
};

#endif
