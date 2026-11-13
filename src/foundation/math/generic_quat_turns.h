/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
static inline vec3_t quat_turn(quat_t q, vec3_t v)
{
    vec3_t axis = { q.x, q.y, q.z };
    vec3_t twice = vec3_get_cross(axis, v) * (real_t)2;

    return (v + twice * q.w + vec3_get_cross(axis, twice));
}

static inline vec3_t quat_turn_back(quat_t q, vec3_t v)
{
    return (quat_turn(quat_get_conjugate(q), v));
}

static inline quat_t quat_make_turn(vec3_t axis, real_t angle)
{
    real_t half = angle * (real_t)0.5;
    real_t sine = real_get_sin(half);
    quat_t out;

    out.x = axis.x * sine;
    out.y = axis.y * sine;
    out.z = axis.z * sine;
    out.w = real_get_cos(half);
    return (out);
}

static inline real_t quat_get_angle(quat_t q)
{
    vec3_t axis = { q.x, q.y, q.z };
    real_t side = vec3_get_length(axis);

    return ((real_t)2 * real_get_atan2(side, real_abs(q.w)));
}

static inline vec3_t quat_get_axis(quat_t q)
{
    vec3_t axis = { q.x, q.y, q.z };
    vec3_t first = { (real_t)1, (real_t)0, (real_t)0 };
    real_t side = vec3_get_length(axis);

    if (side == (real_t)0)
        return (first);
    if (q.w < (real_t)0)
        return (-axis / side);
    return (axis / side);
}
