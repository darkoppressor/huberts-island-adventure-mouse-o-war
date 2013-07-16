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
#include "savepng.h"

#include <time.h>
#include <string>

using namespace std;

Game_Window::Game_Window(){
    SCREEN_WIDTH=DEFAULT_SCREEN_WIDTH;
    SCREEN_HEIGHT=DEFAULT_SCREEN_HEIGHT;

    SCREEN_BPP=32;

    REAL_SCREEN_WIDTH=SCREEN_WIDTH;
    REAL_SCREEN_HEIGHT=SCREEN_HEIGHT;

    screen=NULL;

    icon=NULL;
    icon_colorkey=0;

    fbo=0;
    fbo_texture=0;

    fbo_mode=false;

    boring_cursor=NULL;
}

bool Game_Window::cleanup_opengl(){
    if(fbo_mode){
        glDeleteTextures(1,&fbo_texture);
        glDeleteFramebuffers(1,&fbo);
    }
}

bool Game_Window::initialize_opengl(){
    fbo_mode=false;
    REAL_SCREEN_WIDTH=SCREEN_WIDTH;
    REAL_SCREEN_HEIGHT=SCREEN_HEIGHT;

    if(player.option_screen_width!=SCREEN_WIDTH || player.option_screen_height!=SCREEN_HEIGHT){
        if(player.option_screen_width!=0 && player.option_screen_height!=0){
            if(player.option_renderer==RENDERER_HARDWARE){
                fbo_mode=true;
                REAL_SCREEN_WIDTH=player.option_screen_width;
                REAL_SCREEN_HEIGHT=player.option_screen_height;
            }
        }
    }

    //Set OpenGL attributes:
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,32);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);

    //Set up the screen:
    if(player.option_fullscreen){
        if(player.option_fullscreen_mode==FULLSCREEN_MODE_STANDARD){
            screen=SDL_SetVideoMode(REAL_SCREEN_WIDTH,REAL_SCREEN_HEIGHT,SCREEN_BPP,SDL_OPENGL|SDL_HWSURFACE|SDL_FULLSCREEN);
        }
        else if(player.option_fullscreen_mode==FULLSCREEN_MODE_WINDOWED){
            screen=SDL_SetVideoMode(REAL_SCREEN_WIDTH,REAL_SCREEN_HEIGHT,SCREEN_BPP,SDL_OPENGL|SDL_HWSURFACE|SDL_NOFRAME);
        }
    }
    else{
        screen=SDL_SetVideoMode(REAL_SCREEN_WIDTH,REAL_SCREEN_HEIGHT,SCREEN_BPP,SDL_OPENGL|SDL_HWSURFACE);
    }

    //Initialize OpenGL.
    //I am still a beginner at using OpenGL, so I'll put forth my best explanation for how things work, exactly.

    //Enable 2D textures.
    glEnable(GL_TEXTURE_2D);

    //Enable blending.
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    //Set clear color. When the screen is cleared, it will be filled with this color.
    glClearColor(0,0,0,1);

    //Set the OpenGL viewport to be the same size as our screen.
    glViewport(0,0,REAL_SCREEN_WIDTH,REAL_SCREEN_HEIGHT);

    //Set projection:
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,SCREEN_WIDTH,SCREEN_HEIGHT,0,-1,1);

    //Initialize modelview matrix:
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //Initialize GLEW.
    GLenum glew_err=glewInit();
    if(glew_err!=GLEW_OK){
        msg="GLEW Error: ";
        msg+=(const char*)glewGetErrorString(glew_err);
        update_error_log(msg);
        return false;
    }

    //If OpenGL version 1.1 is not supported.
    if(!GLEW_VERSION_1_1){
        update_error_log("OpenGL 1.1 or higher is not supported by this machine!");
        return false;
    }

    if(fbo_mode){
        //If OpenGL version 3.0 is not supported, check for the needed extension.
        //If that is also unsupported, we cannot use this functionality.
        if(!GLEW_VERSION_3_0){
            if(!GLEW_ARB_framebuffer_object){
                update_error_log("GLEW Error: Extension GL_ARB_framebuffer_object is not supported by your video card drivers.");
                return false;
            }
        }
    }

    if(fbo_mode){
        //Prepare the fbo.
        glGenTextures(1,&fbo_texture);
        glBindTexture(GL_TEXTURE_2D,fbo_texture);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,SCREEN_WIDTH,SCREEN_HEIGHT,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);

        glGenFramebuffers(1,&fbo);
        glBindFramebuffer(GL_FRAMEBUFFER,fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,fbo_texture,0);

        GLenum fbo_status=glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if(fbo_status!=GL_FRAMEBUFFER_COMPLETE){
            ss.clear();ss.str("");ss<<"Framebuffer Object Error: ";ss<<fbo_status;msg=ss.str();
            update_error_log(msg);
            return false;
        }

        glBindFramebuffer(GL_FRAMEBUFFER,0);
    }

    return true;
}

