/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "space_inside.h"

static bool get_parent_place(const bridge_api_t *api, uint64_t id,
    ideatek_world_transform_t *out)
{
    uint64_t parent = get_parent(api, id);
    const ideatek_world_transform_t *w = SPACE_FIND(api, parent, WORLD,
        ideatek_world_transform_t);

    if (!w || !space_find(api, parent, TRANSFORM))
        return (false);
    *out = *w;
    return (true);
}

static void push_children(const bridge_api_t *api, space_stack_t *stack,
    const space_item_t *from, const ideatek_world_transform_t *world)
{
    const ideatek_children_t *c = SPACE_FIND(api, from->id, CHILDREN,
        ideatek_children_t);
    uint64_t child = c ? c->first.id : 0;

    for (uint32_t k = 0; k < (c ? c->count : 0); k++) {
        stack->full |= stack->top == stack->room;
        if (stack->full)
            return;
        stack->items[stack->top++] = (space_item_t){ .id = child,
            .parent = *world, .has_parent = true, .teleport = from->teleport };
        child = get_next_child(api, child);
    }
}

static void say_full(const bridge_api_t *api, uint64_t id)
{
    bridge_native_say(api, BRIDGE_LOG_WARN, "the walk of %llu stops: no room "
        "for its stack", (unsigned long long)id);
}

void walk(const bridge_api_t *api, space_stack_t *stack, uint64_t id)
{
    space_item_t item = { .id = id };
    ideatek_world_transform_t world;

    item.has_parent = get_parent_place(api, id, &item.parent);
    stack->top = 0;
    stack->full = stack->room == 0;
    if (!stack->full)
        stack->items[stack->top++] = item;
    while (stack->top && !stack->full) {
        item = stack->items[--stack->top];
        if (place_one(api, &item, &world))
            push_children(api, stack, &item, &world);
    }
    if (stack->full)
        say_full(api, id);
}
