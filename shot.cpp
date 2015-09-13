/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "shot.h"
#include "render.h"
#include "world.h"
#include "collision.h"
#include "positional_audio.h"
#include "distance.h"
#include "mirror.h"
#include "score.h"

using namespace std;

Shot::Shot(double get_x,double get_y,double get_angle,short get_type,bool get_counts_as_trap){
    //Variables:

    //The direction of the shot's movement is given at its time of creation.
    angle=get_angle;

    //Starting coordinates must be given.
    x=get_x;
    y=get_y;

    type=get_type;

    counts_as_trap=get_counts_as_trap;

    BOSS=false;

    hitbox_size_modifier=-4;

    homing_chance=100;
    homing_stop_chance=0;

    home_on_player=false;
    home_on_npcs=false;

    deadly_water_proof=false;

    kills_npc=true;
    kills_player=true;

    does_damage=true;

    player_shot=false;

    //Gravity:
    gravity_affects=false;
    gravity_max=8.0;
    gravity=0.5;
    air_velocity=0.0;

    render_rotated=false;

    render_angle=0.0;

    frame=0;
    frame_counter=0;
    frame_counter_limit=0;

    light_source.on=false;
    light_source.x=0;
    light_source.y=0;
    light_source.color=color_name_to_doubles(COLOR_WHITE);
    light_source.radius=3*(TILE_SIZE/LIGHTING_TILE_SIZE);
    light_source.dimness=0.0;
    light_source.falloff=0.375/(TILE_SIZE/LIGHTING_TILE_SIZE);

    exists=true;

    dissipating=false;

    explosion=false;

    trigger_used=false;

    sucked_left=false;
    sucked_right=false;

    counter_sucked_left=0;
    counter_sucked_right=0;

    player.existing_shots++;

    //Setup shot type-specific stuff.

    w=SHOT_SPRITE_COUNTS[type].width;
    h=SHOT_SPRITE_COUNTS[type].height;
    SHOT_SPRITES=SHOT_SPRITE_COUNTS[type].sprites;

    //Setup vectors to hold the shot's sprite sheet clipping data.

    for(short i=0;i<SHOT_SPRITES;i++){
        vector_sprites.push_back(SDL_Rect());
        vector_sprites[i]=SPRITES_SHOT[type].move[i];
    }

    sound_data* sound=0;

    if(type==SHOT_PLAYER){
        light_source.on=true;
        light_source.color=color_name_to_doubles(COLOR_VIOLET);

        move_speed=9.0;

        kills_npc=true;
        kills_player=false;

        player_shot=true;

        //Setup a pointer to the shot's sprite sheet image.
        sprite_sheet_image_shot=&image.sprite_sheet_shot;

        sound=&sound_system.player_shoot;
    }
    else if(type==SHOT_PLAYER_SPLODE){
        light_source.on=true;
        light_source.color=color_name_to_doubles(COLOR_VIOLET);

        move_speed=9.0;

        kills_npc=true;
        kills_player=false;
        does_damage=false;
        player_shot=true;

        //Setup a pointer to the shot's sprite sheet image.
        sprite_sheet_image_shot=&image.sprite_sheet_shot_player_homing;

        sound=&sound_system.player_shoot;
    }
    else if(type==SHOT_PLAYER_SPLOSION){
        light_source.on=true;
        light_source.color=color_name_to_doubles(COLOR_VIOLET);

        move_speed=0.0;

        kills_npc=true;
        kills_player=false;
        player_shot=true;

        explosion=true;

        //Setup a pointer to the shot's sprite sheet image.
        sprite_sheet_image_shot=&image.sprite_sheet_shot_player_splosion;

        sound=&sound_system.player_shoot;
    }
    else if(type==SHOT_PLAYER_HOMING){
        light_source.on=true;
        light_source.color=color_name_to_doubles(COLOR_VIOLET);
        light_source.radius=2*(TILE_SIZE/LIGHTING_TILE_SIZE);
        light_source.falloff=0.75/(TILE_SIZE/LIGHTING_TILE_SIZE);

        move_speed=4.5;

        kills_npc=true;
        kills_player=false;

        player_shot=true;

        home_on_npcs=true;

        //Setup a pointer to the shot's sprite sheet image.
        sprite_sheet_image_shot=&image.sprite_sheet_shot_player_homing;

        sound=&sound_system.player_shoot;
    }
    else if(type==SHOT_FRUIT){
        move_speed=6.0;

        kills_npc=true;
        kills_player=true;

        //Setup a pointer to the shot's sprite sheet image.
        sprite_sheet_image_shot=&image.sprite_sheet_shot_fruit;

        sound=&sound_system.shot_fruit;
    }
    else if(type==SHOT_LASER){
        light_source.on=true;
        light_source.color=color_name_to_doubles(COLOR_RED);

        move_speed=3.0;

        kills_npc=true;
        kills_player=true;

        //Setup a pointer to the shot's sprite sheet image.
        sprite_sheet_image_shot=&image.sprite_sheet_shot_laser;

        sound=&sound_system.shot_laser;
    }
    else if(type==SHOT_BALL){
        move_speed=0.0;
        air_velocity=-15.0;

        gravity_affects=true;

        render_rotated=true;

        kills_npc=true;
        kills_player=true;

        //Setup a pointer to the shot's sprite sheet image.
        sprite_sheet_image_shot=&image.sprite_sheet_shot_ball;

        sound=&sound_system.shot_ball;
    }
    else if(type==SHOT_ARROW){
        int player_index=get_nearest_player();
        double PLAYER_X=player.x;
        double PLAYER_Y=player.y;
        if(player_index>=0){
            PLAYER_X=mp_players[player_index].x;
            PLAYER_Y=mp_players[player_index].y;
        }

        double distance_to_target=distance_between_points(x,y,PLAYER_X,PLAYER_Y);
        double distance_divisor=42.0;
        if(distance_to_target>192){
            distance_divisor+=6;
        }
        move_speed=distance_to_target/distance_divisor;
        air_velocity=-10.0;

        gravity_affects=true;

        render_rotated=true;

        kills_npc=true;
        kills_player=true;

        //Setup a pointer to the shot's sprite sheet image.
        sprite_sheet_image_shot=&image.sprite_sheet_shot_arrow;

        sound=&sound_system.shot_arrow;
    }
    else if(type==SHOT_SNOWBALL){
        int player_index=get_nearest_player();
        double PLAYER_X=player.x;
        double PLAYER_Y=player.y;
        if(player_index>=0){
            PLAYER_X=mp_players[player_index].x;
            PLAYER_Y=mp_players[player_index].y;
        }

        double distance_to_target=distance_between_points(x,y,PLAYER_X,PLAYER_Y);
        double distance_divisor=42.0;
        if(distance_to_target>192){
            distance_divisor+=6;
        }
        move_speed=distance_to_target/distance_divisor;
        air_velocity=-10.0;

        kills_npc=true;
        kills_player=true;

        gravity_affects=true;

        render_rotated=true;

        //Setup a pointer to the shot's sprite sheet image.
        sprite_sheet_image_shot=&image.sprite_sheet_shot_snowball;

        sound=&sound_system.shot_snowball;
    }
    else if(type==SHOT_FIREBALL){
        light_source.on=true;
        light_source.color=color_name_to_doubles(COLOR_ORANGE);

        move_speed=7.0;

        deadly_water_proof=true;

        kills_npc=true;
        kills_player=true;

        //Setup a pointer to the shot's sprite sheet image.
        sprite_sheet_image_shot=&image.sprite_sheet_shot_fireball;

        sound=&sound_system.shot_fireball;
    }
    else if(type==SHOT_POISON){
        light_source.on=true;
        light_source.color=color_name_to_doubles(COLOR_GREEN);
        light_source.radius=2*(TILE_SIZE/LIGHTING_TILE_SIZE);
        light_source.falloff=0.75/(TILE_SIZE/LIGHTING_TILE_SIZE);

        move_speed=7.0;

        kills_npc=true;
        kills_player=true;

        //Setup a pointer to the shot's sprite sheet image.
        sprite_sheet_image_shot=&image.sprite_sheet_shot_poison;

        sound=&sound_system.shot_poison;
    }
    else if(type==SHOT_HOMING_MISSILE){
        light_source.on=true;
        light_source.color=color_name_to_doubles(COLOR_ORANGE);

        move_speed=4.0;

        kills_npc=true;
        kills_player=true;

        render_rotated=true;

        home_on_player=true;

        //Setup a pointer to the shot's sprite sheet image.
        sprite_sheet_image_shot=&image.sprite_sheet_shot_homing_missile;

        sound=&sound_system.shot_homing_missile;
    }
    else if(type==SHOT_BOSS_DEFEATED){
        light_source.on=true;
        light_source.color=color_name_to_doubles(COLOR_RED);
        light_source.radius=25*(TILE_SIZE/LIGHTING_TILE_SIZE);
        light_source.falloff=0.035/(TILE_SIZE/LIGHTING_TILE_SIZE);

        move_speed=0.0;

        kills_npc=false;
        kills_player=false;
        does_damage=false;

        dissipate();
        frame=0;

        //Setup a pointer to the shot's sprite sheet image.
        sprite_sheet_image_shot=&image.sprite_sheet_shot_boss_defeated;

        ///sound=&sound_system.player_shoot;
    }
    else if(type==SHOT_BOSS_HAUNTED_HOUSE){
        light_source.on=true;
        light_source.color=color_name_to_doubles(COLOR_SKY_BLUE);
        light_source.radius=2*(TILE_SIZE/LIGHTING_TILE_SIZE);
        light_source.falloff=0.75/(TILE_SIZE/LIGHTING_TILE_SIZE);

        move_speed=1.5;

        BOSS=true;

        kills_npc=false;
        kills_player=true;

        //Setup a pointer to the shot's sprite sheet image.
        sprite_sheet_image_shot=&image.sprite_sheet_shot_boss_haunted_house;

        sound=&sound_system.shot_boss_haunted_house;
    }
    else if(type==SHOT_BOSS_VOLCANO){
        int player_index=get_nearest_player();
        double PLAYER_X=player.x;
        double PLAYER_Y=player.y;
        if(player_index>=0){
            PLAYER_X=mp_players[player_index].x;
            PLAYER_Y=mp_players[player_index].y;
        }

        light_source.on=true;
        light_source.color=color_name_to_doubles(COLOR_RED);
        light_source.radius=2*(TILE_SIZE/LIGHTING_TILE_SIZE);
        light_source.falloff=0.75/(TILE_SIZE/LIGHTING_TILE_SIZE);

        double distance_to_target=distance_between_points(x,y,PLAYER_X,PLAYER_Y);
        double distance_divisor=42.0;
        if(distance_to_target>192){
            distance_divisor+=6;
        }
        move_speed=distance_to_target/distance_divisor;
        if(move_speed>6.0){
            move_speed=6.0;
        }
        air_velocity=-1.0*random_range(8,10);

        gravity_affects=true;

        BOSS=true;

        kills_npc=false;
        kills_player=true;

        //Setup a pointer to the shot's sprite sheet image.
        sprite_sheet_image_shot=&image.sprite_sheet_shot_boss_volcano;

        sound=&sound_system.shot_boss_volcano;
    }
    else if(type==SHOT_BOSS_PYRAMID){
        move_speed=2.5;

        BOSS=true;

        kills_npc=false;
        kills_player=true;

        //Setup a pointer to the shot's sprite sheet image.
        sprite_sheet_image_shot=&image.sprite_sheet_shot_boss_pyramid;

        sound=&sound_system.shot_boss_pyramid;
    }
    else if(type==SHOT_BOSS_LIGHTHOUSE){
        light_source.on=true;
        light_source.color=color_name_to_doubles(COLOR_YELLOW);
        light_source.radius=2*(TILE_SIZE/LIGHTING_TILE_SIZE);
        light_source.falloff=0.75/(TILE_SIZE/LIGHTING_TILE_SIZE);

        move_speed=0.5*random_range(1,6);

        homing_chance=5;
        homing_stop_chance=1;

        BOSS=true;

        kills_npc=false;
        kills_player=true;

        render_rotated=true;

        home_on_player=true;

        //Setup a pointer to the shot's sprite sheet image.
        sprite_sheet_image_shot=&image.sprite_sheet_shot_boss_lighthouse;

        sound=&sound_system.shot_boss_lighthouse;
    }

    if(!player_shot){
        double speed_buff=0.0;

        short difficulty=player.return_actual_difficulty();

        if(difficulty==DIFFICULTY_EASY){
            speed_buff-=0.25;
        }
        else if(difficulty>=DIFFICULTY_HARD){
            speed_buff+=(double)(difficulty-DIFFICULTY_NORMAL)*0.05;
        }

        if(speed_buff>0.0){
            if(!gravity_affects){
                move_speed+=move_speed*speed_buff;

                if(move_speed>10.0){
                    move_speed=10.0;
                }
            }
        }
        else if(speed_buff<0.0){
            move_speed-=move_speed*fabs(speed_buff);

            if(move_speed<0.25){
                move_speed=0.25;
            }

            if(gravity_affects){
                air_velocity+=fabs(air_velocity)*fabs(speed_buff)/4.0;

                if(air_velocity>-0.25){
                    air_velocity=-0.25;
                }
            }
        }
    }

    //If this shot is being rendered with rotation.
    if(render_rotated){
        //Determine the angle of movement.
        if(gravity_affects){
            double x_move=move_speed*(cos(angle*(M_PI/180)));
            render_angle=x_move-air_velocity*8;
            if(x_move<0.0){
                render_angle+=270;
            }
        }
        else{
            render_angle=angle;
        }
    }

    bool touching_solid=false;

    if(!player_shot && !BOSS && !dissipating){
        //The current tile location for the shot.
        int shot_current_x=(int)((int)x/TILE_SIZE);
        int shot_current_y=(int)((int)y/TILE_SIZE);

        //Check all tiles in a square around the shot.
        int check_x_start=shot_current_x-4;
        int check_x_end=shot_current_x+4;
        int check_y_start=shot_current_y-4;
        int check_y_end=shot_current_y+4;

        for(int int_x=check_x_start;int_x<check_x_end;int_x++){
            for(int int_y=check_y_start;int_y<check_y_end;int_y++){
                if(int_x>=0 && int_x<=(level.level_x/TILE_SIZE)-1 && int_y>=0 && int_y<=(level.level_y/TILE_SIZE)-1){
                    Tile* tile=&level.tile_array[int_x][int_y];

                    if(tile->solidity==TILE_SOLIDITY_SOLID && collision_check(x,y,w,h,tile->x,tile->y,TILE_SIZE,TILE_SIZE)){
                        touching_solid=true;

                        break;
                    }
                }
            }
        }
    }

    if(touching_solid){
        frame=0;
        exists=false;
        player.existing_shots--;
    }
    else{
        if(sound!=0){
            //The shot has just been fired, so play its sound.
            play_positional_sound(*sound,x,y);
        }
    }
}

