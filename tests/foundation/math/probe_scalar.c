/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>

#include "math_scalar.h"
#include "probe.h"

static void run_f64_clamp(char **v)
{
    show_f64(f64_clamp(read_f64(v[0]), read_f64(v[1]), read_f64(v[2])));
}

static void run_f32_clamp(char **v)
{
    show_f32(f32_clamp(read_f32(v[0]), read_f32(v[1]), read_f32(v[2])));
}

static void run_f64_blend(char **v)
{
    show_f64(f64_blend(read_f64(v[0]), read_f64(v[1]), read_f64(v[2])));
}

static void run_f32_blend(char **v)
{
    show_f32(f32_blend(read_f32(v[0]), read_f32(v[1]), read_f32(v[2])));
}

static void run_f64_is_finite(char **v)
{
    printf(" %d", (int)f64_is_finite(read_f64(v[0])));
}

static void run_f32_is_finite(char **v)
{
    printf(" %d", (int)f32_is_finite(read_f32(v[0])));
}

static void run_f64_split(char **v)
{
    float high;
    float low;

    f64_split(read_f64(v[0]), &high, &low);
    show_f32(high);
    show_f32(low);
}

static const probe_entry_t ENTRIES[] = {
    { "f64_clamp", 3, run_f64_clamp },
    { "f32_clamp", 3, run_f32_clamp },
    { "f64_blend", 3, run_f64_blend },
    { "f32_blend", 3, run_f32_blend },
    { "f64_is_finite", 1, run_f64_is_finite },
    { "f32_is_finite", 1, run_f32_is_finite },
    { "f64_split", 1, run_f64_split },
};

const probe_group_t SCALAR_PROBES = {
    ENTRIES, sizeof(ENTRIES) / sizeof(ENTRIES[0])
};
