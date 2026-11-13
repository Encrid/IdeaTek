/*
** Copyright (C) 2026 Enkrid
** Copyright (C) 1993 Sun Microsystems, Inc. (musl, the notice of Sun)
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <math.h>
#include <string.h>

#include "trig_inside.h"

static constexpr double PIO2_1 = 1.57079632673412561417e+00;
static constexpr double PIO2_1T = 6.07710050650619224932e-11;

uint32_t trig_get_high(double value)
{
    uint64_t bits;

    memcpy(&bits, &value, sizeof(bits));
    return ((uint32_t)(bits >> 32));
}

static double get_near_count(uint32_t high)
{
    if (high <= 0x400f6a7a && (high & 0xfffff) == 0x921fb)
        return (0.0);
    if (high == 0x4012d97c || high == 0x401921fb)
        return (0.0);
    if (high <= 0x4002d97c)
        return (1.0);
    if (high <= 0x400f6a7a)
        return (2.0);
    if (high <= 0x4015fdbc)
        return (3.0);
    if (high <= 0x401c463b)
        return (4.0);
    return (0.0);
}

static int reduce_by_count(double x, double count, double *rest)
{
    double z = x - count * PIO2_1;
    double tail = count * PIO2_1T;

    rest[0] = z - tail;
    rest[1] = (z - rest[0]) - tail;
    return ((int)count);
}

int trig_reduce(double x, double *rest)
{
    uint32_t high = trig_get_high(x) & 0x7fffffff;
    double count = get_near_count(high);

    if (count != 0.0 && signbit(x))
        return (reduce_by_count(x, -count, rest));
    if (count != 0.0)
        return (reduce_by_count(x, count, rest));
    if (high < 0x413921fb)
        return (trig_reduce_medium(x, rest));
    return (trig_reduce_large(x, rest));
}