double Shot::return_x(){
    return x-hitbox_size_modifier;
}
double Shot::return_y(){
    return y-hitbox_size_modifier;
}
double Shot::return_w(){
    return w+hitbox_size_modifier*2;
}
double Shot::return_h(){
    return h+hitbox_size_modifier*2;
}

bool Shot::get_phasing(){
    bool phasing=false;

    if(player_shot && player.get_upgrade_state("player_shot_noclip")){
        phasing=true;
    }

    return phasing;
}

bool Shot::moving_up(){
    if((angle>0 && angle<180) || (gravity_affects && air_velocity<0.0)){
        return true;
    }

    return false;
}

bool Shot::moving_down(){
    if((angle>180 && angle<360) || (gravity_affects && air_velocity>0.0)){
        return true;
    }

    return false;
}

bool Shot::moving_left(){
    if((angle>90 && angle<270) || (gravity_affects && move_speed<0.0)){
        return true;
    }

    return false;
}

bool Shot::moving_right(){
    if((angle>=0 && angle<90) || (angle>270 && angle<=360) || (gravity_affects && move_speed>0.0)){
        return true;
    }

    return false;
}

int Shot::get_nearest_player(){
    int nearest_player=-1;
    double nearest_distance=-1.0;

    for(int i=-1;i<(int)mp_players.size();i++){
        double PLAYER_X=player.x;
        double PLAYER_Y=player.y;
        bool PLAYER_DYING=player.DYING;
        bool PLAYER_BUBBLE_MODE=player.bubble_mode;
        if(i>=0){
            PLAYER_X=mp_players[i].x;
            PLAYER_Y=mp_players[i].y;
            PLAYER_DYING=mp_players[i].DYING;
            PLAYER_BUBBLE_MODE=mp_players[i].bubble_mode;
        }

        if(!PLAYER_DYING && !PLAYER_BUBBLE_MODE){
            double x1=x;
            double y1=y;
            double x2=PLAYER_X;
            double y2=PLAYER_Y;
            double this_distance=sqrt(((x2-x1)*(x2-x1))+((y2-y1)*(y2-y1)));

            if(nearest_distance==-1.0 || this_distance<nearest_distance){
                nearest_player=i;
                nearest_distance=this_distance;
            }
        }
    }

    return nearest_player;
}

