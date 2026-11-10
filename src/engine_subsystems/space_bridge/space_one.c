/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "space_inside.h"

static bool refuse_transform(const bridge_api_t *api, uint64_t id)
{
    bridge_native_say(api, BRIDGE_LOG_WARN, "a transform of %llu is refused",
        (unsigned long long)id);
    api->detach(api, id, WORLD);
    return (false);
}

static uint8_t get_absolute(const bridge_api_t *api, uint64_t id)
{
    const ideatek_parent_t *p = SPACE_FIND(api, id, PARENT, ideatek_parent_t);

    return (p ? p->absolute : 0);
}

static void keep_previous(const bridge_api_t *api, space_item_t *item,
    const ideatek_world_transform_t *world)
{
    const ideatek_previous_world_transform_t *p = SPACE_FIND(api, item->id,
        PREVIOUS, ideatek_previous_world_transform_t);
    bool fresh = !p && space_find(api, item->id, INTERPOLATED);
    ideatek_previous_world_transform_t next = { .world = *world };

    item->teleport |= space_find(api, item->id, TELEPORTED) != nullptr;
    if (item->teleport)
        api->detach(api, item->id, TELEPORTED);
    if (!fresh && !(item->teleport && p))
        return;
    next.tick = p ? p->tick : 0;
    BRIDGE_NATIVE_ATTACH(api, item->id, PREVIOUS, next);
}

bool place_one(const bridge_api_t *api, space_item_t *item,
    ideatek_world_transform_t *world)
{
    const ideatek_transform_t *t = SPACE_FIND(api, item->id, TRANSFORM,
        ideatek_transform_t);
    const ideatek_world_transform_t *old = SPACE_FIND(api, item->id, WORLD,
        ideatek_world_transform_t);
    ideatek_world_transform_t local;

    if (!t)
        return (false);
    if (!is_transform_ok(t))
        return (refuse_transform(api, item->id));
    get_matrix(t, &local);
    *world = local;
    if (item->has_parent)
        compose(&item->parent, &local, get_absolute(api, item->id), world);
    keep_previous(api, item, world);
    if (!old || !is_same_world(old, world))
        BRIDGE_NATIVE_ATTACH(api, item->id, WORLD, *world);
    return (true);
}
