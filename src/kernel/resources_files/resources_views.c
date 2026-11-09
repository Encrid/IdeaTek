/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "resources_inside.h"

static void fill_view(const bridge_api_t *api, res_loader_view_t *v,
    const ideatek_resource_loader_t *c, uint32_t k)
{
    v->index = k;
    v->name = res_text(api, c->name);
    v->type = res_text(api, c->type);
    v->extensions = res_text(api, c->extensions);
    v->magic = res_text(api, c->magic);
    v->ranged = c->ranged;
}

uint32_t build_views(const bridge_api_t *api)
{
    uint32_t count = 0;
    uint32_t at;

    for (uint32_t k = 0; k < res_module.s.max_loaders; k++) {
        at = res_module.loaders[k].entity ? bridge_native_find_value(api,
            res_module.loaders[k].entity, LOADER) : 0;
        if (!at)
            continue;
        fill_view(api, &res_module.views[count],
            (const void *)(api->shared_base + at), k);
        count++;
    }
    return (count);
}
