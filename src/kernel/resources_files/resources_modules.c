/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "resources_inside.h"

static const char *const SOURCE_LINES[3] = {
    "the source name %.*s is refused", "2 sources have the name %.*s",
    "no room for the source %.*s" };
static const char *const VARIABLE_LINES[2] = {
    "2 variables have the name %.*s", "no room for the variable %.*s" };

static uint32_t source_fault(const res_text_t *name, const res_source_t *s)
{
    uint32_t fault = !source_name_is_allowed(name) ? 1 : 0;

    fault = !fault && find_source(name) != RES_NONE ? 2 : fault;
    return (!fault && !s ? 3 : fault);
}

static void add_source(const bridge_api_t *api, uint64_t id)
{
    uint32_t at = bridge_native_find_value(api, id, SOURCE);
    const ideatek_resource_source_t *c = (const void *)(api->shared_base + at);
    res_text_t name = { "", 0 };
    res_source_t *s = free_source();
    uint32_t fault;

    name = res_text(api, c->name);
    fault = source_fault(&name, s);
    if (fault) {
        bridge_native_say(api, BRIDGE_LOG_WARN, SOURCE_LINES[fault - 1],
            (int)name.len, name.at);
        return;
    }
    s->entity = id;
    s->name_len = name.len;
    s->used = true;
    memcpy(s->name, name.at, name.len);
}

BRIDGE_NATIVE(sources)
{
    const bridge_list_t *l = bridge_native_get_list(api,
        res_module.lists[LIST_SOURCES]);
    const bridge_trigger_t *t = (const void *)(api->private_base + l->at);
    uint32_t k;
    bool leave;

    for (uint32_t i = 0; i < l->count; i++) {
        k = slot_with(&res_module.sources[0].entity, res_module.s.max_sources,
            sizeof(res_source_t), t[i].id);
        leave = (t[i].events & BRIDGE_ON_LEAVE) != 0;
        if (!leave & (k == RES_NONE))
            add_source(api, t[i].id);
        if (leave & (k != RES_NONE))
            memset(&res_module.sources[k], 0, sizeof(res_source_t));
    }
    return (0);
}

static void add_variable(const bridge_api_t *api, uint64_t id)
{
    uint32_t at = bridge_native_find_value(api, id, VARIABLE);
    const ideatek_resource_variable_t *c = (const void *)(api->shared_base
        + at);
    res_text_t name = { "", 0 };
    uint32_t k = slot_with(&res_module.variables[0].entity,
        res_module.s.max_variables, sizeof(res_variable_t), 0);
    uint32_t fault;

    name = res_text(api, c->name);
    fault = find_variable(api, &name) != RES_NONE ? 1 : 0;
    fault = !fault && k == RES_NONE ? 2 : fault;
    if (fault) {
        bridge_native_say(api, BRIDGE_LOG_WARN, VARIABLE_LINES[fault - 1],
            (int)name.len, name.at);
        return;
    }
    res_module.variables[k].entity = id;
}

BRIDGE_NATIVE(variables)
{
    const bridge_list_t *l = bridge_native_get_list(api,
        res_module.lists[LIST_VARIABLES]);
    const bridge_trigger_t *t = (const void *)(api->private_base + l->at);
    uint32_t k;
    bool leave;

    res_module.variables_changed |= l->count > 0;
    for (uint32_t i = 0; i < l->count; i++) {
        k = slot_with(&res_module.variables[0].entity,
            res_module.s.max_variables, sizeof(res_variable_t), t[i].id);
        leave = (t[i].events & BRIDGE_ON_LEAVE) != 0;
        if (leave & (k != RES_NONE))
            res_module.variables[k].entity = 0;
        if (!leave & (k == RES_NONE))
            add_variable(api, t[i].id);
    }
    return (0);
}
