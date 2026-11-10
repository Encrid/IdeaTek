/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "gltf_inside.h"

static void give_label(gltf_build_t *b, gltf_part_t *p, gltf_span_t label)
{
    ideatek_resource_label_t l = { .resource = { b->file->entity },
        .label = text_of(b, label) };

    BRIDGE_NATIVE_ATTACH(b->api, p->entity, LABEL, l);
    p->revision++;
}

static void give_mesh(gltf_build_t *b, size_t i)
{
    gltf_part_t *p = b->parts[KIND_MESH][i];
    const gltf_plan_t *plan = &b->plans[i];
    ideatek_mesh_t m = { .bytes = text_of(b, b->spans[KIND_MESH][i]),
        .vertex_count = plan->vertices, .index_count = plan->indices,
        .index_offset = plan->index_offset, .stream_count = plan->count,
        .submesh_count = plan->submeshes, .lod_count = 1,
        .target_count = plan->targets, .topology = plan->topology,
        .index_format = plan->wide ? IDEATEK_INDEX_FORMAT_U32
        : IDEATEK_INDEX_FORMAT_U16 };

    give_label(b, p, b->labels[KIND_MESH][i]);
    m.revision = p->revision;
    BRIDGE_NATIVE_ATTACH(b->api, p->entity, MESH, m);
}

static void give_motion(gltf_build_t *b, uint32_t kind, size_t i)
{
    gltf_part_t *p = b->parts[kind][i];
    ideatek_skin_binds_t s = { .matrices = text_of(b, b->spans[kind][i]),
        .count = b->joint_count };
    ideatek_clip_t c = { .bytes = s.matrices, .duration = 0.0f };

    give_label(b, p, b->labels[kind][i]);
    s.revision = p->revision;
    c.revision = p->revision;
    if (kind == KIND_SKIN) {
        BRIDGE_NATIVE_ATTACH(b->api, p->entity, BINDS, s);
        return;
    }
    c.duration = b->durations[i];
    c.channel_count = b->channels[i];
    BRIDGE_NATIVE_ATTACH(b->api, p->entity, CLIP, c);
}

static void give_model(gltf_build_t *b, uint64_t id, ideatek_model_t *m,
    uint32_t revision)
{
    m->bytes = text_of(b, (gltf_span_t){ m->bytes.at, m->bytes.len });
    m->revision = revision;
    BRIDGE_NATIVE_ATTACH(b->api, id, MODEL, *m);
}

void attach_all(gltf_build_t *b)
{
    gltf_part_t *p;

    for (size_t i = 0; i < b->data->meshes_count; i++)
        give_mesh(b, i);
    give_materials(b);
    for (size_t i = 0; i < b->data->skins_count; i++)
        give_motion(b, KIND_SKIN, i);
    for (size_t i = 0; i < b->data->animations_count; i++)
        give_motion(b, KIND_CLIP, i);
    for (size_t i = 0; i < b->data->scenes_count; i++) {
        p = b->parts[KIND_SCENE][i];
        give_label(b, p, b->labels[KIND_SCENE][i]);
        give_model(b, p->entity, &b->models[i], p->revision);
    }
    give_model(b, b->file->entity, &b->file_model, ++b->file->revision);
}
