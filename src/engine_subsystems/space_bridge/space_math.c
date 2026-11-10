/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "space_pure.h"

static constexpr uint8_t SPLIT_BITS = IDEATEK_ABSOLUTE_ROTATION
    | IDEATEK_ABSOLUTE_SCALE;

void get_matrix(const ideatek_transform_t *t, ideatek_world_transform_t *out)
{
    float r[9];
    const float s[3] = { t->scale.x, t->scale.y, t->scale.z };

    get_rotation_matrix(&t->rotation, r);
    out->origin = t->position;
    for (int i = 0; i < 9; i++)
        out->basis[i] = r[i] * s[i / 3];
}

void multiply_basis(const float *a, const float *b, float *out)
{
    for (int c = 0; c < 3; c++)
        for (int r = 0; r < 3; r++)
            out[3 * c + r] = a[r] * b[3 * c] + a[3 + r] * b[3 * c + 1]
                + a[6 + r] * b[3 * c + 2];
}

static void mask_basis(const float *basis, uint8_t absolute, float *out)
{
    ideatek_transform_t t = { .rotation = { 0.0f, 0.0f, 0.0f, 1.0f },
        .scale = { 1.0f, 1.0f, 1.0f } };
    ideatek_quat_t q;
    ideatek_vec3_t s;
    ideatek_world_transform_t w;

    if (!(absolute & SPLIT_BITS)) {
        memcpy(out, basis, 9 * sizeof(float));
        return;
    }
    split_basis(basis, &q, &s);
    if (!(absolute & IDEATEK_ABSOLUTE_ROTATION))
        t.rotation = q;
    if (!(absolute & IDEATEK_ABSOLUTE_SCALE))
        t.scale = s;
    get_matrix(&t, &w);
    memcpy(out, w.basis, 9 * sizeof(float));
}

void compose(const ideatek_world_transform_t *parent,
    const ideatek_world_transform_t *local, uint8_t absolute,
    ideatek_world_transform_t *out)
{
    float m[9];
    const ideatek_position_t *p = &local->origin;

    mask_basis(parent->basis, absolute, m);
    multiply_basis(m, local->basis, out->basis);
    out->origin = local->origin;
    if (absolute & IDEATEK_ABSOLUTE_POSITION)
        return;
    out->origin.x = parent->origin.x + m[0] * p->x + m[3] * p->y + m[6] * p->z;
    out->origin.y = parent->origin.y + m[1] * p->x + m[4] * p->y + m[7] * p->z;
    out->origin.z = parent->origin.z + m[2] * p->x + m[5] * p->y + m[8] * p->z;
}
