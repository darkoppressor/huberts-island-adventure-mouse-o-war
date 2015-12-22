/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "collision.h"

using namespace std;

bool collision_check(double a_x,double a_y,double a_w,double a_h,double b_x,double b_y,double b_w,double b_h){
    //The sides of the rectangles:
    double left_a,left_b;
    double right_a,right_b;
    double top_a,top_b;
    double bottom_a,bottom_b;

    //Calculate the sides of rectangle A:
    left_a=a_x;
    right_a=a_x + a_w;
    top_a=a_y;
    bottom_a=a_y + a_h;

    //Calculate the sides of rectangle B:
    left_b=b_x;
    right_b=b_x + b_w;
    top_b=b_y;
    bottom_b=b_y + b_h;

    //Check each side of A.  If it is found to be outside of B, these two rectangles are not colliding, so return false.
    if(bottom_a<=top_b){
        return false;
    }
    if(top_a>=bottom_b){
        return false;
    }
    if(right_a<=left_b){
        return false;
    }
    if(left_a>=right_b){
        return false;
    }

    //If none of the sides from a are outside b, a collision has occured.
    return true;
}

double degrees_to_radians(double degrees){
    return degrees*(ENGINE_MATH_PI/180.0);
}

double radians_to_degrees(double radians){
    return radians*(180.0/ENGINE_MATH_PI);
}

Collision_Event::Collision_Event(){
    object1=0;
    object2=0;
}

Collision_Event::Collision_Event(uint32_t get_object1,uint32_t get_object2){
    object1=get_object1;
    object2=get_object2;
}

bool Collision_Event::operator==(const Collision_Event& collision1) const{
    return object1==collision1.object1 && object2==collision1.object2;
}
