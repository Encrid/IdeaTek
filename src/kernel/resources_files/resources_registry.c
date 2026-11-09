/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "resources_inside.h"

uint32_t find_source(const res_text_t *name)
{
    const res_source_t *s;

    for (uint32_t i = 0; i < res_module.s.max_sources; i++) {
        s = &res_module.sources[i];
        if (s->used && s->name_len == name->len
            && !memcmp(s->name, name->at, name->len))
            return (i);
    }
    return (RES_NONE);
}

uint32_t find_variable(const bridge_api_t *api, const res_text_t *name)
{
    const ideatek_resource_variable_t *v;
    res_text_t own = { "", 0 };
    uint32_t at;

    for (uint32_t i = 0; i < res_module.s.max_variables; i++) {
        at = res_module.variables[i].entity ? bridge_native_find_value(api,
            res_module.variables[i].entity, VARIABLE) : 0;
        v = (const void *)(api->shared_base + at);
        own = at ? res_text(api, v->name) : own;
        if (at && same_text(&own, name))
            return (i);
    }
    return (RES_NONE);
}

uint32_t slot_with(const uint64_t *entities, uint32_t count, uint32_t stride,
    uint64_t entity)
{
    const uint8_t *base = (const uint8_t *)entities;

    for (uint32_t k = 0; k < count; k++)
        if (*(const uint64_t *)(base + (size_t)k * stride) == entity)
            return (k);
    return (RES_NONE);
}

static void add_loader(const bridge_api_t *api, uint64_t id)
{
    uint32_t at = bridge_native_find_value(api, id, LOADER);
    const ideatek_resource_loader_t *c = (const void *)(api->shared_base + at);
    uint32_t k = slot_with(&res_module.loaders[0].entity,
        res_module.s.max_loaders, sizeof(res_loader_t), 0);

    if (!at)
        return;
    if (k == RES_NONE) {
        bridge_native_say(api, BRIDGE_LOG_WARN, "no room for the loader %.*s",
            (int)c->name.len, (const char *)api->shared_base + c->name.at);
        return;
    }
    res_module.loaders[k].entity = id;
    res_module.loaders[k].ranged = c->ranged;
}

BRIDGE_NATIVE(loaders)
{
    const bridge_list_t *l = bridge_native_get_list(api,
        res_module.lists[LIST_LOADERS]);
    const bridge_trigger_t *t = (const void *)(api->private_base + l->at);
    uint32_t k;
    bool leave;

    for (uint32_t i = 0; i < l->count; i++) {
        k = slot_with(&res_module.loaders[0].entity, res_module.s.max_loaders,
            sizeof(res_loader_t), t[i].id);
        leave = (t[i].events & BRIDGE_ON_LEAVE) != 0;
        if (leave && k != RES_NONE)
            res_module.loaders[k].entity = 0;
        else if (!leave && k == RES_NONE)
            add_loader(api, t[i].id);
    }
    return (0);
}
