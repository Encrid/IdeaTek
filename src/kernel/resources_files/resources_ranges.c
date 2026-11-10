/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "resources_inside.h"

static void set_range(uint32_t i, const ideatek_resource_read_t *c)
{
    res_resource_t *r = &res_module.resources[i];

    r->range_offset = c->offset;
    r->range_length = c->length;
    r->range_priority = c->priority;
    r->has_range = true;
}

BRIDGE_NATIVE(ranges)
{
    const bridge_list_t *l = bridge_native_get_list(api,
        res_module.lists[LIST_RANGES]);
    const bridge_trigger_t *t = (const void *)(api->private_base + l->at);
    uint32_t at;
    uint32_t i;

    for (uint32_t k = 0; k < l->count; k++) {
        i = resource_of(t[k].id);
        at = bridge_native_find_value(api, t[k].id, READ);
        if (i != RES_NONE)
            set_range(i, (const void *)(api->shared_base + at));
    }
    return (0);
}

void start_ranges(const bridge_api_t *api)
{
    res_resource_t *r;
    bool waits;

    for (uint32_t i = 0; i < res_module.s.max_resources; i++) {
        r = &res_module.resources[i];
        waits = r->entity && r->has_range && r->read == RES_NONE
            && r->loader != RES_NONE;
        if (!waits)
            continue;
        if (!start_read(api, i, READ_RANGE))
            return;
        r->has_range = false;
        api->detach(api, r->entity, READ);
    }
}

void give_range(const bridge_api_t *api, uint32_t i, const res_arrival_t *in)
{
    res_resource_t *r = &res_module.resources[i];
    const res_read_t *q = &res_module.reads[r->read];
    bool past = q->offset + q->length > r->size;
    bool failed = past | (in->error != IDEATEK_RESOURCE_ERROR_NONE);
    ideatek_resource_bytes_t b = { .offset = q->offset, .size = r->size };

    failed |= !failed && !copy_bytes(api, i, &in->data);
    b.data.at = failed ? 0 : r->block_at;
    b.data.len = failed ? 0 : r->block_len;
    b.settings.at = res_module.texts_at + i * res_module.slot_bytes
        + res_module.s.path_bytes;
    b.settings.len = r->settings_len;
    b.error = failed ? IDEATEK_RESOURCE_ERROR_READ_FAILED
        : IDEATEK_RESOURCE_ERROR_NONE;
    b.loader.id = res_module.loaders[r->loader].entity;
    BRIDGE_NATIVE_ATTACH(api, r->entity, BYTES, b);
}
