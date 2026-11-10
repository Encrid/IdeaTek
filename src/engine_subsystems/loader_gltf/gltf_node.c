/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "gltf_inside.h"

static uint32_t targets_of(gltf_build_t *b, const cgltf_node *n)
{
    return (n->mesh ? b->plans[n->mesh - b->data->meshes].targets : 0);
}

void count_tables(gltf_build_t *b, uint32_t n, gltf_tables_t *t)
{
    const cgltf_node *node;
    char name[NAME_ROOM];

    for (uint32_t i = 0; i < n; i++) {
        node = &b->data->nodes[b->order[i]];
        t->counts[0] += node->light != nullptr;
        t->counts[1] += node->mesh ? b->plans[node->mesh
            - b->data->meshes].submeshes : 0;
        t->counts[2] += targets_of(b, node);
        t->counts[3] += node->camera != nullptr;
        t->counts[4] += name_of_node(b->data, node, name);
    }
}

void place_node(gltf_build_t *b, const cgltf_node *n,
    ideatek_model_node_t *out)
{
    const cgltf_data *d = b->data;

    if (n->has_matrix)
        split_matrix(n->matrix, out);
    else {
        out->position = (ideatek_position_t){ n->translation[0],
            n->translation[1], n->translation[2] };
        out->rotation = (ideatek_quat_t){ n->rotation[0], n->rotation[1],
            n->rotation[2], n->rotation[3] };
        out->scale = (ideatek_vec3_t){ n->scale[0], n->scale[1],
            n->scale[2] };
    }
    out->mesh.id = n->mesh ? b->parts[KIND_MESH][n->mesh - d->meshes]->entity
        : 0;
    out->binds.id = n->skin ? b->parts[KIND_SKIN][n->skin - d->skins]->entity
        : 0;
}

static void fill_extras(gltf_build_t *b, gltf_tables_t *t,
    const cgltf_node *n, ideatek_model_node_t *out)
{
    out->weight_first = t->counts[2];
    out->weight_count = targets_of(b, n);
    for (uint32_t k = 0; k < out->weight_count; k++)
        t->weights[t->counts[2]++] = weight_of(n, k);
    out->light = n->light ? t->counts[0] : GLTF_NONE;
    if (n->light)
        fill_light(n->light, &t->lights[t->counts[0]++]);
    out->camera = n->camera ? t->counts[3] : GLTF_NONE;
    if (n->camera)
        fill_camera(n->camera, &t->cameras[t->counts[3]++]);
}

void fill_node(gltf_build_t *b, gltf_tables_t *t, uint32_t i)
{
    const cgltf_node *n = &b->data->nodes[b->order[i]];
    ideatek_model_node_t *out = &t->nodes[i];
    char name[NAME_ROOM];
    uint32_t len = name_of_node(b->data, n, name);

    *out = (ideatek_model_node_t){ .parent = n->parent
        ? b->where[node_index(b->data, n->parent)] : GLTF_NONE,
        .joint = b->ranks[b->order[i]], .name_offset = t->counts[4],
        .name_length = len };
    memcpy(t->names + t->counts[4], name, len);
    t->counts[4] += len;
    place_node(b, n, out);
    fill_materials(b, t, n, out);
    fill_extras(b, t, n, out);
}
