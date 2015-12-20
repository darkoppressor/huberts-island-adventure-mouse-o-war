/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "game_window.h"
#include "world.h"
#include "quit.h"
#include "pixels.h"
#include "message_log.h"
#include "update.h"
#include "holidays.h"
#include "render.h"
#include "options.h"

#include <time.h>
#include <string>

using namespace std;

Game_Window::Game_Window(){
    pre_initialized=false;
    initialized=false;

    SCREEN_WIDTH=DEFAULT_SCREEN_WIDTH;
    SCREEN_HEIGHT=DEFAULT_SCREEN_HEIGHT;

    screen=0;

    renderer=0;

    icon=NULL;
    icon_colorkey=0;

    boring_cursor=NULL;
}

bool Game_Window::initialize_video(){
    int position_x=SDL_WINDOWPOS_CENTERED;
    int position_y=SDL_WINDOWPOS_CENTERED;
    int desired_resolution_x=0;
    int desired_resolution_y=0;

    if(!set_resolution(&desired_resolution_x,&desired_resolution_y)){
        return false;
    }

    set_position(&desired_resolution_x,&desired_resolution_y,&position_x,&position_y);

    //Set up the screen:
    uint32_t flags=0;

    /**if(Options::bind_cursor){
        flags=SDL_WINDOW_INPUT_GRABBED;
    }*/

    if(player.option_fullscreen){
        if(player.option_fullscreen_mode=="desktop"){
            screen=SDL_CreateWindow("Hubert's Island Adventure: Mouse o' War",position_x,position_y,
                                    0,0,flags|SDL_WINDOW_FULLSCREEN_DESKTOP);
        }
        else if(player.option_fullscreen_mode=="standard"){
            screen=SDL_CreateWindow("Hubert's Island Adventure: Mouse o' War",position_x,position_y,
                                    desired_resolution_x,desired_resolution_y,flags|SDL_WINDOW_FULLSCREEN);
        }
        else if(player.option_fullscreen_mode=="windowed"){
            screen=SDL_CreateWindow("Hubert's Island Adventure: Mouse o' War",position_x,position_y,
                                    desired_resolution_x,desired_resolution_y,flags|SDL_WINDOW_BORDERLESS);
        }
        else{
            update_error_log("Invalid value for fullscreen_mode: '"+player.option_fullscreen_mode+"'");
            return false;
        }
    }
    else{
        screen=SDL_CreateWindow("Hubert's Island Adventure: Mouse o' War",position_x,position_y,
                                desired_resolution_x,desired_resolution_y,flags);
    }

    //If the screen could not be set up.
    if(screen==0){
        msg="Unable to create window: ";
        msg+=SDL_GetError();
        update_error_log(msg);
        return false;
    }

    if(player.option_fullscreen){
        int window_width=0;
        int window_height=0;
        SDL_GetWindowSize(screen,&window_width,&window_height);

        Engine_Rect display_res=get_display_resolution();
        Engine_Rect display_res_max=get_display_resolution_max();

        if(display_res.w>-1 && display_res.h>-1){
            if(player.option_fullscreen_mode=="standard"){
                if(display_res_max.w>-1 && display_res_max.h>-1){
                    if(window_width>display_res_max.w || window_height>display_res_max.h){
                        update_error_log("Window dimensions of "+num_to_string(window_width)+"x"+num_to_string(window_height)+" exceed the maximum display dimensions of "+
                                       num_to_string(display_res_max.w)+"x"+num_to_string(display_res_max.h)+", adjusting window resolution to current display resolution");

                        if(SDL_SetWindowFullscreen(screen,0)!=0){
                            msg="Error toggling fullscreen: ";
                            msg+=SDL_GetError();
                            update_error_log(msg);
                            return false;
                        }

                        SDL_SetWindowSize(screen,display_res.w,display_res.h);

                        if(SDL_SetWindowFullscreen(screen,SDL_WINDOW_FULLSCREEN)!=0){
                            msg="Error toggling fullscreen: ";
                            msg+=SDL_GetError();
                            update_error_log(msg);
                            return false;
                        }
                    }
                }
            }
            else if(player.option_fullscreen_mode=="windowed"){
                if(window_width!=display_res.w || window_height!=display_res.h){
                    SDL_SetWindowSize(screen,display_res.w,display_res.h);
                    SDL_SetWindowPosition(screen,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED);
                }
            }
        }
    }

    update_display_number();

    renderer=SDL_CreateRenderer(screen,-1,SDL_RENDERER_TARGETTEXTURE);
    if(renderer==0){
        msg="Unable to create renderer: ";
        msg+=SDL_GetError();
        update_error_log(msg);
        return false;
    }

    SDL_RenderSetLogicalSize(renderer,SCREEN_WIDTH,SCREEN_HEIGHT);

    if(!SDL_RenderTargetSupported(renderer)){
        update_error_log("Render targets unsupported by renderer.");
        return false;
    }

    //Create a temporary texture to check for the rendering features we need.
    SDL_Texture* texture=SDL_CreateTexture(renderer,SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_STATIC,10,10);
    if(texture==0){
        msg="Unable to check renderer features: ";
        msg+=SDL_GetError();
        update_error_log(msg);
        return false;
    }

    int support_alpha_blending=SDL_SetTextureBlendMode(texture,SDL_BLENDMODE_BLEND);
    if(support_alpha_blending!=0){
        msg="Alpha blending unsupported by renderer: ";
        msg+=SDL_GetError();
        update_error_log(msg);
        return false;
    }

    int support_alpha_mod=SDL_SetTextureAlphaMod(texture,128);
    if(support_alpha_mod!=0){
        msg="Alpha modding unsupported by renderer: ";
        msg+=SDL_GetError();
        update_error_log(msg);
        return false;
    }

    int support_color_mod=SDL_SetTextureColorMod(texture,128,128,128);
    if(support_color_mod!=0){
        msg="Color modding unsupported by renderer: ";
        msg+=SDL_GetError();
        update_error_log(msg);
        return false;
    }

    SDL_DestroyTexture(texture);

    return true;
}

