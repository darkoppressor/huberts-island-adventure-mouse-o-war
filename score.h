/* Copyright (c) Cheese and Bacon Games */
/* See the file docs/COPYING.txt for copying permission. */

#ifndef score_h
#define score_h

#include <stdint.h>

extern uint64_t SCORES_ITEMS[];

extern uint64_t SCORES_ACHIEVEMENTS[];

const uint64_t SCORE_BEAT_LEVEL = 4;

const uint64_t SCORE_LEVEL_END_BONUS_J_BALLOON = 16;

const uint64_t SCORE_BEAT_BOSS = 128;

const uint64_t SCORE_SURVIVAL_SECOND = 4;
const uint64_t SCORE_SURVIVAL_CANDY = 16;

void prepare_scores();

#endif
