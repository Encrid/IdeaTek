/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "math_scalar.h"

static double get_result(const f64_root_t *ask, double value)
{
    return (ask->callback(ask->context, value));
}

static bool has_other_sign(double a, double b)
{
    return ((a < 0.0 && b > 0.0) || (a > 0.0 && b < 0.0));
}

static bool halve_range(const f64_root_t *ask, double at_low, double *root)
{
    double low = ask->low;
    double high = ask->high;
    double middle = low + (high - low) * 0.5;
    double at_middle;

    for (uint32_t i = 0; i < ask->steps; i++) {
        at_middle = get_result(ask, middle);
        if (isnan(at_middle))
            return (false);
        if (at_middle == 0.0)
            break;
        if (has_other_sign(at_low, at_middle))
            high = middle;
        else
            low = middle;
        middle = low + (high - low) * 0.5;
    }
    *root = middle;
    return (true);
}

bool f64_find_root(const f64_root_t *ask, double *root)
{
    double at_low = get_result(ask, ask->low);
    double at_high = get_result(ask, ask->high);

    if (at_low == 0.0 || at_high == 0.0) {
        *root = at_low == 0.0 ? ask->low : ask->high;
        return (true);
    }
    if (!has_other_sign(at_low, at_high))
        return (false);
    return (halve_range(ask, at_low, root));
}
