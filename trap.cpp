/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "trap.h"
#include "render.h"
#include "world.h"
#include "collision.h"
#include "mirror.h"

using namespace std;

Trap::Trap(double get_x,double get_y,short get_type,bool get_active,short get_identifier,short get_move_state,double get_move_speed,double get_circle_radius){
    x=get_x;
    y=get_y;

    type=get_type;

    active=get_active;

    identifier=get_identifier;

    move_state=get_move_state;

    move_speed=get_move_speed;

    circle_radius=get_circle_radius;

    hitbox_size_modifier=0;

    boss_index=-1;

    BOSS=false;

    invisible=false;

    danger_type=0;
    movement_type=0;

    resetting=false;
    reset_distance=32;
    reset_move_state=NONE;
    reset_move_speed=1.0;
    reset_perpetual=false;

    distance_moved=0;

    special_function_type=SPECIAL_FUNCTION_NONE;

    dangerous=true;

    moving=false;

    movement_circle=false;

    circle_center_x=0.0;
    circle_center_y=0.0;
    circle_angle=0.0;

    danger_counter_max=10;
    danger_counter=danger_counter_max;

    movement_counter_max=10;
    movement_counter=movement_counter_max;

    trigger_effect=false;

    can_be_turned_off=false;

    //Animation:
    frame=0;
    frame_counter=0;
    frame_speed=10;
    frame_idle_active=0;
    frame_counter_idle_active=0;
    frame_speed_idle_active=10;
    frame_idle_inactive=0;
    frame_counter_idle_inactive=0;
    frame_speed_idle_inactive=10;

    light_source.on=false;
    light_source.x=0;
    light_source.y=0;
    light_source.color=color_name_to_doubles(COLOR_WHITE);
    light_source.radius=3*(TILE_SIZE/LIGHTING_TILE_SIZE);
    light_source.dimness=0.0;
    light_source.falloff=0.0/(TILE_SIZE/LIGHTING_TILE_SIZE);

    dangerous_toggles_light=false;
    glow=true;
    glow_direction=true;
    glow_rate=48.0;
    glow_counter=0;

    TRAP_MOVE_SPRITES=0;
    TRAP_IDLE_ACTIVE_SPRITES=0;
    TRAP_IDLE_INACTIVE_SPRITES=0;

    //Setup trap type-specific stuff.

    sprite_sheet_image_trap=NULL;

    w=TRAP_SPRITE_COUNTS[type].width;
    h=TRAP_SPRITE_COUNTS[type].height;
    TRAP_MOVE_SPRITES=TRAP_SPRITE_COUNTS[type].move_sprites;
    TRAP_IDLE_ACTIVE_SPRITES=TRAP_SPRITE_COUNTS[type].idle_active_sprites;
    TRAP_IDLE_INACTIVE_SPRITES=TRAP_SPRITE_COUNTS[type].idle_inactive_sprites;

    ptr_sound_danger_on=NULL;
    ptr_sound_danger_off=NULL;
    ptr_sound_active_on=NULL;
    ptr_sound_reset_movement_start=NULL;

    danger_x=0;
    danger_y=0;
    danger_w=w;
    danger_h=h;

    solid=false;

    //Setup vectors to hold the trap's sprite sheet clipping data.

    for(short i=0;i<TRAP_MOVE_SPRITES;i++){
        vector_sprites_move.push_back(SDL_Rect());
        vector_sprites_move[i]=SPRITES_TRAP[type].move[i];
    }
    for(short i=0;i<TRAP_IDLE_ACTIVE_SPRITES;i++){
        vector_sprites_idle_active.push_back(SDL_Rect());
        vector_sprites_idle_active[i]=SPRITES_TRAP[type].idle_active[i];
    }
    for(short i=0;i<TRAP_IDLE_INACTIVE_SPRITES;i++){
        vector_sprites_idle_inactive.push_back(SDL_Rect());
        vector_sprites_idle_inactive[i]=SPRITES_TRAP[type].idle_inactive[i];
    }

    //Fire
    if(type==TRAP_FIRE){
        light_source.on=true;
        light_source.color=color_name_to_doubles(COLOR_RED);

        danger_type=0;
        movement_type=0;

        danger_x=4;
        danger_y=4;
        danger_w=w-8;
        danger_h=h-8;

        //Rendering:

        //Setup a pointer to the trap's sprite sheet image.
        sprite_sheet_image_trap=&image.sprite_sheet_trap_fire;
    }
    //Spikes
    else if(type==TRAP_SPIKES){
        hitbox_size_modifier=-4;

        danger_type=1;
        movement_type=0;

        danger_x=2;
        danger_y=-2;
        danger_w=return_w()-4;
        danger_h=return_h()/2.0+2;
        solid=true;

        danger_counter_max=75;
        danger_counter=danger_counter_max;

        frame_speed_idle_active=75;

        //Rendering:

        //Setup a pointer to the trap's sprite sheet image.
        sprite_sheet_image_trap=&image.sprite_sheet_trap_spikes;

        ptr_sound_danger_on=&sound_system.trap_spikes;
        ptr_sound_danger_off=&sound_system.trap_spikes;
    }
    //Icicle
    else if(type==TRAP_ICICLE){
        hitbox_size_modifier=-4;

        danger_type=0;
        movement_type=0;

        danger_x=2;
        danger_y=-2;
        danger_w=return_w()-4;
        danger_h=return_h()/2.0+2;
        solid=true;

        frame_speed_idle_active=20;

        //Rendering:

        //Setup a pointer to the trap's sprite sheet image.
        sprite_sheet_image_trap=&image.sprite_sheet_trap_icicle;
    }
    //Branch
    else if(type==TRAP_BRANCH){
        danger_type=0;
        movement_type=1;

        move_state=DOWN;
        move_speed=5.0;

        danger_x=4;
        danger_y=h/2;
        danger_w=w-8;
        danger_h=h/2-8;

        //Rendering:

        //Setup a pointer to the trap's sprite sheet image.
        sprite_sheet_image_trap=&image.sprite_sheet_trap_branch;

        ptr_sound_active_on=&sound_system.trap_branch;
    }
    //Electric
    else if(type==TRAP_ELECTRIC){
        light_source.on=false;
        light_source.color=color_name_to_doubles(COLOR_BLUE);
        light_source.radius=6*(TILE_SIZE/LIGHTING_TILE_SIZE);
        light_source.falloff=0.14/(TILE_SIZE/LIGHTING_TILE_SIZE);
        dangerous_toggles_light=true;
        glow=false;

        danger_type=1;
        movement_type=0;

        dangerous=false;

        danger_x=4;
        danger_y=32;
        danger_w=w-8;
        danger_h=h-64;

        danger_counter_max=100;
        danger_counter=danger_counter_max;

        frame_speed_idle_active=10;

        //Rendering:

        //Setup a pointer to the trap's sprite sheet image.
        sprite_sheet_image_trap=&image.sprite_sheet_trap_electric;

        ptr_sound_danger_on=&sound_system.trap_electric;
    }
    //Stone slab
    else if(type==TRAP_STONESLAB){
        hitbox_size_modifier=-4;

        danger_type=0;
        movement_type=2;

        move_state=DOWN;
        move_speed=6.0;

        reset_distance=192;
        reset_move_state=UP;
        reset_move_speed=1.0;

        danger_x=3;
        danger_y=3;
        danger_w=return_w()-6;
        danger_h=return_h()-3;
        solid=true;

        //Rendering:

        //Setup a pointer to the trap's sprite sheet image.
        sprite_sheet_image_trap=&image.sprite_sheet_trap_stoneslab;

        ptr_sound_reset_movement_start=&sound_system.trap_stoneslab;
    }
    //Buzz saw
    else if(type==TRAP_BUZZSAW){
        danger_type=0;
        movement_type=2;

        move_state=LEFT;
        move_speed=4.0;
        moving=true;

        reset_distance=192;
        reset_move_state=RIGHT;
        reset_move_speed=4.0;
        reset_perpetual=true;

        danger_x=4;
        danger_y=4;
        danger_w=w-8;
        danger_h=h-8;

        //Rendering:

        //Setup a pointer to the trap's sprite sheet image.
        sprite_sheet_image_trap=&image.sprite_sheet_trap_buzzsaw;
    }
    //Alarm toggler
    else if(type==TRAP_ALARM_TOGGLER){
        invisible=true;

        danger_type=0;
        movement_type=0;

        dangerous=false;

        trigger_effect=true;

        special_function_type=SPECIAL_FUNCTION_ALARM_TOGGLER_TOGGLE;
    }
    //Weather Machine
    else if(type==TRAP_WEATHER_MACHINE){
        invisible=true;

        danger_type=0;
        movement_type=0;

        dangerous=false;

        trigger_effect=true;

        special_function_type=SPECIAL_FUNCTION_WEATHER_MACHINE;
    }
    //Spikes Up
    else if(type==TRAP_SPIKES_UP){
        hitbox_size_modifier=-4;

        danger_type=0;
        movement_type=0;

        can_be_turned_off=true;

        danger_x=2;
        danger_y=-2;
        danger_w=return_w()-4;
        danger_h=return_h()/2.0+2;
        solid=true;

        //Rendering:

        //Setup a pointer to the trap's sprite sheet image.
        sprite_sheet_image_trap=&image.sprite_sheet_trap_spikes_up;
    }
    //Spikes Down
    else if(type==TRAP_SPIKES_DOWN){
        hitbox_size_modifier=-4;

        danger_type=0;
        movement_type=0;

        danger_x=2;
        danger_y=return_h()/2.0;
        danger_w=return_w()-4;
        danger_h=return_h()/2.0+2;
        solid=true;

        //Rendering:

        //Setup a pointer to the trap's sprite sheet image.
        sprite_sheet_image_trap=&image.sprite_sheet_trap_spikes_down;
    }
    //Spikes Right
    else if(type==TRAP_SPIKES_RIGHT){
        hitbox_size_modifier=-4;

        danger_type=0;
        movement_type=0;

        danger_x=return_w()/2.0;
        danger_y=2;
        danger_w=return_w()/2.0+2;
        danger_h=return_h()-4;
        solid=true;

        //Rendering:

        //Setup a pointer to the trap's sprite sheet image.
        sprite_sheet_image_trap=&image.sprite_sheet_trap_spikes_right;
    }
    //Spikes Left
    else if(type==TRAP_SPIKES_LEFT){
        hitbox_size_modifier=-4;

        danger_type=0;
        movement_type=0;

        danger_x=-2;
        danger_y=2;
        danger_w=return_w()/2.0+2;
        danger_h=return_h()-4;
        solid=true;

        //Rendering:

        //Setup a pointer to the trap's sprite sheet image.
        sprite_sheet_image_trap=&image.sprite_sheet_trap_spikes_left;
    }
    //Boss Castle
    else if(type==TRAP_BOSS_CASTLE){
        danger_type=0;
        movement_type=1;

        BOSS=true;

        solid=true;

        moving=false;
        movement_circle=true;
        if(move_state==RIGHT){
            circle_angle=0.0;
        }
        else if(move_state==LEFT){
            circle_angle=180.0;
        }

        //Rendering:

        //Setup a pointer to the trap's sprite sheet image.
        sprite_sheet_image_trap=&image.sprite_sheet_trap_boss_castle;
    }
    //Boss Pyramid
    else if(type==TRAP_BOSS_PYRAMID){
        danger_type=0;
        movement_type=1;

        moving=false;
        movement_circle=true;
        if(move_state==RIGHT){
            circle_angle=0.0;
        }
        else if(move_state==LEFT){
            circle_angle=180.0;
        }

        BOSS=true;

        solid=true;

        //Rendering:

        //Setup a pointer to the trap's sprite sheet image.
        sprite_sheet_image_trap=&image.sprite_sheet_trap_boss_pyramid;
    }
    //Seaweed
    else if(type==TRAP_SEAWEED){
        danger_type=0;
        movement_type=0;

        dangerous=false;

        //Rendering:

        frame_speed_idle_active=15;

        //Setup a pointer to the trap's sprite sheet image.
        sprite_sheet_image_trap=&image.sprite_sheet_trap_seaweed;
    }
    //Open Shop
    else if(type==TRAP_OPEN_SHOP){
        invisible=true;

        danger_type=0;
        movement_type=0;

        dangerous=false;

        trigger_effect=true;

        special_function_type=SPECIAL_FUNCTION_OPEN_SHOP;
    }
    //Open Upgrades
    else if(type==TRAP_OPEN_UPGRADES){
        invisible=true;

        danger_type=0;
        movement_type=0;

        dangerous=false;

        trigger_effect=true;

        special_function_type=SPECIAL_FUNCTION_OPEN_UPGRADES;
    }
}

