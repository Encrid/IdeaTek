/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "gltf_inside.h"

gltf_cache_t *find_cache(uint64_t resource)
{
    for (uint32_t i = 0; i < gltf_module.s.max_files; i++)
        if (gltf_module.caches[i].resource == resource)
            return (&gltf_module.caches[i]);
    return (nullptr);
}

bool is_used(uint64_t resource)
{
    for (uint32_t k = 0; k < gltf_module.s.max_parts; k++)
        if (gltf_module.deps[k].file && gltf_module.deps[k].resource
            == resource)
            return (true);
    return (false);
}

void free_caches(void)
{
    gltf_cache_t *c;

    for (uint32_t i = 0; i < gltf_module.s.max_files; i++) {
        c = &gltf_module.caches[i];
        if (!c->resource || is_used(c->resource))
            continue;
        free_key(&c->key);
        *c = (gltf_cache_t){};
    }
}

void wake_users(const bridge_api_t *api, uint64_t resource, const char *rest)
{
    const gltf_dep_t *d;

    for (uint32_t k = 0; k < gltf_module.s.max_parts; k++) {
        d = &gltf_module.deps[k];
        if (!d->file || d->resource != resource)
            continue;
        if (rest)
            refuse_file(api, find_file(d->file), rest);
        else
            try_build(api, find_file(d->file));
    }
}
