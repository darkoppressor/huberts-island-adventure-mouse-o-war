/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef window_inventory_h
#define window_inventory_h

#include <SDL.h>

#include "inventory.h"
#include "window.h"

class Window_Inventory: public Window{
    private:
    public:
    Window_Inventory(short get_x,short get_y,short get_w,short get_h,std::string get_title);

    //Toggles the 'on' state of the window.
    void toggle_on();

    //Sets the 'on' state of the window to false.
    void turn_off();

    void display_inventory_item(short type,short slot);

    //Show the dragged item, if there is one.
    void display_dragged_item();

    void drop_dragged_item();

    void handle_input_states();

    void handle_input_events();

    void render();

    //Returns the slot that the mouse is currently over.
    //If the mouse is not over any slot, returns -1.
    moused_slot determine_moused_slot(int mouse_x,int mouse_y);

    struct slot_data{
        short x,y;
        short slot;
    };
    slot_data slots[10][10];

    //The dimensions in pixels of each item slot in the inventory window.
    short slot_w,slot_h;

    short INVENTORY_MAX_SIZE;
};

#endif
