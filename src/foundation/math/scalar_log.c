/*
** Copyright (C) 2026 Enkrid
** Copyright (C) 2018 Arm Limited (musl, MIT)
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <float.h>
#include <string.h>

#include "log_inside.h"
#include "math_scalar.h"

static constexpr uint64_t NEAR_LOW = 0x3fee000000000000;
static constexpr uint64_t NEAR_HIGH = 0x3ff1090000000000;
static constexpr uint64_t OFFSET = 0x3fe6000000000000;
static constexpr double TWO_27 = 134217728.0;
static constexpr double TWO_52 = 4503599627370496.0;

static uint64_t get_bits(double value)
{
    uint64_t bits;

    memcpy(&bits, &value, sizeof(bits));
    return (bits);
}

static double get_near_series(double r)
{
    const double *b = LOG_NEAR;
    double r2 = r * r;
    double r3 = r * r2;
    double inner = b[7] + r * b[8] + r2 * b[9] + r3 * b[10];
    double middle = b[4] + r * b[5] + r2 * b[6] + r3 * inner;

    return (r3 * (b[1] + r * b[2] + r2 * b[3] + r3 * middle));
}

static double get_near_one(double value)
{
    double r = value - 1.0;
    double y = get_near_series(r);
    double w = r * TWO_27;
    double high = r + w - w;
    double low = r - high;
    double sum;

    w = high * high * LOG_NEAR[0];
    sum = r + w;
    low = r - sum + w + LOG_NEAR[0] * low * (high + r);
    y += low;
    y += sum;
    return (y);
}

static double get_far(uint64_t bits)
{
    uint64_t tmp = bits - OFFSET;
    uint64_t whole = bits - (tmp & 0xfffULL << 52);
    const double *a = LOG_FAR;
    int i = (int)((tmp >> 45) % 128);
    double kd = (double)((int64_t)tmp >> 52);
    double z;
    double r;
    double w;
    double high;
    double low;
    double series;

    memcpy(&z, &whole, sizeof(z));
    r = (z - LOG_CENTER[i][0] - LOG_CENTER[i][1]) * LOG_INVERSE[i][0];
    w = kd * LOG_LN2[0] + LOG_INVERSE[i][1];
    high = w + r;
    low = w - high + r + kd * LOG_LN2[1];
    series = a[1] + r * a[2] + r * r * (a[3] + r * a[4]);
    return (low + r * r * a[0] + r * (r * r) * series + high);
}

double f64_get_log(double value)
{
    uint64_t bits = get_bits(value);

    if (bits - NEAR_LOW < NEAR_HIGH - NEAR_LOW)
        return (get_near_one(value));
    if (isnan(value) || value <= 0.0)
        return ((double)NAN);
    if (isinf(value))
        return (value);
    if (value < DBL_MIN)
        bits = get_bits(value * TWO_52) - (52ULL << 52);
    return (get_far(bits));
}
