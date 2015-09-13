/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "window_map.h"
#include "world.h"
#include "render.h"
#include "collision.h"
#include "quit.h"
#include "button_events.h"
#include "holidays.h"

#include <fstream>

using namespace std;

Window_Map::Window_Map(short get_x,short get_y,short get_w,short get_h,string get_title){
    background_image=NULL;

    x=get_x;
    y=get_y;
    w=get_w;
    h=get_h;
    title=get_title;

    on=false;
    moving=false;

    mouse_offset_x=0;
    mouse_offset_y=0;

    cam_state=0;
    camera_speed=16;

    map_camera_x=0;
    map_camera_y=0;
    map_camera_w=w-20;
    map_camera_h=h-45;

    persistent_level_data=false;
    level_x=0;
    level_y=0;

    current_viewable_level=-1;
    current_viewable_sub_level=-1;

    viewable_tiles.load_image("data/levels/3/tiles.png");
    viewable_background_tiles.load_image("data/levels/3/tiles_background.png");
    viewable_water_tiles.load_image("data/levels/3/water_tiles.png");

    //Create the close button.
    create_button(w-23,5,"","X",&button_event_close_window,0,0,BUTTON_VISIBLE);
}

Window_Map::~Window_Map(){
    viewable_tiles.unload_image();
    viewable_background_tiles.unload_image();
    viewable_water_tiles.unload_image();
}

void Window_Map::handle_map_input(){
    //Handle camera directional keys being pressed.
    if(player.command_state(COMMAND_LEFT)){
        cam_state=LEFT;
    }
    if(player.command_state(COMMAND_UP)){
        cam_state=UP;
    }
    if(player.command_state(COMMAND_RIGHT)){
        cam_state=RIGHT;
    }
    if(player.command_state(COMMAND_DOWN)){
        cam_state=DOWN;
    }

    //Handle multiple camera directional keys being pressed at once.
    if(player.command_state(COMMAND_LEFT) && player.command_state(COMMAND_UP)){
        cam_state=LEFT_UP;
    }
    if(player.command_state(COMMAND_UP) && player.command_state(COMMAND_RIGHT)){
        cam_state=RIGHT_UP;
    }
    if(player.command_state(COMMAND_RIGHT) && player.command_state(COMMAND_DOWN)){
        cam_state=RIGHT_DOWN;
    }
    if(player.command_state(COMMAND_DOWN) && player.command_state(COMMAND_LEFT)){
        cam_state=LEFT_DOWN;
    }
    if(player.command_state(COMMAND_LEFT) && player.command_state(COMMAND_RIGHT)){
        cam_state=LEFT;
    }
    if(player.command_state(COMMAND_UP) && player.command_state(COMMAND_DOWN)){
        cam_state=UP;
    }
    if(player.command_state(COMMAND_LEFT) && player.command_state(COMMAND_UP) && player.command_state(COMMAND_RIGHT)){
        cam_state=LEFT_UP;
    }
    if(player.command_state(COMMAND_LEFT) && player.command_state(COMMAND_DOWN) && player.command_state(COMMAND_RIGHT)){
        cam_state=LEFT_DOWN;
    }
    if(player.command_state(COMMAND_LEFT) && player.command_state(COMMAND_UP) && player.command_state(COMMAND_DOWN)){
        cam_state=LEFT_UP;
    }
    if(player.command_state(COMMAND_UP) && player.command_state(COMMAND_RIGHT) && player.command_state(COMMAND_DOWN)){
        cam_state=RIGHT_UP;
    }
    if(player.command_state(COMMAND_LEFT) && player.command_state(COMMAND_UP) && player.command_state(COMMAND_RIGHT) && player.command_state(COMMAND_DOWN)){
        cam_state=LEFT_UP;
    }

    //If no camera directional keys are pressed, stop the camera.
    if(!player.command_state(COMMAND_LEFT) && !player.command_state(COMMAND_UP) && !player.command_state(COMMAND_RIGHT) && !player.command_state(COMMAND_DOWN)){
        cam_state=0;
    }

    //Now check for camera input and move the camera accordingly.
    if(cam_state==LEFT){
        map_camera_x-=camera_speed;
    }
    if(cam_state==UP){
        map_camera_y-=camera_speed;
    }
    if(cam_state==RIGHT){
        map_camera_x+=camera_speed;
    }
    if(cam_state==DOWN){
        map_camera_y+=camera_speed;
    }
    if(cam_state==LEFT_UP){
        map_camera_x-=camera_speed;
        map_camera_y-=camera_speed;
    }
    if(cam_state==RIGHT_UP){
        map_camera_x+=camera_speed;
        map_camera_y-=camera_speed;
    }
    if(cam_state==RIGHT_DOWN){
        map_camera_x+=camera_speed;
        map_camera_y+=camera_speed;
    }
    if(cam_state==LEFT_DOWN){
        map_camera_x-=camera_speed;
        map_camera_y+=camera_speed;
    }

    //If the camera goes past the bounds of the level map, set it back in the bounds.
    if(map_camera_x<0){
        map_camera_x=0;
    }
    if(map_camera_x+map_camera_w>level_x/2){
        map_camera_x=level_x/2-map_camera_w;
    }
    if(map_camera_y<0){
        map_camera_y=0;
    }
    if(map_camera_y+map_camera_h>level_y/2){
        map_camera_y=level_y/2-map_camera_h;
    }
}