double Shot::get_target_direction(double target_x,double target_y,double target_w,double target_h){
    double target_angle=0.0;
    int rotation_target_x=0;
    int rotation_target_y=0;
    double x2=0.0;
    double y2=0.0;
    double x1=0.0;
    double y1=0.0;
    double x_component=0.0;
    double y_component=0.0;

    rotation_target_x=target_x+target_w/2-player.camera_x;
    rotation_target_y=target_y+target_h/2-player.camera_y;

    x2=(x+w/2-player.camera_x)+(rotation_target_x-(x+w/2-player.camera_x));
    y2=rotation_target_y+((y+h/2-player.camera_y)-rotation_target_y);

    x1=x+w/2-player.camera_x;
    y1=y+h/2-player.camera_y;

    x_component=sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
    y_component=sqrt(((rotation_target_x-x2)*(rotation_target_x-x2))+((rotation_target_y-y2)*(rotation_target_y-y2)));

    if(rotation_target_x<(x+w/2-player.camera_x)){
        x_component*=-1;
    }

    target_angle=atan2(y_component,x_component)*(180/M_PI);

    if(rotation_target_y>(y+h/2-player.camera_y)){
        target_angle=180+(180-target_angle);
    }

    return target_angle;
}

void Shot::move_suck(short direction){
    short modifier=0;
    if(direction==LEFT){
        modifier=counter_sucked_left;
    }
    else if(direction==RIGHT){
        modifier=counter_sucked_right;
    }

    //temp_run_speed holds the value of the "run speed" we will be using.
    double temp_run_speed=fabs(move_speed)-(fabs(move_speed)*0.5)+move_speed*0.025*modifier;

    if(!gravity_affects){
        if(temp_run_speed>fabs(move_speed)){
            angle+=180;
            if(angle>=360){
                angle-=360;
            }
        }
    }

    double run_chunk;
    if(fabs(temp_run_speed)<pixel_safety_x){
        run_chunk=fabs(temp_run_speed);
    }
    else{
        run_chunk=pixel_safety_x;
    }

    for(double i=fabs(temp_run_speed);i>0;){
        //If we have run_chunk or more pixels left to move,
        //we will move run_chunk pixels, call handle_events(), and loop back up here.

        //Or, if we have less than run_chunk pixels left to move,
        //we will move whatever pixels we have left and call handle_events() once more.
        if(i<run_chunk){
            run_chunk=i;
            i=0;
        }

        //Move.
        if(direction==LEFT){
            x-=run_chunk;
        }
        else if(direction==RIGHT){
            x+=run_chunk;
        }

        //If we still have pixels left to move.
        if(i!=0){
            i-=run_chunk;
        }

        handle_events();
    }
}

