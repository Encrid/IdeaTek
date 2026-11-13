/*
** Copyright (C) 2026 Enkrid
** Copyright (C) 1993 Sun Microsystems, Inc. (musl, the notice of Sun)
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "math_scalar.h"
#include "trig_inside.h"

static double pick_sine(int count, const double *rest)
{
    switch (count & 3) {
        case 0:
            return (trig_get_sine(rest[0], rest[1], true));
        case 1:
            return (trig_get_cosine(rest[0], rest[1]));
        case 2:
            return (-trig_get_sine(rest[0], rest[1], true));
        default:
            return (-trig_get_cosine(rest[0], rest[1]));
    }
}

double f64_get_sin(double angle)
{
    uint32_t high = trig_get_high(angle) & 0x7fffffff;
    double rest[2];

    if (high < 0x3e500000)
        return (angle);
    if (high <= 0x3fe921fb)
        return (trig_get_sine(angle, 0.0, false));
    if (high >= 0x7ff00000)
        return ((double)NAN);
    return (pick_sine(trig_reduce(angle, rest), rest));
}

double f64_get_cos(double angle)
{
    uint32_t high = trig_get_high(angle) & 0x7fffffff;
    double rest[2];

    if (high < 0x3e46a09e)
        return (1.0);
    if (high <= 0x3fe921fb)
        return (trig_get_cosine(angle, 0.0));
    if (high >= 0x7ff00000)
        return ((double)NAN);
    return (pick_sine(trig_reduce(angle, rest) + 1, rest));
}
