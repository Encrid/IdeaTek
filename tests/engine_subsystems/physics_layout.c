/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <ideatek_physics.h>

#define SIZE(type) printf("size %zu\nalignment %zu\n", sizeof(type), \
    alignof(type))
#define AT(type, field) printf("%s %zu\n", #field, offsetof(type, field))
#define TEXT_OF(...) BRIDGE_SCHEMA_TEXT(__VA_ARGS__)

typedef struct {
    const char *name;
    void (*show)(void);
} show_mode_t;

static void show_body_kind(void)
{
    SIZE(ideatek_body_kind_t);
}

static void show_motion_quality(void)
{
    SIZE(ideatek_motion_quality_t);
}

static void show_mass_mode(void)
{
    SIZE(ideatek_mass_mode_t);
}

static void show_locked(void)
{
    SIZE(ideatek_locked_t);
}

static void show_body_flags(void)
{
    SIZE(ideatek_body_flags_t);
}

static void show_body(void)
{
    SIZE(ideatek_body_t);
    AT(ideatek_body_t, center_of_mass);
    AT(ideatek_body_t, inertia);
    AT(ideatek_body_t, mass);
    AT(ideatek_body_t, gravity_scale);
    AT(ideatek_body_t, linear_damping);
    AT(ideatek_body_t, angular_damping);
    AT(ideatek_body_t, max_linear_speed);
    AT(ideatek_body_t, max_angular_speed);
    AT(ideatek_body_t, sleep_threshold);
    AT(ideatek_body_t, inertia_scale);
    AT(ideatek_body_t, kind);
    AT(ideatek_body_t, mass_mode);
    AT(ideatek_body_t, locked);
    AT(ideatek_body_t, flags);
    AT(ideatek_body_t, motion_quality);
    AT(ideatek_body_t, velocity_steps);
    AT(ideatek_body_t, position_steps);
}

static void show_kinematic_target(void)
{
    SIZE(ideatek_kinematic_target_t);
    AT(ideatek_kinematic_target_t, position);
    AT(ideatek_kinematic_target_t, rotation);
}

static void show_wake_event(void)
{
    SIZE(ideatek_wake_event_t);
    AT(ideatek_wake_event_t, body);
    AT(ideatek_wake_event_t, order);
    AT(ideatek_wake_event_t, asleep);
}

static void show_motion(void)
{
    SIZE(ideatek_motion_t);
    AT(ideatek_motion_t, linear);
    AT(ideatek_motion_t, angular);
    AT(ideatek_motion_t, sleeping);
}

static void show_push(void)
{
    SIZE(ideatek_push_t);
    AT(ideatek_push_t, point);
    AT(ideatek_push_t, force);
    AT(ideatek_push_t, torque);
    AT(ideatek_push_t, impulse);
    AT(ideatek_push_t, angular_impulse);
}

static void show_shape_kind(void)
{
    SIZE(ideatek_shape_kind_t);
}

static void show_shape_flags(void)
{
    SIZE(ideatek_shape_flags_t);
}

static void show_shape(void)
{
    SIZE(ideatek_shape_t);
    AT(ideatek_shape_t, body);
    AT(ideatek_shape_t, material);
    AT(ideatek_shape_t, source);
    AT(ideatek_shape_t, categories);
    AT(ideatek_shape_t, touches);
    AT(ideatek_shape_t, size);
    AT(ideatek_shape_t, part);
    AT(ideatek_shape_t, kind);
    AT(ideatek_shape_t, flags);
}

static void show_shape_materials(void)
{
    SIZE(ideatek_shape_materials_t);
    AT(ideatek_shape_materials_t, materials);
    AT(ideatek_shape_materials_t, indices);
}

static void show_combine(void)
{
    SIZE(ideatek_combine_t);
}

static void show_physics_material(void)
{
    SIZE(ideatek_physics_material_t);
    AT(ideatek_physics_material_t, friction);
    AT(ideatek_physics_material_t, restitution);
    AT(ideatek_physics_material_t, density);
    AT(ideatek_physics_material_t, tensile);
    AT(ideatek_physics_material_t, compression);
    AT(ideatek_physics_material_t, shear);
    AT(ideatek_physics_material_t, friction_combine);
    AT(ideatek_physics_material_t, restitution_combine);
}

static void show_touch_kind(void)
{
    SIZE(ideatek_touch_kind_t);
}

static void show_sensor_event(void)
{
    SIZE(ideatek_sensor_event_t);
    AT(ideatek_sensor_event_t, sensor);
    AT(ideatek_sensor_event_t, visitor);
    AT(ideatek_sensor_event_t, order);
    AT(ideatek_sensor_event_t, kind);
}

static void show_contact_event(void)
{
    SIZE(ideatek_contact_event_t);
    AT(ideatek_contact_event_t, a);
    AT(ideatek_contact_event_t, b);
    AT(ideatek_contact_event_t, order);
    AT(ideatek_contact_event_t, kind);
}

static void show_hit_event(void)
{
    SIZE(ideatek_hit_event_t);
    AT(ideatek_hit_event_t, a);
    AT(ideatek_hit_event_t, b);
    AT(ideatek_hit_event_t, point);
    AT(ideatek_hit_event_t, normal);
    AT(ideatek_hit_event_t, speed);
    AT(ideatek_hit_event_t, order);
}

static void show_query_kind(void)
{
    SIZE(ideatek_query_kind_t);
}

static void show_query_mode(void)
{
    SIZE(ideatek_query_mode_t);
}

static void show_query(void)
{
    SIZE(ideatek_query_t);
    AT(ideatek_query_t, origin);
    AT(ideatek_query_t, categories);
    AT(ideatek_query_t, touches);
    AT(ideatek_query_t, rotation);
    AT(ideatek_query_t, translation);
    AT(ideatek_query_t, size);
    AT(ideatek_query_t, max);
    AT(ideatek_query_t, tick);
    AT(ideatek_query_t, kind);
    AT(ideatek_query_t, shape);
    AT(ideatek_query_t, mode);
    AT(ideatek_query_t, back_faces);
}

static void show_query_hit(void)
{
    SIZE(ideatek_query_hit_t);
    AT(ideatek_query_hit_t, query);
    AT(ideatek_query_hit_t, shape);
    AT(ideatek_query_hit_t, point);
    AT(ideatek_query_hit_t, normal);
    AT(ideatek_query_hit_t, fraction);
    AT(ideatek_query_hit_t, material);
    AT(ideatek_query_hit_t, uv);
    AT(ideatek_query_hit_t, depth);
    AT(ideatek_query_hit_t, face);
    AT(ideatek_query_hit_t, sub_shape);
    AT(ideatek_query_hit_t, order);
}

static void show_joint_flags(void)
{
    SIZE(ideatek_joint_flags_t);
}

