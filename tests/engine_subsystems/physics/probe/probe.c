/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "bridge_native.h"
#include "ideatek_frame.h"
#include "ideatek_rendering.h"
#include "ideatek_physics.h"

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
    STRUCT(api, "ideatek:ideatek_quat_t", ideatek_quat_t);
    STRUCT(api, "ideatek:ideatek_position_t", ideatek_position_t);
    ENUM(api, "ideatek:ideatek_body_kind_t", ideatek_body_kind_t);
    ENUM(api, "ideatek:ideatek_mass_mode_t", ideatek_mass_mode_t);
    MASK(api, "ideatek:ideatek_locked_t", ideatek_locked_t);
    MASK(api, "ideatek:ideatek_body_flags_t", ideatek_body_flags_t);
    ENUM(api, "ideatek:ideatek_shape_kind_t", ideatek_shape_kind_t);
    MASK(api, "ideatek:ideatek_shape_flags_t", ideatek_shape_flags_t);
    ENUM(api, "ideatek:ideatek_combine_t", ideatek_combine_t);
    ENUM(api, "ideatek:ideatek_touch_kind_t", ideatek_touch_kind_t);
    ENUM(api, "ideatek:ideatek_query_kind_t", ideatek_query_kind_t);
    ENUM(api, "ideatek:ideatek_query_mode_t", ideatek_query_mode_t);
    MASK(api, "ideatek:ideatek_joint_flags_t", ideatek_joint_flags_t);
    MASK(api, "ideatek:ideatek_joint_features_t", ideatek_joint_features_t);
    MASK(api, "ideatek:ideatek_wheel_flags_t", ideatek_wheel_flags_t);
    MASK(api, "ideatek:ideatek_rope_flags_t", ideatek_rope_flags_t);
    MASK(api, "ideatek:ideatek_liquid_flags_t", ideatek_liquid_flags_t);
}

[[maybe_unused]] static void declare_late(const bridge_api_t *api)
{
    STRUCT(api, "ideatek:Body", ideatek_body_t);
    declare_fields(api);
}

[[maybe_unused]] static void declare_all(const bridge_api_t *api)
{
    declare_fields(api);
    STRUCT(api, "ideatek:Body", ideatek_body_t);
    STRUCT(api, "ideatek:Motion", ideatek_motion_t);
    STRUCT(api, "ideatek:Push", ideatek_push_t);
    STRUCT(api, "ideatek:Shape", ideatek_shape_t);
    STRUCT(api, "ideatek:PhysicsMaterial", ideatek_physics_material_t);
    STRUCT(api, "ideatek:SensorEvent", ideatek_sensor_event_t);
    STRUCT(api, "ideatek:ContactEvent", ideatek_contact_event_t);
    STRUCT(api, "ideatek:HitEvent", ideatek_hit_event_t);
    STRUCT(api, "ideatek:Query", ideatek_query_t);
    STRUCT(api, "ideatek:QueryHit", ideatek_query_hit_t);
    STRUCT(api, "ideatek:BodyJoint", ideatek_body_joint_t);
    TAG(api, "ideatek:FixedJoint");
    TAG(api, "ideatek:PointJoint");
    STRUCT(api, "ideatek:HingeJoint", ideatek_hinge_joint_t);
    STRUCT(api, "ideatek:SliderJoint", ideatek_slider_joint_t);
    STRUCT(api, "ideatek:DistanceJoint", ideatek_distance_joint_t);
    STRUCT(api, "ideatek:SwingTwistJoint", ideatek_swing_twist_joint_t);
    STRUCT(api, "ideatek:SixDofJoint", ideatek_six_dof_joint_t);
    STRUCT(api, "ideatek:BodyJointEvent", ideatek_body_joint_event_t);
    STRUCT(api, "ideatek:Vehicle", ideatek_vehicle_t);
    STRUCT(api, "ideatek:Wheel", ideatek_wheel_t);
    STRUCT(api, "ideatek:VehicleInput", ideatek_vehicle_input_t);
    STRUCT(api, "ideatek:WheelState", ideatek_wheel_state_t);
    STRUCT(api, "ideatek:PoseDrive", ideatek_pose_drive_t);
    STRUCT(api, "ideatek:Destructible", ideatek_destructible_t);
    STRUCT(api, "ideatek:Damage", ideatek_damage_t);
    STRUCT(api, "ideatek:BreakEvent", ideatek_break_event_t);
    STRUCT(api, "ideatek:HiddenJoint", ideatek_hidden_joint_t);
    STRUCT(api, "ideatek:Cut", ideatek_cut_t);
    STRUCT(api, "ideatek:CutEvent", ideatek_cut_event_t);
    STRUCT(api, "ideatek:SoftBody", ideatek_soft_body_t);
    STRUCT(api, "ideatek:SoftBodyReset", ideatek_soft_body_reset_t);
    STRUCT(api, "ideatek:Rope", ideatek_rope_t);
    STRUCT(api, "ideatek:Water", ideatek_water_t);
    STRUCT(api, "ideatek:Wave", ideatek_wave_t);
    STRUCT(api, "ideatek:WaterSample", ideatek_water_sample_t);
    STRUCT(api, "ideatek:Buoyancy", ideatek_buoyancy_t);
    STRUCT(api, "ideatek:Liquid", ideatek_liquid_t);
    STRUCT(api, "ideatek:LiquidEmitter", ideatek_liquid_emitter_t);
    STRUCT(api, "ideatek:LagHistory", ideatek_lag_history_t);
}

