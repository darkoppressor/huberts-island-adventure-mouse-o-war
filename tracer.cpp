/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "tracer.h"
#include "world.h"
#include "render.h"
#include "enumerations.h"

using namespace std;

Tracer::Tracer(){
    color=COLOR_BLUE;
}

void Tracer::create_tracer(double x,double y){
    if(tracers.size()==0 || x!=tracers[tracers.size()-1].x || y!=tracers[tracers.size()-1].y){
        handle_max_size();

        if(color==COLOR_RED){
            color=COLOR_YELLOW;
        }
        else if(color==COLOR_YELLOW){
            color=COLOR_BLUE;
        }
        else if(color==COLOR_BLUE){
            color=COLOR_RED;
        }

        tracers.push_back(coords());
        tracers[tracers.size()-1].x=x;
        tracers[tracers.size()-1].y=y;
        tracers[tracers.size()-1].color=color;
    }
}

void Tracer::handle_max_size(){
    if(tracers.size()==tracers.max_size()-1){
        tracers.erase(tracers.begin());
    }
}

void Tracer::clear_tracers(){
    tracers.clear();
}

void Tracer::render(){
    for(int i;i<tracers.size();i++){
        if(i+1<tracers.size()){
            if((tracers[i].x>=player.camera_x && tracers[i].x<=player.camera_x+player.camera_w && tracers[i].y>=player.camera_y && tracers[i].y<=player.camera_y+player.camera_h) ||
               (tracers[i+1].x>=player.camera_x && tracers[i+1].x<=player.camera_x+player.camera_w && tracers[i+1].y>=player.camera_y && tracers[i+1].y<=player.camera_y+player.camera_h)){
                render_line((int)((int)tracers[i].x-(int)player.camera_x),(int)((int)tracers[i].y-(int)player.camera_y),(int)(tracers[i+1].x-player.camera_x),(int)(tracers[i+1].y-player.camera_y),1.0,tracers[i].color);
            }
        }
        else{
            break;
        }
    }
}