void Shot::move(){
    if(exists){
        if(sucked_left && !sucked_right){
            if(counter_sucked_right>0){
                counter_sucked_right=0;
            }
            if(counter_sucked_left<120){
                counter_sucked_left++;
            }
            move_suck(LEFT);
        }
        else if(!sucked_left && sucked_right){
            if(counter_sucked_left>0){
                counter_sucked_left=0;
            }
            if(counter_sucked_right<120){
                counter_sucked_right++;
            }
            move_suck(RIGHT);
        }
        else if((sucked_left && sucked_right) || (!sucked_left && !sucked_right)){
            counter_sucked_left=0;
            counter_sucked_right=0;
        }

        //If the shot is not dissipating, we handle its movement.
        if(!dissipating){
            int random=random_range(0,99);
            bool should_home=false;
            if(random<homing_chance){
                should_home=true;
            }

            if(!should_home){
                if(home_on_player || home_on_npcs){
                    random=random_range(0,99);
                    if(random<homing_stop_chance){
                        home_on_player=false;
                        home_on_npcs=false;
                    }
                }
            }
            else{
                //If the shot homes in on the player.
                if(home_on_player){
                    int player_index=get_nearest_player();
                    double PLAYER_X=player.x;
                    double PLAYER_Y=player.y;
                    double PLAYER_WIDTH=player.w;
                    double PLAYER_HEIGHT=player.h;
                    if(player_index>=0){
                        PLAYER_X=mp_players[player_index].x;
                        PLAYER_Y=mp_players[player_index].y;
                        PLAYER_WIDTH=mp_players[player_index].w;
                        PLAYER_HEIGHT=mp_players[player_index].h;
                    }

                    angle=get_target_direction(PLAYER_X,PLAYER_Y,PLAYER_WIDTH,PLAYER_HEIGHT);
                }
                //If the shot homes in on the nearest npc.
                else if(home_on_npcs){
                    double nearest_distance=-1;
                    double target_x=0.0;
                    double target_y=0.0;
                    double target_w=0.0;
                    double target_h=0.0;

                    int home_range=768;

                    for(int i=0;i<vector_npcs.size();i++){
                        if(fabs(vector_npcs[i].x-x)<home_range && fabs(vector_npcs[i].y-y)<home_range){
                            if(vector_npcs[i].exists && !vector_npcs[i].counts_as_trap && !vector_npcs[i].invulnerable &&
                               !vector_npcs[i].friendly){
                                int shot_x=(int)((int)x/TILE_SIZE);
                                int shot_y=(int)((int)y/TILE_SIZE);

                                int npc_x=(int)((int)vector_npcs[i].x/TILE_SIZE);
                                int npc_y=(int)((int)vector_npcs[i].y/TILE_SIZE);

                                int dx=abs(npc_x-shot_x);
                                int dy=abs(npc_y-shot_y);

                                int sx=-1;
                                if(shot_x<npc_x){
                                    sx=1;
                                }
                                int sy=-1;
                                if(shot_y<npc_y){
                                    sy=1;
                                }

                                int err=dx-dy;
                                int err2=0;

                                bool solid_in_way=false;

                                if(!player_shot || !player.get_upgrade_state("player_shot_noclip")){
                                    while(true){
                                        if(shot_x>=0 && shot_x<=(level.level_x/TILE_SIZE)-1 && shot_y>=0 && shot_y<=(level.level_y/TILE_SIZE)-1){
                                            Tile* tile=&level.tile_array[shot_x][shot_y];
                                            if(tile->solidity==TILE_SOLIDITY_SOLID){
                                                solid_in_way=true;
                                                break;
                                            }
                                        }

                                        if(shot_x==npc_x && shot_y==npc_y){
                                            break;
                                        }

                                        err2=err*2;

                                        if(err2>-dy){
                                            err-=dy;
                                            shot_x+=sx;
                                        }

                                        if(shot_x==npc_x && shot_y==npc_y){
                                            if(shot_x>=0 && shot_x<=(level.level_x/TILE_SIZE)-1 && shot_y>=0 && shot_y<=(level.level_y/TILE_SIZE)-1){
                                                Tile* tile=&level.tile_array[shot_x][shot_y];
                                                if(tile->solidity==TILE_SOLIDITY_SOLID){
                                                    solid_in_way=true;
                                                }
                                            }

                                            break;
                                        }

                                        if(err2<dx){
                                            err+=dx;
                                            shot_y+=sy;
                                        }
                                    }
                                }

                                if(!solid_in_way){
                                    double x1=x;
                                    double y1=y;
                                    double x2=vector_npcs[i].x;
                                    double y2=vector_npcs[i].y;
                                    double this_distance=sqrt(((x2-x1)*(x2-x1))+((y2-y1)*(y2-y1)));
                                    if(nearest_distance==-1 || this_distance<nearest_distance){
                                        target_x=vector_npcs[i].x;
                                        target_y=vector_npcs[i].y;
                                        target_w=vector_npcs[i].w;
                                        target_h=vector_npcs[i].h;
                                        nearest_distance=this_distance;
                                    }
                                }
                            }
                        }
                    }

                    if(nearest_distance!=-1){
                        angle=get_target_direction(target_x,target_y,target_w,target_h);
                    }
                }
            }

            //************************************//
            // Handle x-axis and y-axis movement: //
            //************************************//

            //////////////////////////////////////////////
            //Move the shot according to its move state://
            //////////////////////////////////////////////

            double run_chunk;
            if(fabs(move_speed)<pixel_safety_x){
                run_chunk=fabs(move_speed);
            }
            else{
                run_chunk=pixel_safety_x;
            }

            for(double i=fabs(move_speed);i>0;){
                //If we have run_chunk or more pixels left to move,
                //we will move run_chunk pixels, call handle_events(), and loop back up here.

                //Or, if we have less than run_chunk pixels left to move,
                //we will move whatever pixels we have left and call handle_events() once more.
                if(i<run_chunk){
                    run_chunk=i;
                    i=0;
                }

                //Move.
                if(gravity_affects){
                    x+=run_chunk*(cos(angle*(M_PI/180)));
                }
                else{
                    x+=run_chunk*(cos(angle*(M_PI/180)));
                    y+=run_chunk*-(sin(angle*(M_PI/180)));
                }

                //If we still have pixels left to move.
                if(i!=0){
                    i-=run_chunk;
                }

                handle_events();
            }

            if(gravity_affects){
                double air_chunk;
                if(fabs(air_velocity)<pixel_safety_y){
                    air_chunk=fabs(air_velocity);
                }
                else{
                    air_chunk=pixel_safety_y;
                }

                for(double i=fabs(air_velocity);i>0;){
                    //If we have air_chunk or more pixels left to move,
                    //we will move air_chunk pixels, call handle_events(), and loop back up here.

                    //Or, if we have less than air_chunk pixels left to move,
                    //we will move whatever pixels we have left and call handle_events() once more.
                    if(i<air_chunk){
                        air_chunk=i;
                        i=0;
                    }

                    //Move.
                    if(air_velocity<0){
                        y-=air_chunk;
                    }
                    else if(air_velocity>0){
                        y+=air_chunk;
                    }


                    //If we still have pixels left to move.
                    if(i!=0){
                        i-=air_chunk;
                    }

                    handle_events();
                }

                //Handle gravity, which will affect the next tick.

                //As long as air_velocity hasn't exceeded gravity_max, the maximum speed an object can fall, add gravity to air_velocity.
                if(air_velocity<gravity_max){
                    air_velocity+=gravity;
                }
                if(air_velocity>gravity_max){
                    air_velocity=gravity_max;
                }
            }

            //If the shot didn't move at all.
            if(move_speed==0.0 && (!gravity_affects || (gravity_affects && air_velocity==0.0))){
                handle_events();
            }

            //If this shot is being rendered with rotation.
            if(render_rotated){
                //Determine the angle of movement.
                if(gravity_affects){
                    double x_move=move_speed*(cos(angle*(M_PI/180)));

                    if(x_move<0.0){
                        render_angle=x_move+air_velocity*8;
                        render_angle+=180;
                    }
                    else{
                        render_angle=x_move-air_velocity*8;
                    }
                }
                else{
                    render_angle=angle;
                }
            }
        }

        sucked_left=false;
        sucked_right=false;
    }
}

