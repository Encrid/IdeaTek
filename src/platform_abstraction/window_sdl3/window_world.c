/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "window_inside.h"

uint32_t get_cell(const bridge_api_t *api, uint64_t id, uint32_t slot)
{
    const bridge_header_t *h = bridge_native_get_header(api);
    const bridge_entity_t *index = (const bridge_entity_t *)(api->shared_base
        + h->entity_index);
    const uint32_t *bases = (const uint32_t *)(api->private_base
        + h->column_base);
    const uint32_t *strides = (const uint32_t *)(api->private_base
        + h->stride);
    const bridge_entity_t *record = index + (uint32_t)id;
    uint32_t base;

    if ((uint32_t)id >= h->entities
        || record->generation != (uint32_t)(id >> 32))
        return (0);
    base = bases[record->table * h->types + slot];
    if (!base)
        return (0);
    return (base + record->row * strides[slot]);
}

uint64_t spawn_entity(const bridge_api_t *api)
{
    window_module.spawned++;
    return (api->spawn(api));
}

bool has_room(const bridge_api_t *api, uint32_t spawns, uint32_t bytes)
{
    const bridge_header_t *h = bridge_native_get_header(api);

    return (window_module.spawned + spawns <= WINDOW_SPAWNS
        && h->changes_left >= bytes);
}
