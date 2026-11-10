/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <math.h>
#include <string.h>

#include "space_pure.h"

static float get_length(const float *v)
{
    return (sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]));
}

static void remove_part(float *v, const float *axis)
{
    float dot = v[0] * axis[0] + v[1] * axis[1] + v[2] * axis[2];

    for (int i = 0; i < 3; i++)
        v[i] -= dot * axis[i];
}

static void cross(const float *a, const float *b, float *out)
{
    out[0] = a[1] * b[2] - a[2] * b[1];
    out[1] = a[2] * b[0] - a[0] * b[2];
    out[2] = a[0] * b[1] - a[1] * b[0];
}

static void make_unit(float *m, int i, float length)
{
    float zero = length == 0.0f;
    float keep = 1.0f - zero;
    float fill[3];
    float fill_length;

    cross(m + 3 * ((i + 1) % 3), m + 3 * ((i + 2) % 3), fill);
    fill_length = get_length(fill);
    fill_length += fill_length == 0.0f;
    for (int k = 0; k < 3; k++)
        m[3 * i + k] = keep * m[3 * i + k] / (length + zero)
            + zero * fill[k] / fill_length;
}

void split_basis(const float basis[9], ideatek_quat_t *rotation,
    ideatek_vec3_t *scale)
{
    float m[9];
    float s[3];
    float flip;

    memcpy(m, basis, sizeof(m));
    for (int i = 0; i < 3; i++)
        s[i] = get_length(m + 3 * i);
    make_unit(m, 0, get_length(m));
    remove_part(m + 3, m);
    make_unit(m, 1, get_length(m + 3));
    remove_part(m + 6, m);
    remove_part(m + 6, m + 3);
    make_unit(m, 2, get_length(m + 6));
    flip = (basis[0] * (basis[4] * basis[8] - basis[7] * basis[5])
        - basis[3] * (basis[1] * basis[8] - basis[7] * basis[2])
        + basis[6] * (basis[1] * basis[5] - basis[4] * basis[2])) < 0.0f;
    for (int k = 6; k < 9; k++)
        m[k] *= 1.0f - 2.0f * flip;
    *scale = (ideatek_vec3_t){ s[0], s[1], s[2] * (1.0f - 2.0f * flip) };
    *rotation = get_rotation(m);
}
