/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "input_inside.h"

bool has_room(const bridge_api_t *api, uint32_t spawns, uint32_t bytes)
{
    const input_settings_t *s = bridge_native_get_single(api, SETTINGS);
    const bridge_header_t *h = bridge_native_get_header(api);
    uint32_t held = input_module.run.pending
        ? CHANGE_SIZE(input_module.run.size) : 0;

    return (input_module.spawned + spawns <= s->round_entities
        && h->changes_left >= bytes + held);
}

uint32_t next_order(void)
{
    return (input_module.order++);
}

void spawn_event(const bridge_api_t *api, uint32_t slot, const void *value,
    uint32_t size)
{
    uint64_t id = spawn_entity(api);

    api->attach(api, id, slot, bridge_native_lend(api, value, size));
    input_module.items[input_module.item_count++] = id;
}

void turn_items(const bridge_api_t *api)
{
    for (uint32_t i = 0; i < input_module.item_count; i++)
        api->destroy(api, input_module.items[i]);
    input_module.item_count = 0;
    input_module.spawned = 0;
    input_module.order = 0;
}

uint64_t spawn_entity(const bridge_api_t *api)
{
    input_module.spawned++;
    return (api->spawn(api));
}
