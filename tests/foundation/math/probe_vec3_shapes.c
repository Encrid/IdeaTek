/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>

#include "probe.h"

static void run_vec3d_get_unit(char **v)
{
    show_vec3d(vec3d_get_unit(read_vec3d(v)));
}

static void run_vec3f_get_unit(char **v)
{
    show_vec3f(vec3f_get_unit(read_vec3f(v)));
}

static void run_vec3d_get_blend(char **v)
{
    show_vec3d(vec3d_get_blend(read_vec3d(v), read_vec3d(v + 3),
        read_f64(v[6])));
}

static void run_vec3f_get_blend(char **v)
{
    show_vec3f(vec3f_get_blend(read_vec3f(v), read_vec3f(v + 3),
        read_f32(v[6])));
}

static void run_vec3d_get_least(char **v)
{
    show_vec3d(vec3d_get_least(read_vec3d(v), read_vec3d(v + 3)));
}

static void run_vec3f_get_least(char **v)
{
    show_vec3f(vec3f_get_least(read_vec3f(v), read_vec3f(v + 3)));
}

static void run_vec3d_get_most(char **v)
{
    show_vec3d(vec3d_get_most(read_vec3d(v), read_vec3d(v + 3)));
}

static void run_vec3f_get_most(char **v)
{
    show_vec3f(vec3f_get_most(read_vec3f(v), read_vec3f(v + 3)));
}

static void run_vec3d_is_finite(char **v)
{
    printf(" %d", (int)vec3d_is_finite(read_vec3d(v)));
}

static void run_vec3f_is_finite(char **v)
{
    printf(" %d", (int)vec3f_is_finite(read_vec3f(v)));
}

static const probe_entry_t ENTRIES[] = {
    { "vec3d_get_unit", 3, run_vec3d_get_unit },
    { "vec3f_get_unit", 3, run_vec3f_get_unit },
    { "vec3d_get_blend", 7, run_vec3d_get_blend },
    { "vec3f_get_blend", 7, run_vec3f_get_blend },
    { "vec3d_get_least", 6, run_vec3d_get_least },
    { "vec3f_get_least", 6, run_vec3f_get_least },
    { "vec3d_get_most", 6, run_vec3d_get_most },
    { "vec3f_get_most", 6, run_vec3f_get_most },
    { "vec3d_is_finite", 3, run_vec3d_is_finite },
    { "vec3f_is_finite", 3, run_vec3f_is_finite },
};

const probe_group_t VEC3_SHAPE_PROBES = {
    ENTRIES, sizeof(ENTRIES) / sizeof(ENTRIES[0])
};
