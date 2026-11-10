/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "gltf_inside.h"

uint32_t stride_of(const gltf_stream_t *s)
{
    return (s->comps * (s->format == 95 ? 2 : 4));
}

uint32_t indices_of(const cgltf_primitive *p)
{
    uint32_t n = (uint32_t)(p->indices ? p->indices->count
        : position_of(p)->count);

    if (p->type == cgltf_primitive_type_triangle_fan)
        return (n >= 3 ? (n - 2) * 3 : 0);
    if (p->type == cgltf_primitive_type_line_loop)
        return (n >= 2 ? n * 2 : 0);
    return (n);
}

static void say_faults(gltf_build_t *b, size_t mesh)
{
    const cgltf_mesh *m = &b->data->meshes[mesh];
    const cgltf_primitive *p;
    uint64_t id = b->file->entity;

    for (size_t i = 0; i < m->primitives_count; i++) {
        p = &m->primitives[i];
        if (is_packed(p))
            say_index(b->api, id, ": a primitive of the mesh %zu is "
                "compressed, and is skipped", mesh);
        else if (!position_of(p))
            say_index(b->api, id, ": a primitive of the mesh %zu has no "
                "POSITION, and is skipped", mesh);
        else if (!is_usable(&b->plans[mesh], p))
            say_index(b->api, id, ": a primitive of the mesh %zu has another "
                "topology, and is skipped", mesh);
    }
    if (b->plans[mesh].full)
        say_index(b->api, id, ": the mesh %zu has more than 256 streams, and "
            "the rest is skipped", mesh);
}

static void write_tables(gltf_build_t *b, size_t mesh, uint8_t *m)
{
    const gltf_plan_t *plan = &b->plans[mesh];
    const gltf_stream_t *s;
    ideatek_mesh_stream_t r;
    ideatek_submesh_t sub = { .first = 0 };
    uint8_t *at = m + plan->count * 16;

    for (uint32_t k = 0; k < plan->count; k++) {
        s = &plan->streams[k];
        r = (ideatek_mesh_stream_t){ s->format, s->offset, stride_of(s),
            s->target, s->semantic, s->set };
        memcpy(m + k * 16, &r, sizeof(r));
    }
    for (size_t i = 0; i < b->data->meshes[mesh].primitives_count; i++) {
        if (!is_usable(plan, &b->data->meshes[mesh].primitives[i]))
            continue;
        sub.count = indices_of(&b->data->meshes[mesh].primitives[i]);
        memcpy(at, &sub, sizeof(sub));
        at += sizeof(sub);
        sub.first += sub.count;
    }
}

void write_mesh(gltf_build_t *b, size_t mesh)
{
    gltf_plan_t *plan = &b->plans[mesh];
    uint8_t *m;
    uint32_t at;

    plan_mesh(b, mesh);
    if (!b->out.base)
        say_faults(b, mesh);
    out_align(&b->out, 8);
    at = b->out.at;
    m = out_take(&b->out, plan->size);
    b->spans[KIND_MESH][mesh] = (gltf_span_t){ at, plan->size };
    if (!m)
        return;
    remap_of(b, mesh);
    write_tables(b, mesh, m);
    write_vertices(b, mesh, m);
    write_indices(b, mesh, m + plan->index_offset);
}
