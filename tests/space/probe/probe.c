/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "bridge_native.h"
#include "ideatek_space.h"

[[gnu::section(MANIFEST_SECTION), gnu::used, gnu::retain]]
static const char WHO[] = MANIFEST;

#define DECLARE(api, name, T) BRIDGE_NATIVE_COMPONENT(api, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)

[[maybe_unused]] static void declare_fields(const bridge_api_t *api)
{
    DECLARE(api, "ideatek:ideatek_vec3_t", ideatek_vec3_t);
    DECLARE(api, "ideatek:ideatek_quat_t", ideatek_quat_t);
    DECLARE(api, "ideatek:ideatek_position_t", ideatek_position_t);
    DECLARE(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
}

[[maybe_unused]] static uint32_t declare_transform(const bridge_api_t *api)
{
    return (DECLARE(api, "ideatek:Transform", ideatek_transform_t));
}

[[maybe_unused]] static uint32_t declare_parent(const bridge_api_t *api)
{
    return (DECLARE(api, "ideatek:Parent", ideatek_parent_t));
}

[[maybe_unused]] static uint32_t declare_world(const bridge_api_t *api)
{
    return (DECLARE(api, "ideatek:WorldTransform",
        ideatek_world_transform_t));
}

[[maybe_unused]] static void move(const bridge_api_t *api)
{
    ideatek_transform_t transform = { .rotation = { .w = 1.0f },
        .scale = { 1.0f, 1.0f, 1.0f } };
    uint32_t slot = declare_transform(api);

    declare_parent(api);
    BRIDGE_NATIVE_ATTACH(api, api->spawn(api), slot, transform);
}

[[maybe_unused]] static void place(const bridge_api_t *api)
{
    ideatek_world_transform_t world = { .basis = { 1.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f, 1.0f } };

    declare_transform(api);
    declare_parent(api);
    BRIDGE_NATIVE_ATTACH(api, api->spawn(api), declare_world(api), world);
}

BRIDGE_NATIVE(init)
{
#if defined(ROLE_LATE)
    declare_transform(api);
    declare_fields(api);
#elif defined(ROLE_MOVER)
    declare_fields(api);
    move(api);
#elif defined(ROLE_SPACE)
    declare_fields(api);
    place(api);
#elif defined(ROLE_READER)
    DECLARE(api, "ideatek:ideatek_position_t", ideatek_position_t);
    declare_world(api);
#else
    declare_fields(api);
    declare_transform(api);
    declare_parent(api);
    declare_world(api);
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