void Game_Window::cleanup_video(){
    if(renderer!=0){
        SDL_DestroyRenderer(renderer);
        renderer=0;
    }

    if(screen!=0){
        SDL_DestroyWindow(screen);
        screen=0;
    }
}

Engine_Rect Game_Window::get_display_resolution(){
    Engine_Rect resolution;

    if(screen!=0){
        int display_index=SDL_GetWindowDisplayIndex(screen);

        if(display_index>=0){
            SDL_DisplayMode mode;

            if(SDL_GetDesktopDisplayMode(display_index,&mode)==0){
                resolution.w=mode.w;
                resolution.h=mode.h;
            }
            else{
                msg="Unable to get display mode for display #"+num_to_string(display_index)+": ";
                msg+=SDL_GetError();
                update_error_log(msg);
            }
        }
        else{
            msg="Unable to get display index for window: ";
            msg+=SDL_GetError();
            update_error_log(msg);
        }
    }
    else{
        update_error_log("Attempted to get display resolution for uninitialized window");
    }

    return resolution;
}

Engine_Rect Game_Window::get_display_resolution_max(int display_number){
    Engine_Rect resolution;

    if(screen!=0 || display_number>=0){
        int display_index=display_number;

        if(display_index<0){
            SDL_GetWindowDisplayIndex(screen);
        }

        if(display_index>=0){
            int display_modes=SDL_GetNumDisplayModes(display_index);

            if(display_modes>=1){
                for(int i=0;i<display_modes;i++){
                    SDL_DisplayMode mode;

                    if(SDL_GetDisplayMode(display_index,i,&mode)==0){
                        if(mode.w>resolution.w){
                            resolution.w=mode.w;
                        }

                        if(mode.h>resolution.h){
                            resolution.h=mode.h;
                        }
                    }
                    else{
                        msg="Unable to get display mode #"+num_to_string(i)+" for display #"+num_to_string(display_index)+": ";
                        msg+=SDL_GetError();
                        update_error_log(msg);
                    }
                }
            }
            else{
                msg="Unable to get display modes for display #"+num_to_string(display_index)+": ";
                msg+=SDL_GetError();
                update_error_log(msg);
            }
        }
        else{
            msg="Unable to get display index for window: ";
            msg+=SDL_GetError();
            update_error_log(msg);
        }
    }
    else{
        update_error_log("Attempted to get maximum display resolution for uninitialized window");
    }

    return resolution;
}

