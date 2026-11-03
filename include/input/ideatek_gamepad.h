/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_GAMEPAD_H
    #define BC_IDEATEK_GAMEPAD_H

    #include <stdint.h>

    #include "bridge_abi.h"
    #include "bridge_schema.h"
    #include "ideatek_world.h"

    #define IDEATEK_JOYSTICK_TYPE_VALUES { \
        IDEATEK_JOYSTICK_TYPE_UNKNOWN = 0, \
        IDEATEK_JOYSTICK_TYPE_GAMEPAD = 1, \
        IDEATEK_JOYSTICK_TYPE_WHEEL = 2, \
        IDEATEK_JOYSTICK_TYPE_ARCADE_STICK = 3, \
        IDEATEK_JOYSTICK_TYPE_FLIGHT_STICK = 4, \
        IDEATEK_JOYSTICK_TYPE_DANCE_PAD = 5, \
        IDEATEK_JOYSTICK_TYPE_GUITAR = 6, \
        IDEATEK_JOYSTICK_TYPE_DRUM_KIT = 7, \
        IDEATEK_JOYSTICK_TYPE_ARCADE_PAD = 8, \
        IDEATEK_JOYSTICK_TYPE_THROTTLE = 9 \
    }

    #define IDEATEK_HAT_VALUES { \
        IDEATEK_HAT_UP = 0x01, \
        IDEATEK_HAT_RIGHT = 0x02, \
        IDEATEK_HAT_DOWN = 0x04, \
        IDEATEK_HAT_LEFT = 0x08 \
    }

    #define IDEATEK_JOYSTICK_EVENT_KIND_VALUES { \
        IDEATEK_JOYSTICK_EVENT_KIND_AXIS = 0, \
        IDEATEK_JOYSTICK_EVENT_KIND_BUTTON = 1, \
        IDEATEK_JOYSTICK_EVENT_KIND_HAT = 2, \
        IDEATEK_JOYSTICK_EVENT_KIND_BALL = 3 \
    }

    #define IDEATEK_GAMEPAD_TYPE_VALUES { \
        IDEATEK_GAMEPAD_TYPE_UNKNOWN = 0, \
        IDEATEK_GAMEPAD_TYPE_STANDARD = 1, \
        IDEATEK_GAMEPAD_TYPE_XBOX360 = 2, \
        IDEATEK_GAMEPAD_TYPE_XBOXONE = 3, \
        IDEATEK_GAMEPAD_TYPE_PS3 = 4, \
        IDEATEK_GAMEPAD_TYPE_PS4 = 5, \
        IDEATEK_GAMEPAD_TYPE_PS5 = 6, \
        IDEATEK_GAMEPAD_TYPE_NINTENDO_SWITCH_PRO = 7, \
        IDEATEK_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_LEFT = 8, \
        IDEATEK_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_RIGHT = 9, \
        IDEATEK_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_PAIR = 10, \
        IDEATEK_GAMEPAD_TYPE_GAMECUBE = 11, \
        IDEATEK_GAMEPAD_TYPE_STEAM = 12 \
    }

    #define IDEATEK_FACE_LABEL_VALUES { \
        IDEATEK_FACE_LABEL_UNKNOWN = 0, \
        IDEATEK_FACE_LABEL_A = 1, \
        IDEATEK_FACE_LABEL_B = 2, \
        IDEATEK_FACE_LABEL_X = 3, \
        IDEATEK_FACE_LABEL_Y = 4, \
        IDEATEK_FACE_LABEL_CROSS = 5, \
        IDEATEK_FACE_LABEL_CIRCLE = 6, \
        IDEATEK_FACE_LABEL_SQUARE = 7, \
        IDEATEK_FACE_LABEL_TRIANGLE = 8 \
    }

    #define IDEATEK_GAMEPAD_BUTTON_VALUES { \
        IDEATEK_GAMEPAD_BUTTON_SOUTH = 0, \
        IDEATEK_GAMEPAD_BUTTON_EAST = 1, \
        IDEATEK_GAMEPAD_BUTTON_WEST = 2, \
        IDEATEK_GAMEPAD_BUTTON_NORTH = 3, \
        IDEATEK_GAMEPAD_BUTTON_BACK = 4, \
        IDEATEK_GAMEPAD_BUTTON_GUIDE = 5, \
        IDEATEK_GAMEPAD_BUTTON_START = 6, \
        IDEATEK_GAMEPAD_BUTTON_LEFT_STICK = 7, \
        IDEATEK_GAMEPAD_BUTTON_RIGHT_STICK = 8, \
        IDEATEK_GAMEPAD_BUTTON_LEFT_SHOULDER = 9, \
        IDEATEK_GAMEPAD_BUTTON_RIGHT_SHOULDER = 10, \
        IDEATEK_GAMEPAD_BUTTON_DPAD_UP = 11, \
        IDEATEK_GAMEPAD_BUTTON_DPAD_DOWN = 12, \
        IDEATEK_GAMEPAD_BUTTON_DPAD_LEFT = 13, \
        IDEATEK_GAMEPAD_BUTTON_DPAD_RIGHT = 14, \
        IDEATEK_GAMEPAD_BUTTON_MISC1 = 15, \
        IDEATEK_GAMEPAD_BUTTON_RIGHT_PADDLE1 = 16, \
        IDEATEK_GAMEPAD_BUTTON_LEFT_PADDLE1 = 17, \
        IDEATEK_GAMEPAD_BUTTON_RIGHT_PADDLE2 = 18, \
        IDEATEK_GAMEPAD_BUTTON_LEFT_PADDLE2 = 19, \
        IDEATEK_GAMEPAD_BUTTON_TOUCHPAD = 20, \
        IDEATEK_GAMEPAD_BUTTON_MISC2 = 21, \
        IDEATEK_GAMEPAD_BUTTON_MISC3 = 22, \
        IDEATEK_GAMEPAD_BUTTON_MISC4 = 23, \
        IDEATEK_GAMEPAD_BUTTON_MISC5 = 24, \
        IDEATEK_GAMEPAD_BUTTON_MISC6 = 25 \
    }

    #define IDEATEK_GAMEPAD_AXIS_VALUES { \
        IDEATEK_GAMEPAD_AXIS_LEFTX = 0, \
        IDEATEK_GAMEPAD_AXIS_LEFTY = 1, \
        IDEATEK_GAMEPAD_AXIS_RIGHTX = 2, \
        IDEATEK_GAMEPAD_AXIS_RIGHTY = 3, \
        IDEATEK_GAMEPAD_AXIS_LEFT_TRIGGER = 4, \
        IDEATEK_GAMEPAD_AXIS_RIGHT_TRIGGER = 5 \
    }

    #define IDEATEK_CAPSENSE_VALUES { \
        IDEATEK_CAPSENSE_LEFT_STICK = 0, \
        IDEATEK_CAPSENSE_RIGHT_STICK = 1, \
        IDEATEK_CAPSENSE_LEFT_GRIP = 2, \
        IDEATEK_CAPSENSE_RIGHT_GRIP = 3 \
    }

    #define IDEATEK_GAMEPAD_EVENT_KIND_VALUES { \
        IDEATEK_GAMEPAD_EVENT_KIND_AXIS = 0, \
        IDEATEK_GAMEPAD_EVENT_KIND_BUTTON = 1, \
        IDEATEK_GAMEPAD_EVENT_KIND_CAPSENSE = 2 \
    }

    #define IDEATEK_JOYSTICK_FIELDS { \
        uint16_t axes; \
        uint16_t buttons; \
        uint16_t hats; \
        uint16_t balls; \
        ideatek_joystick_type_t type; \
    }

    #define IDEATEK_JOYSTICK_STATE_FIELDS { \
        uint64_t buttons[2]; \
        int16_t axes[16]; \
        ideatek_hat_t hats[4]; \
    }

    #define IDEATEK_JOYSTICK_EVENT_FIELDS { \
        ideatek_entity_t joystick; \
        uint32_t order; \
        uint32_t index; \
        int16_t value; \
        int16_t rel_x; \
        int16_t rel_y; \
        ideatek_hat_t hat; \
        ideatek_joystick_event_kind_t kind; \
        bool down; \
    }

    #define IDEATEK_GAMEPAD_FIELDS { \
        ideatek_gamepad_type_t type; \
        ideatek_face_label_t labels[4]; \
    }

    #define IDEATEK_GAMEPAD_STATE_FIELDS { \
        float left_x; \
        float left_y; \
        float right_x; \
        float right_y; \
        float left_trigger; \
        float right_trigger; \
        uint32_t buttons; \
        bool pressed[26]; \
        uint8_t capsense; \
    }

    #define IDEATEK_GAMEPAD_EVENT_FIELDS { \
        ideatek_entity_t gamepad; \
        uint32_t order; \
        float value; \
        uint8_t index; \
        ideatek_gamepad_event_kind_t kind; \
        bool down; \
    }

    #define IDEATEK_JOYSTICK_REQUEST_FIELDS { \
        uint32_t duration_ms; \
        uint32_t trigger_duration_ms; \
        uint32_t rumbles; \
        uint32_t trigger_rumbles; \
        int32_t player_index; \
        float low; \
        float high; \
        float trigger_left; \
        float trigger_right; \
        uint8_t red; \
        uint8_t green; \
        uint8_t blue; \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_joystick_type_t, uint8_t,
    IDEATEK_JOYSTICK_TYPE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_joystick_type_t, uint8_t,
    IDEATEK_JOYSTICK_TYPE_VALUES);
BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_hat_t, uint8_t, IDEATEK_HAT_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_hat_t, uint8_t, IDEATEK_HAT_VALUES);
BRIDGE_SCHEMA_ENUM_TYPE(ideatek_joystick_event_kind_t, uint8_t,
    IDEATEK_JOYSTICK_EVENT_KIND_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_joystick_event_kind_t, uint8_t,
    IDEATEK_JOYSTICK_EVENT_KIND_VALUES);
BRIDGE_SCHEMA_ENUM_TYPE(ideatek_gamepad_type_t, uint8_t,
    IDEATEK_GAMEPAD_TYPE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_gamepad_type_t, uint8_t,
    IDEATEK_GAMEPAD_TYPE_VALUES);
BRIDGE_SCHEMA_ENUM_TYPE(ideatek_face_label_t, uint8_t,
    IDEATEK_FACE_LABEL_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_face_label_t, uint8_t,
    IDEATEK_FACE_LABEL_VALUES);
BRIDGE_SCHEMA_ENUM_TYPE(ideatek_gamepad_button_t, uint8_t,
    IDEATEK_GAMEPAD_BUTTON_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_gamepad_button_t, uint8_t,
    IDEATEK_GAMEPAD_BUTTON_VALUES);
BRIDGE_SCHEMA_ENUM_TYPE(ideatek_gamepad_axis_t, uint8_t,
    IDEATEK_GAMEPAD_AXIS_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_gamepad_axis_t, uint8_t,
    IDEATEK_GAMEPAD_AXIS_VALUES);
