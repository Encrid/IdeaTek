/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "space_inside.h"

static uint32_t fill_set(const bridge_list_t *list,
    const bridge_trigger_t *t, space_set_t *set)
{
    uint32_t missed = 0;

    for (uint32_t i = 0; i < list->count; i++)
        missed += !set_put(set, t[i].id);
    return (missed);
}

static void place_entity(const bridge_api_t *api, space_stack_t *stack,
    uint64_t id, const space_set_t *set)
{
    uint32_t state = get_ancestor_state(api, id, set);

    if (state == ANCESTOR_CYCLE) {
        bridge_native_say(api, BRIDGE_LOG_WARN, "%llu is in a cycle of "
            "parents", (unsigned long long)id);
        api->detach(api, id, WORLD);
        return;
    }
    if (state == ANCESTOR_ROOT)
        walk(api, stack, id);
}

static void place_list(const bridge_api_t *api, uint32_t slot)
{
    const bridge_list_t *l = bridge_native_get_list(api,
        space_module.lists[slot]);
    const bridge_trigger_t *t = (const void *)(api->private_base + l->at);
    space_set_t set;
    space_stack_t stack;
    uint32_t missed;

    take_set(api, &set, l->count);
    take_stack(api, &stack);
    missed = fill_set(l, t, &set);
    if (missed)
        bridge_native_say(api, BRIDGE_LOG_WARN, "%u entities placed without "
            "the test of the ancestors", missed);
    for (uint32_t i = 0; i < l->count; i++)
        place_entity(api, &stack, t[i].id, &set);
}

BRIDGE_NATIVE(place)
{
    place_list(api, LIST_PLACE);
    return (0);
}

BRIDGE_NATIVE(relink)
{
    place_list(api, LIST_RELINK);
    return (0);
}