[[maybe_unused]] static void role_author(const bridge_api_t *api)
{
    uint64_t id = api->spawn(api);

    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    STRUCT(api, "ideatek:ideatek_vec3_t", ideatek_vec3_t);
    STRUCT(api, "ideatek:ideatek_quat_t", ideatek_quat_t);
    STRUCT(api, "ideatek:ideatek_position_t", ideatek_position_t);
    ENUM(api, "ideatek:ideatek_body_kind_t", ideatek_body_kind_t);
    ENUM(api, "ideatek:ideatek_mass_mode_t", ideatek_mass_mode_t);
    MASK(api, "ideatek:ideatek_locked_t", ideatek_locked_t);
    MASK(api, "ideatek:ideatek_body_flags_t", ideatek_body_flags_t);
    ENUM(api, "ideatek:ideatek_shape_kind_t", ideatek_shape_kind_t);
    MASK(api, "ideatek:ideatek_shape_flags_t", ideatek_shape_flags_t);
    ENUM(api, "ideatek:ideatek_combine_t", ideatek_combine_t);
    ENUM(api, "ideatek:ideatek_query_kind_t", ideatek_query_kind_t);
    ENUM(api, "ideatek:ideatek_query_mode_t", ideatek_query_mode_t);
    MASK(api, "ideatek:ideatek_joint_flags_t", ideatek_joint_flags_t);
    MASK(api, "ideatek:ideatek_joint_features_t", ideatek_joint_features_t);
    MASK(api, "ideatek:ideatek_wheel_flags_t", ideatek_wheel_flags_t);
    MASK(api, "ideatek:ideatek_rope_flags_t", ideatek_rope_flags_t);
    MASK(api, "ideatek:ideatek_liquid_flags_t", ideatek_liquid_flags_t);
    api->attach(api, id, STRUCT(api, "ideatek:Body", ideatek_body_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Push", ideatek_push_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Shape", ideatek_shape_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:PhysicsMaterial",
        ideatek_physics_material_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Query", ideatek_query_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:BodyJoint", ideatek_body_joint_t),
        BRIDGE_ZEROED);
    api->attach(api, id, TAG(api, "ideatek:FixedJoint"), BRIDGE_ZEROED);
    api->attach(api, id, TAG(api, "ideatek:PointJoint"), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:HingeJoint",
        ideatek_hinge_joint_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:SliderJoint",
        ideatek_slider_joint_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:DistanceJoint",
        ideatek_distance_joint_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:SwingTwistJoint",
        ideatek_swing_twist_joint_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:SixDofJoint",
        ideatek_six_dof_joint_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Vehicle", ideatek_vehicle_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Wheel", ideatek_wheel_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:VehicleInput",
        ideatek_vehicle_input_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:PoseDrive", ideatek_pose_drive_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Destructible",
        ideatek_destructible_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Damage", ideatek_damage_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:HiddenJoint",
        ideatek_hidden_joint_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Cut", ideatek_cut_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:SoftBody", ideatek_soft_body_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:SoftBodyReset",
        ideatek_soft_body_reset_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Rope", ideatek_rope_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Water", ideatek_water_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Wave", ideatek_wave_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Buoyancy", ideatek_buoyancy_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Liquid", ideatek_liquid_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:LiquidEmitter",
        ideatek_liquid_emitter_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:LagHistory",
        ideatek_lag_history_t), BRIDGE_ZEROED);
}

