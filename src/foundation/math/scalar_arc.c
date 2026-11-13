/*
** Copyright (C) 2026 Enkrid
** Copyright (C) 1993 Sun Microsystems, Inc. (musl, the notice of Sun)
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "math_scalar.h"
#include "trig_inside.h"

static constexpr double PI = 3.1415926535897931160e+00;
static constexpr double PI_LOW = 1.2246467991473531772e-16;
static constexpr uint32_t FAR = 64 << 20;

static double get_edge_size(double y, double x)
{
    if (isinf(x) && isinf(y))
        return (signbit(x) ? 3.0 * PI / 4.0 : PI / 4.0);
    if (y == 0.0 || isinf(x))
        return (signbit(x) ? PI : 0.0);
    return (PI / 2.0);
}

static double get_edge(double y, double x)
{
    double size = get_edge_size(y, x);

    return (signbit(y) ? -size : size);
}

static double get_inner(double y, double x)
{
    uint32_t high_x = trig_get_high(x) & 0x7fffffff;
    uint32_t high_y = trig_get_high(y) & 0x7fffffff;
    double z = 0.0;

    if (high_x + FAR < high_y)
        return (signbit(y) ? -PI / 2.0 : PI / 2.0);
    if (!signbit(x) || high_y + FAR >= high_x)
        z = trig_get_arc_tangent(fabs(y / x));
    if (!signbit(x))
        return (signbit(y) ? -z : z);
    return (signbit(y) ? (z - PI_LOW) - PI : PI - (z - PI_LOW));
}

double f64_get_atan2(double y, double x)
{
    if (isnan(x) || isnan(y))
        return ((double)NAN);
    if (x == 0.0 && y == 0.0)
        return (0.0);
    if (x == 1.0)
        return (trig_get_arc_tangent(y));
    if (y == 0.0 || x == 0.0 || isinf(x) || isinf(y))
        return (get_edge(y, x));
    return (get_inner(y, x));
}
