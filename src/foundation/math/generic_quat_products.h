/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
static inline real_t quat_get_dot(quat_t a, quat_t b)
{
    return (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
}

static inline real_t quat_get_length(quat_t q)
{
    return (real_sqrt(quat_get_dot(q, q)));
}

static inline quat_t quat_get_product(quat_t a, quat_t b)
{
    quat_t out;

    out.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;
    out.y = a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x;
    out.z = a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w;
    out.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;
    return (out);
}

static inline quat_t quat_get_conjugate(quat_t q)
{
    quat_t out;

    out.x = -q.x;
    out.y = -q.y;
    out.z = -q.z;
    out.w = q.w;
    return (out);
}

static inline quat_t quat_get_unit(quat_t q)
{
    real_t length = quat_get_length(q);
    quat_t same = { (real_t)0, (real_t)0, (real_t)0, (real_t)1 };

    if (length == (real_t)0)
        return (same);
    return (q / length);
}
