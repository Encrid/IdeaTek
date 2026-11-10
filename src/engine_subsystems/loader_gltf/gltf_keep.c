/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "gltf_inside.h"

static bool store(const bridge_api_t *api, gltf_cache_t *c,
    uint64_t resource, const ideatek_raw_bytes_t *raw)
{
    uint32_t key = 0;
    uint32_t start = 0;

    if (!take_block(raw->data.len, &key, &start))
        return (false);
    memcpy(api->shared_base + gltf_module.arena_at + start,
        api->shared_base + raw->data.at, raw->data.len);
    free_key(&c->key);
    *c = (gltf_cache_t){ .resource = resource, .key = key,
        .at = gltf_module.arena_at + start, .size = raw->data.len };
    return (true);
}

void keep_bytes(const bridge_api_t *api, uint64_t resource)
{
    uint32_t at = bridge_native_find_value(api, resource, RAW);
    const ideatek_raw_bytes_t *raw = (const void *)(api->shared_base + at);
    gltf_cache_t *c;

    if (!at || !is_used(resource))
        return;
    c = find_cache(resource);
    c = c ? c : find_cache(0);
    if (c && store(api, c, resource, raw))
        wake_users(api, resource, nullptr);
    else
        wake_users(api, resource, ": no room for the bytes of a buffer");
}
