/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "probe.h"

vec3d_t read_vec3d(char **values)
{
    vec3d_t out;

    out.x = read_f64(values[0]);
    out.y = read_f64(values[1]);
    out.z = read_f64(values[2]);
    return (out);
}

vec3f_t read_vec3f(char **values)
{
    vec3f_t out;

    out.x = read_f32(values[0]);
    out.y = read_f32(values[1]);
    out.z = read_f32(values[2]);
    return (out);
}

void show_vec3d(vec3d_t value)
{
    show_f64(value.x);
    show_f64(value.y);
    show_f64(value.z);
}

void show_vec3f(vec3f_t value)
{
    show_f32(value.x);
    show_f32(value.y);
    show_f32(value.z);
}
