/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "ktx2_inside.h"

static bool check_count(const bridge_api_t *api, const char *name,
    uint32_t value, const uint32_t range[2])
{
    bool ok = (value >= range[0]) & (value <= range[1]);

    if (!ok)
        bridge_native_say(api, BRIDGE_LOG_ERR, "%s: %u is outside %u to %u",
            name, value, range[0], range[1]);
    return (ok);
}

static void *take(const bridge_api_t *api, uint32_t count, uint32_t size)
{
    uint32_t at = api->alloc(api, count * size);

    memset(api->shared_base + at, 0, (size_t)count * size);
    return (api->shared_base + at);
}

bool start_module(const bridge_api_t *api)
{
    const ktx2_settings_t *s = bridge_native_get_single(api, SETTINGS);
    const uint32_t bytes[2] = { 4096, 1073741824 };
    const uint32_t textures[2] = { 1, 65536 };
    bool ok = check_count(api, "texture_bytes", s->texture_bytes, bytes);
    ktx2_module_t *m = &ktx2_module;

    ok &= check_count(api, "max_textures", s->max_textures, textures);
    m->started = true;
    m->broken = !ok;
    if (!ok)
        return (false);
    m->s = *s;
    m->textures = take(api, s->max_textures, sizeof(ktx2_texture_t));
    m->blocks.blocks = take(api, 2 * s->max_textures, sizeof(res_block_t));
    m->blocks.count = 2 * s->max_textures;
    m->blocks.size = s->texture_bytes;
    m->arena_at = api->alloc(api, s->texture_bytes);
    return (true);
}
