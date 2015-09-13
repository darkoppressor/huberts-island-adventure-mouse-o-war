/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "update.h"
#include "world.h"
#include "quit.h"
#include "render.h"
#include "version.h"
#include "mirror.h"
#include "hud_buttons.h"
#include "holidays.h"
#include "time_string.h"
#include "score.h"

#include <SDL.h>
#include <SDL_gfxPrimitives.h>

using namespace std;

//Gather input.
void input(){
    new_shots.clear();
    new_npcs.clear();

    //Start each frame of updates with the tooltip off.
    tooltip.on=false;
    tooltip_hint.on=false;

    player.prepare_for_input();

    if(!player.pause && player.game_in_progress){
        for(int i=0;i<mp_players.size();i++){
            mp_players[i].prepare_for_input();

            if(!player.on_worldmap()){
                mp_players[i].ai(i);
            }
            else{
                mp_players[i].worldmap_ai();
            }
        }

        if(!player.on_worldmap()){
            player.handle_input_states();

            for(int i=0;i<mp_players.size();i++){
                mp_players[i].handle_input_states();
            }
        }
    }

    while(SDL_PollEvent(&event)){
        hud_buttons_handle_input_events();

        window_inventory[0].handle_input_events();

        window_map[0].handle_input_events();

        window_message[0].handle_input_events();

        window_setup_survival[0].handle_input_events();

        window_shop[0].handle_input_events();

        window_upgrades[0].handle_input_events();

        for(int i=0;i<windows_level_info.size();i++){
            windows_level_info[i].handle_input_events();
        }

        for(int i=0;i<vector_windows.size();i++){
            vector_windows[i].handle_input_events();
        }

        player.handle_input_events();

        if(!player.on_worldmap()){
            for(int i=0;i<mp_players.size();i++){
                if(!mp_players[i].ai_controlled){
                    mp_players[i].handle_input_events();
                }
            }
        }

        switch(event.type){
            //Check to see if the player has X'ed out of the game. If so, run quit_game().
            case SDL_QUIT:
            quit_game();
            break;
        }
    }

    if(!player.on_worldmap()){
        for(int i=0;i<mp_players.size();i++){
            if(mp_players[i].ai_controlled){
                mp_players[i].handle_input_events();
            }
        }
    }

    hud_buttons_handle_input_states();

    window_inventory[0].handle_input_states();

    window_map[0].handle_input_states();

    window_message[0].handle_input_states();

    window_setup_survival[0].handle_input_states();

    window_shop[0].handle_input_states();

    window_upgrades[0].handle_input_states();

    for(int i=0;i<windows_level_info.size();i++){
        windows_level_info[i].handle_input_states();
    }

    for(int i=0;i<vector_windows.size();i++){
        vector_windows[i].handle_input_states();
    }

    //As long as the game is not paused and is in progress.
    if(!player.pause && player.game_in_progress){
        //Gather input from the players.
        //If the player is on the world map.
        if(player.on_worldmap()){
            player.worldmap_handle_input_states();

            for(int i=0;i<mp_players.size();i++){
                mp_players[i].worldmap_handle_input_states();
            }
        }

        //Run the AI for NPCs.
        for(int i=0;i<vector_npcs.size();i++){
            if(fabs(vector_npcs[i].x-player.cam_focused_x())<PROCESS_RANGE && fabs(vector_npcs[i].y-player.cam_focused_y())<PROCESS_RANGE){
                vector_npcs[i].handle_ai(i);
                vector_npcs[i].handle_input();
            }
        }
    }
    //If the game is paused and is in progress.
    else if(player.pause && player.game_in_progress){
        player.handle_input_states_always();
        player.handle_input_states_during_play();
    }
    //If the game is not in progress.
    else if(!player.game_in_progress){
        player.handle_input_states_always();
    }
}

