/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "space_inside.h"

uint64_t get_next_child(const bridge_api_t *api, uint64_t child)
{
    const ideatek_sibling_t *s = SPACE_FIND(api, child, SIBLING,
        ideatek_sibling_t);

    return (s ? s->next.id : 0);
}

uint32_t get_ancestor_state(const bridge_api_t *api, uint64_t id,
    const space_set_t *set)
{
    uint64_t mark = id;
    uint64_t walker = get_parent(api, id);
    uint32_t power = 1;
    uint32_t steps = 1;
    bool changed = false;

    while (walker) {
        if (walker == mark)
            return (ANCESTOR_CYCLE);
        changed |= set_has(set, walker);
        if (steps == power) {
            mark = walker;
            power *= 2;
            steps = 0;
        }
        walker = get_parent(api, walker);
        steps++;
    }
    return (changed ? ANCESTOR_CHANGED : ANCESTOR_ROOT);
}
