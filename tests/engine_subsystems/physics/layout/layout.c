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
    AT(ideatek_body_t, kind);
    AT(ideatek_body_t, mass_mode);
    AT(ideatek_body_t, locked);
    AT(ideatek_body_t, flags);
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
    AT(ideatek_shape_t, mesh);
    AT(ideatek_shape_t, categories);
    AT(ideatek_shape_t, touches);
    AT(ideatek_shape_t, size);
    AT(ideatek_shape_t, part);
    AT(ideatek_shape_t, kind);
    AT(ideatek_shape_t, flags);
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
}

static void show_query_hit(void)
{
    SIZE(ideatek_query_hit_t);
    AT(ideatek_query_hit_t, query);
    AT(ideatek_query_hit_t, shape);
    AT(ideatek_query_hit_t, point);
    AT(ideatek_query_hit_t, normal);
    AT(ideatek_query_hit_t, fraction);
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

static void show_hinge_joint(void)
{
    SIZE(ideatek_hinge_joint_t);
    AT(ideatek_hinge_joint_t, lower);
    AT(ideatek_hinge_joint_t, upper);
    AT(ideatek_hinge_joint_t, spring_hertz);
    AT(ideatek_hinge_joint_t, spring_damping);
    AT(ideatek_hinge_joint_t, motor_speed);
    AT(ideatek_hinge_joint_t, max_motor_torque);
    AT(ideatek_hinge_joint_t, features);
}

static void show_slider_joint(void)
{
    SIZE(ideatek_slider_joint_t);
    AT(ideatek_slider_joint_t, lower);
    AT(ideatek_slider_joint_t, upper);
    AT(ideatek_slider_joint_t, spring_hertz);
    AT(ideatek_slider_joint_t, spring_damping);
    AT(ideatek_slider_joint_t, motor_speed);
    AT(ideatek_slider_joint_t, max_motor_force);
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
}

static void show_body_joint_event(void)
{
    SIZE(ideatek_body_joint_event_t);
    AT(ideatek_body_joint_event_t, joint);
    AT(ideatek_body_joint_event_t, force);
    AT(ideatek_body_joint_event_t, torque);
    AT(ideatek_body_joint_event_t, order);
}

static void show_vehicle(void)
{
    SIZE(ideatek_vehicle_t);
    AT(ideatek_vehicle_t, max_drive_torque);
    AT(ideatek_vehicle_t, max_pitch_roll);
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
    AT(ideatek_wheel_t, flags);
}

static void show_vehicle_input(void)
{
    SIZE(ideatek_vehicle_input_t);
    AT(ideatek_vehicle_input_t, forward);
    AT(ideatek_vehicle_input_t, right);
    AT(ideatek_vehicle_input_t, brake);
    AT(ideatek_vehicle_input_t, hand_brake);
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
    AT(ideatek_wheel_state_t, contact);
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
}

static void show_damage(void)
{
    SIZE(ideatek_damage_t);
    AT(ideatek_damage_t, point);
    AT(ideatek_damage_t, radius);
    AT(ideatek_damage_t, pressure);
    AT(ideatek_damage_t, order);
}

static void show_break_event(void)
{
    SIZE(ideatek_break_event_t);
    AT(ideatek_break_event_t, body);
    AT(ideatek_break_event_t, piece);
    AT(ideatek_break_event_t, point);
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

static void show_liquid(void)
{
    SIZE(ideatek_liquid_t);
    AT(ideatek_liquid_t, categories);
    AT(ideatek_liquid_t, touches);
    AT(ideatek_liquid_t, density);
    AT(ideatek_liquid_t, viscosity);
    AT(ideatek_liquid_t, resolution);
    AT(ideatek_liquid_t, flags);
}

static void show_liquid_emitter(void)
{
    SIZE(ideatek_liquid_emitter_t);
    AT(ideatek_liquid_emitter_t, liquid);
    AT(ideatek_liquid_emitter_t, velocity);
    AT(ideatek_liquid_emitter_t, rate);
}

static void show_lag_history(void)
{
    SIZE(ideatek_lag_history_t);
    AT(ideatek_lag_history_t, seconds);
}

static void show_descriptors(void)
{
    const char *descriptors[] = {
        ideatek_body_kind_t_desc, ideatek_mass_mode_t_desc,
        ideatek_locked_t_desc, ideatek_body_flags_t_desc, ideatek_body_t_desc,
        ideatek_motion_t_desc, ideatek_push_t_desc, ideatek_shape_kind_t_desc,
        ideatek_shape_flags_t_desc, ideatek_shape_t_desc,
        ideatek_combine_t_desc, ideatek_physics_material_t_desc,
        ideatek_touch_kind_t_desc, ideatek_sensor_event_t_desc,
        ideatek_contact_event_t_desc, ideatek_hit_event_t_desc,
        ideatek_query_kind_t_desc, ideatek_query_mode_t_desc,
        ideatek_query_t_desc, ideatek_query_hit_t_desc,
        ideatek_joint_flags_t_desc, ideatek_body_joint_t_desc,
        ideatek_joint_features_t_desc, ideatek_hinge_joint_t_desc,
        ideatek_slider_joint_t_desc, ideatek_distance_joint_t_desc,
        ideatek_swing_twist_joint_t_desc, ideatek_six_dof_joint_t_desc,
        ideatek_body_joint_event_t_desc, ideatek_vehicle_t_desc,
        ideatek_wheel_flags_t_desc, ideatek_wheel_t_desc,
        ideatek_vehicle_input_t_desc, ideatek_wheel_state_t_desc,
        ideatek_pose_drive_t_desc, ideatek_destructible_t_desc,
        ideatek_damage_t_desc, ideatek_break_event_t_desc,
        ideatek_hidden_joint_t_desc, ideatek_cut_t_desc,
        ideatek_cut_event_t_desc, ideatek_soft_body_t_desc,
        ideatek_soft_body_reset_t_desc, ideatek_rope_flags_t_desc,
        ideatek_rope_t_desc, ideatek_water_t_desc, ideatek_wave_t_desc,
        ideatek_water_sample_t_desc, ideatek_buoyancy_t_desc,
        ideatek_liquid_flags_t_desc, ideatek_liquid_t_desc,
        ideatek_liquid_emitter_t_desc, ideatek_lag_history_t_desc };

    for (size_t i = 0; i < sizeof(descriptors) / sizeof(descriptors[0]); i++)
        printf("%s\n", descriptors[i]);
}

static void show_texts(void)
{
    const char *texts[] = {
        TEXT_OF(IDEATEK_BODY_KIND_VALUES), TEXT_OF(IDEATEK_MASS_MODE_VALUES),
        TEXT_OF(IDEATEK_LOCKED_VALUES), TEXT_OF(IDEATEK_BODY_FLAGS_VALUES),
        TEXT_OF(IDEATEK_BODY_FIELDS), TEXT_OF(IDEATEK_MOTION_FIELDS),
        TEXT_OF(IDEATEK_PUSH_FIELDS), TEXT_OF(IDEATEK_SHAPE_KIND_VALUES),
        TEXT_OF(IDEATEK_SHAPE_FLAGS_VALUES), TEXT_OF(IDEATEK_SHAPE_FIELDS),
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
        TEXT_OF(IDEATEK_HINGE_JOINT_FIELDS),
        TEXT_OF(IDEATEK_SLIDER_JOINT_FIELDS),
        TEXT_OF(IDEATEK_DISTANCE_JOINT_FIELDS),
        TEXT_OF(IDEATEK_SWING_TWIST_JOINT_FIELDS),
        TEXT_OF(IDEATEK_SIX_DOF_JOINT_FIELDS),
        TEXT_OF(IDEATEK_BODY_JOINT_EVENT_FIELDS),
        TEXT_OF(IDEATEK_VEHICLE_FIELDS), TEXT_OF(IDEATEK_WHEEL_FLAGS_VALUES),
        TEXT_OF(IDEATEK_WHEEL_FIELDS), TEXT_OF(IDEATEK_VEHICLE_INPUT_FIELDS),
        TEXT_OF(IDEATEK_WHEEL_STATE_FIELDS), TEXT_OF(IDEATEK_POSE_DRIVE_FIELDS),
        TEXT_OF(IDEATEK_DESTRUCTIBLE_FIELDS), TEXT_OF(IDEATEK_DAMAGE_FIELDS),
        TEXT_OF(IDEATEK_BREAK_EVENT_FIELDS),
        TEXT_OF(IDEATEK_HIDDEN_JOINT_FIELDS), TEXT_OF(IDEATEK_CUT_FIELDS),
        TEXT_OF(IDEATEK_CUT_EVENT_FIELDS), TEXT_OF(IDEATEK_SOFT_BODY_FIELDS),
        TEXT_OF(IDEATEK_SOFT_BODY_RESET_FIELDS),
        TEXT_OF(IDEATEK_ROPE_FLAGS_VALUES), TEXT_OF(IDEATEK_ROPE_FIELDS),
        TEXT_OF(IDEATEK_WATER_FIELDS), TEXT_OF(IDEATEK_WAVE_FIELDS),
        TEXT_OF(IDEATEK_WATER_SAMPLE_FIELDS), TEXT_OF(IDEATEK_BUOYANCY_FIELDS),
        TEXT_OF(IDEATEK_LIQUID_FLAGS_VALUES), TEXT_OF(IDEATEK_LIQUID_FIELDS),
        TEXT_OF(IDEATEK_LIQUID_EMITTER_FIELDS),
        TEXT_OF(IDEATEK_LAG_HISTORY_FIELDS) };

    for (size_t i = 0; i < sizeof(texts) / sizeof(texts[0]); i++)
        printf("%s\n", texts[i]);
}

int main(int argc, char **argv)
{
    static const show_mode_t MODES[] = {
        { "body_kind", show_body_kind }, { "mass_mode", show_mass_mode },
        { "locked", show_locked }, { "body_flags", show_body_flags },
        { "body", show_body }, { "motion", show_motion }, { "push", show_push },
        { "shape_kind", show_shape_kind }, { "shape_flags", show_shape_flags },
        { "shape", show_shape }, { "combine", show_combine },
        { "physics_material", show_physics_material },
        { "touch_kind", show_touch_kind },
        { "sensor_event", show_sensor_event },
        { "contact_event", show_contact_event },
        { "hit_event", show_hit_event }, { "query_kind", show_query_kind },
        { "query_mode", show_query_mode }, { "query", show_query },
        { "query_hit", show_query_hit }, { "joint_flags", show_joint_flags },
        { "body_joint", show_body_joint },
        { "joint_features", show_joint_features },
        { "hinge_joint", show_hinge_joint },
        { "slider_joint", show_slider_joint },
        { "distance_joint", show_distance_joint },
        { "swing_twist_joint", show_swing_twist_joint },
        { "six_dof_joint", show_six_dof_joint },
        { "body_joint_event", show_body_joint_event },
        { "vehicle", show_vehicle }, { "wheel_flags", show_wheel_flags },
        { "wheel", show_wheel }, { "vehicle_input", show_vehicle_input },
        { "wheel_state", show_wheel_state }, { "pose_drive", show_pose_drive },
        { "destructible", show_destructible }, { "damage", show_damage },
        { "break_event", show_break_event },
        { "hidden_joint", show_hidden_joint }, { "cut", show_cut },
        { "cut_event", show_cut_event }, { "soft_body", show_soft_body },
        { "soft_body_reset", show_soft_body_reset },
        { "rope_flags", show_rope_flags }, { "rope", show_rope },
        { "water", show_water }, { "wave", show_wave },
        { "water_sample", show_water_sample }, { "buoyancy", show_buoyancy },
        { "liquid_flags", show_liquid_flags }, { "liquid", show_liquid },
        { "liquid_emitter", show_liquid_emitter },
        { "lag_history", show_lag_history },
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
