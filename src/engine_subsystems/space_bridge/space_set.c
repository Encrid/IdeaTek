/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "space_pure.h"

static uint32_t get_home(const space_set_t *set, uint64_t id)
{
    return ((uint32_t)((id * 0x9E3779B97F4A7C15ull) >> 32) % (2 * set->room));
}

void start_set(space_set_t *set, uint64_t *slots, uint32_t room)
{
    set->slots = slots;
    set->room = room;
    set->count = 0;
    memset(slots, 0, 2 * (size_t)room * sizeof(uint64_t));
}

bool set_put(space_set_t *set, uint64_t id)
{
    uint32_t at;

    if (set->count == set->room)
        return (false);
    at = get_home(set, id);
    while (set->slots[at] && set->slots[at] != id)
        at = (at + 1) % (2 * set->room);
    set->count += set->slots[at] != id;
    set->slots[at] = id;
    return (true);
}

bool set_has(const space_set_t *set, uint64_t id)
{
    uint32_t at;

    if (!set->room || !id)
        return (false);
    at = get_home(set, id);
    while (set->slots[at] && set->slots[at] != id)
        at = (at + 1) % (2 * set->room);
    return (set->slots[at] == id);
}
