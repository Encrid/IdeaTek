/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "gltf_inside.h"

static const cgltf_skin *skin_of(const cgltf_data *data, size_t mesh)
{
    for (size_t i = 0; i < data->nodes_count; i++)
        if (data->nodes[i].mesh == &data->meshes[mesh] && data->nodes[i].skin)
            return (data->nodes[i].skin);
    return (nullptr);
}

void remap_of(gltf_build_t *b, size_t mesh)
{
    const cgltf_skin *s = skin_of(b->data, mesh);

    b->remap_count = s ? (uint32_t)s->joints_count : 0;
    for (uint32_t j = 0; j < b->remap_count; j++)
        b->remap[j] = b->ranks[node_index(b->data, s->joints[j])];
}

void write_joints(gltf_build_t *b, const cgltf_accessor *a, uint16_t *dst)
{
    float *values = (float *)b->temp;
    uint32_t j;
    uint32_t v;

    cgltf_accessor_unpack_floats(a, values, a->count * 4);
    for (size_t i = 0; i < a->count * 4; i++) {
        j = (uint32_t)values[i];
        v = j;
        if (b->remap_count)
            v = j < b->remap_count ? b->remap[j] : 0;
        dst[i] = (uint16_t)v;
    }
}