void Shot::handle_events(){
    //If the shot is not dissipating, handle its collision detection.
    if(!dissipating){
        //Check for a collision with the player.
        if(kills_player && collision_check(return_x(),return_y(),return_w(),return_h(),player.return_x(),player.return_y(),player.return_w(),player.return_h())){
            if(does_damage && (type!=SHOT_FRUIT || (type==SHOT_FRUIT && !player.suit_banana)) && !player.bubble_mode){
                //If the player was not already dying, and is not invulnerable.
                if(!player.DYING && !player.invulnerable){
                    if(!counts_as_trap){
                        player.stat_deaths_enemies++;
                    }
                    else{
                        player.stat_deaths_traps++;
                    }
                }

                //The player dies!
                player.handle_death(return_x(),return_y(),return_w(),return_h());
            }

            if(type!=SHOT_FRUIT || (type==SHOT_FRUIT && !player.suit_banana)){
                if(!explosion){
                    dissipate();
                    return;
                }
            }
            else if(type==SHOT_FRUIT && player.suit_banana){
                if(random_range(0,99)<50){
                    angle+=180.0+random_range(5,20);
                }
                else{
                    angle+=180.0-random_range(5,20);
                }

                while(angle>359.0){
                    angle-=360.0;
                }
                while(angle<0.0){
                    angle+=360.0;
                }

                if(x<player.x){
                    x=player.x-w;
                }
                else if(x+w>player.x+player.w){
                    x=player.x+player.w;
                }

                if(y<player.y){
                    y=player.y-h;
                }
                else if(y+h>player.y+player.h){
                    y=player.y+player.h;
                }

                play_positional_sound(sound_system.reflect_banana,x,y);

                return;
            }
        }

        for(int i=0;i<mp_players.size();i++){
            if(kills_player && collision_check(return_x(),return_y(),return_w(),return_h(),mp_players[i].return_x(),mp_players[i].return_y(),mp_players[i].return_w(),mp_players[i].return_h())){
                if(does_damage && (type!=SHOT_FRUIT || (type==SHOT_FRUIT && !mp_players[i].suit_banana)) && !mp_players[i].bubble_mode){
                    //The player dies!
                    mp_players[i].handle_death(return_x(),return_y(),return_w(),return_h());
                }

                if(type!=SHOT_FRUIT || (type==SHOT_FRUIT && !mp_players[i].suit_banana)){
                    if(!explosion){
                        dissipate();
                        return;
                    }
                }
                else if(type==SHOT_FRUIT && mp_players[i].suit_banana){
                    if(random_range(0,99)<50){
                        angle+=180.0+random_range(5,20);
                    }
                    else{
                        angle+=180.0-random_range(5,20);
                    }

                    while(angle>359.0){
                        angle-=360.0;
                    }
                    while(angle<0.0){
                        angle+=360.0;
                    }

                    if(x<mp_players[i].x){
                        x=mp_players[i].x-w;
                    }
                    else if(x+w>mp_players[i].x+mp_players[i].w){
                        x=mp_players[i].x+mp_players[i].w;
                    }

                    if(y<mp_players[i].y){
                        y=mp_players[i].y-h;
                    }
                    else if(y+h>mp_players[i].y+mp_players[i].h){
                        y=mp_players[i].y+mp_players[i].h;
                    }

                    play_positional_sound(sound_system.reflect_banana,x,y);

                    return ;
                }
            }
        }

        bool phasing=get_phasing();

        //Check for collisions with npcs.
        for(int i=0;i<vector_npcs.size();i++){
            if(fabs(vector_npcs[i].x-x)<PROCESS_RANGE && fabs(vector_npcs[i].y-y)<PROCESS_RANGE){
                if(vector_npcs[i].exists && !vector_npcs[i].counts_as_trap){
                    //If the shot strikes the npc and can kill npcs.
                    if(kills_npc && collision_check(return_x(),return_y(),return_w(),return_h(),vector_npcs[i].return_x(),vector_npcs[i].return_y(),vector_npcs[i].return_w(),vector_npcs[i].return_h())){
                        if(does_damage){
                            //If this is a player shot, and the npc is alive and not invulnerable.
                            if(player_shot && vector_npcs[i].exists && !vector_npcs[i].invulnerable){
                                player.stat_enemies_stunned++;
                                player.special_count_kills_this_level++;
                            }

                            vector_npcs[i].handle_death();
                        }

                        if(!explosion && !phasing){
                            dissipate();
                            return;
                        }
                    }
                }
            }
        }

        if(!phasing){
            //The current tile location for the shot.
            int shot_current_x=(int)((int)x/TILE_SIZE);
            int shot_current_y=(int)((int)y/TILE_SIZE);

            //Check all tiles in a square around the shot.
            int check_x_start=shot_current_x-4;
            int check_x_end=shot_current_x+4;
            int check_y_start=shot_current_y-4;
            int check_y_end=shot_current_y+4;

            //First, we check for collisions with tiles.

            for(int int_y=check_y_start;int_y<check_y_end;int_y++){
                for(int int_x=check_x_start;int_x<check_x_end;int_x++){
                    //As long as the current tile is within the level's boundaries.
                    if(int_x>=0 && int_x<=(level.level_x/TILE_SIZE)-1 && int_y>=0 && int_y<=(level.level_y/TILE_SIZE)-1){
                        Tile* tile=&level.tile_array[int_x][int_y];

                        //If the tile is a solid tile or a water tile.
                        if(tile->solidity==TILE_SOLIDITY_SOLID ||
                           (tile->special==TILE_SPECIAL_WATER && (!player.deadly_water || (player.deadly_water && !deadly_water_proof)))){
                            //We check the whole shot against the whole tile.
                            //xxxxxx
                            //xxxxxx
                            //xxxxxx
                            //xxxxxx
                            //If the tile is flat.
                            if(level.tile_array[int_x][int_y].slope==180 &&
                               collision_check(return_x(),return_y(),return_w(),return_h(),level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE)){
                                //If the tile is a water tile and this is a player shot.
                                if(!player.deadly_water && player_shot && level.tile_array[int_x][int_y].special==TILE_SPECIAL_WATER){
                                    player.stat_water_shot++;
                                }
                                //If we have a collision, the shot has hit a solid tile and no longer exists.
                                if(!explosion){
                                    dissipate();
                                    return;
                                }
                            }
                            //If the tile is sloped and the shot is moving right.
                            else if(moving_right() && level.tile_array[int_x][int_y].slope!=180 &&
                                    collision_check(return_x(),return_y(),return_w()/2.0,return_h()/2.0,level.tile_array[int_x][int_y].x+TILE_SIZE/2,level.tile_array[int_x][int_y].y+6,TILE_SIZE/2,TILE_SIZE-12) &&
                                    x+w<level.tile_array[int_x][int_y].x+TILE_SIZE){
                                //If the tile is a water tile and this is a player shot.
                                if(!player.deadly_water && player_shot && level.tile_array[int_x][int_y].special==TILE_SPECIAL_WATER){
                                    player.stat_water_shot++;
                                }
                                //If we have a collision, the shot has hit a solid tile and no longer exists.
                                if(!explosion){
                                    dissipate();
                                    return;
                                }
                            }
                            //If the tile is sloped and the shot is moving left.
                            else if(moving_left() && level.tile_array[int_x][int_y].slope!=180 &&
                                    collision_check(return_x()+return_w()/2.0,return_y(),return_w()/2.0,return_h()/2.0,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y+6,TILE_SIZE/2,TILE_SIZE-12) &&
                                    x>level.tile_array[int_x][int_y].x){
                                //If the tile is a water tile and this is a player shot.
                                if(!player.deadly_water && player_shot && level.tile_array[int_x][int_y].special==TILE_SPECIAL_WATER){
                                    player.stat_water_shot++;
                                }
                                //If we have a collision, the shot has hit a solid tile and no longer exists.
                                if(!explosion){
                                    dissipate();
                                    return;
                                }
                            }
                            //If the tile is sloped and the shot is moving down.
                            else if(moving_down() && level.tile_array[int_x][int_y].slope!=180 &&
                                    collision_check(return_x(),return_y(),return_w(),return_h()/2.0,level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y+TILE_SIZE/2,TILE_SIZE,TILE_SIZE/2)){
                                //If the tile is a water tile and this is a player shot.
                                if(!player.deadly_water && player_shot && level.tile_array[int_x][int_y].special==TILE_SPECIAL_WATER){
                                    player.stat_water_shot++;
                                }
                                //If we have a collision, the shot has hit a solid tile and no longer exists.
                                if(!explosion){
                                    dissipate();
                                    return;
                                }
                            }
                        }

                        //If the tile is a solid-only-on-top tile, and the shot is moving downwards.
                        else if(type!=SHOT_FRUIT && !player_shot && (level.tile_array[int_x][int_y].solidity==TILE_SOLIDITY_CLOUD || level.tile_array[int_x][int_y].special==TILE_SPECIAL_CLIMBABLE_TOP) &&
                                moving_down()){
                            //We check the whole shot against the whole tile.
                            //xxxxxx
                            //xxxxxx
                            //xxxxxx
                            //xxxxxx
                            if(collision_check(return_x(),return_y(),return_w(),return_h(),level.tile_array[int_x][int_y].x,level.tile_array[int_x][int_y].y,TILE_SIZE,TILE_SIZE)){
                                //If we have a collision, the shot has hit a solid tile and no longer exists.
                                if(!explosion){
                                    dissipate();
                                    return;
                                }
                            }
                        }
                    }
                }
            }

            //Check for collisions with traps.
            for(int i=0;i<vector_traps.size();i++){
                if(fabs(vector_traps[i].x-x)<PROCESS_RANGE && fabs(vector_traps[i].y-y)<PROCESS_RANGE){
                    if(vector_traps[i].dangerous && vector_traps[i].solid && collision_check(return_x(),return_y(),return_w(),return_h(),vector_traps[i].x,vector_traps[i].y,vector_traps[i].w,vector_traps[i].h)){
                        if(!explosion){
                            dissipate();
                            return;
                        }
                    }
                }
            }

            //Check for collisions with moving platforms.
            for(int i=0;i<vector_moving_platforms.size();i++){
                if(fabs(vector_moving_platforms[i].x-x)<PROCESS_RANGE && fabs(vector_moving_platforms[i].y-y)<PROCESS_RANGE){
                    //If the shot strikes the top of the moving platform.
                    if(collision_check(return_x(),return_y(),return_w(),return_h(),vector_moving_platforms[i].x,vector_moving_platforms[i].y,vector_moving_platforms[i].w,vector_moving_platforms[i].h) && (angle>180 && angle<360)){
                        if(!explosion){
                            dissipate();
                            return;
                        }
                    }
                }
            }
        }

        //Check for collisions with triggers.
        if(!trigger_used){
            for(int i=0;i<vector_triggers.size();i++){
                if(fabs(vector_triggers[i].x-x)<PROCESS_RANGE && fabs(vector_triggers[i].y-y)<PROCESS_RANGE){
                    if(vector_triggers[i].render_trigger && collision_check(x,y,w,h,vector_triggers[i].x,vector_triggers[i].y,vector_triggers[i].w,vector_triggers[i].h)){
                        if(type!=SHOT_PLAYER_SPLODE){
                            vector_triggers[i].use();

                            if(player_shot){
                                player.stat_triggers_shot++;
                            }
                        }

                        if(!explosion && !phasing){
                            dissipate();
                            return;
                        }
                        else if(explosion){
                            if(type!=SHOT_PLAYER_SPLODE){
                                trigger_used=true;
                            }
                        }
                    }
                }
            }
        }

        if(!phasing){
            //Check for collisions with doors.
            for(int i=0;i<vector_doors.size();i++){
                if(fabs(vector_doors[i].x-x)<PROCESS_RANGE && fabs(vector_doors[i].y-y)<PROCESS_RANGE){
                    //If the shot strikes the door and the door is closed.
                    if(!vector_doors[i].open && collision_check(return_x(),return_y(),return_w(),return_h(),vector_doors[i].x+8,vector_doors[i].y,vector_doors[i].w-16,vector_doors[i].h)){
                        if(!explosion){
                            dissipate();
                            return;
                        }
                    }
                }
            }
        }

        if(!phasing){
            //Check for collisions with other shots.
            for(int i=0;i<vector_shots.size();i++){
                if(fabs(vector_shots[i].x-x)<PROCESS_RANGE && fabs(vector_shots[i].y-y)<PROCESS_RANGE){
                    //If the shot is not this shot, strikes the shot, and the shot is not already dissipating.
                    if(&vector_shots[i]!=this && !vector_shots[i].dissipating && !vector_shots[i].get_phasing() &&
                       collision_check(x,y,w,h,vector_shots[i].x,vector_shots[i].y,vector_shots[i].w,vector_shots[i].h)){///collision_check(return_x(),return_y(),return_w(),return_h(),vector_shots[i].return_x(),vector_shots[i].return_y(),vector_shots[i].return_w(),vector_shots[i].return_h())
                        //Both shots dissipate.
                        if(!vector_shots[i].explosion){
                            vector_shots[i].dissipate();
                        }
                        if(!explosion){
                            dissipate();
                            return;
                        }
                    }
                }
            }
        }

        //If the shot hits a level boundary, it no longer exists.
        if(return_x()<0){
            if(!explosion){
                dissipate();
                return;
            }
        }
        if(return_x()+return_w()>level.level_x){
            if(!explosion){
                dissipate();
                return;
            }
        }
        if(return_y()<0){
            if(!explosion){
                dissipate();
                return;
            }
        }
        if(return_y()+return_h()>level.level_y){
            if(!explosion){
                dissipate();
                return;
            }
        }
    }
}