double Trap::return_x(){
    return x-hitbox_size_modifier;
}
double Trap::return_y(){
    return y-hitbox_size_modifier;
}
double Trap::return_w(){
    return w+hitbox_size_modifier*2;
}
double Trap::return_h(){
    return h+hitbox_size_modifier*2;
}
double Trap::return_danger_x(){
    ///return danger_x-hitbox_size_modifier;
    return danger_x;
}
double Trap::return_danger_y(){
    ///return danger_y-hitbox_size_modifier;
    return danger_y;
}
double Trap::return_danger_w(){
    ///return danger_w+hitbox_size_modifier*2;
    return danger_w;
}
double Trap::return_danger_h(){
    ///return danger_h+hitbox_size_modifier*2;
    return danger_h;
}

bool Trap::is_sharp(){
    if(type==TRAP_BUZZSAW || type==TRAP_ICICLE || type==TRAP_SPIKES || type==TRAP_SPIKES_DOWN || type==TRAP_SPIKES_LEFT || type==TRAP_SPIKES_RIGHT || type==TRAP_SPIKES_UP){
        return true;
    }
    else{
        return false;
    }
}

void Trap::trigger_special_function(){
    if(special_function_type==SPECIAL_FUNCTION_ALARM_TOGGLER_TOGGLE){
        special_function_alarm_toggler_toggle();
    }
    else if(special_function_type==SPECIAL_FUNCTION_WEATHER_MACHINE){
        special_function_weather_machine();
    }
    else if(special_function_type==SPECIAL_FUNCTION_OPEN_SHOP){
        special_function_open_shop();
    }
    else if(special_function_type==SPECIAL_FUNCTION_OPEN_UPGRADES){
        special_function_open_upgrades();
    }
}