void Window_Map::handle_input_states(){
    if(on){
        int mouse_x,mouse_y;
        SDL_GetMouseState(&mouse_x,&mouse_y);
        mouse_x*=(double)((double)main_window.SCREEN_WIDTH/(double)main_window.REAL_SCREEN_WIDTH);
        mouse_y*=(double)((double)main_window.SCREEN_HEIGHT/(double)main_window.REAL_SCREEN_HEIGHT);

        //If the window is moving, center it on the mouse's current position - the offsets.
        if(moving){
            x=mouse_x-mouse_offset_x;
            y=mouse_y-mouse_offset_y;

            if(x<0){
                x=0;
            }
            if(y<0){
                y=0;
            }
            if(x+w>main_window.SCREEN_WIDTH){
                x=main_window.SCREEN_WIDTH-w;
            }
            if(y+h>main_window.SCREEN_HEIGHT){
                y=main_window.SCREEN_HEIGHT-h;
            }
        }

        else if(!moving){
            //Check to see if the mouse is hovering over any of this window's buttons.
            for(int i=0;i<buttons.size();i++){
                //For each button, reset its moused over state before anything else.
                //Remember whether or not the button was moused over before this reset.
                bool already_moused_over=buttons[i].reset_moused_over();

                //If the mouse is hovering over this button.
                if(buttons[i].is_moused_over(mouse_x,mouse_y,x,y,i)){
                    //The button is now being moused over.
                    buttons[i].mouse_over(already_moused_over);

                    //Setup the button's tooltip.
                    if(buttons[i].has_tooltip()){
                        tooltip.setup(buttons[i].return_tooltip_text(),mouse_x,mouse_y);
                    }

                    if(buttons[i].enabled){
                        player.current_button=i;
                    }
                }
            }
        }

        handle_map_input();
    }
}