//Move things that need to be moved.
void movement(){
    if(player.game_in_progress){
        level.process_title();
    }

    if(!player.game_in_progress || (player.game_in_progress && !player.pause)){
        if(player.fireworks_rate>0){
            unsigned int random_max=(unsigned int)(UPDATE_RATE+UPDATE_RATE/2.0);
            if(!player.on_worldmap()){
                random_max=(unsigned int)(UPDATE_RATE*2.0);
            }
            if(random_range(0,random_max)<player.fireworks_rate){
                player.create_firework();
            }
        }

        for(int i=0;i<vector_effect_firework.size();i++){
            vector_effect_firework[i].move();
        }
    }

    if(!player.pause && player.game_in_progress){
        //Move the moving platforms.
        for(int i=0;i<vector_moving_platforms.size();i++){
            vector_moving_platforms[i].move();
        }

        player.handle_tracers();

        level.devworld_tiles.process();

        //Move the player.
        //If the player is on the world map.
        if(player.on_worldmap()){
            player.worldmap_move();

            for(int i=0;i<mp_players.size();i++){
                mp_players[i].worldmap_move();
            }
        }
        //If the player is in a level.
        else{
            player.move();

            for(int i=0;i<mp_players.size();i++){
                mp_players[i].move();
            }
        }

        //Move the NPCs.
        for(int i=0;i<vector_npcs.size();i++){
            if(fabs(vector_npcs[i].x-player.cam_focused_x())<PROCESS_RANGE && fabs(vector_npcs[i].y-player.cam_focused_y())<PROCESS_RANGE){
                vector_npcs[i].move();
            }
        }

        //Move the traps.
        for(int i=0;i<vector_traps.size();i++){
            if(fabs(vector_traps[i].x-player.cam_focused_x())<PROCESS_RANGE && fabs(vector_traps[i].y-player.cam_focused_y())<PROCESS_RANGE){
                vector_traps[i].move();
            }
        }

        //Move the shots.
        for(int i=0;i<vector_shots.size();i++){
            if(fabs(vector_shots[i].x-player.cam_focused_x())<PROCESS_RANGE && fabs(vector_shots[i].y-player.cam_focused_y())<PROCESS_RANGE){
                vector_shots[i].move();
            }
        }

        //Move the items that are in the air.
        //Items only move while in the air. Once they land, they don't move anymore.
        for(int i=0;i<vector_items.size();i++){
            if(fabs(vector_items[i].x-player.cam_focused_x())<PROCESS_RANGE && fabs(vector_items[i].y-player.cam_focused_y())<PROCESS_RANGE){
                vector_items[i].move();
            }
        }

        //Move the effects.
        for(int i=0;i<vector_effect_water_bubble.size();i++){
            vector_effect_water_bubble[i].move();
        }
        for(int i=0;i<vector_effect_score_floater.size();i++){
            vector_effect_score_floater[i].move();
        }
    }

    //If there is at least one queued slider.
    if(sliders.size()>0){
        //Move the current slider.

        //If the slider has left the screen.
        if(!sliders[0].move()){
            //Delete this slider.
            sliders.erase(sliders.begin());
        }
    }
}

//Handle events, such as collisions.
void events(){
    if(player.game_mode==GAME_MODE_SP_ADVENTURE){
        //If the playing timer is started and a second or over.
        if(player.timer_playing.is_started() && player.timer_playing.get_ticks()>=1000){
            player.timer_playing.start();

            player.stat_seconds_playing++;
        }
        //If the traveling timer is started and a second or over.
        if(player.timer_traveling.is_started() && player.timer_traveling.get_ticks()>=1000){
            player.timer_traveling.start();

            player.stat_seconds_traveling++;
        }
        //If the menus timer is started and a second or over.
        if(player.timer_menus.is_started() && player.timer_menus.get_ticks()>=1000){
            player.timer_menus.start();

            player.stat_seconds_menus++;
        }
        //If the cow riding timer is started and a second or over.
        if(player.timer_riding_cows.is_started() && player.timer_riding_cows.get_ticks()>=1000){
            player.timer_riding_cows.start();

            player.stat_seconds_riding_cows++;
        }
    }
    else if(player.game_mode==GAME_MODE_SP_SURVIVAL || player.game_mode==GAME_MODE_MP_SURVIVAL){
        if(level.timer_survival_escape_counter.is_started() && level.timer_survival_escape_counter.get_ticks()>=1000){
            level.timer_survival_escape_counter.start();

            level.survival_seconds_played++;

            player.gain_score(SCORE_SURVIVAL_SECOND,player.x+player.w/2,player.y,false);

            bool any_players_underwater=false;

            if(player.underwater){
                any_players_underwater=true;
            }

            for(int i=0;i<mp_players.size();i++){
                if(mp_players[i].underwater){
                    any_players_underwater=true;
                    break;
                }
            }

            if(any_players_underwater){
                for(int i=0;i<SCORE_SURVIVAL_SECOND*2;i++){
                    player.lose_score();
                }
            }
        }
        if(level.timer_survival_spawn_items.is_started() && level.timer_survival_spawn_items.get_ticks()>=1000*level.survival_spawn_items_time()){
            level.timer_survival_spawn_items.start();

            for(int i=0;i<vector_spawners.size();i++){
                vector_spawners[i].spawn_object(SPAWN_ITEM);
            }
        }
        if(level.timer_survival_spawn_npcs.is_started() && level.timer_survival_spawn_npcs.get_ticks()>=1000*level.survival_spawn_npcs_time()){
            level.timer_survival_spawn_npcs.start();

            for(int i=0;i<vector_spawners.size();i++){
                vector_spawners[i].spawn_object(SPAWN_NPC);
            }
        }

        if(level.survival_seconds_played>=player.survival_seconds_to_escape){
            player.survival_escape=true;
        }
    }

    //If the playing timer is started and a second or over.
    if(level.timer_current_level.is_started() && level.timer_current_level.get_ticks()>=1000){
        level.timer_current_level.start();

        player.seconds_total_current_level++;
        level.seconds_in_current_level++;
    }

    //If the achievements update timer has expired, update the achievements.
    if(player.timer_update_achievements.is_started() && player.timer_update_achievements.get_ticks()>=1000){
        player.timer_update_achievements.start();

        profile.update_achievements();
    }

    //If the save data timer has expired, save data.
    if(player.timer_save_data.is_started() && player.timer_save_data.get_ticks()>=5000){
        player.timer_save_data.start();

        profile.save_profile_global_data();
        profile.save_level_data();
    }

    music.fadein_tracks();
    music.fadeout_tracks();

    //As long as the game is not paused and is in progress.
    if(!player.pause && player.game_in_progress){
        player.update_camera_focus();

        if(player.game_mode==GAME_MODE_SP_ADVENTURE && player.persistent_level_data){
            profile.update_fov_map();
        }

        player.update_fov_lighting();

        //Handle trigger events.
        for(int i=0;i<vector_triggers.size();i++){
            if(fabs(vector_triggers[i].x-player.cam_focused_x())<PROCESS_RANGE && fabs(vector_triggers[i].y-player.cam_focused_y())<PROCESS_RANGE){
                vector_triggers[i].handle_events();
            }
        }

        //Remove any NPCs that have faded out.
        if(player.get_npc_fade()>0){
            bool npc_erased=false;

            for(int i=0;i<vector_npcs.size();i++){
                if(vector_npcs[i].counter_fade==0){
                    npc_erased=true;

                    vector_npcs.erase(vector_npcs.begin()+i);

                    i--;
                }
                else{
                    if(!vector_npcs[i].exists && !vector_npcs[i].starts_dead){
                        vector_npcs[i].counter_fade--;
                    }
                }
            }

            if(npc_erased){
                int boss_index=-1;

                for(int i=0;i<vector_npcs.size();i++){
                    if(vector_npcs[i].BOSS){
                        boss_index=i;
                        break;
                    }
                }

                for(int i=0;i<vector_traps.size();i++){
                    if(vector_traps[i].BOSS){
                        vector_traps[i].boss_index=boss_index;
                    }
                }

                if(player.boss_index!=-1){
                    player.boss_index=boss_index;
                }
            }
        }

        //If no shots exist, then we clear the respective vector.
        if(player.existing_shots==0){
            vector_shots.clear();
        }

        //If no effects exist, then we clear the respective vector.
        if(player.existing_effects_water_splash==0){
            vector_effect_water_splash.clear();
        }
        if(player.existing_effects_water_bubble==0){
            vector_effect_water_bubble.clear();
        }
        if(player.existing_effects_score_floater==0){
            vector_effect_score_floater.clear();
        }
    }

    if(player.existing_effects_firework==0){
        vector_effect_firework.clear();
    }
}

