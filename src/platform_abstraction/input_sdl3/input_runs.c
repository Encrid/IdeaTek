/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "input_inside.h"

bool joins_run(const bridge_api_t *api, uint32_t slot, uint64_t key)
{
    const input_settings_t *s = bridge_native_get_single(api, SETTINGS);
    const input_run_t *run = &input_module.run;

    return (s->merge_motion && run->pending && run->slot == slot
        && run->key == key);
}

void flush_run(const bridge_api_t *api)
{
    input_run_t *run = &input_module.run;

    if (!run->pending)
        return;
    run->pending = false;
    api->attach(api, run->entity, run->slot, bridge_native_lend(api,
        &run->value, run->size));
    input_module.items[input_module.item_count++] = run->entity;
}

void start_run(const bridge_api_t *api, uint32_t slot, uint64_t key,
    uint32_t size)
{
    input_run_t *run = &input_module.run;

    flush_run(api);
    run->slot = slot;
    run->key = key;
    run->size = size;
    run->entity = spawn_entity(api);
    run->pending = true;
}
