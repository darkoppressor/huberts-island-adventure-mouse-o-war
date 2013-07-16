/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "effect_firework.h"
#include "world.h"
#include "render.h"
#include "random_number_generator.h"

using namespace std;

Effect_Firework::Effect_Firework(double get_x,double get_y,int magnitude,short get_color,short shape,int thickness,double speed_x,double speed_y){
    x=get_x;
    y=get_y;

    move_vector_x=speed_x;
    move_vector_y=speed_y;
    if(random_range(0,1)==0){
        move_vector_x*=-1;
    }
    move_vector_y*=-1;

    color=color_name_to_doubles(get_color);
    if(get_color==COLOR_RAINBOW){
        color=color_name_to_doubles(COLOR_WHITE);
    }

    if(magnitude<15){
        magnitude=15;
    }

    int amount=random_range(thickness-10,thickness+10);

    for(int i=0;i<amount;i++){
        int mag_min=magnitude-10;

        if(random_range(0,99)<75){
            mag_min=5;
        }
        double p_vector_x=0.1*random_range(mag_min,magnitude);

        mag_min=magnitude-10;
        if(random_range(0,99)<75){
            mag_min=5;
        }
        double p_vector_y=0.1*random_range(mag_min,magnitude);

        if(random_range(0,1)==0){
            p_vector_x*=-1;
        }
        if(random_range(0,1)==0){
            p_vector_y*=-1;
        }

        if(fabs(p_vector_x)-magnitude<10 && fabs(p_vector_y)-magnitude<10 &&
           random_range(0,99)<80){
            if(random_range(0,1)==0){
                if(p_vector_x>0.0){
                    p_vector_x-=random_range(2,4);
                }
                else{
                    p_vector_x+=random_range(2,4);
                }
            }
            else{
                if(p_vector_y>0.0){
                    p_vector_y-=random_range(2,4);
                }
                else{
                    p_vector_y+=random_range(2,4);
                }
            }
        }

        particles.push_back(Effect_Firework_Particle(x,y,p_vector_x,p_vector_y,get_color));
    }

    exists=true;

    exploding=false;

    decel=0.01*random_range(5,6);
    light_fade=0.004;

    light_source.on=true;
    light_source.x=0;
    light_source.y=0;
    light_source.color=color;
    light_source.radius=3*(TILE_SIZE/LIGHTING_TILE_SIZE);
    light_source.dimness=0.0;
    light_source.falloff=0.45/(TILE_SIZE/LIGHTING_TILE_SIZE);

    if(!player.on_worldmap()){
        light_source.on=false;
    }

    player.existing_effects_firework++;

    double falloff=24.0;
    if(!player.on_worldmap()){
        falloff=12.0;
    }

    int random=random_range(0,2);
    if(random==0){
        play_positional_sound(sound_system.firework_shoot_1,x,y,falloff);
    }
    else if(random==1){
        play_positional_sound(sound_system.firework_shoot_2,x,y,falloff);
    }
    else if(random==2){
        play_positional_sound(sound_system.firework_shoot_3,x,y,falloff);
    }
}

void Effect_Firework::move(){
    if(exists){
        if(exploding){
            light_source.dimness+=light_fade;
            if(light_source.dimness>=1.0){
                light_source.dimness=1.0;
            }

            bool any_existing=false;

            for(int i=0;i<particles.size();i++){
                particles[i].move();
                if(particles[i].exists){
                    any_existing=true;
                }
            }

            if(!any_existing){
                exists=false;
                player.existing_effects_firework--;
            }
        }
        else{
            x+=move_vector_x;
            y+=move_vector_y;

            move_vector_y+=decel;
            if(move_vector_y>=0.0){
                exploding=true;

                light_source.radius=15*(TILE_SIZE/LIGHTING_TILE_SIZE);
                light_source.falloff=0.15/(TILE_SIZE/LIGHTING_TILE_SIZE);

                for(int i=0;i<particles.size();i++){
                    particles[i].set_coords(x,y);
                }

                double falloff=64.0;
                if(!player.on_worldmap()){
                    falloff=28.0;
                }

                int random=random_range(0,2);
                if(random==0){
                    play_positional_sound(sound_system.firework_explode_1,x,y,falloff);
                }
                else if(random==1){
                    play_positional_sound(sound_system.firework_explode_2,x,y,falloff);
                }
                else if(random==2){
                    play_positional_sound(sound_system.firework_explode_3,x,y,falloff);
                }
            }
        }
    }
}

