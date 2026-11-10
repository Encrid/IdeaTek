/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "resources_inside.h"

void tick_time(const bridge_api_t *api)
{
    const ideatek_frame_t *f = bridge_native_get_single(api, FRAME);

    res_module.now += f ? f->real_delta : 0.0;
}

BRIDGE_NATIVE(reloads)
{
    const bridge_list_t *l = bridge_native_get_list(api,
        res_module.lists[LIST_RELOADS]);
    const bridge_trigger_t *t = (const void *)(api->private_base + l->at);
    uint32_t i;

    for (uint32_t k = 0; k < l->count; k++) {
        i = resource_of(t[k].id);
        if (i != RES_NONE)
            res_module.resources[i].reload_asked = true;
    }
    return (0);
}

static void restart(res_resource_t *r)
{
    r->view.state = IDEATEK_LOAD_STATE_LOADING;
    r->view.error = IDEATEK_RESOURCE_ERROR_NONE;
    r->step = STEP_NEW;
    r->loader = RES_NONE;
    r->reloading = true;
    r->reload_asked = false;
    r->reload_set = false;
    r->fallback_used = false;
    r->reload_at = 0;
}

void begin_reload(const bridge_api_t *api, uint32_t i)
{
    res_resource_t *r = &res_module.resources[i];

    if (!r->path_len || r->parent != RES_NONE)
        return;
    if (r->step != STEP_DONE) {
        r->reload_asked = true;
        return;
    }
    api->detach(api, r->entity, RELOAD);
    restart(r);
    for (uint32_t j = 0; j < res_module.s.max_resources; j++)
        res_module.resources[j].seen &= res_module.resources[j].parent != i;
    touch(i);
}

void start_reloads(const bridge_api_t *api)
{
    res_resource_t *r;
    res_text_t path;
    res_text_t name;
    bool due;

    for (uint32_t i = 0; i < res_module.s.max_resources; i++) {
        r = &res_module.resources[i];
        if (!r->entity)
            continue;
        path = text_of(i, 0);
        due = r->reload_asked | (r->reload_set & (res_module.now
            >= r->reload_at)) | (res_module.variables_changed
            & path_variable(&path, &name));
        if (due)
            begin_reload(api, i);
    }
    res_module.variables_changed = false;
}
