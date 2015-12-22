/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef collision_h
#define collision_h

#include <algorithm>
#include <stdint.h>
#include <cmath>

const double ENGINE_MATH_PI=3.14159265358979323846;
const double ENGINE_MATH_E=2.71828182845904523536;

//Checks for a collision between two rectangles.
bool collision_check(double a_x,double a_y,double a_w,double a_h,double b_x,double b_y,double b_w,double b_h);

double degrees_to_radians(double degrees);
double radians_to_degrees(double radians);

template<typename T>
class Collision_Rect{
public:

    T x;
    T y;
    T w;
    T h;

    Collision_Rect(){
        x=(T)0;
        y=(T)0;
        w=(T)0;
        h=(T)0;
    }

    Collision_Rect(T get_x,T get_y,T get_w,T get_h){
        x=get_x;
        y=get_y;
        w=get_w;
        h=get_h;
    }

    Collision_Rect<T> operator*(T scalar){
        return Collision_Rect(x*scalar,y*scalar,w*scalar,h*scalar);
    }

    Collision_Rect<T> operator/(T scalar){
        return Collision_Rect(x/scalar,y/scalar,w/scalar,h/scalar);
    }

    T center_x() const{
        return x+w/(T)2;
    }

    T center_y() const{
        return y+h/(T)2;
    }
};

template<typename T>
class Collision_Circ{
public:

    T x;
    T y;
    T r;

    Collision_Circ(){
        x=(T)0;
        y=(T)0;
        r=(T)0;
    }

    Collision_Circ(T get_x,T get_y,T get_r){
        x=get_x;
        y=get_y;
        r=get_r;
    }

    Collision_Circ<T> operator*(T scalar){
        return Collision_Circ(x*scalar,y*scalar,r*scalar);
    }

    Collision_Circ<T> operator/(T scalar){
        return Collision_Circ(x/scalar,y/scalar,r/scalar);
    }
};

class Collision{
public:

    template<typename T>
    static bool check_rect(Collision_Rect<T> box_a,Collision_Rect<T> box_b){
        if(box_a.y+box_a.h<box_b.y){
            return false;
        }
        else if(box_a.y>box_b.y+box_b.h){
            return false;
        }
        else if(box_a.x+box_a.w<box_b.x){
            return false;
        }
        else if(box_a.x>box_b.x+box_b.w){
            return false;
        }

        return true;
    }

    template<typename T>
    static bool check_circ(Collision_Circ<T> circle_a,Collision_Circ<T> circle_b){
        T a=circle_a.r+circle_b.r;

        T dx=circle_a.x-circle_b.x;
        T dy=circle_a.y-circle_b.y;

        return a*a>(dx*dx)+(dy*dy);
    }

    template<typename T>
    static bool check_circ_rect(Collision_Circ<T> circle,Collision_Rect<T> box){
        T closest_x=(T)0;
        T closest_y=(T)0;

        if(circle.x<box.x){
            closest_x=box.x;
        }
        else if(circle.x>box.x+box.w){
            closest_x=box.x+box.w;
        }
        else{
            closest_x=circle.x;
        }

        if(circle.y<box.y){
            closest_y=box.y;
        }
        else if(circle.y>box.y+box.h){
            closest_y=box.y+box.h;
        }
        else{
            closest_y=circle.y;
        }

        T dx=circle.x-closest_x;
        T dy=circle.y-closest_y;

        return circle.r*circle.r>(dx*dx)+(dy*dy);
    }

    //Returns a rectangle containing the collision area of the two passed rectangles
    template<typename T>
    static Collision_Rect<T> get_collision_area_rect(Collision_Rect<T> box_a,Collision_Rect<T> box_b){
        if(check_rect(box_a,box_b)){
            Collision_Rect<T> box;

            box.x=max(box_a.x,box_b.x);
            box.y=max(box_a.y,box_b.y);
            box.w=min(box_a.x+box_a.w,box_b.x+box_b.w)-box.x;
            box.h=min(box_a.y+box_a.h,box_b.y+box_b.h)-box.y;

            return box;
        }
        else{
            Collision_Rect<T> box;

            return box;
        }
    }

    static double get_angle_to_rect(Collision_Rect<double> box_a,Collision_Rect<double> box_b){
        double x1=box_a.center_x();
        double y1=box_a.center_y();

        double x2=box_b.center_x();
        double y2=box_b.center_y();

        double x_component=abs(x2-x1);
        double y_component=abs(y2-y1);

        if(x2<x1){
            x_component*=-1.0;
        }

        double angle=radians_to_degrees(atan2(y_component,x_component));

        if(y2>y1){
            angle=360.0-angle;
        }

        return angle;
    }

    static double get_angle_to_rect(Collision_Circ<double> circle,Collision_Rect<double> box){
        double x1=circle.x;
        double y1=circle.y;

        double x2=box.center_x();
        double y2=box.center_y();

        double x_component=abs(x2-x1);
        double y_component=abs(y2-y1);

        if(x2<x1){
            x_component*=-1.0;
        }

        double angle=radians_to_degrees(atan2(y_component,x_component));

        if(y2>y1){
            angle=360.0-angle;
        }

        return angle;
    }

    static double get_angle_to_circ(Collision_Circ<double> circle_a,Collision_Circ<double> circle_b){
        double x1=circle_a.x;
        double y1=circle_a.y;

        double x2=circle_b.x;
        double y2=circle_b.y;

        double x_component=abs(x2-x1);
        double y_component=abs(y2-y1);

        if(x2<x1){
            x_component*=-1.0;
        }

        double angle=radians_to_degrees(atan2(y_component,x_component));

        if(y2>y1){
            angle=360.0-angle;
        }

        return angle;
    }

    static double get_angle_to_circ(Collision_Rect<double> box,Collision_Circ<double> circle){
        double x1=box.center_x();
        double y1=box.center_y();

        double x2=circle.x;
        double y2=circle.y;

        double x_component=abs(x2-x1);
        double y_component=abs(y2-y1);

        if(x2<x1){
            x_component*=-1.0;
        }

        double angle=radians_to_degrees(atan2(y_component,x_component));

        if(y2>y1){
            angle=360.0-angle;
        }

        return angle;
    }
};

class Collision_Event{
public:

    uint32_t object1;
    uint32_t object2;

    Collision_Event();
    Collision_Event(uint32_t get_object1,uint32_t get_object2);

    bool operator==(const Collision_Event& collision1) const;
};

#endif