void Shot::dissipate(){
    if(!dissipating){
        dissipating=true;
        frame_counter=0;
        frame=SHOT_SPRITES-4;

        if(type==SHOT_PLAYER){
            //Play the shot impact sound.
            play_positional_sound(sound_system.player_shot_impact,x,y);
        }
        else if(type==SHOT_PLAYER_SPLODE){
            //Play the shot impact sound.
            ///Probably not needed.
            ///play_positional_sound(sound_system.player_shot_impact,x,y);

            //Create the splosion shot.
            new_shots.push_back(new_shot_data((x+w/2.0)-SHOT_SPLOSION_W/2.0,(y+h/2.0)-SHOT_SPLOSION_H/2.0,0,SHOT_PLAYER_SPLOSION,false));
        }
        else if(type==SHOT_PLAYER_SPLOSION){
            //Play the shot impact sound.
            ///Probably not needed.
            ///play_positional_sound(sound_system.player_shot_impact,x,y);
        }
        else if(type==SHOT_PLAYER_HOMING){
            //Play the shot impact sound.
            play_positional_sound(sound_system.player_shot_impact,x,y);
        }
        else if(type==SHOT_FRUIT){
            //Play the shot impact sound.
            play_positional_sound(sound_system.shot_impact_fruit,x,y);
        }
        else if(type==SHOT_LASER){
            //Play the shot impact sound.
            play_positional_sound(sound_system.shot_impact_laser,x,y);
        }
        else if(type==SHOT_BALL){
            //Play the shot impact sound.
            play_positional_sound(sound_system.shot_impact_ball,x,y);
        }
        else if(type==SHOT_ARROW){
            //Play the shot impact sound.
            play_positional_sound(sound_system.shot_impact_arrow,x,y);
        }
        else if(type==SHOT_SNOWBALL){
            //Play the shot impact sound.
            play_positional_sound(sound_system.shot_impact_snowball,x,y);
        }
        else if(type==SHOT_FIREBALL){
            //Play the shot impact sound.
            play_positional_sound(sound_system.shot_impact_fireball,x,y);
        }
        else if(type==SHOT_POISON){
            //Play the shot impact sound.
            play_positional_sound(sound_system.shot_impact_poison,x,y);
        }
        else if(type==SHOT_HOMING_MISSILE){
            //Play the shot impact sound.
            play_positional_sound(sound_system.shot_impact_homing_missile,x,y);
        }
        else if(type==SHOT_BOSS_DEFEATED){
            //Play the shot impact sound.
            ///play_positional_sound(sound_system.shot_impact_,x,y);
        }
        else if(type==SHOT_BOSS_HAUNTED_HOUSE){
            //Play the shot impact sound.
            play_positional_sound(sound_system.shot_impact_boss_haunted_house,x,y);
        }
        else if(type==SHOT_BOSS_VOLCANO){
            //Play the shot impact sound.
            play_positional_sound(sound_system.shot_impact_boss_volcano,x,y);
        }
        else if(type==SHOT_BOSS_PYRAMID){
            //Play the shot impact sound.
            play_positional_sound(sound_system.shot_impact_boss_pyramid,x,y);
        }
        else if(type==SHOT_BOSS_LIGHTHOUSE){
            //Play the shot impact sound.
            play_positional_sound(sound_system.shot_impact_boss_lighthouse,x,y);
        }
    }
}

