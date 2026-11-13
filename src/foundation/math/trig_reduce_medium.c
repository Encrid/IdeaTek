/*
** Copyright (C) 2026 Enkrid
** Copyright (C) 1993 Sun Microsystems, Inc. (musl, the notice of Sun)
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "trig_inside.h"

static constexpr double TOINT = 6755399441055744.0;
static constexpr double PIO4 = 7.85398163397448278999e-01;
static constexpr double INVPIO2 = 6.36619772367581382433e-01;
static constexpr double PIO2_1 = 1.57079632673412561417e+00;
static constexpr double PIO2_1T = 6.07710050650619224932e-11;
static constexpr double SECOND[2] = {
    6.07710050630396597660e-11, 2.02226624879595063154e-21
};
static constexpr double THIRD[2] = {
    2.02226624871116645580e-21, 8.47842766036889956997e-32
};

static int get_exponent(double value)
{
    uint64_t bits;

    memcpy(&bits, &value, sizeof(bits));
    return ((int)(bits >> 52 & 0x7ff));
}

static double round_count(double x)
{
    double count = x * INVPIO2 + TOINT - TOINT;
    double rest = (x - count * PIO2_1) - count * PIO2_1T;

    if (rest < -PIO4)
        return (count - 1.0);
    if (rest > PIO4)
        return (count + 1.0);
    return (count);
}

static void refine_rest(double count, const double *part, double *r, double *w)
{
    double t = *r;

    *w = count * part[0];
    *r = t - *w;
    *w = count * part[1] - ((t - *r) - *w);
}

int trig_reduce_medium(double x, double *rest)
{
    double count = round_count(x);
    double r = x - count * PIO2_1;
    double w = count * PIO2_1T;
    int from = get_exponent(x);

    if (from - get_exponent(r - w) > 16) {
        refine_rest(count, SECOND, &r, &w);
        if (from - get_exponent(r - w) > 49)
            refine_rest(count, THIRD, &r, &w);
    }
    rest[0] = r - w;
    rest[1] = (r - rest[0]) - w;
    return ((int)count);
}
