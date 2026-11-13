/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
static inline real_t real_clamp(real_t value, real_t low, real_t high)
{
    if (value < low)
        return (low);
    if (value > high)
        return (high);
    return (value);
}

static inline real_t real_blend(real_t a, real_t b, real_t share)
{
    real_t rest = (real_t)1 - share;

    return (a * rest + b * share);
}

static inline bool real_is_finite(real_t value)
{
    return (isfinite(value));
}