bool Game_Window::set_resolution(int* desired_resolution_x,int* desired_resolution_y){
    SCREEN_WIDTH=DEFAULT_SCREEN_WIDTH;
    SCREEN_HEIGHT=DEFAULT_SCREEN_HEIGHT;

    *desired_resolution_x=player.option_screen_width;
    *desired_resolution_y=player.option_screen_height;

    return true;
}

bool Game_Window::set_position(int* desired_resolution_x,int* desired_resolution_y,int* position_x,int* position_y){
    if(player.option_display_number!=-1){
        int displays=SDL_GetNumVideoDisplays();

        if(displays>=1){
            if(player.option_display_number>=displays){
                player.option_display_number=0;

                global_options_save();
            }

            SDL_Rect display_bounds={0,0,0,0};

            if(SDL_GetDisplayBounds(player.option_display_number,&display_bounds)==0){
                if(player.option_fullscreen){
                    if(player.option_fullscreen_mode=="desktop"){
                        *desired_resolution_x=display_bounds.w;
                        *desired_resolution_y=display_bounds.h;
                    }
                    else if(player.option_fullscreen_mode=="standard"){
                        Engine_Rect display_res_max=get_display_resolution_max(player.option_display_number);

                        if(display_res_max.w>-1 && display_res_max.h>-1){
                            if(*desired_resolution_x>display_res_max.w || *desired_resolution_y>display_res_max.h){
                                update_error_log("Window dimensions of "+num_to_string(*desired_resolution_x)+"x"+num_to_string(*desired_resolution_y)+
                                               " exceed the maximum display dimensions of "+num_to_string(display_res_max.w)+"x"+num_to_string(display_res_max.h)+
                                               ", adjusting window resolution to display resolution");

                                *desired_resolution_x=display_bounds.w;
                                *desired_resolution_y=display_bounds.h;
                            }
                        }
                    }
                    else if(player.option_fullscreen_mode=="windowed"){
                        *desired_resolution_x=display_bounds.w;
                        *desired_resolution_y=display_bounds.h;
                    }
                }

                *position_x=display_bounds.x-*desired_resolution_x/2+display_bounds.w/2;
                *position_y=display_bounds.y-*desired_resolution_y/2+display_bounds.h/2;

                return true;
            }
            else{
                msg="Unable to get display bounds for display #"+num_to_string(player.option_display_number)+": ";
                msg+=SDL_GetError();
                update_error_log(msg);
            }
        }
        else{
            msg="Unable to get number of displays: ";
            msg+=SDL_GetError();
            update_error_log(msg);
        }
    }

    return false;
}

