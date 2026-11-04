/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "bridge_native.h"
#include "ideatek_rendering.h"
#include "ideatek_animation.h"
#include "ideatek_input_mapping.h"
#include "ideatek_xr.h"

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
    ENUM(api, "ideatek:ideatek_xr_space_t", ideatek_xr_space_t);
    ENUM(api, "ideatek:ideatek_xr_blend_t", ideatek_xr_blend_t);
    ENUM(api, "ideatek:ideatek_xr_session_state_t", ideatek_xr_session_state_t);
    MASK(api, "ideatek:ideatek_xr_blends_t", ideatek_xr_blends_t);
    ENUM(api, "ideatek:ideatek_xr_tracker_kind_t", ideatek_xr_tracker_kind_t);
    ENUM(api, "ideatek:ideatek_xr_hand_t", ideatek_xr_hand_t);
    ENUM(api, "ideatek:ideatek_xr_pose_t", ideatek_xr_pose_t);
    MASK(api, "ideatek:ideatek_xr_location_t", ideatek_xr_location_t);
}

[[maybe_unused]] static void declare_late(const bridge_api_t *api)
{
    STRUCT(api, "ideatek:XrOrigin", ideatek_xr_origin_t);
    declare_fields(api);
}

[[maybe_unused]] static void declare_all(const bridge_api_t *api)
{
    declare_fields(api);
    STRUCT(api, "ideatek:XrOrigin", ideatek_xr_origin_t);
    STRUCT(api, "ideatek:XrSession", ideatek_xr_session_t);
    STRUCT(api, "ideatek:XrTracker", ideatek_xr_tracker_t);
    STRUCT(api, "ideatek:XrPoseState", ideatek_xr_pose_state_t);
    STRUCT(api, "ideatek:XrBindingPath", ideatek_xr_binding_path_t);
    STRUCT(api, "ideatek:XrControlState", ideatek_xr_control_state_t);
    STRUCT(api, "ideatek:XrHand", ideatek_xr_hand_record_t);
    STRUCT(api, "ideatek:XrJointState", ideatek_xr_joint_state_t);
    STRUCT(api, "ideatek:XrHaptic", ideatek_xr_haptic_t);
    STRUCT(api, "ideatek:XrPlayArea", ideatek_xr_play_area_t);
}

[[maybe_unused]] static void role_game(const bridge_api_t *api)
{
    uint64_t id = api->spawn(api);

    ENUM(api, "ideatek:ideatek_xr_space_t", ideatek_xr_space_t);
    ENUM(api, "ideatek:ideatek_xr_blend_t", ideatek_xr_blend_t);
    api->attach(api, id, STRUCT(api, "ideatek:XrOrigin", ideatek_xr_origin_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:XrBindingPath",
        ideatek_xr_binding_path_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:XrHaptic", ideatek_xr_haptic_t),
        BRIDGE_ZEROED);
}

[[maybe_unused]] static void role_module(const bridge_api_t *api)
{
    uint64_t id = api->spawn(api);

    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    STRUCT(api, "ideatek:ideatek_vec3_t", ideatek_vec3_t);
    STRUCT(api, "ideatek:ideatek_quat_t", ideatek_quat_t);
    STRUCT(api, "ideatek:ideatek_position_t", ideatek_position_t);
    STRUCT(api, "ideatek:ideatek_rect_t", ideatek_rect_t);
    ENUM(api, "ideatek:ideatek_projection_kind_t", ideatek_projection_kind_t);
    ENUM(api, "ideatek:ideatek_input_source_t", ideatek_input_source_t);
    ENUM(api, "ideatek:ideatek_xr_space_t", ideatek_xr_space_t);
    ENUM(api, "ideatek:ideatek_xr_blend_t", ideatek_xr_blend_t);
    ENUM(api, "ideatek:ideatek_xr_session_state_t", ideatek_xr_session_state_t);
    MASK(api, "ideatek:ideatek_xr_blends_t", ideatek_xr_blends_t);
    ENUM(api, "ideatek:ideatek_xr_tracker_kind_t", ideatek_xr_tracker_kind_t);
    ENUM(api, "ideatek:ideatek_xr_hand_t", ideatek_xr_hand_t);
    ENUM(api, "ideatek:ideatek_xr_pose_t", ideatek_xr_pose_t);
    MASK(api, "ideatek:ideatek_xr_location_t", ideatek_xr_location_t);
    STRUCT(api, "ideatek:XrOrigin", ideatek_xr_origin_t);
    STRUCT(api, "ideatek:XrBindingPath", ideatek_xr_binding_path_t);
    STRUCT(api, "ideatek:XrHaptic", ideatek_xr_haptic_t);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:InputBinding",
        ideatek_input_binding_t, BRIDGE_STRIP, BRIDGE_TABLE,
        BRIDGE_ON_DESTROY_REMOVE);
    api->attach(api, id, STRUCT(api, "ideatek:XrSession", ideatek_xr_session_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:XrTracker", ideatek_xr_tracker_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:XrPoseState",
        ideatek_xr_pose_state_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:XrControlState",
        ideatek_xr_control_state_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:XrHand",
        ideatek_xr_hand_record_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:XrJointState",
        ideatek_xr_joint_state_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:XrPlayArea",
        ideatek_xr_play_area_t), BRIDGE_ZEROED);
    api->attach(api, id, BRIDGE_NATIVE_COMPONENT(api, "ideatek:Transform",
        ideatek_transform_t, BRIDGE_STRIP, BRIDGE_TABLE,
        BRIDGE_ON_DESTROY_REMOVE), BRIDGE_ZEROED);
    api->attach(api, id, BRIDGE_NATIVE_COMPONENT(api, "ideatek:Camera",
        ideatek_camera_t, BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE),
        BRIDGE_ZEROED);
    api->attach(api, id, BRIDGE_NATIVE_COMPONENT(api, "ideatek:Projection",
        ideatek_projection_t, BRIDGE_STRIP, BRIDGE_TABLE,
        BRIDGE_ON_DESTROY_REMOVE), BRIDGE_ZEROED);
    api->attach(api, id, BRIDGE_NATIVE_COMPONENT(api,
        "ideatek:ProjectionAngles", ideatek_projection_angles_t, BRIDGE_STRIP,
        BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE), BRIDGE_ZEROED);
    api->attach(api, id, BRIDGE_NATIVE_COMPONENT(api, "ideatek:Joint",
        ideatek_joint_t, BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE),
        BRIDGE_ZEROED);
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
