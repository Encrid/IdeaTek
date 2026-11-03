/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_PEN_H
    #define BC_IDEATEK_PEN_H

    #include <stdint.h>

    #include "bridge_abi.h"
    #include "bridge_schema.h"
    #include "ideatek_world.h"

    #define IDEATEK_PEN_TYPE_VALUES { \
        IDEATEK_PEN_TYPE_UNKNOWN = 0, \
        IDEATEK_PEN_TYPE_DIRECT = 1, \
        IDEATEK_PEN_TYPE_INDIRECT = 2 \
    }

    #define IDEATEK_PEN_AXIS_VALUES { \
        IDEATEK_PEN_AXIS_PRESSURE = 0, \
        IDEATEK_PEN_AXIS_XTILT = 1, \
        IDEATEK_PEN_AXIS_YTILT = 2, \
        IDEATEK_PEN_AXIS_DISTANCE = 3, \
        IDEATEK_PEN_AXIS_ROTATION = 4, \
        IDEATEK_PEN_AXIS_SLIDER = 5, \
        IDEATEK_PEN_AXIS_TANGENTIAL_PRESSURE = 6 \
    }

    #define IDEATEK_PEN_INPUT_VALUES { \
        IDEATEK_PEN_INPUT_DOWN = 0x00000001, \
        IDEATEK_PEN_INPUT_BUTTON_1 = 0x00000002, \
        IDEATEK_PEN_INPUT_BUTTON_2 = 0x00000004, \
        IDEATEK_PEN_INPUT_BUTTON_3 = 0x00000008, \
        IDEATEK_PEN_INPUT_BUTTON_4 = 0x00000010, \
        IDEATEK_PEN_INPUT_BUTTON_5 = 0x00000020, \
        IDEATEK_PEN_INPUT_ERASER_TIP = 0x40000000, \
        IDEATEK_PEN_INPUT_IN_PROXIMITY = 0x80000000 \
    }

    #define IDEATEK_PEN_EVENT_KIND_VALUES { \
        IDEATEK_PEN_EVENT_KIND_PROXIMITY_IN = 0, \
        IDEATEK_PEN_EVENT_KIND_PROXIMITY_OUT = 1, \
        IDEATEK_PEN_EVENT_KIND_TIP_DOWN = 2, \
        IDEATEK_PEN_EVENT_KIND_TIP_UP = 3, \
        IDEATEK_PEN_EVENT_KIND_BUTTON_DOWN = 4, \
        IDEATEK_PEN_EVENT_KIND_BUTTON_UP = 5, \
        IDEATEK_PEN_EVENT_KIND_MOTION = 6, \
        IDEATEK_PEN_EVENT_KIND_AXIS = 7 \
    }

    #define IDEATEK_PEN_FIELDS { \
        ideatek_pen_type_t type; \
    }

    #define IDEATEK_PEN_STATE_FIELDS { \
        ideatek_entity_t window; \
        float x; \
        float y; \
        float axes[7]; \
        ideatek_pen_input_t flags; \
    }

    #define IDEATEK_PEN_EVENT_FIELDS { \
        ideatek_entity_t pen; \
        ideatek_entity_t window; \
        float x; \
        float y; \
        float value; \
        ideatek_pen_input_t flags; \
        uint32_t order; \
        uint8_t button; \
        ideatek_pen_axis_t axis; \
        ideatek_pen_event_kind_t kind; \
        bool eraser; \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_pen_type_t, uint8_t, IDEATEK_PEN_TYPE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_pen_type_t, uint8_t, IDEATEK_PEN_TYPE_VALUES);
BRIDGE_SCHEMA_ENUM_TYPE(ideatek_pen_axis_t, uint8_t, IDEATEK_PEN_AXIS_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_pen_axis_t, uint8_t, IDEATEK_PEN_AXIS_VALUES);
BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_pen_input_t, uint32_t,
    IDEATEK_PEN_INPUT_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_pen_input_t, uint32_t,
    IDEATEK_PEN_INPUT_VALUES);
BRIDGE_SCHEMA_ENUM_TYPE(ideatek_pen_event_kind_t, uint8_t,
    IDEATEK_PEN_EVENT_KIND_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_pen_event_kind_t, uint8_t,
    IDEATEK_PEN_EVENT_KIND_VALUES);
BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_pen_t, IDEATEK_PEN_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_pen_t, IDEATEK_PEN_FIELDS);
BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_pen_state_t, IDEATEK_PEN_STATE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_pen_state_t, IDEATEK_PEN_STATE_FIELDS);
BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_pen_event_t, IDEATEK_PEN_EVENT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_pen_event_t, IDEATEK_PEN_EVENT_FIELDS);
#endif
