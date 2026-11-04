/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "bridge_native.h"
#include "ideatek_physics.h"
#include "ideatek_navigation.h"

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
    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    STRUCT(api, "ideatek:ideatek_vec3_t", ideatek_vec3_t);
    STRUCT(api, "ideatek:ideatek_position_t", ideatek_position_t);
    ENUM(api, "ideatek:ideatek_nav_build_t", ideatek_nav_build_t);
    MASK(api, "ideatek:ideatek_nav_area_flags_t", ideatek_nav_area_flags_t);
    MASK(api, "ideatek:ideatek_nav_link_flags_t", ideatek_nav_link_flags_t);
    ENUM(api, "ideatek:ideatek_nav_query_kind_t", ideatek_nav_query_kind_t);
    MASK(api, "ideatek:ideatek_nav_query_flags_t", ideatek_nav_query_flags_t);
    ENUM(api, "ideatek:ideatek_nav_result_t", ideatek_nav_result_t);
    STRUCT(api, "ideatek:ideatek_nav_point_t", ideatek_nav_point_t);
    MASK(api, "ideatek:ideatek_nav_agent_flags_t", ideatek_nav_agent_flags_t);
    ENUM(api, "ideatek:ideatek_nav_steer_state_t", ideatek_nav_steer_state_t);
}

[[maybe_unused]] static void declare_late(const bridge_api_t *api)
{
    STRUCT(api, "ideatek:NavMesh", ideatek_nav_mesh_t);
    declare_fields(api);
}

[[maybe_unused]] static void declare_all(const bridge_api_t *api)
{
    declare_fields(api);
    STRUCT(api, "ideatek:NavMesh", ideatek_nav_mesh_t);
    STRUCT(api, "ideatek:NavMeshState", ideatek_nav_mesh_state_t);
    STRUCT(api, "ideatek:NavArea", ideatek_nav_area_t);
    STRUCT(api, "ideatek:NavModifier", ideatek_nav_modifier_t);
    STRUCT(api, "ideatek:NavLink", ideatek_nav_link_t);
    STRUCT(api, "ideatek:NavQuery", ideatek_nav_query_t);
    STRUCT(api, "ideatek:NavPath", ideatek_nav_path_t);
    STRUCT(api, "ideatek:NavAgent", ideatek_nav_agent_t);
    STRUCT(api, "ideatek:NavSteer", ideatek_nav_steer_t);
    STRUCT(api, "ideatek:NavLinkReached", ideatek_nav_link_reached_t);
    STRUCT(api, "ideatek:NavObstacle", ideatek_nav_obstacle_t);
}

[[maybe_unused]] static void role_game(const bridge_api_t *api)
{
    uint64_t id = api->spawn(api);

    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    STRUCT(api, "ideatek:ideatek_vec3_t", ideatek_vec3_t);
    STRUCT(api, "ideatek:ideatek_position_t", ideatek_position_t);
    MASK(api, "ideatek:ideatek_nav_area_flags_t", ideatek_nav_area_flags_t);
    MASK(api, "ideatek:ideatek_nav_link_flags_t", ideatek_nav_link_flags_t);
    ENUM(api, "ideatek:ideatek_nav_query_kind_t", ideatek_nav_query_kind_t);
    MASK(api, "ideatek:ideatek_nav_query_flags_t", ideatek_nav_query_flags_t);
    MASK(api, "ideatek:ideatek_nav_agent_flags_t", ideatek_nav_agent_flags_t);
    api->attach(api, id, STRUCT(api, "ideatek:NavMesh", ideatek_nav_mesh_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:NavArea", ideatek_nav_area_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:NavModifier",
        ideatek_nav_modifier_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:NavLink", ideatek_nav_link_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:NavQuery", ideatek_nav_query_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:NavAgent", ideatek_nav_agent_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:NavObstacle",
        ideatek_nav_obstacle_t), BRIDGE_ZEROED);
}

[[maybe_unused]] static void role_module(const bridge_api_t *api)
{
    uint64_t id = api->spawn(api);

    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    STRUCT(api, "ideatek:ideatek_vec3_t", ideatek_vec3_t);
    STRUCT(api, "ideatek:ideatek_position_t", ideatek_position_t);
    ENUM(api, "ideatek:ideatek_shape_kind_t", ideatek_shape_kind_t);
    MASK(api, "ideatek:ideatek_shape_flags_t", ideatek_shape_flags_t);
    ENUM(api, "ideatek:ideatek_body_kind_t", ideatek_body_kind_t);
    ENUM(api, "ideatek:ideatek_mass_mode_t", ideatek_mass_mode_t);
    MASK(api, "ideatek:ideatek_locked_t", ideatek_locked_t);
    MASK(api, "ideatek:ideatek_body_flags_t", ideatek_body_flags_t);
    ENUM(api, "ideatek:ideatek_motion_quality_t", ideatek_motion_quality_t);
    ENUM(api, "ideatek:ideatek_nav_build_t", ideatek_nav_build_t);
    MASK(api, "ideatek:ideatek_nav_area_flags_t", ideatek_nav_area_flags_t);
    MASK(api, "ideatek:ideatek_nav_link_flags_t", ideatek_nav_link_flags_t);
    ENUM(api, "ideatek:ideatek_nav_query_kind_t", ideatek_nav_query_kind_t);
    MASK(api, "ideatek:ideatek_nav_query_flags_t", ideatek_nav_query_flags_t);
    ENUM(api, "ideatek:ideatek_nav_result_t", ideatek_nav_result_t);
    MASK(api, "ideatek:ideatek_nav_agent_flags_t", ideatek_nav_agent_flags_t);
    ENUM(api, "ideatek:ideatek_nav_steer_state_t", ideatek_nav_steer_state_t);
    STRUCT(api, "ideatek:NavMesh", ideatek_nav_mesh_t);
    STRUCT(api, "ideatek:NavArea", ideatek_nav_area_t);
    STRUCT(api, "ideatek:NavModifier", ideatek_nav_modifier_t);
    STRUCT(api, "ideatek:NavLink", ideatek_nav_link_t);
    STRUCT(api, "ideatek:NavQuery", ideatek_nav_query_t);
    STRUCT(api, "ideatek:NavAgent", ideatek_nav_agent_t);
    STRUCT(api, "ideatek:NavObstacle", ideatek_nav_obstacle_t);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:Shape", ideatek_shape_t, BRIDGE_STRIP,
        BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:Body", ideatek_body_t, BRIDGE_STRIP,
        BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:WorldTransform",
        ideatek_world_transform_t, BRIDGE_STRIP, BRIDGE_TABLE,
        BRIDGE_ON_DESTROY_REMOVE);
    api->attach(api, id, STRUCT(api, "ideatek:NavMeshState",
        ideatek_nav_mesh_state_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:NavPath", ideatek_nav_path_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:NavSteer", ideatek_nav_steer_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:NavLinkReached",
        ideatek_nav_link_reached_t), BRIDGE_ZEROED);
}

BRIDGE_NATIVE(init)
{
#if defined(ROLE_LATE)
    declare_late(api);
#elif defined(ROLE_GAME)
    role_game(api);
#elif defined(ROLE_MODULE)
    role_module(api);
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
