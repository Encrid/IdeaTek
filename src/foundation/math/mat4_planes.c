/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "mat4_inside.h"

static vec4f_t get_row(mat4f_t m, int row)
{
    vec4f_t out = { m[row][0], m[row][1], m[row][2], m[row][3] };

    return (out);
}

static vec4f_t make_plane(vec4f_t raw)
{
    float square = raw.x * raw.x + raw.y * raw.y + raw.z * raw.z;
    float length = sqrtf(square);
    vec4f_t all = { 0.0f, 0.0f, 0.0f, 1.0f };

    if (length == 0.0f)
        return (all);
    return (raw / length);
}

void mat4f_get_planes(mat4f_t m, vec4f_t *planes)
{
    vec4f_t last = get_row(m, 3);

    planes[0] = make_plane(last + get_row(m, 0));
    planes[1] = make_plane(last - get_row(m, 0));
    planes[2] = make_plane(last + get_row(m, 1));
    planes[3] = make_plane(last - get_row(m, 1));
    planes[4] = make_plane(get_row(m, 2));
    planes[5] = make_plane(last - get_row(m, 2));
}
