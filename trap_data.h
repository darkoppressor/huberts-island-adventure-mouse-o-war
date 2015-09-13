/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef trap_data_h
#define trap_data_h

#include <vector>

//Trap types:

enum{
    //0 is reserved for "no trap".
    TRAP_RESERVED,
    TRAP_FIRE,
    TRAP_SPIKES,
    TRAP_ICICLE,
    TRAP_BRANCH,
    TRAP_ELECTRIC,
    TRAP_STONESLAB,
    TRAP_BUZZSAW,
    TRAP_ALARM_TOGGLER,
    TRAP_WEATHER_MACHINE,
    TRAP_SPIKES_UP,
    TRAP_SPIKES_DOWN,
    TRAP_SPIKES_RIGHT,
    TRAP_SPIKES_LEFT,
    TRAP_BOSS_CASTLE,
    TRAP_BOSS_PYRAMID,
    TRAP_SEAWEED,
    TRAP_OPEN_SHOP,
    TRAP_OPEN_UPGRADES,
    TRAP_END
};

//Trap Special Functions:

const short SPECIAL_FUNCTION_NONE=-1;

enum{
    SPECIAL_FUNCTION_ALARM_TOGGLER_TOGGLE,
    SPECIAL_FUNCTION_WEATHER_MACHINE,
    SPECIAL_FUNCTION_OPEN_SHOP,
    SPECIAL_FUNCTION_OPEN_UPGRADES
};

struct trap_sprite_count_data{
    short move_sprites;
    short idle_active_sprites;
    short idle_inactive_sprites;
    double width;
    double height;
};

extern std::vector<trap_sprite_count_data> TRAP_SPRITE_COUNTS;

void prepare_trap_sprite_counts();

#endif
