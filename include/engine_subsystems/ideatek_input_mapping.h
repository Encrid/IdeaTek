/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_INPUT_MAPPING_H
    #define BC_IDEATEK_INPUT_MAPPING_H

    #include <stdint.h>

    #include "bridge_abi.h"
    #include "bridge_schema.h"
    #include "ideatek_world.h"

    #define IDEATEK_ACTION_TYPE_VALUES { \
        IDEATEK_ACTION_TYPE_BUTTON = 0, \
        IDEATEK_ACTION_TYPE_AXIS_1 = 1, \
        IDEATEK_ACTION_TYPE_AXIS_2 = 2, \
        IDEATEK_ACTION_TYPE_AXIS_3 = 3 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_action_type_t,
    uint8_t, IDEATEK_ACTION_TYPE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_action_type_t,
    uint8_t, IDEATEK_ACTION_TYPE_VALUES);

    #define IDEATEK_ACCUMULATE_VALUES { \
        IDEATEK_ACCUMULATE_HIGHEST = 0, \
        IDEATEK_ACCUMULATE_SUM = 1 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_accumulate_t,
    uint8_t, IDEATEK_ACCUMULATE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_accumulate_t,
    uint8_t, IDEATEK_ACCUMULATE_VALUES);

    #define IDEATEK_INPUT_ACTION_FIELDS { \
        bridge_text_t name; \
        ideatek_action_type_t type; \
        ideatek_accumulate_t accumulate; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_input_action_t, IDEATEK_INPUT_ACTION_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_input_action_t, IDEATEK_INPUT_ACTION_FIELDS);

    #define IDEATEK_ACTION_STATE_VALUES { \
        IDEATEK_ACTION_STATE_IDLE = 0, \
        IDEATEK_ACTION_STATE_ONGOING = 1, \
        IDEATEK_ACTION_STATE_TRIGGERED = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_action_state_t,
    uint8_t, IDEATEK_ACTION_STATE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_action_state_t,
    uint8_t, IDEATEK_ACTION_STATE_VALUES);

    #define IDEATEK_ACTION_EDGES_VALUES { \
        IDEATEK_ACTION_EDGES_STARTED = 0x01, \
        IDEATEK_ACTION_EDGES_TRIGGERED = 0x02, \
        IDEATEK_ACTION_EDGES_CANCELED = 0x04, \
        IDEATEK_ACTION_EDGES_COMPLETED = 0x08 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_action_edges_t,
    uint8_t, IDEATEK_ACTION_EDGES_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_action_edges_t,
    uint8_t, IDEATEK_ACTION_EDGES_VALUES);

    #define IDEATEK_INPUT_ACTION_STATE_FIELDS { \
        float x; \
        float y; \
        float z; \
        float held; \
        ideatek_action_state_t state; \
        ideatek_action_edges_t edges; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_input_action_state_t,
    IDEATEK_INPUT_ACTION_STATE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_input_action_state_t,
    IDEATEK_INPUT_ACTION_STATE_FIELDS);

    #define IDEATEK_INPUT_SOURCE_VALUES { \
        IDEATEK_INPUT_SOURCE_KEY = 0, \
        IDEATEK_INPUT_SOURCE_KEY_LAYOUT = 1, \
        IDEATEK_INPUT_SOURCE_MOUSE_BUTTON = 2, \
        IDEATEK_INPUT_SOURCE_MOUSE_MOTION = 3, \
        IDEATEK_INPUT_SOURCE_MOUSE_WHEEL = 4, \
        IDEATEK_INPUT_SOURCE_GAMEPAD_BUTTON = 5, \
        IDEATEK_INPUT_SOURCE_GAMEPAD_STICK = 6, \
        IDEATEK_INPUT_SOURCE_GAMEPAD_TRIGGER = 7, \
        IDEATEK_INPUT_SOURCE_GAMEPAD_GYRO = 8, \
        IDEATEK_INPUT_SOURCE_GAMEPAD_ACCEL = 9, \
        IDEATEK_INPUT_SOURCE_TOUCH = 10, \
        IDEATEK_INPUT_SOURCE_TOUCH_MOTION = 11, \
        IDEATEK_INPUT_SOURCE_EXTERNAL = 12 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_input_source_t,
    uint8_t, IDEATEK_INPUT_SOURCE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_input_source_t,
    uint8_t, IDEATEK_INPUT_SOURCE_VALUES);

    #define IDEATEK_INPUT_BINDING_FIELDS { \
        ideatek_entity_t context; \
        ideatek_entity_t action; \
        uint32_t control; \
        ideatek_input_source_t source; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_input_binding_t,
    IDEATEK_INPUT_BINDING_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_input_binding_t,
    IDEATEK_INPUT_BINDING_FIELDS);

    #define IDEATEK_DEAD_ZONE_KIND_VALUES { \
        IDEATEK_DEAD_ZONE_KIND_AXIAL = 0, \
        IDEATEK_DEAD_ZONE_KIND_RADIAL = 1, \
        IDEATEK_DEAD_ZONE_KIND_UNSCALED_RADIAL = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_dead_zone_kind_t,
    uint8_t, IDEATEK_DEAD_ZONE_KIND_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_dead_zone_kind_t,
    uint8_t, IDEATEK_DEAD_ZONE_KIND_VALUES);

    #define IDEATEK_INPUT_DEAD_ZONE_FIELDS { \
        float lower; \
        float upper; \
        ideatek_dead_zone_kind_t kind; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_input_dead_zone_t,
    IDEATEK_INPUT_DEAD_ZONE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_input_dead_zone_t,
    IDEATEK_INPUT_DEAD_ZONE_FIELDS);

    #define IDEATEK_INPUT_CURVE_FIELDS { \
        float x; \
        float y; \
        float z; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_input_curve_t, IDEATEK_INPUT_CURVE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_input_curve_t, IDEATEK_INPUT_CURVE_FIELDS);

    #define IDEATEK_INPUT_SCALE_FIELDS { \
        float x; \
        float y; \
        float z; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_input_scale_t, IDEATEK_INPUT_SCALE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_input_scale_t, IDEATEK_INPUT_SCALE_FIELDS);

    #define IDEATEK_AXES_VALUES { \
        IDEATEK_AXES_X = 0x01, \
        IDEATEK_AXES_Y = 0x02, \
        IDEATEK_AXES_Z = 0x04 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_axes_t, uint8_t, IDEATEK_AXES_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_axes_t, uint8_t, IDEATEK_AXES_VALUES);

    #define IDEATEK_INPUT_NEGATE_FIELDS { \
        ideatek_axes_t axes; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_input_negate_t, IDEATEK_INPUT_NEGATE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_input_negate_t, IDEATEK_INPUT_NEGATE_FIELDS);

    #define IDEATEK_SWIZZLE_VALUES { \
        IDEATEK_SWIZZLE_YXZ = 0, \
        IDEATEK_SWIZZLE_ZYX = 1, \
        IDEATEK_SWIZZLE_XZY = 2, \
        IDEATEK_SWIZZLE_YZX = 3, \
        IDEATEK_SWIZZLE_ZXY = 4 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_swizzle_t, uint8_t, IDEATEK_SWIZZLE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_swizzle_t, uint8_t, IDEATEK_SWIZZLE_VALUES);

    #define IDEATEK_INPUT_SWIZZLE_FIELDS { \
        ideatek_swizzle_t order; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_input_swizzle_t,
    IDEATEK_INPUT_SWIZZLE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_input_swizzle_t,
    IDEATEK_INPUT_SWIZZLE_FIELDS);

    #define IDEATEK_INPUT_RESPONSE_CURVE_FIELDS { \
        bridge_text_t x_points; \
        bridge_text_t y_points; \
        bridge_text_t z_points; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_input_response_curve_t,
    IDEATEK_INPUT_RESPONSE_CURVE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_input_response_curve_t,
    IDEATEK_INPUT_RESPONSE_CURVE_FIELDS);

    #define IDEATEK_INPUT_CLAMP_FIELDS { \
        float min[3]; \
        float max[3]; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_input_clamp_t, IDEATEK_INPUT_CLAMP_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_input_clamp_t, IDEATEK_INPUT_CLAMP_FIELDS);

    #define IDEATEK_INPUT_SMOOTH_FIELDS { \
        float decay_rate; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_input_smooth_t, IDEATEK_INPUT_SMOOTH_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_input_smooth_t, IDEATEK_INPUT_SMOOTH_FIELDS);

    #define IDEATEK_INPUT_LINEAR_STEP_FIELDS { \
        float accel_rate; \
        float decel_rate; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_input_linear_step_t,
    IDEATEK_INPUT_LINEAR_STEP_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_input_linear_step_t,
    IDEATEK_INPUT_LINEAR_STEP_FIELDS);

    #define IDEATEK_INPUT_PRESSED_FIELDS { \
        float threshold; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_input_pressed_t,
    IDEATEK_INPUT_PRESSED_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_input_pressed_t,
    IDEATEK_INPUT_PRESSED_FIELDS);

    #define IDEATEK_INPUT_RELEASED_FIELDS { \
        float threshold; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_input_released_t,
    IDEATEK_INPUT_RELEASED_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_input_released_t,
    IDEATEK_INPUT_RELEASED_FIELDS);

    #define IDEATEK_HOLD_FLAGS_VALUES { \
        IDEATEK_HOLD_FLAGS_ONE_SHOT = 0x01 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_hold_flags_t,
    uint8_t, IDEATEK_HOLD_FLAGS_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_hold_flags_t,
    uint8_t, IDEATEK_HOLD_FLAGS_VALUES);

    #define IDEATEK_INPUT_HOLD_FIELDS { \
        float threshold; \
        float time; \
        ideatek_hold_flags_t flags; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_input_hold_t, IDEATEK_INPUT_HOLD_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_input_hold_t, IDEATEK_INPUT_HOLD_FIELDS);

    #define IDEATEK_INPUT_TAP_FIELDS { \
        float threshold; \
        float time; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_input_tap_t, IDEATEK_INPUT_TAP_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_input_tap_t, IDEATEK_INPUT_TAP_FIELDS);

    #define IDEATEK_PULSE_FLAGS_VALUES { \
        IDEATEK_PULSE_FLAGS_ON_START = 0x01 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_pulse_flags_t,
    uint8_t, IDEATEK_PULSE_FLAGS_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_pulse_flags_t,
    uint8_t, IDEATEK_PULSE_FLAGS_VALUES);

    #define IDEATEK_INPUT_PULSE_FIELDS { \
        float threshold; \
        float interval; \
        uint32_t limit; \
        ideatek_pulse_flags_t flags; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_input_pulse_t, IDEATEK_INPUT_PULSE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_input_pulse_t, IDEATEK_INPUT_PULSE_FIELDS);

    #define IDEATEK_INPUT_HOLD_RELEASE_FIELDS { \
        float threshold; \
        float time; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_input_hold_release_t,
    IDEATEK_INPUT_HOLD_RELEASE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_input_hold_release_t,
    IDEATEK_INPUT_HOLD_RELEASE_FIELDS);

    #define IDEATEK_INPUT_MULTI_TAP_FIELDS { \
        float threshold; \
        float time; \
        float gap; \
        uint32_t count; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_input_multi_tap_t,
    IDEATEK_INPUT_MULTI_TAP_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_input_multi_tap_t,
    IDEATEK_INPUT_MULTI_TAP_FIELDS);

    #define IDEATEK_INPUT_TOGGLE_FIELDS { \
        float threshold; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_input_toggle_t, IDEATEK_INPUT_TOGGLE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_input_toggle_t, IDEATEK_INPUT_TOGGLE_FIELDS);

    #define IDEATEK_INPUT_FLICK_FIELDS { \
        float threshold; \
        float rest; \
        float time; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_input_flick_t, IDEATEK_INPUT_FLICK_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_input_flick_t, IDEATEK_INPUT_FLICK_FIELDS);

    #define IDEATEK_INPUT_COOLDOWN_FIELDS { \
        float threshold; \
        float time; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_input_cooldown_t,
    IDEATEK_INPUT_COOLDOWN_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_input_cooldown_t,
    IDEATEK_INPUT_COOLDOWN_FIELDS);

    #define IDEATEK_CHORD_FLAGS_VALUES { \
        IDEATEK_CHORD_FLAGS_BLOCK = 0x01 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_chord_flags_t,
    uint8_t, IDEATEK_CHORD_FLAGS_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_chord_flags_t,
    uint8_t, IDEATEK_CHORD_FLAGS_VALUES);

    #define IDEATEK_INPUT_CHORD_FIELDS { \
        ideatek_entity_t action; \
        ideatek_chord_flags_t flags; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_input_chord_t, IDEATEK_INPUT_CHORD_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_input_chord_t, IDEATEK_INPUT_CHORD_FIELDS);

    #define IDEATEK_INPUT_COMBO_FIELDS { \
        bridge_text_t steps; \
        bridge_text_t cancels; \
        uint32_t steps_count; \
        uint32_t cancels_count; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_input_combo_t, IDEATEK_INPUT_COMBO_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_input_combo_t, IDEATEK_INPUT_COMBO_FIELDS);

    #define IDEATEK_INPUT_CARDINAL_FIELDS { \
        uint32_t up; \
        uint32_t down; \
        uint32_t left; \
        uint32_t right; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_input_cardinal_t,
    IDEATEK_INPUT_CARDINAL_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_input_cardinal_t,
    IDEATEK_INPUT_CARDINAL_FIELDS);

    #define IDEATEK_INPUT_EXTERNAL_FIELDS { \
        float x; \
        float y; \
        float z; \
        uint32_t origin; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_input_external_t,
    IDEATEK_INPUT_EXTERNAL_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_input_external_t,
    IDEATEK_INPUT_EXTERNAL_FIELDS);

    #define IDEATEK_INPUT_ACTION_ORIGINS_FIELDS { \
        bridge_text_t origins; \
        uint32_t count; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_input_action_origins_t,
    IDEATEK_INPUT_ACTION_ORIGINS_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_input_action_origins_t,
    IDEATEK_INPUT_ACTION_ORIGINS_FIELDS);

    #define IDEATEK_CONTEXT_FLAGS_VALUES { \
        IDEATEK_CONTEXT_FLAGS_ACTIVE = 0x01, \
        IDEATEK_CONTEXT_FLAGS_CONSUME = 0x02, \
        IDEATEK_CONTEXT_FLAGS_IGNORE_HELD = 0x04, \
        IDEATEK_CONTEXT_FLAGS_REAL_TIME = 0x08 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_context_flags_t,
    uint8_t, IDEATEK_CONTEXT_FLAGS_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_context_flags_t,
    uint8_t, IDEATEK_CONTEXT_FLAGS_VALUES);

    #define IDEATEK_INPUT_CONTEXT_FIELDS { \
        bridge_text_t name; \
        ideatek_entity_t player; \
        int32_t priority; \
        ideatek_context_flags_t flags; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_input_context_t,
    IDEATEK_INPUT_CONTEXT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_input_context_t,
    IDEATEK_INPUT_CONTEXT_FIELDS);

    #define IDEATEK_INPUT_PLAYER_FIELDS { \
        uint8_t flags; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_input_player_t, IDEATEK_INPUT_PLAYER_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_input_player_t, IDEATEK_INPUT_PLAYER_FIELDS);

    #define IDEATEK_INPUT_DEVICE_FIELDS { \
        ideatek_entity_t player; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_input_device_t, IDEATEK_INPUT_DEVICE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_input_device_t, IDEATEK_INPUT_DEVICE_FIELDS);

    #define IDEATEK_CAPTURE_SOURCES_VALUES { \
        IDEATEK_CAPTURE_SOURCES_KEY = 0x01, \
        IDEATEK_CAPTURE_SOURCES_KEY_LAYOUT = 0x02, \
        IDEATEK_CAPTURE_SOURCES_MOUSE_BUTTON = 0x04, \
        IDEATEK_CAPTURE_SOURCES_MOUSE_MOTION = 0x08, \
        IDEATEK_CAPTURE_SOURCES_MOUSE_WHEEL = 0x10, \
        IDEATEK_CAPTURE_SOURCES_GAMEPAD_BUTTON = 0x20, \
        IDEATEK_CAPTURE_SOURCES_GAMEPAD_STICK = 0x40, \
        IDEATEK_CAPTURE_SOURCES_GAMEPAD_TRIGGER = 0x80 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_capture_sources_t,
    uint8_t, IDEATEK_CAPTURE_SOURCES_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_capture_sources_t,
    uint8_t, IDEATEK_CAPTURE_SOURCES_VALUES);

    #define IDEATEK_INPUT_CAPTURE_FIELDS { \
        ideatek_entity_t player; \
        ideatek_capture_sources_t sources; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_input_capture_t,
    IDEATEK_INPUT_CAPTURE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_input_capture_t,
    IDEATEK_INPUT_CAPTURE_FIELDS);

    #define IDEATEK_INPUT_CAPTURED_FIELDS { \
        ideatek_entity_t capture; \
        ideatek_entity_t device; \
        uint32_t control; \
        uint32_t order; \
        ideatek_input_source_t source; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_input_captured_t,
    IDEATEK_INPUT_CAPTURED_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_input_captured_t,
    IDEATEK_INPUT_CAPTURED_FIELDS);

    #define IDEATEK_DEVICE_KIND_VALUES { \
        IDEATEK_DEVICE_KIND_KEYBOARD_MOUSE = 0, \
        IDEATEK_DEVICE_KIND_GAMEPAD = 1, \
        IDEATEK_DEVICE_KIND_TOUCH = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_device_kind_t,
    uint8_t, IDEATEK_DEVICE_KIND_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_device_kind_t,
    uint8_t, IDEATEK_DEVICE_KIND_VALUES);

    #define IDEATEK_INPUT_LAST_DEVICE_FIELDS { \
        ideatek_entity_t device; \
        ideatek_device_kind_t kind; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_input_last_device_t,
    IDEATEK_INPUT_LAST_DEVICE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_input_last_device_t,
    IDEATEK_INPUT_LAST_DEVICE_FIELDS);

#endif
