/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "space_inside.h"

static void step_all(const bridge_api_t *api)
{
    const bridge_query_t *q = bridge_native_get_query(api, space_module.step);
    uint64_t tick = get_tick(api);
    const uint32_t *span;
    const uint64_t *ids;

    if (tick == space_module.tick)
        return;
    for (uint32_t k = 0; k < q->spans; k++) {
        span = bridge_native_get_span(api, q, k, 1);
        ids = bridge_native_get_block(api, span);
        for (uint32_t r = 0; r < bridge_native_get_rows(api, span); r++)
            remember(api, ids[r], space_module.tick);
    }
    space_module.tick = tick;
}

BRIDGE_NATIVE(step)
{
    return (run_started(api, step_all));
}