BRIDGE_SCHEMA_ENUM_TYPE(ideatek_capsense_t, uint8_t, IDEATEK_CAPSENSE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_capsense_t, uint8_t, IDEATEK_CAPSENSE_VALUES);
BRIDGE_SCHEMA_ENUM_TYPE(ideatek_gamepad_event_kind_t, uint8_t,
    IDEATEK_GAMEPAD_EVENT_KIND_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_gamepad_event_kind_t, uint8_t,
    IDEATEK_GAMEPAD_EVENT_KIND_VALUES);
BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_joystick_t, IDEATEK_JOYSTICK_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_joystick_t, IDEATEK_JOYSTICK_FIELDS);
BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_joystick_state_t,
    IDEATEK_JOYSTICK_STATE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_joystick_state_t,
    IDEATEK_JOYSTICK_STATE_FIELDS);
BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_joystick_event_t,
    IDEATEK_JOYSTICK_EVENT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_joystick_event_t,
    IDEATEK_JOYSTICK_EVENT_FIELDS);
BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_gamepad_t, IDEATEK_GAMEPAD_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_gamepad_t, IDEATEK_GAMEPAD_FIELDS);
BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_gamepad_state_t,
    IDEATEK_GAMEPAD_STATE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_gamepad_state_t,
    IDEATEK_GAMEPAD_STATE_FIELDS);
BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_gamepad_event_t,
    IDEATEK_GAMEPAD_EVENT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_gamepad_event_t,
    IDEATEK_GAMEPAD_EVENT_FIELDS);
BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_joystick_request_t,
    IDEATEK_JOYSTICK_REQUEST_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_joystick_request_t,
    IDEATEK_JOYSTICK_REQUEST_FIELDS);
#endif
