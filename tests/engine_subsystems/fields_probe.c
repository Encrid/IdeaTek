/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "bridge_native.h"
#include "ideatek_fields.h"

[[gnu::section(MANIFEST_SECTION), gnu::used, gnu::retain]]
static const char WHO[] = MANIFEST;

#define STRUCT(api, name, T) BRIDGE_NATIVE_COMPONENT(api, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define ENUM(api, name, T) BRIDGE_NATIVE_DECLARE(api, BRIDGE_ENUM, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define MASK(api, name, T) BRIDGE_NATIVE_DECLARE(api, BRIDGE_BITMASK, name, \
    T, BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define TAG(api, name) BRIDGE_NATIVE_DECLARE_TAG(api, name, BRIDGE_STRIP, \
    BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)

[[maybe_unused]] static void declare_fields(const bridge_api_t *api)
{
    STRUCT(api, "ideatek:ideatek_vec3_t", ideatek_vec3_t);
    ENUM(api, "ideatek:ideatek_field_shape_t", ideatek_field_shape_t);
    ENUM(api, "ideatek:ideatek_force_kind_t", ideatek_force_kind_t);
}

[[maybe_unused]] static void declare_late(const bridge_api_t *api)
{
    STRUCT(api, "ideatek:Wind", ideatek_wind_t);
    declare_fields(api);
}

[[maybe_unused]] static void declare_all(const bridge_api_t *api)
{
    declare_fields(api);
    STRUCT(api, "ideatek:Wind", ideatek_wind_t);
    STRUCT(api, "ideatek:ForceField", ideatek_force_field_t);
}

[[maybe_unused]] static void role_author(const bridge_api_t *api)
{
    uint64_t id = api->spawn(api);

    STRUCT(api, "ideatek:ideatek_vec3_t", ideatek_vec3_t);
    ENUM(api, "ideatek:ideatek_field_shape_t", ideatek_field_shape_t);
    ENUM(api, "ideatek:ideatek_force_kind_t", ideatek_force_kind_t);
    api->attach(api, id, STRUCT(api, "ideatek:Wind", ideatek_wind_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ForceField",
        ideatek_force_field_t), BRIDGE_ZEROED);
}

[[maybe_unused]] static void role_reader(const bridge_api_t *api)
{
    STRUCT(api, "ideatek:ideatek_vec3_t", ideatek_vec3_t);
    STRUCT(api, "ideatek:ideatek_position_t", ideatek_position_t);
    ENUM(api, "ideatek:ideatek_field_shape_t", ideatek_field_shape_t);
    ENUM(api, "ideatek:ideatek_force_kind_t", ideatek_force_kind_t);
    STRUCT(api, "ideatek:Wind", ideatek_wind_t);
    STRUCT(api, "ideatek:ForceField", ideatek_force_field_t);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:WorldTransform",
        ideatek_world_transform_t, BRIDGE_STRIP, BRIDGE_TABLE,
        BRIDGE_ON_DESTROY_REMOVE);
}

BRIDGE_NATIVE(init)
{
#if defined(ROLE_LATE)
    declare_late(api);
#elif defined(ROLE_AUTHOR)
    role_author(api);
#elif defined(ROLE_READER)
    role_reader(api);
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
