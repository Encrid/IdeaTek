/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <math.h>

#include "space_pure.h"

static float get_part(float sum, float sign)
{
    return (copysignf(sqrtf(fmaxf(0.0f, sum)) * 0.5f, sign));
}

ideatek_quat_t get_rotation(const float m[9])
{
    ideatek_quat_t q = { get_part(1.0f + m[0] - m[4] - m[8], m[5] - m[7]),
        get_part(1.0f - m[0] + m[4] - m[8], m[6] - m[2]),
        get_part(1.0f - m[0] - m[4] + m[8], m[1] - m[3]),
        get_part(1.0f + m[0] + m[4] + m[8], 1.0f) };
    float length = sqrtf(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);

    return ((ideatek_quat_t){ q.x / length, q.y / length, q.z / length,
        q.w / length });
}

void get_rotation_matrix(const ideatek_quat_t *r, float m[9])
{
    float n = sqrtf(r->x * r->x + r->y * r->y + r->z * r->z + r->w * r->w);
    float x = r->x / n;
    float y = r->y / n;
    float z = r->z / n;
    float w = r->w / n;

    m[0] = 1.0f - 2.0f * (y * y + z * z);
    m[1] = 2.0f * (x * y + w * z);
    m[2] = 2.0f * (x * z - w * y);
    m[3] = 2.0f * (x * y - w * z);
    m[4] = 1.0f - 2.0f * (x * x + z * z);
    m[5] = 2.0f * (y * z + w * x);
    m[6] = 2.0f * (x * z + w * y);
    m[7] = 2.0f * (y * z - w * x);
    m[8] = 1.0f - 2.0f * (x * x + y * y);
}
