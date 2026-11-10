/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "space_inside.h"

static uint64_t get_old_parent(const bridge_api_t *api, uint64_t id)
{
    const ideatek_sibling_t *s = SPACE_FIND(api, id, SIBLING,
        ideatek_sibling_t);

    return (s ? s->parent.id : 0);
}

void rewrite(const bridge_api_t *api, uint64_t parent,
    const bridge_list_t *list)
{
    const ideatek_children_t *c = SPACE_FIND(api, parent, CHILDREN,
        ideatek_children_t);
    space_ids_t buf = { .room = (c ? c->count : 0) + list->count };

    buf.ids = take_scratch(api, buf.room * (uint32_t)sizeof(uint64_t));
    if (!buf.ids) {
        bridge_native_say(api, BRIDGE_LOG_WARN, "the list of %llu stays: no "
            "room for its children", (unsigned long long)parent);
        return;
    }
    gather_children(api, parent, list, &buf);
    sort_children(api, &buf);
    write_list(api, parent, &buf);
}

static void move_child(const bridge_api_t *api, uint64_t id,
    space_set_t *parents, const bridge_list_t *list)
{
    uint64_t old = get_old_parent(api, id);
    uint64_t now = get_parent(api, id);
    const ideatek_parent_t *p = SPACE_FIND(api, id, PARENT, ideatek_parent_t);

    if (!now)
        api->detach(api, id, SIBLING);
    if (p && p->keep == IDEATEK_KEEP_WORLD && now != old)
        keep_world(api, id, now);
    if (parents->room) {
        set_put(parents, old);
        set_put(parents, now);
        return;
    }
    rewrite(api, old, list);
    rewrite(api, now, list);
}

BRIDGE_NATIVE(link)
{
    const bridge_list_t *l = bridge_native_get_list(api,
        space_module.lists[LIST_LINK]);
    const bridge_trigger_t *t = (const void *)(api->private_base + l->at);
    space_set_t parents;

    take_set(api, &parents, 2 * l->count);
    for (uint32_t i = 0; i < l->count; i++)
        move_child(api, t[i].id, &parents, l);
    for (uint32_t k = 0; k < 2 * parents.room; k++)
        if (parents.slots[k])
            rewrite(api, parents.slots[k], l);
    return (0);
}
