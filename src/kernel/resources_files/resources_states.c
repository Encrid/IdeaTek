/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "resources_inside.h"

static const uint32_t TAG_SLOTS[4] = { READY, FAILED, MODIFIED, RAW };

static void fold_uses(void)
{
    const res_use_t *u;
    const res_resource_t *dep;
    uint32_t o;

    for (uint32_t i = 0; i < res_module.s.max_resources; i++) {
        fold_start(&res_module.deps[i]);
        fold_start(&res_module.recs[i]);
    }
    for (uint32_t k = 0; k < res_module.s.max_uses; k++) {
        u = &res_module.uses[k];
        o = u->entity && u->resource != RES_NONE ? resource_of(u->owner)
            : RES_NONE;
        if (o == RES_NONE)
            continue;
        dep = &res_module.resources[u->resource];
        fold_add(&res_module.deps[o], dep->view.state, dep->view.error);
        fold_add(&res_module.recs[o], dep->view.recursive, dep->view.error);
    }
}

static bool fold_views(void)
{
    res_resource_t *r;
    ideatek_resource_t view;
    bool moved = false;
    bool differs;

    for (uint32_t i = 0; i < res_module.s.max_resources; i++) {
        r = &res_module.resources[i];
        view = r->view;
        fold_add(&res_module.recs[i], r->view.state, r->view.error);
        view.dependencies = fold_state(&res_module.deps[i]);
        view.recursive = fold_state(&res_module.recs[i]);
        view.error = r->view.error ? r->view.error : res_module.recs[i].error;
        differs = r->entity && (view.dependencies != r->view.dependencies
            || view.recursive != r->view.recursive
            || view.error != r->view.error);
        moved |= differs;
        r->changed |= differs;
        r->view = view;
    }
    return (moved);
}

void write_view(const bridge_api_t *api, uint32_t i)
{
    res_resource_t *r = &res_module.resources[i];
    bool ready = r->view.recursive == IDEATEK_LOAD_STATE_LOADED;

    r->pending |= (ready & !r->ready) * TAG_READY;
    r->ready = ready;
    BRIDGE_NATIVE_ATTACH(api, r->entity, RESOURCE, r->view);
    for (uint32_t b = 0; b < 3; b++)
        if (r->pending & (1u << b))
            api->attach(api, r->entity, TAG_SLOTS[b], BRIDGE_ZEROED);
    r->tags |= r->pending;
    r->pending = 0;
    r->changed = false;
    r->fresh = false;
}

void fold_states(const bridge_api_t *api)
{
    uint32_t pass = 0;
    bool moved = true;

    if (!res_module.dirty)
        return;
    while (pass < RES_PASSES && moved) {
        fold_uses();
        moved = fold_views();
        pass++;
    }
    for (uint32_t i = 0; i < res_module.s.max_resources; i++)
        if (res_module.resources[i].entity && res_module.resources[i].changed)
            write_view(api, i);
    res_module.dirty = false;
}
