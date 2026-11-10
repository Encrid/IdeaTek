/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "space_pure.h"

static float invert_basis(const float *m, float *out)
{
    float det = m[0] * (m[4] * m[8] - m[7] * m[5])
        - m[3] * (m[1] * m[8] - m[7] * m[2])
        + m[6] * (m[1] * m[5] - m[4] * m[2]);
    float f = 1.0f / (det + (det == 0.0f));

    out[0] = f * (m[4] * m[8] - m[7] * m[5]);
    out[1] = f * (m[7] * m[2] - m[1] * m[8]);
    out[2] = f * (m[1] * m[5] - m[4] * m[2]);
    out[3] = f * (m[6] * m[5] - m[3] * m[8]);
    out[4] = f * (m[0] * m[8] - m[6] * m[2]);
    out[5] = f * (m[3] * m[2] - m[0] * m[5]);
    out[6] = f * (m[3] * m[7] - m[6] * m[4]);
    out[7] = f * (m[6] * m[1] - m[0] * m[7]);
    out[8] = f * (m[0] * m[4] - m[3] * m[1]);
    return (det);
}

bool get_local(const ideatek_world_transform_t *parent,
    const ideatek_world_transform_t *world, ideatek_transform_t *out)
{
    float inv[9];
    float basis[9];
    double d[3] = { world->origin.x - parent->origin.x,
        world->origin.y - parent->origin.y,
        world->origin.z - parent->origin.z };

    if (invert_basis(parent->basis, inv) == 0.0f)
        return (false);
    multiply_basis(inv, world->basis, basis);
    out->position.x = inv[0] * d[0] + inv[3] * d[1] + inv[6] * d[2];
    out->position.y = inv[1] * d[0] + inv[4] * d[1] + inv[7] * d[2];
    out->position.z = inv[2] * d[0] + inv[5] * d[1] + inv[8] * d[2];
    split_basis(basis, &out->rotation, &out->scale);
    return (true);
}