static void show_body_joint(void)
{
    SIZE(ideatek_body_joint_t);
    AT(ideatek_body_joint_t, a);
    AT(ideatek_body_joint_t, b);
    AT(ideatek_body_joint_t, rotation_a);
    AT(ideatek_body_joint_t, rotation_b);
    AT(ideatek_body_joint_t, position_a);
    AT(ideatek_body_joint_t, position_b);
    AT(ideatek_body_joint_t, force_threshold);
    AT(ideatek_body_joint_t, torque_threshold);
    AT(ideatek_body_joint_t, flags);
}

static void show_joint_features(void)
{
    SIZE(ideatek_joint_features_t);
}

static void show_drive_mode(void)
{
    SIZE(ideatek_drive_mode_t);
}

static void show_spring_mode(void)
{
    SIZE(ideatek_spring_mode_t);
}

static void show_joint_drive(void)
{
    SIZE(ideatek_joint_drive_t);
    AT(ideatek_joint_drive_t, target);
    AT(ideatek_joint_drive_t, min_force);
    AT(ideatek_joint_drive_t, max_force);
    AT(ideatek_joint_drive_t, spring);
    AT(ideatek_joint_drive_t, damping);
    AT(ideatek_joint_drive_t, mode);
    AT(ideatek_joint_drive_t, spring_mode);
}

static void show_hinge_joint(void)
{
    SIZE(ideatek_hinge_joint_t);
    AT(ideatek_hinge_joint_t, lower);
    AT(ideatek_hinge_joint_t, upper);
    AT(ideatek_hinge_joint_t, limit_hertz);
    AT(ideatek_hinge_joint_t, limit_damping);
    AT(ideatek_hinge_joint_t, friction);
    AT(ideatek_hinge_joint_t, drive);
    AT(ideatek_hinge_joint_t, features);
}

static void show_slider_joint(void)
{
    SIZE(ideatek_slider_joint_t);
    AT(ideatek_slider_joint_t, lower);
    AT(ideatek_slider_joint_t, upper);
    AT(ideatek_slider_joint_t, limit_hertz);
    AT(ideatek_slider_joint_t, limit_damping);
    AT(ideatek_slider_joint_t, friction);
    AT(ideatek_slider_joint_t, drive);
    AT(ideatek_slider_joint_t, features);
}

static void show_distance_joint(void)
{
    SIZE(ideatek_distance_joint_t);
    AT(ideatek_distance_joint_t, length);
    AT(ideatek_distance_joint_t, min_length);
    AT(ideatek_distance_joint_t, max_length);
    AT(ideatek_distance_joint_t, spring_hertz);
    AT(ideatek_distance_joint_t, spring_damping);
    AT(ideatek_distance_joint_t, motor_speed);
    AT(ideatek_distance_joint_t, max_motor_force);
    AT(ideatek_distance_joint_t, features);
}

static void show_swing_twist_joint(void)
{
    SIZE(ideatek_swing_twist_joint_t);
    AT(ideatek_swing_twist_joint_t, swing_y);
    AT(ideatek_swing_twist_joint_t, swing_z);
    AT(ideatek_swing_twist_joint_t, twist_min);
    AT(ideatek_swing_twist_joint_t, twist_max);
}

static void show_six_dof_joint(void)
{
    SIZE(ideatek_six_dof_joint_t);
    AT(ideatek_six_dof_joint_t, linear_min);
    AT(ideatek_six_dof_joint_t, linear_max);
    AT(ideatek_six_dof_joint_t, angular_min);
    AT(ideatek_six_dof_joint_t, angular_max);
    AT(ideatek_six_dof_joint_t, drives);
    AT(ideatek_six_dof_joint_t, friction);
}

static void show_gear_joint(void)
{
    SIZE(ideatek_gear_joint_t);
    AT(ideatek_gear_joint_t, hinge_a);
    AT(ideatek_gear_joint_t, hinge_b);
    AT(ideatek_gear_joint_t, ratio);
}

static void show_rack_and_pinion_joint(void)
{
    SIZE(ideatek_rack_and_pinion_joint_t);
    AT(ideatek_rack_and_pinion_joint_t, hinge);
    AT(ideatek_rack_and_pinion_joint_t, slider);
    AT(ideatek_rack_and_pinion_joint_t, ratio);
}

static void show_pulley_joint(void)
{
    SIZE(ideatek_pulley_joint_t);
    AT(ideatek_pulley_joint_t, fixed_a);
    AT(ideatek_pulley_joint_t, fixed_b);
    AT(ideatek_pulley_joint_t, ratio);
    AT(ideatek_pulley_joint_t, min_length);
    AT(ideatek_pulley_joint_t, max_length);
}

static void show_path_rotation(void)
{
    SIZE(ideatek_path_rotation_t);
}

static void show_path_joint(void)
{
    SIZE(ideatek_path_joint_t);
    AT(ideatek_path_joint_t, path);
    AT(ideatek_path_joint_t, fraction);
    AT(ideatek_path_joint_t, max_friction);
    AT(ideatek_path_joint_t, drive);
    AT(ideatek_path_joint_t, rotation);
    AT(ideatek_path_joint_t, looping);
}

static void show_cone_joint(void)
{
    SIZE(ideatek_cone_joint_t);
    AT(ideatek_cone_joint_t, half_angle);
}

static void show_motor_joint(void)
{
    SIZE(ideatek_motor_joint_t);
    AT(ideatek_motor_joint_t, linear_velocity);
    AT(ideatek_motor_joint_t, angular_velocity);
    AT(ideatek_motor_joint_t, max_force);
    AT(ideatek_motor_joint_t, max_torque);
    AT(ideatek_motor_joint_t, linear_hertz);
    AT(ideatek_motor_joint_t, linear_damping);
    AT(ideatek_motor_joint_t, max_spring_force);
    AT(ideatek_motor_joint_t, angular_hertz);
    AT(ideatek_motor_joint_t, angular_damping);
    AT(ideatek_motor_joint_t, max_spring_torque);
}

static void show_wheel_joint(void)
{
    SIZE(ideatek_wheel_joint_t);
    AT(ideatek_wheel_joint_t, lower);
    AT(ideatek_wheel_joint_t, upper);
    AT(ideatek_wheel_joint_t, suspension_hertz);
    AT(ideatek_wheel_joint_t, suspension_damping);
    AT(ideatek_wheel_joint_t, max_steer);
    AT(ideatek_wheel_joint_t, steer_target);
    AT(ideatek_wheel_joint_t, spin);
}

static void show_body_joint_event(void)
{
    SIZE(ideatek_body_joint_event_t);
    AT(ideatek_body_joint_event_t, joint);
    AT(ideatek_body_joint_event_t, force);
    AT(ideatek_body_joint_event_t, torque);
    AT(ideatek_body_joint_event_t, order);
}

static void show_body_joint_state(void)
{
    SIZE(ideatek_body_joint_state_t);
    AT(ideatek_body_joint_state_t, force);
    AT(ideatek_body_joint_state_t, torque);
}

