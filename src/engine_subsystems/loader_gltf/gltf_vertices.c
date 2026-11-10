/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "gltf_inside.h"

const cgltf_accessor *accessor_of(const cgltf_primitive *p,
    const gltf_stream_t *s)
{
    const cgltf_attribute *list = p->attributes;
    size_t count = p->attributes_count;
    uint8_t semantic = s->semantic;
    gltf_stream_t found;

    if (semantic >= IDEATEK_SEMANTIC_MORPH_POSITION) {
        semantic -= IDEATEK_SEMANTIC_MORPH_POSITION;
        list = p->targets[s->target].attributes;
        count = p->targets[s->target].attributes_count;
    }
    for (size_t i = 0; i < count; i++)
        if (stream_of(&list[i], &found) && found.semantic == semantic
            && found.set == s->set)
            return (list[i].data);
    return (nullptr);
}

static void put_defaults(const gltf_stream_t *s, uint8_t *dst, uint32_t n)
{
    float *f = (float *)dst;
    bool color = s->semantic == IDEATEK_SEMANTIC_COLOR;
    bool weight = s->semantic == IDEATEK_SEMANTIC_WEIGHTS && s->set == 0;

    memset(dst, 0, (size_t)n * stride_of(s));
    if (!color && !weight)
        return;
    for (uint32_t i = 0; i < n * s->comps; i++)
        if (color || i % 4 == 0)
            f[i] = 1.0f;
}

static void expand(gltf_build_t *b, const cgltf_accessor *a, float *dst)
{
    float *rgb = (float *)b->temp;

    cgltf_accessor_unpack_floats(a, rgb, a->count * 3);
    for (size_t i = 0; i < a->count; i++) {
        memcpy(dst + i * 4, rgb + i * 3, 3 * sizeof(float));
        dst[i * 4 + 3] = 1.0f;
    }
}

void fill_stream(gltf_build_t *b, const cgltf_primitive *p,
    const gltf_stream_t *s, uint8_t *dst)
{
    const cgltf_accessor *a = accessor_of(p, s);
    uint32_t n = (uint32_t)position_of(p)->count;

    if (!a) {
        put_defaults(s, dst, n);
        return;
    }
    if (s->semantic == IDEATEK_SEMANTIC_JOINTS)
        write_joints(b, a, (uint16_t *)dst);
    else if (cgltf_num_components(a->type) != s->comps)
        expand(b, a, (float *)dst);
    else
        cgltf_accessor_unpack_floats(a, (float *)dst, (size_t)n * s->comps);
}

void write_vertices(gltf_build_t *b, size_t mesh, uint8_t *base)
{
    const gltf_plan_t *plan = &b->plans[mesh];
    const cgltf_mesh *m = &b->data->meshes[mesh];
    const gltf_stream_t *s;
    uint32_t vb = 0;

    for (size_t i = 0; i < m->primitives_count; i++) {
        if (!is_usable(plan, &m->primitives[i]))
            continue;
        for (uint32_t k = 0; k < plan->count; k++) {
            s = &plan->streams[k];
            fill_stream(b, &m->primitives[i], s, base + s->offset + vb
                * stride_of(s));
        }
        vb += (uint32_t)position_of(&m->primitives[i])->count;
    }
}
