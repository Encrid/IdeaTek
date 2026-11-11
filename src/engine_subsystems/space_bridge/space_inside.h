/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef SPACE_INSIDE_H
    #define SPACE_INSIDE_H

    #include "bridge_native.h"
    #include "bridge_native_value.h"
    #include "ideatek_frame.h"
    #include "space_pure.h"

    #define SPACE_TYPE(api, name, T) BRIDGE_NATIVE_DECLARE(api, \
        BRIDGE_STRUCT, name, T, BRIDGE_STRIP, BRIDGE_TABLE, \
        BRIDGE_ON_DESTROY_REMOVE)
    #define SPACE_ENUM(api, name, T) BRIDGE_NATIVE_DECLARE(api, \
        BRIDGE_ENUM, name, T, BRIDGE_STRIP, BRIDGE_TABLE, \
        BRIDGE_ON_DESTROY_REMOVE)
    #define SPACE_TAG(api, name) BRIDGE_NATIVE_DECLARE_TAG(api, name, \
        BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
    #define SPACE_FIND(api, id, slot, T) ((const T *)space_find(api, id, \
        slot))
    #define SPACE_EDIT(api, id, slot, T) ((T *)space_find(api, id, slot))

constexpr uint32_t ENTITY = 0;
constexpr uint32_t TRANSFORM = 7;
constexpr uint32_t PARENT = 8;
constexpr uint32_t WORLD = 9;
constexpr uint32_t CHILDREN = 10;
constexpr uint32_t SIBLING = 11;
constexpr uint32_t PREVIOUS = 12;
constexpr uint32_t INTERPOLATED = 13;
constexpr uint32_t TELEPORTED = 14;
constexpr uint32_t TICK = 15;

constexpr uint32_t LIST_LINK = 0;
constexpr uint32_t LIST_PLACE = 1;
constexpr uint32_t LIST_RELINK = 2;
constexpr uint32_t LIST_PREVIOUS = 3;
constexpr uint32_t LIST_GONE = 4;

constexpr uint32_t ANCESTOR_ROOT = 0;
constexpr uint32_t ANCESTOR_CHANGED = 1;
constexpr uint32_t ANCESTOR_CYCLE = 2;

constexpr uint32_t STACK_ROOM = 1024;

typedef struct {
    uint32_t lists[5];
    uint32_t step;
    uint64_t tick;
} space_module_t;

typedef struct {
    uint64_t id;
    ideatek_world_transform_t parent;
    bool has_parent;
    bool teleport;
} space_item_t;

typedef struct {
    space_item_t *items;
    uint32_t top;
    uint32_t room;
    bool full;
} space_stack_t;

typedef struct {
    uint64_t *ids;
    uint32_t count;
    uint32_t room;
} space_ids_t;

extern space_module_t space_module;

void declare_types(const bridge_api_t *api);
void add_observers(const bridge_api_t *api);

void *space_find(const bridge_api_t *api, uint64_t id, uint32_t slot);
void *take_scratch(const bridge_api_t *api, uint32_t size);
bool take_set(const bridge_api_t *api, space_set_t *set, uint32_t room);
void take_stack(const bridge_api_t *api, space_stack_t *stack);

uint64_t get_parent(const bridge_api_t *api, uint64_t id);
uint64_t get_next_child(const bridge_api_t *api, uint64_t child);
uint32_t get_ancestor_state(const bridge_api_t *api, uint64_t id,
    const space_set_t *set);

bool place_one(const bridge_api_t *api, space_item_t *item,
    ideatek_world_transform_t *world);
void walk(const bridge_api_t *api, space_stack_t *stack, uint64_t id);
void rewrite(const bridge_api_t *api, uint64_t parent,
    const bridge_list_t *list);
void gather_children(const bridge_api_t *api, uint64_t parent,
    const bridge_list_t *list, space_ids_t *buf);
void sort_children(const bridge_api_t *api, space_ids_t *buf);
void write_list(const bridge_api_t *api, uint64_t parent,
    const space_ids_t *buf);
void keep_world(const bridge_api_t *api, uint64_t child, uint64_t parent);
void keep_orphan(const bridge_api_t *api, uint64_t child);

#endif
