/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "resources_inside.h"

void attach_handle(const bridge_api_t *api, uint32_t k, uint64_t resource)
{
    ideatek_resource_handle_t handle = { .resource = { resource } };

    BRIDGE_NATIVE_ATTACH(api, res_module.uses[k].entity, HANDLE, handle);
    res_module.uses[k].handled = true;
}

static uint32_t add_record(const bridge_api_t *api,
    const ideatek_resource_add_t *add)
{
    res_text_t type = res_text(api, add->type);
    res_text_t empty = { "", 0 };
    uint32_t i = resource_of(add->resource.id);

    if (i != RES_NONE)
        return (i);
    i = take_resource(&empty, &type, &empty);
    if (i == RES_NONE) {
        bridge_native_say(api, BRIDGE_LOG_WARN, "no room for the resource %.*s",
            (int)type.len, type.at);
        return (RES_NONE);
    }
    bind_resource(i, add->resource.id);
    res_module.resources[i].step = STEP_DONE;
    res_module.resources[i].view.state = IDEATEK_LOAD_STATE_LOADED;
    return (i);
}

void place_add(const bridge_api_t *api, uint32_t k)
{
    res_use_t *u = &res_module.uses[k];
    uint32_t at = bridge_native_find_value(api, u->entity, ADD);
    const ideatek_resource_add_t *add = (const void *)(api->shared_base + at);
    uint32_t i = add_record(api, add);

    u->handled = true;
    if (i == RES_NONE)
        return;
    u->resource = i;
    res_module.resources[i].count++;
    res_module.dirty = true;
    attach_handle(api, k, add->resource.id);
}

BRIDGE_NATIVE(adds)
{
    record_uses(api, LIST_ADDS, true);
    return (0);
}
