/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_PHYSICS_H
    #define BC_IDEATEK_PHYSICS_H

    #include <stdint.h>

    #include "bridge_abi.h"
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

    #define IDEATEK_MOTION_QUALITY_VALUES { \
        IDEATEK_MOTION_QUALITY_DISCRETE = 0, \
        IDEATEK_MOTION_QUALITY_LINEAR_CAST = 1 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_motion_quality_t,
    uint8_t, IDEATEK_MOTION_QUALITY_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_motion_quality_t,
    uint8_t, IDEATEK_MOTION_QUALITY_VALUES);

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
        IDEATEK_BODY_FLAGS_ENHANCED_EDGES = 0x02, \
        IDEATEK_BODY_FLAGS_DESTRUCTIBLE = 0x04, \
        IDEATEK_BODY_FLAGS_GYROSCOPIC = 0x08 \
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
        float max_linear_speed; \
        float max_angular_speed; \
        float sleep_threshold; \
        float inertia_scale; \
        ideatek_body_kind_t kind; \
        ideatek_mass_mode_t mass_mode; \
        ideatek_locked_t locked; \
        ideatek_body_flags_t flags; \
        ideatek_motion_quality_t motion_quality; \
        uint8_t velocity_steps; \
        uint8_t position_steps; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_body_t, IDEATEK_BODY_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_body_t, IDEATEK_BODY_FIELDS);

    #define IDEATEK_KINEMATIC_TARGET_FIELDS { \
        ideatek_position_t position; \
        ideatek_quat_t rotation; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_kinematic_target_t,
    IDEATEK_KINEMATIC_TARGET_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_kinematic_target_t,
    IDEATEK_KINEMATIC_TARGET_FIELDS);

    #define IDEATEK_WAKE_EVENT_FIELDS { \
        ideatek_entity_t body; \
        uint32_t order; \
        bool asleep; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_wake_event_t, IDEATEK_WAKE_EVENT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_wake_event_t, IDEATEK_WAKE_EVENT_FIELDS);

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
        IDEATEK_SHAPE_KIND_PLANE = 6, \
        IDEATEK_SHAPE_KIND_HEIGHT_FIELD = 7 \
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
        ideatek_entity_t source; \
        uint64_t categories; \
        uint64_t touches; \
        ideatek_vec3_t size; \
        uint32_t part; \
        ideatek_shape_kind_t kind; \
        ideatek_shape_flags_t flags; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_shape_t, IDEATEK_SHAPE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_shape_t, IDEATEK_SHAPE_FIELDS);

    #define IDEATEK_SHAPE_MATERIALS_FIELDS { \
        bridge_text_t materials; \
        bridge_text_t indices; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_shape_materials_t,
    IDEATEK_SHAPE_MATERIALS_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_shape_materials_t,
    IDEATEK_SHAPE_MATERIALS_FIELDS);

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
        bool back_faces; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_query_t, IDEATEK_QUERY_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_query_t, IDEATEK_QUERY_FIELDS);

    #define IDEATEK_QUERY_HIT_FIELDS { \
        ideatek_entity_t query; \
        ideatek_entity_t shape; \
        ideatek_position_t point; \
        ideatek_vec3_t normal; \
        float fraction; \
        ideatek_entity_t material; \
        float uv[2]; \
        float depth; \
        uint32_t face; \
        uint32_t sub_shape; \
        uint32_t order; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_query_hit_t, IDEATEK_QUERY_HIT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_query_hit_t, IDEATEK_QUERY_HIT_FIELDS);

    #define IDEATEK_JOINT_FLAGS_VALUES { \
        IDEATEK_JOINT_FLAGS_COLLIDE_CONNECTED = 0x01, \
        IDEATEK_JOINT_FLAGS_BREAKABLE = 0x02, \
        IDEATEK_JOINT_FLAGS_REPORT = 0x04 \
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

    #define IDEATEK_DRIVE_MODE_VALUES { \
        IDEATEK_DRIVE_MODE_OFF = 0, \
        IDEATEK_DRIVE_MODE_VELOCITY = 1, \
        IDEATEK_DRIVE_MODE_POSITION = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_drive_mode_t,
    uint8_t, IDEATEK_DRIVE_MODE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_drive_mode_t,
    uint8_t, IDEATEK_DRIVE_MODE_VALUES);

    #define IDEATEK_SPRING_MODE_VALUES { \
        IDEATEK_SPRING_MODE_FREQUENCY = 0, \
        IDEATEK_SPRING_MODE_STIFFNESS = 1 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_spring_mode_t,
    uint8_t, IDEATEK_SPRING_MODE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_spring_mode_t,
    uint8_t, IDEATEK_SPRING_MODE_VALUES);

    #define IDEATEK_JOINT_DRIVE_FIELDS { \
        float target; \
        float min_force; \
        float max_force; \
        float spring; \
        float damping; \
        ideatek_drive_mode_t mode; \
        ideatek_spring_mode_t spring_mode; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_joint_drive_t, IDEATEK_JOINT_DRIVE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_joint_drive_t, IDEATEK_JOINT_DRIVE_FIELDS);

    #define IDEATEK_HINGE_JOINT_FIELDS { \
        float lower; \
        float upper; \
        float limit_hertz; \
        float limit_damping; \
        float friction; \
        ideatek_joint_drive_t drive; \
        ideatek_joint_features_t features; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_hinge_joint_t, IDEATEK_HINGE_JOINT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_hinge_joint_t, IDEATEK_HINGE_JOINT_FIELDS);

    #define IDEATEK_SLIDER_JOINT_FIELDS { \
        float lower; \
        float upper; \
        float limit_hertz; \
        float limit_damping; \
        float friction; \
        ideatek_joint_drive_t drive; \
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
        bridge_text_t drives; \
        bridge_text_t friction; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_six_dof_joint_t,
    IDEATEK_SIX_DOF_JOINT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_six_dof_joint_t,
    IDEATEK_SIX_DOF_JOINT_FIELDS);

    #define IDEATEK_GEAR_JOINT_FIELDS { \
        ideatek_entity_t hinge_a; \
        ideatek_entity_t hinge_b; \
        float ratio; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_gear_joint_t, IDEATEK_GEAR_JOINT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_gear_joint_t, IDEATEK_GEAR_JOINT_FIELDS);

    #define IDEATEK_RACK_AND_PINION_JOINT_FIELDS { \
        ideatek_entity_t hinge; \
        ideatek_entity_t slider; \
        float ratio; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_rack_and_pinion_joint_t,
    IDEATEK_RACK_AND_PINION_JOINT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_rack_and_pinion_joint_t,
    IDEATEK_RACK_AND_PINION_JOINT_FIELDS);

    #define IDEATEK_PULLEY_JOINT_FIELDS { \
        ideatek_position_t fixed_a; \
        ideatek_position_t fixed_b; \
        float ratio; \
        float min_length; \
        float max_length; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_pulley_joint_t, IDEATEK_PULLEY_JOINT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_pulley_joint_t, IDEATEK_PULLEY_JOINT_FIELDS);

    #define IDEATEK_PATH_ROTATION_VALUES { \
        IDEATEK_PATH_ROTATION_FREE = 0, \
        IDEATEK_PATH_ROTATION_TANGENT = 1, \
        IDEATEK_PATH_ROTATION_NORMAL = 2, \
        IDEATEK_PATH_ROTATION_BINORMAL = 3, \
        IDEATEK_PATH_ROTATION_PATH = 4, \
        IDEATEK_PATH_ROTATION_FIXED = 5 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_path_rotation_t,
    uint8_t, IDEATEK_PATH_ROTATION_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_path_rotation_t,
    uint8_t, IDEATEK_PATH_ROTATION_VALUES);

    #define IDEATEK_PATH_JOINT_FIELDS { \
        bridge_text_t path; \
        float fraction; \
        float max_friction; \
        ideatek_joint_drive_t drive; \
        ideatek_path_rotation_t rotation; \
        bool looping; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_path_joint_t, IDEATEK_PATH_JOINT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_path_joint_t, IDEATEK_PATH_JOINT_FIELDS);

    #define IDEATEK_CONE_JOINT_FIELDS { \
        float half_angle; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_cone_joint_t, IDEATEK_CONE_JOINT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_cone_joint_t, IDEATEK_CONE_JOINT_FIELDS);

    #define IDEATEK_MOTOR_JOINT_FIELDS { \
        ideatek_vec3_t linear_velocity; \
        ideatek_vec3_t angular_velocity; \
        float max_force; \
        float max_torque; \
        float hertz; \
        float damping; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_motor_joint_t, IDEATEK_MOTOR_JOINT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_motor_joint_t, IDEATEK_MOTOR_JOINT_FIELDS);

    #define IDEATEK_WHEEL_JOINT_FIELDS { \
        float lower; \
        float upper; \
        float suspension_hertz; \
        float suspension_damping; \
        float max_steer; \
        float steer_target; \
        ideatek_joint_drive_t spin; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_wheel_joint_t, IDEATEK_WHEEL_JOINT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_wheel_joint_t, IDEATEK_WHEEL_JOINT_FIELDS);

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

    #define IDEATEK_BODY_JOINT_STATE_FIELDS { \
        ideatek_vec3_t force; \
        ideatek_vec3_t torque; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_body_joint_state_t,
    IDEATEK_BODY_JOINT_STATE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_body_joint_state_t,
    IDEATEK_BODY_JOINT_STATE_FIELDS);

    #define IDEATEK_ARTICULATION_JOINT_VALUES { \
        IDEATEK_ARTICULATION_JOINT_FIX = 0, \
        IDEATEK_ARTICULATION_JOINT_PRISMATIC = 1, \
        IDEATEK_ARTICULATION_JOINT_REVOLUTE = 2, \
        IDEATEK_ARTICULATION_JOINT_SPHERICAL = 3 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_articulation_joint_t,
    uint8_t, IDEATEK_ARTICULATION_JOINT_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_articulation_joint_t,
    uint8_t, IDEATEK_ARTICULATION_JOINT_VALUES);

    #define IDEATEK_ARTICULATION_FLAGS_VALUES { \
        IDEATEK_ARTICULATION_FLAGS_FIX_BASE = 0x01, \
        IDEATEK_ARTICULATION_FLAGS_SELF_COLLISION = 0x02 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_articulation_flags_t,
    uint8_t, IDEATEK_ARTICULATION_FLAGS_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_articulation_flags_t,
    uint8_t, IDEATEK_ARTICULATION_FLAGS_VALUES);

    #define IDEATEK_ARTICULATION_FIELDS { \
        float sleep_threshold; \
        uint8_t velocity_steps; \
        uint8_t position_steps; \
        ideatek_articulation_flags_t flags; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_articulation_t, IDEATEK_ARTICULATION_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_articulation_t, IDEATEK_ARTICULATION_FIELDS);

    #define IDEATEK_ARTICULATION_LINK_FIELDS { \
        ideatek_entity_t articulation; \
        ideatek_entity_t parent; \
        bridge_text_t limits; \
        bridge_text_t drives; \
        float friction; \
        float max_joint_velocity; \
        uint8_t motions[6]; \
        ideatek_articulation_joint_t joint; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_articulation_link_t,
    IDEATEK_ARTICULATION_LINK_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_articulation_link_t,
    IDEATEK_ARTICULATION_LINK_FIELDS);

    #define IDEATEK_ARTICULATION_STATE_FIELDS { \
        bridge_text_t positions; \
        bridge_text_t velocities; \
        uint32_t count; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_articulation_state_t,
    IDEATEK_ARTICULATION_STATE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_articulation_state_t,
    IDEATEK_ARTICULATION_STATE_FIELDS);

    #define IDEATEK_ARTICULATION_COMMAND_FIELDS { \
        bridge_text_t positions; \
        bridge_text_t velocities; \
        uint32_t count; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_articulation_command_t,
    IDEATEK_ARTICULATION_COMMAND_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_articulation_command_t,
    IDEATEK_ARTICULATION_COMMAND_FIELDS);

    #define IDEATEK_GROUND_STATE_VALUES { \
        IDEATEK_GROUND_STATE_ON_GROUND = 0, \
        IDEATEK_GROUND_STATE_ON_STEEP_GROUND = 1, \
        IDEATEK_GROUND_STATE_NOT_SUPPORTED = 2, \
        IDEATEK_GROUND_STATE_IN_AIR = 3 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_ground_state_t,
    uint8_t, IDEATEK_GROUND_STATE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_ground_state_t,
    uint8_t, IDEATEK_GROUND_STATE_VALUES);

    #define IDEATEK_NON_WALKABLE_VALUES { \
        IDEATEK_NON_WALKABLE_SLIDE = 0, \
        IDEATEK_NON_WALKABLE_PREVENT_CLIMB = 1 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_non_walkable_t,
    uint8_t, IDEATEK_NON_WALKABLE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_non_walkable_t,
    uint8_t, IDEATEK_NON_WALKABLE_VALUES);

    #define IDEATEK_CHARACTER_FLAGS_VALUES { \
        IDEATEK_CHARACTER_FLAGS_INNER_BODY = 0x01, \
        IDEATEK_CHARACTER_FLAGS_CAN_PUSH = 0x02, \
        IDEATEK_CHARACTER_FLAGS_CAN_BE_PUSHED = 0x04 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_character_flags_t,
    uint8_t, IDEATEK_CHARACTER_FLAGS_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_character_flags_t,
    uint8_t, IDEATEK_CHARACTER_FLAGS_VALUES);

    #define IDEATEK_CHARACTER_COLLISION_VALUES { \
        IDEATEK_CHARACTER_COLLISION_SIDES = 0x01, \
        IDEATEK_CHARACTER_COLLISION_UP = 0x02, \
        IDEATEK_CHARACTER_COLLISION_DOWN = 0x04 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_character_collision_t,
    uint8_t, IDEATEK_CHARACTER_COLLISION_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_character_collision_t,
    uint8_t, IDEATEK_CHARACTER_COLLISION_VALUES);

    #define IDEATEK_CHARACTER_FIELDS { \
        ideatek_vec3_t up; \
        float max_slope; \
        float step_up; \
        float stick_down; \
        float min_step_forward; \
        float padding; \
        float predictive_distance; \
        float mass; \
        float max_strength; \
        ideatek_non_walkable_t non_walkable; \
        ideatek_character_flags_t flags; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_character_t, IDEATEK_CHARACTER_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_character_t, IDEATEK_CHARACTER_FIELDS);

    #define IDEATEK_CHARACTER_MOVE_FIELDS { \
        ideatek_vec3_t velocity; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_character_move_t,
    IDEATEK_CHARACTER_MOVE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_character_move_t,
    IDEATEK_CHARACTER_MOVE_FIELDS);

    #define IDEATEK_CHARACTER_STATE_FIELDS { \
        ideatek_entity_t ground; \
        ideatek_entity_t ground_material; \
        ideatek_vec3_t ground_normal; \
        ideatek_vec3_t ground_velocity; \
        ideatek_ground_state_t ground_state; \
        ideatek_character_collision_t collision; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_character_state_t,
    IDEATEK_CHARACTER_STATE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_character_state_t,
    IDEATEK_CHARACTER_STATE_FIELDS);

    #define IDEATEK_SNAPSHOT_ACTION_VALUES { \
        IDEATEK_SNAPSHOT_ACTION_SAVE = 0, \
        IDEATEK_SNAPSHOT_ACTION_RESTORE = 1 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_snapshot_action_t,
    uint8_t, IDEATEK_SNAPSHOT_ACTION_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_snapshot_action_t,
    uint8_t, IDEATEK_SNAPSHOT_ACTION_VALUES);

    #define IDEATEK_SNAPSHOT_STATUS_VALUES { \
        IDEATEK_SNAPSHOT_STATUS_OK = 0, \
        IDEATEK_SNAPSHOT_STATUS_TOO_BIG = 1, \
        IDEATEK_SNAPSHOT_STATUS_FAILED = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_snapshot_status_t,
    uint8_t, IDEATEK_SNAPSHOT_STATUS_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_snapshot_status_t,
    uint8_t, IDEATEK_SNAPSHOT_STATUS_VALUES);

    #define IDEATEK_PHYSICS_SNAPSHOT_REQUEST_FIELDS { \
        uint64_t categories; \
        bridge_text_t data; \
        ideatek_snapshot_action_t action; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_physics_snapshot_request_t,
    IDEATEK_PHYSICS_SNAPSHOT_REQUEST_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_physics_snapshot_request_t,
    IDEATEK_PHYSICS_SNAPSHOT_REQUEST_FIELDS);

    #define IDEATEK_PHYSICS_SNAPSHOT_FIELDS { \
        bridge_text_t data; \
        ideatek_snapshot_status_t status; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_physics_snapshot_t,
    IDEATEK_PHYSICS_SNAPSHOT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_physics_snapshot_t,
    IDEATEK_PHYSICS_SNAPSHOT_FIELDS);

    #define IDEATEK_VEHICLE_KIND_VALUES { \
        IDEATEK_VEHICLE_KIND_WHEELED = 0, \
        IDEATEK_VEHICLE_KIND_TRACKED = 1, \
        IDEATEK_VEHICLE_KIND_MOTORCYCLE = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_vehicle_kind_t,
    uint8_t, IDEATEK_VEHICLE_KIND_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_vehicle_kind_t,
    uint8_t, IDEATEK_VEHICLE_KIND_VALUES);

    #define IDEATEK_WHEEL_TEST_VALUES { \
        IDEATEK_WHEEL_TEST_RAY = 0, \
        IDEATEK_WHEEL_TEST_SPHERE = 1, \
        IDEATEK_WHEEL_TEST_CYLINDER = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_wheel_test_t,
    uint8_t, IDEATEK_WHEEL_TEST_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_wheel_test_t,
    uint8_t, IDEATEK_WHEEL_TEST_VALUES);

    #define IDEATEK_TRANSMISSION_MODE_VALUES { \
        IDEATEK_TRANSMISSION_MODE_AUTO = 0, \
        IDEATEK_TRANSMISSION_MODE_MANUAL = 1 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_transmission_mode_t,
    uint8_t, IDEATEK_TRANSMISSION_MODE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_transmission_mode_t,
    uint8_t, IDEATEK_TRANSMISSION_MODE_VALUES);

    #define IDEATEK_VEHICLE_FIELDS { \
        float max_drive_torque; \
        float max_pitch_roll; \
        ideatek_vehicle_kind_t kind; \
        ideatek_wheel_test_t test; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_vehicle_t, IDEATEK_VEHICLE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_vehicle_t, IDEATEK_VEHICLE_FIELDS);

    #define IDEATEK_VEHICLE_ENGINE_FIELDS { \
        bridge_text_t torque_curve; \
        float max_torque; \
        float min_rpm; \
        float max_rpm; \
        float inertia; \
        float damping; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_vehicle_engine_t,
    IDEATEK_VEHICLE_ENGINE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_vehicle_engine_t,
    IDEATEK_VEHICLE_ENGINE_FIELDS);

    #define IDEATEK_VEHICLE_TRANSMISSION_FIELDS { \
        bridge_text_t gears; \
        bridge_text_t reverse; \
        float shift_up_rpm; \
        float shift_down_rpm; \
        float switch_time; \
        float clutch_release_time; \
        float switch_latency; \
        float clutch_strength; \
        ideatek_transmission_mode_t mode; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_vehicle_transmission_t,
    IDEATEK_VEHICLE_TRANSMISSION_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_vehicle_transmission_t,
    IDEATEK_VEHICLE_TRANSMISSION_FIELDS);

    #define IDEATEK_VEHICLE_DIFFERENTIAL_FIELDS { \
        ideatek_entity_t vehicle; \
        ideatek_entity_t left; \
        ideatek_entity_t right; \
        float ratio; \
        float split; \
        float limited_slip; \
        float engine_torque_ratio; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_vehicle_differential_t,
    IDEATEK_VEHICLE_DIFFERENTIAL_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_vehicle_differential_t,
    IDEATEK_VEHICLE_DIFFERENTIAL_FIELDS);

    #define IDEATEK_ANTI_ROLL_BAR_FIELDS { \
        ideatek_entity_t vehicle; \
        ideatek_entity_t left; \
        ideatek_entity_t right; \
        float stiffness; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_anti_roll_bar_t,
    IDEATEK_ANTI_ROLL_BAR_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_anti_roll_bar_t,
    IDEATEK_ANTI_ROLL_BAR_FIELDS);

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
        ideatek_vec3_t wheel_up; \
        ideatek_vec3_t wheel_forward; \
        bridge_text_t longitudinal_friction; \
        bridge_text_t lateral_friction; \
        ideatek_wheel_flags_t flags; \
        uint8_t track; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_wheel_t, IDEATEK_WHEEL_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_wheel_t, IDEATEK_WHEEL_FIELDS);

    #define IDEATEK_VEHICLE_INPUT_FIELDS { \
        float forward; \
        float right; \
        float brake; \
        float hand_brake; \
        float clutch; \
        float left_ratio; \
        float right_ratio; \
        int32_t gear; \
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
        ideatek_entity_t contact_body; \
        ideatek_entity_t contact_material; \
        bool contact; \
        bool hit_hard_point; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_wheel_state_t, IDEATEK_WHEEL_STATE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_wheel_state_t, IDEATEK_WHEEL_STATE_FIELDS);

    #define IDEATEK_VEHICLE_STATE_FIELDS { \
        float rpm; \
        float clutch; \
        int32_t gear; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_vehicle_state_t,
    IDEATEK_VEHICLE_STATE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_vehicle_state_t,
    IDEATEK_VEHICLE_STATE_FIELDS);

    #define IDEATEK_POSE_DRIVE_FIELDS { \
        float position_strength; \
        float orientation_strength; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_pose_drive_t, IDEATEK_POSE_DRIVE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_pose_drive_t, IDEATEK_POSE_DRIVE_FIELDS);

    #define IDEATEK_DESTRUCTIBLE_FIELDS { \
        ideatek_entity_t interior; \
        ideatek_entity_t asset; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_destructible_t, IDEATEK_DESTRUCTIBLE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_destructible_t, IDEATEK_DESTRUCTIBLE_FIELDS);

    #define IDEATEK_DESTRUCTIBLE_ASSET_FIELDS { \
        bridge_text_t chunks; \
        bridge_text_t bonds; \
        ideatek_entity_t mesh; \
        uint32_t chunk_count; \
        uint32_t bond_count; \
        uint32_t revision; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_destructible_asset_t,
    IDEATEK_DESTRUCTIBLE_ASSET_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_destructible_asset_t,
    IDEATEK_DESTRUCTIBLE_ASSET_FIELDS);

    #define IDEATEK_DAMAGE_SHAPE_VALUES { \
        IDEATEK_DAMAGE_SHAPE_RADIAL = 0, \
        IDEATEK_DAMAGE_SHAPE_CAPSULE = 1, \
        IDEATEK_DAMAGE_SHAPE_SHEAR = 2, \
        IDEATEK_DAMAGE_SHAPE_TRIANGLE = 3, \
        IDEATEK_DAMAGE_SHAPE_IMPACT = 4 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_damage_shape_t,
    uint8_t, IDEATEK_DAMAGE_SHAPE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_damage_shape_t,
    uint8_t, IDEATEK_DAMAGE_SHAPE_VALUES);

    #define IDEATEK_DAMAGE_FIELDS { \
        ideatek_position_t point; \
        ideatek_position_t point_b; \
        ideatek_position_t point_c; \
        ideatek_vec3_t normal; \
        float min_radius; \
        float radius; \
        float pressure; \
        uint32_t order; \
        ideatek_damage_shape_t shape; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_damage_t, IDEATEK_DAMAGE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_damage_t, IDEATEK_DAMAGE_FIELDS);

    #define IDEATEK_BREAK_EVENT_FIELDS { \
        ideatek_entity_t body; \
        ideatek_entity_t piece; \
        ideatek_position_t point; \
        ideatek_vec3_t excess_force; \
        bridge_text_t chunks; \
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

    #define IDEATEK_BEND_VALUES { \
        IDEATEK_BEND_NONE = 0, \
        IDEATEK_BEND_DISTANCE = 1, \
        IDEATEK_BEND_DIHEDRAL = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_bend_t, uint8_t, IDEATEK_BEND_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_bend_t, uint8_t, IDEATEK_BEND_VALUES);

    #define IDEATEK_LRA_VALUES { \
        IDEATEK_LRA_NONE = 0, \
        IDEATEK_LRA_EUCLIDEAN = 1, \
        IDEATEK_LRA_GEODESIC = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_lra_t, uint8_t, IDEATEK_LRA_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_lra_t, uint8_t, IDEATEK_LRA_VALUES);

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
        float volume_compliance; \
        float lra_scale; \
        float skin_distance_scale; \
        uint32_t iterations; \
        ideatek_bend_t bend; \
        ideatek_lra_t lra; \
        bool double_sided; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_soft_body_t, IDEATEK_SOFT_BODY_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_soft_body_t, IDEATEK_SOFT_BODY_FIELDS);

    #define IDEATEK_SOFT_BODY_ATTACHMENT_FIELDS { \
        ideatek_entity_t soft_body; \
        ideatek_entity_t body; \
        ideatek_vec3_t offset; \
        uint32_t vertex; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_soft_body_attachment_t,
    IDEATEK_SOFT_BODY_ATTACHMENT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_soft_body_attachment_t,
    IDEATEK_SOFT_BODY_ATTACHMENT_FIELDS);

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
        float stretch_compliance; \
        float bend_compliance; \
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
        IDEATEK_LIQUID_FLAGS_PUSH_BODIES = 0x01, \
        IDEATEK_LIQUID_FLAGS_FOAM = 0x02 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_liquid_flags_t,
    uint8_t, IDEATEK_LIQUID_FLAGS_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_liquid_flags_t,
    uint8_t, IDEATEK_LIQUID_FLAGS_VALUES);

    #define IDEATEK_LIQUID_TECHNIQUE_VALUES { \
        IDEATEK_LIQUID_TECHNIQUE_SPH = 0, \
        IDEATEK_LIQUID_TECHNIQUE_FLIP = 1, \
        IDEATEK_LIQUID_TECHNIQUE_PBD = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_liquid_technique_t,
    uint8_t, IDEATEK_LIQUID_TECHNIQUE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_liquid_technique_t,
    uint8_t, IDEATEK_LIQUID_TECHNIQUE_VALUES);

    #define IDEATEK_EMITTER_SHAPE_VALUES { \
        IDEATEK_EMITTER_SHAPE_SHAPE = 0, \
        IDEATEK_EMITTER_SHAPE_BOX = 1, \
        IDEATEK_EMITTER_SHAPE_DISC = 2, \
        IDEATEK_EMITTER_SHAPE_POINT = 3 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_emitter_shape_t,
    uint8_t, IDEATEK_EMITTER_SHAPE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_emitter_shape_t,
    uint8_t, IDEATEK_EMITTER_SHAPE_VALUES);

    #define IDEATEK_LIQUID_FIELDS { \
        uint64_t categories; \
        uint64_t touches; \
        float density; \
        float viscosity; \
        float resolution; \
        float surface_tension; \
        float cohesion; \
        float adhesion; \
        float vorticity; \
        uint32_t max_particles; \
        uint32_t phase; \
        ideatek_liquid_flags_t flags; \
        ideatek_liquid_technique_t technique; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_liquid_t, IDEATEK_LIQUID_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_liquid_t, IDEATEK_LIQUID_FIELDS);

    #define IDEATEK_LIQUID_EMITTER_FIELDS { \
        ideatek_entity_t liquid; \
        ideatek_vec3_t velocity; \
        float rate; \
        ideatek_vec3_t size; \
        float start; \
        float end; \
        float jitter; \
        ideatek_emitter_shape_t shape; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_liquid_emitter_t,
    IDEATEK_LIQUID_EMITTER_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_liquid_emitter_t,
    IDEATEK_LIQUID_EMITTER_FIELDS);

    #define IDEATEK_LIQUID_PARTICLES_FIELDS { \
        bridge_text_t positions; \
        bridge_text_t velocities; \
        uint32_t count; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_liquid_particles_t,
    IDEATEK_LIQUID_PARTICLES_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_liquid_particles_t,
    IDEATEK_LIQUID_PARTICLES_FIELDS);

    #define IDEATEK_LIQUID_FOAM_FIELDS { \
        bridge_text_t positions; \
        bridge_text_t lifetimes; \
        uint32_t count; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_liquid_foam_t, IDEATEK_LIQUID_FOAM_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_liquid_foam_t, IDEATEK_LIQUID_FOAM_FIELDS);

    #define IDEATEK_SMOKE_FLAGS_VALUES { \
        IDEATEK_SMOKE_FLAGS_COLLIDE = 0x01 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_smoke_flags_t,
    uint8_t, IDEATEK_SMOKE_FLAGS_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_smoke_flags_t,
    uint8_t, IDEATEK_SMOKE_FLAGS_VALUES);

    #define IDEATEK_SMOKE_VOLUME_FIELDS { \
        float cell_size; \
        float steps_per_second; \
        float buoyancy_per_temp; \
        float buoyancy_per_smoke; \
        float cooling; \
        float vorticity; \
        float ignition_temp; \
        float burn_per_temp; \
        float fuel_per_burn; \
        float temp_per_burn; \
        float smoke_per_burn; \
        ideatek_smoke_flags_t flags; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_smoke_volume_t, IDEATEK_SMOKE_VOLUME_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_smoke_volume_t, IDEATEK_SMOKE_VOLUME_FIELDS);

    #define IDEATEK_SMOKE_EMITTER_FIELDS { \
        ideatek_entity_t volume; \
        ideatek_vec3_t velocity; \
        ideatek_vec3_t size; \
        float temperature; \
        float fuel; \
        float smoke; \
        float couple_rate; \
        ideatek_emitter_shape_t shape; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_smoke_emitter_t,
    IDEATEK_SMOKE_EMITTER_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_smoke_emitter_t,
    IDEATEK_SMOKE_EMITTER_FIELDS);

    #define IDEATEK_SMOKE_GRID_FIELDS { \
        ideatek_entity_t texture; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_smoke_grid_t, IDEATEK_SMOKE_GRID_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_smoke_grid_t, IDEATEK_SMOKE_GRID_FIELDS);

    #define IDEATEK_LAG_HISTORY_FIELDS { \
        float seconds; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_lag_history_t, IDEATEK_LAG_HISTORY_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_lag_history_t, IDEATEK_LAG_HISTORY_FIELDS);

    #define IDEATEK_FIELD_SHAPE_VALUES { \
        IDEATEK_FIELD_SHAPE_INFINITE = 0, \
        IDEATEK_FIELD_SHAPE_SPHERE = 1, \
        IDEATEK_FIELD_SHAPE_BOX = 2, \
        IDEATEK_FIELD_SHAPE_CYLINDER = 3 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_field_shape_t,
    uint8_t, IDEATEK_FIELD_SHAPE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_field_shape_t,
    uint8_t, IDEATEK_FIELD_SHAPE_VALUES);

    #define IDEATEK_FORCE_KIND_VALUES { \
        IDEATEK_FORCE_KIND_ATTRACTOR = 0, \
        IDEATEK_FORCE_KIND_VORTEX = 1, \
        IDEATEK_FORCE_KIND_DIRECTIONAL = 2, \
        IDEATEK_FORCE_KIND_NOISE = 3, \
        IDEATEK_FORCE_KIND_DRAG = 4, \
        IDEATEK_FORCE_KIND_VECTOR_FIELD = 5 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_force_kind_t,
    uint8_t, IDEATEK_FORCE_KIND_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_force_kind_t,
    uint8_t, IDEATEK_FORCE_KIND_VALUES);

    #define IDEATEK_FIELD_TARGET_VALUES { \
        IDEATEK_FIELD_TARGET_FORCE = 0, \
        IDEATEK_FIELD_TARGET_VELOCITY = 1, \
        IDEATEK_FIELD_TARGET_STRAIN = 2, \
        IDEATEK_FIELD_TARGET_SLEEP = 3, \
        IDEATEK_FIELD_TARGET_WAKE = 4 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_field_target_t,
    uint8_t, IDEATEK_FIELD_TARGET_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_field_target_t,
    uint8_t, IDEATEK_FIELD_TARGET_VALUES);

    #define IDEATEK_GRAVITY_MODE_VALUES { \
        IDEATEK_GRAVITY_MODE_REPLACE = 0, \
        IDEATEK_GRAVITY_MODE_ADD = 1 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_gravity_mode_t,
    uint8_t, IDEATEK_GRAVITY_MODE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_gravity_mode_t,
    uint8_t, IDEATEK_GRAVITY_MODE_VALUES);

    #define IDEATEK_WIND_FIELDS { \
        ideatek_vec3_t size; \
        float inner; \
        float speed; \
        float gust_min; \
        float gust_max; \
        float gust_frequency; \
        float falloff; \
        ideatek_field_shape_t shape; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_wind_t, IDEATEK_WIND_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_wind_t, IDEATEK_WIND_FIELDS);

    #define IDEATEK_FORCE_FIELD_FIELDS { \
        ideatek_entity_t texture; \
        ideatek_vec3_t size; \
        float inner; \
        float acceleration; \
        float frequency; \
        float falloff; \
        ideatek_force_kind_t kind; \
        ideatek_field_shape_t shape; \
        ideatek_field_target_t target; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_force_field_t, IDEATEK_FORCE_FIELD_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_force_field_t, IDEATEK_FORCE_FIELD_FIELDS);

    #define IDEATEK_GRAVITY_AREA_FIELDS { \
        ideatek_vec3_t size; \
        float gravity; \
        float inner; \
        float falloff; \
        int32_t priority; \
        ideatek_gravity_mode_t mode; \
        ideatek_field_shape_t shape; \
        bool point; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_gravity_area_t, IDEATEK_GRAVITY_AREA_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_gravity_area_t, IDEATEK_GRAVITY_AREA_FIELDS);

#endif