void Game_Window::reload(){
    int position_x=SDL_WINDOWPOS_CENTERED;
    int position_y=SDL_WINDOWPOS_CENTERED;
    int desired_resolution_x=0;
    int desired_resolution_y=0;

    if(set_resolution(&desired_resolution_x,&desired_resolution_y)){
        bool position_adjusted=set_position(&desired_resolution_x,&desired_resolution_y,&position_x,&position_y);

        if(!position_adjusted && player.option_fullscreen){
            Engine_Rect display_res=get_display_resolution();
            Engine_Rect display_res_max=get_display_resolution_max();

            if(display_res.w>-1 && display_res.h>-1){
                if(player.option_fullscreen_mode=="standard"){
                    if(display_res_max.w>-1 && display_res_max.h>-1){
                        if(desired_resolution_x>display_res_max.w || desired_resolution_y>display_res_max.h){
                            update_error_log("Window dimensions of "+num_to_string(desired_resolution_x)+"x"+num_to_string(desired_resolution_y)+" exceed the maximum display dimensions of "+
                                           num_to_string(display_res_max.w)+"x"+num_to_string(display_res_max.h)+", adjusting window resolution to current display resolution");

                            desired_resolution_x=display_res.w;
                            desired_resolution_y=display_res.h;
                        }
                    }
                }
                else if(player.option_fullscreen_mode=="windowed"){
                    if(desired_resolution_x!=display_res.w || desired_resolution_y!=display_res.h){
                        desired_resolution_x=display_res.w;
                        desired_resolution_y=display_res.h;
                    }
                }
            }
        }

        int toggle_fullscreen=0;

        if(player.option_fullscreen){
            if(player.option_fullscreen_mode=="desktop"){
                if(position_adjusted){
                    toggle_fullscreen=SDL_SetWindowFullscreen(screen,0);
                }

                if(toggle_fullscreen==0){
                    if(position_adjusted){
                        SDL_SetWindowPosition(screen,position_x,position_y);
                    }

                    toggle_fullscreen=SDL_SetWindowFullscreen(screen,SDL_WINDOW_FULLSCREEN_DESKTOP);
                }
            }
            else if(player.option_fullscreen_mode=="standard"){
                toggle_fullscreen=SDL_SetWindowFullscreen(screen,0);

                if(toggle_fullscreen==0){
                    SDL_SetWindowSize(screen,desired_resolution_x,desired_resolution_y);

                    if(position_adjusted){
                        SDL_SetWindowPosition(screen,position_x,position_y);
                    }

                    toggle_fullscreen=SDL_SetWindowFullscreen(screen,SDL_WINDOW_FULLSCREEN);
                }
            }
            else if(player.option_fullscreen_mode=="windowed"){
                toggle_fullscreen=SDL_SetWindowFullscreen(screen,0);
                SDL_SetWindowBordered(screen,SDL_FALSE);
                SDL_SetWindowSize(screen,desired_resolution_x,desired_resolution_y);
                SDL_SetWindowPosition(screen,position_x,position_y);
            }
        }
        else{
            toggle_fullscreen=SDL_SetWindowFullscreen(screen,0);
            SDL_SetWindowBordered(screen,SDL_TRUE);
            SDL_SetWindowSize(screen,desired_resolution_x,desired_resolution_y);
            SDL_SetWindowPosition(screen,position_x,position_y);
        }

        if(toggle_fullscreen!=0){
            msg="Error toggling fullscreen: ";
            msg+=SDL_GetError();
            update_error_log(msg);
        }
    }
}

void Game_Window::get_mouse_state(int* mouse_x,int* mouse_y){
    SDL_Rect rect;
    get_renderer_viewport(&rect);

    float scale_x=0.0f;
    float scale_y=0.0f;
    get_renderer_scale(&scale_x,&scale_y);

    SDL_GetMouseState(mouse_x,mouse_y);

    float offset_x=(float)rect.x*scale_x;
    float offset_y=(float)rect.y*scale_y;

    *mouse_x=(int)ceil(((float)*mouse_x-offset_x)/scale_x);
    *mouse_y=(int)ceil(((float)*mouse_y-offset_y)/scale_y);
}

bool Game_Window::pre_initialize(){
    if(!pre_initialized){
        if(SDL_Init(SDL_INIT_EVERYTHING)!=0){
            msg="Unable to init SDL: ";
            msg+=SDL_GetError();
            update_error_log(msg);
            return false;
        }

        pre_initialized=true;

        return true;
    }
    else{
        return false;
    }
}

