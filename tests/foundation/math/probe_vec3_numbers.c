/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "probe.h"

static void run_vec3d_load(char **v)
{
    double numbers[3] = { read_f64(v[0]), read_f64(v[1]), read_f64(v[2]) };

    show_vec3d(vec3d_load(numbers));
}

static void run_vec3f_load(char **v)
{
    float numbers[3] = { read_f32(v[0]), read_f32(v[1]), read_f32(v[2]) };

    show_vec3f(vec3f_load(numbers));
}

static void run_vec3d_store(char **v)
{
    double numbers[4] = { -1.0, -1.0, -1.0, -1.0 };

    vec3d_store(read_vec3d(v), numbers);
    for (int i = 0; i < 4; i++)
        show_f64(numbers[i]);
}

static void run_vec3f_store(char **v)
{
    float numbers[4] = { -1.0f, -1.0f, -1.0f, -1.0f };

    vec3f_store(read_vec3f(v), numbers);
    for (int i = 0; i < 4; i++)
        show_f32(numbers[i]);
}

static void run_vec3d_get_narrow(char **v)
{
    show_vec3f(vec3d_get_narrow(read_vec3d(v)));
}

static void run_vec3f_get_wide(char **v)
{
    show_vec3d(vec3f_get_wide(read_vec3f(v)));
}

static void run_vec3d_split(char **v)
{
    vec3f_t high;
    vec3f_t low;

    vec3d_split(read_vec3d(v), &high, &low);
    show_vec3f(high);
    show_vec3f(low);
}

static const probe_entry_t ENTRIES[] = {
    { "vec3d_load", 3, run_vec3d_load },
    { "vec3f_load", 3, run_vec3f_load },
    { "vec3d_store", 3, run_vec3d_store },
    { "vec3f_store", 3, run_vec3f_store },
    { "vec3d_get_narrow", 3, run_vec3d_get_narrow },
    { "vec3f_get_wide", 3, run_vec3f_get_wide },
    { "vec3d_split", 3, run_vec3d_split },
};

const probe_group_t VEC3_NUMBER_PROBES = {
    ENTRIES, sizeof(ENTRIES) / sizeof(ENTRIES[0])
};
