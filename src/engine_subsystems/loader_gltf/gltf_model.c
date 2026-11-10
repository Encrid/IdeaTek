/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "gltf_inside.h"

static void visit(gltf_build_t *b, const cgltf_node *node, uint32_t *n)
{
    uint32_t index = node_index(b->data, node);

    b->where[index] = *n;
    b->order[(*n)++] = index;
    for (size_t i = 0; i < node->children_count; i++)
        visit(b, node->children[i], n);
}

static uint32_t size_of(const gltf_tables_t *t, uint32_t n)
{
    return (n * (uint32_t)sizeof(ideatek_model_node_t) + t->counts[0]
        * (uint32_t)sizeof(ideatek_light_t) + t->counts[1] * 8 + t->counts[2]
        * 4 + t->counts[3] * (uint32_t)sizeof(ideatek_projection_t)
        + t->counts[4]);
}

static void point_tables(gltf_tables_t *t, uint8_t *base, uint32_t n)
{
    t->nodes = (void *)base;
    t->lights = (void *)(t->nodes + n);
    t->materials = (void *)(t->lights + t->counts[0]);
    t->weights = (void *)(t->materials + t->counts[1]);
    t->cameras = (void *)(t->weights + t->counts[2]);
    t->names = (void *)(t->cameras + t->counts[3]);
    for (uint32_t k = 0; k < 5; k++)
        t->counts[k] = 0;
}

static ideatek_model_t model_of(gltf_build_t *b, const gltf_tables_t *t,
    uint32_t n)
{
    ideatek_model_t m = {
        .bytes = { b->out.at, size_of(t, n) },
        .node_count = n,
        .light_count = t->counts[0],
        .material_count = t->counts[1],
        .weight_count = t->counts[2],
        .camera_count = t->counts[3],
        .joint_count = b->joint_count };

    return (m);
}

void write_model(gltf_build_t *b, cgltf_node *const *roots, size_t count,
    ideatek_model_t *model)
{
    gltf_tables_t t = {};
    uint32_t n = 0;
    uint8_t *base;

    for (size_t i = 0; i < count; i++)
        visit(b, roots[i], &n);
    count_tables(b, n, &t);
    out_align(&b->out, 8);
    *model = model_of(b, &t, n);
    base = out_take(&b->out, model->bytes.len);
    if (!base)
        return;
    point_tables(&t, base, n);
    for (uint32_t i = 0; i < n; i++)
        fill_node(b, &t, i);
}