static void show_articulation_joint(void)
{
    SIZE(ideatek_articulation_joint_t);
}

static void show_articulation_flags(void)
{
    SIZE(ideatek_articulation_flags_t);
}

static void show_articulation(void)
{
    SIZE(ideatek_articulation_t);
    AT(ideatek_articulation_t, sleep_threshold);
    AT(ideatek_articulation_t, velocity_steps);
    AT(ideatek_articulation_t, position_steps);
    AT(ideatek_articulation_t, flags);
}

static void show_articulation_link(void)
{
    SIZE(ideatek_articulation_link_t);
    AT(ideatek_articulation_link_t, articulation);
    AT(ideatek_articulation_link_t, parent);
    AT(ideatek_articulation_link_t, limits);
    AT(ideatek_articulation_link_t, drives);
    AT(ideatek_articulation_link_t, friction);
    AT(ideatek_articulation_link_t, max_joint_velocity);
    AT(ideatek_articulation_link_t, motions);
    AT(ideatek_articulation_link_t, joint);
}

static void show_articulation_state(void)
{
    SIZE(ideatek_articulation_state_t);
    AT(ideatek_articulation_state_t, positions);
    AT(ideatek_articulation_state_t, velocities);
    AT(ideatek_articulation_state_t, count);
}

static void show_articulation_command(void)
{
    SIZE(ideatek_articulation_command_t);
    AT(ideatek_articulation_command_t, positions);
    AT(ideatek_articulation_command_t, velocities);
    AT(ideatek_articulation_command_t, count);
}

static void show_ground_state(void)
{
    SIZE(ideatek_ground_state_t);
}

static void show_non_walkable(void)
{
    SIZE(ideatek_non_walkable_t);
}

static void show_character_flags(void)
{
    SIZE(ideatek_character_flags_t);
}

static void show_character_collision(void)
{
    SIZE(ideatek_character_collision_t);
}

static void show_character(void)
{
    SIZE(ideatek_character_t);
    AT(ideatek_character_t, up);
    AT(ideatek_character_t, max_slope);
    AT(ideatek_character_t, step_up);
    AT(ideatek_character_t, stick_down);
    AT(ideatek_character_t, min_step_forward);
    AT(ideatek_character_t, padding);
    AT(ideatek_character_t, predictive_distance);
    AT(ideatek_character_t, mass);
    AT(ideatek_character_t, max_strength);
    AT(ideatek_character_t, non_walkable);
    AT(ideatek_character_t, flags);
}

static void show_character_move(void)
{
    SIZE(ideatek_character_move_t);
    AT(ideatek_character_move_t, velocity);
}

static void show_character_state(void)
{
    SIZE(ideatek_character_state_t);
    AT(ideatek_character_state_t, ground);
    AT(ideatek_character_state_t, ground_material);
    AT(ideatek_character_state_t, ground_normal);
    AT(ideatek_character_state_t, ground_velocity);
    AT(ideatek_character_state_t, ground_state);
    AT(ideatek_character_state_t, collision);
}

static void show_snapshot_action(void)
{
    SIZE(ideatek_snapshot_action_t);
}

static void show_snapshot_status(void)
{
    SIZE(ideatek_snapshot_status_t);
}

static void show_physics_snapshot_request(void)
{
    SIZE(ideatek_physics_snapshot_request_t);
    AT(ideatek_physics_snapshot_request_t, categories);
    AT(ideatek_physics_snapshot_request_t, data);
    AT(ideatek_physics_snapshot_request_t, action);
}

static void show_physics_snapshot(void)
{
    SIZE(ideatek_physics_snapshot_t);
    AT(ideatek_physics_snapshot_t, data);
    AT(ideatek_physics_snapshot_t, status);
}

static void show_vehicle_kind(void)
{
    SIZE(ideatek_vehicle_kind_t);
}

static void show_wheel_test(void)
{
    SIZE(ideatek_wheel_test_t);
}

static void show_transmission_mode(void)
{
    SIZE(ideatek_transmission_mode_t);
}

static void show_vehicle(void)
{
    SIZE(ideatek_vehicle_t);
    AT(ideatek_vehicle_t, max_drive_torque);
    AT(ideatek_vehicle_t, max_pitch_roll);
    AT(ideatek_vehicle_t, kind);
    AT(ideatek_vehicle_t, test);
}

static void show_vehicle_engine(void)
{
    SIZE(ideatek_vehicle_engine_t);
    AT(ideatek_vehicle_engine_t, torque_curve);
    AT(ideatek_vehicle_engine_t, max_torque);
    AT(ideatek_vehicle_engine_t, min_rpm);
    AT(ideatek_vehicle_engine_t, max_rpm);
    AT(ideatek_vehicle_engine_t, inertia);
    AT(ideatek_vehicle_engine_t, damping);
}

static void show_vehicle_transmission(void)
{
    SIZE(ideatek_vehicle_transmission_t);
    AT(ideatek_vehicle_transmission_t, gears);
    AT(ideatek_vehicle_transmission_t, reverse);
    AT(ideatek_vehicle_transmission_t, shift_up_rpm);
    AT(ideatek_vehicle_transmission_t, shift_down_rpm);
    AT(ideatek_vehicle_transmission_t, switch_time);
    AT(ideatek_vehicle_transmission_t, clutch_release_time);
    AT(ideatek_vehicle_transmission_t, switch_latency);
    AT(ideatek_vehicle_transmission_t, clutch_strength);
    AT(ideatek_vehicle_transmission_t, mode);
}

static void show_vehicle_differential(void)
{
    SIZE(ideatek_vehicle_differential_t);
    AT(ideatek_vehicle_differential_t, vehicle);
    AT(ideatek_vehicle_differential_t, left);
    AT(ideatek_vehicle_differential_t, right);
    AT(ideatek_vehicle_differential_t, ratio);
    AT(ideatek_vehicle_differential_t, split);
    AT(ideatek_vehicle_differential_t, limited_slip);
    AT(ideatek_vehicle_differential_t, engine_torque_ratio);
}

static void show_anti_roll_bar(void)
{
    SIZE(ideatek_anti_roll_bar_t);
    AT(ideatek_anti_roll_bar_t, vehicle);
    AT(ideatek_anti_roll_bar_t, left);
    AT(ideatek_anti_roll_bar_t, right);
    AT(ideatek_anti_roll_bar_t, stiffness);
}

static void show_wheel_flags(void)
{
    SIZE(ideatek_wheel_flags_t);
}

