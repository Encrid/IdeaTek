/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "resources_inside.h"

static void fill_use(res_use_t *u, uint64_t entity,
    const ideatek_resource_load_t *load, bool add)
{
    u->entity = entity;
    u->resource = RES_NONE;
    u->add = add;
    u->handled = false;
    u->owner = add ? 0 : load->owner.id;
    u->priority = add ? 0 : load->priority;
    u->reload_owner = !add && load->reload_owner;
}

static void take_use(const bridge_api_t *api, uint64_t entity, bool add)
{
    uint32_t at = bridge_native_find_value(api, entity, add ? ADD : LOAD);
    const ideatek_resource_load_t *load = (const void *)(api->shared_base
        + at);
    uint32_t k = 0;

    while (k < res_module.s.max_uses && res_module.uses[k].entity)
        k++;
    if (k == res_module.s.max_uses) {
        bridge_native_say(api, BRIDGE_LOG_WARN, "no room for the use %llu",
            (unsigned long long)entity);
        return;
    }
    fill_use(&res_module.uses[k], entity, load, add);
    map_put(&res_module.use_by_entity, entity, k);
}

static void drop_use(uint32_t k)
{
    res_use_t *u = &res_module.uses[k];

    if (u->resource != RES_NONE) {
        res_module.resources[u->resource].count--;
        res_module.dirty = true;
    }
    map_drop(&res_module.use_by_entity, u->entity);
    u->entity = 0;
}

void record_uses(const bridge_api_t *api, uint32_t list, bool add)
{
    const bridge_list_t *l = bridge_native_get_list(api,
        res_module.lists[list]);
    const bridge_trigger_t *t = (const void *)(api->private_base + l->at);
    uint32_t k;
    bool leave;

    for (uint32_t i = 0; i < l->count; i++) {
        k = use_of(t[i].id);
        leave = (t[i].events & BRIDGE_ON_LEAVE) != 0;
        if (leave & (k != RES_NONE))
            drop_use(k);
        if (!leave & (k == RES_NONE))
            take_use(api, t[i].id, add);
    }
}

BRIDGE_NATIVE(loads)
{
    record_uses(api, LIST_LOADS, false);
    return (0);
}
