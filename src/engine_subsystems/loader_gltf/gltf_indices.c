/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "gltf_inside.h"

static void put(gltf_emit_t *e, uint32_t v)
{
    uint32_t value = v + e->base;
    uint16_t narrow = (uint16_t)value;

    if (e->wide)
        memcpy(e->dst + (size_t)e->at * 4, &value, 4);
    else
        memcpy(e->dst + (size_t)e->at * 2, &narrow, 2);
    e->at++;
}

static const uint32_t *source_of(gltf_build_t *b, const cgltf_primitive *p,
    uint32_t *n)
{
    uint32_t *src = (uint32_t *)b->temp;

    *n = (uint32_t)(p->indices ? p->indices->count : position_of(p)->count);
    if (p->indices)
        cgltf_accessor_unpack_indices(p->indices, src, 4, *n);
    else
        for (uint32_t i = 0; i < *n; i++)
            src[i] = i;
    return (src);
}

static void emit(gltf_emit_t *e, cgltf_primitive_type type,
    const uint32_t *src, uint32_t n)
{
    if (type == cgltf_primitive_type_triangle_fan) {
        for (uint32_t i = 1; i + 1 < n; i++) {
            put(e, src[0]);
            put(e, src[i]);
            put(e, src[i + 1]);
        }
        return;
    }
    if (type == cgltf_primitive_type_line_loop) {
        for (uint32_t i = 0; n >= 2 && i < n; i++) {
            put(e, src[i]);
            put(e, src[(i + 1) % n]);
        }
        return;
    }
    for (uint32_t i = 0; i < n; i++)
        put(e, src[i]);
}

void write_indices(gltf_build_t *b, size_t mesh, uint8_t *dst)
{
    const gltf_plan_t *plan = &b->plans[mesh];
    const cgltf_mesh *m = &b->data->meshes[mesh];
    gltf_emit_t e = { .dst = dst, .wide = plan->wide };
    const uint32_t *src;
    uint32_t n;

    for (size_t i = 0; i < m->primitives_count; i++) {
        if (!is_usable(plan, &m->primitives[i]))
            continue;
        src = source_of(b, &m->primitives[i], &n);
        emit(&e, m->primitives[i].type, src, n);
        e.base += (uint32_t)position_of(&m->primitives[i])->count;
    }
}