void Effect_Firework::render(){
    if(exists){
        if(exploding){
            if(x>=player.camera_x-1600 && x-1600<=player.camera_x+player.camera_w && y>=player.camera_y-1600 && y-1600<=player.camera_y+player.camera_h){
                for(int i=0;i<particles.size();i++){
                    particles[i].render();
                }
            }
        }
        else{
            int render_width=5;
            int render_height=5;
            if(!player.on_worldmap()){
                render_width=2;
                render_height=2;
            }

            if(x>=player.camera_x-render_width && x<=player.camera_x+player.camera_w && y>=player.camera_y-render_height && y<=player.camera_y+player.camera_h){
                render_rectangle((int)((int)x-(int)player.camera_x),(int)((int)y-(int)player.camera_y),render_width,render_height,1.0,color);
            }
        }
    }
}

Effect_Firework_Particle::Effect_Firework_Particle(double get_x,double get_y,double get_vector_x,double get_vector_y,short get_color){
    x=get_x;
    y=get_y;
    move_vector_x=get_vector_x;
    move_vector_y=get_vector_y;
    color=color_name_to_doubles(get_color);

    opacity=1.0;

    exists=true;

    exploding=true;

    fade_to_black=false;

    decel=0.01*random_range(5,6);
    fade=0.0001*random_range(65,95);
    color_fade=0.01*random_range(1,3);
}

void Effect_Firework_Particle::set_coords(double get_x,double get_y){
    x=get_x;
    y=get_y;
}

void Effect_Firework_Particle::stop_exploding(){
    exploding=false;

    move_vector_x=0.0;
    move_vector_y=0.5;
}

void Effect_Firework_Particle::move(){
    if(exists){
        x+=move_vector_x;
        y+=move_vector_y;

        if(exploding){
            if(move_vector_x<0.0){
                move_vector_x+=decel;
                if(move_vector_x>=0.0){
                    move_vector_x=0.0;
                }
            }
            else if(move_vector_x>0.0){
                move_vector_x-=decel;
                if(move_vector_x<=0.0){
                    move_vector_x=0.0;
                }
            }
            if(move_vector_y<0.0){
                move_vector_y+=decel;
                if(move_vector_y>=0.0){
                    move_vector_y=0.0;
                }
            }
            else if(move_vector_y>0.0){
                move_vector_y-=decel;
                if(move_vector_y<=0.0){
                    move_vector_y=0.0;
                }
            }

            if(move_vector_x==0.0 && move_vector_y==0.0){
                stop_exploding();
            }
        }
        else{
            if(!fade_to_black){
                color.red+=color_fade;
                if(color.red>=1.0){
                    color.red=1.0;
                }
                color.green+=color_fade;
                if(color.green>=1.0){
                    color.green=1.0;
                }
                color.blue-=color_fade;
                if(color.blue<=0.0){
                    color.blue=0.0;
                }

                if(color.red==1.0 && color.green==1.0 && color.blue==0.0){
                    fade_to_black=true;
                }
            }
            else{
                color.red-=color_fade;
                if(color.red<=0.0){
                    color.red=0.0;
                }
                color.green-=color_fade;
                if(color.green<=0.0){
                    color.green=0.0;
                }
            }

            opacity-=fade;
            if(opacity<=0.0){
                exists=false;
            }
        }
    }
}

void Effect_Firework_Particle::render(){
    if(exists){
        int render_width=2;
        int render_height=2;
        if(!player.on_worldmap()){
            render_width=1;
            render_height=1;
        }

        if(x>=player.camera_x-render_width && x<=player.camera_x+player.camera_w && y>=player.camera_y-render_height && y<=player.camera_y+player.camera_h){
            render_rectangle((int)((int)x-(int)player.camera_x),(int)((int)y-(int)player.camera_y),render_width,render_height,opacity,color);
        }
    }
}