void Trap::toggle_active(){
    if(!active){
        active=true;

        if(movement_type==1 || movement_type==2){
            moving=true;
        }

        if(ptr_sound_active_on!=NULL){
            play_positional_sound(*ptr_sound_active_on,x,y);
        }
    }
    else{
        if(can_be_turned_off){
            active=false;
            dangerous=false;
        }
    }
}

void Trap::toggle_movement(){
    moving=!moving;
}

void Trap::move(){
    if(active){
        if(boss_index!=-1 && movement_circle){
            circle_center_x=vector_npcs[boss_index].x+vector_npcs[boss_index].w/2.0-w/2.0;
            circle_center_y=vector_npcs[boss_index].y+vector_npcs[boss_index].h/2.0-h/2.0;
        }

        if(danger_type==1 && --danger_counter<=0){
            dangerous=!dangerous;
            danger_counter=danger_counter_max;

            if(dangerous_toggles_light){
                light_source.on=!light_source.on;
            }

            if(dangerous && ptr_sound_danger_on!=NULL){
                play_positional_sound(*ptr_sound_danger_on,x,y,32.0);
            }
            else if(!dangerous && ptr_sound_danger_off!=NULL){
                play_positional_sound(*ptr_sound_danger_off,x,y,32.0);
            }
        }

        if(movement_type==3 && --movement_counter<=0){
            moving=true;
            movement_counter=movement_counter_max;
        }

        //If the trap is currently moving.
        if(moving){
            //If the trap is a resettable trap.
            if(movement_type==2){
                //If the trap has moved the reset_distance.
                if(distance_moved>=reset_distance){
                    resetting=!resetting;
                    distance_moved=0;

                    //If the trap just finished resetting, and is not a perpetual moving trap.
                    if(!resetting && !reset_perpetual){
                        ///toggle_active();
                        active=false;
                        moving=false;
                    }

                    if(resetting && ptr_sound_reset_movement_start!=NULL){
                        play_positional_sound(*ptr_sound_reset_movement_start,x,y);
                    }
                }
            }

            //************************************//
            // Handle x-axis and y-axis movement: //
            //************************************//

            //This is the maximum number of pixels that the trap will move before calling handle_events().
            double run_chunk;

            if(movement_type!=2 || (movement_type==2 && !resetting)){
                run_chunk=fabs(move_speed);
            }
            else{
                run_chunk=fabs(reset_move_speed);
            }

            for(double i=fabs(run_chunk);i>0;){
                //If we have run_chunk or more pixels left to move,
                //we will move run_chunk pixels, call handle_events(), and loop back up here.

                //Or, if we have less than run_chunk pixels left to move,
                //we will move whatever pixels we have left and call handle_events() once more.
                if(i<run_chunk){
                    run_chunk=i;
                    i=0;
                }

                distance_moved+=fabs(run_chunk);

                //Move.

                if(movement_type!=2 || (movement_type==2 && !resetting)){
                    if(movement_circle){
                        if(move_state==RIGHT){
                            circle_angle+=run_chunk;
                            if(circle_angle>=360.0){
                                circle_angle-=360.0;
                            }
                        }
                        else if(move_state==LEFT){
                            circle_angle-=run_chunk;
                            if(circle_angle<=0.0){
                                circle_angle+=360.0;
                            }
                        }
                        x=circle_radius*cos(circle_angle*(M_PI/180))+circle_center_x;
                        y=circle_radius*sin(circle_angle*(M_PI/180))+circle_center_y;
                    }
                    else{
                        if(move_state==LEFT){
                            x-=run_chunk;
                        }
                        else if(move_state==RIGHT){
                            x+=run_chunk;
                        }
                        else if(move_state==UP){
                            y-=run_chunk;
                        }
                        else if(move_state==DOWN){
                            y+=run_chunk;
                        }
                    }
                }
                else{
                    if(reset_move_state==LEFT){
                        x-=run_chunk;
                    }
                    else if(reset_move_state==RIGHT){
                        x+=run_chunk;
                    }
                    else if(reset_move_state==UP){
                        y-=run_chunk;
                    }
                    else if(reset_move_state==DOWN){
                        y+=run_chunk;
                    }
                }

                //If we still have pixels left to move.
                if(i!=0){
                    i-=run_chunk;
                }

                handle_events();
            }
        }
        //If the trap is not currently moving.
        else{
            handle_events();
        }
    }
}

