/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "player.h"
#include "world.h"
#include "mirror.h"
#include "pixels.h"
#include "render.h"
#include "message_log.h"
#include "rtt_manager.h"
#include "file_io.h"

using namespace std;

void Player::levelshot(){
    #ifdef GAME_OS_ANDROID
        update_error_log("Sorry, levelshots are disabled in Android, due to them exploding.");
        return;
    #endif

    if(name!="\x1F" && game_in_progress){
        double temp_cam_x=camera_x;
        double temp_cam_y=camera_y;
        double temp_cam_w=camera_w;
        double temp_cam_h=camera_h;

        camera_x=0.0;
        camera_y=0.0;
        camera_w=level.level_x;
        camera_h=level.level_y;

        Rtt_Manager::add_texture("levelshot",level.level_x,level.level_y);

        Rtt_Manager::set_render_target("levelshot");

        //As long as the current level is not DevWorld.
        if(current_level!=2){
            main_window.clear_renderer(Color(0,0,0,255));
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

        File_IO::create_directory(profile.get_home_directory()+"levelshots");

        //Set the filename.
        string levelshot_name=profile.get_home_directory()+"levelshots/";
        levelshot_name+=level.return_level_name(current_level)+" "+date;
        levelshot_name+=".png";

        int actual_width=0;
        int actual_height=0;
        SDL_GetRendererOutputSize(main_window.renderer,&actual_width,&actual_height);

        unsigned char* pixel_data=new unsigned char[4*actual_width*actual_height];

        if(pixel_data!=0){
            if(SDL_RenderReadPixels(main_window.renderer,NULL,SDL_PIXELFORMAT_ABGR8888,pixel_data,actual_width*4)!=0){
                msg="Error reading renderer pixels: ";
                msg+=SDL_GetError();
                update_error_log(msg);
            }

            uint32_t rmask,gmask,bmask,amask;
            main_window.get_rgba_masks(&rmask,&gmask,&bmask,&amask);

            SDL_Surface* surface=SDL_CreateRGBSurfaceFrom(pixel_data,actual_width,actual_height,32,actual_width*4,rmask,gmask,bmask,amask);

            if(surface==0){
                msg="Error creating surface for levelshot: ";
                msg+=SDL_GetError();
                update_error_log(msg);
            }
            else{
                if(IMG_SavePNG(surface,levelshot_name.c_str())!=0){
                    msg="Error saving levelshot: ";
                    msg+=IMG_GetError();
                    update_error_log(msg);
                }

                SDL_FreeSurface(surface);
            }

            delete[] pixel_data;
        }
        else{
            update_error_log("Error allocating memory for levelshot.");
        }

        Rtt_Manager::reset_render_target();

        Rtt_Manager::remove_texture("levelshot");

        camera_x=temp_cam_x;
        camera_y=temp_cam_y;
        camera_w=temp_cam_w;
        camera_h=temp_cam_h;
    }
}