bool Game_Window::init(){
    //Initialize all of the standard SDL stuff, and return false if it did not initialize properly.
    if(SDL_Init(SDL_INIT_EVERYTHING)==-1){
        msg="Unable to init SDL: ";
        msg+=SDL_GetError();
        update_error_log(msg);
        return false;
    }

    //Attempt to center the window on the screen.
    SDL_putenv("SDL_VIDEO_CENTERED=center");

    //Set the window icon.
    msg="data/images/icon/";
    msg+=return_holiday_name(holiday);
    msg+=".bmp";
    icon=SDL_LoadBMP(msg.c_str());
    icon_colorkey=SDL_MapRGB(icon->format,0,0,0);
    SDL_SetColorKey(icon,SDL_SRCCOLORKEY,icon_colorkey);
    SDL_WM_SetIcon(icon,NULL);

    if(player.option_renderer==RENDERER_HARDWARE){
        if(!initialize_opengl()){
            return false;
        }
    }
    else if(player.option_renderer==RENDERER_SOFTWARE){
        //Set up the screen:
        if(player.option_fullscreen){
            screen=SDL_SetVideoMode(REAL_SCREEN_WIDTH,REAL_SCREEN_HEIGHT,SCREEN_BPP,SDL_SWSURFACE|SDL_FULLSCREEN);
        }
        else{
            screen=SDL_SetVideoMode(REAL_SCREEN_WIDTH,REAL_SCREEN_HEIGHT,SCREEN_BPP,SDL_SWSURFACE);
        }
    }

    //If the screen could not be set up.
    if(!screen){
        msg="Unable to set video mode: ";
        msg+=SDL_GetError();
        update_error_log(msg);
        return false;
    }

    if(player.option_renderer==RENDERER_HARDWARE){
        //If there were any OpenGL errors.
        int gl_error=glGetError();
        if(gl_error!=GL_NO_ERROR){
            ss.clear();ss.str("");ss<<"OpenGL initialization failed. glGetError returned ";ss<<gl_error;msg=ss.str();
            update_error_log(msg);
            return false;
        }
    }

    //Start up the audio system.
    if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,1024)==-1){
        update_error_log("SDL_Mixer initialization failed.");
    }

    Mix_AllocateChannels(2000);

    //Open all available joysticks.
    for(int i=0;i<SDL_NumJoysticks();i++){
        joystick.push_back(joy_stick());
        joystick[i].joy=NULL;
        joystick[i].joy=SDL_JoystickOpen(i);
    }

    //Check for any joystick open failures.
    for(int i=0;i<SDL_NumJoysticks();i++){
        //If the joystick could not be opened.
        if(joystick[i].joy==NULL){
            string error_string="";
            ss.clear();ss.str("");ss<<"Failed to open joystick ";ss<<i;ss<<".";error_string=ss.str();
            update_error_log(error_string);
        }
    }

    number_of_joysticks=SDL_NumJoysticks();

    //Set the window caption.
    SDL_WM_SetCaption("Hubert's Island Adventure: Mouse o' War","Hubert's Island Adventure: Mouse o' War");

    //Show or hide the hardware mouse cursor.
    if(player.option_hardware_cursor){
        SDL_ShowCursor(SDL_ENABLE);
    }
    else{
        SDL_ShowCursor(SDL_DISABLE);
    }

    //Setup the hardware cursor.
    SDL_Surface* temp_surface=load_image_sdl("data/images/cursor.png");

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

    //Everything initialized fine, so return true.
    return true;
}