static void show_wheel(void)
{
    SIZE(ideatek_wheel_t);
    AT(ideatek_wheel_t, vehicle);
    AT(ideatek_wheel_t, position);
    AT(ideatek_wheel_t, suspension_direction);
    AT(ideatek_wheel_t, steering_axis);
    AT(ideatek_wheel_t, radius);
    AT(ideatek_wheel_t, width);
    AT(ideatek_wheel_t, suspension_min);
    AT(ideatek_wheel_t, suspension_max);
    AT(ideatek_wheel_t, spring_hertz);
    AT(ideatek_wheel_t, spring_damping);
    AT(ideatek_wheel_t, max_steer);
    AT(ideatek_wheel_t, max_brake_torque);
    AT(ideatek_wheel_t, max_hand_brake_torque);
    AT(ideatek_wheel_t, wheel_up);
    AT(ideatek_wheel_t, wheel_forward);
    AT(ideatek_wheel_t, longitudinal_friction);
    AT(ideatek_wheel_t, lateral_friction);
    AT(ideatek_wheel_t, flags);
    AT(ideatek_wheel_t, track);
}

static void show_vehicle_input(void)
{
    SIZE(ideatek_vehicle_input_t);
    AT(ideatek_vehicle_input_t, forward);
    AT(ideatek_vehicle_input_t, right);
    AT(ideatek_vehicle_input_t, brake);
    AT(ideatek_vehicle_input_t, hand_brake);
    AT(ideatek_vehicle_input_t, clutch);
    AT(ideatek_vehicle_input_t, left_ratio);
    AT(ideatek_vehicle_input_t, right_ratio);
    AT(ideatek_vehicle_input_t, gear);
}

static void show_wheel_state(void)
{
    SIZE(ideatek_wheel_state_t);
    AT(ideatek_wheel_state_t, steer_angle);
    AT(ideatek_wheel_state_t, angle);
    AT(ideatek_wheel_state_t, angular_velocity);
    AT(ideatek_wheel_state_t, suspension_length);
    AT(ideatek_wheel_state_t, longitudinal_slip);
    AT(ideatek_wheel_state_t, lateral_slip);
    AT(ideatek_wheel_state_t, contact_body);
    AT(ideatek_wheel_state_t, contact_material);
    AT(ideatek_wheel_state_t, contact);
    AT(ideatek_wheel_state_t, hit_hard_point);
}

static void show_vehicle_state(void)
{
    SIZE(ideatek_vehicle_state_t);
    AT(ideatek_vehicle_state_t, rpm);
    AT(ideatek_vehicle_state_t, clutch);
    AT(ideatek_vehicle_state_t, gear);
}

static void show_pose_drive(void)
{
    SIZE(ideatek_pose_drive_t);
    AT(ideatek_pose_drive_t, position_strength);
    AT(ideatek_pose_drive_t, orientation_strength);
}

static void show_destructible(void)
{
    SIZE(ideatek_destructible_t);
    AT(ideatek_destructible_t, interior);
    AT(ideatek_destructible_t, asset);
}

static void show_destructible_asset(void)
{
    SIZE(ideatek_destructible_asset_t);
    AT(ideatek_destructible_asset_t, chunks);
    AT(ideatek_destructible_asset_t, bonds);
    AT(ideatek_destructible_asset_t, mesh);
    AT(ideatek_destructible_asset_t, chunk_count);
    AT(ideatek_destructible_asset_t, bond_count);
    AT(ideatek_destructible_asset_t, revision);
}

static void show_damage_shape(void)
{
    SIZE(ideatek_damage_shape_t);
}

static void show_damage(void)
{
    SIZE(ideatek_damage_t);
    AT(ideatek_damage_t, point);
    AT(ideatek_damage_t, point_b);
    AT(ideatek_damage_t, point_c);
    AT(ideatek_damage_t, normal);
    AT(ideatek_damage_t, min_radius);
    AT(ideatek_damage_t, radius);
    AT(ideatek_damage_t, pressure);
    AT(ideatek_damage_t, order);
    AT(ideatek_damage_t, shape);
}

static void show_break_event(void)
{
    SIZE(ideatek_break_event_t);
    AT(ideatek_break_event_t, body);
    AT(ideatek_break_event_t, piece);
    AT(ideatek_break_event_t, point);
    AT(ideatek_break_event_t, excess_force);
    AT(ideatek_break_event_t, chunks);
    AT(ideatek_break_event_t, order);
}

static void show_hidden_joint(void)
{
    SIZE(ideatek_hidden_joint_t);
    AT(ideatek_hidden_joint_t, cap);
}

static void show_cut(void)
{
    SIZE(ideatek_cut_t);
    AT(ideatek_cut_t, drawn);
    AT(ideatek_cut_t, point);
    AT(ideatek_cut_t, normal);
    AT(ideatek_cut_t, order);
}

static void show_cut_event(void)
{
    SIZE(ideatek_cut_event_t);
    AT(ideatek_cut_event_t, drawn);
    AT(ideatek_cut_event_t, piece);
    AT(ideatek_cut_event_t, order);
}

static void show_bend(void)
{
    SIZE(ideatek_bend_t);
}

static void show_lra(void)
{
    SIZE(ideatek_lra_t);
}

static void show_soft_body(void)
{
    SIZE(ideatek_soft_body_t);
    AT(ideatek_soft_body_t, source);
    AT(ideatek_soft_body_t, categories);
    AT(ideatek_soft_body_t, touches);
    AT(ideatek_soft_body_t, edge_compliance);
    AT(ideatek_soft_body_t, shear_compliance);
    AT(ideatek_soft_body_t, bend_compliance);
    AT(ideatek_soft_body_t, vertex_mass);
    AT(ideatek_soft_body_t, linear_damping);
    AT(ideatek_soft_body_t, friction);
    AT(ideatek_soft_body_t, gravity_scale);
    AT(ideatek_soft_body_t, vertex_radius);
    AT(ideatek_soft_body_t, pressure);
    AT(ideatek_soft_body_t, restitution);
    AT(ideatek_soft_body_t, volume_compliance);
    AT(ideatek_soft_body_t, lra_scale);
    AT(ideatek_soft_body_t, skin_distance_scale);
    AT(ideatek_soft_body_t, iterations);
    AT(ideatek_soft_body_t, bend);
    AT(ideatek_soft_body_t, lra);
    AT(ideatek_soft_body_t, double_sided);
}

static void show_soft_body_attachment(void)
{
    SIZE(ideatek_soft_body_attachment_t);
    AT(ideatek_soft_body_attachment_t, soft_body);
    AT(ideatek_soft_body_attachment_t, body);
    AT(ideatek_soft_body_attachment_t, offset);
    AT(ideatek_soft_body_attachment_t, vertex);
}

static void show_soft_body_reset(void)
{
    SIZE(ideatek_soft_body_reset_t);
    AT(ideatek_soft_body_reset_t, body);
    AT(ideatek_soft_body_reset_t, order);
}

static void show_rope_flags(void)
{
    SIZE(ideatek_rope_flags_t);
}

