/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "profile.h"
#include "player.h"
#include "world.h"
#include "render.h"

using namespace std;

void Profile::update_fov_map(){
    fov_radius_map=10;

    if(player.new_game_plus>0){
        fov_radius_map=15;
    }

    //The current tile location for the player.
    int player_current_x=(int)((int)player.x/TILE_SIZE);
    int player_current_y=(int)((int)player.y/TILE_SIZE);

    if(player_current_x>=0 && player_current_x<=(level.level_x/TILE_SIZE)-1 && player_current_y>=0 && player_current_y<=(level.level_y/TILE_SIZE)-1){
        fov_circle(&fov_settings_map,&level,NULL,player_current_x,player_current_y,fov_radius_map);

        //Reveal the player's own space.
        current_level_map_data[player_current_x][player_current_y]=true;
    }
}

void Player::update_fov_lighting(){
    if((on_worldmap() && option_time_of_day) || (!on_worldmap() && option_time_of_day_levels)){
        int LIGHTING_RANGE=1120;

        //The current tile location for the player.
        int player_current_x=(int)(((int)x+(int)((int)w/2.0))/LIGHTING_TILE_SIZE);
        int player_current_y=(int)(((int)y+(int)((int)h/2.0))/LIGHTING_TILE_SIZE);

        for(int int_x=player_current_x-LIGHTING_RANGE/LIGHTING_TILE_SIZE;int_x<player_current_x+LIGHTING_RANGE/LIGHTING_TILE_SIZE;int_x++){
            for(int int_y=player_current_y-LIGHTING_RANGE/LIGHTING_TILE_SIZE;int_y<player_current_y+LIGHTING_RANGE/LIGHTING_TILE_SIZE;int_y++){
                if(int_x>=0 && int_x<=(level.level_x/LIGHTING_TILE_SIZE)-1 && int_y>=0 && int_y<=(level.level_y/LIGHTING_TILE_SIZE)-1){
                    light_data[int_x][int_y].seen=false;
                    light_data[int_x][int_y].color.red=-1.0;
                    light_data[int_x][int_y].color.green=-1.0;
                    light_data[int_x][int_y].color.blue=-1.0;
                    light_data[int_x][int_y].light_intensity=-1.0;
                    light_data[int_x][int_y].dimness=-1.0;
                }
            }
        }

        if(light_source.on){
            if(player_current_x>=0 && player_current_x<=(level.level_x/LIGHTING_TILE_SIZE)-1 && player_current_y>=0 && player_current_y<=(level.level_y/LIGHTING_TILE_SIZE)-1){
                if(bubble_mode){
                    light_source.color=color_name_to_doubles(get_player_color(which_mp_player));
                }
                else{
                    light_source.color=color_name_to_doubles(COLOR_WHITE);
                }

                light_source.x=player_current_x;
                light_source.y=player_current_y;

                fov_circle(&fov_settings_lighting,&level,&light_source,player_current_x,player_current_y,light_source.radius);

                //Reveal the player's own space.
                level.setSeen_lighting(player_current_x,player_current_y,&light_source);
            }
        }

        for(int i=0;i<mp_players.size();i++){
            if(mp_players[i].light_source.on){
                int mp_player_current_x=(int)(((int)mp_players[i].x+(int)((int)mp_players[i].w/2.0))/LIGHTING_TILE_SIZE);
                int mp_player_current_y=(int)(((int)mp_players[i].y+(int)((int)mp_players[i].h/2.0))/LIGHTING_TILE_SIZE);

                if(mp_player_current_x>=0 && mp_player_current_x<=(level.level_x/LIGHTING_TILE_SIZE)-1 && mp_player_current_y>=0 && mp_player_current_y<=(level.level_y/LIGHTING_TILE_SIZE)-1){
                    if(mp_players[i].bubble_mode){
                        mp_players[i].light_source.color=color_name_to_doubles(get_player_color(mp_players[i].which_mp_player));
                    }
                    else{
                        mp_players[i].light_source.color=color_name_to_doubles(COLOR_WHITE);
                    }

                    mp_players[i].light_source.x=mp_player_current_x;
                    mp_players[i].light_source.y=mp_player_current_y;

                    fov_circle(&fov_settings_lighting,&level,&mp_players[i].light_source,mp_player_current_x,mp_player_current_y,mp_players[i].light_source.radius);

                    //Reveal the player's own space.
                    level.setSeen_lighting(mp_player_current_x,mp_player_current_y,&mp_players[i].light_source);
                }
            }
        }

        int tile_current_x=(int)(((int)player.x+(int)((int)player.w/2.0))/TILE_SIZE);
        int tile_current_y=(int)(((int)player.y+(int)((int)player.h/2.0))/TILE_SIZE);

        for(int int_x=tile_current_x-LIGHTING_RANGE/TILE_SIZE;int_x<tile_current_x+LIGHTING_RANGE/TILE_SIZE;int_x++){
            for(int int_y=tile_current_y-LIGHTING_RANGE/TILE_SIZE;int_y<tile_current_y+LIGHTING_RANGE/TILE_SIZE;int_y++){
                if(int_x>=0 && int_x<=(level.level_x/TILE_SIZE)-1 && int_y>=0 && int_y<=(level.level_y/TILE_SIZE)-1){
                    level.tile_array[int_x][int_y].process_glow();

                    if(level.tile_array[int_x][int_y].light_source.on){
                        //The current tile location for the object.
                        int current_x=(int)(((int)level.tile_array[int_x][int_y].x+(int)((int)TILE_SIZE/2.0))/LIGHTING_TILE_SIZE);
                        int current_y=(int)(((int)level.tile_array[int_x][int_y].y+(int)((int)TILE_SIZE/2.0))/LIGHTING_TILE_SIZE);

                        if(current_x>=0 && current_x<=(level.level_x/LIGHTING_TILE_SIZE)-1 && current_y>=0 && current_y<=(level.level_y/LIGHTING_TILE_SIZE)-1){
                            level.tile_array[int_x][int_y].light_source.x=current_x;
                            level.tile_array[int_x][int_y].light_source.y=current_y;

                            fov_circle(&fov_settings_lighting,&level,&level.tile_array[int_x][int_y].light_source,current_x,current_y,level.tile_array[int_x][int_y].light_source.radius);

                            //Reveal the object's own space.
                            level.setSeen_lighting(current_x,current_y,&level.tile_array[int_x][int_y].light_source);
                        }
                    }
                }
            }
        }

        for(int i=0;i<vector_doors.size();i++){
            if(!vector_doors[i].open){
                if(fabs(vector_doors[i].x-x)<LIGHTING_RANGE && fabs(vector_doors[i].y-y)<LIGHTING_RANGE){
                    if(vector_doors[i].light_source.on){
                        //The current tile location for the object.
                        int current_x=(int)(((int)vector_doors[i].x+(int)((int)vector_doors[i].w/2.0))/LIGHTING_TILE_SIZE);
                        int current_y=(int)(((int)vector_doors[i].y+(int)((int)vector_doors[i].h/2.0))/LIGHTING_TILE_SIZE);

                        if(current_x>=0 && current_x<=(level.level_x/LIGHTING_TILE_SIZE)-1 && current_y>=0 && current_y<=(level.level_y/LIGHTING_TILE_SIZE)-1){
                            vector_doors[i].light_source.x=current_x;
                            vector_doors[i].light_source.y=current_y;

                            fov_circle(&fov_settings_lighting,&level,&vector_doors[i].light_source,current_x,current_y,vector_doors[i].light_source.radius);

                            //Reveal the object's own space.
                            level.setSeen_lighting(current_x,current_y,&vector_doors[i].light_source);
                        }
                    }
                }
            }
        }

        for(int i=0;i<vector_npcs.size();i++){
            if(vector_npcs[i].exists){
                if(fabs(vector_npcs[i].x-x)<LIGHTING_RANGE && fabs(vector_npcs[i].y-y)<LIGHTING_RANGE){
                    if(vector_npcs[i].light_source.on){
                        //The current tile location for the object.
                        int current_x=(int)(((int)vector_npcs[i].x+(int)((int)vector_npcs[i].w/2.0))/LIGHTING_TILE_SIZE);
                        int current_y=(int)(((int)vector_npcs[i].y+(int)((int)vector_npcs[i].h/2.0))/LIGHTING_TILE_SIZE);

                        if(current_x>=0 && current_x<=(level.level_x/LIGHTING_TILE_SIZE)-1 && current_y>=0 && current_y<=(level.level_y/LIGHTING_TILE_SIZE)-1){
                            vector_npcs[i].light_source.x=current_x;
                            vector_npcs[i].light_source.y=current_y;

                            fov_circle(&fov_settings_lighting,&level,&vector_npcs[i].light_source,current_x,current_y,vector_npcs[i].light_source.radius);

                            //Reveal the object's own space.
                            level.setSeen_lighting(current_x,current_y,&vector_npcs[i].light_source);
                        }
                    }
                }
            }
        }

        for(int i=0;i<vector_shots.size();i++){
            if(vector_shots[i].exists){
                if(fabs(vector_shots[i].x-x)<LIGHTING_RANGE && fabs(vector_shots[i].y-y)<LIGHTING_RANGE){
                    if(vector_shots[i].light_source.on){
                        //The current tile location for the object.
                        int current_x=(int)(((int)vector_shots[i].x+(int)((int)vector_shots[i].w/2.0))/LIGHTING_TILE_SIZE);
                        int current_y=(int)(((int)vector_shots[i].y+(int)((int)vector_shots[i].h/2.0))/LIGHTING_TILE_SIZE);

                        if(current_x>=0 && current_x<=(level.level_x/LIGHTING_TILE_SIZE)-1 && current_y>=0 && current_y<=(level.level_y/LIGHTING_TILE_SIZE)-1){
                            vector_shots[i].light_source.x=current_x;
                            vector_shots[i].light_source.y=current_y;

                            fov_circle(&fov_settings_lighting,&level,&vector_shots[i].light_source,current_x,current_y,vector_shots[i].light_source.radius);

                            //Reveal the object's own space.
                            level.setSeen_lighting(current_x,current_y,&vector_shots[i].light_source);
                        }
                    }
                }
            }
        }

        for(int i=0;i<vector_traps.size();i++){
            if(vector_traps[i].active){
                if(fabs(vector_traps[i].x-x)<LIGHTING_RANGE && fabs(vector_traps[i].y-y)<LIGHTING_RANGE){
                    if(vector_traps[i].light_source.on){
                        //The current tile location for the object.
                        int current_x=(int)(((int)vector_traps[i].x+(int)((int)vector_traps[i].w/2.0))/LIGHTING_TILE_SIZE);
                        int current_y=(int)(((int)vector_traps[i].y+(int)((int)vector_traps[i].h/2.0))/LIGHTING_TILE_SIZE);

                        if(current_x>=0 && current_x<=(level.level_x/LIGHTING_TILE_SIZE)-1 && current_y>=0 && current_y<=(level.level_y/LIGHTING_TILE_SIZE)-1){
                            vector_traps[i].light_source.x=current_x;
                            vector_traps[i].light_source.y=current_y;

                            fov_circle(&fov_settings_lighting,&level,&vector_traps[i].light_source,current_x,current_y,vector_traps[i].light_source.radius);

                            //Reveal the object's own space.
                            level.setSeen_lighting(current_x,current_y,&vector_traps[i].light_source);
                        }
                    }
                }
            }
        }

        for(int i=0;i<vector_effect_firework.size();i++){
            if(vector_effect_firework[i].exists){
                if(fabs(vector_effect_firework[i].x-x)<LIGHTING_RANGE && fabs(vector_effect_firework[i].y-y)<LIGHTING_RANGE){
                    if(vector_effect_firework[i].light_source.on){
                        //The current tile location for the object.
                        int current_x=(int)(((int)vector_effect_firework[i].x+(int)((int)4/2.0))/LIGHTING_TILE_SIZE);
                        int current_y=(int)(((int)vector_effect_firework[i].y+(int)((int)4/2.0))/LIGHTING_TILE_SIZE);

                        if(current_x>=0 && current_x<=(level.level_x/LIGHTING_TILE_SIZE)-1 && current_y>=0 && current_y<=(level.level_y/LIGHTING_TILE_SIZE)-1){
                            vector_effect_firework[i].light_source.x=current_x;
                            vector_effect_firework[i].light_source.y=current_y;

                            fov_circle(&fov_settings_lighting,&level,&vector_effect_firework[i].light_source,current_x,current_y,vector_effect_firework[i].light_source.radius);

                            //Reveal the object's own space.
                            level.setSeen_lighting(current_x,current_y,&vector_effect_firework[i].light_source);
                        }
                    }
                }
            }
        }
    }
}
