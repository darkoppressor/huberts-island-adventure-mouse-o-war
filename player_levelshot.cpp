/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "player.h"
#include "world.h"
#include "mirror.h"
#include "savepng.h"
#include "pixels.h"
#include "render.h"
#include "message_log.h"

#include <boost/filesystem.hpp>

using namespace std;

void Player::levelshot(){
    if(name!="\x1F" && game_in_progress && option_renderer==RENDERER_HARDWARE){
        double temp_cam_x=camera_x;
        double temp_cam_y=camera_y;
        double temp_cam_w=camera_w;
        double temp_cam_h=camera_h;

        camera_x=0.0;
        camera_y=0.0;
        camera_w=level.level_x;
        camera_h=level.level_y;

        GLuint fbo=0;
        GLuint fbo_texture=0;

        glGenTextures(1,&fbo_texture);
        glBindTexture(GL_TEXTURE_2D,fbo_texture);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,level.level_x,level.level_y,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);

        glGenFramebuffers(1,&fbo);
        glBindFramebuffer(GL_FRAMEBUFFER,fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,fbo_texture,0);

        GLenum fbo_status=glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if(fbo_status!=GL_FRAMEBUFFER_COMPLETE){
            ss.clear();ss.str("");ss<<"Framebuffer Object Error: ";ss<<fbo_status;msg=ss.str();
            update_error_log(msg);
        }

        glPushAttrib(GL_VIEWPORT_BIT);
        glViewport(0,0,level.level_x,level.level_y);
        glBindTexture(GL_TEXTURE_2D,0);
        image.current_texture=0;

        //As long as the current level is not DevWorld.
        if(current_level!=2){
            glClear(GL_COLOR_BUFFER_BIT);
        }

        ///POOP
        ///render_background();

        int check_x_start=0;
        int check_x_end=level.level_x/TILE_SIZE;
        int check_y_start=0;
        int check_y_end=level.level_y/TILE_SIZE;

        //Render each onscreen mirror tile.
        /**for(int int_y=check_y_start;int_y<check_y_end;int_y++){
            for(int int_x=check_x_start;int_x<check_x_end;int_x++){
                //As long as the current tile is within the level's boundaries.
                if(int_x>=0 && int_x<=(level.level_x/TILE_SIZE)-1 && int_y>=0 && int_y<=(level.level_y/TILE_SIZE)-1){
                    if(!level.tile_array[int_x][int_y].foreground && level.tile_array[int_x][int_y].special==TILE_SPECIAL_MIRROR){
                        level.tile_array[int_x][int_y].render();
                    }
                }
            }
        }

        //Render mirrored things:

        for(int i=0;i<vector_signs.size();i++){
            vector_signs[i].render(MIRRORED);
        }

        for(int i=0;i<vector_springs.size();i++){
            vector_springs[i].render(MIRRORED);
        }

        for(int i=0;i<vector_boosters.size();i++){
            vector_boosters[i].render(MIRRORED);
        }

        for(int i=0;i<vector_effect_water_splash.size();i++){
            vector_effect_water_splash[i].render(MIRRORED);
        }
        for(int i=0;i<vector_effect_water_bubble.size();i++){
            vector_effect_water_bubble[i].render(MIRRORED);
        }

        for(int i=0;i<vector_triggers.size();i++){
            vector_triggers[i].render(MIRRORED);
        }

        for(int i=0;i<vector_doors.size();i++){
            vector_doors[i].render(MIRRORED);
        }

        for(int i=0;i<vector_items.size();i++){
            vector_items[i].render(MIRRORED);
        }

        for(int i=0;i<vector_npcs.size();i++){
            vector_npcs[i].render(MIRRORED);
        }

        for(int i=0;i<vector_shots.size();i++){
            vector_shots[i].render(MIRRORED);
        }

        //If the player is in a level.
        if(!on_worldmap()){
            render(MIRRORED);

            for(int i=0;i<mp_players.size();i++){
                mp_players[i].render(MIRRORED);
            }
        }

        for(int i=0;i<vector_traps.size();i++){
            vector_traps[i].render(MIRRORED);
        }

        for(int i=0;i<vector_moving_platforms.size();i++){
            vector_moving_platforms[i].render(MIRRORED);
        }

        //Render each onscreen background tile.
        for(int int_y=check_y_start;int_y<check_y_end;int_y++){
            for(int int_x=check_x_start;int_x<check_x_end;int_x++){
                //As long as the current tile is within the level's boundaries.
                if(int_x>=0 && int_x<=(level.level_x/TILE_SIZE)-1 && int_y>=0 && int_y<=(level.level_y/TILE_SIZE)-1){
                    //Render this background tile.
                    if(level.tile_array[int_x][int_y].special!=TILE_SPECIAL_MIRROR){
                        level.tile_background_array[int_x][int_y].render();
                    }
                }
            }
        }*/

        //Render each onscreen terrain tile.
        for(int int_y=check_y_start;int_y<check_y_end;int_y++){
            for(int int_x=check_x_start;int_x<check_x_end;int_x++){
                //As long as the current tile is within the level's boundaries.
                if(int_x>=0 && int_x<=(level.level_x/TILE_SIZE)-1 && int_y>=0 && int_y<=(level.level_y/TILE_SIZE)-1){
                    if(!level.tile_array[int_x][int_y].foreground && level.tile_array[int_x][int_y].special!=TILE_SPECIAL_MIRROR){
                        level.tile_array[int_x][int_y].render();
                    }
                }
            }
        }

        /**for(int i=0;i<vector_signs.size();i++){
            vector_signs[i].render();
        }

        for(int i=0;i<vector_springs.size();i++){
            vector_springs[i].render();
        }

        for(int i=0;i<vector_boosters.size();i++){
            vector_boosters[i].render();
        }

        for(int i=0;i<vector_effect_water_splash.size();i++){
            vector_effect_water_splash[i].render();
        }
        for(int i=0;i<vector_effect_water_bubble.size();i++){
            vector_effect_water_bubble[i].render();
        }

        for(int i=0;i<vector_triggers.size();i++){
            vector_triggers[i].render();
        }

        for(int i=0;i<vector_doors.size();i++){
            vector_doors[i].render();
        }

        for(int i=0;i<vector_items.size();i++){
            vector_items[i].render();
        }

        for(int i=0;i<vector_npcs.size();i++){
            vector_npcs[i].render();
        }

        //Render the shots.
        for(int i=0;i<vector_shots.size();i++){
            vector_shots[i].render();
        }

        //Render each player held in the player vector in sequence.
        //If the player is on the world map.
        if(on_worldmap()){
            worldmap_render();

            for(int i=0;i<mp_players.size();i++){
                mp_players[i].worldmap_render();
            }
        }
        //If the player is in a level.
        else{
            //If the player is alive, render him in the normal place.
            //If he is dying, he will instead be rendered on top of the foreground tiles.
            if(!DYING && !bubble_mode){
                render();
            }

            for(int i=0;i<mp_players.size();i++){
                if(!mp_players[i].DYING && !mp_players[i].bubble_mode){
                    mp_players[i].render();
                }
            }
        }

        for(int i=0;i<vector_traps.size();i++){
            vector_traps[i].render();
        }

        for(int i=0;i<vector_moving_platforms.size();i++){
            vector_moving_platforms[i].render();
        }

        //Render the spawn(s).
        for(int i=0;i<vector_items.size();i++){
            if(vector_items[i].type==ITEM_SPAWNPOINT){
                vector_items[i].render(false,true);
            }
        }

        //Render each onscreen foreground tile.
        for(int int_y=check_y_start;int_y<check_y_end;int_y++){
            for(int int_x=check_x_start;int_x<check_x_end;int_x++){
                //As long as the current tile is within the level's boundaries.
                if(int_x>=0 && int_x<=(level.level_x/TILE_SIZE)-1 && int_y>=0 && int_y<=(level.level_y/TILE_SIZE)-1){
                    if(level.tile_array[int_x][int_y].foreground){
                        level.tile_array[int_x][int_y].render();
                    }
                }
            }
        }

        //Since the player is dying, render him here instead of the usual place.
        if(DYING || bubble_mode){
            render();
        }

        if(!on_worldmap()){
            for(int i=0;i<mp_players.size();i++){
                if(mp_players[i].DYING || mp_players[i].bubble_mode){
                    mp_players[i].render();
                }
            }
        }

        if(on_worldmap()){
            for(int i=0;i<vector_effect_firework.size();i++){
                vector_effect_firework[i].render();
            }
        }*/

        ///POOP
        ///render_foreground();

        glBindFramebuffer(GL_FRAMEBUFFER,0);
        glClear(GL_COLOR_BUFFER_BIT);
        glPopAttrib();

        glBindTexture(GL_TEXTURE_2D,fbo_texture);

        //Determine the date and time.
        time_t now;
        struct tm *tm_now;
        char buff[BUFSIZ];
        now=time(NULL);
        tm_now=localtime(&now);

        //Store the date and time in buff.
        strftime(buff,sizeof buff,"%Y-%m-%d_%H.%M.%S",tm_now);

        //Copy buff's data into the date string for use below.
        string date=buff;

        boost::filesystem::create_directory(profile.get_home_directory()+"levelshots");

        //Set the filename.
        string levelshot_name=profile.get_home_directory()+"levelshots/";
        levelshot_name+=level.return_level_name(current_level)+" "+date;
        levelshot_name+=".png";

        //Create temporary surfaces.
        SDL_Surface *surface=NULL;
        SDL_Surface *flipped_surface=NULL;

        //This will hold the pixel data from the frame buffer.
        unsigned char pixel_data[4*level.level_x*level.level_y];

        //Read the pixels from the frame buffer and store them in pixel_data.
        glGetTexImage(GL_TEXTURE_2D,0,GL_BGRA,GL_UNSIGNED_BYTE,&pixel_data);

        //Create an SDL surface from this pixel data.
        surface=SDL_CreateRGBSurfaceFrom(&pixel_data,level.level_x,level.level_y,main_window.screen->format->BitsPerPixel,level.level_x*main_window.screen->format->BytesPerPixel,main_window.screen->format->Rmask,main_window.screen->format->Gmask,main_window.screen->format->Bmask,main_window.screen->format->Amask);

        //Create a SDL surface for holding the flipped image.
        flipped_surface=SDL_CreateRGBSurface(SDL_SWSURFACE,surface->w,surface->h,surface->format->BitsPerPixel,surface->format->Rmask,surface->format->Gmask,surface->format->Bmask,surface->format->Amask);

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
        IMG_SavePNG(levelshot_name.c_str(),flipped_surface,-1);

        //Free the surfaces.
        SDL_FreeSurface(flipped_surface);
        SDL_FreeSurface(surface);

        glBindTexture(GL_TEXTURE_2D,0);

        glDeleteTextures(1,&fbo_texture);
        glDeleteFramebuffers(1,&fbo);

        camera_x=temp_cam_x;
        camera_y=temp_cam_y;
        camera_w=temp_cam_w;
        camera_h=temp_cam_h;
    }
}
