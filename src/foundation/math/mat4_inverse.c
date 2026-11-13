/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "mat4_inside.h"

static void get_minors(mat4f_t a, int first, float *out)
{
    int next = first + 1;

    out[0] = a[first][0] * a[next][1] - a[next][0] * a[first][1];
    out[1] = a[first][0] * a[next][2] - a[next][0] * a[first][2];
    out[2] = a[first][0] * a[next][3] - a[next][0] * a[first][3];
    out[3] = a[first][1] * a[next][2] - a[next][1] * a[first][2];
    out[4] = a[first][1] * a[next][3] - a[next][1] * a[first][3];
    out[5] = a[first][2] * a[next][3] - a[next][2] * a[first][3];
}

static void fill_low(mat4f_t a, const float *c, float det, mat4f_t *b)
{
    (*b)[0][0] = (a[1][1] * c[5] - a[1][2] * c[4] + a[1][3] * c[3]) / det;
    (*b)[0][1] = (-a[0][1] * c[5] + a[0][2] * c[4] - a[0][3] * c[3]) / det;
    (*b)[1][0] = (-a[1][0] * c[5] + a[1][2] * c[2] - a[1][3] * c[1]) / det;
    (*b)[1][1] = (a[0][0] * c[5] - a[0][2] * c[2] + a[0][3] * c[1]) / det;
    (*b)[2][0] = (a[1][0] * c[4] - a[1][1] * c[2] + a[1][3] * c[0]) / det;
    (*b)[2][1] = (-a[0][0] * c[4] + a[0][1] * c[2] - a[0][3] * c[0]) / det;
    (*b)[3][0] = (-a[1][0] * c[3] + a[1][1] * c[1] - a[1][2] * c[0]) / det;
    (*b)[3][1] = (a[0][0] * c[3] - a[0][1] * c[1] + a[0][2] * c[0]) / det;
}

static void fill_high(mat4f_t a, const float *s, float det, mat4f_t *b)
{
    (*b)[0][2] = (a[3][1] * s[5] - a[3][2] * s[4] + a[3][3] * s[3]) / det;
    (*b)[0][3] = (-a[2][1] * s[5] + a[2][2] * s[4] - a[2][3] * s[3]) / det;
    (*b)[1][2] = (-a[3][0] * s[5] + a[3][2] * s[2] - a[3][3] * s[1]) / det;
    (*b)[1][3] = (a[2][0] * s[5] - a[2][2] * s[2] + a[2][3] * s[1]) / det;
    (*b)[2][2] = (a[3][0] * s[4] - a[3][1] * s[2] + a[3][3] * s[0]) / det;
    (*b)[2][3] = (-a[2][0] * s[4] + a[2][1] * s[2] - a[2][3] * s[0]) / det;
    (*b)[3][2] = (-a[3][0] * s[3] + a[3][1] * s[1] - a[3][2] * s[0]) / det;
    (*b)[3][3] = (a[2][0] * s[3] - a[2][1] * s[1] + a[2][2] * s[0]) / det;
}

bool mat4f_get_inverse(mat4f_t m, mat4f_t *inverse)
{
    float s[6];
    float c[6];
    float det;

    get_minors(m, 0, s);
    get_minors(m, 2, c);
    det = s[0] * c[5] - s[1] * c[4] + s[2] * c[3] + s[3] * c[2] - s[4] * c[1]
        + s[5] * c[0];
    if (det == 0.0f)
        return (false);
    fill_low(m, c, det, inverse);
    fill_high(m, s, det, inverse);
    return (true);
}
