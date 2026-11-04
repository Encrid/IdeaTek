/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_PHYSICS_H
    #define BC_IDEATEK_PHYSICS_H

    #include <stdint.h>

    #include "bridge_schema.h"
    #include "ideatek_world.h"
    #include "ideatek_space.h"

    #define IDEATEK_BODY_KIND_VALUES { \
        IDEATEK_BODY_KIND_STATIC = 0, \
        IDEATEK_BODY_KIND_KINEMATIC = 1, \
        IDEATEK_BODY_KIND_DYNAMIC = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_body_kind_t,
    uint8_t, IDEATEK_BODY_KIND_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_body_kind_t,
    uint8_t, IDEATEK_BODY_KIND_VALUES);

    #define IDEATEK_MASS_MODE_VALUES { \
        IDEATEK_MASS_MODE_SHAPES = 0, \
        IDEATEK_MASS_MODE_CUSTOM = 1 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_mass_mode_t,
    uint8_t, IDEATEK_MASS_MODE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_mass_mode_t,
    uint8_t, IDEATEK_MASS_MODE_VALUES);

    #define IDEATEK_LOCKED_VALUES { \
        IDEATEK_LOCKED_X = 0x01, \
        IDEATEK_LOCKED_Y = 0x02, \
        IDEATEK_LOCKED_Z = 0x04, \
        IDEATEK_LOCKED_ROLL = 0x08, \
        IDEATEK_LOCKED_PITCH = 0x10, \
        IDEATEK_LOCKED_YAW = 0x20 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_locked_t, uint8_t, IDEATEK_LOCKED_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_locked_t, uint8_t, IDEATEK_LOCKED_VALUES);

    #define IDEATEK_BODY_FLAGS_VALUES { \
        IDEATEK_BODY_FLAGS_CAN_SLEEP = 0x01, \
        IDEATEK_BODY_FLAGS_CONTINUOUS = 0x02, \
        IDEATEK_BODY_FLAGS_DESTRUCTIBLE = 0x04 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_body_flags_t,
    uint8_t, IDEATEK_BODY_FLAGS_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_body_flags_t,
    uint8_t, IDEATEK_BODY_FLAGS_VALUES);

    #define IDEATEK_BODY_FIELDS { \
        ideatek_vec3_t center_of_mass; \
        ideatek_vec3_t inertia; \
        float mass; \
        float gravity_scale; \
        float linear_damping; \
        float angular_damping; \
        ideatek_body_kind_t kind; \
        ideatek_mass_mode_t mass_mode; \
        ideatek_locked_t locked; \
        ideatek_body_flags_t flags; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_body_t, IDEATEK_BODY_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_body_t, IDEATEK_BODY_FIELDS);

    #define IDEATEK_MOTION_FIELDS { \
        ideatek_vec3_t linear; \
        ideatek_vec3_t angular; \
        bool sleeping; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_motion_t, IDEATEK_MOTION_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_motion_t, IDEATEK_MOTION_FIELDS);

    #define IDEATEK_PUSH_FIELDS { \
        ideatek_position_t point; \
        ideatek_vec3_t force; \
        ideatek_vec3_t torque; \
        ideatek_vec3_t impulse; \
        ideatek_vec3_t angular_impulse; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_push_t, IDEATEK_PUSH_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_push_t, IDEATEK_PUSH_FIELDS);

    #define IDEATEK_SHAPE_KIND_VALUES { \
        IDEATEK_SHAPE_KIND_SPHERE = 0, \
        IDEATEK_SHAPE_KIND_BOX = 1, \
        IDEATEK_SHAPE_KIND_CAPSULE = 2, \
        IDEATEK_SHAPE_KIND_CYLINDER = 3, \
        IDEATEK_SHAPE_KIND_CONVEX = 4, \
        IDEATEK_SHAPE_KIND_TRIANGLES = 5, \
        IDEATEK_SHAPE_KIND_PLANE = 6 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_shape_kind_t,
    uint8_t, IDEATEK_SHAPE_KIND_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_shape_kind_t,
    uint8_t, IDEATEK_SHAPE_KIND_VALUES);

    #define IDEATEK_SHAPE_FLAGS_VALUES { \
        IDEATEK_SHAPE_FLAGS_SENSOR = 0x01, \
        IDEATEK_SHAPE_FLAGS_SENSOR_EVENTS = 0x02, \
        IDEATEK_SHAPE_FLAGS_CONTACT_EVENTS = 0x04, \
        IDEATEK_SHAPE_FLAGS_HIT_EVENTS = 0x08 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_shape_flags_t,
    uint8_t, IDEATEK_SHAPE_FLAGS_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_shape_flags_t,
    uint8_t, IDEATEK_SHAPE_FLAGS_VALUES);

    #define IDEATEK_SHAPE_FIELDS { \
        ideatek_entity_t body; \
        ideatek_entity_t material; \
        ideatek_entity_t mesh; \
        uint64_t categories; \
        uint64_t touches; \
        ideatek_vec3_t size; \
        uint32_t part; \
        ideatek_shape_kind_t kind; \
        ideatek_shape_flags_t flags; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_shape_t, IDEATEK_SHAPE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_shape_t, IDEATEK_SHAPE_FIELDS);

    #define IDEATEK_COMBINE_VALUES { \
        IDEATEK_COMBINE_AVERAGE = 0, \
        IDEATEK_COMBINE_MIN = 1, \
        IDEATEK_COMBINE_MULTIPLY = 2, \
        IDEATEK_COMBINE_MAX = 3 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_combine_t, uint8_t, IDEATEK_COMBINE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_combine_t, uint8_t, IDEATEK_COMBINE_VALUES);

    #define IDEATEK_PHYSICS_MATERIAL_FIELDS { \
        float friction; \
        float restitution; \
        float density; \
        float tensile; \
        float compression; \
        float shear; \
        ideatek_combine_t friction_combine; \
        ideatek_combine_t restitution_combine; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_physics_material_t,
    IDEATEK_PHYSICS_MATERIAL_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_physics_material_t,
    IDEATEK_PHYSICS_MATERIAL_FIELDS);

    #define IDEATEK_TOUCH_KIND_VALUES { \
        IDEATEK_TOUCH_KIND_BEGAN = 0, \
        IDEATEK_TOUCH_KIND_ENDED = 1 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_touch_kind_t,
    uint8_t, IDEATEK_TOUCH_KIND_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_touch_kind_t,
    uint8_t, IDEATEK_TOUCH_KIND_VALUES);

    #define IDEATEK_SENSOR_EVENT_FIELDS { \
        ideatek_entity_t sensor; \
        ideatek_entity_t visitor; \
        uint32_t order; \
        ideatek_touch_kind_t kind; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_sensor_event_t, IDEATEK_SENSOR_EVENT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_sensor_event_t, IDEATEK_SENSOR_EVENT_FIELDS);

    #define IDEATEK_CONTACT_EVENT_FIELDS { \
        ideatek_entity_t a; \
        ideatek_entity_t b; \
        uint32_t order; \
        ideatek_touch_kind_t kind; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_contact_event_t,
    IDEATEK_CONTACT_EVENT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_contact_event_t,
    IDEATEK_CONTACT_EVENT_FIELDS);

    #define IDEATEK_HIT_EVENT_FIELDS { \
        ideatek_entity_t a; \
        ideatek_entity_t b; \
        ideatek_position_t point; \
        ideatek_vec3_t normal; \
        float speed; \
        uint32_t order; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_hit_event_t, IDEATEK_HIT_EVENT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_hit_event_t, IDEATEK_HIT_EVENT_FIELDS);

    #define IDEATEK_QUERY_KIND_VALUES { \
        IDEATEK_QUERY_KIND_RAY = 0, \
        IDEATEK_QUERY_KIND_SHAPE_CAST = 1, \
        IDEATEK_QUERY_KIND_OVERLAP = 2, \
        IDEATEK_QUERY_KIND_WATER = 3 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_query_kind_t,
    uint8_t, IDEATEK_QUERY_KIND_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_query_kind_t,
    uint8_t, IDEATEK_QUERY_KIND_VALUES);

    #define IDEATEK_QUERY_MODE_VALUES { \
        IDEATEK_QUERY_MODE_TEST = 0, \
        IDEATEK_QUERY_MODE_CLOSEST = 1, \
        IDEATEK_QUERY_MODE_ALL = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_query_mode_t,
    uint8_t, IDEATEK_QUERY_MODE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_query_mode_t,
    uint8_t, IDEATEK_QUERY_MODE_VALUES);

    #define IDEATEK_QUERY_FIELDS { \
        ideatek_position_t origin; \
        uint64_t categories; \
        uint64_t touches; \
        ideatek_quat_t rotation; \
        ideatek_vec3_t translation; \
        ideatek_vec3_t size; \
        uint32_t max; \
        uint32_t tick; \
        ideatek_query_kind_t kind; \
        ideatek_shape_kind_t shape; \
        ideatek_query_mode_t mode; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_query_t, IDEATEK_QUERY_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_query_t, IDEATEK_QUERY_FIELDS);

    #define IDEATEK_QUERY_HIT_FIELDS { \
        ideatek_entity_t query; \
        ideatek_entity_t shape; \
        ideatek_position_t point; \
        ideatek_vec3_t normal; \
        float fraction; \
        uint32_t order; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_query_hit_t, IDEATEK_QUERY_HIT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_query_hit_t, IDEATEK_QUERY_HIT_FIELDS);

    #define IDEATEK_JOINT_FLAGS_VALUES { \
        IDEATEK_JOINT_FLAGS_COLLIDE_CONNECTED = 0x01, \
        IDEATEK_JOINT_FLAGS_BREAKABLE = 0x02 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_joint_flags_t,
    uint8_t, IDEATEK_JOINT_FLAGS_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_joint_flags_t,
    uint8_t, IDEATEK_JOINT_FLAGS_VALUES);

    #define IDEATEK_BODY_JOINT_FIELDS { \
        ideatek_entity_t a; \
        ideatek_entity_t b; \
        ideatek_quat_t rotation_a; \
        ideatek_quat_t rotation_b; \
        ideatek_vec3_t position_a; \
        ideatek_vec3_t position_b; \
        float force_threshold; \
        float torque_threshold; \
        ideatek_joint_flags_t flags; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_body_joint_t, IDEATEK_BODY_JOINT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_body_joint_t, IDEATEK_BODY_JOINT_FIELDS);

    #define IDEATEK_JOINT_FEATURES_VALUES { \
        IDEATEK_JOINT_FEATURES_LIMIT = 0x01, \
        IDEATEK_JOINT_FEATURES_SPRING = 0x02, \
        IDEATEK_JOINT_FEATURES_MOTOR = 0x04 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_joint_features_t,
    uint8_t, IDEATEK_JOINT_FEATURES_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_joint_features_t,
    uint8_t, IDEATEK_JOINT_FEATURES_VALUES);

    #define IDEATEK_HINGE_JOINT_FIELDS { \
        float lower; \
        float upper; \
        float spring_hertz; \
        float spring_damping; \
        float motor_speed; \
        float max_motor_torque; \
        ideatek_joint_features_t features; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_hinge_joint_t, IDEATEK_HINGE_JOINT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_hinge_joint_t, IDEATEK_HINGE_JOINT_FIELDS);

    #define IDEATEK_SLIDER_JOINT_FIELDS { \
        float lower; \
        float upper; \
        float spring_hertz; \
        float spring_damping; \
        float motor_speed; \
        float max_motor_force; \
        ideatek_joint_features_t features; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_slider_joint_t, IDEATEK_SLIDER_JOINT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_slider_joint_t, IDEATEK_SLIDER_JOINT_FIELDS);

    #define IDEATEK_DISTANCE_JOINT_FIELDS { \
        float length; \
        float min_length; \
        float max_length; \
        float spring_hertz; \
        float spring_damping; \
        float motor_speed; \
        float max_motor_force; \
        ideatek_joint_features_t features; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_distance_joint_t,
    IDEATEK_DISTANCE_JOINT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_distance_joint_t,
    IDEATEK_DISTANCE_JOINT_FIELDS);

    #define IDEATEK_SWING_TWIST_JOINT_FIELDS { \
        float swing_y; \
        float swing_z; \
        float twist_min; \
        float twist_max; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_swing_twist_joint_t,
    IDEATEK_SWING_TWIST_JOINT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_swing_twist_joint_t,
    IDEATEK_SWING_TWIST_JOINT_FIELDS);

    #define IDEATEK_SIX_DOF_JOINT_FIELDS { \
        ideatek_vec3_t linear_min; \
        ideatek_vec3_t linear_max; \
        ideatek_vec3_t angular_min; \
        ideatek_vec3_t angular_max; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_six_dof_joint_t,
    IDEATEK_SIX_DOF_JOINT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_six_dof_joint_t,
    IDEATEK_SIX_DOF_JOINT_FIELDS);

    #define IDEATEK_BODY_JOINT_EVENT_FIELDS { \
        ideatek_entity_t joint; \
        float force; \
        float torque; \
        uint32_t order; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_body_joint_event_t,
    IDEATEK_BODY_JOINT_EVENT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_body_joint_event_t,
    IDEATEK_BODY_JOINT_EVENT_FIELDS);

    #define IDEATEK_VEHICLE_FIELDS { \
        float max_drive_torque; \
        float max_pitch_roll; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_vehicle_t, IDEATEK_VEHICLE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_vehicle_t, IDEATEK_VEHICLE_FIELDS);

    #define IDEATEK_WHEEL_FLAGS_VALUES { \
        IDEATEK_WHEEL_FLAGS_DRIVEN = 0x01, \
        IDEATEK_WHEEL_FLAGS_STEERS = 0x02 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_wheel_flags_t,
    uint8_t, IDEATEK_WHEEL_FLAGS_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_wheel_flags_t,
    uint8_t, IDEATEK_WHEEL_FLAGS_VALUES);

    #define IDEATEK_WHEEL_FIELDS { \
        ideatek_entity_t vehicle; \
        ideatek_vec3_t position; \
        ideatek_vec3_t suspension_direction; \
        ideatek_vec3_t steering_axis; \
        float radius; \
        float width; \
        float suspension_min; \
        float suspension_max; \
        float spring_hertz; \
        float spring_damping; \
        float max_steer; \
        float max_brake_torque; \
        float max_hand_brake_torque; \
        ideatek_wheel_flags_t flags; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_wheel_t, IDEATEK_WHEEL_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_wheel_t, IDEATEK_WHEEL_FIELDS);

    #define IDEATEK_VEHICLE_INPUT_FIELDS { \
        float forward; \
        float right; \
        float brake; \
        float hand_brake; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_vehicle_input_t,
    IDEATEK_VEHICLE_INPUT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_vehicle_input_t,
    IDEATEK_VEHICLE_INPUT_FIELDS);

    #define IDEATEK_WHEEL_STATE_FIELDS { \
        float steer_angle; \
        float angle; \
        float angular_velocity; \
        float suspension_length; \
        float longitudinal_slip; \
        float lateral_slip; \
        bool contact; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_wheel_state_t, IDEATEK_WHEEL_STATE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_wheel_state_t, IDEATEK_WHEEL_STATE_FIELDS);

    #define IDEATEK_POSE_DRIVE_FIELDS { \
        float position_strength; \
        float orientation_strength; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_pose_drive_t, IDEATEK_POSE_DRIVE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_pose_drive_t, IDEATEK_POSE_DRIVE_FIELDS);

    #define IDEATEK_DESTRUCTIBLE_FIELDS { \
        ideatek_entity_t interior; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_destructible_t, IDEATEK_DESTRUCTIBLE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_destructible_t, IDEATEK_DESTRUCTIBLE_FIELDS);

    #define IDEATEK_DAMAGE_FIELDS { \
        ideatek_position_t point; \
        float radius; \
        float pressure; \
        uint32_t order; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_damage_t, IDEATEK_DAMAGE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_damage_t, IDEATEK_DAMAGE_FIELDS);

    #define IDEATEK_BREAK_EVENT_FIELDS { \
        ideatek_entity_t body; \
        ideatek_entity_t piece; \
        ideatek_position_t point; \
        uint32_t order; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_break_event_t, IDEATEK_BREAK_EVENT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_break_event_t, IDEATEK_BREAK_EVENT_FIELDS);

    #define IDEATEK_HIDDEN_JOINT_FIELDS { \
        ideatek_entity_t cap; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_hidden_joint_t, IDEATEK_HIDDEN_JOINT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_hidden_joint_t, IDEATEK_HIDDEN_JOINT_FIELDS);

    #define IDEATEK_CUT_FIELDS { \
        ideatek_entity_t drawn; \
        ideatek_position_t point; \
        ideatek_vec3_t normal; \
        uint32_t order; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_cut_t, IDEATEK_CUT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_cut_t, IDEATEK_CUT_FIELDS);

    #define IDEATEK_CUT_EVENT_FIELDS { \
        ideatek_entity_t drawn; \
        ideatek_entity_t piece; \
        uint32_t order; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_cut_event_t, IDEATEK_CUT_EVENT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_cut_event_t, IDEATEK_CUT_EVENT_FIELDS);

    #define IDEATEK_SOFT_BODY_FIELDS { \
        ideatek_entity_t source; \
        uint64_t categories; \
        uint64_t touches; \
        float edge_compliance; \
        float shear_compliance; \
        float bend_compliance; \
        float vertex_mass; \
        float linear_damping; \
        float friction; \
        float gravity_scale; \
        float vertex_radius; \
        float pressure; \
        float restitution; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_soft_body_t, IDEATEK_SOFT_BODY_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_soft_body_t, IDEATEK_SOFT_BODY_FIELDS);

    #define IDEATEK_SOFT_BODY_RESET_FIELDS { \
        ideatek_entity_t body; \
        uint32_t order; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_soft_body_reset_t,
    IDEATEK_SOFT_BODY_RESET_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_soft_body_reset_t,
    IDEATEK_SOFT_BODY_RESET_FIELDS);

    #define IDEATEK_ROPE_FLAGS_VALUES { \
        IDEATEK_ROPE_FLAGS_STIFF = 0x01, \
        IDEATEK_ROPE_FLAGS_COLLIDE = 0x02 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_rope_flags_t,
    uint8_t, IDEATEK_ROPE_FLAGS_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_rope_flags_t,
    uint8_t, IDEATEK_ROPE_FLAGS_VALUES);

    #define IDEATEK_ROPE_FIELDS { \
        ideatek_entity_t start; \
        ideatek_entity_t end; \
        uint64_t categories; \
        uint64_t touches; \
        ideatek_vec3_t start_offset; \
        ideatek_vec3_t end_offset; \
        float length; \
        float width; \
        float gravity_scale; \
        float friction; \
        uint32_t segments; \
        uint32_t sides; \
        ideatek_rope_flags_t flags; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_rope_t, IDEATEK_ROPE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_rope_t, IDEATEK_ROPE_FIELDS);

    #define IDEATEK_WATER_FIELDS { \
        ideatek_vec3_t flow; \
        float density; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_water_t, IDEATEK_WATER_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_water_t, IDEATEK_WATER_FIELDS);

    #define IDEATEK_WAVE_FIELDS { \
        ideatek_entity_t water; \
        ideatek_vec3_t direction; \
        float wavelength; \
        float amplitude; \
        float steepness; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_wave_t, IDEATEK_WAVE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_wave_t, IDEATEK_WAVE_FIELDS);

    #define IDEATEK_WATER_SAMPLE_FIELDS { \
        ideatek_entity_t query; \
        ideatek_entity_t water; \
        ideatek_position_t point; \
        ideatek_vec3_t normal; \
        ideatek_vec3_t velocity; \
        float depth; \
        uint32_t order; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_water_sample_t, IDEATEK_WATER_SAMPLE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_water_sample_t, IDEATEK_WATER_SAMPLE_FIELDS);

    #define IDEATEK_BUOYANCY_FIELDS { \
        float linear_drag; \
        float angular_drag; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_buoyancy_t, IDEATEK_BUOYANCY_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_buoyancy_t, IDEATEK_BUOYANCY_FIELDS);

    #define IDEATEK_LIQUID_FLAGS_VALUES { \
        IDEATEK_LIQUID_FLAGS_PUSH_BODIES = 0x01 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_liquid_flags_t,
    uint8_t, IDEATEK_LIQUID_FLAGS_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_liquid_flags_t,
    uint8_t, IDEATEK_LIQUID_FLAGS_VALUES);

    #define IDEATEK_LIQUID_FIELDS { \
        uint64_t categories; \
        uint64_t touches; \
        float density; \
        float viscosity; \
        float resolution; \
        ideatek_liquid_flags_t flags; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_liquid_t, IDEATEK_LIQUID_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_liquid_t, IDEATEK_LIQUID_FIELDS);

    #define IDEATEK_LIQUID_EMITTER_FIELDS { \
        ideatek_entity_t liquid; \
        ideatek_vec3_t velocity; \
        float rate; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_liquid_emitter_t,
    IDEATEK_LIQUID_EMITTER_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_liquid_emitter_t,
    IDEATEK_LIQUID_EMITTER_FIELDS);

    #define IDEATEK_LAG_HISTORY_FIELDS { \
        float seconds; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_lag_history_t, IDEATEK_LAG_HISTORY_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_lag_history_t, IDEATEK_LAG_HISTORY_FIELDS);

#endif
