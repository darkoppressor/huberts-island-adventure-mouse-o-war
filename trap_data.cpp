/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "trap_data.h"

using namespace std;

vector<trap_sprite_count_data> TRAP_SPRITE_COUNTS;

void prepare_trap_sprite_counts(){
    //Clear the trap sprite counts vector.
    TRAP_SPRITE_COUNTS.clear();

    for(short i=0;i<TRAP_END;i++){
        TRAP_SPRITE_COUNTS.push_back(trap_sprite_count_data());
    }

    //Fire
    TRAP_SPRITE_COUNTS[TRAP_FIRE].move_sprites=0;
    TRAP_SPRITE_COUNTS[TRAP_FIRE].idle_active_sprites=8;
    TRAP_SPRITE_COUNTS[TRAP_FIRE].idle_inactive_sprites=1;
    TRAP_SPRITE_COUNTS[TRAP_FIRE].width=16;
    TRAP_SPRITE_COUNTS[TRAP_FIRE].height=32;

    //Spikes
    TRAP_SPRITE_COUNTS[TRAP_SPIKES].move_sprites=0;
    TRAP_SPRITE_COUNTS[TRAP_SPIKES].idle_active_sprites=2;
    TRAP_SPRITE_COUNTS[TRAP_SPIKES].idle_inactive_sprites=0;
    TRAP_SPRITE_COUNTS[TRAP_SPIKES].width=32;
    TRAP_SPRITE_COUNTS[TRAP_SPIKES].height=32;

    //Icicle
    TRAP_SPRITE_COUNTS[TRAP_ICICLE].move_sprites=0;
    TRAP_SPRITE_COUNTS[TRAP_ICICLE].idle_active_sprites=6;
    TRAP_SPRITE_COUNTS[TRAP_ICICLE].idle_inactive_sprites=0;
    TRAP_SPRITE_COUNTS[TRAP_ICICLE].width=16;
    TRAP_SPRITE_COUNTS[TRAP_ICICLE].height=32;

    //Branch
    TRAP_SPRITE_COUNTS[TRAP_BRANCH].move_sprites=1;
    TRAP_SPRITE_COUNTS[TRAP_BRANCH].idle_active_sprites=0;
    TRAP_SPRITE_COUNTS[TRAP_BRANCH].idle_inactive_sprites=1;
    TRAP_SPRITE_COUNTS[TRAP_BRANCH].width=41;
    TRAP_SPRITE_COUNTS[TRAP_BRANCH].height=23;

    //Electric
    TRAP_SPRITE_COUNTS[TRAP_ELECTRIC].move_sprites=0;
    TRAP_SPRITE_COUNTS[TRAP_ELECTRIC].idle_active_sprites=20;
    TRAP_SPRITE_COUNTS[TRAP_ELECTRIC].idle_inactive_sprites=0;
    TRAP_SPRITE_COUNTS[TRAP_ELECTRIC].width=32;
    TRAP_SPRITE_COUNTS[TRAP_ELECTRIC].height=192;

    //Stone slab
    TRAP_SPRITE_COUNTS[TRAP_STONESLAB].move_sprites=1;
    TRAP_SPRITE_COUNTS[TRAP_STONESLAB].idle_active_sprites=0;
    TRAP_SPRITE_COUNTS[TRAP_STONESLAB].idle_inactive_sprites=1;
    TRAP_SPRITE_COUNTS[TRAP_STONESLAB].width=64;
    TRAP_SPRITE_COUNTS[TRAP_STONESLAB].height=32;

    //Buzz saw
    TRAP_SPRITE_COUNTS[TRAP_BUZZSAW].move_sprites=2;
    TRAP_SPRITE_COUNTS[TRAP_BUZZSAW].idle_active_sprites=0;
    TRAP_SPRITE_COUNTS[TRAP_BUZZSAW].idle_inactive_sprites=0;
    TRAP_SPRITE_COUNTS[TRAP_BUZZSAW].width=24;
    TRAP_SPRITE_COUNTS[TRAP_BUZZSAW].height=24;

    //Alarm toggler
    TRAP_SPRITE_COUNTS[TRAP_ALARM_TOGGLER].move_sprites=0;
    TRAP_SPRITE_COUNTS[TRAP_ALARM_TOGGLER].idle_active_sprites=0;
    TRAP_SPRITE_COUNTS[TRAP_ALARM_TOGGLER].idle_inactive_sprites=0;
    TRAP_SPRITE_COUNTS[TRAP_ALARM_TOGGLER].width=0;
    TRAP_SPRITE_COUNTS[TRAP_ALARM_TOGGLER].height=0;

    //Weather machine
    TRAP_SPRITE_COUNTS[TRAP_WEATHER_MACHINE].move_sprites=0;
    TRAP_SPRITE_COUNTS[TRAP_WEATHER_MACHINE].idle_active_sprites=0;
    TRAP_SPRITE_COUNTS[TRAP_WEATHER_MACHINE].idle_inactive_sprites=0;
    TRAP_SPRITE_COUNTS[TRAP_WEATHER_MACHINE].width=0;
    TRAP_SPRITE_COUNTS[TRAP_WEATHER_MACHINE].height=0;

    //Spikes Up
    TRAP_SPRITE_COUNTS[TRAP_SPIKES_UP].move_sprites=0;
    TRAP_SPRITE_COUNTS[TRAP_SPIKES_UP].idle_active_sprites=1;
    TRAP_SPRITE_COUNTS[TRAP_SPIKES_UP].idle_inactive_sprites=0;
    TRAP_SPRITE_COUNTS[TRAP_SPIKES_UP].width=32;
    TRAP_SPRITE_COUNTS[TRAP_SPIKES_UP].height=32;

    //Spikes Down
    TRAP_SPRITE_COUNTS[TRAP_SPIKES_DOWN].move_sprites=0;
    TRAP_SPRITE_COUNTS[TRAP_SPIKES_DOWN].idle_active_sprites=1;
    TRAP_SPRITE_COUNTS[TRAP_SPIKES_DOWN].idle_inactive_sprites=0;
    TRAP_SPRITE_COUNTS[TRAP_SPIKES_DOWN].width=32;
    TRAP_SPRITE_COUNTS[TRAP_SPIKES_DOWN].height=32;

    //Spikes Right
    TRAP_SPRITE_COUNTS[TRAP_SPIKES_RIGHT].move_sprites=0;
    TRAP_SPRITE_COUNTS[TRAP_SPIKES_RIGHT].idle_active_sprites=1;
    TRAP_SPRITE_COUNTS[TRAP_SPIKES_RIGHT].idle_inactive_sprites=0;
    TRAP_SPRITE_COUNTS[TRAP_SPIKES_RIGHT].width=32;
    TRAP_SPRITE_COUNTS[TRAP_SPIKES_RIGHT].height=32;

    //Spikes Left
    TRAP_SPRITE_COUNTS[TRAP_SPIKES_LEFT].move_sprites=0;
    TRAP_SPRITE_COUNTS[TRAP_SPIKES_LEFT].idle_active_sprites=1;
    TRAP_SPRITE_COUNTS[TRAP_SPIKES_LEFT].idle_inactive_sprites=0;
    TRAP_SPRITE_COUNTS[TRAP_SPIKES_LEFT].width=32;
    TRAP_SPRITE_COUNTS[TRAP_SPIKES_LEFT].height=32;

    //Boss Castle
    TRAP_SPRITE_COUNTS[TRAP_BOSS_CASTLE].move_sprites=1;
    TRAP_SPRITE_COUNTS[TRAP_BOSS_CASTLE].idle_active_sprites=1;
    TRAP_SPRITE_COUNTS[TRAP_BOSS_CASTLE].idle_inactive_sprites=0;
    TRAP_SPRITE_COUNTS[TRAP_BOSS_CASTLE].width=48;
    TRAP_SPRITE_COUNTS[TRAP_BOSS_CASTLE].height=48;

    //Boss Pyramid
    TRAP_SPRITE_COUNTS[TRAP_BOSS_PYRAMID].move_sprites=1;
    TRAP_SPRITE_COUNTS[TRAP_BOSS_PYRAMID].idle_active_sprites=1;
    TRAP_SPRITE_COUNTS[TRAP_BOSS_PYRAMID].idle_inactive_sprites=0;
    TRAP_SPRITE_COUNTS[TRAP_BOSS_PYRAMID].width=48;
    TRAP_SPRITE_COUNTS[TRAP_BOSS_PYRAMID].height=48;

    //Seaweed
    TRAP_SPRITE_COUNTS[TRAP_SEAWEED].move_sprites=0;
    TRAP_SPRITE_COUNTS[TRAP_SEAWEED].idle_active_sprites=8;
    TRAP_SPRITE_COUNTS[TRAP_SEAWEED].idle_inactive_sprites=0;
    TRAP_SPRITE_COUNTS[TRAP_SEAWEED].width=32;
    TRAP_SPRITE_COUNTS[TRAP_SEAWEED].height=64;

    //Open Shop
    TRAP_SPRITE_COUNTS[TRAP_OPEN_SHOP].move_sprites=0;
    TRAP_SPRITE_COUNTS[TRAP_OPEN_SHOP].idle_active_sprites=0;
    TRAP_SPRITE_COUNTS[TRAP_OPEN_SHOP].idle_inactive_sprites=0;
    TRAP_SPRITE_COUNTS[TRAP_OPEN_SHOP].width=0;
    TRAP_SPRITE_COUNTS[TRAP_OPEN_SHOP].height=0;

    //Open Upgrades
    TRAP_SPRITE_COUNTS[TRAP_OPEN_UPGRADES].move_sprites=0;
    TRAP_SPRITE_COUNTS[TRAP_OPEN_UPGRADES].idle_active_sprites=0;
    TRAP_SPRITE_COUNTS[TRAP_OPEN_UPGRADES].idle_inactive_sprites=0;
    TRAP_SPRITE_COUNTS[TRAP_OPEN_UPGRADES].width=0;
    TRAP_SPRITE_COUNTS[TRAP_OPEN_UPGRADES].height=0;
}
