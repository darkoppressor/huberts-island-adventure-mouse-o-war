/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef spawner_h
#define spawner_h

class Spawner{
    private:
    public:

    //The kind of object this spawner spawns.
    short spawner_type;

    short type;

    double x;
    double y;

    //If true, only allows a spawn if an identical object is NOT in the way.
    bool disallow_doubles;

    //Instead of falling when spawned, items stay in the air.
    bool items_stay;

    Spawner(short get_spawner_type,short get_type,double get_x,double get_y,bool get_disallow_doubles,bool get_items_stay);

    bool allow_spawn();

    void spawn_object(short object_type);
};

#endif