void Window_Map::handle_input_events(){
    if(on){
        int mouse_x,mouse_y;
        SDL_GetMouseState(&mouse_x,&mouse_y);
        mouse_x*=(double)((double)main_window.SCREEN_WIDTH/(double)main_window.REAL_SCREEN_WIDTH);
        mouse_y*=(double)((double)main_window.SCREEN_HEIGHT/(double)main_window.REAL_SCREEN_HEIGHT);

        switch(event.type){
            case SDL_QUIT:
                quit_game();
                break;

            case SDL_MOUSEBUTTONDOWN:
                if(event.button.button==SDL_BUTTON_LEFT){
                    bool button_clicked=false;

                    //Look through all of the buttons.
                    for(int i=0;i<buttons.size();i++){
                        //If this button is moused over,
                        //it has been clicked down on.
                        if(buttons[i].is_moused_over(mouse_x,mouse_y,x,y,-2)){
                            buttons[i].mouse_button_down();
                            //A button has just been clicked, so we keep that in mind.
                            button_clicked=true;
                        }
                    }

                    //If no buttons were just clicked and the title bar of the window is clicked.
                    if(!button_clicked && collision_check(mouse_x,mouse_y,2,2,x,y,w,30)){
                        //Begin moving the window.
                        moving=true;
                        mouse_offset_x=mouse_x-x;
                        mouse_offset_y=mouse_y-y;
                    }
                }
                break;

            case SDL_MOUSEBUTTONUP:
                if(event.button.button==SDL_BUTTON_LEFT){
                    //Stop moving the inventory window.
                    moving=false;

                    //Look through all of the buttons.
                    for(int i=0;i<buttons.size();i++){
                        //If this button is moused over,
                        //the mouse button has been released over it.
                        if(buttons[i].is_moused_over(mouse_x,mouse_y,x,y,-2)){
                            buttons[i].mouse_button_up(this);
                            buttons[i].reset_clicked();
                        }
                        //Otherwise, the mouse was not released over this button.
                        else{
                            buttons[i].reset_clicked();
                        }
                    }
                }
                break;
        }
    }
}

void Window_Map::set_camera_coordinates(double cam_x,double cam_y){
    map_camera_x=cam_x;
    map_camera_y=cam_y;
}

void Window_Map::set_map_level(short get_level,short get_sub_level){
    if(get_level==player.current_level && get_sub_level==player.current_sub_level && player.persistent_level_data){
        profile.save_map();
    }

    bool need_to_load=false;

    if(current_viewable_level!=get_level || current_viewable_sub_level!=get_sub_level){
        need_to_load=true;
    }

    current_viewable_level=get_level;
    current_viewable_sub_level=get_sub_level;

    load_map_level_properties();
    load_map_data();

    if(need_to_load){
        load_map_images();
    }
}

void Window_Map::load_map_level_properties(){
    //Create a string to hold the current level number.
    string current_level="";
    ss.clear();ss.str("");ss<<current_viewable_level;current_level=ss.str();

    //Create a string to hold the current sub level number.
    string current_sub_level="";
    ss.clear();ss.str("");ss<<current_viewable_sub_level;current_sub_level=ss.str();

    string load_path="";

    ifstream load;

    persistent_level_data=false;
    level_x=0;
    level_y=0;

    //Load up the level properties for the level we want to view.

    //If the current sub level is 0, meaning "load the main level."
    if(current_viewable_sub_level==0){
        load_path="data/levels/"+current_level+"/level_properties.blazelevel";
    }
    //If there is a current sub level to load instead of the main one.
    else{
        load_path="data/levels/"+current_level+"/"+current_sub_level+"/level_properties.blazelevel";
    }
    load.open(load_path.c_str(),ifstream::in);

    if(load!=NULL){
        string catch_unneeded="";
        short number_of_background_layers=0;

        //Skip over the unneeded level data.
        load >> level_x;
        load >> level_y;
        load >> catch_unneeded;
        load >> catch_unneeded;
        load >> catch_unneeded;
        load >> catch_unneeded;
        load >> number_of_background_layers;
        for(int i=0;i<number_of_background_layers;i++){
            load >> catch_unneeded;
            load >> catch_unneeded;
        }
        load >> persistent_level_data;

        load >> catch_unneeded;

        load >> catch_unneeded;

        load.close();
        load.clear();
    }
}

