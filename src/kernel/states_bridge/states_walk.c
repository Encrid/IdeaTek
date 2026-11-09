/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "states_inside.h"

const void *cell_of(const bridge_api_t *api, uint32_t table, uint32_t slot,
    uint32_t row)
{
    const bridge_header_t *h = bridge_native_get_header(api);
    const uint32_t *bases = (const void *)(api->private_base + h->column_base);
    const uint32_t *strides = (const void *)(api->private_base + h->stride);
    uint32_t base = bases[table * h->types + slot];

    if (!base)
        return (nullptr);
    return (api->shared_base + base + row * strides[slot]);
}

void walk_query(const bridge_api_t *api, uint32_t query, states_visit_t visit)
{
    const bridge_query_t *q = bridge_native_get_query(api, query);
    const uint32_t *span;
    const uint64_t *ids;
    uint32_t rows;

    for (uint32_t s = 0; s < q->spans; s++) {
        span = bridge_native_get_span(api, q, s, 1);
        ids = bridge_native_get_block(api, span);
        rows = bridge_native_get_rows(api, span);
        for (uint32_t r = 0; r < rows; r++)
            visit(api, ids[r], span[0], r);
    }
}
