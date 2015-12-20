/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "rtt_manager.h"
#include "color.h"
#include "message_log.h"
#include "world.h"

using namespace std;

vector<Rtt_Data> Rtt_Manager::textures;
vector<string> Rtt_Manager::texture_names;

void Rtt_Manager::add_texture(string name,double width,double height){
    textures.push_back(Rtt_Data());

    textures.back().create_texture(width,height);

    texture_names.push_back(name);
}

void Rtt_Manager::remove_texture(string name){
    for(size_t i=0;i<texture_names.size();i++){
        if(texture_names[i]==name){
            texture_names.erase(texture_names.begin()+i);

            textures[i].unload_texture();
            textures.erase(textures.begin()+i);

            break;
        }
    }
}

void Rtt_Manager::unload_textures(){
    for(size_t i=0;i<textures.size();i++){
        textures[i].unload_texture();
    }

    textures.clear();
    texture_names.clear();
}

Rtt_Data* Rtt_Manager::get_texture(string name){
    Rtt_Data* ptr_object=0;

    for(size_t i=0;i<texture_names.size();i++){
        if(texture_names[i]==name){
            ptr_object=&textures[i];

            break;
        }
    }

    if(ptr_object==0){
        update_error_log("Error accessing RTT texture '"+name+"'");
    }

    return ptr_object;
}

void Rtt_Manager::set_render_target(string name){
    Rtt_Data* ptr=get_texture(name);

    if(main_window.set_render_target(ptr->texture)!=0){
        msg="Unable to set render target to '"+name+"': ";
        msg+=SDL_GetError();
        update_error_log(msg);
    }

    main_window.clear_renderer(Color(0,0,0,0));
}

void Rtt_Manager::reset_render_target(){
    if(main_window.set_render_target(0)!=0){
        msg="Unable to reset render target: ";
        msg+=SDL_GetError();
        update_error_log(msg);
    }
}
