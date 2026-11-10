/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "gltf_inside.h"

void drop_deps(const bridge_api_t *api, gltf_file_t *f)
{
    gltf_dep_t *d;

    for (uint32_t k = 0; k < gltf_module.s.max_parts; k++) {
        d = &gltf_module.deps[k];
        if (d->file != f->entity || d->kept)
            continue;
        if (d->use)
            api->destroy(api, d->use);
        *d = (gltf_dep_t){};
    }
}

static uint32_t total_of(const gltf_deps_t *p)
{
    uint32_t total = 0;

    for (uint32_t i = 0; i < p->count; i++)
        total += p->news[i].path_len + p->news[i].settings_len;
    return (total);
}

static void spawn_one(gltf_deps_t *p, const gltf_new_t *n, uint32_t *at)
{
    const bridge_api_t *api = p->api;
    ideatek_resource_load_t l = { .owner = { p->f->entity },
        .type = gltf_module.types[n->dep->kind], .reload_owner = 1 };

    memcpy(api->shared_base + *at, n->path, n->path_len);
    l.path = (bridge_text_t){ *at, n->path_len };
    *at += n->path_len;
    memcpy(api->shared_base + *at, n->settings, n->settings_len);
    l.settings = (bridge_text_t){ *at, n->settings_len };
    *at += n->settings_len;
    n->dep->use = api->spawn(api);
    BRIDGE_NATIVE_ATTACH(api, n->dep->use, LOAD, l);
}

bool spawn_deps(gltf_deps_t *p)
{
    uint32_t total = total_of(p);
    uint32_t start = 0;
    uint32_t at;

    if (!p->count)
        return (true);
    free_key(&p->f->texts);
    if (!take_block(total, &p->f->texts, &start))
        return (false);
    at = gltf_module.arena_at + start;
    for (uint32_t i = 0; i < p->count; i++)
        spawn_one(p, &p->news[i], &at);
    return (true);
}
