/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
static inline real_t mat3_get_part(real_t sum, real_t sign)
{
    real_t half = real_sqrt(real_max((real_t)0, sum)) * (real_t)0.5;

    return (real_copysign(half, sign));
}

static inline quat_t mat3_get_rotation(mat3_t m)
{
    real_t one = (real_t)1;
    real_t a = m[0][0];
    real_t b = m[1][1];
    real_t c = m[2][2];
    quat_t out;

    out.x = mat3_get_part(one + a - b - c, m[2][1] - m[1][2]);
    out.y = mat3_get_part(one - a + b - c, m[0][2] - m[2][0]);
    out.z = mat3_get_part(one - a - b + c, m[1][0] - m[0][1]);
    out.w = mat3_get_part(one + a + b + c, one);
    return (quat_get_unit(out));
}

static inline mat3_t mat3_make_rotation(quat_t q)
{
    real_t one = (real_t)1;
    real_t two = (real_t)2;
    mat3_t out;

    out[0][0] = one - two * (q.y * q.y + q.z * q.z);
    out[1][0] = two * (q.x * q.y + q.w * q.z);
    out[2][0] = two * (q.x * q.z - q.w * q.y);
    out[0][1] = two * (q.x * q.y - q.w * q.z);
    out[1][1] = one - two * (q.x * q.x + q.z * q.z);
    out[2][1] = two * (q.y * q.z + q.w * q.x);
    out[0][2] = two * (q.x * q.z + q.w * q.y);
    out[1][2] = two * (q.y * q.z - q.w * q.x);
    out[2][2] = one - two * (q.x * q.x + q.y * q.y);
    return (out);
}

static inline mat3_t mat3_make_basis(quat_t rotation, vec3_t scale)
{
    mat3_t out = mat3_make_rotation(rotation);

    for (int row = 0; row < 3; row++) {
        out[row][0] = out[row][0] * scale.x;
        out[row][1] = out[row][1] * scale.y;
        out[row][2] = out[row][2] * scale.z;
    }
    return (out);
}
