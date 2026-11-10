/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "gltf_inside.h"

static const struct {
    const char *name;
    uint32_t range[2];
} RULES[4] = {
    { "parse_bytes", { 65536, 1073741824 } },
    { "output_bytes", { 65536, 1073741824 } },
    { "max_files", { 1, 65536 } }, { "max_parts", { 1, 1048576 } },
};

static bool check_counts(const bridge_api_t *api, const gltf_settings_t *s)
{
    const uint32_t values[4] = { s->parse_bytes, s->output_bytes,
        s->max_files, s->max_parts };
    bool ok = true;
    bool in;

    for (uint32_t k = 0; k < 4; k++) {
        in = values[k] >= RULES[k].range[0] && values[k] <= RULES[k].range[1];
        if (!in)
            bridge_native_say(api, BRIDGE_LOG_ERR, "%s: %u is outside %u to "
                "%u", RULES[k].name, values[k], RULES[k].range[0],
                RULES[k].range[1]);
        ok &= in;
    }
    return (ok);
}

static void *take(const bridge_api_t *api, uint32_t count, uint32_t size)
{
    uint32_t at = api->alloc(api, count * size);

    memset(api->shared_base + at, 0, (size_t)count * size);
    return (api->shared_base + at);
}

static void take_tables(const bridge_api_t *api, const gltf_settings_t *s)
{
    gltf_module_t *m = &gltf_module;

    m->s = *s;
    m->s.parse_bytes &= ~15u;
    m->files = take(api, s->max_files, sizeof(gltf_file_t));
    m->deps = take(api, s->max_parts, sizeof(gltf_dep_t));
    m->caches = take(api, s->max_files, sizeof(gltf_cache_t));
    m->parts = take(api, s->max_parts, sizeof(gltf_part_t));
    m->blocks.blocks = take(api, 4 * s->max_files,
        sizeof(res_block_t));
    m->blocks.count = 4 * s->max_files;
    m->blocks.size = s->output_bytes;
    m->arena_at = api->alloc(api, s->output_bytes);
    m->parse = api->shared_base + api->alloc(api, s->parse_bytes);
}

bool start_module(const bridge_api_t *api)
{
    const gltf_settings_t *s = bridge_native_get_single(api, SETTINGS);

    if (gltf_module.started)
        return (!gltf_module.broken);
    gltf_module.started = true;
    gltf_module.broken = !check_counts(api, s);
    if (!gltf_module.broken)
        take_tables(api, s);
    return (!gltf_module.broken);
}
