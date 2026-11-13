/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "math_mat3.h"
#include "space_pure.h"

void split_basis(const float basis[9], ideatek_quat_t *rotation,
    ideatek_vec3_t *scale)
{
    quatf_t q;
    vec3f_t s;

    mat3f_split(mat3f_load(basis), &q, &s);
    *rotation = (ideatek_quat_t){ q.x, q.y, q.z, q.w };
    *scale = (ideatek_vec3_t){ s.x, s.y, s.z };
}
