/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "space_inside.h"

static constexpr uint32_t PLACES[] = { WORLD, PREVIOUS, TELEPORTED };
static constexpr uint32_t REMEMBERS[] = { PREVIOUS };
static constexpr uint32_t LINKS[] = { SIBLING, CHILDREN, TRANSFORM };
static constexpr uint32_t GONE[] = { WORLD, PREVIOUS, CHILDREN, PARENT,
    SIBLING, TRANSFORM };
static constexpr uint32_t EVENTS[] = { BRIDGE_ON_ENTER | BRIDGE_ON_ATTACH
    | BRIDGE_ON_LEAVE, BRIDGE_ON_ENTER | BRIDGE_ON_ATTACH, BRIDGE_ON_ENTER
    | BRIDGE_ON_ATTACH | BRIDGE_ON_LEAVE, BRIDGE_ON_ENTER | BRIDGE_ON_LEAVE,
    BRIDGE_ON_LEAVE };
static constexpr uint32_t TERMS[] = { PARENT, TRANSFORM, SIBLING,
    INTERPOLATED, TRANSFORM };
static const char *const NAMES[] = { "link", "place", "relink", "previous",
    "gone" };

static uint32_t add_observer(const bridge_api_t *api, uint32_t k,
    const uint32_t *changes, uint32_t count)
{
    bridge_term_t term[1] = {{ TERMS[k], BRIDGE_READ, BRIDGE_AND, 0 }};
    const uint32_t *before = k ? &space_module.lists[k - 1] : nullptr;
    bridge_observer_statement_t s = {
        .name = bridge_native_put(api, NAMES[k], (uint32_t)strlen(NAMES[k])),
        .name_len = (uint32_t)strlen(NAMES[k]), .events = EVENTS[k],
        .terms = bridge_native_put(api, term, sizeof(term)), .term_count = 1,
        .changes = bridge_native_put_slots(api, changes, count),
        .change_count = count,
        .before = bridge_native_put_slots(api, before, k > 0),
        .before_count = k > 0 };

    return (api->observe(api, bridge_native_put(api, &s, sizeof(s))));
}

static void add_step(const bridge_api_t *api)
{
    const uint32_t changes[] = { PREVIOUS };
    bridge_term_t terms[3] = {{ INTERPOLATED, BRIDGE_READ, BRIDGE_AND, 0 },
        { WORLD, BRIDGE_READ, BRIDGE_AND, 0 },
        { PREVIOUS, BRIDGE_READ, BRIDGE_AND, 0 }};

    space_module.step = BRIDGE_NATIVE_ADD_SYSTEM(api, "step", "fixed", terms,
        3, changes, 1, nullptr, 0);
}

void add_observers(const bridge_api_t *api)
{
    space_module.lists[LIST_LINK] = add_observer(api, LIST_LINK, LINKS, 3);
    space_module.lists[LIST_PLACE] = add_observer(api, LIST_PLACE, PLACES, 3);
    space_module.lists[LIST_RELINK] = add_observer(api, LIST_RELINK, PLACES,
        3);
    space_module.lists[LIST_PREVIOUS] = add_observer(api, LIST_PREVIOUS,
        REMEMBERS, 1);
    space_module.lists[LIST_GONE] = add_observer(api, LIST_GONE, GONE, 6);
    add_step(api);
}
