/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef devworld_h
#define devworld_h

#include <vector>

struct devworld_data{
    double scale_x;
    double scale_y;

    bool scale_x_increasing;
    bool scale_y_increasing;
};

class DevWorld{
    private:
    public:

    std::vector< std::vector<devworld_data> > tiles;

    void clear_tiles();

    void setup(int level_x,int level_y);

    void process();
};

#endif
