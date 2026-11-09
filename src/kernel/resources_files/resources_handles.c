/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "resources_inside.h"

static uint32_t find_label(uint32_t parent, const res_text_t *label)
{
    const res_resource_t *r;
    res_text_t path;
    res_path_t p;

    for (uint32_t j = 0; j < res_module.s.max_resources; j++) {
        r = &res_module.resources[j];
        if (!r->entity || r->parent != parent)
            continue;
        path = text_of(j, 0);
        path_split(&path, &p);
        if (same_text(&p.label, label))
            return (j);
    }
    return (RES_NONE);
}

void place_labels(const bridge_api_t *api)
{
    const res_resource_t *r;

    for (uint32_t j = 0; j < res_module.s.max_resources; j++) {
        r = &res_module.resources[j];
        if (r->entity && r->fresh && r->parent != RES_NONE)
            write_view(api, j);
    }
}

static uint32_t label_target(const bridge_api_t *api, uint32_t parent,
    const res_text_t *label)
{
    uint32_t j = find_label(parent, label);

    if (j != RES_NONE)
        return (j);
    j = new_label(api, parent, label, api->spawn(api));
    if (j != RES_NONE)
        fail_resource(api, j, IDEATEK_RESOURCE_ERROR_NO_LABEL);
    return (j);
}

bool label_handle(const bridge_api_t *api, uint32_t use)
{
    const res_use_t *u = &res_module.uses[use];
    const res_resource_t *parent = &res_module.resources[u->resource];
    uint32_t at = bridge_native_find_value(api, u->entity, LOAD);
    const ideatek_resource_load_t *load = (const void *)(api->shared_base
        + at);
    res_text_t full = { "", 0 };
    res_path_t p;
    uint32_t j = u->resource;
    bool failed = parent->view.state == IDEATEK_LOAD_STATE_FAILED;

    if (!at || parent->step != STEP_DONE)
        return (false);
    full = res_text(api, load->path);
    path_split(&full, &p);
    j = failed ? j : label_target(api, u->resource, &p.label);
    j = j == RES_NONE ? u->resource : j;
    attach_handle(api, use, res_module.resources[j].entity);
    return (true);
}

void mark_labels(const bridge_api_t *api, uint32_t parent)
{
    res_resource_t *r;
    const res_resource_t *p = &res_module.resources[parent];
    bool gone;

    for (uint32_t j = 0; j < res_module.s.max_resources; j++) {
        r = &res_module.resources[j];
        if (!r->entity || r->parent != parent)
            continue;
        gone = !r->seen;
        if (gone)
            fail_resource(api, j, IDEATEK_RESOURCE_ERROR_NO_LABEL);
        if (!gone && r->view.state != IDEATEK_LOAD_STATE_FAILED) {
            r->view.state = p->view.state;
            touch(j);
        }
        r->seen = false;
    }
}
