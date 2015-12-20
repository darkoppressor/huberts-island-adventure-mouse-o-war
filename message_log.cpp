/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "message_log.h"
#include "world.h"

#include <ctime>
#include <cstdio>
#include <fstream>

using namespace std;

void update_error_log(string message,bool allow_save){
    time_t now;
    struct tm *tm_now;
    char buff[BUFSIZ];
    now=time(NULL);
    tm_now=localtime(&now);
    strftime(buff,sizeof buff,"[%Y-%m-%d %H:%M:%S] ",tm_now);

    message=buff+message;

    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,SDL_LOG_PRIORITY_ERROR,message.c_str());

    if(allow_save){
        //Append this message to the error log.
        string save_location=profile.get_home_directory()+"error_log.txt";
        ofstream save_log(save_location.c_str(),ifstream::app);
        if(save_log!=NULL){
            save_log<<message<<"\n";

            save_log.close();
            save_log.clear();
        }
    }
}

void update_survival_log(string message,bool multiplayer){
    time_t now;
    struct tm *tm_now;
    char buff[BUFSIZ];
    now=time(NULL);
    tm_now=localtime(&now);
    strftime(buff,sizeof buff,"[%Y-%m-%d %H:%M:%S] ",tm_now);

    message=buff+message;

    string log_file="sp_survival_log.txt";
    if(multiplayer){
        log_file="mp_survival_log.txt";
    }

    //Append this message to the error log.
    string save_location=profile.get_home_directory()+log_file;
    ofstream save_log(save_location.c_str(),ifstream::app);
    if(save_log!=NULL){
        save_log<<message<<"\n\n";

        save_log.close();
        save_log.clear();
    }
}