static void show_rope(void)
{
    SIZE(ideatek_rope_t);
    AT(ideatek_rope_t, start);
    AT(ideatek_rope_t, end);
    AT(ideatek_rope_t, categories);
    AT(ideatek_rope_t, touches);
    AT(ideatek_rope_t, start_offset);
    AT(ideatek_rope_t, end_offset);
    AT(ideatek_rope_t, length);
    AT(ideatek_rope_t, width);
    AT(ideatek_rope_t, gravity_scale);
    AT(ideatek_rope_t, friction);
    AT(ideatek_rope_t, stretch_compliance);
    AT(ideatek_rope_t, bend_compliance);
    AT(ideatek_rope_t, segments);
    AT(ideatek_rope_t, sides);
    AT(ideatek_rope_t, flags);
}

static void show_water(void)
{
    SIZE(ideatek_water_t);
    AT(ideatek_water_t, flow);
    AT(ideatek_water_t, density);
}

static void show_wave(void)
{
    SIZE(ideatek_wave_t);
    AT(ideatek_wave_t, water);
    AT(ideatek_wave_t, direction);
    AT(ideatek_wave_t, wavelength);
    AT(ideatek_wave_t, amplitude);
    AT(ideatek_wave_t, steepness);
}

static void show_water_sample(void)
{
    SIZE(ideatek_water_sample_t);
    AT(ideatek_water_sample_t, query);
    AT(ideatek_water_sample_t, water);
    AT(ideatek_water_sample_t, point);
    AT(ideatek_water_sample_t, normal);
    AT(ideatek_water_sample_t, velocity);
    AT(ideatek_water_sample_t, depth);
    AT(ideatek_water_sample_t, order);
}

static void show_buoyancy(void)
{
    SIZE(ideatek_buoyancy_t);
    AT(ideatek_buoyancy_t, linear_drag);
    AT(ideatek_buoyancy_t, angular_drag);
}

static void show_liquid_flags(void)
{
    SIZE(ideatek_liquid_flags_t);
}

static void show_liquid_technique(void)
{
    SIZE(ideatek_liquid_technique_t);
}

static void show_emitter_shape(void)
{
    SIZE(ideatek_emitter_shape_t);
}

static void show_liquid(void)
{
    SIZE(ideatek_liquid_t);
    AT(ideatek_liquid_t, categories);
    AT(ideatek_liquid_t, touches);
    AT(ideatek_liquid_t, density);
    AT(ideatek_liquid_t, viscosity);
    AT(ideatek_liquid_t, resolution);
    AT(ideatek_liquid_t, surface_tension);
    AT(ideatek_liquid_t, cohesion);
    AT(ideatek_liquid_t, adhesion);
    AT(ideatek_liquid_t, vorticity);
    AT(ideatek_liquid_t, max_particles);
    AT(ideatek_liquid_t, phase);
    AT(ideatek_liquid_t, flags);
    AT(ideatek_liquid_t, technique);
}

static void show_liquid_emitter(void)
{
    SIZE(ideatek_liquid_emitter_t);
    AT(ideatek_liquid_emitter_t, liquid);
    AT(ideatek_liquid_emitter_t, velocity);
    AT(ideatek_liquid_emitter_t, rate);
    AT(ideatek_liquid_emitter_t, size);
    AT(ideatek_liquid_emitter_t, start);
    AT(ideatek_liquid_emitter_t, end);
    AT(ideatek_liquid_emitter_t, jitter);
    AT(ideatek_liquid_emitter_t, shape);
}

static void show_liquid_particles(void)
{
    SIZE(ideatek_liquid_particles_t);
    AT(ideatek_liquid_particles_t, positions);
    AT(ideatek_liquid_particles_t, velocities);
    AT(ideatek_liquid_particles_t, count);
}

static void show_liquid_foam(void)
{
    SIZE(ideatek_liquid_foam_t);
    AT(ideatek_liquid_foam_t, positions);
    AT(ideatek_liquid_foam_t, lifetimes);
    AT(ideatek_liquid_foam_t, count);
}

static void show_smoke_flags(void)
{
    SIZE(ideatek_smoke_flags_t);
}

static void show_smoke_volume(void)
{
    SIZE(ideatek_smoke_volume_t);
    AT(ideatek_smoke_volume_t, cell_size);
    AT(ideatek_smoke_volume_t, steps_per_second);
    AT(ideatek_smoke_volume_t, buoyancy_per_temp);
    AT(ideatek_smoke_volume_t, buoyancy_per_smoke);
    AT(ideatek_smoke_volume_t, cooling);
    AT(ideatek_smoke_volume_t, vorticity);
    AT(ideatek_smoke_volume_t, ignition_temp);
    AT(ideatek_smoke_volume_t, burn_per_temp);
    AT(ideatek_smoke_volume_t, fuel_per_burn);
    AT(ideatek_smoke_volume_t, temp_per_burn);
    AT(ideatek_smoke_volume_t, smoke_per_burn);
    AT(ideatek_smoke_volume_t, flags);
}

static void show_smoke_emitter(void)
{
    SIZE(ideatek_smoke_emitter_t);
    AT(ideatek_smoke_emitter_t, volume);
    AT(ideatek_smoke_emitter_t, velocity);
    AT(ideatek_smoke_emitter_t, size);
    AT(ideatek_smoke_emitter_t, temperature);
    AT(ideatek_smoke_emitter_t, fuel);
    AT(ideatek_smoke_emitter_t, smoke);
    AT(ideatek_smoke_emitter_t, couple_rate);
    AT(ideatek_smoke_emitter_t, shape);
}

static void show_smoke_grid(void)
{
    SIZE(ideatek_smoke_grid_t);
    AT(ideatek_smoke_grid_t, texture);
}

static void show_lag_history(void)
{
    SIZE(ideatek_lag_history_t);
    AT(ideatek_lag_history_t, seconds);
}

static void show_field_shape(void)
{
    SIZE(ideatek_field_shape_t);
}

static void show_force_kind(void)
{
    SIZE(ideatek_force_kind_t);
}

static void show_field_target(void)
{
    SIZE(ideatek_field_target_t);
}

static void show_gravity_mode(void)
{
    SIZE(ideatek_gravity_mode_t);
}

static void show_wind(void)
{
    SIZE(ideatek_wind_t);
    AT(ideatek_wind_t, size);
    AT(ideatek_wind_t, inner);
    AT(ideatek_wind_t, speed);
    AT(ideatek_wind_t, gust_min);
    AT(ideatek_wind_t, gust_max);
    AT(ideatek_wind_t, gust_frequency);
    AT(ideatek_wind_t, falloff);
    AT(ideatek_wind_t, shape);
}

static void show_force_field(void)
{
    SIZE(ideatek_force_field_t);
    AT(ideatek_force_field_t, texture);
    AT(ideatek_force_field_t, size);
    AT(ideatek_force_field_t, inner);
    AT(ideatek_force_field_t, acceleration);
    AT(ideatek_force_field_t, frequency);
    AT(ideatek_force_field_t, falloff);
    AT(ideatek_force_field_t, kind);
    AT(ideatek_force_field_t, shape);
    AT(ideatek_force_field_t, target);
}

