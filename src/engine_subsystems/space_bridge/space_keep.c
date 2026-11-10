/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "space_inside.h"

static void get_root_transform(const ideatek_world_transform_t *world,
    ideatek_transform_t *out)
{
    out->position = world->origin;
    split_basis(world->basis, &out->rotation, &out->scale);
}

void keep_world(const bridge_api_t *api, uint64_t child, uint64_t parent)
{
    const ideatek_world_transform_t *w = SPACE_FIND(api, child, WORLD,
        ideatek_world_transform_t);
    const ideatek_world_transform_t *p = SPACE_FIND(api, parent, WORLD,
        ideatek_world_transform_t);
    ideatek_transform_t t;

    if (!w)
        return;
    if (!p)
        get_root_transform(w, &t);
    else if (!get_local(p, w, &t)) {
        bridge_native_say(api, BRIDGE_LOG_WARN, "%llu keeps its transform: its "
            "new parent has no inverse", (unsigned long long)child);
        return;
    }
    BRIDGE_NATIVE_ATTACH(api, child, TRANSFORM, t);
}

void keep_orphan(const bridge_api_t *api, uint64_t child)
{
    const ideatek_world_transform_t *w = SPACE_FIND(api, child, WORLD,
        ideatek_world_transform_t);
    ideatek_transform_t t;

    api->detach(api, child, PARENT);
    api->detach(api, child, SIBLING);
    if (!w)
        return;
    get_root_transform(w, &t);
    BRIDGE_NATIVE_ATTACH(api, child, TRANSFORM, t);
}
