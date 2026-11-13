/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "math_vec4.h"
#include "probe.h"

static vec4f_t read_vec4f(char **values)
{
    float numbers[4];

    for (int i = 0; i < 4; i++)
        numbers[i] = read_f32(values[i]);
    return ((vec4f_t){ numbers[0], numbers[1], numbers[2], numbers[3] });
}

static void run_vec4f_get_dot(char **v)
{
    show_f32(vec4f_get_dot(read_vec4f(v), read_vec4f(v + 4)));
}

static void run_vec4f_load(char **v)
{
    float numbers[4];
    vec4f_t out;

    for (int i = 0; i < 4; i++)
        numbers[i] = read_f32(v[i]);
    out = vec4f_load(numbers);
    show_f32(out.x);
    show_f32(out.y);
    show_f32(out.z);
    show_f32(out.w);
}

static void run_vec4f_store(char **v)
{
    float numbers[5] = { -1.0f, -1.0f, -1.0f, -1.0f, -1.0f };

    vec4f_store(read_vec4f(v), numbers);
    for (int i = 0; i < 5; i++)
        show_f32(numbers[i]);
}

static const probe_entry_t ENTRIES[] = {
    { "vec4f_get_dot", 8, run_vec4f_get_dot },
    { "vec4f_load", 4, run_vec4f_load },
    { "vec4f_store", 4, run_vec4f_store },
};

const probe_group_t VEC4_PROBES = {
    ENTRIES, sizeof(ENTRIES) / sizeof(ENTRIES[0])
};
