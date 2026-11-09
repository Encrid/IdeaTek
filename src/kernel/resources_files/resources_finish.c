/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "resources_inside.h"

BRIDGE_NATIVE(outcomes)
{
    const bridge_list_t *l = bridge_native_get_list(api,
        res_module.lists[LIST_OUTCOMES]);
    const bridge_trigger_t *t = (const void *)(api->private_base + l->at);
    const ideatek_resource_outcome_t *c;
    uint32_t at;
    uint32_t i;

    for (uint32_t k = 0; k < l->count; k++) {
        i = resource_of(t[k].id);
        at = bridge_native_find_value(api, t[k].id, OUTCOME);
        if (i == RES_NONE || !at)
            continue;
        c = (const void *)(api->shared_base + at);
        res_module.resources[i].outcome = c->error;
        res_module.resources[i].has_outcome = true;
    }
    return (0);
}

static void reload_owners(const bridge_api_t *api, uint32_t i)
{
    const res_use_t *u;
    uint32_t owner;

    for (uint32_t k = 0; k < res_module.s.max_uses; k++) {
        u = &res_module.uses[k];
        owner = u->entity && u->resource == i && u->reload_owner
            ? resource_of(u->owner) : RES_NONE;
        if (owner != RES_NONE)
            begin_reload(api, owner);
    }
}

static void end_load(const bridge_api_t *api, uint32_t i)
{
    res_resource_t *r = &res_module.resources[i];
    bool reloaded = r->reloading;

    if (r->outcome) {
        fail_resource(api, i, IDEATEK_RESOURCE_ERROR_LOADER_FAILED);
        return;
    }
    r->view.state = IDEATEK_LOAD_STATE_LOADED;
    r->step = STEP_DONE;
    r->pending |= reloaded * TAG_MODIFIED;
    r->reloading = false;
    if (reloaded)
        reload_owners(api, i);
    mark_labels(api, i);
    touch(i);
}

void finish_loads(const bridge_api_t *api)
{
    res_resource_t *r;

    for (uint32_t i = 0; i < res_module.s.max_resources; i++) {
        r = &res_module.resources[i];
        if (!r->entity || !r->has_outcome)
            continue;
        r->has_outcome = false;
        api->detach(api, r->entity, BYTES);
        api->detach(api, r->entity, OUTCOME);
        blocks_free(&res_module.blocks, i);
        r->block_len = 0;
        if (r->step == STEP_LOADER)
            end_load(api, i);
    }
}
