/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "probe.h"

static uint64_t next_random(uint64_t *state)
{
    *state = *state * 6364136223846793005ULL + 1442695040888963407ULL;
    return (*state >> 11);
}

static double next_number(uint64_t *state, int low, int high)
{
    double fraction = (double)next_random(state) / 9007199254740992.0;
    int exponent = low + (int)(next_random(state) % (uint64_t)(high - low));

    return (ldexp(1.0 + fraction, exponent));
}

static int64_t get_distance(double a, double b)
{
    int64_t first;
    int64_t second;

    memcpy(&first, &a, sizeof(first));
    memcpy(&second, &b, sizeof(second));
    if ((first < 0) != (second < 0))
        return (a == b ? 0 : INT64_MAX);
    return (first > second ? first - second : second - first);
}

static int64_t measure(const char *name, uint64_t *state)
{
    double x = next_number(state, -20, 40);
    double y = next_number(state, -20, 20);

    if (next_random(state) & 1)
        x = -x;
    if (!strcmp(name, "sin"))
        return (get_distance(f64_get_sin(x), sin(x)));
    if (!strcmp(name, "cos"))
        return (get_distance(f64_get_cos(x), cos(x)));
    if (!strcmp(name, "atan2"))
        return (get_distance(f64_get_atan2(y, x), atan2(y, x)));
    x = next_number(state, -200, 200);
    return (get_distance(f64_get_log(x), log(x)));
}

static void run_distance(char **v)
{
    uint64_t state = 20261113;
    int64_t bound = strtoll(v[1], nullptr, 10);
    int64_t most = 0;
    int64_t one;

    for (int i = 0; i < 20000; i++) {
        one = measure(v[0], &state);
        most = one > most ? one : most;
    }
    if (most <= bound)
        printf(" %s within %lld", v[0], (long long)bound);
    else
        printf(" %s outside %lld", v[0], (long long)bound);
}

static const probe_entry_t ENTRIES[] = {
    { "f64_distance", 2, run_distance },
};

const probe_group_t DISTANCE_PROBES = {
    ENTRIES, sizeof(ENTRIES) / sizeof(ENTRIES[0])
};
