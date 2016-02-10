/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "touch_controller.h"
#include "world.h"
#include "render.h"

using namespace std;

Touch_Controller::Touch_Controller(){
    touch_controller_xy=true;
    touch_controller_guide=true;
    touch_controller_shoulders=true;

    touch_finger_size=4.0;
    touch_controller_opacity=0.5;

    scale(1280.0,720.0);
}

void Touch_Controller::scale(double resolution_x,double resolution_y){
    //The native resolution of the touch controller.
    double native_x=1280.0;
    double native_y=720.0;

    double scale_x=resolution_x/native_x;
    double scale_y=resolution_y/native_y;
    double scale_mean=(scale_x+scale_y)/2.0;

    double anchor_r_dpad=192.0*scale_mean;
    double anchor_r_main=192.0*scale_mean;
    if(!touch_controller_xy){
        anchor_r_main=128.0*scale_mean;
    }
    double anchor_w_middle=464.0*scale_x;
    if(!touch_controller_guide){
        anchor_w_middle=304.0*scale_x;
    }
    double anchor_h_middle=96.0*scale_y;

    scale_anchor_dpad=scale_mean;
    scale_anchor_main=scale_mean;
    scale_shoulders=scale_mean;
    scale_anchor_middle_x=scale_x;
    scale_anchor_middle_y=scale_y;

    double button_size_standard=64.0*scale_mean;
    double button_w_middle=132.0*scale_x;
    double button_h_middle=44.0*scale_y;
    double button_size_shoulder=64.0*scale_mean;

    double middle_padding=6.0*scale_x;

    // Anchors //

    anchor_dpad.r=anchor_r_dpad;
    anchor_dpad.x=anchor_r_dpad;
    anchor_dpad.y=resolution_y-anchor_r_dpad;

    anchor_main.r=anchor_r_main;
    anchor_main.x=resolution_x-anchor_r_main;
    anchor_main.y=resolution_y-anchor_r_main;

    anchor_middle.w=anchor_w_middle;
    anchor_middle.h=anchor_h_middle;
    anchor_middle.x=(resolution_x-anchor_w_middle)/2.0;
    anchor_middle.y=resolution_y-anchor_h_middle;

    // Dpad //

    double anchor_left_dpad=anchor_dpad.x-anchor_dpad.r;
    double anchor_top_dpad=anchor_dpad.y-anchor_dpad.r;

    double anchor_left_main=anchor_main.x-anchor_main.r;
    double anchor_top_main=anchor_main.y-anchor_main.r;

    dpad_left.r=button_size_standard;
    dpad_left.x=anchor_left_dpad+button_size_standard;
    dpad_left.y=anchor_top_dpad+anchor_r_dpad;

    dpad_right.r=button_size_standard;
    dpad_right.x=anchor_left_dpad+(anchor_r_dpad*2.0)-button_size_standard;
    dpad_right.y=anchor_top_dpad+anchor_r_dpad;

    dpad_up.r=button_size_standard;
    dpad_up.x=anchor_left_dpad+anchor_r_dpad;
    dpad_up.y=anchor_top_dpad+button_size_standard;

    dpad_down.r=button_size_standard;
    dpad_down.x=anchor_left_dpad+anchor_r_dpad;
    dpad_down.y=anchor_top_dpad+(anchor_r_dpad*2.0)-button_size_standard;

    // Dpad Diagonals //

    dpad_left_up.r=button_size_standard;
    dpad_left_up.x=anchor_left_dpad+anchor_r_dpad/2.0;
    dpad_left_up.y=anchor_top_dpad+anchor_r_dpad/2.0;

    dpad_up_right.r=button_size_standard;
    dpad_up_right.x=anchor_left_dpad+(anchor_r_dpad*1.5);
    dpad_up_right.y=anchor_top_dpad+anchor_r_dpad/2.0;

    dpad_right_down.r=button_size_standard;
    dpad_right_down.x=anchor_left_dpad+(anchor_r_dpad*1.5);
    dpad_right_down.y=anchor_top_dpad+(anchor_r_dpad*1.5);

    dpad_down_left.r=button_size_standard;
    dpad_down_left.x=anchor_left_dpad+anchor_r_dpad/2.0;
    dpad_down_left.y=anchor_top_dpad+(anchor_r_dpad*1.5);

    // Main Buttons //

    main_x.r=button_size_standard;
    main_x.x=anchor_left_main+button_size_standard;
    main_x.y=anchor_top_main+anchor_r_main;

    main_b.r=button_size_standard;
    if(touch_controller_xy){
        main_b.x=anchor_left_main+(anchor_r_main*2.0)-button_size_standard;
        main_b.y=anchor_top_main+anchor_r_main;
    }
    else{
        main_b.x=anchor_left_main+(anchor_r_main*2.0)-button_size_standard;
        main_b.y=anchor_top_main+button_size_standard;
    }

    main_y.r=button_size_standard;
    main_y.x=anchor_left_main+anchor_r_main;
    main_y.y=anchor_top_main+button_size_standard;

    main_a.r=button_size_standard;
    if(touch_controller_xy){
        main_a.x=anchor_left_main+anchor_r_main;
        main_a.y=anchor_top_main+(anchor_r_main*2.0)-button_size_standard;
    }
    else{
        main_a.x=anchor_left_main+button_size_standard;
        main_a.y=anchor_top_main+(anchor_r_main*2.0)-button_size_standard;
    }

    // Main Button Diagonals //

    main_x_y.r=button_size_standard;
    main_x_y.x=anchor_left_main+anchor_r_main/2.0;
    main_x_y.y=anchor_top_main+anchor_r_main/2.0;

    main_y_b.r=button_size_standard;
    main_y_b.x=anchor_left_main+(anchor_r_main*1.5);
    main_y_b.y=anchor_top_main+anchor_r_main/2.0;

    main_b_a.r=button_size_standard;
    main_b_a.x=anchor_left_main+(anchor_r_main*1.5);
    main_b_a.y=anchor_top_main+(anchor_r_main*1.5);

    main_a_x.r=button_size_standard;
    main_a_x.x=anchor_left_main+anchor_r_main/2.0;
    main_a_x.y=anchor_top_main+(anchor_r_main*1.5);

    // Shoulder Buttons //

    shoulder_left.r=button_size_shoulder;
    shoulder_left.x=button_size_shoulder;
    shoulder_left.y=anchor_top_dpad-button_size_shoulder;

    shoulder_right.r=button_size_shoulder;
    shoulder_right.x=resolution_x-button_size_shoulder;
    shoulder_right.y=anchor_top_dpad-button_size_shoulder;

    // Middle Buttons //

    back_button.w=button_w_middle;
    back_button.h=button_h_middle;
    back_button.x=anchor_middle.x+middle_padding;
    back_button.y=anchor_middle.y+(anchor_h_middle-button_h_middle)/2.0;

    start.w=button_w_middle;
    start.h=button_h_middle;
    start.x=anchor_middle.x+anchor_w_middle-middle_padding-button_w_middle;
    start.y=anchor_middle.y+(anchor_h_middle-button_h_middle)/2.0;

    guide.w=button_w_middle;
    guide.h=button_h_middle;
    guide.x=anchor_middle.x+(anchor_w_middle-button_w_middle)/2.0;
    guide.y=anchor_middle.y+(anchor_h_middle-button_h_middle)/2.0;
}

