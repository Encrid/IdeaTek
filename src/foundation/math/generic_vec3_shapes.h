/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
static inline vec3_t vec3_get_unit(vec3_t a)
{
    real_t length = vec3_get_length(a);

    if (length == (real_t)0)
        return (a);
    return (a / length);
}

static inline vec3_t vec3_get_blend(vec3_t a, vec3_t b, real_t share)
{
    real_t rest = (real_t)1 - share;

    return (a * rest + b * share);
}

static inline vec3_t vec3_get_least(vec3_t a, vec3_t b)
{
    vec3_t out;

    out.x = a.x < b.x ? a.x : b.x;
    out.y = a.y < b.y ? a.y : b.y;
    out.z = a.z < b.z ? a.z : b.z;
    return (out);
}

static inline vec3_t vec3_get_most(vec3_t a, vec3_t b)
{
    vec3_t out;

    out.x = a.x > b.x ? a.x : b.x;
    out.y = a.y > b.y ? a.y : b.y;
    out.z = a.z > b.z ? a.z : b.z;
    return (out);
}

static inline bool vec3_is_finite(vec3_t a)
{
    return (isfinite(a.x) && isfinite(a.y) && isfinite(a.z));
}
