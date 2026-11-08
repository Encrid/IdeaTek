/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "frame_values.h"

static void show_rounds(char **argv)
{
    frame_clock_t clock = {};
    frame_rule_t rule = { .scale = atof(argv[3]),
        .rate = (uint32_t)atoi(argv[4]), .most_steps = (uint32_t)atoi(argv[5]),
        .paused = atoi(argv[6]) != 0 };
    double real = atof(argv[2]);
    int rounds = atoi(argv[7]);
    frame_values_t out = {};

    for (int i = 0; i < rounds; i++) {
        start_round(&clock, &rule, real, &out);
        printf("round %llu delta %.6f elapsed %.6f real %.6f steps %u"
            " fraction %.4f\n", (unsigned long long)out.frame.index,
            out.frame.delta, out.frame.elapsed, out.frame.real_elapsed,
            out.tick.steps, out.tick.fraction);
    }
}

int main(int argc, char **argv)
{
    if (argc >= 8 && !strcmp(argv[1], "round")) {
        show_rounds(argv);
        return (0);
    }
    if (argc >= 3 && !strcmp(argv[1], "smooth")) {
        printf("smooth %.6f\n", smoothed(atof(argv[2])));
        return (0);
    }
    return (2);
}
