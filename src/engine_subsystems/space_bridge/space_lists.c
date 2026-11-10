/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "space_inside.h"

static void add_child(const bridge_api_t *api, uint64_t parent,
    space_ids_t *buf, uint64_t id)
{
    for (uint32_t i = 0; i < buf->count; i++)
        if (buf->ids[i] == id)
            return;
    if (get_parent(api, id) == parent)
        buf->ids[buf->count++] = id;
}

void gather_children(const bridge_api_t *api, uint64_t parent,
    const bridge_list_t *list, space_ids_t *buf)
{
    const ideatek_children_t *c = SPACE_FIND(api, parent, CHILDREN,
        ideatek_children_t);
    const bridge_trigger_t *t = (const void *)(api->private_base + list->at);
    uint64_t child = c ? c->first.id : 0;

    for (uint32_t k = 0; k < (c ? c->count : 0); k++) {
        add_child(api, parent, buf, child);
        child = get_next_child(api, child);
    }
    for (uint32_t i = 0; i < list->count; i++)
        add_child(api, parent, buf, t[i].id);
}

static bool is_before(const bridge_api_t *api, uint64_t a, uint64_t b)
{
    const ideatek_parent_t *pa = SPACE_FIND(api, a, PARENT, ideatek_parent_t);
    const ideatek_parent_t *pb = SPACE_FIND(api, b, PARENT, ideatek_parent_t);

    return (pa->order < pb->order || (pa->order == pb->order && a < b));
}

void sort_children(const bridge_api_t *api, space_ids_t *buf)
{
    uint64_t id;
    uint32_t k;

    for (uint32_t i = 1; i < buf->count; i++) {
        id = buf->ids[i];
        k = i;
        while (k > 0 && is_before(api, id, buf->ids[k - 1])) {
            buf->ids[k] = buf->ids[k - 1];
            k--;
        }
        buf->ids[k] = id;
    }
}

void write_list(const bridge_api_t *api, uint64_t parent,
    const space_ids_t *buf)
{
    ideatek_children_t c = { .count = buf->count };
    ideatek_sibling_t s = { .parent = { parent } };
    uint32_t last = buf->count - 1;

    if (!buf->count) {
        api->detach(api, parent, CHILDREN);
        return;
    }
    c.first.id = buf->ids[0];
    c.last.id = buf->ids[last];
    for (uint32_t i = 0; i < buf->count; i++) {
        s.prev.id = i ? buf->ids[i - 1] : 0;
        s.next.id = i < last ? buf->ids[i + 1] : 0;
        BRIDGE_NATIVE_ATTACH(api, buf->ids[i], SIBLING, s);
    }
    BRIDGE_NATIVE_ATTACH(api, parent, CHILDREN, c);
}
