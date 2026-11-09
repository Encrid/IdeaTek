/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "states_inside.h"

void declare_types(const bridge_api_t *api)
{
    STATES_TYPE(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    STATES_TYPE(api, "ideatek:State", ideatek_state_t);
    STATES_TYPE(api, "ideatek:StateRequest", ideatek_state_request_t);
    STATES_TYPE(api, "ideatek:StateChange", ideatek_state_change_t);
    STATES_TYPE(api, "ideatek:StateScoped", ideatek_state_scoped_t);
}

void add_runs(const bridge_api_t *api)
{
    const uint32_t forgets[] = { CHANGE };
    const uint32_t applies[] = { STATE, REQUEST, CHANGE };
    bridge_term_t change[1] = {{ CHANGE, BRIDGE_WRITE, BRIDGE_AND, 0 }};
    bridge_term_t scoped[3] = {{ SCOPED, BRIDGE_READ, BRIDGE_AND, 0 },
        { STATE, BRIDGE_READ, BRIDGE_OPTIONAL, 0 },
        { REQUEST, BRIDGE_READ, BRIDGE_OPTIONAL, 0 }};
    bridge_term_t request[2] = {{ REQUEST, BRIDGE_READ, BRIDGE_AND, 0 },
        { STATE, BRIDGE_WRITE, BRIDGE_OPTIONAL, 0 }};
    uint32_t before[2];

    states_module.forget = BRIDGE_NATIVE_ADD_SYSTEM(api, "forget", "first",
        change, 1, forgets, 1, nullptr, 0);
    before[0] = states_module.forget;
    states_module.clear = BRIDGE_NATIVE_ADD_SYSTEM(api, "clear", "first",
        scoped, 3, nullptr, 0, before, 1);
    before[1] = states_module.clear;
    states_module.apply = BRIDGE_NATIVE_ADD_SYSTEM(api, "apply", "first",
        request, 2, applies, 3, before, 2);
}
