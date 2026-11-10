/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "gltf_inside.h"

static void add_unique(gltf_plan_t *plan, const gltf_stream_t *s)
{
    const gltf_stream_t *o;

    for (uint32_t k = 0; k < plan->count; k++) {
        o = &plan->streams[k];
        if (o->semantic == s->semantic && o->set == s->set
            && o->target == s->target)
            return;
    }
    plan->full |= plan->count == MAX_STREAMS;
    if (plan->count < MAX_STREAMS)
        plan->streams[plan->count++] = *s;
}

static void add_morph(gltf_plan_t *plan, const cgltf_attribute *a,
    uint16_t target)
{
    gltf_stream_t s;

    if (!stream_of(a, &s) || s.semantic > IDEATEK_SEMANTIC_TANGENT)
        return;
    s = (gltf_stream_t){ .semantic = (uint8_t)(s.semantic
        + IDEATEK_SEMANTIC_MORPH_POSITION), .comps = 3, .format = 106,
        .target = target };
    add_unique(plan, &s);
}

static void add_streams(gltf_plan_t *plan, const cgltf_primitive *p)
{
    gltf_stream_t s;
    const cgltf_morph_target *t;

    for (size_t i = 0; i < p->attributes_count; i++)
        if (stream_of(&p->attributes[i], &s))
            add_unique(plan, &s);
    for (size_t k = 0; k < p->targets_count; k++) {
        t = &p->targets[k];
        for (size_t i = 0; i < t->attributes_count; i++)
            add_morph(plan, &t->attributes[i], (uint16_t)k);
    }
    plan->targets = p->targets_count > plan->targets
        ? (uint32_t)p->targets_count : plan->targets;
}

static void lay_out(gltf_plan_t *plan)
{
    uint32_t at = plan->count * 16 + plan->submeshes * 8;
    gltf_stream_t *s;

    for (uint32_t k = 0; k < plan->count; k++) {
        s = &plan->streams[k];
        s->offset = at;
        at += plan->vertices * stride_of(s);
    }
    plan->wide = plan->vertices > 65535;
    plan->index_offset = at;
    plan->size = (at + plan->indices * (plan->wide ? 4 : 2) + 3) & ~3u;
}

void plan_mesh(gltf_build_t *b, size_t mesh)
{
    const cgltf_mesh *m = &b->data->meshes[mesh];
    gltf_plan_t *plan = &b->plans[mesh];
    const cgltf_primitive *p;

    *plan = (gltf_plan_t){ .topology = IDEATEK_TOPOLOGY_TRIANGLE_LIST };
    for (size_t i = m->primitives_count; i-- > 0;)
        if (!is_packed(&m->primitives[i]))
            plan->topology = topology_of(m->primitives[i].type);
    for (size_t i = 0; i < m->primitives_count; i++) {
        p = &m->primitives[i];
        if (!is_usable(plan, p))
            continue;
        plan->submeshes++;
        plan->vertices += (uint32_t)position_of(p)->count;
        plan->indices += indices_of(p);
        add_streams(plan, p);
    }
    lay_out(plan);
}
