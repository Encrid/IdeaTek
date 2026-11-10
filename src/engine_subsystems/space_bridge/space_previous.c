/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "space_inside.h"

static void remember(const bridge_api_t *api, uint64_t id, uint64_t tick)
{
    const ideatek_world_transform_t *w = SPACE_FIND(api, id, WORLD,
        ideatek_world_transform_t);
    ideatek_previous_world_transform_t p = { .tick = tick };

    if (!w)
        return;
    p.world = *w;
    BRIDGE_NATIVE_ATTACH(api, id, PREVIOUS, p);
}

static uint64_t get_tick(const bridge_api_t *api)
{
    const ideatek_tick_t *t = bridge_native_get_single(api, TICK);

    return (t ? t->tick : 0);
}

BRIDGE_NATIVE(previous)
{
    const bridge_list_t *l = bridge_native_get_list(api,
        space_module.lists[LIST_PREVIOUS]);
    const bridge_trigger_t *t = (const void *)(api->private_base + l->at);

    for (uint32_t i = 0; i < l->count; i++) {
        if (t[i].events & BRIDGE_ON_LEAVE)
            api->detach(api, t[i].id, PREVIOUS);
        else
            remember(api, t[i].id, get_tick(api));
    }
    return (0);
}

BRIDGE_NATIVE(step)
{
    const bridge_query_t *q = bridge_native_get_query(api, space_module.step);
    uint64_t tick = get_tick(api);
    const uint32_t *span;
    const uint64_t *ids;

    if (tick == space_module.tick)
        return (0);
    for (uint32_t k = 0; k < q->spans; k++) {
        span = bridge_native_get_span(api, q, k, 1);
        ids = bridge_native_get_block(api, span);
        for (uint32_t r = 0; r < bridge_native_get_rows(api, span); r++)
            remember(api, ids[r], space_module.tick);
    }
    space_module.tick = tick;
    return (0);
}