void Trap::handle_events(){
    //If the trap leaves the level.
    if((x+w<0) || (x>level.level_x) || (y+h<0) || (y>level.level_y)){
        active=false;
        moving=false;
    }
}

void Trap::animate(){
    frame_counter++;
    frame_counter_idle_active++;
    frame_counter_idle_inactive++;

    if(frame_counter>=frame_speed){
        frame_counter=0;

        frame++;

        if(frame>TRAP_MOVE_SPRITES-1){
            frame=0;
        }
    }

    if(frame_counter_idle_active>=frame_speed_idle_active){
        frame_counter_idle_active=0;

        frame_idle_active++;

        if(frame_idle_active>TRAP_IDLE_ACTIVE_SPRITES-1){
            frame_idle_active=0;
        }
    }

    if(frame_counter_idle_inactive>=frame_speed_idle_inactive){
        frame_counter_idle_inactive=0;

        frame_idle_inactive++;

        if(frame_idle_inactive>TRAP_IDLE_INACTIVE_SPRITES-1){
            frame_idle_inactive=0;
        }
    }

    if(light_source.on && active && glow){
        if(glow_direction){
            light_source.dimness+=1.0/glow_rate/24.0;
            light_source.falloff+=1.0/glow_rate/24.0;
            if(light_source.falloff>0.2){
                light_source.falloff=0.2;
            }
            if(light_source.dimness>0.1){
                light_source.dimness=0.1;
            }
        }
        else{
            light_source.falloff-=1.0/glow_rate/24.0;
            light_source.dimness-=1.0/glow_rate/24.0;
            if(light_source.falloff<0.1){
                light_source.falloff=0.1;
            }
            if(light_source.dimness<0.0){
                light_source.dimness=0.0;
            }
        }

        if(++glow_counter>glow_rate){
            glow_counter=0;
            glow_direction=!glow_direction;
        }
    }
}