//Handle animations.
void animation(){
    player.update_menu_background();

    //If no game is in progress.
    if(!player.game_in_progress){
        //Animate the overlays.
        player.animate_overlays();
    }

    //As long as the game is not paused and is in progress.
    if(!player.pause && player.game_in_progress){
        //Animate the player.
        //If the player is on the world map.
        if(player.on_worldmap()){
            player.worldmap_animate();

            for(int i=0;i<mp_players.size();i++){
                mp_players[i].worldmap_animate();
            }
        }
        //If the player is in a level.
        else{
            player.animate();

            for(int i=0;i<mp_players.size();i++){
                mp_players[i].animate();
            }
        }

        //Animate the NPCs.
        for(int i=0;i<vector_npcs.size();i++){
            if(fabs(vector_npcs[i].x-player.cam_focused_x())<PROCESS_RANGE && fabs(vector_npcs[i].y-player.cam_focused_y())<PROCESS_RANGE){
                vector_npcs[i].animate();
            }
        }

        //Animate the traps.
        for(int i=0;i<vector_traps.size();i++){
            if(fabs(vector_traps[i].x-player.cam_focused_x())<PROCESS_RANGE && fabs(vector_traps[i].y-player.cam_focused_y())<PROCESS_RANGE){
                vector_traps[i].animate();
            }
        }

        //Animate the shots.
        for(int i=0;i<vector_shots.size();i++){
            if(fabs(vector_shots[i].x-player.cam_focused_x())<PROCESS_RANGE && fabs(vector_shots[i].y-player.cam_focused_y())<PROCESS_RANGE){
                vector_shots[i].animate();
            }
        }

        //Animate the moving platforms.
        for(int i=0;i<vector_moving_platforms.size();i++){
            if(fabs(vector_moving_platforms[i].x-player.cam_focused_x())<PROCESS_RANGE && fabs(vector_moving_platforms[i].y-player.cam_focused_y())<PROCESS_RANGE){
                vector_moving_platforms[i].animate();
            }
        }

        //Animate the doors.
        for(int i=0;i<vector_doors.size();i++){
            if(fabs(vector_doors[i].x-player.cam_focused_x())<PROCESS_RANGE && fabs(vector_doors[i].y-player.cam_focused_y())<PROCESS_RANGE){
                vector_doors[i].animate();
            }
        }

        //Animate the springs.
        for(int i=0;i<vector_springs.size();i++){
            if(fabs(vector_springs[i].x-player.cam_focused_x())<PROCESS_RANGE && fabs(vector_springs[i].y-player.cam_focused_y())<PROCESS_RANGE){
                vector_springs[i].animate();
            }
        }

        //Animate the boosters.
        for(int i=0;i<vector_boosters.size();i++){
            if(fabs(vector_boosters[i].x-player.cam_focused_x())<PROCESS_RANGE && fabs(vector_boosters[i].y-player.cam_focused_y())<PROCESS_RANGE){
                vector_boosters[i].animate();
            }
        }

        //Animate the items.
        for(int i=0;i<vector_items.size();i++){
            if(fabs(vector_items[i].x-player.cam_focused_x())<PROCESS_RANGE && fabs(vector_items[i].y-player.cam_focused_y())<PROCESS_RANGE){
                vector_items[i].animate();
            }
        }

        //Animate the effects.
        for(int i=0;i<vector_effect_water_splash.size();i++){
            vector_effect_water_splash[i].animate();
        }
        for(int i=0;i<vector_effect_water_bubble.size();i++){
            vector_effect_water_bubble[i].animate();
        }

        //Animate the overlays.
        player.animate_overlays();
    }

    //Create any new shots.
    if(new_shots.size()>0){
        for(int i=0;i<new_shots.size();i++){
            vector_shots.push_back(Shot(new_shots[i].x,new_shots[i].y,new_shots[i].angle,new_shots[i].type,new_shots[i].counts_as_trap));
        }
    }
    //Create any new npcs.
    if(new_npcs.size()>0){
        for(int i=0;i<new_npcs.size();i++){
            vector_npcs.push_back(Npc(new_npcs[i].x,new_npcs[i].y,new_npcs[i].type));
        }
    }
}