[[maybe_unused]] static void role_physics(const bridge_api_t *api)
{
    uint64_t id = api->spawn(api);

    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    STRUCT(api, "ideatek:ideatek_vec3_t", ideatek_vec3_t);
    STRUCT(api, "ideatek:ideatek_quat_t", ideatek_quat_t);
    STRUCT(api, "ideatek:ideatek_position_t", ideatek_position_t);
    ENUM(api, "ideatek:ideatek_topology_t", ideatek_topology_t);
    ENUM(api, "ideatek:ideatek_index_format_t", ideatek_index_format_t);
    ENUM(api, "ideatek:ideatek_body_kind_t", ideatek_body_kind_t);
    ENUM(api, "ideatek:ideatek_mass_mode_t", ideatek_mass_mode_t);
    MASK(api, "ideatek:ideatek_locked_t", ideatek_locked_t);
    MASK(api, "ideatek:ideatek_body_flags_t", ideatek_body_flags_t);
    ENUM(api, "ideatek:ideatek_shape_kind_t", ideatek_shape_kind_t);
    MASK(api, "ideatek:ideatek_shape_flags_t", ideatek_shape_flags_t);
    ENUM(api, "ideatek:ideatek_combine_t", ideatek_combine_t);
    ENUM(api, "ideatek:ideatek_touch_kind_t", ideatek_touch_kind_t);
    ENUM(api, "ideatek:ideatek_query_kind_t", ideatek_query_kind_t);
    ENUM(api, "ideatek:ideatek_query_mode_t", ideatek_query_mode_t);
    MASK(api, "ideatek:ideatek_joint_flags_t", ideatek_joint_flags_t);
    MASK(api, "ideatek:ideatek_joint_features_t", ideatek_joint_features_t);
    MASK(api, "ideatek:ideatek_wheel_flags_t", ideatek_wheel_flags_t);
    MASK(api, "ideatek:ideatek_rope_flags_t", ideatek_rope_flags_t);
    MASK(api, "ideatek:ideatek_liquid_flags_t", ideatek_liquid_flags_t);
    STRUCT(api, "ideatek:Body", ideatek_body_t);
    STRUCT(api, "ideatek:Push", ideatek_push_t);
    STRUCT(api, "ideatek:Shape", ideatek_shape_t);
    STRUCT(api, "ideatek:PhysicsMaterial", ideatek_physics_material_t);
    STRUCT(api, "ideatek:Query", ideatek_query_t);
    STRUCT(api, "ideatek:BodyJoint", ideatek_body_joint_t);
    TAG(api, "ideatek:FixedJoint");
    TAG(api, "ideatek:PointJoint");
    STRUCT(api, "ideatek:HingeJoint", ideatek_hinge_joint_t);
    STRUCT(api, "ideatek:SliderJoint", ideatek_slider_joint_t);
    STRUCT(api, "ideatek:DistanceJoint", ideatek_distance_joint_t);
    STRUCT(api, "ideatek:SwingTwistJoint", ideatek_swing_twist_joint_t);
    STRUCT(api, "ideatek:SixDofJoint", ideatek_six_dof_joint_t);
    STRUCT(api, "ideatek:Vehicle", ideatek_vehicle_t);
    STRUCT(api, "ideatek:Wheel", ideatek_wheel_t);
    STRUCT(api, "ideatek:VehicleInput", ideatek_vehicle_input_t);
    STRUCT(api, "ideatek:PoseDrive", ideatek_pose_drive_t);
    STRUCT(api, "ideatek:Destructible", ideatek_destructible_t);
    STRUCT(api, "ideatek:Damage", ideatek_damage_t);
    STRUCT(api, "ideatek:HiddenJoint", ideatek_hidden_joint_t);
    STRUCT(api, "ideatek:Cut", ideatek_cut_t);
    STRUCT(api, "ideatek:SoftBody", ideatek_soft_body_t);
    STRUCT(api, "ideatek:SoftBodyReset", ideatek_soft_body_reset_t);
    STRUCT(api, "ideatek:Rope", ideatek_rope_t);
    STRUCT(api, "ideatek:Water", ideatek_water_t);
    STRUCT(api, "ideatek:Wave", ideatek_wave_t);
    STRUCT(api, "ideatek:Buoyancy", ideatek_buoyancy_t);
    STRUCT(api, "ideatek:Liquid", ideatek_liquid_t);
    STRUCT(api, "ideatek:LiquidEmitter", ideatek_liquid_emitter_t);
    STRUCT(api, "ideatek:LagHistory", ideatek_lag_history_t);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:Tick", ideatek_tick_t, BRIDGE_STRIP,
        BRIDGE_SINGLETON, BRIDGE_ON_DESTROY_REMOVE);
    api->attach(api, id, BRIDGE_NATIVE_COMPONENT(api, "ideatek:Transform",
        ideatek_transform_t, BRIDGE_STRIP, BRIDGE_TABLE,
        BRIDGE_ON_DESTROY_REMOVE), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Motion", ideatek_motion_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:SensorEvent",
        ideatek_sensor_event_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ContactEvent",
        ideatek_contact_event_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:HitEvent", ideatek_hit_event_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:QueryHit", ideatek_query_hit_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:BodyJointEvent",
        ideatek_body_joint_event_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:WheelState",
        ideatek_wheel_state_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:BreakEvent",
        ideatek_break_event_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:CutEvent", ideatek_cut_event_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:WaterSample",
        ideatek_water_sample_t), BRIDGE_ZEROED);
    api->attach(api, id, BRIDGE_NATIVE_COMPONENT(api, "ideatek:Mesh",
        ideatek_mesh_t, BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE),
        BRIDGE_ZEROED);
    api->attach(api, id, BRIDGE_NATIVE_COMPONENT(api, "ideatek:MeshInstance",
        ideatek_mesh_instance_t, BRIDGE_STRIP, BRIDGE_TABLE,
        BRIDGE_ON_DESTROY_REMOVE), BRIDGE_ZEROED);
}

[[maybe_unused]] static void role_renderer(const bridge_api_t *api)
{
    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    STRUCT(api, "ideatek:ideatek_vec3_t", ideatek_vec3_t);
    STRUCT(api, "ideatek:HiddenJoint", ideatek_hidden_joint_t);
    STRUCT(api, "ideatek:Water", ideatek_water_t);
    STRUCT(api, "ideatek:Wave", ideatek_wave_t);
}

BRIDGE_NATIVE(init)
{
#if defined(ROLE_LATE)
    declare_late(api);
#elif defined(ROLE_AUTHOR)
    role_author(api);
#elif defined(ROLE_PHYSICS)
    role_physics(api);
#elif defined(ROLE_RENDERER)
    role_renderer(api);
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