bool Game_Window::init(){
    if(!initialized){
        set_sdl_hints();

        //Set the window icon.
        msg="data/images/icon/";
        msg+=return_holiday_name(holiday);
        msg+=".bmp";
        icon=SDL_LoadBMP(msg.c_str());

        if(icon==0){
            msg="Unable to load icon: ";
            msg+=SDL_GetError();
            update_error_log(msg);
        }

        icon_colorkey=SDL_MapRGB(icon->format,0,0,0);
        if(SDL_SetColorKey(icon,SDL_TRUE,icon_colorkey)!=0){
            msg="Unable to set icon color key: ";
            msg+=SDL_GetError();
            update_error_log(msg);
        }

        SDL_SetWindowIcon(screen,icon);

        if(!initialize_video()){
            return false;
        }

        if(Mix_Init(MIX_INIT_OGG)==0){
            msg="SDL2_mixer initialization failed: ";
            msg+=Mix_GetError();
            update_error_log(msg);
        }
        else{
            if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,1024)==-1){
                msg="SDL2_mixer failed to open mixer: ";
                msg+=Mix_GetError();
                update_error_log(msg);
            }
            else{
                int channels_requested=2048;
                int channels_allocated=Mix_AllocateChannels(channels_requested);
                if(channels_allocated!=channels_requested){
                    msg="Error allocating mixer channels: Requested "+num_to_string(channels_requested)+", allocated "+num_to_string(channels_allocated)+".";
                    update_error_log(msg);
                }
            }
        }

        if(IMG_Init(IMG_INIT_PNG)==0){
            msg="SDL2_image initialization failed: ";
            msg+=IMG_GetError();
            update_error_log(msg);
            return false;
        }

        if(SDL_GameControllerAddMappingsFromFile("data/game_controller_db")==-1){
            msg="Error loading game controller database: ";
            msg+=SDL_GetError();
            update_error_log(msg);
        }

        //Show or hide the hardware mouse cursor.
        if(player.option_hardware_cursor){
            SDL_ShowCursor(SDL_ENABLE);
        }
        else{
            SDL_ShowCursor(SDL_DISABLE);
        }

        //Setup the hardware cursor.
        SDL_Surface* temp_surface=load_image("data/images/cursor.png");

        Uint8 cursor_data[16*4];
        Uint8 cursor_mask[16*4];
        int i=-1;
        for(int int_x=0;int_x<16;int_x++){
            for(int int_y=0;int_y<16;int_y++){
                if(int_y%8){
                    cursor_data[i]<<=1;
                    cursor_mask[i]<<=1;
                }
                else{
                    i++;
                    cursor_data[i]=cursor_mask[i]=0;
                }

                //If the surface must be locked.
                if(SDL_MUSTLOCK(temp_surface)){
                    //Lock the surface.
                    SDL_LockSurface(temp_surface);
                }

                Uint8 check_red;
                Uint8 check_green;
                Uint8 check_blue;
                Uint8 check_alpha;
                SDL_GetRGBA(surface_get_pixel(temp_surface,int_x,int_y),temp_surface->format,&check_red,&check_green,&check_blue,&check_alpha);

                //As long as the pixel is not transparent.
                if(check_alpha!=0){
                    //If the pixel is black.
                    if(check_red==0 && check_green==0 && check_blue==0){
                        cursor_mask[i]|=0x01;
                    }
                    else{
                        cursor_data[i]|=0x01;
                        cursor_mask[i]|=0x01;
                    }
                }

                //If the surface had to be locked.
                if(SDL_MUSTLOCK(temp_surface)){
                    //Unlock the surface.
                    SDL_UnlockSurface(temp_surface);
                }
            }
        }
        SDL_FreeSurface(temp_surface);
        boring_cursor=SDL_CreateCursor(cursor_data,cursor_mask,16,16,0,0);
        SDL_SetCursor(boring_cursor);

        initialized=true;

        return true;
    }
    else{
        return false;
    }
}

void Game_Window::deinitialize(){
    if(initialized){
        initialized=false;

        //Close any opened joysticks.
        for(int i=0;i<joystick.size();i++){
            //Close the joystick, if it is opened.
            if(SDL_JoystickGetAttached(joystick[i].joy)){
                SDL_JoystickClose(joystick[i].joy);
            }
        }
        joystick.clear();

        IMG_Quit();

        Mix_CloseAudio();
        Mix_Quit();

        cleanup_video();
    }

    if(pre_initialized){
        pre_initialized=false;

        SDL_Quit();
    }
}

bool Game_Window::is_initialized(){
    return initialized;
}

void Game_Window::get_renderer_viewport(SDL_Rect* rect){
    SDL_RenderGetViewport(renderer,rect);
}

void Game_Window::get_renderer_scale(float* x,float* y){
    SDL_RenderGetScale(renderer,x,y);
}

void Game_Window::update_display_number(){
    int display_index=SDL_GetWindowDisplayIndex(screen);

    if(display_index>=0){
        if(display_index!=player.option_display_number){
            player.option_display_number=display_index;

            global_options_save();
        }
    }
    else{
        msg="Unable to get display index for window: ";
        msg+=SDL_GetError();
        update_error_log(msg);
    }
}

void Game_Window::set_sdl_hints(){
    SDL_SetHint(SDL_HINT_RENDER_DRIVER,"opengl");
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"nearest");
    SDL_SetHint(SDL_HINT_RENDER_VSYNC,num_to_string((int)false).c_str());
    SDL_SetHint(SDL_HINT_VIDEO_ALLOW_SCREENSAVER,num_to_string((int)false).c_str());
}

SDL_Texture* Game_Window::create_texture_from_surface(SDL_Surface* surface){
    return SDL_CreateTextureFromSurface(renderer,surface);
}

