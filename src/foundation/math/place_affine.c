/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "math_place.h"

void affine_compose(const affine_t *parent, const affine_t *local,
    affine_t *out)
{
    vec3d_t origin = affine_move(parent, local->origin);

    out->basis = mat3f_get_product(parent->basis, local->basis);
    out->origin = origin;
}

bool affine_invert(const affine_t *place, affine_t *out)
{
    mat3f_t back;

    if (!mat3f_get_inverse(place->basis, &back))
        return (false);
    out->origin = -mat3d_turn(mat3f_get_wide(back), place->origin);
    out->basis = back;
    return (true);
}
