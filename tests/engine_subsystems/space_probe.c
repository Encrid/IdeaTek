/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "bridge_native.h"
#include "ideatek_frame.h"
#include "ideatek_space.h"

[[gnu::section(MANIFEST_SECTION), gnu::used, gnu::retain]]
static const char WHO[] = MANIFEST;

#define STRUCT(api, name, T) BRIDGE_NATIVE_COMPONENT(api, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define ENUM(api, name, T) BRIDGE_NATIVE_DECLARE(api, BRIDGE_ENUM, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define TAG(api, name) BRIDGE_NATIVE_DECLARE_TAG(api, name, BRIDGE_STRIP, \
    BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)

static uint32_t declare_children(const bridge_api_t *api)
{
#if defined(ROLE_REMOVE)
    return (STRUCT(api, "ideatek:Children", ideatek_children_t));
#else
    return (BRIDGE_NATIVE_COMPONENT(api, "ideatek:Children",
        ideatek_children_t, BRIDGE_STRIP, BRIDGE_TABLE,
        BRIDGE_ON_DESTROY_KEEP));
#endif
}

typedef struct {
    uint32_t transform;
    uint32_t parent;
    uint32_t world;
    uint32_t children;
    uint32_t sibling;
    uint32_t previous;
    uint32_t interpolated;
    uint32_t teleported;
} slots_t;

[[maybe_unused]] static void declare_fields(const bridge_api_t *api)
{
    STRUCT(api, "ideatek:ideatek_vec3_t", ideatek_vec3_t);
    STRUCT(api, "ideatek:ideatek_quat_t", ideatek_quat_t);
    STRUCT(api, "ideatek:ideatek_position_t", ideatek_position_t);
    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    ENUM(api, "ideatek:ideatek_keep_t", ideatek_keep_t);
    ENUM(api, "ideatek:ideatek_orphan_t", ideatek_orphan_t);
    STRUCT(api, "ideatek:ideatek_world_transform_t",
        ideatek_world_transform_t);
}

[[maybe_unused]] static uint32_t declare_transform(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:Transform", ideatek_transform_t));
}

[[maybe_unused]] static slots_t declare_all(const bridge_api_t *api)
{
    slots_t slots;

    declare_fields(api);
    slots.transform = declare_transform(api);
    slots.parent = STRUCT(api, "ideatek:Parent", ideatek_parent_t);
    slots.world = STRUCT(api, "ideatek:WorldTransform",
        ideatek_world_transform_t);
    slots.children = declare_children(api);
    slots.sibling = STRUCT(api, "ideatek:Sibling", ideatek_sibling_t);
    slots.previous = STRUCT(api, "ideatek:PreviousWorldTransform",
        ideatek_previous_world_transform_t);
    slots.interpolated = TAG(api, "ideatek:Interpolated");
    slots.teleported = TAG(api, "ideatek:Teleported");
    return (slots);
}

[[maybe_unused]] static void read_all(const bridge_api_t *api)
{
    declare_fields(api);
    STRUCT(api, "ideatek:WorldTransform", ideatek_world_transform_t);
    declare_children(api);
    STRUCT(api, "ideatek:Sibling", ideatek_sibling_t);
    STRUCT(api, "ideatek:PreviousWorldTransform",
        ideatek_previous_world_transform_t);
    TAG(api, "ideatek:Interpolated");
}

[[maybe_unused]] static void move(const bridge_api_t *api)
{
    ideatek_transform_t transform = { .rotation = { .w = 1.0f },
        .scale = { 1.0f, 1.0f, 1.0f } };
    ideatek_parent_t parent = { .keep = IDEATEK_KEEP_WORLD,
        .orphan = IDEATEK_ORPHAN_KEEP_WORLD,
        .absolute = IDEATEK_ABSOLUTE_SCALE };
    uint64_t id = api->spawn(api);

    declare_fields(api);
    BRIDGE_NATIVE_ATTACH(api, id, declare_transform(api), transform);
    BRIDGE_NATIVE_ATTACH(api, id, STRUCT(api, "ideatek:Parent",
        ideatek_parent_t), parent);
    api->attach(api, id, TAG(api, "ideatek:Interpolated"), BRIDGE_ZEROED);
    api->attach(api, id, TAG(api, "ideatek:Teleported"), BRIDGE_ZEROED);
}

[[maybe_unused]] static void place(const bridge_api_t *api)
{
    ideatek_world_transform_t world = { .basis = { 1.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f, 1.0f } };
    ideatek_previous_world_transform_t previous = { .world = world };
    ideatek_children_t children = { .count = 0 };
    ideatek_sibling_t sibling = { .prev = { 0 } };
    ideatek_transform_t transform = { .rotation = { .w = 1.0f },
        .scale = { 1.0f, 1.0f, 1.0f } };
    ideatek_parent_t parent = { .order = 0 };
    slots_t slots = declare_all(api);
    uint64_t id = api->spawn(api);

    BRIDGE_NATIVE_COMPONENT(api, "ideatek:Tick", ideatek_tick_t, BRIDGE_STRIP,
        BRIDGE_SINGLETON, BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_ATTACH(api, id, slots.transform, transform);
    BRIDGE_NATIVE_ATTACH(api, id, slots.parent, parent);
    BRIDGE_NATIVE_ATTACH(api, id, slots.world, world);
    BRIDGE_NATIVE_ATTACH(api, id, slots.children, children);
    BRIDGE_NATIVE_ATTACH(api, id, slots.sibling, sibling);
    BRIDGE_NATIVE_ATTACH(api, id, slots.previous, previous);
    api->attach(api, id, slots.teleported, BRIDGE_ZEROED);
}

BRIDGE_NATIVE(init)
{
#if defined(ROLE_LATE)
    declare_transform(api);
    declare_fields(api);
#elif defined(ROLE_MOVER)
    move(api);
#elif defined(ROLE_SPACE)
    place(api);
#elif defined(ROLE_READER)
    read_all(api);
#else
    declare_all(api);
#endif
    return (0);
}

BRIDGE_NATIVE(shutdown)
{
    return (0);
}

BRIDGE_NATIVE(reload)
{
    return (1);
}
