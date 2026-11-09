/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "resources_inside.h"

static void *take(const bridge_api_t *api, uint32_t count, uint32_t size)
{
    uint32_t at = api->alloc(api, count * size);

    memset(api->shared_base + at, 0, (size_t)count * size);
    return (api->shared_base + at);
}

static void take_maps(const bridge_api_t *api)
{
    const res_settings_t *s = &res_module.s;
    res_module_t *m = &res_module;

    m->by_key.slots = take(api, 2 * s->max_resources, sizeof(res_slot_t));
    m->by_key.count = 2 * s->max_resources;
    m->by_entity.slots = take(api, 2 * s->max_resources, sizeof(res_slot_t));
    m->by_entity.count = 2 * s->max_resources;
    m->use_by_entity.slots = take(api, 2 * s->max_uses, sizeof(res_slot_t));
    m->use_by_entity.count = 2 * s->max_uses;
    m->blocks.blocks = take(api, s->max_resources, sizeof(res_block_t));
    m->blocks.count = s->max_resources;
    m->blocks.size = s->load_bytes;
    m->arena_at = api->alloc(api, s->load_bytes);
}

static void take_tables(const bridge_api_t *api)
{
    const res_settings_t *s = &res_module.s;
    res_module_t *m = &res_module;

    m->resources = take(api, s->max_resources, sizeof(res_resource_t));
    m->uses = take(api, s->max_uses, sizeof(res_use_t));
    m->loaders = take(api, s->max_loaders, sizeof(res_loader_t));
    m->sources = take(api, s->max_sources, sizeof(res_source_t));
    m->variables = take(api, s->max_variables, sizeof(res_variable_t));
    m->reads = take(api, s->max_reads, sizeof(res_read_t));
    m->views = take(api, s->max_loaders, sizeof(res_loader_view_t));
    m->deps = take(api, s->max_resources, sizeof(res_fold_t));
    m->recs = take(api, s->max_resources, sizeof(res_fold_t));
    m->slot_bytes = s->path_bytes + s->settings_bytes;
    m->texts_at = api->alloc(api, s->max_resources * m->slot_bytes);
    m->texts = api->shared_base + m->texts_at;
    m->paths_at = api->alloc(api, s->max_reads * RES_PATH);
    take_maps(api);
}

bool start_resources(const bridge_api_t *api)
{
    res_module.started = true;
    if (!read_settings(api)) {
        res_module.broken = true;
        return (false);
    }
    take_tables(api);
    read_sources(api);
    if (!res_module.s.hot_reload)
        return (true);
    start_watch(api, RES_DEFAULT);
    for (uint32_t i = 0; i < res_module.s.max_sources; i++)
        if (res_module.sources[i].used && !res_module.sources[i].entity)
            start_watch(api, i);
    return (true);
}
