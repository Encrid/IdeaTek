/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "states_chain.h"

bool link_leaves(const chain_link_t *link)
{
    bool orphan = !link->has_state;
    bool leaving = link->has_request & (link->state == link->value);

    return (orphan | leaving);
}

chain_verdict_t chain_verdict(const chain_link_t *links, uint32_t count)
{
    for (uint32_t i = 0; i < count; i++) {
        if (link_leaves(&links[i]))
            return (CHAIN_LEAVES);
        if (!links[i].scoped)
            return (CHAIN_STAYS);
    }
    return (CHAIN_DEEP);
}
