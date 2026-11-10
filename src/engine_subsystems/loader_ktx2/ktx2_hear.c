/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "ktx2_inside.h"

static const bridge_trigger_t *get_triggers(const bridge_api_t *api,
    uint32_t list, uint32_t *count)
{
    const bridge_list_t *l = bridge_native_get_list(api,
        ktx2_module.lists[list]);

    *count = l->count;
    return ((const void *)(api->private_base + l->at));
}

BRIDGE_NATIVE(bytes)
{
    uint32_t count;
    const bridge_trigger_t *t = get_triggers(api, LIST_BYTES, &count);
    uint32_t at;
    const ideatek_resource_bytes_t *b;

    for (uint32_t i = 0; i < count; i++) {
        at = bridge_native_find_value(api, t[i].id, BYTES);
        b = (const void *)(api->shared_base + at);
        if (at && b->loader.id == ktx2_module.loader)
            load_one(api, t[i].id, b);
    }
    return (0);
}

BRIDGE_NATIVE(swap)
{
    uint32_t count;
    const bridge_trigger_t *t = get_triggers(api, LIST_SWAP, &count);
    ktx2_texture_t *k;

    for (uint32_t i = 0; i < count; i++) {
        k = find_texture(t[i].id);
        if (k)
            free_block(&k->old);
    }
    return (0);
}

BRIDGE_NATIVE(gone)
{
    uint32_t count;
    const bridge_trigger_t *t = get_triggers(api, LIST_GONE, &count);
    ktx2_texture_t *k;

    for (uint32_t i = 0; i < count; i++) {
        k = find_texture(t[i].id);
        if (k)
            forget_texture(k);
    }
    return (0);
}
