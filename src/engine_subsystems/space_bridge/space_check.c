/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "space_pure.h"

static bool is_finite(double v)
{
    return (v - v == 0.0);
}

bool is_transform_ok(const ideatek_transform_t *t)
{
    const ideatek_quat_t *q = &t->rotation;
    float length = q->x * q->x + q->y * q->y + q->z * q->z + q->w * q->w;
    double sum = t->position.x + t->position.y + t->position.z
        + t->scale.x + t->scale.y + t->scale.z + length;

    return (is_finite(sum) && length > 0.0f);
}

bool is_same_world(const ideatek_world_transform_t *a,
    const ideatek_world_transform_t *b)
{
    return (memcmp(&a->origin, &b->origin, sizeof(a->origin)) == 0
        && memcmp(a->basis, b->basis, sizeof(a->basis)) == 0);
}
