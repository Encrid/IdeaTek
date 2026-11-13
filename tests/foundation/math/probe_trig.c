/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "probe.h"

static void run_f64_get_sin(char **v)
{
    show_bits(f64_get_sin(read_f64(v[0])));
}

static void run_f64_get_cos(char **v)
{
    show_bits(f64_get_cos(read_f64(v[0])));
}

static void run_f32_get_sin(char **v)
{
    show_f32(f32_get_sin(read_f32(v[0])));
}

static void run_f32_get_cos(char **v)
{
    show_f32(f32_get_cos(read_f32(v[0])));
}

static void run_f64_get_atan2(char **v)
{
    show_bits(f64_get_atan2(read_f64(v[0]), read_f64(v[1])));
}

static void run_f64_get_log(char **v)
{
    show_bits(f64_get_log(read_f64(v[0])));
}

static void run_f32_get_atan2(char **v)
{
    show_f32(f32_get_atan2(read_f32(v[0]), read_f32(v[1])));
}

static const probe_entry_t ENTRIES[] = {
    { "f64_get_sin", 1, run_f64_get_sin },
    { "f64_get_cos", 1, run_f64_get_cos },
    { "f32_get_sin", 1, run_f32_get_sin },
    { "f32_get_cos", 1, run_f32_get_cos },
    { "f64_get_atan2", 2, run_f64_get_atan2 },
    { "f32_get_atan2", 2, run_f32_get_atan2 },
    { "f64_get_log", 1, run_f64_get_log },
};

const probe_group_t TRIG_PROBES = {
    ENTRIES, sizeof(ENTRIES) / sizeof(ENTRIES[0])
};