static void show_gravity_area(void)
{
    SIZE(ideatek_gravity_area_t);
    AT(ideatek_gravity_area_t, size);
    AT(ideatek_gravity_area_t, gravity);
    AT(ideatek_gravity_area_t, inner);
    AT(ideatek_gravity_area_t, falloff);
    AT(ideatek_gravity_area_t, priority);
    AT(ideatek_gravity_area_t, mode);
    AT(ideatek_gravity_area_t, shape);
    AT(ideatek_gravity_area_t, point);
}

static void show_descriptors(void)
{
    const char *descriptors[] = {
        ideatek_body_kind_t_desc, ideatek_motion_quality_t_desc,
        ideatek_mass_mode_t_desc, ideatek_locked_t_desc,
        ideatek_body_flags_t_desc, ideatek_body_t_desc,
        ideatek_kinematic_target_t_desc, ideatek_wake_event_t_desc,
        ideatek_motion_t_desc, ideatek_push_t_desc, ideatek_shape_kind_t_desc,
        ideatek_shape_flags_t_desc, ideatek_shape_t_desc,
        ideatek_shape_materials_t_desc, ideatek_combine_t_desc,
        ideatek_physics_material_t_desc, ideatek_touch_kind_t_desc,
        ideatek_sensor_event_t_desc, ideatek_contact_event_t_desc,
        ideatek_hit_event_t_desc, ideatek_query_kind_t_desc,
        ideatek_query_mode_t_desc, ideatek_query_t_desc,
        ideatek_query_hit_t_desc, ideatek_joint_flags_t_desc,
        ideatek_body_joint_t_desc, ideatek_joint_features_t_desc,
        ideatek_drive_mode_t_desc, ideatek_spring_mode_t_desc,
        ideatek_joint_drive_t_desc, ideatek_hinge_joint_t_desc,
        ideatek_slider_joint_t_desc, ideatek_distance_joint_t_desc,
        ideatek_swing_twist_joint_t_desc, ideatek_six_dof_joint_t_desc,
        ideatek_gear_joint_t_desc, ideatek_rack_and_pinion_joint_t_desc,
        ideatek_pulley_joint_t_desc, ideatek_path_rotation_t_desc,
        ideatek_path_joint_t_desc, ideatek_cone_joint_t_desc,
        ideatek_motor_joint_t_desc, ideatek_wheel_joint_t_desc,
        ideatek_body_joint_event_t_desc, ideatek_body_joint_state_t_desc,
        ideatek_articulation_joint_t_desc, ideatek_articulation_flags_t_desc,
        ideatek_articulation_t_desc, ideatek_articulation_link_t_desc,
        ideatek_articulation_state_t_desc, ideatek_articulation_command_t_desc,
        ideatek_ground_state_t_desc, ideatek_non_walkable_t_desc,
        ideatek_character_flags_t_desc, ideatek_character_collision_t_desc,
        ideatek_character_t_desc, ideatek_character_move_t_desc,
        ideatek_character_state_t_desc, ideatek_snapshot_action_t_desc,
        ideatek_snapshot_status_t_desc, ideatek_physics_snapshot_request_t_desc,
        ideatek_physics_snapshot_t_desc, ideatek_vehicle_kind_t_desc,
        ideatek_wheel_test_t_desc, ideatek_transmission_mode_t_desc,
        ideatek_vehicle_t_desc, ideatek_vehicle_engine_t_desc,
        ideatek_vehicle_transmission_t_desc,
        ideatek_vehicle_differential_t_desc, ideatek_anti_roll_bar_t_desc,
        ideatek_wheel_flags_t_desc, ideatek_wheel_t_desc,
        ideatek_vehicle_input_t_desc, ideatek_wheel_state_t_desc,
        ideatek_vehicle_state_t_desc, ideatek_pose_drive_t_desc,
        ideatek_destructible_t_desc, ideatek_destructible_asset_t_desc,
        ideatek_damage_shape_t_desc, ideatek_damage_t_desc,
        ideatek_break_event_t_desc, ideatek_hidden_joint_t_desc,
        ideatek_cut_t_desc, ideatek_cut_event_t_desc, ideatek_bend_t_desc,
        ideatek_lra_t_desc, ideatek_soft_body_t_desc,
        ideatek_soft_body_attachment_t_desc, ideatek_soft_body_reset_t_desc,
        ideatek_rope_flags_t_desc, ideatek_rope_t_desc, ideatek_water_t_desc,
        ideatek_wave_t_desc, ideatek_water_sample_t_desc,
        ideatek_buoyancy_t_desc, ideatek_liquid_flags_t_desc,
        ideatek_liquid_technique_t_desc, ideatek_emitter_shape_t_desc,
        ideatek_liquid_t_desc, ideatek_liquid_emitter_t_desc,
        ideatek_liquid_particles_t_desc, ideatek_liquid_foam_t_desc,
        ideatek_smoke_flags_t_desc, ideatek_smoke_volume_t_desc,
        ideatek_smoke_emitter_t_desc, ideatek_smoke_grid_t_desc,
        ideatek_lag_history_t_desc, ideatek_field_shape_t_desc,
        ideatek_force_kind_t_desc, ideatek_field_target_t_desc,
        ideatek_gravity_mode_t_desc, ideatek_wind_t_desc,
        ideatek_force_field_t_desc, ideatek_gravity_area_t_desc };

    for (size_t i = 0; i < sizeof(descriptors) / sizeof(descriptors[0]); i++)
        printf("%s\n", descriptors[i]);
}

