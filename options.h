/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef options_h
#define options_h

#include <fstream>
#include <string>

#include <boost/algorithm/string.hpp>

bool load_current_profile();

bool save_current_profile();

//Compares two version strings.
//Returns 0 if they are the same.
//Returns -1 if current_version is less than version.
//Returns 1 if current_version is greater than version.
int compare_versions(std::string current_version,std::string version);

//Returns true if the version of the passed profile is the same as the current version.
//Returns false if the versions do not match.
bool options_version_same(std::string name_to_check);

bool options_load();

bool options_save();

bool global_options_load();

bool global_options_save();

bool save_location_load();

bool save_location_save();

#endif
