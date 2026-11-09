/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "resources_inside.h"

static bool refuse_file(const bridge_api_t *api, uint32_t i,
    const res_arrival_t *in)
{
    res_resource_t *r = &res_module.resources[i];
    bool ranged = r->loader < RES_RAW && res_module.loaders[r->loader].ranged;
    res_text_t path = text_of(i, 0);
    res_text_t name;
    bool retry = in->error == IDEATEK_RESOURCE_ERROR_NOT_FOUND
        && !r->fallback_used && path_variable(&path, &name);
    bool big = (r->size > res_module.s.load_bytes) & !ranged;
    bool refused = retry | (in->error != 0) | big;

    r->fallback_used |= retry;
    r->step = retry ? STEP_READ : r->step;
    if (!retry && refused)
        fail_resource(api, i, in->error ? in->error
            : IDEATEK_RESOURCE_ERROR_TOO_BIG);
    return (refused);
}

static void take_file(const bridge_api_t *api, uint32_t i,
    const res_arrival_t *in)
{
    res_resource_t *r = &res_module.resources[i];

    if (refuse_file(api, i, in))
        return;
    if (!copy_bytes(api, i, &in->data))
        r->step = STEP_READ;
    else if (r->loader == RES_NONE)
        step_resource(api, i);
    else
        give_bytes(api, i);
}

static void dispatch(const bridge_api_t *api, uint32_t i, uint32_t kind,
    const res_arrival_t *in)
{
    bool missing = in->error == IDEATEK_RESOURCE_ERROR_NOT_FOUND;
    ideatek_resource_error_t error = missing ? IDEATEK_RESOURCE_ERROR_NONE
        : in->error;

    switch (kind) {
        case READ_META:
            apply_meta(api, i, in->error ? nullptr : &in->data, error);
            break;
        case READ_RANGE:
            give_range(api, i, in);
            break;
        default:
            take_file(api, i, in);
    }
}

static void take_result(const bridge_api_t *api, uint32_t j)
{
    res_read_t *read = &res_module.reads[j];
    uint32_t i = read->resource;
    res_resource_t *r = &res_module.resources[i];
    res_arrival_t in;

    read_arrival(api, j, &in);
    r->size = read->kind == READ_RANGE ? r->size : in.size;
    dispatch(api, i, read->kind, &in);
    if (in.module)
        api->destroy(api, read->entity);
    read->entity = 0;
    r->read = r->read == j ? RES_NONE : r->read;
}

void take_results(const bridge_api_t *api)
{
    for (uint32_t j = 0; j < res_module.s.max_reads; j++)
        if (res_module.reads[j].entity && res_module.reads[j].done)
            take_result(api, j);
}
