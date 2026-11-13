/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "probe.h"

static void run_vec3d_get_dot(char **v)
{
    show_f64(vec3d_get_dot(read_vec3d(v), read_vec3d(v + 3)));
}

static void run_vec3f_get_dot(char **v)
{
    show_f32(vec3f_get_dot(read_vec3f(v), read_vec3f(v + 3)));
}

static void run_vec3d_get_cross(char **v)
{
    show_vec3d(vec3d_get_cross(read_vec3d(v), read_vec3d(v + 3)));
}

static void run_vec3f_get_cross(char **v)
{
    show_vec3f(vec3f_get_cross(read_vec3f(v), read_vec3f(v + 3)));
}

static void run_vec3d_get_square(char **v)
{
    show_f64(vec3d_get_square(read_vec3d(v)));
}

static void run_vec3f_get_square(char **v)
{
    show_f32(vec3f_get_square(read_vec3f(v)));
}

static void run_vec3d_get_length(char **v)
{
    show_f64(vec3d_get_length(read_vec3d(v)));
}

static void run_vec3f_get_length(char **v)
{
    show_f32(vec3f_get_length(read_vec3f(v)));
}

static void run_vec3d_get_distance(char **v)
{
    show_f64(vec3d_get_distance(read_vec3d(v), read_vec3d(v + 3)));
}

static void run_vec3f_get_distance(char **v)
{
    show_f32(vec3f_get_distance(read_vec3f(v), read_vec3f(v + 3)));
}

static const probe_entry_t ENTRIES[] = {
    { "vec3d_get_dot", 6, run_vec3d_get_dot },
    { "vec3f_get_dot", 6, run_vec3f_get_dot },
    { "vec3d_get_cross", 6, run_vec3d_get_cross },
    { "vec3f_get_cross", 6, run_vec3f_get_cross },
    { "vec3d_get_square", 3, run_vec3d_get_square },
    { "vec3f_get_square", 3, run_vec3f_get_square },
    { "vec3d_get_length", 3, run_vec3d_get_length },
    { "vec3f_get_length", 3, run_vec3f_get_length },
    { "vec3d_get_distance", 6, run_vec3d_get_distance },
    { "vec3f_get_distance", 6, run_vec3f_get_distance },
};

const probe_group_t VEC3_PROBES = {
    ENTRIES, sizeof(ENTRIES) / sizeof(ENTRIES[0])
};
