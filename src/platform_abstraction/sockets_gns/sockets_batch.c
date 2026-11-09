/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "sockets_inside.h"

const ideatek_batch_t *batch_of(const bridge_api_t *api, uint64_t entity,
    uint32_t slot)
{
    return ((const void *)(api->shared_base
            + bridge_native_find_value(api, entity, slot)));
}

void attach_batch(const bridge_api_t *api, uint32_t slot, uint32_t start)
{
    sockets_half_t *h = &sockets_module.half;
    ideatek_batch_t batch = { .data = { .at = h->at + h->side * h->size
        + start, .len = h->used - start } };

    if (batch.data.len)
        BRIDGE_NATIVE_ATTACH(api, sockets_module.self, slot, batch);
}

void say_malformed(const bridge_api_t *api, uint64_t entity, uint32_t at)
{
    bridge_native_say(api, BRIDGE_LOG_WARN,
        "the batch of %llu is malformed at %u", (unsigned long long)entity,
        at);
}
