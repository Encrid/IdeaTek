/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "math_place.h"
#include "space_pure.h"

static constexpr uint8_t SPLIT_BITS = IDEATEK_ABSOLUTE_ROTATION
    | IDEATEK_ABSOLUTE_SCALE;

void get_matrix(const ideatek_transform_t *t, ideatek_world_transform_t *out)
{
    quatf_t q = { t->rotation.x, t->rotation.y, t->rotation.z, t->rotation.w };
    vec3f_t s = { t->scale.x, t->scale.y, t->scale.z };

    out->origin = t->position;
    mat3f_store(mat3f_make_basis(quatf_get_unit(q), s), out->basis);
}

void multiply_basis(const float *a, const float *b, float *out)
{
    mat3f_store(mat3f_get_product(mat3f_load(a), mat3f_load(b)), out);
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
    vec3d_t point = { local->origin.x, local->origin.y, local->origin.z };
    affine_t place = { .origin = { parent->origin.x, parent->origin.y,
        parent->origin.z } };
    vec3d_t moved;
    float m[9];

    mask_basis(parent->basis, absolute, m);
    multiply_basis(m, local->basis, out->basis);
    out->origin = local->origin;
    if (absolute & IDEATEK_ABSOLUTE_POSITION)
        return;
    place.basis = mat3f_load(m);
    moved = affine_move(&place, point);
    out->origin = (ideatek_position_t){ moved.x, moved.y, moved.z };
}
