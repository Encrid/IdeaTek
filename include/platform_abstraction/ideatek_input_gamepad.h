/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_INPUT_GAMEPAD_H
    #define BC_IDEATEK_INPUT_GAMEPAD_H

    #include <stdint.h>

    #include "bridge_abi.h"
    #include "bridge_schema.h"
    #include "ideatek_world.h"

    #define IDEATEK_GAMEPAD_BUTTON_VALUES { \
        IDEATEK_GAMEPAD_BUTTON_SOUTH = 0x00001, \
        IDEATEK_GAMEPAD_BUTTON_EAST = 0x00002, \
        IDEATEK_GAMEPAD_BUTTON_WEST = 0x00004, \
        IDEATEK_GAMEPAD_BUTTON_NORTH = 0x00008, \
        IDEATEK_GAMEPAD_BUTTON_LEFT_SHOULDER = 0x00010, \
        IDEATEK_GAMEPAD_BUTTON_RIGHT_SHOULDER = 0x00020, \
        IDEATEK_GAMEPAD_BUTTON_BACK = 0x00100, \
        IDEATEK_GAMEPAD_BUTTON_START = 0x00200, \
        IDEATEK_GAMEPAD_BUTTON_LEFT_STICK = 0x00400, \
        IDEATEK_GAMEPAD_BUTTON_RIGHT_STICK = 0x00800, \
        IDEATEK_GAMEPAD_BUTTON_DPAD_UP = 0x01000, \
        IDEATEK_GAMEPAD_BUTTON_DPAD_DOWN = 0x02000, \
        IDEATEK_GAMEPAD_BUTTON_DPAD_LEFT = 0x04000, \
        IDEATEK_GAMEPAD_BUTTON_DPAD_RIGHT = 0x08000, \
        IDEATEK_GAMEPAD_BUTTON_GUIDE = 0x10000 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_gamepad_button_t, uint32_t,
    IDEATEK_GAMEPAD_BUTTON_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_gamepad_button_t, uint32_t,
    IDEATEK_GAMEPAD_BUTTON_VALUES);

    #define IDEATEK_GAMEPAD_FEATURES_VALUES { \
        IDEATEK_GAMEPAD_FEATURES_GYRO = 0x01, \
        IDEATEK_GAMEPAD_FEATURES_ACCEL = 0x02 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_gamepad_features_t, uint8_t,
    IDEATEK_GAMEPAD_FEATURES_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_gamepad_features_t, uint8_t,
    IDEATEK_GAMEPAD_FEATURES_VALUES);

    #define IDEATEK_GAMEPAD_FIELDS { \
        char name[379]; \
        ideatek_gamepad_features_t features; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_gamepad_t, IDEATEK_GAMEPAD_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_gamepad_t, IDEATEK_GAMEPAD_FIELDS);

    #define IDEATEK_GAMEPAD_STATE_FIELDS { \
        float left_x; \
        float left_y; \
        float right_x; \
        float right_y; \
        float left_trigger; \
        float right_trigger; \
        ideatek_gamepad_button_t buttons; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_gamepad_state_t,
    IDEATEK_GAMEPAD_STATE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_gamepad_state_t,
    IDEATEK_GAMEPAD_STATE_FIELDS);

    #define IDEATEK_GAMEPAD_EVENT_FIELDS { \
        ideatek_entity_t gamepad; \
        uint32_t order; \
        ideatek_gamepad_button_t button; \
        bool down; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_gamepad_event_t,
    IDEATEK_GAMEPAD_EVENT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_gamepad_event_t,
    IDEATEK_GAMEPAD_EVENT_FIELDS);

    #define IDEATEK_GAMEPAD_REQUEST_FIELDS { \
        float weak; \
        float strong; \
        uint32_t duration_ms; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_gamepad_request_t,
    IDEATEK_GAMEPAD_REQUEST_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_gamepad_request_t,
    IDEATEK_GAMEPAD_REQUEST_FIELDS);

    #define IDEATEK_GAMEPAD_SENSOR_REQUEST_FIELDS { \
        bool enabled; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_gamepad_sensor_request_t,
    IDEATEK_GAMEPAD_SENSOR_REQUEST_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_gamepad_sensor_request_t,
    IDEATEK_GAMEPAD_SENSOR_REQUEST_FIELDS);

    #define IDEATEK_GAMEPAD_MOTION_FIELDS { \
        float gyro[3]; \
        float accel[3]; \
        float turn[3]; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_gamepad_motion_t,
    IDEATEK_GAMEPAD_MOTION_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_gamepad_motion_t,
    IDEATEK_GAMEPAD_MOTION_FIELDS);

#endif