void Game_Window::reinitialize(){
    int things_loaded=0;
    int things_to_load=2;
    if(player.game_in_progress){
        things_to_load++;
    }

    //Remember which window is currently open, if any.
    short which_window_open=window_manager.which_window_open();

    //Close all windows.
    window_manager.close_windows(0);

    //Pause the game.
    player.toggle_pause(true);

    render_loading_screen((double)++things_loaded/(double)things_to_load);

    if(player.option_renderer==RENDERER_HARDWARE){
        //To toggle fullscreen with OpenGL, we must reinitialize all of the OpenGL stuff and reload the textures.
        cleanup_opengl();
        initialize_opengl();
    }
    else if(player.option_renderer==RENDERER_SOFTWARE){
        if(player.option_fullscreen){
            screen=SDL_SetVideoMode(REAL_SCREEN_WIDTH,REAL_SCREEN_HEIGHT,SCREEN_BPP,SDL_SWSURFACE|SDL_FULLSCREEN);
        }
        else{
            screen=SDL_SetVideoMode(REAL_SCREEN_WIDTH,REAL_SCREEN_HEIGHT,SCREEN_BPP,SDL_SWSURFACE);
        }
    }

    unload_world();
    load_world();
    render_loading_screen((double)++things_loaded/(double)things_to_load);

    //Only reload level images if the game is in progress (and thus a level is already loaded).
    if(player.game_in_progress){
        image.unload_images_level(player.current_level);
        image.load_images_level(player.current_level,player.current_sub_level,player.non_number_level);
        render_loading_screen((double)++things_loaded/(double)things_to_load);
    }

    //If a window was open before.
    if(which_window_open!=-1){
        if(which_window_open==WHICH_WINDOW_INVENTORY){
            window_inventory[0].turn_on();
        }
        else if(which_window_open==WHICH_WINDOW_MAP){
            window_map[0].turn_on();
        }
        else if(which_window_open==WHICH_WINDOW_MESSAGE){
            window_message[0].turn_on();
        }
        else if(which_window_open==WHICH_WINDOW_SETUP_SURVIVAL){
            player.survival_seconds_to_escape=window_setup_survival[0].recall_seconds;

            if(!window_setup_survival[0].is_multiplayer){
                window_setup_survival[0].setup(false);
            }
            else{
                window_setup_survival[0].setup(true);
            }

            window_setup_survival[0].turn_on();

            player.current_button=6;
        }
        else if(which_window_open==WHICH_WINDOW_SHOP){
            window_shop[0].setup();

            window_shop[0].turn_on();

            player.current_button=2;
        }
        else if(which_window_open==WHICH_WINDOW_UPGRADES){
            window_upgrades[0].setup();

            window_upgrades[0].turn_on();

            player.current_button=2;
        }
        else if(which_window_open>=WHICH_WINDOW_OTHER){
            vector_windows[which_window_open-WHICH_WINDOW_OTHER].turn_on();
        }
    }
    else{
        //Unpause the game.
        player.toggle_pause(false);
    }
}

void Game_Window::screenshot(){
    if(player.name!="\x1F"){
        //First, we setup the filename for the screenshot.

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

        //Used to store the filename of the screenshot.
        string screenshot_name;

        //Set the filename.
        screenshot_name=profile.get_home_directory()+"profiles/";
        screenshot_name+=player.name;
        screenshot_name+="/screenshots/";
        screenshot_name+=date;
        screenshot_name+=".png";

        if(player.option_renderer==RENDERER_HARDWARE){
            //Create temporary surfaces.
            SDL_Surface *surface=NULL;
            SDL_Surface *flipped_surface=NULL;

            //This will hold the pixel data from the frame buffer.
            unsigned char pixel_data[4*REAL_SCREEN_WIDTH*REAL_SCREEN_HEIGHT];

            //Read the pixels from the frame buffer and store them in pixel_data.
            glReadPixels(0,0,REAL_SCREEN_WIDTH,REAL_SCREEN_HEIGHT,GL_RGBA,GL_UNSIGNED_BYTE,pixel_data);

            uint32_t rmask,gmask,bmask,amask;
            if(SDL_BYTEORDER==SDL_BIG_ENDIAN){
                rmask=0xff000000;
                gmask=0x00ff0000;
                bmask=0x0000ff00;
                amask=0x000000ff;
            }
            else{
                rmask=0x000000ff;
                gmask=0x0000ff00;
                bmask=0x00ff0000;
                amask=0xff000000;
            }

            //Create an SDL surface from this pixel data.
            surface=SDL_CreateRGBSurfaceFrom(pixel_data,REAL_SCREEN_WIDTH,REAL_SCREEN_HEIGHT,screen->format->BitsPerPixel,REAL_SCREEN_WIDTH*screen->format->BytesPerPixel,rmask,gmask,bmask,amask);

            //Create a SDL surface for holding the flipped image.
            flipped_surface=SDL_CreateRGBSurface(SDL_SWSURFACE,surface->w,surface->h,surface->format->BitsPerPixel,rmask,gmask,bmask,amask);

            //If the surface must be locked.
            if(SDL_MUSTLOCK(surface)){
                //Lock the surface.
                SDL_LockSurface(surface);
            }

            //Read the pixel data from surface and store it (flipped vertically) in flipped_surface.
            for(int x=0,rx=surface->w-1;x<surface->w;x++,rx--){
                for(int y=0,ry=surface->h-1;y<surface->h;y++,ry--){
                    Uint32 pixel=surface_get_pixel(surface,x,y);
                    surface_put_pixel(flipped_surface,x,ry,pixel);
                }
            }

            //If the surface had to be locked.
            if(SDL_MUSTLOCK(surface)){
                //Unlock the surface.
                SDL_UnlockSurface(surface);
            }

            //Save the flipped surface to the screenshot file.
            IMG_SavePNG(screenshot_name.c_str(),flipped_surface,-1);

            //Free the surfaces.
            SDL_FreeSurface(flipped_surface);
            SDL_FreeSurface(surface);
        }
        else if(player.option_renderer==RENDERER_SOFTWARE){
            IMG_SavePNG(screenshot_name.c_str(),screen,-1);
        }
    }
}
