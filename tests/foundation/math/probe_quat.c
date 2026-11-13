/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>

#include "math_quat.h"
#include "probe.h"

static quatd_t read_d(char **v)
{
    quatd_t out = { read_f64(v[0]), read_f64(v[1]), read_f64(v[2]),
        read_f64(v[3]) };

    return (out);
}

static void show_d(quatd_t q)
{
    show_f64(q.x);
    show_f64(q.y);
    show_f64(q.z);
    show_f64(q.w);
}

static void run_get_dot(char **v)
{
    show_f64(quatd_get_dot(read_d(v), read_d(v + 4)));
}

static void run_get_length(char **v)
{
    show_f64(quatd_get_length(read_d(v)));
}

static void run_get_product(char **v)
{
    show_d(quatd_get_product(read_d(v), read_d(v + 4)));
}

static void run_get_conjugate(char **v)
{
    show_d(quatd_get_conjugate(read_d(v)));
}

static void run_get_unit(char **v)
{
    show_d(quatd_get_unit(read_d(v)));
}

static void run_turn(char **v)
{
    show_vec3d(quatd_turn(read_d(v), read_vec3d(v + 4)));
}

static void run_turn_back(char **v)
{
    show_vec3d(quatd_turn_back(read_d(v), read_vec3d(v + 4)));
}

static void run_turn_product(char **v)
{
    quatd_t both = quatd_get_product(read_d(v), read_d(v + 4));

    show_vec3d(quatd_turn(both, read_vec3d(v + 8)));
}

static void run_make_turn(char **v)
{
    show_d(quatd_make_turn(read_vec3d(v), read_f64(v[3])));
}

static void run_get_angle(char **v)
{
    show_f64(quatd_get_angle(read_d(v)));
}

static void run_get_axis(char **v)
{
    show_vec3d(quatd_get_axis(read_d(v)));
}

static void run_get_blend(char **v)
{
    show_d(quatd_get_blend(read_d(v), read_d(v + 4), read_f64(v[8])));
}

static void run_is_finite(char **v)
{
    printf(" %d", (int)quatd_is_finite(read_d(v)));
}

static void run_is_unit(char **v)
{
    printf(" %d", (int)quatd_is_unit(read_d(v), read_f64(v[4])));
}

static void run_step(char **v)
{
    show_d(quatd_step(read_d(v), read_vec3d(v + 4), read_f64(v[7])));
}

static void run_get_narrow(char **v)
{
    quatf_t q = quatd_get_narrow(read_d(v));

    show_f32(q.x);
    show_f32(q.y);
    show_f32(q.z);
    show_f32(q.w);
}

static const probe_entry_t ENTRIES[] = {
    { "quatd_get_dot", 8, run_get_dot },
    { "quatd_get_length", 4, run_get_length },
    { "quatd_get_product", 8, run_get_product },
    { "quatd_get_conjugate", 4, run_get_conjugate },
    { "quatd_get_unit", 4, run_get_unit },
    { "quatd_turn", 7, run_turn },
    { "quatd_turn_back", 7, run_turn_back },
    { "quatd_turn_product", 11, run_turn_product },
    { "quatd_make_turn", 4, run_make_turn },
    { "quatd_get_angle", 4, run_get_angle },
    { "quatd_get_axis", 4, run_get_axis },
    { "quatd_get_blend", 9, run_get_blend },
    { "quatd_is_finite", 4, run_is_finite },
    { "quatd_is_unit", 5, run_is_unit },
    { "quatd_step", 8, run_step },
    { "quatd_get_narrow", 4, run_get_narrow },
};

const probe_group_t QUATD_PROBES = {
    ENTRIES, sizeof(ENTRIES) / sizeof(ENTRIES[0])
};
