/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef moused_slot_h
#define moused_slot_h

#include <string>

struct inventory_item{
    //The item's type.
    short type;
    //The slot that the item occupies in the inventory window.
    short slot;
    //The name of the item.
    std::string name;
};

struct dragged_inventory_item: inventory_item{
    //Mouse offsets.
    short offset_x;
    short offset_y;
};

struct moused_slot{
    short slot;
    short offset_x;
    short offset_y;
};

#endif