void Window_Map::load_map_images(){
    if(persistent_level_data){
        //Create a string to hold the current level number.
        string current_level="";
        ss.clear();ss.str("");ss<<current_viewable_level;current_level=ss.str();

        //Create a string to hold the current sub level number.
        string current_sub_level="";
        ss.clear();ss.str("");ss<<current_viewable_sub_level;current_sub_level=ss.str();

        string load_path="";

        //If the current sub level is 0, meaning "load the main level."
        if(current_viewable_sub_level==0){
            load_path="data/levels/"+current_level+"/tiles.png";
        }
        //If there is a current sub level to load instead of the main one.
        else{
            load_path="data/levels/"+current_level+"/"+current_sub_level+"/tiles.png";
        }
        ///Doing this in hardware mode doesn't work, for some reason.
        ///It screws up the background. In software mode it works like it should.
        if(player.option_renderer==RENDERER_SOFTWARE){
            viewable_tiles.unload_image();
        }
        viewable_tiles.load_image(load_path);

        if(!player.is_level_worldmap(current_viewable_level)){
            //If the current sub level is 0, meaning "load the main level."
            if(current_viewable_sub_level==0){
                load_path="data/levels/"+current_level+"/tiles_background.png";
            }
            //If there is a current sub level to load instead of the main one.
            else{
                load_path="data/levels/"+current_level+"/"+current_sub_level+"/tiles_background.png";
            }
            ///Doing this in hardware mode doesn't work, for some reason.
            ///It screws up the background. In software mode it works like it should.
            if(player.option_renderer==RENDERER_SOFTWARE){
                viewable_background_tiles.unload_image();
            }
            viewable_background_tiles.load_image(load_path);
        }

        //If the current sub level is 0, meaning "load the main level."
        if(current_viewable_sub_level==0){
            load_path="data/levels/"+current_level+"/water_tiles.png";
        }
        //If there is a current sub level to load instead of the main one.
        else{
            load_path="data/levels/"+current_level+"/"+current_sub_level+"/water_tiles.png";
        }
        ///Doing this in hardware mode doesn't work, for some reason.
        ///It screws up the background. In software mode it works like it should.
        if(player.option_renderer==RENDERER_SOFTWARE){
            viewable_water_tiles.unload_image();
        }
        viewable_water_tiles.load_image(load_path);
    }
}

