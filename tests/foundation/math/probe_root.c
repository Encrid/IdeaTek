/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "math_scalar.h"
#include "probe.h"

#pragma STDC FP_CONTRACT OFF

static double get_rising(void *context, double value)
{
    return (value * value - *(double *)context);
}

static double get_falling(void *context, double value)
{
    return (*(double *)context - value * value);
}

static double get_line(void *context, double value)
{
    return (*(double *)context - value);
}

static double get_broken(void *context, double value)
{
    if (value == 1.0 || value == 2.0)
        return (get_rising(context, value));
    return ((double)NAN);
}

static f64_function_t pick_function(const char *name)
{
    if (!strcmp(name, "falling"))
        return (get_falling);
    if (!strcmp(name, "line"))
        return (get_line);
    if (!strcmp(name, "broken"))
        return (get_broken);
    return (get_rising);
}

static void run_f64_find_root(char **v)
{
    double constant = read_f64(v[1]);
    double root = -1.0;
    f64_root_t ask = { pick_function(v[0]), &constant, read_f64(v[2]),
        read_f64(v[3]), (uint32_t)strtoul(v[4], nullptr, 10) };

    printf(" %d", (int)f64_find_root(&ask, &root));
    show_f64(root);
}

static const probe_entry_t ENTRIES[] = {
    { "f64_find_root", 5, run_f64_find_root },
};

const probe_group_t ROOT_PROBES = {
    ENTRIES, sizeof(ENTRIES) / sizeof(ENTRIES[0])
};
