/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef tracer_h
#define tracer_h

#include <vector>

struct coords{
    double x;
    double y;
    short color;
};

class Tracer{
    private:
    public:

    std::vector<coords> tracers;

    short color;

    Tracer();

    void create_tracer(double x,double y);

    void handle_max_size();

    void clear_tracers();

    void render();
};

#endif