bool Touch_Controller::check_button_state(SDL_Scancode button){
    for(int i=0;i<SDL_GetNumTouchDevices();i++){
        SDL_TouchID touch_device_id=SDL_GetTouchDevice(i);

        if(touch_device_id!=0){
            for(int j=0;j<SDL_GetNumTouchFingers(touch_device_id);j++){
                SDL_Finger* touch_finger=SDL_GetTouchFinger(touch_device_id,j);

                if(touch_finger!=0){
                    vector<SDL_Scancode> touch_buttons=check_for_button_press(touch_finger->x,touch_finger->y);

                    for(int t=0;t<touch_buttons.size();t++){
                        if(touch_buttons[t]==button){
                            return true;
                        }
                    }
                }
            }
        }
    }

    return false;
}

vector<SDL_Scancode> Touch_Controller::check_for_button_press(float x,float y){
    SDL_Rect rect;
    main_window.get_renderer_viewport(&rect);

    float scale_x=0.0f;
    float scale_y=0.0f;
    main_window.get_renderer_scale(&scale_x,&scale_y);

    int actual_width=0;
    int actual_height=0;
    main_window.get_renderer_output_size(&actual_width,&actual_height);

    double touch_x=x*(float)actual_width;
    double touch_y=y*(float)actual_height;

    float offset_x=(float)rect.x*scale_x;
    float offset_y=(float)rect.y*scale_y;

    Collision_Circ<double> circle_touch(((float)touch_x-offset_x)/scale_x,((float)touch_y-offset_y)/scale_y,touch_finger_size);

    vector<SDL_Scancode> touch_buttons;

    if(Collision::check_circ(circle_touch,dpad_left)){
        touch_buttons.push_back(SDL_SCANCODE_A);
    }
    else if(Collision::check_circ(circle_touch,dpad_up)){
        touch_buttons.push_back(SDL_SCANCODE_W);
    }
    else if(Collision::check_circ(circle_touch,dpad_right)){
        touch_buttons.push_back(SDL_SCANCODE_D);
    }
    else if(Collision::check_circ(circle_touch,dpad_down)){
        touch_buttons.push_back(SDL_SCANCODE_S);
    }

    else if(Collision::check_circ(circle_touch,dpad_left_up)){
        touch_buttons.push_back(SDL_SCANCODE_A);
        touch_buttons.push_back(SDL_SCANCODE_W);
    }
    else if(Collision::check_circ(circle_touch,dpad_up_right)){
        touch_buttons.push_back(SDL_SCANCODE_W);
        touch_buttons.push_back(SDL_SCANCODE_D);
    }
    else if(Collision::check_circ(circle_touch,dpad_right_down)){
        touch_buttons.push_back(SDL_SCANCODE_D);
        touch_buttons.push_back(SDL_SCANCODE_S);
    }
    else if(Collision::check_circ(circle_touch,dpad_down_left)){
        touch_buttons.push_back(SDL_SCANCODE_S);
        touch_buttons.push_back(SDL_SCANCODE_A);
    }

    else if(touch_controller_xy && Collision::check_circ(circle_touch,main_x)){
        touch_buttons.push_back(SDL_SCANCODE_RETURN);
    }
    else if(touch_controller_xy && Collision::check_circ(circle_touch,main_y)){
        touch_buttons.push_back(SDL_SCANCODE_I);
    }
    else if(Collision::check_circ(circle_touch,main_b)){
        touch_buttons.push_back(SDL_SCANCODE_C);
    }
    else if(Collision::check_circ(circle_touch,main_a)){
        touch_buttons.push_back(SDL_SCANCODE_SPACE);
    }

    /**else if(Collision::check_circ(circle_touch,main_x_y)){
        if(touch_controller_xy){
            touch_buttons.push_back(SDL_SCANCODE_SPACE);
            touch_buttons.push_back(SDL_SCANCODE_SLASH);
        }
        else{
            touch_buttons.push_back(SDL_SCANCODE_RSHIFT);
            touch_buttons.push_back(SDL_SCANCODE_RCTRL);
        }
    }
    else if(touch_controller_xy && Collision::check_circ(circle_touch,main_y_b)){
        touch_buttons.push_back(SDL_SCANCODE_SLASH);
        touch_buttons.push_back(SDL_SCANCODE_RSHIFT);
    }
    else if(Collision::check_circ(circle_touch,main_b_a)){
        touch_buttons.push_back(SDL_SCANCODE_RSHIFT);
        touch_buttons.push_back(SDL_SCANCODE_RCTRL);
    }
    else if(touch_controller_xy && Collision::check_circ(circle_touch,main_a_x)){
        touch_buttons.push_back(SDL_SCANCODE_RCTRL);
        touch_buttons.push_back(SDL_SCANCODE_SPACE);
    }*/

    else if(touch_controller_shoulders && Collision::check_circ(circle_touch,shoulder_left)){
        touch_buttons.push_back(SDL_SCANCODE_L);
    }
    else if(touch_controller_shoulders && Collision::check_circ(circle_touch,shoulder_right)){
        touch_buttons.push_back(SDL_SCANCODE_R);
    }

    else if(Collision::check_circ_rect(circle_touch,back_button)){
        touch_buttons.push_back(SDL_SCANCODE_M);
    }
    else if(Collision::check_circ_rect(circle_touch,start)){
        touch_buttons.push_back(SDL_SCANCODE_P);
    }
    else if(touch_controller_guide && Collision::check_circ_rect(circle_touch,guide)){
        touch_buttons.push_back(SDL_SCANCODE_F10);
    }

    return touch_buttons;
}

