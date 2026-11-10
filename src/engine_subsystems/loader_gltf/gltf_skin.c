/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "gltf_inside.h"

static const float IDENTITY[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0,
    0, 1 };

static void fill_binds(gltf_build_t *b, const cgltf_skin *s, float *m)
{
    const float *ibm = (const float *)b->temp;
    uint32_t r;

    if (s->inverse_bind_matrices)
        cgltf_accessor_unpack_floats(s->inverse_bind_matrices,
            (float *)b->temp, s->joints_count * 16);
    for (size_t j = 0; j < s->joints_count; j++) {
        r = b->ranks[node_index(b->data, s->joints[j])];
        memcpy(m + (size_t)r * 16, s->inverse_bind_matrices ? ibm + j * 16
            : IDENTITY, sizeof(IDENTITY));
    }
}

void write_skin(gltf_build_t *b, size_t skin)
{
    uint32_t len = b->joint_count * (uint32_t)sizeof(IDENTITY);
    uint32_t at;
    float *m;

    out_align(&b->out, 8);
    at = b->out.at;
    m = out_take(&b->out, len);
    b->spans[KIND_SKIN][skin] = (gltf_span_t){ at, len };
    if (!m)
        return;
    for (uint32_t r = 0; r < b->joint_count; r++)
        memcpy(m + (size_t)r * 16, IDENTITY, sizeof(IDENTITY));
    fill_binds(b, &b->data->skins[skin], m);
}