void Trap::render(bool mirrored){
    //If the trap is in camera bounds, render it.
    if(!invisible && x>=player.camera_x-w && x<=player.camera_x+player.camera_w && y>=player.camera_y-h && y<=player.camera_y+player.camera_h){
        double render_x=x;
        double render_y=y;
        if(mirrored && touching_mirror(x+MIRROR_DISTANCE_X,y+MIRROR_DISTANCE_Y,w,h)){
            render_x+=MIRROR_DISTANCE_X;
            render_y+=MIRROR_DISTANCE_Y;
        }

        //If the trap is idle and active.
        if(active && !moving && TRAP_IDLE_ACTIVE_SPRITES>0){
            render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*sprite_sheet_image_trap,&vector_sprites_idle_active[frame_idle_active]);
        }
        //If the trap is moving and active.
        else if(active && moving && TRAP_MOVE_SPRITES>0){
            render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*sprite_sheet_image_trap,&vector_sprites_move[frame]);
        }
        //If the trap is idle and inactive.
        else{
            if(TRAP_IDLE_INACTIVE_SPRITES>0){
                render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*sprite_sheet_image_trap,&vector_sprites_idle_inactive[frame_idle_inactive]);
            }
        }

        ///render_rectangle((int)((int)x+(int)danger_x-(int)player.camera_x),(int)((int)y+(int)danger_y-(int)player.camera_y),danger_w,danger_h,0.5,COLOR_RED);
    }
}
