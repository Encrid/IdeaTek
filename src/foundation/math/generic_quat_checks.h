/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
static inline bool quat_is_finite(quat_t q)
{
    return (isfinite(q.x) && isfinite(q.y) && isfinite(q.z) && isfinite(q.w));
}

static inline bool quat_is_unit(quat_t q, real_t margin)
{
    return (real_abs(quat_get_length(q) - (real_t)1) <= margin);
}

static inline quat_t quat_get_blend(quat_t a, quat_t b, real_t share)
{
    quat_t to = quat_get_dot(a, b) < (real_t)0 ? -b : b;
    real_t apart = quat_get_length(a - to);
    real_t angle = (real_t)2 * real_get_atan2(apart, quat_get_length(a + to));
    real_t sine = real_get_sin(angle);
    real_t first;
    real_t second;

    if (sine == (real_t)0)
        return (a);
    first = real_get_sin(((real_t)1 - share) * angle) / sine;
    second = real_get_sin(share * angle) / sine;
    return (a * first + to * second);
}
