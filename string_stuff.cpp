/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "string_stuff.h"

#include <algorithm>

using namespace std;

string make_string_lower_case(string string_input){
    string string_output="";
    char temp=0;

    for(int i=0;i<string_input.size();i++){
        temp=(char)tolower(string_input[i]);
        string_output+=temp;
    }

    return string_output;
}

string num_to_string(int number){
    string message="";

    stringstream strstream("");
    strstream.clear();strstream.str("");strstream<<number;message=strstream.str();

    return message;
}
