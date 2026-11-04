/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_XR_H
    #define BC_IDEATEK_XR_H

    #include <stdint.h>

    #include "bridge_abi.h"
    #include "bridge_schema.h"
    #include "ideatek_space.h"

    #define IDEATEK_XR_SPACE_VALUES { \
        IDEATEK_XR_SPACE_LOCAL = 0, \
        IDEATEK_XR_SPACE_STAGE = 1, \
        IDEATEK_XR_SPACE_LOCAL_FLOOR = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_xr_space_t, uint8_t, IDEATEK_XR_SPACE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_xr_space_t, uint8_t, IDEATEK_XR_SPACE_VALUES);

    #define IDEATEK_XR_BLEND_VALUES { \
        IDEATEK_XR_BLEND_OPAQUE = 1, \
        IDEATEK_XR_BLEND_ADDITIVE = 2, \
        IDEATEK_XR_BLEND_ALPHA_BLEND = 3 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_xr_blend_t, uint8_t, IDEATEK_XR_BLEND_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_xr_blend_t, uint8_t, IDEATEK_XR_BLEND_VALUES);

    #define IDEATEK_XR_ORIGIN_FIELDS { \
        float scale; \
        ideatek_xr_space_t space; \
        ideatek_xr_blend_t blend; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_xr_origin_t, IDEATEK_XR_ORIGIN_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_xr_origin_t, IDEATEK_XR_ORIGIN_FIELDS);

    #define IDEATEK_XR_SESSION_STATE_VALUES { \
        IDEATEK_XR_SESSION_STATE_UNKNOWN = 0, \
        IDEATEK_XR_SESSION_STATE_IDLE = 1, \
        IDEATEK_XR_SESSION_STATE_READY = 2, \
        IDEATEK_XR_SESSION_STATE_SYNCHRONIZED = 3, \
        IDEATEK_XR_SESSION_STATE_VISIBLE = 4, \
        IDEATEK_XR_SESSION_STATE_FOCUSED = 5, \
        IDEATEK_XR_SESSION_STATE_STOPPING = 6, \
        IDEATEK_XR_SESSION_STATE_LOSS_PENDING = 7, \
        IDEATEK_XR_SESSION_STATE_EXITING = 8 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_xr_session_state_t,
    uint8_t, IDEATEK_XR_SESSION_STATE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_xr_session_state_t,
    uint8_t, IDEATEK_XR_SESSION_STATE_VALUES);

    #define IDEATEK_XR_BLENDS_VALUES { \
        IDEATEK_XR_BLENDS_OPAQUE = 0x02, \
        IDEATEK_XR_BLENDS_ADDITIVE = 0x04, \
        IDEATEK_XR_BLENDS_ALPHA_BLEND = 0x08 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_xr_blends_t,
    uint8_t, IDEATEK_XR_BLENDS_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_xr_blends_t,
    uint8_t, IDEATEK_XR_BLENDS_VALUES);

    #define IDEATEK_XR_SESSION_FIELDS { \
        float refresh; \
        ideatek_xr_session_state_t state; \
        ideatek_xr_blend_t blend; \
        ideatek_xr_blends_t blends; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_xr_session_t, IDEATEK_XR_SESSION_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_xr_session_t, IDEATEK_XR_SESSION_FIELDS);

    #define IDEATEK_XR_TRACKER_KIND_VALUES { \
        IDEATEK_XR_TRACKER_KIND_HEAD = 0, \
        IDEATEK_XR_TRACKER_KIND_CONTROLLER = 1, \
        IDEATEK_XR_TRACKER_KIND_TRACKER = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_xr_tracker_kind_t,
    uint8_t, IDEATEK_XR_TRACKER_KIND_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_xr_tracker_kind_t,
    uint8_t, IDEATEK_XR_TRACKER_KIND_VALUES);

    #define IDEATEK_XR_HAND_VALUES { \
        IDEATEK_XR_HAND_UNKNOWN = 0, \
        IDEATEK_XR_HAND_LEFT = 1, \
        IDEATEK_XR_HAND_RIGHT = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_xr_hand_t, uint8_t, IDEATEK_XR_HAND_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_xr_hand_t, uint8_t, IDEATEK_XR_HAND_VALUES);

    #define IDEATEK_XR_POSE_VALUES { \
        IDEATEK_XR_POSE_GRIP = 0, \
        IDEATEK_XR_POSE_AIM = 1 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_xr_pose_t, uint8_t, IDEATEK_XR_POSE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_xr_pose_t, uint8_t, IDEATEK_XR_POSE_VALUES);

    #define IDEATEK_XR_TRACKER_FIELDS { \
        ideatek_xr_tracker_kind_t kind; \
        ideatek_xr_hand_t hand; \
        ideatek_xr_pose_t pose; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_xr_tracker_t, IDEATEK_XR_TRACKER_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_xr_tracker_t, IDEATEK_XR_TRACKER_FIELDS);

    #define IDEATEK_XR_LOCATION_VALUES { \
        IDEATEK_XR_LOCATION_ORIENTATION_VALID = 0x01, \
        IDEATEK_XR_LOCATION_POSITION_VALID = 0x02, \
        IDEATEK_XR_LOCATION_ORIENTATION_TRACKED = 0x04, \
        IDEATEK_XR_LOCATION_POSITION_TRACKED = 0x08 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_xr_location_t,
    uint8_t, IDEATEK_XR_LOCATION_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_xr_location_t,
    uint8_t, IDEATEK_XR_LOCATION_VALUES);

    #define IDEATEK_XR_POSE_STATE_FIELDS { \
        ideatek_vec3_t linear; \
        ideatek_vec3_t angular; \
        ideatek_xr_location_t flags; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_xr_pose_state_t,
    IDEATEK_XR_POSE_STATE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_xr_pose_state_t,
    IDEATEK_XR_POSE_STATE_FIELDS);

    #define IDEATEK_XR_BINDING_PATH_FIELDS { \
        bridge_text_t profile; \
        bridge_text_t path; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_xr_binding_path_t,
    IDEATEK_XR_BINDING_PATH_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_xr_binding_path_t,
    IDEATEK_XR_BINDING_PATH_FIELDS);

    #define IDEATEK_XR_CONTROL_STATE_FIELDS { \
        float x; \
        float y; \
        bool active; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_xr_control_state_t,
    IDEATEK_XR_CONTROL_STATE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_xr_control_state_t,
    IDEATEK_XR_CONTROL_STATE_FIELDS);

    #define IDEATEK_XR_HAND_RECORD_FIELDS { \
        ideatek_xr_hand_t hand; \
        bool active; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_xr_hand_record_t,
    IDEATEK_XR_HAND_RECORD_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_xr_hand_record_t,
    IDEATEK_XR_HAND_RECORD_FIELDS);

    #define IDEATEK_XR_JOINT_STATE_FIELDS { \
        float radius; \
        ideatek_xr_location_t flags; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_xr_joint_state_t,
    IDEATEK_XR_JOINT_STATE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_xr_joint_state_t,
    IDEATEK_XR_JOINT_STATE_FIELDS);

    #define IDEATEK_XR_HAPTIC_FIELDS { \
        float amplitude; \
        float frequency; \
        uint32_t duration_ms; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_xr_haptic_t, IDEATEK_XR_HAPTIC_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_xr_haptic_t, IDEATEK_XR_HAPTIC_FIELDS);

    #define IDEATEK_XR_PLAY_AREA_FIELDS { \
        float width; \
        float depth; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_xr_play_area_t, IDEATEK_XR_PLAY_AREA_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_xr_play_area_t, IDEATEK_XR_PLAY_AREA_FIELDS);

#endif
