/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "space_inside.h"

static bool is_husk(const bridge_api_t *api, uint64_t id)
{
    return (space_find(api, id, CHILDREN) && !space_find(api, id, WORLD));
}

static void release_child(const bridge_api_t *api, uint64_t husk,
    uint64_t child)
{
    const ideatek_parent_t *p = SPACE_FIND(api, child, PARENT,
        ideatek_parent_t);

    if (!p || p->entity.id != husk)
        return;
    if (p->orphan == IDEATEK_ORPHAN_KEEP_WORLD)
        keep_orphan(api, child);
    else
        api->destroy(api, child);
}

static void visit_children(const bridge_api_t *api, uint64_t id,
    space_stack_t *stack)
{
    const ideatek_children_t *c = SPACE_FIND(api, id, CHILDREN,
        ideatek_children_t);
    uint64_t child = c ? c->first.id : 0;
    uint64_t next;
    bool husk = is_husk(api, id);

    for (uint32_t k = 0; k < (c ? c->count : 0); k++) {
        next = get_next_child(api, child);
        if (husk)
            release_child(api, id, child);
        else
            walk(api, stack, child);
        child = next;
    }
    if (husk)
        api->detach(api, id, CHILDREN);
}

BRIDGE_NATIVE(gone)
{
    const bridge_list_t *l = bridge_native_get_list(api,
        space_module.lists[LIST_GONE]);
    const bridge_trigger_t *t = (const void *)(api->private_base + l->at);
    space_stack_t stack;

    take_stack(api, &stack);
    for (uint32_t i = 0; i < l->count; i++) {
        api->detach(api, t[i].id, WORLD);
        api->detach(api, t[i].id, PREVIOUS);
        visit_children(api, t[i].id, &stack);
    }
    return (0);
}
