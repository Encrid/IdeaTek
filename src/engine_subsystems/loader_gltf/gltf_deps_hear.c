/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "gltf_inside.h"

static void take_handle(const bridge_api_t *api, gltf_dep_t *d)
{
    uint32_t at = bridge_native_find_value(api, d->use, HANDLE);
    const ideatek_resource_handle_t *h = (const void *)(api->shared_base + at);
    uint32_t state_at;
    const ideatek_resource_t *r;

    d->resource = at ? h->resource.id : 0;
    state_at = bridge_native_find_value(api, d->resource, RESOURCE);
    r = (const void *)(api->shared_base + state_at);
    if (d->kind != DEP_BUFFER || find_cache(d->resource) || !state_at)
        return;
    if (r->state == IDEATEK_LOAD_STATE_LOADED)
        api->attach(api, d->resource, RELOAD, BRIDGE_ZEROED);
    if (r->state == IDEATEK_LOAD_STATE_FAILED)
        refuse_file(api, find_file(d->file), ": a buffer did not load");
}

BRIDGE_NATIVE(handles)
{
    uint32_t count;
    const bridge_trigger_t *t = get_triggers(api, LIST_HANDLES, &count);
    gltf_dep_t *d;

    for (uint32_t i = 0; i < count; i++) {
        d = find_dep(t[i].id);
        if (!d)
            continue;
        take_handle(api, d);
        try_build(api, find_file(d->file));
    }
    return (0);
}

BRIDGE_NATIVE(raw)
{
    uint32_t count;
    const bridge_trigger_t *t = get_triggers(api, LIST_RAW, &count);

    for (uint32_t i = 0; i < count; i++)
        keep_bytes(api, t[i].id);
    return (0);
}

static void refuse_users(const bridge_api_t *api, uint64_t resource)
{
    const gltf_dep_t *d;

    for (uint32_t k = 0; k < gltf_module.s.max_parts; k++) {
        d = &gltf_module.deps[k];
        if (d->use && d->resource == resource && d->kind == DEP_BUFFER)
            refuse_file(api, find_file(d->file), ": a buffer did not load");
    }
}

BRIDGE_NATIVE(failed)
{
    uint32_t count;
    const bridge_trigger_t *t = get_triggers(api, LIST_FAILED, &count);

    for (uint32_t i = 0; i < count; i++)
        refuse_users(api, t[i].id);
    return (0);
}
