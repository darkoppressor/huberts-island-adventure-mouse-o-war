/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "sign_compare.h"

using namespace std;

bool signs_same(double a,double b){
    if(a<0.0 && b<0.0){
        return true;
    }
    if(a>0.0 && b>0.0){
        return true;
    }
    if(a==0.0 && b==0.0){
        return true;
    }

    return false;
}
