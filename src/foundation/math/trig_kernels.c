/*
** Copyright (C) 2026 Enkrid
** Copyright (C) 1993 Sun Microsystems, Inc. (musl, the notice of Sun)
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "trig_inside.h"

static constexpr double S1 = -1.66666666666666324348e-01;
static constexpr double S2 = 8.33333333332248946124e-03;
static constexpr double S3 = -1.98412698298579493134e-04;
static constexpr double S4 = 2.75573137070700676789e-06;
static constexpr double S5 = -2.50507602534068634195e-08;
static constexpr double S6 = 1.58969099521155010221e-10;
static constexpr double C1 = 4.16666666666666019037e-02;
static constexpr double C2 = -1.38888888888741095749e-03;
static constexpr double C3 = 2.48015872894767294178e-05;
static constexpr double C4 = -2.75573143513906633035e-07;
static constexpr double C5 = 2.08757232129817482790e-09;
static constexpr double C6 = -1.13596475577881948265e-11;

double trig_get_sine(double x, double tail, bool has_tail)
{
    double z = x * x;
    double w = z * z;
    double r = S2 + z * (S3 + z * S4) + z * w * (S5 + z * S6);
    double v = z * x;

    if (!has_tail)
        return (x + v * (S1 + z * r));
    return (x - ((z * (0.5 * tail - v * r) - tail) - v * S1));
}

double trig_get_cosine(double x, double tail)
{
    double z = x * x;
    double w = z * z;
    double low = z * (C1 + z * (C2 + z * C3));
    double r = low + w * w * (C4 + z * (C5 + z * C6));
    double hz = 0.5 * z;

    w = 1.0 - hz;
    return (w + (((1.0 - w) - hz) + (z * r - x * tail)));
}
