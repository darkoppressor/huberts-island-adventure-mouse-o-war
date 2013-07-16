/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef trap_h
#define trap_h

#include "image_data.h"
#include "enumerations.h"
#include "light_data.h"
#include "sound_data.h"

#include <vector>

#include <SDL_mixer.h>

//Incomplete declaration of Npc.
class Npc;

class Trap{
    private:
    public:

    Trap(double get_x,double get_y,short get_type,bool get_active,short get_identifier,short get_move_state=NONE,double get_move_speed=1.0,double get_circle_radius=0.0);

    //Return the corresponding coordinate or dimension, adjusted for the object's hitbox size.
    double return_x();
    double return_y();
    double return_w();
    double return_h();
    double return_danger_x();
    double return_danger_y();
    double return_danger_w();
    double return_danger_h();

    bool is_sharp();

    void move();

    void handle_events();

    void animate();

    void render(bool mirrored=false);

    void toggle_active();

    void toggle_movement();

    void trigger_special_function();

    //Special functions:
    void special_function_alarm_toggler_toggle();
    void special_function_weather_machine();
    void special_function_open_shop();
    void special_function_open_upgrades();

    //The size modifier of the hitbox.
    short hitbox_size_modifier;

    //Index in the npcs vector for the boss this trap is associated with.
    int boss_index;

    //If true, the trap is not rendered.
    //If false, the trap will render like normal.
    bool invisible;

    //The type of trap.
    //See trap_data.h for a full list of trap types.
    short type;

    //If true, this trap is asscoiated with a boss.
    //If false, it isn't.
    bool BOSS;

    //If true, this trap is solid anywhere it is not dangerous.
    //If false, this trap has no solidity.
    bool solid;

    //The effect a trigger has on this trap.
    //false = toggle active
    //true = cause trap to do something (boulder falls, etc.)
    bool trigger_effect;

    //If true, the trap can be turned off in toggle_active().
    //If false, it can only be turned on in toggle_active().
    bool can_be_turned_off;

    //The type of special function.
    short special_function_type;

    //The type of danger.
    //0 = Always dangerous.
    //1 = Dangerous on a timer.
    short danger_type;

    //The type of movement.
    //0 = Immobile.
    //1 = Always moving.
    //2 = Same as 1, but has a reset distance and reset speed/direction.
    //3 = Movement on a timer.
    short movement_type;

    //If true, the trap is resetting.
    //If false, the trap is not resetting.
    bool resetting;

    //The distance a trap will travel before resetting.
    short reset_distance;

    //The direction and speed the trap travels when resetting.
    short reset_move_state;
    double reset_move_speed;

    //If true, upon finishing a reset the trap immediately begins moving again.
    //If false, the trap resets normally.
    bool reset_perpetual;

    //How far the trap has moved since last changing its resetting status.
    short distance_moved;

    //If danger_type==1, this counter determines how frequently the trap toggles dangerous.
    short danger_counter;
    short danger_counter_max;

    //If movement_type==1, this counter determines how frequently the trap moves.
    short movement_counter;
    short movement_counter_max;

    //If true, the danger area of this trap will be deadly.
    //If false, the danger area will be harmless.
    bool dangerous;

    //If true, the trap acts according to its danger_type and movement_type.
    //If false, the trap is harmless and does nothing.
    bool active;

    //If true, the trap is moving.
    //If false, the trap is not moving.
    bool moving;

    //The direction the trap moves when moving==true.
    short move_state;

    //The speed at which the trap moves.
    double move_speed;

    //If true, the trap moves in a circle.
    //If false, it doesn't.
    bool movement_circle;

    //The coords the trap circles around.
    double circle_center_x;
    double circle_center_y;

    //The angle of circle movement.
    double circle_angle;

    //The radius of circle movement.
    double circle_radius;

    //Coordinate offsets for the dangerous zone.
    double danger_x,danger_y;

    //The dimensions of the dangerous zone.
    double danger_w,danger_h;

    //Coordinates.
    double x,y;

    //Dimensions.
    double w,h;

    short identifier;

    //Animation frame and frame counter.
    short frame,frame_counter,frame_speed;
    short frame_idle_active,frame_counter_idle_active,frame_speed_idle_active;
    short frame_idle_inactive,frame_counter_idle_inactive,frame_speed_idle_inactive;

    //These will be set to the relevant sprite numbers for the current trap type upon this trap's creation.
    short TRAP_MOVE_SPRITES;
    short TRAP_IDLE_ACTIVE_SPRITES;
    short TRAP_IDLE_INACTIVE_SPRITES;

    image_data* sprite_sheet_image_trap;
    std::vector<SDL_Rect> vector_sprites_move;
    std::vector<SDL_Rect> vector_sprites_idle_active;
    std::vector<SDL_Rect> vector_sprites_idle_inactive;

    sound_data* ptr_sound_danger_on;
    sound_data* ptr_sound_danger_off;
    sound_data* ptr_sound_active_on;
    sound_data* ptr_sound_reset_movement_start;

    //The object's light source data.
    Light_Source light_source;

    //If true, dangerous change toggles light.
    //If false, it doesn't.
    bool dangerous_toggles_light;

    //If true, glow.
    //If false, do not glow.
    bool glow;

    //If true, dimness is rising.
    //If false, dimness is falling.
    bool glow_direction;

    //The rate of glow change.
    double glow_rate;

    unsigned short glow_counter;
};

#endif
