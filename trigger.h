/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef trigger_h
#define trigger_h

#include <vector>

struct target{
    //The type of game object that this trigger targets.
    //0 = Moving platform.
    //1 = Door.
    //2 = Trap.
    short type;

    //The unique identifier of this target.
    short identifier;
};

class Trigger{
    private:
    public:
    Trigger(double get_x,double get_y,double get_w,double get_h,std::vector<target> get_targets,bool get_trigger_method,short get_user_type,bool get_repeating,short get_repeat_time,short get_render_trigger);

    void use();

    //Returns true if this trigger targets any traps of the passed type.
    bool targets_trap(short trap_type);

    //Handle collision checks.
    void handle_events();

    //Animate the sprite.
    void animate();

    //Render the sprite.
    void render(bool mirrored=false);

    //This trigger's active state.
    bool active;

    std::vector<target> targets;

    //What causes this trigger to change states.
    //False = Manual activation.
    //True = Touch.
    bool trigger_method;

    //What kind of game objects can use this trigger.
    short user_type;

    //If true, the trigger will remain active after use.
    //If false, the trigger will become inactive after use.
    bool repeating;

    //If repeat is true, this is the number of frames to wait before resetting and becoming active again after use.
    short repeat_time;
    //The repeat timer. Increments until it equals repeat_time.
    short repeat_current;

    //If 0, this trigger will not be rendered.
    //If non-zero, this trigger will be rendered, and the number will determine the sprite.
    short render_trigger;

    //Current coordinates.
    double x,y;

    //Dimensions.
    double w,h;

    //Animation frame and frame counter.
    short frame,frame_counter;
};

#endif
