/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "gltf_inside.h"

bool is_ready(const gltf_file_t *f)
{
    const gltf_dep_t *d;
    const gltf_cache_t *c;

    for (uint32_t k = 0; k < gltf_module.s.max_parts; k++) {
        d = &gltf_module.deps[k];
        if (d->file != f->entity)
            continue;
        c = d->resource ? find_cache(d->resource) : nullptr;
        if (!d->resource || (d->kind == DEP_BUFFER && !c))
            return (false);
    }
    return (true);
}

void refuse_file(const bridge_api_t *api, gltf_file_t *f, const char *rest)
{
    say_path(api, f->entity, rest);
    give_outcome(api, f->entity, false);
    f->waiting = false;
    if (f->now)
        return;
    for (uint32_t k = 0; k < gltf_module.s.max_parts; k++)
        gltf_module.deps[k].kept &= gltf_module.deps[k].file != f->entity;
    drop_deps(api, f);
    forget_file(f);
}

void wait_for(const bridge_api_t *api, gltf_file_t *f)
{
    f->waiting = true;
    try_build(api, f);
}

void try_build(const bridge_api_t *api, gltf_file_t *f)
{
    uint32_t at = bridge_native_find_value(api, f->entity, BYTES);
    const ideatek_resource_bytes_t *b = (const void *)(api->shared_base + at);
    cgltf_data *data = nullptr;

    if (!f->waiting || !is_ready(f))
        return;
    parse_file(api->shared_base + b->data.at, b->data.len, &data);
    f->waiting = false;
    if (build_file(api, f, data))
        give_outcome(api, f->entity, true);
    else
        refuse_file(api, f, ": the file was not built");
}
