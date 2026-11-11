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
#define SINGLE(api, name, T) BRIDGE_NATIVE_COMPONENT(api, name, T, \
    BRIDGE_STRIP, BRIDGE_SINGLETON, BRIDGE_ON_DESTROY_REMOVE)
#define TAG(api, name) BRIDGE_NATIVE_DECLARE_TAG(api, name, BRIDGE_STRIP, \
    BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)

[[maybe_unused]] static void declare_fields(const bridge_api_t *api)
{
    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    STRUCT(api, "ideatek:ideatek_vec3_t", ideatek_vec3_t);
    STRUCT(api, "ideatek:ideatek_quat_t", ideatek_quat_t);
    STRUCT(api, "ideatek:ideatek_position_t", ideatek_position_t);
    ENUM(api, "ideatek:ideatek_body_kind_t", ideatek_body_kind_t);
    ENUM(api, "ideatek:ideatek_motion_quality_t", ideatek_motion_quality_t);
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
    ENUM(api, "ideatek:ideatek_drive_mode_t", ideatek_drive_mode_t);
    ENUM(api, "ideatek:ideatek_spring_mode_t", ideatek_spring_mode_t);
    STRUCT(api, "ideatek:ideatek_joint_drive_t", ideatek_joint_drive_t);
    ENUM(api, "ideatek:ideatek_path_rotation_t", ideatek_path_rotation_t);
    ENUM(api, "ideatek:ideatek_articulation_joint_t",
        ideatek_articulation_joint_t);
    MASK(api, "ideatek:ideatek_articulation_flags_t",
        ideatek_articulation_flags_t);
    ENUM(api, "ideatek:ideatek_ground_state_t", ideatek_ground_state_t);
    ENUM(api, "ideatek:ideatek_non_walkable_t", ideatek_non_walkable_t);
    MASK(api, "ideatek:ideatek_character_flags_t", ideatek_character_flags_t);
    MASK(api, "ideatek:ideatek_character_collision_t",
        ideatek_character_collision_t);
    ENUM(api, "ideatek:ideatek_snapshot_action_t", ideatek_snapshot_action_t);
    ENUM(api, "ideatek:ideatek_snapshot_status_t", ideatek_snapshot_status_t);
    ENUM(api, "ideatek:ideatek_vehicle_kind_t", ideatek_vehicle_kind_t);
    ENUM(api, "ideatek:ideatek_wheel_test_t", ideatek_wheel_test_t);
    ENUM(api, "ideatek:ideatek_transmission_mode_t",
        ideatek_transmission_mode_t);
    MASK(api, "ideatek:ideatek_wheel_flags_t", ideatek_wheel_flags_t);
    ENUM(api, "ideatek:ideatek_damage_shape_t", ideatek_damage_shape_t);
    ENUM(api, "ideatek:ideatek_bend_t", ideatek_bend_t);
    ENUM(api, "ideatek:ideatek_lra_t", ideatek_lra_t);
    MASK(api, "ideatek:ideatek_rope_flags_t", ideatek_rope_flags_t);
    MASK(api, "ideatek:ideatek_liquid_flags_t", ideatek_liquid_flags_t);
    ENUM(api, "ideatek:ideatek_liquid_technique_t", ideatek_liquid_technique_t);
    ENUM(api, "ideatek:ideatek_emitter_shape_t", ideatek_emitter_shape_t);
    MASK(api, "ideatek:ideatek_gas_flags_t", ideatek_gas_flags_t);
    ENUM(api, "ideatek:ideatek_field_shape_t", ideatek_field_shape_t);
    ENUM(api, "ideatek:ideatek_force_kind_t", ideatek_force_kind_t);
    ENUM(api, "ideatek:ideatek_field_target_t", ideatek_field_target_t);
    ENUM(api, "ideatek:ideatek_gravity_mode_t", ideatek_gravity_mode_t);
    ENUM(api, "ideatek:ideatek_time_mode_t", ideatek_time_mode_t);
    ENUM(api, "ideatek:ideatek_air_mode_t", ideatek_air_mode_t);
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
    STRUCT(api, "ideatek:KinematicTarget", ideatek_kinematic_target_t);
    STRUCT(api, "ideatek:WakeEvent", ideatek_wake_event_t);
    STRUCT(api, "ideatek:Motion", ideatek_motion_t);
    STRUCT(api, "ideatek:Push", ideatek_push_t);
    STRUCT(api, "ideatek:Shape", ideatek_shape_t);
    STRUCT(api, "ideatek:ShapeMaterials", ideatek_shape_materials_t);
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
    STRUCT(api, "ideatek:GearJoint", ideatek_gear_joint_t);
    STRUCT(api, "ideatek:RackAndPinionJoint", ideatek_rack_and_pinion_joint_t);
    STRUCT(api, "ideatek:PulleyJoint", ideatek_pulley_joint_t);
    STRUCT(api, "ideatek:PathJoint", ideatek_path_joint_t);
    STRUCT(api, "ideatek:ConeJoint", ideatek_cone_joint_t);
    STRUCT(api, "ideatek:MotorJoint", ideatek_motor_joint_t);
    STRUCT(api, "ideatek:WheelJoint", ideatek_wheel_joint_t);
    TAG(api, "ideatek:NoCollideJoint");
    STRUCT(api, "ideatek:BodyJointEvent", ideatek_body_joint_event_t);
    STRUCT(api, "ideatek:BodyJointState", ideatek_body_joint_state_t);
    STRUCT(api, "ideatek:Articulation", ideatek_articulation_t);
    STRUCT(api, "ideatek:ArticulationLink", ideatek_articulation_link_t);
    STRUCT(api, "ideatek:ArticulationState", ideatek_articulation_state_t);
    STRUCT(api, "ideatek:ArticulationCommand", ideatek_articulation_command_t);
    STRUCT(api, "ideatek:Character", ideatek_character_t);
    STRUCT(api, "ideatek:CharacterMove", ideatek_character_move_t);
    STRUCT(api, "ideatek:CharacterState", ideatek_character_state_t);
    STRUCT(api, "ideatek:PhysicsSnapshotRequest",
        ideatek_physics_snapshot_request_t);
    STRUCT(api, "ideatek:PhysicsSnapshot", ideatek_physics_snapshot_t);
    STRUCT(api, "ideatek:Vehicle", ideatek_vehicle_t);
    STRUCT(api, "ideatek:VehicleEngine", ideatek_vehicle_engine_t);
    STRUCT(api, "ideatek:VehicleTransmission", ideatek_vehicle_transmission_t);
    STRUCT(api, "ideatek:VehicleDifferential", ideatek_vehicle_differential_t);
    STRUCT(api, "ideatek:AntiRollBar", ideatek_anti_roll_bar_t);
    STRUCT(api, "ideatek:Wheel", ideatek_wheel_t);
    STRUCT(api, "ideatek:VehicleInput", ideatek_vehicle_input_t);
    STRUCT(api, "ideatek:WheelState", ideatek_wheel_state_t);
    STRUCT(api, "ideatek:VehicleState", ideatek_vehicle_state_t);
    STRUCT(api, "ideatek:PoseDrive", ideatek_pose_drive_t);
    STRUCT(api, "ideatek:Destructible", ideatek_destructible_t);
    STRUCT(api, "ideatek:DestructibleAsset", ideatek_destructible_asset_t);
    STRUCT(api, "ideatek:Damage", ideatek_damage_t);
    STRUCT(api, "ideatek:BreakEvent", ideatek_break_event_t);
    STRUCT(api, "ideatek:SoftBody", ideatek_soft_body_t);
    STRUCT(api, "ideatek:SoftBodyAttachment", ideatek_soft_body_attachment_t);
    STRUCT(api, "ideatek:SoftBodyReset", ideatek_soft_body_reset_t);
    STRUCT(api, "ideatek:Rope", ideatek_rope_t);
    STRUCT(api, "ideatek:Water", ideatek_water_t);
    STRUCT(api, "ideatek:Wave", ideatek_wave_t);
    STRUCT(api, "ideatek:WaterSample", ideatek_water_sample_t);
    STRUCT(api, "ideatek:Buoyancy", ideatek_buoyancy_t);
    STRUCT(api, "ideatek:Liquid", ideatek_liquid_t);
    STRUCT(api, "ideatek:LiquidEmitter", ideatek_liquid_emitter_t);
    STRUCT(api, "ideatek:LiquidParticles", ideatek_liquid_particles_t);
    STRUCT(api, "ideatek:LiquidFoam", ideatek_liquid_foam_t);
    STRUCT(api, "ideatek:GasVolume", ideatek_gas_volume_t);
    STRUCT(api, "ideatek:GasEmitter", ideatek_gas_emitter_t);
    STRUCT(api, "ideatek:GasGrid", ideatek_gas_grid_t);
    STRUCT(api, "ideatek:GasSample", ideatek_gas_sample_t);
    STRUCT(api, "ideatek:SightSample", ideatek_sight_sample_t);
    STRUCT(api, "ideatek:LagHistory", ideatek_lag_history_t);
    STRUCT(api, "ideatek:Wind", ideatek_wind_t);
    STRUCT(api, "ideatek:ForceField", ideatek_force_field_t);
    STRUCT(api, "ideatek:GravityArea", ideatek_gravity_area_t);
    STRUCT(api, "ideatek:TimeArea", ideatek_time_area_t);
    STRUCT(api, "ideatek:AirArea", ideatek_air_area_t);
    ENUM(api, "ideatek:ideatek_gas_face_kind_t", ideatek_gas_face_kind_t);
    STRUCT(api, "ideatek:ideatek_gas_face_t", ideatek_gas_face_t);
    STRUCT(api, "ideatek:GasFill", ideatek_gas_fill_t);
    STRUCT(api, "ideatek:AirProbe", ideatek_air_probe_t);
    STRUCT(api, "ideatek:SpinArea", ideatek_spin_area_t);
    STRUCT(api, "ideatek:ideatek_contact_work_t", ideatek_contact_work_t);
    SINGLE(api, "ideatek:ContactWork", ideatek_contact_work_list_t);
    MASK(api, "ideatek:ideatek_particle_set_flags_t", ideatek_particle_set_flags_t);
    STRUCT(api, "ideatek:ideatek_particle_t", ideatek_particle_t);
    STRUCT(api, "ideatek:ParticleSet", ideatek_particle_set_t);
    ENUM(api, "ideatek:ideatek_solver_t", ideatek_solver_t);
    STRUCT(api, "ideatek:ideatek_phase_request_t", ideatek_phase_request_t);
    SINGLE(api, "ideatek:PhaseRequest", ideatek_phase_request_list_t);
    STRUCT(api, "ideatek:ideatek_surface_material_t", ideatek_surface_material_t);
    SINGLE(api, "ideatek:SurfaceMaterial", ideatek_surface_material_list_t);
    STRUCT(api, "ideatek:ideatek_heat_source_t", ideatek_heat_source_t);
    SINGLE(api, "ideatek:HeatSource", ideatek_heat_source_list_t);
    SINGLE(api, "ideatek:HeatTaken", ideatek_heat_taken_t);
}

