/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "resources_inside.h"

static void add_span(const bridge_api_t *api, const uint32_t *span)
{
    const uint64_t *ids = bridge_native_get_block(api, span);
    uint32_t rows = bridge_native_get_rows(api, span);

    for (uint32_t r = 0; r < rows; r++)
        add_loader(api, ids[r]);
}

BRIDGE_NATIVE(found)
{
    const bridge_query_t *q = bridge_native_get_query(api, res_module.found);

    if (res_module.found_done)
        return (0);
    res_module.found_done = true;
    for (uint32_t k = 0; k < q->spans; k++)
        add_span(api, bridge_native_get_span(api, q, k, 1));
    return (0);
}
