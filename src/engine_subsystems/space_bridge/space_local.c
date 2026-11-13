/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "math_place.h"
#include "space_pure.h"

bool get_local(const ideatek_world_transform_t *parent,
    const ideatek_world_transform_t *world, ideatek_transform_t *out)
{
    vec3d_t apart = { world->origin.x - parent->origin.x,
        world->origin.y - parent->origin.y,
        world->origin.z - parent->origin.z };
    mat3f_t inverse;
    vec3d_t position;
    float basis[9];

    if (!mat3f_get_inverse(mat3f_load(parent->basis), &inverse))
        return (false);
    mat3f_store(mat3f_get_product(inverse, mat3f_load(world->basis)), basis);
    position = mat3d_turn(mat3f_get_wide(inverse), apart);
    out->position = (ideatek_position_t){ position.x, position.y, position.z };
    split_basis(basis, &out->rotation, &out->scale);
    return (true);
}
