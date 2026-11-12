/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "space_inside.h"

void remember(const bridge_api_t *api, uint64_t id, uint64_t tick)
{
    const ideatek_world_transform_t *w = SPACE_FIND(api, id, WORLD,
        ideatek_world_transform_t);
    ideatek_previous_world_transform_t *old = SPACE_EDIT(api, id, PREVIOUS,
        ideatek_previous_world_transform_t);
    ideatek_previous_world_transform_t p = { .tick = tick };

    if (!w)
        return;
    p.world = *w;
    if (old)
        *old = p;
    else
        BRIDGE_NATIVE_ATTACH(api, id, PREVIOUS, p);
}

uint64_t get_tick(const bridge_api_t *api)
{
    const ideatek_tick_t *t = bridge_native_get_single(api, TICK);

    return (t ? t->tick : 0);
}

static void previous_all(const bridge_api_t *api)
{
    const bridge_list_t *l = bridge_native_get_list(api,
        space_module.lists[LIST_PREVIOUS]);
    const bridge_trigger_t *t = (const void *)(api->private_base + l->at);

    for (uint32_t i = 0; i < l->count; i++) {
        if (t[i].events & BRIDGE_ON_LEAVE)
            api->detach(api, t[i].id, PREVIOUS);
        else
            remember(api, t[i].id, get_tick(api));
    }
}

BRIDGE_NATIVE(previous)
{
    return (run_started(api, previous_all));
}
