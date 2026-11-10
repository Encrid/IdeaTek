/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "resources_inside.h"

static const res_text_t USER = { "user", 4 };

static uint32_t source_of(const res_path_t *p)
{
    if (!p->source.len)
        return (RES_DEFAULT);
    if (same_text(&p->source, &USER))
        return (RES_USER);
    return (find_source(&p->source));
}

static uint32_t new_resource(const bridge_api_t *api, const res_load_t *load)
{
    uint32_t i = take_resource(&load->keyed, &load->type, &load->settings);
    uint64_t id = i == RES_NONE ? 0 : api->spawn(api);
    uint32_t source = source_of(&load->p);

    if (!id) {
        bridge_native_say(api, BRIDGE_LOG_WARN, "no room for the resource %.*s",
            (int)load->keyed.len, load->keyed.at);
        if (i != RES_NONE)
            drop_resource(i);
        return (RES_NONE);
    }
    bind_resource(i, id);
    res_module.resources[i].source = source;
    if (source == RES_NONE)
        fail_resource(api, i, IDEATEK_RESOURCE_ERROR_NOT_FOUND);
    return (i);
}

static void read_load(const bridge_api_t *api, uint32_t k, res_load_t *load)
{
    const res_use_t *u = &res_module.uses[k];
    uint32_t at = bridge_native_find_value(api, u->entity, LOAD);
    const ideatek_resource_load_t *c = (const void *)(api->shared_base + at);

    load->full = res_text(api, c->path);
    load->type = res_text(api, c->type);
    load->settings = res_text(api, c->settings);
    path_split(&load->full, &load->p);
    load->keyed.at = load->full.at;
    load->keyed.len = (uint32_t)(load->p.path.at + load->p.path.len
        - load->full.at);
}

static void place_load(const bridge_api_t *api, uint32_t k)
{
    res_use_t *u = &res_module.uses[k];
    res_load_t load;
    res_resource_t *r;
    uint32_t i;

    read_load(api, k, &load);
    i = find_resource(&load.keyed, &load.type);
    i = i == RES_NONE ? new_resource(api, &load) : i;
    u->handled = i == RES_NONE;
    if (i == RES_NONE)
        return;
    r = &res_module.resources[i];
    u->resource = i;
    r->count++;
    r->priority = u->priority > r->priority ? u->priority : r->priority;
    res_module.dirty = true;
    if (!load.p.label.len)
        attach_handle(api, k, r->entity);
}

void place_uses(const bridge_api_t *api)
{
    res_use_t *u;

    for (uint32_t k = 0; k < res_module.s.max_uses; k++) {
        u = &res_module.uses[k];
        if (!u->entity || u->handled)
            continue;
        if (u->resource != RES_NONE)
            label_handle(api, k);
        else if (u->add)
            place_add(api, k);
        else
            place_load(api, k);
    }
}
