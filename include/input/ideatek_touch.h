/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_TOUCH_H
    #define BC_IDEATEK_TOUCH_H

    #include <stdint.h>

    #include "bridge_abi.h"
    #include "bridge_schema.h"
    #include "ideatek_world.h"

    #define IDEATEK_TOUCH_TYPE_VALUES { \
        IDEATEK_TOUCH_TYPE_DIRECT = 0, \
        IDEATEK_TOUCH_TYPE_INDIRECT_ABSOLUTE = 1, \
        IDEATEK_TOUCH_TYPE_INDIRECT_RELATIVE = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_touch_type_t, uint8_t,
    IDEATEK_TOUCH_TYPE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_touch_type_t, uint8_t,
    IDEATEK_TOUCH_TYPE_VALUES);

    #define IDEATEK_FINGER_EVENT_KIND_VALUES { \
        IDEATEK_FINGER_EVENT_KIND_DOWN = 0, \
        IDEATEK_FINGER_EVENT_KIND_MOTION = 1, \
        IDEATEK_FINGER_EVENT_KIND_UP = 2, \
        IDEATEK_FINGER_EVENT_KIND_CANCELED = 3 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_finger_event_kind_t, uint8_t,
    IDEATEK_FINGER_EVENT_KIND_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_finger_event_kind_t, uint8_t,
    IDEATEK_FINGER_EVENT_KIND_VALUES);

    #define IDEATEK_PINCH_EVENT_KIND_VALUES { \
        IDEATEK_PINCH_EVENT_KIND_BEGIN = 0, \
        IDEATEK_PINCH_EVENT_KIND_UPDATE = 1, \
        IDEATEK_PINCH_EVENT_KIND_END = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_pinch_event_kind_t, uint8_t,
    IDEATEK_PINCH_EVENT_KIND_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_pinch_event_kind_t, uint8_t,
    IDEATEK_PINCH_EVENT_KIND_VALUES);

    #define IDEATEK_FINGER_FIELDS { \
        uint64_t id; \
        ideatek_entity_t window; \
        float x; \
        float y; \
        float pressure; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_finger_t, IDEATEK_FINGER_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_finger_t, IDEATEK_FINGER_FIELDS);

    #define IDEATEK_TOUCH_DEVICE_FIELDS { \
        ideatek_entity_t gamepad; \
        ideatek_touch_type_t type; \
        bool pinch; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_touch_device_t, IDEATEK_TOUCH_DEVICE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_touch_device_t, IDEATEK_TOUCH_DEVICE_FIELDS);

    #define IDEATEK_TOUCH_STATE_FIELDS { \
        bridge_text_t fingers; \
        uint32_t count; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_touch_state_t, IDEATEK_TOUCH_STATE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_touch_state_t, IDEATEK_TOUCH_STATE_FIELDS);

    #define IDEATEK_FINGER_EVENT_FIELDS { \
        uint64_t finger; \
        ideatek_entity_t touch; \
        ideatek_entity_t window; \
        float x; \
        float y; \
        float dx; \
        float dy; \
        float pressure; \
        uint32_t order; \
        ideatek_finger_event_kind_t kind; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_finger_event_t, IDEATEK_FINGER_EVENT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_finger_event_t, IDEATEK_FINGER_EVENT_FIELDS);

    #define IDEATEK_PINCH_EVENT_FIELDS { \
        ideatek_entity_t window; \
        float scale; \
        uint32_t order; \
        ideatek_pinch_event_kind_t kind; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_pinch_event_t, IDEATEK_PINCH_EVENT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_pinch_event_t, IDEATEK_PINCH_EVENT_FIELDS);

#endif
