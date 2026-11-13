/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
static inline real_t vec3_get_dot(vec3_t a, vec3_t b)
{
    return (a.x * b.x + a.y * b.y + a.z * b.z);
}

static inline vec3_t vec3_get_cross(vec3_t a, vec3_t b)
{
    vec3_t out;

    out.x = a.y * b.z - a.z * b.y;
    out.y = a.z * b.x - a.x * b.z;
    out.z = a.x * b.y - a.y * b.x;
    return (out);
}

static inline real_t vec3_get_square(vec3_t a)
{
    return (vec3_get_dot(a, a));
}

static inline real_t vec3_get_length(vec3_t a)
{
    return (real_sqrt(vec3_get_square(a)));
}

static inline real_t vec3_get_distance(vec3_t a, vec3_t b)
{
    return (vec3_get_length(b - a));
}
