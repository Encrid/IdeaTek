/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <string.h>

#include "gltf_inside.h"

uint32_t node_index(const cgltf_data *data, const cgltf_node *node)
{
    return ((uint32_t)(node - data->nodes));
}

void rank_joints(gltf_build_t *b)
{
    const cgltf_skin *s;
    uint32_t n;

    for (size_t i = 0; i < b->data->nodes_count; i++)
        b->ranks[i] = GLTF_NONE;
    b->joint_count = 0;
    for (size_t k = 0; k < b->data->skins_count; k++) {
        s = &b->data->skins[k];
        for (size_t j = 0; j < s->joints_count; j++) {
            n = node_index(b->data, s->joints[j]);
            b->ranks[n] = b->ranks[n] == GLTF_NONE ? b->joint_count++
                : b->ranks[n];
        }
    }
}

uint32_t name_of_node(const cgltf_data *data, const cgltf_node *node,
    char *out)
{
    if (node->name && node->name[0])
        snprintf(out, NAME_ROOM, "%s", node->name);
    else
        snprintf(out, NAME_ROOM, "%u", node_index(data, node));
    return ((uint32_t)strlen(out));
}

static uint64_t hash_path(const cgltf_data *data, const cgltf_node *node,
    uint64_t h)
{
    char name[NAME_ROOM];
    uint32_t len = name_of_node(data, node, name);

    if (node->parent)
        h = hash_text("/", 1, hash_path(data, node->parent, h));
    return (hash_text(name, len, h));
}

uint64_t target_of(const cgltf_data *data, const cgltf_node *node)
{
    return (hash_path(data, node, 0));
}