void Window_Map::load_map_data(){
    if(persistent_level_data){
        //Create a string to hold the current level number.
        string current_level="";
        ss.clear();ss.str("");ss<<current_viewable_level;current_level=ss.str();

        //Create a string to hold the current sub level number.
        string current_sub_level="";
        ss.clear();ss.str("");ss<<current_viewable_sub_level;current_sub_level=ss.str();

        string load_path="";

        ifstream load;

        //Prepare the map data vector.
        viewable_map_data.clear();
        viewable_map_data.resize(level_x/TILE_SIZE,vector<map_data>(level_y/TILE_SIZE));

        viewable_items.clear();

        //Load the map seen data from the map save file.

        load_path=profile.get_home_directory()+"profiles/"+player.name+"/saves/"+current_level+"/map.blazesave";
        load.open(load_path.c_str(),ifstream::in);

        if(load!=NULL){
            bool seen=false;

            for(int int_y=0;int_y<level_y/TILE_SIZE;int_y++){
                for(int int_x=0;int_x<level_x/TILE_SIZE;int_x++){
                    load >> seen;
                    viewable_map_data[int_x][int_y].seen=seen;
                }
            }

            load.close();
            load.clear();
        }
        else{
            //We have failed to load the map save data, so it must not have been saved yet.
            persistent_level_data=false;
            return;
        }

        //Load the map sprite data from the level files.

        //If the current sub level is 0, meaning "load the main level."
        if(current_viewable_sub_level==0){
            load_path="data/levels/"+current_level+"/tiles.blazelevel";
        }
        //If there is a current sub level to load instead of the main one.
        else{
            load_path="data/levels/"+current_level+"/"+current_sub_level+"/tiles.blazelevel";
        }
        load.open(load_path.c_str(),ifstream::in);

        if(load!=NULL){
            short sprite=0;
            string catch_unneeded="";

            for(int int_y=0;int_y<level_y/TILE_SIZE;int_y++){
                for(int int_x=0;int_x<level_x/TILE_SIZE;int_x++){
                    load>>catch_unneeded;
                    load>>sprite;
                    load>>catch_unneeded;
                    load>>catch_unneeded;
                    load>>catch_unneeded;

                    viewable_map_data[int_x][int_y].sprite=sprite;
                }
            }

            load.close();
            load.clear();
        }

        if(!player.is_level_worldmap(current_viewable_level)){
            //If the current sub level is 0, meaning "load the main level."
            if(current_viewable_sub_level==0){
                load_path="data/levels/"+current_level+"/tiles_background.blazelevel";
            }
            //If there is a current sub level to load instead of the main one.
            else{
                load_path="data/levels/"+current_level+"/"+current_sub_level+"/tiles_background.blazelevel";
            }
            load.open(load_path.c_str(),ifstream::in);

            if(load!=NULL){
                short sprite=0;

                for(int int_y=0;int_y<level_y/TILE_SIZE;int_y++){
                    for(int int_x=0;int_x<level_x/TILE_SIZE;int_x++){
                        load>>sprite;

                        viewable_map_data[int_x][int_y].background_sprite=sprite;
                    }
                }

                load.close();
                load.clear();
            }

            //If the current sub level is 0, meaning "load the main level."
            if(current_viewable_sub_level==0){
                load_path="data/levels/"+current_level+"/items.blazelevel";
            }
            //If there is a current sub level to load instead of the main one.
            else{
                load_path="data/levels/"+current_level+"/"+current_sub_level+"/items.blazelevel";
            }
            load.open(load_path.c_str(),ifstream::in);

            if(load!=NULL){
                short type=0;
                double x=0.0;
                double y=0.0;
                int goal_level_to_load=0;
                bool goal_secret=false;

                while(!load.eof()){
                    type=30000;
                    x=0.0;
                    y=0.0;
                    goal_level_to_load=0;
                    goal_secret=false;

                    load >> type;
                    load >> x;
                    load >> y;
                    load >> goal_level_to_load;
                    load >> goal_secret;

                    if(type!=30000){
                        viewable_items.push_back(Item(x,y,false,type,goal_level_to_load,goal_secret));
                    }
                }

                load.close();
                load.clear();
            }

            //Load the item seen data from the item save file.

            load_path=profile.get_home_directory()+"profiles/"+player.name+"/saves/"+current_level+"/items.blazesave";
            load.open(load_path.c_str(),ifstream::in);

            if(load!=NULL){
                bool collected=false;

                for(int i=0;i<viewable_items.size();i++){
                    //If the item is of a collectable type.
                    if(profile.is_item_collectable(viewable_items[i].type)){
                        load>>collected;
                        if(collected){
                            viewable_items[i].exists=false;
                        }
                    }
                    else{
                        if(viewable_items[i].type!=ITEM_SPAWNPOINT && viewable_items[i].type!=ITEM_CHECKPOINT && viewable_items[i].type!=ITEM_ENDPOINT){
                            viewable_items[i].exists=false;
                        }
                    }
                }

                load.close();
                load.clear();
            }
        }
    }
}

