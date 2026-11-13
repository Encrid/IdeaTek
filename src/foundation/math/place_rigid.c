/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "math_place.h"

void rigid_compose(const rigid_t *parent, const rigid_t *local, rigid_t *out)
{
    vec3d_t origin = rigid_move(parent, local->origin);

    out->rotation = quatd_get_product(parent->rotation, local->rotation);
    out->origin = origin;
}

void rigid_invert(const rigid_t *place, rigid_t *out)
{
    quatd_t back = quatd_get_conjugate(place->rotation);

    out->origin = -quatd_turn(back, place->origin);
    out->rotation = back;
}
