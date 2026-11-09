/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "states_inside.h"

static void change(const bridge_api_t *api, uint64_t id, uint32_t from,
    uint32_t to)
{
    ideatek_state_t state = { .value = to };
    ideatek_state_change_t changed = { .from = from, .to = to };

    BRIDGE_NATIVE_ATTACH(api, id, STATE, state);
    BRIDGE_NATIVE_ATTACH(api, id, CHANGE, changed);
}

static void take_request(const bridge_api_t *api, uint64_t id, uint32_t table,
    uint32_t row)
{
    const ideatek_state_request_t *r = cell_of(api, table, REQUEST, row);
    const ideatek_state_t *s = cell_of(api, table, STATE, row);

    api->detach(api, id, REQUEST);
    if (!s) {
        bridge_native_say(api, BRIDGE_LOG_WARN, "no state machine for %llu",
            (unsigned long long)id);
        return;
    }
    change(api, id, s->value, r->value);
}

BRIDGE_NATIVE(apply)
{
    walk_query(api, states_module.apply, take_request);
    return (0);
}