[[maybe_unused]] static void role_author(const bridge_api_t *api)
{
    uint64_t id = api->spawn(api);

    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    STRUCT(api, "ideatek:ideatek_vec3_t", ideatek_vec3_t);
    STRUCT(api, "ideatek:ideatek_quat_t", ideatek_quat_t);
    STRUCT(api, "ideatek:ideatek_position_t", ideatek_position_t);
    ENUM(api, "ideatek:ideatek_body_kind_t", ideatek_body_kind_t);
    ENUM(api, "ideatek:ideatek_motion_quality_t", ideatek_motion_quality_t);
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
    ENUM(api, "ideatek:ideatek_drive_mode_t", ideatek_drive_mode_t);
    ENUM(api, "ideatek:ideatek_spring_mode_t", ideatek_spring_mode_t);
    STRUCT(api, "ideatek:ideatek_joint_drive_t", ideatek_joint_drive_t);
    ENUM(api, "ideatek:ideatek_path_rotation_t", ideatek_path_rotation_t);
    ENUM(api, "ideatek:ideatek_articulation_joint_t",
        ideatek_articulation_joint_t);
    MASK(api, "ideatek:ideatek_articulation_flags_t",
        ideatek_articulation_flags_t);
    ENUM(api, "ideatek:ideatek_non_walkable_t", ideatek_non_walkable_t);
    MASK(api, "ideatek:ideatek_character_flags_t", ideatek_character_flags_t);
    ENUM(api, "ideatek:ideatek_snapshot_action_t", ideatek_snapshot_action_t);
    ENUM(api, "ideatek:ideatek_vehicle_kind_t", ideatek_vehicle_kind_t);
    ENUM(api, "ideatek:ideatek_wheel_test_t", ideatek_wheel_test_t);
    ENUM(api, "ideatek:ideatek_transmission_mode_t",
        ideatek_transmission_mode_t);
    MASK(api, "ideatek:ideatek_wheel_flags_t", ideatek_wheel_flags_t);
    ENUM(api, "ideatek:ideatek_damage_shape_t", ideatek_damage_shape_t);
    ENUM(api, "ideatek:ideatek_bend_t", ideatek_bend_t);
    ENUM(api, "ideatek:ideatek_lra_t", ideatek_lra_t);
    MASK(api, "ideatek:ideatek_rope_flags_t", ideatek_rope_flags_t);
    MASK(api, "ideatek:ideatek_liquid_flags_t", ideatek_liquid_flags_t);
    ENUM(api, "ideatek:ideatek_liquid_technique_t", ideatek_liquid_technique_t);
    ENUM(api, "ideatek:ideatek_emitter_shape_t", ideatek_emitter_shape_t);
    MASK(api, "ideatek:ideatek_gas_flags_t", ideatek_gas_flags_t);
    ENUM(api, "ideatek:ideatek_field_shape_t", ideatek_field_shape_t);
    ENUM(api, "ideatek:ideatek_force_kind_t", ideatek_force_kind_t);
    ENUM(api, "ideatek:ideatek_field_target_t", ideatek_field_target_t);
    ENUM(api, "ideatek:ideatek_gravity_mode_t", ideatek_gravity_mode_t);
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
    api->attach(api, id, STRUCT(api, "ideatek:KinematicTarget",
        ideatek_kinematic_target_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ShapeMaterials",
        ideatek_shape_materials_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Character", ideatek_character_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:CharacterMove",
        ideatek_character_move_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:PhysicsSnapshotRequest",
        ideatek_physics_snapshot_request_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:DestructibleAsset",
        ideatek_destructible_asset_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:GearJoint", ideatek_gear_joint_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:RackAndPinionJoint",
        ideatek_rack_and_pinion_joint_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:PulleyJoint",
        ideatek_pulley_joint_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:PathJoint", ideatek_path_joint_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ConeJoint", ideatek_cone_joint_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:MotorJoint",
        ideatek_motor_joint_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:WheelJoint",
        ideatek_wheel_joint_t), BRIDGE_ZEROED);
    api->attach(api, id, TAG(api, "ideatek:NoCollideJoint"), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:VehicleEngine",
        ideatek_vehicle_engine_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:VehicleTransmission",
        ideatek_vehicle_transmission_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:VehicleDifferential",
        ideatek_vehicle_differential_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:AntiRollBar",
        ideatek_anti_roll_bar_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:SoftBodyAttachment",
        ideatek_soft_body_attachment_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:GasVolume",
        ideatek_gas_volume_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:GasEmitter",
        ideatek_gas_emitter_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Articulation",
        ideatek_articulation_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ArticulationLink",
        ideatek_articulation_link_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ArticulationCommand",
        ideatek_articulation_command_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Wind", ideatek_wind_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ForceField",
        ideatek_force_field_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:GravityArea",
        ideatek_gravity_area_t), BRIDGE_ZEROED);
    ENUM(api, "ideatek:ideatek_time_mode_t", ideatek_time_mode_t);
    api->attach(api, id, STRUCT(api, "ideatek:TimeArea",
        ideatek_time_area_t), BRIDGE_ZEROED);
    ENUM(api, "ideatek:ideatek_air_mode_t", ideatek_air_mode_t);
    api->attach(api, id, STRUCT(api, "ideatek:AirArea", ideatek_air_area_t),
        BRIDGE_ZEROED);
    ENUM(api, "ideatek:ideatek_gas_face_kind_t", ideatek_gas_face_kind_t);
    STRUCT(api, "ideatek:ideatek_gas_face_t", ideatek_gas_face_t);
    MASK(api, "ideatek:ideatek_particle_set_flags_t",
        ideatek_particle_set_flags_t);
    STRUCT(api, "ideatek:ideatek_particle_t", ideatek_particle_t);
    api->attach(api, id, STRUCT(api, "ideatek:GasFill", ideatek_gas_fill_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:AirProbe", ideatek_air_probe_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:SpinArea", ideatek_spin_area_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ParticleSet",
        ideatek_particle_set_t), BRIDGE_ZEROED);
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
    ENUM(api, "ideatek:ideatek_dimension_t", ideatek_dimension_t);
    ENUM(api, "ideatek:ideatek_body_kind_t", ideatek_body_kind_t);
    ENUM(api, "ideatek:ideatek_motion_quality_t", ideatek_motion_quality_t);
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
    ENUM(api, "ideatek:ideatek_drive_mode_t", ideatek_drive_mode_t);
    ENUM(api, "ideatek:ideatek_spring_mode_t", ideatek_spring_mode_t);
    STRUCT(api, "ideatek:ideatek_joint_drive_t", ideatek_joint_drive_t);
    ENUM(api, "ideatek:ideatek_path_rotation_t", ideatek_path_rotation_t);
    ENUM(api, "ideatek:ideatek_articulation_joint_t",
        ideatek_articulation_joint_t);
    MASK(api, "ideatek:ideatek_articulation_flags_t",
        ideatek_articulation_flags_t);
    ENUM(api, "ideatek:ideatek_ground_state_t", ideatek_ground_state_t);
    ENUM(api, "ideatek:ideatek_non_walkable_t", ideatek_non_walkable_t);
    MASK(api, "ideatek:ideatek_character_flags_t", ideatek_character_flags_t);
    MASK(api, "ideatek:ideatek_character_collision_t",
        ideatek_character_collision_t);
    ENUM(api, "ideatek:ideatek_snapshot_action_t", ideatek_snapshot_action_t);
    ENUM(api, "ideatek:ideatek_snapshot_status_t", ideatek_snapshot_status_t);
    ENUM(api, "ideatek:ideatek_vehicle_kind_t", ideatek_vehicle_kind_t);
    ENUM(api, "ideatek:ideatek_wheel_test_t", ideatek_wheel_test_t);
    ENUM(api, "ideatek:ideatek_transmission_mode_t",
        ideatek_transmission_mode_t);
    MASK(api, "ideatek:ideatek_wheel_flags_t", ideatek_wheel_flags_t);
    ENUM(api, "ideatek:ideatek_damage_shape_t", ideatek_damage_shape_t);
    ENUM(api, "ideatek:ideatek_bend_t", ideatek_bend_t);
    ENUM(api, "ideatek:ideatek_lra_t", ideatek_lra_t);
    MASK(api, "ideatek:ideatek_rope_flags_t", ideatek_rope_flags_t);
    MASK(api, "ideatek:ideatek_liquid_flags_t", ideatek_liquid_flags_t);
    ENUM(api, "ideatek:ideatek_liquid_technique_t", ideatek_liquid_technique_t);
    ENUM(api, "ideatek:ideatek_emitter_shape_t", ideatek_emitter_shape_t);
    MASK(api, "ideatek:ideatek_gas_flags_t", ideatek_gas_flags_t);
    ENUM(api, "ideatek:ideatek_field_shape_t", ideatek_field_shape_t);
    ENUM(api, "ideatek:ideatek_force_kind_t", ideatek_force_kind_t);
    ENUM(api, "ideatek:ideatek_field_target_t", ideatek_field_target_t);
    ENUM(api, "ideatek:ideatek_gravity_mode_t", ideatek_gravity_mode_t);
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
    STRUCT(api, "ideatek:SoftBody", ideatek_soft_body_t);
    STRUCT(api, "ideatek:SoftBodyReset", ideatek_soft_body_reset_t);
    STRUCT(api, "ideatek:Rope", ideatek_rope_t);
    STRUCT(api, "ideatek:Water", ideatek_water_t);
    STRUCT(api, "ideatek:Wave", ideatek_wave_t);
    STRUCT(api, "ideatek:Buoyancy", ideatek_buoyancy_t);
    STRUCT(api, "ideatek:Liquid", ideatek_liquid_t);
    STRUCT(api, "ideatek:LiquidEmitter", ideatek_liquid_emitter_t);
    STRUCT(api, "ideatek:LagHistory", ideatek_lag_history_t);
    STRUCT(api, "ideatek:KinematicTarget", ideatek_kinematic_target_t);
    STRUCT(api, "ideatek:ShapeMaterials", ideatek_shape_materials_t);
    STRUCT(api, "ideatek:Character", ideatek_character_t);
    STRUCT(api, "ideatek:CharacterMove", ideatek_character_move_t);
    STRUCT(api, "ideatek:PhysicsSnapshotRequest",
        ideatek_physics_snapshot_request_t);
    STRUCT(api, "ideatek:DestructibleAsset", ideatek_destructible_asset_t);
    STRUCT(api, "ideatek:GearJoint", ideatek_gear_joint_t);
    STRUCT(api, "ideatek:RackAndPinionJoint", ideatek_rack_and_pinion_joint_t);
    STRUCT(api, "ideatek:PulleyJoint", ideatek_pulley_joint_t);
    STRUCT(api, "ideatek:PathJoint", ideatek_path_joint_t);
    STRUCT(api, "ideatek:ConeJoint", ideatek_cone_joint_t);
    STRUCT(api, "ideatek:MotorJoint", ideatek_motor_joint_t);
    STRUCT(api, "ideatek:WheelJoint", ideatek_wheel_joint_t);
    TAG(api, "ideatek:NoCollideJoint");
    STRUCT(api, "ideatek:VehicleEngine", ideatek_vehicle_engine_t);
    STRUCT(api, "ideatek:VehicleTransmission", ideatek_vehicle_transmission_t);
    STRUCT(api, "ideatek:VehicleDifferential", ideatek_vehicle_differential_t);
    STRUCT(api, "ideatek:AntiRollBar", ideatek_anti_roll_bar_t);
    STRUCT(api, "ideatek:SoftBodyAttachment", ideatek_soft_body_attachment_t);
    STRUCT(api, "ideatek:GasVolume", ideatek_gas_volume_t);
    STRUCT(api, "ideatek:GasEmitter", ideatek_gas_emitter_t);
    STRUCT(api, "ideatek:Articulation", ideatek_articulation_t);
    STRUCT(api, "ideatek:ArticulationLink", ideatek_articulation_link_t);
    STRUCT(api, "ideatek:ArticulationCommand", ideatek_articulation_command_t);
    STRUCT(api, "ideatek:Wind", ideatek_wind_t);
    STRUCT(api, "ideatek:ForceField", ideatek_force_field_t);
    STRUCT(api, "ideatek:GravityArea", ideatek_gravity_area_t);
    ENUM(api, "ideatek:ideatek_time_mode_t", ideatek_time_mode_t);
    STRUCT(api, "ideatek:TimeArea", ideatek_time_area_t);
    ENUM(api, "ideatek:ideatek_air_mode_t", ideatek_air_mode_t);
    STRUCT(api, "ideatek:AirArea", ideatek_air_area_t);
    ENUM(api, "ideatek:ideatek_gas_face_kind_t", ideatek_gas_face_kind_t);
    STRUCT(api, "ideatek:ideatek_gas_face_t", ideatek_gas_face_t);
    MASK(api, "ideatek:ideatek_particle_set_flags_t",
        ideatek_particle_set_flags_t);
    STRUCT(api, "ideatek:ideatek_particle_t", ideatek_particle_t);
    ENUM(api, "ideatek:ideatek_solver_t", ideatek_solver_t);
    STRUCT(api, "ideatek:ideatek_contact_work_t", ideatek_contact_work_t);
    STRUCT(api, "ideatek:ideatek_phase_request_t", ideatek_phase_request_t);
    STRUCT(api, "ideatek:ideatek_surface_material_t",
        ideatek_surface_material_t);
    STRUCT(api, "ideatek:ideatek_heat_source_t", ideatek_heat_source_t);
    SINGLE(api, "ideatek:PhaseRequest", ideatek_phase_request_list_t);
    SINGLE(api, "ideatek:SurfaceMaterial", ideatek_surface_material_list_t);
    SINGLE(api, "ideatek:HeatSource", ideatek_heat_source_list_t);
    api->attach(api, 0, SINGLE(api, "ideatek:ContactWork",
        ideatek_contact_work_list_t), BRIDGE_ZEROED);
    api->attach(api, 0, SINGLE(api, "ideatek:HeatTaken", ideatek_heat_taken_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:AirProbe", ideatek_air_probe_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ParticleSet",
        ideatek_particle_set_t), BRIDGE_ZEROED);
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
    api->attach(api, id, STRUCT(api, "ideatek:WaterSample",
        ideatek_water_sample_t), BRIDGE_ZEROED);
    api->attach(api, id, BRIDGE_NATIVE_COMPONENT(api, "ideatek:Mesh",
        ideatek_mesh_t, BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE),
        BRIDGE_ZEROED);
    api->attach(api, id, BRIDGE_NATIVE_COMPONENT(api, "ideatek:MeshInstance",
        ideatek_mesh_instance_t, BRIDGE_STRIP, BRIDGE_TABLE,
        BRIDGE_ON_DESTROY_REMOVE), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:WakeEvent", ideatek_wake_event_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:BodyJointState",
        ideatek_body_joint_state_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:CharacterState",
        ideatek_character_state_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:PhysicsSnapshot",
        ideatek_physics_snapshot_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:VehicleState",
        ideatek_vehicle_state_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:LiquidParticles",
        ideatek_liquid_particles_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:LiquidFoam",
        ideatek_liquid_foam_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:GasGrid", ideatek_gas_grid_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:GasSample",
        ideatek_gas_sample_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:SightSample",
        ideatek_sight_sample_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:ArticulationState",
        ideatek_articulation_state_t), BRIDGE_ZEROED);
    api->attach(api, id, BRIDGE_NATIVE_COMPONENT(api, "ideatek:Texture",
        ideatek_texture_t, BRIDGE_STRIP, BRIDGE_TABLE,
        BRIDGE_ON_DESTROY_REMOVE), BRIDGE_ZEROED);
}

