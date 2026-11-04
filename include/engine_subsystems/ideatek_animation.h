/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_ANIMATION_H
    #define BC_IDEATEK_ANIMATION_H

    #include <stdint.h>

    #include "bridge_abi.h"
    #include "bridge_schema.h"
    #include "ideatek_world.h"
    #include "ideatek_space.h"

    #define IDEATEK_CHANNEL_PROPERTY_VALUES { \
        IDEATEK_CHANNEL_PROPERTY_TRANSLATION = 0, \
        IDEATEK_CHANNEL_PROPERTY_ROTATION = 1, \
        IDEATEK_CHANNEL_PROPERTY_SCALE = 2, \
        IDEATEK_CHANNEL_PROPERTY_WEIGHTS = 3, \
        IDEATEK_CHANNEL_PROPERTY_FLOAT = 4 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_channel_property_t,
    uint8_t, IDEATEK_CHANNEL_PROPERTY_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_channel_property_t,
    uint8_t, IDEATEK_CHANNEL_PROPERTY_VALUES);

    #define IDEATEK_INTERPOLATION_VALUES { \
        IDEATEK_INTERPOLATION_STEP = 0, \
        IDEATEK_INTERPOLATION_LINEAR = 1, \
        IDEATEK_INTERPOLATION_CUBIC_SPLINE = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_interpolation_t,
    uint8_t, IDEATEK_INTERPOLATION_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_interpolation_t,
    uint8_t, IDEATEK_INTERPOLATION_VALUES);

    #define IDEATEK_LAYER_BLEND_VALUES { \
        IDEATEK_LAYER_BLEND_OVERLAY = 0, \
        IDEATEK_LAYER_BLEND_ADDITIVE = 1, \
        IDEATEK_LAYER_BLEND_MODEL_SPACE = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_layer_blend_t,
    uint8_t, IDEATEK_LAYER_BLEND_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_layer_blend_t,
    uint8_t, IDEATEK_LAYER_BLEND_VALUES);

    #define IDEATEK_CLIP_EVENT_KIND_VALUES { \
        IDEATEK_CLIP_EVENT_KIND_GENERIC = 0, \
        IDEATEK_CLIP_EVENT_KIND_SYNC = 1, \
        IDEATEK_CLIP_EVENT_KIND_WARP = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_clip_event_kind_t,
    uint8_t, IDEATEK_CLIP_EVENT_KIND_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_clip_event_kind_t,
    uint8_t, IDEATEK_CLIP_EVENT_KIND_VALUES);

    #define IDEATEK_PARAMETER_TYPE_VALUES { \
        IDEATEK_PARAMETER_TYPE_BOOL = 0, \
        IDEATEK_PARAMETER_TYPE_ID = 1, \
        IDEATEK_PARAMETER_TYPE_FLOAT = 2, \
        IDEATEK_PARAMETER_TYPE_VECTOR = 3, \
        IDEATEK_PARAMETER_TYPE_TARGET = 4 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_parameter_type_t,
    uint8_t, IDEATEK_PARAMETER_TYPE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_parameter_type_t,
    uint8_t, IDEATEK_PARAMETER_TYPE_VALUES);

    #define IDEATEK_CLIP_CHANNEL_FIELDS { \
        uint64_t target; \
        uint32_t key_count; \
        uint32_t times; \
        uint32_t values; \
        ideatek_channel_property_t property; \
        ideatek_interpolation_t interpolation; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_clip_channel_t, IDEATEK_CLIP_CHANNEL_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_clip_channel_t, IDEATEK_CLIP_CHANNEL_FIELDS);

    #define IDEATEK_CLIP_EVENT_FIELDS { \
        uint64_t name; \
        float time; \
        float duration; \
        float value; \
        ideatek_clip_event_kind_t kind; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_clip_event_t, IDEATEK_CLIP_EVENT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_clip_event_t, IDEATEK_CLIP_EVENT_FIELDS);

    #define IDEATEK_REPEAT_VALUES { \
        IDEATEK_REPEAT_ONCE = 0, \
        IDEATEK_REPEAT_LOOP = 1, \
        IDEATEK_REPEAT_PING_PONG = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_repeat_t, uint8_t, IDEATEK_REPEAT_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_repeat_t, uint8_t, IDEATEK_REPEAT_VALUES);

    #define IDEATEK_STATE_SWITCH_VALUES { \
        IDEATEK_STATE_SWITCH_IMMEDIATE = 0, \
        IDEATEK_STATE_SWITCH_AT_END = 1 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_state_switch_t,
    uint8_t, IDEATEK_STATE_SWITCH_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_state_switch_t,
    uint8_t, IDEATEK_STATE_SWITCH_VALUES);

    #define IDEATEK_EVENT_PHASE_VALUES { \
        IDEATEK_EVENT_PHASE_BEGIN = 0, \
        IDEATEK_EVENT_PHASE_END = 1 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_event_phase_t,
    uint8_t, IDEATEK_EVENT_PHASE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_event_phase_t,
    uint8_t, IDEATEK_EVENT_PHASE_VALUES);

    #define IDEATEK_MOTION_AXES_VALUES { \
        IDEATEK_MOTION_AXES_X = 0x01, \
        IDEATEK_MOTION_AXES_Y = 0x02, \
        IDEATEK_MOTION_AXES_Z = 0x04, \
        IDEATEK_MOTION_AXES_YAW = 0x08, \
        IDEATEK_MOTION_AXES_PITCH = 0x10, \
        IDEATEK_MOTION_AXES_ROLL = 0x20 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_motion_axes_t,
    uint8_t, IDEATEK_MOTION_AXES_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_motion_axes_t,
    uint8_t, IDEATEK_MOTION_AXES_VALUES);

    #define IDEATEK_AXIS_VALUES { \
        IDEATEK_AXIS_POSITIVE_X = 0, \
        IDEATEK_AXIS_NEGATIVE_X = 1, \
        IDEATEK_AXIS_POSITIVE_Y = 2, \
        IDEATEK_AXIS_NEGATIVE_Y = 3, \
        IDEATEK_AXIS_POSITIVE_Z = 4, \
        IDEATEK_AXIS_NEGATIVE_Z = 5 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_axis_t, uint8_t, IDEATEK_AXIS_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_axis_t, uint8_t, IDEATEK_AXIS_VALUES);

    #define IDEATEK_COLLIDER_SHAPE_VALUES { \
        IDEATEK_COLLIDER_SHAPE_SPHERE = 0, \
        IDEATEK_COLLIDER_SHAPE_CAPSULE = 1, \
        IDEATEK_COLLIDER_SHAPE_PLANE = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_collider_shape_t,
    uint8_t, IDEATEK_COLLIDER_SHAPE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_collider_shape_t,
    uint8_t, IDEATEK_COLLIDER_SHAPE_VALUES);

    #define IDEATEK_JOINT_FIELDS { \
        ideatek_entity_t skeleton; \
        uint32_t index; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_joint_t, IDEATEK_JOINT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_joint_t, IDEATEK_JOINT_FIELDS);

    #define IDEATEK_SKIN_FIELDS { \
        ideatek_entity_t skeleton; \
        ideatek_entity_t binds; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_skin_t, IDEATEK_SKIN_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_skin_t, IDEATEK_SKIN_FIELDS);

    #define IDEATEK_SKIN_BINDS_FIELDS { \
        bridge_text_t matrices; \
        uint32_t count; \
        uint32_t revision; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_skin_binds_t, IDEATEK_SKIN_BINDS_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_skin_binds_t, IDEATEK_SKIN_BINDS_FIELDS);

    #define IDEATEK_MORPH_WEIGHTS_FIELDS { \
        bridge_text_t weights; \
        uint32_t count; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_morph_weights_t,
    IDEATEK_MORPH_WEIGHTS_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_morph_weights_t,
    IDEATEK_MORPH_WEIGHTS_FIELDS);

    #define IDEATEK_CLIP_FIELDS { \
        bridge_text_t bytes; \
        float duration; \
        uint32_t channel_count; \
        uint32_t event_count; \
        uint32_t revision; \
        bool additive; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_clip_t, IDEATEK_CLIP_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_clip_t, IDEATEK_CLIP_FIELDS);

    #define IDEATEK_ANIMATION_TARGET_FIELDS { \
        uint64_t id; \
        ideatek_entity_t player; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_animation_target_t,
    IDEATEK_ANIMATION_TARGET_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_animation_target_t,
    IDEATEK_ANIMATION_TARGET_FIELDS);

    #define IDEATEK_CLIP_PLAY_FIELDS { \
        ideatek_entity_t player; \
        ideatek_entity_t clip; \
        ideatek_entity_t mask; \
        uint64_t sync_group; \
        float time; \
        float speed; \
        float weight; \
        float event_weight; \
        int32_t layer; \
        ideatek_repeat_t repeat; \
        bool paused; \
        bool real_time; \
        ideatek_layer_blend_t blend; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_clip_play_t, IDEATEK_CLIP_PLAY_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_clip_play_t, IDEATEK_CLIP_PLAY_FIELDS);

    #define IDEATEK_BLEND_SPACE_FIELDS { \
        ideatek_entity_t player; \
        float x; \
        float y; \
        uint8_t dimensions; \
        bool synced; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_blend_space_t, IDEATEK_BLEND_SPACE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_blend_space_t, IDEATEK_BLEND_SPACE_FIELDS);

    #define IDEATEK_BLEND_POINT_FIELDS { \
        ideatek_entity_t space; \
        ideatek_entity_t clip; \
        float x; \
        float y; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_blend_point_t, IDEATEK_BLEND_POINT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_blend_point_t, IDEATEK_BLEND_POINT_FIELDS);

    #define IDEATEK_ANIMATION_STATES_FIELDS { \
        ideatek_entity_t player; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_animation_states_t,
    IDEATEK_ANIMATION_STATES_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_animation_states_t,
    IDEATEK_ANIMATION_STATES_FIELDS);

    #define IDEATEK_ANIMATION_STATE_FIELDS { \
        ideatek_entity_t machine; \
        ideatek_entity_t source; \
        uint32_t value; \
        float fade; \
        ideatek_state_switch_t when; \
        bool inertial; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_animation_state_t,
    IDEATEK_ANIMATION_STATE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_animation_state_t,
    IDEATEK_ANIMATION_STATE_FIELDS);

    #define IDEATEK_CLIP_EVENT_RECORD_FIELDS { \
        ideatek_entity_t play; \
        ideatek_entity_t player; \
        uint64_t name; \
        float value; \
        uint32_t order; \
        ideatek_event_phase_t phase; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_clip_event_record_t,
    IDEATEK_CLIP_EVENT_RECORD_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_clip_event_record_t,
    IDEATEK_CLIP_EVENT_RECORD_FIELDS);

    #define IDEATEK_ROOT_MOTION_FIELDS { \
        uint64_t root; \
        ideatek_motion_axes_t axes; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_root_motion_t, IDEATEK_ROOT_MOTION_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_root_motion_t, IDEATEK_ROOT_MOTION_FIELDS);

    #define IDEATEK_ROOT_MOTION_DELTA_FIELDS { \
        ideatek_quat_t rotation; \
        ideatek_vec3_t translation; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_root_motion_delta_t,
    IDEATEK_ROOT_MOTION_DELTA_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_root_motion_delta_t,
    IDEATEK_ROOT_MOTION_DELTA_FIELDS);

    #define IDEATEK_POSE_MODIFIER_FIELDS { \
        ideatek_entity_t player; \
        float influence; \
        int32_t order; \
        bool active; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_pose_modifier_t,
    IDEATEK_POSE_MODIFIER_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_pose_modifier_t,
    IDEATEK_POSE_MODIFIER_FIELDS);

    #define IDEATEK_TWO_BONE_IK_FIELDS { \
        ideatek_entity_t root; \
        ideatek_entity_t end; \
        ideatek_entity_t target; \
        ideatek_entity_t pole; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_two_bone_ik_t, IDEATEK_TWO_BONE_IK_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_two_bone_ik_t, IDEATEK_TWO_BONE_IK_FIELDS);

    #define IDEATEK_CHAIN_IK_FIELDS { \
        ideatek_entity_t root; \
        ideatek_entity_t end; \
        ideatek_entity_t target; \
        uint32_t iterations; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_chain_ik_t, IDEATEK_CHAIN_IK_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_chain_ik_t, IDEATEK_CHAIN_IK_FIELDS);

    #define IDEATEK_LOOK_AT_FIELDS { \
        ideatek_entity_t joint; \
        ideatek_entity_t target; \
        float max_angle; \
        ideatek_axis_t forward; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_look_at_t, IDEATEK_LOOK_AT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_look_at_t, IDEATEK_LOOK_AT_FIELDS);

    #define IDEATEK_COPY_TRANSFORM_FIELDS { \
        ideatek_entity_t joint; \
        ideatek_entity_t source; \
        ideatek_motion_axes_t axes; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_copy_transform_t,
    IDEATEK_COPY_TRANSFORM_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_copy_transform_t,
    IDEATEK_COPY_TRANSFORM_FIELDS);

    #define IDEATEK_SPRING_CHAIN_FIELDS { \
        ideatek_entity_t root; \
        ideatek_entity_t end; \
        ideatek_entity_t center; \
        ideatek_vec3_t gravity_direction; \
        float stiffness; \
        float drag; \
        float gravity; \
        float radius; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_spring_chain_t, IDEATEK_SPRING_CHAIN_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_spring_chain_t, IDEATEK_SPRING_CHAIN_FIELDS);

    #define IDEATEK_SPRING_COLLIDER_FIELDS { \
        ideatek_entity_t modifier; \
        float radius; \
        float height; \
        ideatek_collider_shape_t shape; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_spring_collider_t,
    IDEATEK_SPRING_COLLIDER_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_spring_collider_t,
    IDEATEK_SPRING_COLLIDER_FIELDS);

    #define IDEATEK_BONE_MASK_FIELDS { \
        bridge_text_t weights; \
        uint32_t count; \
        uint32_t revision; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_bone_mask_t, IDEATEK_BONE_MASK_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_bone_mask_t, IDEATEK_BONE_MASK_FIELDS);

    #define IDEATEK_ANIMATION_CURVES_FIELDS { \
        bridge_text_t values; \
        uint32_t count; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_animation_curves_t,
    IDEATEK_ANIMATION_CURVES_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_animation_curves_t,
    IDEATEK_ANIMATION_CURVES_FIELDS);

    #define IDEATEK_ANIMATION_PARAMETERS_FIELDS { \
        bridge_text_t values; \
        uint32_t count; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_animation_parameters_t,
    IDEATEK_ANIMATION_PARAMETERS_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_animation_parameters_t,
    IDEATEK_ANIMATION_PARAMETERS_FIELDS);

    #define IDEATEK_WARP_TARGETS_FIELDS { \
        bridge_text_t targets; \
        uint32_t count; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_warp_targets_t, IDEATEK_WARP_TARGETS_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_warp_targets_t, IDEATEK_WARP_TARGETS_FIELDS);

    #define IDEATEK_ORIENTATION_WARP_FIELDS { \
        ideatek_vec3_t direction; \
        float max_angle; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_orientation_warp_t,
    IDEATEK_ORIENTATION_WARP_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_orientation_warp_t,
    IDEATEK_ORIENTATION_WARP_FIELDS);

    #define IDEATEK_ANIMATION_LOD_FIELDS { \
        uint32_t joint_count; \
        uint8_t interval; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_animation_lod_t,
    IDEATEK_ANIMATION_LOD_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_animation_lod_t,
    IDEATEK_ANIMATION_LOD_FIELDS);

    #define IDEATEK_TRAJECTORY_FIELDS { \
        bridge_text_t samples; \
        uint32_t count; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_trajectory_t, IDEATEK_TRAJECTORY_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_trajectory_t, IDEATEK_TRAJECTORY_FIELDS);

    #define IDEATEK_MOTION_MATCH_FIELDS { \
        ideatek_entity_t player; \
        ideatek_entity_t database; \
        float blend_time; \
        float search_interval; \
        bool inertial; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_motion_match_t, IDEATEK_MOTION_MATCH_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_motion_match_t, IDEATEK_MOTION_MATCH_FIELDS);

    #define IDEATEK_INERTIALIZE_FIELDS { \
        float duration; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_inertialize_t, IDEATEK_INERTIALIZE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_inertialize_t, IDEATEK_INERTIALIZE_FIELDS);

    #define IDEATEK_PLAY_LINK_FIELDS { \
        ideatek_entity_t parent; \
        float start; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_play_link_t, IDEATEK_PLAY_LINK_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_play_link_t, IDEATEK_PLAY_LINK_FIELDS);

#endif