void Window_Map::display_map(){
    //If the level data for this level is persistent, there is a map to view.
    if(persistent_level_data){
        render_rectangle(x+10,y+35,map_camera_w,map_camera_h,1.0,COLOR_PALE_BROWN);

        //Render each onscreen tile.
        for(int int_y=0;int_y<level_y/TILE_SIZE;int_y++){
            for(int int_x=0;int_x<level_x/TILE_SIZE;int_x++){
                //As long as the current tile is within the level, camera, and map boundaries.
                if(int_x>=0 && int_x<=(level_x/TILE_SIZE)-1 && int_y>=0 && int_y<=(level_y/TILE_SIZE)-1 &&
                   int_x*16>=map_camera_x-16 && int_x*16<=map_camera_x+map_camera_w && int_y*16>=map_camera_y-16 && int_y*16<=map_camera_y+map_camera_h &&
                   (int)(int_x*16-map_camera_x)>=x+10 && (int)(int_x*16+TILE_SIZE/2-map_camera_x)<=x+10+map_camera_w && 35+(int)(int_y*16-map_camera_y)>=y+35 && 35+(int)(int_y*16+TILE_SIZE/2-map_camera_y)<=y+35+map_camera_h){

                    if(viewable_map_data[int_x][int_y].seen){
                        //If the current level is the world map.
                        if(player.is_level_worldmap(current_viewable_level)){
                            if(viewable_map_data[int_x][int_y].sprite!=0){
                                //If the tile is a water tile.
                                if(viewable_map_data[int_x][int_y].sprite==1 || viewable_map_data[int_x][int_y].sprite==2){
                                    render_sprite((int)((int)int_x*16-(int)map_camera_x),35+(int)((int)int_y*16-(int)map_camera_y),viewable_water_tiles,&sprites_tiles[viewable_map_data[int_x][int_y].sprite],0.75,0.5,0.5);
                                }
                                else{
                                    render_sprite((int)((int)int_x*16-(int)map_camera_x),35+(int)((int)int_y*16-(int)map_camera_y),viewable_tiles,&worldmap_sprites_tiles[viewable_map_data[int_x][int_y].sprite],1.0,0.5,0.5);
                                }
                            }
                        }
                        //If the current level is not the world map.
                        else{
                            if(viewable_map_data[int_x][int_y].background_sprite!=0){
                                render_sprite((int)((int)int_x*16-(int)map_camera_x),35+(int)((int)int_y*16-(int)map_camera_y),viewable_background_tiles,&sprites_tiles_background[viewable_map_data[int_x][int_y].background_sprite],1.0,0.5,0.5);
                            }
                            if(viewable_map_data[int_x][int_y].sprite!=0){
                                //If the tile is a water tile.
                                if(viewable_map_data[int_x][int_y].sprite==1 || viewable_map_data[int_x][int_y].sprite==2){
                                    render_sprite((int)((int)int_x*16-(int)map_camera_x),35+(int)((int)int_y*16-(int)map_camera_y),viewable_water_tiles,&sprites_tiles[viewable_map_data[int_x][int_y].sprite],0.75,0.5,0.5);
                                }
                                else{
                                    render_sprite((int)((int)int_x*16-(int)map_camera_x),35+(int)((int)int_y*16-(int)map_camera_y),viewable_tiles,&sprites_tiles[viewable_map_data[int_x][int_y].sprite],1.0,0.5,0.5);
                                }
                            }
                        }
                    }
                    else{
                        render_rectangle((int)((int)int_x*16-(int)map_camera_x),35+(int)((int)int_y*16-(int)map_camera_y),16,16,1.0,COLOR_BLACK);
                    }
                }
            }
        }

        if(!player.is_level_worldmap(current_viewable_level)){
            for(int i=0;i<viewable_items.size();i++){
                if(viewable_items[i].exists){
                    int current_x=(int)((int)viewable_items[i].x/TILE_SIZE);
                    int current_y=(int)((int)viewable_items[i].y/TILE_SIZE);

                    int check_x_start=current_x-4;
                    int check_x_end=current_x+4;
                    int check_y_start=current_y-4;
                    int check_y_end=current_y+4;

                    bool touching_seen_tile=false;

                    for(int int_y=check_y_start;int_y<=check_y_end && !touching_seen_tile;int_y++){
                        for(int int_x=check_x_start;int_x<=check_x_end && !touching_seen_tile;int_x++){
                            if(int_x>=0 && int_x<=(level_x/TILE_SIZE)-1 && int_y>=0 && int_y<=(level_y/TILE_SIZE)-1){
                                if(viewable_map_data[int_x][int_y].seen && collision_check(viewable_items[i].x,viewable_items[i].y,viewable_items[i].w,viewable_items[i].h,int_x*TILE_SIZE,int_y*TILE_SIZE,TILE_SIZE,TILE_SIZE)){
                                    touching_seen_tile=true;

                                    break;
                                }
                            }
                        }
                    }

                    if(touching_seen_tile){
                        if((int)(viewable_items[i].x/2-map_camera_x)>=x+10 && (int)(viewable_items[i].x/2+viewable_items[i].w/2-map_camera_x)<=x+10+map_camera_w && 35+(int)(viewable_items[i].y/2-map_camera_y)>=y+35 && 35+(int)(viewable_items[i].y/2+viewable_items[i].h/2-map_camera_y)<=y+35+map_camera_h){
                            image_data item_image=image.sprite_sheet_items;
                            if(viewable_items[i].type==ITEM_SPAWNPOINT || viewable_items[i].type==ITEM_CHECKPOINT || viewable_items[i].type==ITEM_ENDPOINT){
                                item_image=image.sprite_sheet_level_items;
                            }

                            int y_adjust=0;
                            if(viewable_items[i].type==ITEM_SPAWNPOINT){
                                y_adjust=-32;
                            }

                            render_sprite((int)((int)viewable_items[i].x/2-(int)map_camera_x),35+(int)(((int)viewable_items[i].y+y_adjust)/2-(int)map_camera_y),item_image,viewable_items[i].get_texture_clip(true),1.0,0.5,0.5,0.0,viewable_items[i].color);
                        }
                    }
                }
            }
        }

        if(current_viewable_level==player.current_level && current_viewable_sub_level==player.current_sub_level){
            //Render the player marker.
            //As long as the player is within the map boundaries.
            if((int)(player.x/2-map_camera_x)>=x+10 && (int)(player.x/2+player.w/2-map_camera_x)<=x+10+map_camera_w && 35+(int)(player.y/2-map_camera_y)>=y+35 && 35+(int)(player.y/2+player.h/2-map_camera_y)<=y+35+map_camera_h){
                //If the current level is the world map.
                if(player.is_level_worldmap(current_viewable_level)){
                    render_sprite((int)((int)player.x/2-(int)map_camera_x),35+(int)((int)player.y/2-(int)map_camera_y),image.sprite_sheet_player_worldmap,&sprites_player_worldmap_idle_right[0],1.0,0.5,0.5);
                }
                //If the current level is not the world map.
                else{
                    render_sprite((int)((int)player.x/2-(int)map_camera_x),35+(int)((int)player.y/2-(int)map_camera_y),image.sprite_sheet_player,&sprites_player_idle_right[0],1.0,0.5,0.5);
                }
            }
        }
    }
    else{
        //Tell the player there is no map to display for this level.
        string message="You haven't been here yet!";
        font.show(x+(w-(message.length()*12))/2+2,y+(h-12)/2+2,message,COLOR_BLACK,1.0);
        font.show(x+(w-(message.length()*12))/2,y+(h-12)/2,message,return_gui_color(holiday,3),1.0);
    }
}

void Window_Map::render(){
    if(on){
        //Render the border.
        render_rectangle(x,y,w,h,1.0,return_gui_color(holiday,0));

        //Render the background.
        render_rectangle(x+5,y+5,w-10,h-10,1.0,return_gui_color(holiday,1));

        //Render the title bar.
        render_rectangle(x+10,y+10,w-20,20,1.0,return_gui_color(holiday,2));

        //Display the window's title.
        font.show(x+(w-(title.length()*12))/2+2,y+12+2,title,COLOR_BLACK,1.0);
        font.show(x+(w-(title.length()*12))/2,y+12,title,return_gui_color(holiday,3),1.0);

        display_map();

        //Render the buttons.
        for(int i=0;i<buttons.size();i++){
            buttons[i].render(x,y,i);
        }
    }
}
