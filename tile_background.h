/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef tile_background_h
#define tile_background_h

#include "tile_sprites.h"
#include "enumerations.h"

#include <SDL.h>

class Tile_Background{
    private:
    public:
    //The constructor takes the tile's coordinates, as well as its various properties.
    Tile_Background();

    //Render the tile to the screen.
    void render();

    //The coordinates of the tile. These are used for collision detection.
    int x,y;

    //Governs the look of the tile. This corresponds to the tileset of the current level.
    short sprite;
};

#endif
