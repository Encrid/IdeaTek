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

    #define IDEATEK_GAMEPAD_BUTTON_VALUES { \
        IDEATEK_GAMEPAD_BUTTON_SOUTH = 0x0001, \
        IDEATEK_GAMEPAD_BUTTON_EAST = 0x0002, \
        IDEATEK_GAMEPAD_BUTTON_WEST = 0x0004, \
        IDEATEK_GAMEPAD_BUTTON_NORTH = 0x0008, \
        IDEATEK_GAMEPAD_BUTTON_BACK = 0x0010, \
        IDEATEK_GAMEPAD_BUTTON_START = 0x0020, \
        IDEATEK_GAMEPAD_BUTTON_GUIDE = 0x0040, \
        IDEATEK_GAMEPAD_BUTTON_LEFT_STICK = 0x0080, \
        IDEATEK_GAMEPAD_BUTTON_RIGHT_STICK = 0x0100, \
        IDEATEK_GAMEPAD_BUTTON_LEFT_SHOULDER = 0x0200, \
        IDEATEK_GAMEPAD_BUTTON_RIGHT_SHOULDER = 0x0400, \
        IDEATEK_GAMEPAD_BUTTON_DPAD_UP = 0x0800, \
        IDEATEK_GAMEPAD_BUTTON_DPAD_DOWN = 0x1000, \
        IDEATEK_GAMEPAD_BUTTON_DPAD_LEFT = 0x2000, \
        IDEATEK_GAMEPAD_BUTTON_DPAD_RIGHT = 0x4000 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_gamepad_button_t, uint16_t,
    IDEATEK_GAMEPAD_BUTTON_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_gamepad_button_t, uint16_t,
    IDEATEK_GAMEPAD_BUTTON_VALUES);

    #define IDEATEK_GAMEPAD_FIELDS { \
        char name[128]; \
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

#endif
