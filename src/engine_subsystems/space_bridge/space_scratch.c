/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "space_inside.h"

const void *space_find(const bridge_api_t *api, uint64_t id, uint32_t slot)
{
    uint32_t at = id ? bridge_native_find_value(api, id, slot) : 0;

    return (at ? api->shared_base + at : nullptr);
}

void *take_scratch(const bridge_api_t *api, uint32_t size)
{
    bridge_header_t *h = bridge_native_get_header(api);
    uint32_t used = (h->scratch_used + 7u) & ~7u;
    uint32_t free = (h->scratch_len - used) * (used < h->scratch_len);

    if (size > free)
        return (nullptr);
    h->scratch_used = used + size;
    return (api->private_base + h->scratch + used);
}

bool take_set(const bridge_api_t *api, space_set_t *set, uint32_t room)
{
    uint64_t *slots = take_scratch(api, 2 * room * (uint32_t)sizeof(uint64_t));

    *set = (space_set_t){};
    if (!slots)
        return (false);
    start_set(set, slots, room);
    return (true);
}

uint64_t get_parent(const bridge_api_t *api, uint64_t id)
{
    const ideatek_parent_t *p = SPACE_FIND(api, id, PARENT, ideatek_parent_t);

    return (p ? p->entity.id : 0);
}

void take_stack(const bridge_api_t *api, space_stack_t *stack)
{
    const bridge_header_t *h = bridge_native_get_header(api);
    uint32_t used = (h->scratch_used + 7u) & ~7u;
    uint32_t free = (h->scratch_len - used) * (used < h->scratch_len);
    uint32_t room = free / 2u / (uint32_t)sizeof(space_item_t);

    room = room < STACK_ROOM ? room : STACK_ROOM;
    *stack = (space_stack_t){ .items = take_scratch(api, room
        * (uint32_t)sizeof(space_item_t)), .room = room };
}
