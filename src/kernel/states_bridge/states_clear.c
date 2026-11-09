/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "states_inside.h"

static uint32_t read_link(const bridge_api_t *api, uint64_t machine,
    uint32_t value, chain_link_t *link)
{
    uint32_t state = bridge_native_find_value(api, machine, STATE);
    uint32_t request = bridge_native_find_value(api, machine, REQUEST);
    uint32_t scoped = bridge_native_find_value(api, machine, SCOPED);
    const ideatek_state_t *s = (const void *)(api->shared_base + state);

    link->has_state = state != 0;
    link->has_request = request != 0;
    link->scoped = scoped != 0;
    link->state = link->has_state * s->value;
    link->value = value;
    return (scoped);
}

static uint32_t read_chain(const bridge_api_t *api,
    const ideatek_state_scoped_t *tie, chain_link_t *links)
{
    uint64_t machine = tie->machine.id;
    uint32_t value = tie->value;
    uint32_t count = 0;
    uint32_t at;

    while (count < STATES_DEPTH) {
        at = read_link(api, machine, value, &links[count]);
        count++;
        if (link_leaves(&links[count - 1]) || !links[count - 1].scoped)
            break;
        tie = (const void *)(api->shared_base + at);
        machine = tie->machine.id;
        value = tie->value;
    }
    return (count);
}

static void judge(const bridge_api_t *api, uint64_t id, uint32_t table,
    uint32_t row)
{
    const ideatek_state_scoped_t *tie = cell_of(api, table, SCOPED, row);
    chain_link_t links[STATES_DEPTH];
    uint32_t count = read_chain(api, tie, links);
    chain_verdict_t verdict = chain_verdict(links, count);

    if (verdict == CHAIN_LEAVES)
        api->destroy(api, id);
    if (verdict == CHAIN_DEEP)
        bridge_native_say(api, BRIDGE_LOG_WARN,
            "the scoped chain of %llu is deeper than %u",
            (unsigned long long)id, STATES_DEPTH);
}

BRIDGE_NATIVE(clear)
{
    walk_query(api, states_module.clear, judge);
    return (0);
}