void camera(int frame_rate,double ms_per_frame,int logic_frame_rate){
    if(player.game_in_progress){
        //Handle camera movements for the player.
        player.set_camera();
        player.update_background();
    }

    player.update_window_caption(frame_rate,ms_per_frame,logic_frame_rate);
}

void render(int frame_rate,double ms_per_frame,int logic_frame_rate){
    if(player.option_renderer==RENDERER_HARDWARE){
        if(main_window.fbo_mode){
            glPushAttrib(GL_VIEWPORT_BIT);
            glViewport(0,0,main_window.SCREEN_WIDTH,main_window.SCREEN_HEIGHT);
            glBindTexture(GL_TEXTURE_2D,0);
            image.current_texture=0;
            glBindFramebuffer(GL_FRAMEBUFFER,main_window.fbo);
        }

        //As long as the current level is not DevWorld.
        if(player.current_level!=2){
            glClear(GL_COLOR_BUFFER_BIT);
        }
    }
    else if(player.option_renderer==RENDERER_SOFTWARE){
        //As long as the current level is not DevWorld.
        if(player.current_level!=2){
            ///render_rectangle(0,0,main_window.SCREEN_WIDTH,main_window.SCREEN_HEIGHT,1.0,COLOR_BLACK);
        }
    }

    player.render_background();

    //As long as the game is in progress.
    if(player.game_in_progress){
        //Set the camera's current tile position.
        int camera_current_x=(int)((int)player.camera_x/TILE_SIZE);
        int camera_current_y=(int)((int)player.camera_y/TILE_SIZE);

        //Check all tiles in a rectangle around the player.
        int check_x_start=camera_current_x;
        int check_x_end=camera_current_x+(int)((int)player.camera_w/TILE_SIZE)+2;
        int check_y_start=camera_current_y;
        int check_y_end=camera_current_y+(int)((int)player.camera_h/TILE_SIZE)+2;

        //Render each onscreen mirror tile.
        for(int int_y=check_y_start;int_y<check_y_end;int_y++){
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
        if(!player.on_worldmap()){
            player.render(MIRRORED);

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
        }

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

        for(int i=0;i<vector_signs.size();i++){
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

        level.amount_replicators=0;
        for(int i=0;i<vector_npcs.size();i++){
            vector_npcs[i].render();

            if(vector_npcs[i].exists && vector_npcs[i].type==NPC_REPLICATOR){
                level.amount_replicators++;
            }
        }

        //Render the shots.
        for(int i=0;i<vector_shots.size();i++){
            vector_shots[i].render();
        }

        //Render each player held in the player vector in sequence.
        //If the player is on the world map.
        if(player.on_worldmap()){
            player.worldmap_render();

            for(int i=0;i<mp_players.size();i++){
                mp_players[i].worldmap_render();
            }
        }
        //If the player is in a level.
        else{
            //If the player is alive, render him in the normal place.
            //If he is dying, he will instead be rendered on top of the foreground tiles.
            if(!player.DYING && !player.bubble_mode){
                player.render();
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
        if(player.DYING || player.bubble_mode){
            player.render();
        }

        if(!player.on_worldmap()){
            for(int i=0;i<mp_players.size();i++){
                if(mp_players[i].DYING || mp_players[i].bubble_mode){
                    mp_players[i].render();
                }
                else{
                    if(!player.hide_gui && player.option_dev && player.show_paths){
                        mp_players[i].render_path();
                    }
                }
            }
        }

        if(player.on_worldmap()){
            for(int i=0;i<vector_effect_firework.size();i++){
                vector_effect_firework[i].render();
            }
        }

        player.render_foreground();

        if(!player.hide_gui){
            for(int i=0;i<vector_effect_score_floater.size();i++){
                vector_effect_score_floater[i].render();
            }
        }

        if(!player.hide_gui && player.option_dev && player.show_tracers){
            player.tracer_manager.render();
        }

        level.render_title();

        if(player.fadeout){
            render_rectangle(0,0,main_window.SCREEN_WIDTH,main_window.SCREEN_HEIGHT,player.fade_opacity,COLOR_BLACK);
        }

        if(player.DYING){
            if(player.game_mode==GAME_MODE_SP_ADVENTURE){
                ss.clear();ss.str("");ss<<"Death #";ss<<player.stat_deaths_enemies+player.stat_deaths_traps+player.stat_deaths_drowning;msg=ss.str();
                font_large.show((main_window.SCREEN_WIDTH-msg.length()*font_large.spacing_x*2.0)/2.0+2,(main_window.SCREEN_HEIGHT-font_large.spacing_y*2.0)/2.0+2,msg,COLOR_BLACK,1.0,2.0);
                font_large.show((main_window.SCREEN_WIDTH-msg.length()*font_large.spacing_x*2.0)/2.0,(main_window.SCREEN_HEIGHT-font_large.spacing_y*2.0)/2.0,msg,return_gui_color(holiday,3),1.0,2.0);
            }
        }

        //If the game is paused.
        if(!player.hide_gui && player.pause && window_manager.which_window_open()==-1 && !level.return_title_pause()){
            render_rectangle((main_window.SCREEN_WIDTH-160)/2,(main_window.SCREEN_HEIGHT-85)/2,160,85,1.0,return_gui_color(holiday,0));
            render_rectangle((main_window.SCREEN_WIDTH-160)/2+2,(main_window.SCREEN_HEIGHT-85)/2+2,160-4,85-4,1.0,return_gui_color(holiday,1));
            font.show((main_window.SCREEN_WIDTH-71)/2+2,(main_window.SCREEN_HEIGHT-16)/2+2,"Paused",COLOR_BLACK);
            font.show((main_window.SCREEN_WIDTH-71)/2,(main_window.SCREEN_HEIGHT-16)/2,"Paused",return_gui_color(holiday,3));
        }

        if(!player.hide_gui && !level.return_title_pause() && ((player.pause && window_manager.which_window_open()==-1) || (window_manager.which_window_open()!=-1 && window_manager.which_window_open()!=WHICH_WINDOW_MESSAGE))){
            player.render_menu_pretties();
        }
    }

    if(!player.hide_gui){
        if((player.game_in_progress && !level.return_title_pause() && ((player.pause && window_manager.which_window_open()==-1) || (window_manager.which_window_open()!=-1 && window_manager.which_window_open()!=WHICH_WINDOW_MESSAGE))) ||
           (!player.game_in_progress && player.game_beginning_cutscene==0)){
            //Display the game version number.
            ss.clear();ss.str("");ss<<"Version: ";ss<<Version::MAJOR;ss<<".";ss<<Version::MINOR;ss<<".";ss<<Version::MICRO;ss<<" ";ss<<Version::STATUS;ss<<"\xA";msg=ss.str();
            font.show(0+2,575+2,msg,COLOR_BLACK);
            font.show(0,575,msg,return_gui_color(holiday,3));
        }
    }

    if(!player.hide_gui && player.game_in_progress){
        if(!player.game_mode_is_multiplayer()){
            render_sprite(15-5+2,10+2,image.sprite_sheet_items,&sprites_item_ammo[0],1.0,1.0,1.0,0.0,COLOR_BLACK);
            render_sprite(15-5,10,image.sprite_sheet_items,&sprites_item_ammo[0],1.0,1.0,1.0);
            ss.clear();ss.str("");ss<<"x ";ss<<player.ammo;msg=ss.str();
            font.show(25+24-1+2,20+2,msg,COLOR_BLACK);
            font.show(25+24-1,20,msg,return_gui_color(holiday,3));
        }
        else{
            for(int i=-1;i<(int)mp_players.size();i++){
                uint64_t PLAYER_AMMO=player.ammo;
                short PLAYER_CURRENT_SHOT=player.current_shot;
                bool PLAYER_SHOT_HOMING=player.shot_homing;
                if(i>=0){
                    PLAYER_AMMO=mp_players[i].ammo;
                    PLAYER_CURRENT_SHOT=mp_players[i].current_shot;
                    PLAYER_SHOT_HOMING=mp_players[i].shot_homing;
                }

                render_sprite(215-5+((i+1)*100)+2,10+2,image.sprite_sheet_items,&sprites_item_ammo[0],1.0,1.0,1.0,0.0,COLOR_BLACK);
                render_sprite(215-5+((i+1)*100),10,image.sprite_sheet_items,&sprites_item_ammo[0],1.0,1.0,1.0);
                ss.clear();ss.str("");ss<<"x ";ss<<PLAYER_AMMO;msg=ss.str();
                font.show(225+24-1+((i+1)*100)+2,20+2,msg,COLOR_BLACK);
                font.show(225+24-1+((i+1)*100),20,msg,return_gui_color(holiday,3));

                if(!player.on_worldmap() && PLAYER_SHOT_HOMING){
                    render_texture(225+((i+1)*100)+2,10+4+32+2,image.ammo_box,1.0,COLOR_BLACK);
                    render_texture(225+((i+1)*100),10+4+32,image.ammo_box);
                    if(PLAYER_CURRENT_SHOT==SHOT_PLAYER){
                        render_sprite(233+((i+1)*100)+2,18+4+32+2,image.shot_types,&sprites_shot_types[0],1.0,1.0,1.0,0.0,COLOR_BLACK);
                        render_sprite(233+((i+1)*100),18+4+32,image.shot_types,&sprites_shot_types[0]);
                    }
                    else if(PLAYER_CURRENT_SHOT==SHOT_PLAYER_SPLODE){
                        render_sprite(233+((i+1)*100)+2,18+4+32+2,image.shot_types,&sprites_shot_types[1],1.0,1.0,1.0,0.0,COLOR_BLACK);
                        render_sprite(233+((i+1)*100),18+4+32,image.shot_types,&sprites_shot_types[1]);
                    }
                    else if(PLAYER_CURRENT_SHOT==SHOT_PLAYER_HOMING){
                        render_sprite(233+((i+1)*100)+2,18+4+32+2,image.shot_types,&sprites_shot_types[2],1.0,1.0,1.0,0.0,COLOR_BLACK);
                        render_sprite(233+((i+1)*100),18+4+32,image.shot_types,&sprites_shot_types[2]);
                    }
                }
            }
        }

        if(player.option_fps){
            ss.clear();ss.str("");ss<<"FPS: ";ss<<frame_rate;msg=ss.str();
            font.show(25-7+2,60+2,msg,COLOR_BLACK);
            font.show(25-7,60,msg,return_gui_color(holiday,3));
        }

        if(!player.on_worldmap()){
            if(!player.game_mode_is_multiplayer()){
                int player_box_offset=-32;

                if(player.shot_homing){
                    player_box_offset=0;

                    render_texture(344+2,10+2,image.ammo_box,1.0,COLOR_BLACK);
                    render_texture(344,10,image.ammo_box);
                    if(player.current_shot==SHOT_PLAYER){
                        render_sprite(352+2,18+2,image.shot_types,&sprites_shot_types[0],1.0,1.0,1.0,0.0,COLOR_BLACK);
                        render_sprite(352,18,image.shot_types,&sprites_shot_types[0]);
                    }
                    else if(player.current_shot==SHOT_PLAYER_SPLODE){
                        render_sprite(352+2,18+2,image.shot_types,&sprites_shot_types[1],1.0,1.0,1.0,0.0,COLOR_BLACK);
                        render_sprite(352,18,image.shot_types,&sprites_shot_types[1]);
                    }
                    else if(player.current_shot==SHOT_PLAYER_HOMING){
                        render_sprite(352+2,18+2,image.shot_types,&sprites_shot_types[2],1.0,1.0,1.0,0.0,COLOR_BLACK);
                        render_sprite(352,18,image.shot_types,&sprites_shot_types[2]);
                    }
                }

                render_texture(344+48+ITEM_W+player_box_offset+2,10+2,image.ammo_box,1.0,COLOR_BLACK);
                render_texture(344+48+ITEM_W+player_box_offset,10,image.ammo_box);
                render_texture(352+48+ITEM_W+7+player_box_offset+2,18+2,*player.return_character_box_image(),1.0,COLOR_BLACK);
                render_texture(352+48+ITEM_W+7+player_box_offset,18,*player.return_character_box_image());
            }
            else{
                if(!player.pause && player.all_humans_dead()){
                    string tooltip_text="";

                    if(player.game_mode==GAME_MODE_MP_ADVENTURE){
                        tooltip_text="Player 1: Press 'shoot' to force a respawn.";
                    }
                    else if(player.game_mode==GAME_MODE_MP_SURVIVAL){
                        tooltip_text="Player 1: Press 'shoot' to end the game.";
                    }

                    tooltip_hint.setup(tooltip_text,(player.camera_w-tooltip_text.length()*12)/2.0,40);
                }
            }

            if(player.game_mode==GAME_MODE_SP_ADVENTURE || player.game_mode==GAME_MODE_MP_ADVENTURE){
                render_sprite(650+2,30+2,image.sprite_sheet_items,&sprites_item_leaf[0],1.0,1.0,1.0,0.0,COLOR_BLACK);
                render_sprite(650,30,image.sprite_sheet_items,&sprites_item_leaf[0]);
                ss.clear();ss.str("");ss<<"x ";ss<<player.current_level_leaves;ss<<"/";ss<<level.leaves;msg=ss.str();
                font.show(650+ITEM_W+2+2,34+2,msg,COLOR_BLACK);
                font.show(650+ITEM_W+2,34,msg,return_gui_color(holiday,3));

                render_sprite(650+2,50+2,image.sprite_sheet_items,&sprites_item_cheese[0],1.0,0.25,0.25,0.0,COLOR_BLACK);
                render_sprite(650,50,image.sprite_sheet_items,&sprites_item_cheese[0],1.0,0.25,0.25);
                ss.clear();ss.str("");ss<<"x ";ss<<player.current_level_cheese;ss<<"/";ss<<level.cheese;msg=ss.str();
                font.show(650+ITEM_W+2+2,54+2,msg,COLOR_BLACK);
                font.show(650+ITEM_W+2,54,msg,return_gui_color(holiday,3));
            }
            else if(player.game_mode==GAME_MODE_SP_SURVIVAL){
                string time_string=get_time_string_short(level.survival_seconds_played);
                font.show((main_window.SCREEN_WIDTH-time_string.length()*font.spacing_x)/2.0+2,64+2,time_string,COLOR_BLACK);
                font.show((main_window.SCREEN_WIDTH-time_string.length()*font.spacing_x)/2.0,64,time_string,return_gui_color(holiday,3));
            }
            else if(player.game_mode==GAME_MODE_MP_SURVIVAL){
                string time_string=get_time_string_short(level.survival_seconds_played);
                font.show(20+2,10+2,time_string,COLOR_BLACK);
                font.show(20,10,time_string,return_gui_color(holiday,3));
            }

            //If a boss battle is in progress, show the boss's health bar.
            if(player.boss_index!=-1){
                double health_percentage=(double)vector_npcs[player.boss_index].health/(double)vector_npcs[player.boss_index].health_max;
                double health_bar_width=240.0*health_percentage;
                double health_max_bar_width=240.0*1.0;
                string boss_text=player.num_to_string(vector_npcs[player.boss_index].health)+"/"+player.num_to_string(vector_npcs[player.boss_index].health_max);

                render_rectangle(main_window.SCREEN_WIDTH/2.0-120-2,main_window.SCREEN_HEIGHT-75-2,health_max_bar_width+4,30+4,1.0,COLOR_BLACK);
                render_rectangle(main_window.SCREEN_WIDTH/2.0-120,main_window.SCREEN_HEIGHT-75,health_bar_width,30,1.0,COLOR_RED);

                font.show((main_window.SCREEN_WIDTH-boss_text.length()*font.spacing_x)/2.0+2,main_window.SCREEN_HEIGHT-68+2,boss_text,COLOR_BLACK);
                font.show((main_window.SCREEN_WIDTH-boss_text.length()*font.spacing_x)/2.0,main_window.SCREEN_HEIGHT-68,boss_text,COLOR_WHITE);
            }

            if(!player.game_mode_is_multiplayer()){
                //If the player has less than max oxygen, show the oxygen meter.
                if(player.oxygen<player.oxygen_max_capacity){
                    double oxygen_percentage=(double)player.oxygen/(double)player.oxygen_max_capacity;
                    double oxygen_bar_width=120.0*oxygen_percentage;
                    double oxygen_max_bar_width=120.0*1.0;

                    render_rectangle(main_window.SCREEN_WIDTH/2.0-60-2,75-2,oxygen_max_bar_width+4,30+4,1.0,COLOR_BLACK);
                    render_rectangle(main_window.SCREEN_WIDTH/2.0-60,75,oxygen_bar_width,30,1.0,COLOR_BLUE_OCEAN);
                }
            }
            else{
                for(int i=-1;i<(int)mp_players.size();i++){
                    short PLAYER_OXYGEN=player.oxygen;
                    short PLAYER_OXYGEN_MAX_CAPACITY=player.oxygen_max_capacity;
                    bool PLAYER_DYING=player.DYING;
                    bool PLAYER_BUBBLE_MODE=player.bubble_mode;
                    if(i>=0){
                        PLAYER_OXYGEN=mp_players[i].oxygen;
                        PLAYER_OXYGEN_MAX_CAPACITY=mp_players[i].oxygen_max_capacity;
                        PLAYER_DYING=mp_players[i].DYING;
                        PLAYER_BUBBLE_MODE=mp_players[i].bubble_mode;
                    }

                    if(!PLAYER_DYING && !PLAYER_BUBBLE_MODE && PLAYER_OXYGEN<PLAYER_OXYGEN_MAX_CAPACITY){
                        double oxygen_percentage=(double)PLAYER_OXYGEN/(double)PLAYER_OXYGEN_MAX_CAPACITY;
                        double oxygen_bar_width=60.0*oxygen_percentage;
                        double oxygen_max_bar_width=60.0*1.0;

                        render_rectangle(220+((i+1)*100)-2,96-2,oxygen_max_bar_width+4,15+4,1.0,COLOR_BLACK);
                        render_rectangle(220+((i+1)*100),96,oxygen_bar_width,15,1.0,COLOR_BLUE_OCEAN);
                    }
                }
            }
        }
        else{
            if(!player.game_mode_is_multiplayer()){
                render_texture(376+2,10+2,image.ammo_box,1.0,COLOR_BLACK);
                render_texture(376,10,image.ammo_box);
                render_texture(384+2+7,18+2,*player.return_character_box_image(),1.0,COLOR_BLACK);
                render_texture(384+7,18,*player.return_character_box_image());
            }

            tooltip_levelname.render();
        }

        tooltip_hint.render();

        short candy_color=COLOR_RED;
        if(holiday!=HOLIDAY_NONE){
            candy_color=return_holiday_color(holiday);
        }
        uint64_t which_score=player.score;
        if(player.game_mode==GAME_MODE_SP_ADVENTURE || player.game_mode==GAME_MODE_MP_ADVENTURE){
            which_score=player.score;
        }
        else if(player.game_mode==GAME_MODE_SP_SURVIVAL || player.game_mode==GAME_MODE_MP_SURVIVAL){
            which_score=player.survival_score;
        }
        render_sprite(650+2,10+2,image.sprite_sheet_items,&sprites_item_candy[0],1.0,1.0,1.0,0.0,COLOR_BLACK);
        render_sprite(650,10,image.sprite_sheet_items,&sprites_item_candy[0],1.0,1.0,1.0,0.0,candy_color);
        ss.clear();ss.str("");ss<<"x ";ss<<which_score;msg=ss.str();
        font.show(650+ITEM_W+2+2,14+2,msg,COLOR_BLACK);
        font.show(650+ITEM_W+2,14,msg,return_gui_color(holiday,3));
    }

    if(!player.hide_gui){
        for(int i=0;i<hud_buttons.size();i++){
            hud_buttons[i].render(0,0,-2);
        }

        window_inventory[0].render();

        window_map[0].render();

        window_message[0].render();

        window_setup_survival[0].render();

        window_shop[0].render();

        window_upgrades[0].render();

        for(int i=0;i<windows_level_info.size();i++){
            windows_level_info[i].render();
        }

        for(int i=0;i<vector_windows.size();i++){
            vector_windows[i].render();
        }

        //If there is at least one queued slider.
        if(sliders.size()>0){
            //Display the current slider.
            sliders[0].render();
        }

        tooltip.render();
    }

    /**ss.clear();ss.str("");ss<<"Number of joysticks: ";ss<<SDL_NumJoysticks();ss<<"\xA";msg=ss.str();
    if(SDL_NumJoysticks()>0 && SDL_JoystickOpened(0)){
        ss.clear();ss.str("");ss<<"Joystick 0's name: ";ss<<SDL_JoystickName(0);ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Number of axes: ";ss<<SDL_JoystickNumAxes(joystick[0].joy);ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Number of buttons: ";ss<<SDL_JoystickNumButtons(joystick[0].joy);ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Number of balls: ";ss<<SDL_JoystickNumBalls(joystick[0].joy);ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Number of hats: ";ss<<SDL_JoystickNumHats(joystick[0].joy);ss<<"\xA";msg+=ss.str();
    }
    font.show(0,0,msg,COLOR_RED);*/

    //Display development stuff.
    if(player.option_dev){
        ///ss.clear();ss.str("");ss<<player.boss_index;ss<<"\xA";msg=ss.str();
        ///ss.clear();ss.str("");ss<<compare_versions("1.0","1.1");ss<<"\xA";msg=ss.str();
        ///font.show(450,0,msg,COLOR_RED);
    }

    //Render the software mouse cursor.
    if(!player.option_hardware_cursor && (window_manager.which_window_open()!=-1 || player.cam_state!=CAM_STICKY)){
        int mouse_x=0;
        int mouse_y=0;
        SDL_GetMouseState(&mouse_x,&mouse_y);
        mouse_x*=(double)((double)main_window.SCREEN_WIDTH/(double)main_window.REAL_SCREEN_WIDTH);
        mouse_y*=(double)((double)main_window.SCREEN_HEIGHT/(double)main_window.REAL_SCREEN_HEIGHT);
        render_texture(mouse_x,mouse_y,image.cursor);
    }

    if(player.option_renderer==RENDERER_HARDWARE){
        if(main_window.fbo_mode){
            glBindFramebuffer(GL_FRAMEBUFFER,0);
            glClear(GL_COLOR_BUFFER_BIT);
            glPopAttrib();
            render_fbo_texture();
        }

        //Swap the buffers, updating the screen.
        SDL_GL_SwapBuffers();
    }
    else if(player.option_renderer==RENDERER_SOFTWARE){
        SDL_Flip(main_window.screen);
    }
}

void render_loading_screen(double percentage,int level_loading){
    if(player.option_renderer==RENDERER_HARDWARE){
        if(main_window.fbo_mode){
            glPushAttrib(GL_VIEWPORT_BIT);
            glViewport(0,0,main_window.SCREEN_WIDTH,main_window.SCREEN_HEIGHT);
            glBindTexture(GL_TEXTURE_2D,0);
            image.current_texture=0;
            glBindFramebuffer(GL_FRAMEBUFFER,main_window.fbo);
        }
        glClear(GL_COLOR_BUFFER_BIT);
    }
    else if(player.option_renderer==RENDERER_SOFTWARE){
        render_rectangle(0,0,main_window.SCREEN_WIDTH,main_window.SCREEN_HEIGHT,1.0,COLOR_BLACK);
    }

    render_texture(0,0,image.loading_screen_main,0.25);
    render_texture((main_window.SCREEN_WIDTH-image.logo_hubert.w)/2.0,30,image.logo_hubert);

    double bar_width=240.0*percentage;
    double max_bar_width=240.0*1.0;
    render_rectangle(main_window.SCREEN_WIDTH/2.0-120-2,main_window.SCREEN_HEIGHT-75-2,max_bar_width+4,30+4,1.0,COLOR_BLACK);
    render_rectangle(main_window.SCREEN_WIDTH/2.0-120,main_window.SCREEN_HEIGHT-75,bar_width,30,1.0,return_gui_color(holiday,5));

    ss.clear();ss.str("");ss<<(int)(percentage*100.0);ss<<"%";msg=ss.str();
    font.show(main_window.SCREEN_WIDTH/2.0-120+(max_bar_width-msg.length()*font.spacing_x)/2.0+2,main_window.SCREEN_HEIGHT-75+font.spacing_y/4.0+2,msg,COLOR_BLACK);
    font.show(main_window.SCREEN_WIDTH/2.0-120+(max_bar_width-msg.length()*font.spacing_x)/2.0,main_window.SCREEN_HEIGHT-75+font.spacing_y/4.0,msg,return_gui_color(holiday,6));

    if(player.option_renderer==RENDERER_HARDWARE){
        if(main_window.fbo_mode){
            glBindFramebuffer(GL_FRAMEBUFFER,0);
            glClear(GL_COLOR_BUFFER_BIT);
            glPopAttrib();
            render_fbo_texture();
        }

        //Swap the buffers, updating the screen.
        SDL_GL_SwapBuffers();
    }
    else if(player.option_renderer==RENDERER_SOFTWARE){
        SDL_Flip(main_window.screen);
    }
}
