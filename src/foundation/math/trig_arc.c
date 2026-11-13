/*
** Copyright (C) 2026 Enkrid
** Copyright (C) 1993 Sun Microsystems, Inc. (musl, the notice of Sun)
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <math.h>

#include "trig_inside.h"

static constexpr double ATANHI[4] = {
    4.63647609000806093515e-01, 7.85398163397448278999e-01,
    9.82793723247329054082e-01, 1.57079632679489655800e+00
};
static constexpr double ATANLO[4] = {
    2.26987774529616870924e-17, 3.06161699786838301793e-17,
    1.39033110312309984516e-17, 6.12323399573676603587e-17
};
static constexpr double AT[11] = {
    3.33333333333329318027e-01, -1.99999999998764832476e-01,
    1.42857142725034663711e-01, -1.11111104054623557880e-01,
    9.09088713343650656196e-02, -7.69187620504482999495e-02,
    6.66107313738753120669e-02, -5.83357013379057348645e-02,
    4.97687799461593236017e-02, -3.65315727442169155270e-02,
    1.62858201153657823623e-02
};

static double reduce_ratio(double x, uint32_t high, int *part)
{
    if (high < 0x3fe60000) {
        *part = 0;
        return ((2.0 * x - 1.0) / (2.0 + x));
    }
    if (high < 0x3ff30000) {
        *part = 1;
        return ((x - 1.0) / (x + 1.0));
    }
    if (high < 0x40038000) {
        *part = 2;
        return ((x - 1.5) / (1.0 + 1.5 * x));
    }
    *part = 3;
    return (-1.0 / x);
}

static double get_series(double x)
{
    double z = x * x;
    double w = z * z;
    double odd = AT[10];
    double even = AT[9];

    for (int i = 8; i >= 0; i -= 2)
        odd = AT[i] + w * odd;
    for (int i = 7; i >= 1; i -= 2)
        even = AT[i] + w * even;
    return (x * (z * odd + w * even));
}

double trig_get_arc_tangent(double x)
{
    uint32_t high = trig_get_high(x) & 0x7fffffff;
    double reduced = x;
    double series;
    double z;
    int part = -1;

    if (high >= 0x44100000)
        return (signbit(x) ? -ATANHI[3] : ATANHI[3]);
    if (high < 0x3e400000)
        return (x);
    if (high >= 0x3fdc0000)
        reduced = reduce_ratio(fabs(x), high, &part);
    series = get_series(reduced);
    if (part < 0)
        return (reduced - series);
    z = ATANHI[part] - (series - ATANLO[part] - reduced);
    return (signbit(x) ? -z : z);
}
