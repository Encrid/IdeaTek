/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>

#include "math_quat.h"
#include "probe.h"

static quatf_t read_f(char **v)
{
    quatf_t out = { read_f32(v[0]), read_f32(v[1]), read_f32(v[2]),
        read_f32(v[3]) };

    return (out);
}

static void show_f(quatf_t q)
{
    show_f32(q.x);
    show_f32(q.y);
    show_f32(q.z);
    show_f32(q.w);
}

static void run_get_dot(char **v)
{
    show_f32(quatf_get_dot(read_f(v), read_f(v + 4)));
}

static void run_get_length(char **v)
{
    show_f32(quatf_get_length(read_f(v)));
}

static void run_get_product(char **v)
{
    show_f(quatf_get_product(read_f(v), read_f(v + 4)));
}

static void run_get_conjugate(char **v)
{
    show_f(quatf_get_conjugate(read_f(v)));
}

static void run_get_unit(char **v)
{
    show_f(quatf_get_unit(read_f(v)));
}

static void run_turn(char **v)
{
    show_vec3f(quatf_turn(read_f(v), read_vec3f(v + 4)));
}

static void run_turn_back(char **v)
{
    show_vec3f(quatf_turn_back(read_f(v), read_vec3f(v + 4)));
}

static void run_make_turn(char **v)
{
    show_f(quatf_make_turn(read_vec3f(v), read_f32(v[3])));
}

static void run_get_angle(char **v)
{
    show_f32(quatf_get_angle(read_f(v)));
}

static void run_get_axis(char **v)
{
    show_vec3f(quatf_get_axis(read_f(v)));
}

static void run_get_blend(char **v)
{
    show_f(quatf_get_blend(read_f(v), read_f(v + 4), read_f32(v[8])));
}

static void run_is_finite(char **v)
{
    printf(" %d", (int)quatf_is_finite(read_f(v)));
}

static void run_is_unit(char **v)
{
    printf(" %d", (int)quatf_is_unit(read_f(v), read_f32(v[4])));
}

static void run_get_wide(char **v)
{
    quatd_t q = quatf_get_wide(read_f(v));

    show_f64(q.x);
    show_f64(q.y);
    show_f64(q.z);
    show_f64(q.w);
}

static const probe_entry_t ENTRIES[] = {
    { "quatf_get_dot", 8, run_get_dot },
    { "quatf_get_length", 4, run_get_length },
    { "quatf_get_product", 8, run_get_product },
    { "quatf_get_conjugate", 4, run_get_conjugate },
    { "quatf_get_unit", 4, run_get_unit },
    { "quatf_turn", 7, run_turn },
    { "quatf_turn_back", 7, run_turn_back },
    { "quatf_make_turn", 4, run_make_turn },
    { "quatf_get_angle", 4, run_get_angle },
    { "quatf_get_axis", 4, run_get_axis },
    { "quatf_get_blend", 9, run_get_blend },
    { "quatf_is_finite", 4, run_is_finite },
    { "quatf_is_unit", 5, run_is_unit },
    { "quatf_get_wide", 4, run_get_wide },
};

const probe_group_t QUATF_PROBES = {
    ENTRIES, sizeof(ENTRIES) / sizeof(ENTRIES[0])
};