static void show_texts(void)
{
    const char *texts[] = {
        TEXT_OF(IDEATEK_BODY_KIND_VALUES),
        TEXT_OF(IDEATEK_MOTION_QUALITY_VALUES),
        TEXT_OF(IDEATEK_MASS_MODE_VALUES), TEXT_OF(IDEATEK_LOCKED_VALUES),
        TEXT_OF(IDEATEK_BODY_FLAGS_VALUES), TEXT_OF(IDEATEK_BODY_FIELDS),
        TEXT_OF(IDEATEK_KINEMATIC_TARGET_FIELDS),
        TEXT_OF(IDEATEK_WAKE_EVENT_FIELDS), TEXT_OF(IDEATEK_MOTION_FIELDS),
        TEXT_OF(IDEATEK_PUSH_FIELDS), TEXT_OF(IDEATEK_SHAPE_KIND_VALUES),
        TEXT_OF(IDEATEK_SHAPE_FLAGS_VALUES), TEXT_OF(IDEATEK_SHAPE_FIELDS),
        TEXT_OF(IDEATEK_SHAPE_MATERIALS_FIELDS),
        TEXT_OF(IDEATEK_COMBINE_VALUES),
        TEXT_OF(IDEATEK_PHYSICS_MATERIAL_FIELDS),
        TEXT_OF(IDEATEK_TOUCH_KIND_VALUES),
        TEXT_OF(IDEATEK_SENSOR_EVENT_FIELDS),
        TEXT_OF(IDEATEK_CONTACT_EVENT_FIELDS),
        TEXT_OF(IDEATEK_HIT_EVENT_FIELDS), TEXT_OF(IDEATEK_QUERY_KIND_VALUES),
        TEXT_OF(IDEATEK_QUERY_MODE_VALUES), TEXT_OF(IDEATEK_QUERY_FIELDS),
        TEXT_OF(IDEATEK_QUERY_HIT_FIELDS), TEXT_OF(IDEATEK_JOINT_FLAGS_VALUES),
        TEXT_OF(IDEATEK_BODY_JOINT_FIELDS),
        TEXT_OF(IDEATEK_JOINT_FEATURES_VALUES),
        TEXT_OF(IDEATEK_DRIVE_MODE_VALUES), TEXT_OF(IDEATEK_SPRING_MODE_VALUES),
        TEXT_OF(IDEATEK_JOINT_DRIVE_FIELDS),
        TEXT_OF(IDEATEK_HINGE_JOINT_FIELDS),
        TEXT_OF(IDEATEK_SLIDER_JOINT_FIELDS),
        TEXT_OF(IDEATEK_DISTANCE_JOINT_FIELDS),
        TEXT_OF(IDEATEK_SWING_TWIST_JOINT_FIELDS),
        TEXT_OF(IDEATEK_SIX_DOF_JOINT_FIELDS),
        TEXT_OF(IDEATEK_GEAR_JOINT_FIELDS),
        TEXT_OF(IDEATEK_RACK_AND_PINION_JOINT_FIELDS),
        TEXT_OF(IDEATEK_PULLEY_JOINT_FIELDS),
        TEXT_OF(IDEATEK_PATH_ROTATION_VALUES),
        TEXT_OF(IDEATEK_PATH_JOINT_FIELDS), TEXT_OF(IDEATEK_CONE_JOINT_FIELDS),
        TEXT_OF(IDEATEK_MOTOR_JOINT_FIELDS),
        TEXT_OF(IDEATEK_WHEEL_JOINT_FIELDS),
        TEXT_OF(IDEATEK_BODY_JOINT_EVENT_FIELDS),
        TEXT_OF(IDEATEK_BODY_JOINT_STATE_FIELDS),
        TEXT_OF(IDEATEK_ARTICULATION_JOINT_VALUES),
        TEXT_OF(IDEATEK_ARTICULATION_FLAGS_VALUES),
        TEXT_OF(IDEATEK_ARTICULATION_FIELDS),
        TEXT_OF(IDEATEK_ARTICULATION_LINK_FIELDS),
        TEXT_OF(IDEATEK_ARTICULATION_STATE_FIELDS),
        TEXT_OF(IDEATEK_ARTICULATION_COMMAND_FIELDS),
        TEXT_OF(IDEATEK_GROUND_STATE_VALUES),
        TEXT_OF(IDEATEK_NON_WALKABLE_VALUES),
        TEXT_OF(IDEATEK_CHARACTER_FLAGS_VALUES),
        TEXT_OF(IDEATEK_CHARACTER_COLLISION_VALUES),
        TEXT_OF(IDEATEK_CHARACTER_FIELDS),
        TEXT_OF(IDEATEK_CHARACTER_MOVE_FIELDS),
        TEXT_OF(IDEATEK_CHARACTER_STATE_FIELDS),
        TEXT_OF(IDEATEK_SNAPSHOT_ACTION_VALUES),
        TEXT_OF(IDEATEK_SNAPSHOT_STATUS_VALUES),
        TEXT_OF(IDEATEK_PHYSICS_SNAPSHOT_REQUEST_FIELDS),
        TEXT_OF(IDEATEK_PHYSICS_SNAPSHOT_FIELDS),
        TEXT_OF(IDEATEK_VEHICLE_KIND_VALUES),
        TEXT_OF(IDEATEK_WHEEL_TEST_VALUES),
        TEXT_OF(IDEATEK_TRANSMISSION_MODE_VALUES),
        TEXT_OF(IDEATEK_VEHICLE_FIELDS), TEXT_OF(IDEATEK_VEHICLE_ENGINE_FIELDS),
        TEXT_OF(IDEATEK_VEHICLE_TRANSMISSION_FIELDS),
        TEXT_OF(IDEATEK_VEHICLE_DIFFERENTIAL_FIELDS),
        TEXT_OF(IDEATEK_ANTI_ROLL_BAR_FIELDS),
        TEXT_OF(IDEATEK_WHEEL_FLAGS_VALUES), TEXT_OF(IDEATEK_WHEEL_FIELDS),
        TEXT_OF(IDEATEK_VEHICLE_INPUT_FIELDS),
        TEXT_OF(IDEATEK_WHEEL_STATE_FIELDS),
        TEXT_OF(IDEATEK_VEHICLE_STATE_FIELDS),
        TEXT_OF(IDEATEK_POSE_DRIVE_FIELDS),
        TEXT_OF(IDEATEK_DESTRUCTIBLE_FIELDS),
        TEXT_OF(IDEATEK_DESTRUCTIBLE_ASSET_FIELDS),
        TEXT_OF(IDEATEK_DAMAGE_SHAPE_VALUES), TEXT_OF(IDEATEK_DAMAGE_FIELDS),
        TEXT_OF(IDEATEK_BREAK_EVENT_FIELDS),
        TEXT_OF(IDEATEK_HIDDEN_JOINT_FIELDS), TEXT_OF(IDEATEK_CUT_FIELDS),
        TEXT_OF(IDEATEK_CUT_EVENT_FIELDS), TEXT_OF(IDEATEK_BEND_VALUES),
        TEXT_OF(IDEATEK_LRA_VALUES), TEXT_OF(IDEATEK_SOFT_BODY_FIELDS),
        TEXT_OF(IDEATEK_SOFT_BODY_ATTACHMENT_FIELDS),
        TEXT_OF(IDEATEK_SOFT_BODY_RESET_FIELDS),
        TEXT_OF(IDEATEK_ROPE_FLAGS_VALUES), TEXT_OF(IDEATEK_ROPE_FIELDS),
        TEXT_OF(IDEATEK_WATER_FIELDS), TEXT_OF(IDEATEK_WAVE_FIELDS),
        TEXT_OF(IDEATEK_WATER_SAMPLE_FIELDS), TEXT_OF(IDEATEK_BUOYANCY_FIELDS),
        TEXT_OF(IDEATEK_LIQUID_FLAGS_VALUES),
        TEXT_OF(IDEATEK_LIQUID_TECHNIQUE_VALUES),
        TEXT_OF(IDEATEK_EMITTER_SHAPE_VALUES), TEXT_OF(IDEATEK_LIQUID_FIELDS),
        TEXT_OF(IDEATEK_LIQUID_EMITTER_FIELDS),
        TEXT_OF(IDEATEK_LIQUID_PARTICLES_FIELDS),
        TEXT_OF(IDEATEK_LIQUID_FOAM_FIELDS),
        TEXT_OF(IDEATEK_SMOKE_FLAGS_VALUES),
        TEXT_OF(IDEATEK_SMOKE_VOLUME_FIELDS),
        TEXT_OF(IDEATEK_SMOKE_EMITTER_FIELDS),
        TEXT_OF(IDEATEK_SMOKE_GRID_FIELDS), TEXT_OF(IDEATEK_LAG_HISTORY_FIELDS),
        TEXT_OF(IDEATEK_FIELD_SHAPE_VALUES), TEXT_OF(IDEATEK_FORCE_KIND_VALUES),
        TEXT_OF(IDEATEK_FIELD_TARGET_VALUES),
        TEXT_OF(IDEATEK_GRAVITY_MODE_VALUES), TEXT_OF(IDEATEK_WIND_FIELDS),
        TEXT_OF(IDEATEK_FORCE_FIELD_FIELDS),
        TEXT_OF(IDEATEK_GRAVITY_AREA_FIELDS) };

    for (size_t i = 0; i < sizeof(texts) / sizeof(texts[0]); i++)
        printf("%s\n", texts[i]);
}

