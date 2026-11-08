/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef FRAME_VALUES_H
    #define FRAME_VALUES_H

    #include <stdint.h>

    #include "ideatek_frame.h"

typedef struct {
    double pending;
    double elapsed;
    double real_elapsed;
    uint64_t index;
} frame_clock_t;

typedef struct {
    double scale;
    uint32_t rate;
    uint32_t most_steps;
    bool paused;
} frame_rule_t;

typedef struct {
    ideatek_frame_t frame;
    ideatek_tick_t tick;
} frame_values_t;

void start_round(frame_clock_t *clock, const frame_rule_t *rule, double real,
    frame_values_t *out);
double smoothed(double real);

#endif
