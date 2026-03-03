/* Copyright (c) Cheese and Bacon Games */
/* See the file docs/COPYING.txt for copying permission. */

#include "distance.h"
#include <cmath>

using namespace std;

double distance_between_points (double x1, double y1, double x2, double y2) {
    return sqrt(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1)));
}
