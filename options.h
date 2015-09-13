/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef options_h
#define options_h

#include <fstream>
#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>

class Version_Series{
public:

    int major_1;
    int minor_1;
    int micro_1;

    int major_2;
    int minor_2;
    int micro_2;

    Version_Series(int get_major_1,int get_minor_1,int get_micro_1,int get_major_2,int get_minor_2,int get_micro_2);
};

bool load_current_profile();

bool save_current_profile();

//Compares two versions.
//Returns 0 if they are the same.
//Returns -1 if version 1 is less than version 2.
//Returns 1 if version 1 is greater than version 2.
int options_version_compare(int major_1,int minor_1,int micro_1,int major_2,int minor_2,int micro_2);

//Returns the index into the passed vector that the passed version is a part of.
//Returns -1 if the passed version is not in any series.
int options_which_version_series(std::vector<Version_Series>* version_series,int major,int minor,int micro);

//Returns true if the version of the passed profile is in the same series of compatible releases as the current version.
bool options_version_compatible(std::string name_to_check);

bool options_load();

bool options_save();

bool global_options_load();

bool global_options_save();

bool save_location_load();

bool save_location_save();

#endif