SDL_Texture* Game_Window::create_texture(uint32_t format,int access,int width,int height){
    return SDL_CreateTexture(renderer,format,access,width,height);
}

int Game_Window::set_render_target(SDL_Texture* texture){
    return SDL_SetRenderTarget(renderer,texture);
}

void Game_Window::set_render_draw_blend_mode(SDL_BlendMode blend_mode){
    SDL_SetRenderDrawBlendMode(renderer,blend_mode);
}

void Game_Window::set_render_draw_color(const Color& color,double opacity){
    SDL_SetRenderDrawColor(renderer,(uint8_t)color.get_red(),(uint8_t)color.get_green(),(uint8_t)color.get_blue(),(uint8_t)(opacity*255.0));
}

void Game_Window::render_copy_ex(SDL_Texture* texture,const SDL_Rect* srcrect,const SDL_Rect* dstrect,const double angle,const SDL_Point* center,const SDL_RendererFlip flip){
    SDL_RenderCopyEx(renderer,texture,srcrect,dstrect,angle,center,flip);
}

void Game_Window::render_fill_rect(SDL_Rect* rect){
    SDL_RenderFillRect(renderer,rect);
}

void Game_Window::render_draw_line(int x1,int y1,int x2,int y2){
    SDL_RenderDrawLine(renderer,x1,y1,x2,y2);
}

void Game_Window::clear_renderer(const Color& color){
    set_render_draw_color(color,color.get_alpha_double());
    SDL_RenderClear(renderer);
}

void Game_Window::render_present(){
    SDL_RenderPresent(renderer);
}

void Game_Window::get_rgba_masks(uint32_t* rmask,uint32_t* gmask,uint32_t* bmask,uint32_t* amask){
    if(SDL_BYTEORDER==SDL_BIG_ENDIAN){
        *rmask=0xff000000;
        *gmask=0x00ff0000;
        *bmask=0x0000ff00;
        *amask=0x000000ff;
    }
    else{
        *rmask=0x000000ff;
        *gmask=0x0000ff00;
        *bmask=0x00ff0000;
        *amask=0xff000000;
    }
}

void Game_Window::screenshot(){
    #ifdef GAME_OS_ANDROID
        update_error_log("Sorry, screenshots are disabled in Android, due to them exploding. Please use Android's own screenshot feature.");
        return;
    #endif

    if(player.name!="\x1F"){
        //Determine the date and time.
        time_t now;
        struct tm *tm_now;
        char buff[BUFSIZ];
        now=time(NULL);
        tm_now=localtime(&now);

        //Store the date and time in buff.
        strftime(buff,sizeof buff,"%Y-%m-%d_%H.%M.%S",tm_now);

        //Copy buff's data into the date string for use with screenshot_name below.
        string date=buff;

        string screenshot_name=profile.get_home_directory()+"profiles/"+player.name+"/screenshots/";
        screenshot_name+=date;
        screenshot_name+=".png";

        int actual_width=0;
        int actual_height=0;
        SDL_GetRendererOutputSize(renderer,&actual_width,&actual_height);

        unsigned char* pixel_data=new unsigned char[4*actual_width*actual_height];

        if(pixel_data!=0){
            if(SDL_RenderReadPixels(renderer,NULL,SDL_PIXELFORMAT_ABGR8888,pixel_data,actual_width*4)!=0){
                msg="Error reading renderer pixels: ";
                msg+=SDL_GetError();
                update_error_log(msg);
            }

            uint32_t rmask,gmask,bmask,amask;
            get_rgba_masks(&rmask,&gmask,&bmask,&amask);

            SDL_Surface* surface=SDL_CreateRGBSurfaceFrom(pixel_data,actual_width,actual_height,32,actual_width*4,rmask,gmask,bmask,amask);

            if(surface==0){
                msg="Error creating surface for screenshot: ";
                msg+=SDL_GetError();
                update_error_log(msg);
            }
            else{
                if(IMG_SavePNG(surface,screenshot_name.c_str())!=0){
                    msg="Error saving screenshot: ";
                    msg+=IMG_GetError();
                    update_error_log(msg);
                }

                SDL_FreeSurface(surface);
            }

            delete[] pixel_data;
        }
        else{
            update_error_log("Error allocating memory for screenshot.");
        }
    }
}