void Shot::animate(){
    if(exists){
        //Increment the frame counter.
        frame_counter++;

        //Setup the frame change limit.
        if(!dissipating){
            frame_counter_limit=5;
        }
        else if(dissipating){
            //If the frame is not the last frame.
            if(frame<SHOT_SPRITES-1){
                frame_counter_limit=4;
            }
            //If the frame is the last frame, prolong it.
            else{
                frame_counter_limit=10;
            }
        }

        if(frame_counter>=frame_counter_limit){
            frame_counter=0;

            //Now increment the frame.
            frame++;

            if(!dissipating && frame>SHOT_SPRITES-4){
                if(!explosion){
                    frame=0;
                }
                else{
                    dissipate();
                }
            }
            else if(dissipating && frame>SHOT_SPRITES-1){
                frame=0;
                exists=false;
                player.existing_shots--;
            }
        }
    }
}

void Shot::render(bool mirrored){
    //If the shot exists, render it.
    if(exists){
        //If the shot is in camera bounds, render the shot.
        if(x>=player.camera_x-w && x<=player.camera_x+player.camera_w && y>=player.camera_y-h && y<=player.camera_y+player.camera_h){
            double render_x=x;
            double render_y=y;
            if(mirrored && touching_mirror(x+MIRROR_DISTANCE_X,y+MIRROR_DISTANCE_Y,w,h)){
                render_x+=MIRROR_DISTANCE_X;
                render_y+=MIRROR_DISTANCE_Y;
            }

            if(render_rotated){
                render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*sprite_sheet_image_shot,&vector_sprites[frame],1.0,1.0,1.0,render_angle);
            }
            else{
                render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*sprite_sheet_image_shot,&vector_sprites[frame]);
            }

            ///render_rectangle((int)(x-player.camera_x),(int)(y-player.camera_y),w,h,0.5,COLOR_BLUE);
            ///render_rectangle((int)(return_x()-player.camera_x),(int)(return_y()-player.camera_y),return_w(),return_h(),0.5,COLOR_RED);
        }
    }
}
