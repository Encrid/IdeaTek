/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "gltf_inside.h"

static void *carve(uint8_t **at, size_t size)
{
    void *here = *at;

    *at += (size + 15) & ~(size_t)15;
    return (here);
}

static size_t most_of(const cgltf_data *d)
{
    size_t most = 0;

    for (size_t i = 0; i < d->accessors_count; i++)
        most = d->accessors[i].count > most ? d->accessors[i].count : most;
    for (size_t i = 0; i < d->skins_count; i++)
        most = d->skins[i].joints_count > most ? d->skins[i].joints_count
            : most;
    return (most);
}

static size_t bytes_of(const cgltf_data *d, size_t most)
{
    size_t parts = d->meshes_count + d->materials_count + d->skins_count
        + d->animations_count + d->scenes_count;

    return (parts * 64 + d->meshes_count * sizeof(gltf_plan_t)
        + d->animations_count * 8 + d->scenes_count * sizeof(ideatek_model_t)
        + d->nodes_count * 12 + most * 68 + 768);
}

static void carve_kinds(gltf_build_t *b, uint8_t **at)
{
    for (uint32_t k = 0; k < KINDS; k++) {
        b->spans[k] = carve(at, sizeof(gltf_span_t) * count_of(b->data, k));
        b->labels[k] = carve(at, sizeof(gltf_span_t) * count_of(b->data, k));
        b->parts[k] = carve(at, sizeof(gltf_part_t *) * count_of(b->data, k));
    }
}

bool take_arrays(gltf_build_t *b)
{
    const cgltf_data *d = b->data;
    size_t most = most_of(d);
    uint8_t *at = parse_take(bytes_of(d, most));

    if (!at)
        return (false);
    carve_kinds(b, &at);
    b->plans = carve(&at, sizeof(gltf_plan_t) * d->meshes_count);
    b->durations = carve(&at, sizeof(float) * d->animations_count);
    b->channels = carve(&at, sizeof(uint32_t) * d->animations_count);
    b->models = carve(&at, sizeof(ideatek_model_t) * d->scenes_count);
    b->ranks = carve(&at, sizeof(uint32_t) * d->nodes_count);
    b->order = carve(&at, sizeof(uint32_t) * d->nodes_count);
    b->where = carve(&at, sizeof(uint32_t) * d->nodes_count);
    b->temp = carve(&at, most * 64);
    b->remap = carve(&at, most * 4);
    return (true);
}