[[maybe_unused]] static void role_matter(const bridge_api_t *api)
{
    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    STRUCT(api, "ideatek:ideatek_vec3_t", ideatek_vec3_t);
    STRUCT(api, "ideatek:ideatek_position_t", ideatek_position_t);
    ENUM(api, "ideatek:ideatek_combine_t", ideatek_combine_t);
    ENUM(api, "ideatek:ideatek_body_kind_t", ideatek_body_kind_t);
    ENUM(api, "ideatek:ideatek_mass_mode_t", ideatek_mass_mode_t);
    MASK(api, "ideatek:ideatek_locked_t", ideatek_locked_t);
    MASK(api, "ideatek:ideatek_body_flags_t", ideatek_body_flags_t);
    ENUM(api, "ideatek:ideatek_motion_quality_t", ideatek_motion_quality_t);
    MASK(api, "ideatek:ideatek_particle_set_flags_t",
        ideatek_particle_set_flags_t);
    ENUM(api, "ideatek:ideatek_solver_t", ideatek_solver_t);
    STRUCT(api, "ideatek:ideatek_contact_work_t", ideatek_contact_work_t);
    STRUCT(api, "ideatek:ideatek_particle_t", ideatek_particle_t);
    STRUCT(api, "ideatek:ideatek_phase_request_t", ideatek_phase_request_t);
    STRUCT(api, "ideatek:ideatek_surface_material_t",
        ideatek_surface_material_t);
    STRUCT(api, "ideatek:ideatek_heat_source_t", ideatek_heat_source_t);
    SINGLE(api, "ideatek:ContactWork", ideatek_contact_work_list_t);
    SINGLE(api, "ideatek:HeatTaken", ideatek_heat_taken_t);
    STRUCT(api, "ideatek:AirProbe", ideatek_air_probe_t);
    STRUCT(api, "ideatek:ParticleSet", ideatek_particle_set_t);
    STRUCT(api, "ideatek:DestructibleAsset", ideatek_destructible_asset_t);
    STRUCT(api, "ideatek:PhysicsMaterial", ideatek_physics_material_t);
    STRUCT(api, "ideatek:Body", ideatek_body_t);
    api->attach(api, 0, SINGLE(api, "ideatek:PhaseRequest",
        ideatek_phase_request_list_t), BRIDGE_ZEROED);
    api->attach(api, 0, SINGLE(api, "ideatek:SurfaceMaterial",
        ideatek_surface_material_list_t), BRIDGE_ZEROED);
    api->attach(api, 0, SINGLE(api, "ideatek:HeatSource",
        ideatek_heat_source_list_t), BRIDGE_ZEROED);
}

[[maybe_unused]] static void role_renderer(const bridge_api_t *api)
{
    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    STRUCT(api, "ideatek:ideatek_vec3_t", ideatek_vec3_t);
    STRUCT(api, "ideatek:Water", ideatek_water_t);
    STRUCT(api, "ideatek:Wave", ideatek_wave_t);
    STRUCT(api, "ideatek:LiquidParticles", ideatek_liquid_particles_t);
    STRUCT(api, "ideatek:LiquidFoam", ideatek_liquid_foam_t);
    MASK(api, "ideatek:ideatek_gas_flags_t", ideatek_gas_flags_t);
    STRUCT(api, "ideatek:GasVolume", ideatek_gas_volume_t);
    STRUCT(api, "ideatek:GasGrid", ideatek_gas_grid_t);
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
#elif defined(ROLE_MATTER)
    role_matter(api);
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
