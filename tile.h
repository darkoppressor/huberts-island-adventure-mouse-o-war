/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef tile_h
#define tile_h

#include "tile_sprites.h"
#include "sprite_sheets.h"
#include "enumerations.h"
#include "light_data.h"

class Tile{
    private:
    public:
    Tile();

    //Return the corresponding coordinate or dimension, adjusted for the object's hitbox size.
    //Currently only used for hazard tiles.
    double return_x();
    double return_y();
    double return_w();
    double return_h();

    void set_height_mask();
    short get_height_mask(short x_position);

    //Render the tile to the screen.
    void render();

    void process_glow();

    //The coordinates of the tile. These are used for collision detection.
    double x,y;

    //Here we have the variables for storing the tile's properties. By mixing these different properties, all sorts of cool things are possible.
    //For instance, you could make an invisible but solid tile with tile properties '1 0 0'.

    //Governs whether the tile is completely passable, solid, or solid only on top.
    short solidity;

    //Governs the look of the tile. This corresponds to the tileset of the current level.
    short sprite;

    //Special tile properties.
    short special;

    //Governs whether or not the tile is a foreground tile.
    //A foreground tile will be rendered on top of everything else (player, items, npcs, etc.)
    bool foreground;

    //The angle of the ground.
    //If not 180, this is a sloping ground tile.
    //If 180, this is just flat ground.
    short slope;

    //The height mask of the tile for collision detection.
    ///short height_mask[TILE_SIZE];

    //The object's light source data.
    Light_Source light_source;

    //If true, dimness is rising.
    //If false, dimness is falling.
    bool glow_direction;

    //The rate of glow change.
    double glow_rate;

    unsigned short glow_counter;
};

#endif