void Touch_Controller::render(){
    int actual_width=DEFAULT_SCREEN_WIDTH;
    int actual_height=DEFAULT_SCREEN_HEIGHT;
    ///main_window.get_renderer_output_size(&actual_width,&actual_height);

    double scale_x=(double)main_window.SCREEN_WIDTH/(double)actual_width;
    double scale_y=(double)main_window.SCREEN_HEIGHT/(double)actual_height;
    double scale_mean=(scale_x+scale_y)/2.0;

    image_data* image_dpad=&image.touch_controller_dpad;
    render_texture(anchor_dpad.x*scale_x-anchor_dpad.r*scale_mean,anchor_dpad.y*scale_y-anchor_dpad.r*scale_mean,*image_dpad,touch_controller_opacity,COLOR_WHITE,scale_anchor_dpad*scale_mean,scale_anchor_dpad*scale_mean);

    if(touch_controller_xy){
        image_data* image_main=&image.touch_controller_main;
        render_texture(anchor_main.x*scale_x-anchor_main.r*scale_mean,anchor_main.y*scale_y-anchor_main.r*scale_mean,*image_main,touch_controller_opacity,COLOR_WHITE,scale_anchor_main*scale_mean,scale_anchor_main*scale_mean);
    }
    else{
        image_data* image_main=&image.touch_controller_main_small;
        render_texture(anchor_main.x*scale_x-anchor_main.r*scale_mean,anchor_main.y*scale_y-anchor_main.r*scale_mean,*image_main,touch_controller_opacity,COLOR_WHITE,scale_anchor_main*scale_mean,scale_anchor_main*scale_mean);
    }

    if(touch_controller_shoulders){
        image_data* image_shoulder_left=&image.touch_controller_shoulder_left;
        render_texture(shoulder_left.x*scale_x-shoulder_left.r*scale_mean,shoulder_left.y*scale_y-shoulder_left.r*scale_mean,*image_shoulder_left,touch_controller_opacity,COLOR_WHITE,scale_shoulders*scale_mean,scale_shoulders*scale_mean);

        image_data* image_shoulder_right=&image.touch_controller_shoulder_right;
        render_texture(shoulder_right.x*scale_x-shoulder_right.r*scale_mean,shoulder_right.y*scale_y-shoulder_right.r*scale_mean,*image_shoulder_right,touch_controller_opacity,COLOR_WHITE,scale_shoulders*scale_mean,scale_shoulders*scale_mean);
    }

    if(touch_controller_guide){
        image_data* image_middle=&image.touch_controller_middle;
        render_texture(anchor_middle.x*scale_x,anchor_middle.y*scale_y,*image_middle,touch_controller_opacity,COLOR_WHITE,scale_anchor_middle_x*scale_x,scale_anchor_middle_y*scale_y);
    }
    else{
        image_data* image_middle=&image.touch_controller_middle_small;
        render_texture(anchor_middle.x*scale_x,anchor_middle.y*scale_y,*image_middle,touch_controller_opacity,COLOR_WHITE,scale_anchor_middle_x*scale_x,scale_anchor_middle_y*scale_y);
    }
}
