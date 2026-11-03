/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_INPUT_H
    #define BC_IDEATEK_INPUT_H

    #include <stdint.h>

    #include "bridge_abi.h"
    #include "bridge_schema.h"
    #include "ideatek_world.h"

    #define IDEATEK_DEVICE_KIND_VALUES { \
        IDEATEK_DEVICE_KIND_KEYBOARD = 0, \
        IDEATEK_DEVICE_KIND_MOUSE = 1, \
        IDEATEK_DEVICE_KIND_JOYSTICK = 2, \
        IDEATEK_DEVICE_KIND_TOUCH = 3, \
        IDEATEK_DEVICE_KIND_PEN = 4, \
        IDEATEK_DEVICE_KIND_SENSOR = 5 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_device_kind_t, uint8_t,
    IDEATEK_DEVICE_KIND_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_device_kind_t, uint8_t,
    IDEATEK_DEVICE_KIND_VALUES);

    #define IDEATEK_POWER_VALUES { \
        IDEATEK_POWER_UNKNOWN = 0, \
        IDEATEK_POWER_ON_BATTERY = 1, \
        IDEATEK_POWER_NO_BATTERY = 2, \
        IDEATEK_POWER_CHARGING = 3, \
        IDEATEK_POWER_CHARGED = 4 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_power_t, uint8_t, IDEATEK_POWER_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_power_t, uint8_t, IDEATEK_POWER_VALUES);

    #define IDEATEK_LINK_VALUES { \
        IDEATEK_LINK_UNKNOWN = 0, \
        IDEATEK_LINK_WIRED = 1, \
        IDEATEK_LINK_WIRELESS = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_link_t, uint8_t, IDEATEK_LINK_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_link_t, uint8_t, IDEATEK_LINK_VALUES);

    #define IDEATEK_INPUT_EVENT_KIND_VALUES { \
        IDEATEK_INPUT_EVENT_KIND_CONNECTED = 0, \
        IDEATEK_INPUT_EVENT_KIND_DISCONNECTED = 1, \
        IDEATEK_INPUT_EVENT_KIND_POWER = 2, \
        IDEATEK_INPUT_EVENT_KIND_USER = 3, \
        IDEATEK_INPUT_EVENT_KIND_REMAPPED = 4, \
        IDEATEK_INPUT_EVENT_KIND_LAYOUT = 5, \
        IDEATEK_INPUT_EVENT_KIND_FOCUS_GAINED = 6, \
        IDEATEK_INPUT_EVENT_KIND_FOCUS_LOST = 7 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_input_event_kind_t, uint8_t,
    IDEATEK_INPUT_EVENT_KIND_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_input_event_kind_t, uint8_t,
    IDEATEK_INPUT_EVENT_KIND_VALUES);

    #define IDEATEK_INPUT_DEVICE_FIELDS { \
        bridge_text_t name; \
        bridge_text_t serial; \
        uint16_t vendor; \
        uint16_t product; \
        uint16_t version; \
        uint8_t guid[16]; \
        ideatek_device_kind_t kind; \
        bool rumble; \
        bool trigger_rumble; \
        bool mono_led; \
        bool rgb_led; \
        bool player_led; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_input_device_t, IDEATEK_INPUT_DEVICE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_input_device_t, IDEATEK_INPUT_DEVICE_FIELDS);

    #define IDEATEK_INPUT_USER_FIELDS { \
        uint32_t user; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_input_user_t, IDEATEK_INPUT_USER_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_input_user_t, IDEATEK_INPUT_USER_FIELDS);

    #define IDEATEK_DEVICE_STATE_FIELDS { \
        int32_t player_index; \
        int32_t percent; \
        ideatek_power_t power; \
        ideatek_link_t link; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_device_state_t, IDEATEK_DEVICE_STATE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_device_state_t, IDEATEK_DEVICE_STATE_FIELDS);

    #define IDEATEK_INPUT_EVENT_FIELDS { \
        ideatek_entity_t device; \
        ideatek_entity_t window; \
        uint32_t user; \
        uint32_t old_user; \
        uint32_t order; \
        ideatek_input_event_kind_t kind; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_input_event_t, IDEATEK_INPUT_EVENT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_input_event_t, IDEATEK_INPUT_EVENT_FIELDS);

#endif
