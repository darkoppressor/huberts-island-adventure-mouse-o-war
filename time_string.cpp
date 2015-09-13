/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "time_string.h"
#include "world.h"

using namespace std;

string get_time_string_short(unsigned long seconds){
    string text="";

    unsigned long minutes=0;
    unsigned long hours=0;

    for(int i=seconds;i>=60;){
        minutes++;
        i-=60;
        if(i<60){
            seconds=i;
        }
    }
    for(int i=minutes;i>=60;){
        hours++;
        i-=60;
        if(i<60){
            minutes=i;
        }
    }

    if(hours<10){
        ss.clear();ss.str("");ss<<"0";text+=ss.str();
    }
    ss.clear();ss.str("");ss<<hours;text+=ss.str();
    ss.clear();ss.str("");ss<<":";text+=ss.str();

    if(minutes<10){
        ss.clear();ss.str("");ss<<"0";text+=ss.str();
    }
    ss.clear();ss.str("");ss<<minutes;text+=ss.str();
    ss.clear();ss.str("");ss<<":";text+=ss.str();

    if(seconds<10){
        ss.clear();ss.str("");ss<<"0";text+=ss.str();
    }
    ss.clear();ss.str("");ss<<seconds;text+=ss.str();

    return text;
}

string get_time_string_long(unsigned long seconds){
    string text="";

    unsigned long minutes=0;
    unsigned long hours=0;
    unsigned long days=0;
    unsigned long weeks=0;
    unsigned long months=0;
    unsigned long years=0;

    for(unsigned long i=seconds;i>=60;){
        minutes++;
        i-=60;
        if(i<60){
            seconds=i;
        }
    }
    for(unsigned long i=minutes;i>=60;){
        hours++;
        i-=60;
        if(i<60){
            minutes=i;
        }
    }
    for(unsigned long i=hours;i>=24;){
        days++;
        i-=24;
        if(i<24){
            hours=i;
        }
    }
    for(unsigned long i=days;i>=7;){
        weeks++;
        i-=7;
        if(i<7){
            days=i;
        }
    }
    for(unsigned long i=weeks;i>=4;){
        months++;
        i-=4;
        if(i<4){
            weeks=i;
        }
    }
    for(unsigned long i=months;i>=12;){
        years++;
        i-=12;
        if(i<12){
            months=i;
        }
    }

    if(years>0){
        ss.clear();ss.str("");ss<<years;text+=ss.str();
        text+=" year";
        if(years!=1){
            text+="s";
        }
        text+=", ";
    }

    if(months>0){
        ss.clear();ss.str("");ss<<months;text+=ss.str();
        text+=" month";
        if(months!=1){
            text+="s";
        }
        text+=", ";
    }

    if(weeks>0){
        ss.clear();ss.str("");ss<<weeks;text+=ss.str();
        text+=" week";
        if(weeks!=1){
            text+="s";
        }
        text+=", ";
    }

    if(days>0){
        ss.clear();ss.str("");ss<<days;text+=ss.str();
        text+=" day";
        if(days!=1){
            text+="s";
        }
        text+=", ";
    }

    if(hours>0){
        ss.clear();ss.str("");ss<<hours;text+=ss.str();
        text+=" hour";
        if(hours!=1){
            text+="s";
        }
        text+=", ";
    }

    if(minutes>0){
        ss.clear();ss.str("");ss<<minutes;text+=ss.str();
        text+=" minute";
        if(minutes!=1){
            text+="s";
        }
        if(seconds>0){
            text+=", ";
        }
    }

    if(seconds>0){
        ss.clear();ss.str("");ss<<seconds;text+=ss.str();
        text+=" second";
        if(seconds!=1){
            text+="s";
        }
    }

    return text;
}
