/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "gltf_inside.h"

uint32_t out_put(gltf_out_t *o, const void *src, uint32_t len)
{
    uint32_t at = o->at;

    if (o->base)
        memcpy(o->base + at, src, len);
    o->at += len;
    return (at);
}

void *out_take(gltf_out_t *o, uint32_t len)
{
    uint8_t *at = o->base ? o->base + o->at : nullptr;

    o->at += len;
    return (at);
}

void out_align(gltf_out_t *o, uint32_t align)
{
    o->at = (o->at + align - 1) & ~(align - 1);
}

static void write_labels(gltf_build_t *b)
{
    char label[NAME_ROOM];
    uint32_t len;

    for (uint32_t k = 0; k < KINDS; k++)
        for (size_t i = 0; i < count_of(b->data, k); i++) {
            len = label_of(b->data, k, i, label);
            b->labels[k][i] = (gltf_span_t){ out_put(&b->out, label, len),
                len };
        }
    b->fallback_label = (gltf_span_t){ out_put(&b->out, "material/default",
        16), 16 };
}

void write_all(gltf_build_t *b)
{
    const cgltf_scene *s = b->data->scene;

    b->out.at = 0;
    for (size_t i = 0; i < b->data->meshes_count; i++)
        write_mesh(b, i);
    for (size_t i = 0; i < b->data->skins_count; i++)
        write_skin(b, i);
    for (size_t i = 0; i < b->data->animations_count; i++)
        write_clip(b, i);
    for (size_t i = 0; i < b->data->scenes_count; i++)
        write_model(b, b->data->scenes[i].nodes,
            b->data->scenes[i].nodes_count, &b->models[i]);
    s = s ? s : b->data->scenes;
    if (s)
        write_model(b, s->nodes, s->nodes_count, &b->file_model);
    else
        write_model(b, nullptr, 0, &b->file_model);
    write_labels(b);
}
