/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "resources_inside.h"

static const char *const STATUSES[6] = { "ok", "not found", "too big",
    "denied", "failed", "unsupported" };

uint32_t read_of(uint64_t entity)
{
    for (uint32_t j = 0; j < res_module.s.max_reads; j++)
        if (res_module.reads[j].entity == entity)
            return (j);
    return (RES_NONE);
}

static void watch_result(const bridge_api_t *api, uint64_t entity,
    ideatek_file_status_t status)
{
    const ideatek_file_request_t *q;
    uint32_t at = bridge_native_find_value(api, entity, FILE_REQUEST);
    bool watch = entity == res_module.default_watch;

    for (uint32_t k = 0; k < res_module.s.max_sources; k++)
        watch |= res_module.sources[k].watch == entity;
    if (!watch || status == IDEATEK_FILE_STATUS_OK)
        return;
    q = (const void *)(api->shared_base + at);
    bridge_native_say(api, BRIDGE_LOG_WARN, "no watch of %.*s: %s",
        (int)q->path.len, (const char *)api->shared_base + q->path.at,
        STATUSES[status % 6]);
}

BRIDGE_NATIVE(results)
{
    const bridge_list_t *l = bridge_native_get_list(api,
        res_module.lists[LIST_RESULTS]);
    const bridge_trigger_t *t = (const void *)(api->private_base + l->at);
    const ideatek_file_result_t *c;
    uint32_t at;
    uint32_t j;

    for (uint32_t i = 0; i < l->count; i++) {
        at = bridge_native_find_value(api, t[i].id, FILE_RESULT);
        c = (const void *)(api->shared_base + at);
        j = read_of(t[i].id);
        if (j == RES_NONE) {
            watch_result(api, t[i].id, c->status);
            continue;
        }
        res_module.reads[j].status = c->status;
        res_module.reads[j].done = true;
    }
    return (0);
}

BRIDGE_NATIVE(answers)
{
    const bridge_list_t *l = bridge_native_get_list(api,
        res_module.lists[LIST_ANSWERS]);
    const bridge_trigger_t *t = (const void *)(api->private_base + l->at);
    const ideatek_resource_source_result_t *c;
    uint32_t at;
    uint32_t j;

    for (uint32_t i = 0; i < l->count; i++) {
        at = bridge_native_find_value(api, t[i].id, SOURCE_RESULT);
        c = (const void *)(api->shared_base + at);
        j = read_of(t[i].id);
        if (j == RES_NONE)
            continue;
        res_module.reads[j].error = c->error;
        res_module.reads[j].status = c->error ? IDEATEK_FILE_STATUS_FAILED
            : IDEATEK_FILE_STATUS_OK;
        res_module.reads[j].done = true;
    }
    return (0);
}