int main(int argc, char **argv)
{
    static const show_mode_t MODES[] = {
        { "body_kind", show_body_kind },
        { "motion_quality", show_motion_quality },
        { "mass_mode", show_mass_mode }, { "locked", show_locked },
        { "body_flags", show_body_flags }, { "body", show_body },
        { "kinematic_target", show_kinematic_target },
        { "wake_event", show_wake_event }, { "motion", show_motion },
        { "push", show_push }, { "shape_kind", show_shape_kind },
        { "shape_flags", show_shape_flags }, { "shape", show_shape },
        { "shape_materials", show_shape_materials },
        { "combine", show_combine },
        { "physics_material", show_physics_material },
        { "touch_kind", show_touch_kind },
        { "sensor_event", show_sensor_event },
        { "contact_event", show_contact_event },
        { "hit_event", show_hit_event }, { "query_kind", show_query_kind },
        { "query_mode", show_query_mode }, { "query", show_query },
        { "query_hit", show_query_hit }, { "joint_flags", show_joint_flags },
        { "body_joint", show_body_joint },
        { "joint_features", show_joint_features },
        { "drive_mode", show_drive_mode }, { "spring_mode", show_spring_mode },
        { "joint_drive", show_joint_drive },
        { "hinge_joint", show_hinge_joint },
        { "slider_joint", show_slider_joint },
        { "distance_joint", show_distance_joint },
        { "swing_twist_joint", show_swing_twist_joint },
        { "six_dof_joint", show_six_dof_joint },
        { "gear_joint", show_gear_joint },
        { "rack_and_pinion_joint", show_rack_and_pinion_joint },
        { "pulley_joint", show_pulley_joint },
        { "path_rotation", show_path_rotation },
        { "path_joint", show_path_joint }, { "cone_joint", show_cone_joint },
        { "motor_joint", show_motor_joint },
        { "wheel_joint", show_wheel_joint },
        { "body_joint_event", show_body_joint_event },
        { "body_joint_state", show_body_joint_state },
        { "articulation_joint", show_articulation_joint },
        { "articulation_flags", show_articulation_flags },
        { "articulation", show_articulation },
        { "articulation_link", show_articulation_link },
        { "articulation_state", show_articulation_state },
        { "articulation_command", show_articulation_command },
        { "ground_state", show_ground_state },
        { "non_walkable", show_non_walkable },
        { "character_flags", show_character_flags },
        { "character_collision", show_character_collision },
        { "character", show_character },
        { "character_move", show_character_move },
        { "character_state", show_character_state },
        { "snapshot_action", show_snapshot_action },
        { "snapshot_status", show_snapshot_status },
        { "physics_snapshot_request", show_physics_snapshot_request },
        { "physics_snapshot", show_physics_snapshot },
        { "vehicle_kind", show_vehicle_kind },
        { "wheel_test", show_wheel_test },
        { "transmission_mode", show_transmission_mode },
        { "vehicle", show_vehicle }, { "vehicle_engine", show_vehicle_engine },
        { "vehicle_transmission", show_vehicle_transmission },
        { "vehicle_differential", show_vehicle_differential },
        { "anti_roll_bar", show_anti_roll_bar },
        { "wheel_flags", show_wheel_flags }, { "wheel", show_wheel },
        { "vehicle_input", show_vehicle_input },
        { "wheel_state", show_wheel_state },
        { "vehicle_state", show_vehicle_state },
        { "pose_drive", show_pose_drive },
        { "destructible", show_destructible },
        { "destructible_asset", show_destructible_asset },
        { "damage_shape", show_damage_shape }, { "damage", show_damage },
        { "break_event", show_break_event },
        { "hidden_joint", show_hidden_joint }, { "cut", show_cut },
        { "cut_event", show_cut_event }, { "bend", show_bend },
        { "lra", show_lra }, { "soft_body", show_soft_body },
        { "soft_body_attachment", show_soft_body_attachment },
        { "soft_body_reset", show_soft_body_reset },
        { "rope_flags", show_rope_flags }, { "rope", show_rope },
        { "water", show_water }, { "wave", show_wave },
        { "water_sample", show_water_sample }, { "buoyancy", show_buoyancy },
        { "liquid_flags", show_liquid_flags },
        { "liquid_technique", show_liquid_technique },
        { "emitter_shape", show_emitter_shape }, { "liquid", show_liquid },
        { "liquid_emitter", show_liquid_emitter },
        { "liquid_particles", show_liquid_particles },
        { "liquid_foam", show_liquid_foam },
        { "smoke_flags", show_smoke_flags },
        { "smoke_volume", show_smoke_volume },
        { "smoke_emitter", show_smoke_emitter },
        { "smoke_grid", show_smoke_grid }, { "lag_history", show_lag_history },
        { "field_shape", show_field_shape }, { "force_kind", show_force_kind },
        { "field_target", show_field_target },
        { "gravity_mode", show_gravity_mode }, { "wind", show_wind },
        { "force_field", show_force_field },
        { "gravity_area", show_gravity_area },
        { "descriptors", show_descriptors }, { "texts", show_texts }
    };

    for (size_t i = 0; argc > 1 && i < sizeof(MODES) / sizeof(MODES[0]); i++)
        if (!strcmp(argv[1], MODES[i].name)) {
            MODES[i].show();
            return (0);
        }
    printf("usage: layout <mode>\n");
    return (2);
}
