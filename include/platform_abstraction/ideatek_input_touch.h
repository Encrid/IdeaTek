/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_INPUT_TOUCH_H
    #define BC_IDEATEK_INPUT_TOUCH_H

    #include <stdint.h>

    #include "bridge_abi.h"
    #include "bridge_schema.h"

    #define IDEATEK_FINGER_EVENT_KIND_VALUES { \
        IDEATEK_FINGER_EVENT_KIND_BEGAN = 0, \
        IDEATEK_FINGER_EVENT_KIND_MOVED = 1, \
        IDEATEK_FINGER_EVENT_KIND_ENDED = 2, \
        IDEATEK_FINGER_EVENT_KIND_CANCELED = 3 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_finger_event_kind_t, uint8_t,
    IDEATEK_FINGER_EVENT_KIND_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_finger_event_kind_t, uint8_t,
    IDEATEK_FINGER_EVENT_KIND_VALUES);

    #define IDEATEK_FINGER_EVENT_FIELDS { \
        uint64_t id; \
        float x; \
        float y; \
        uint32_t order; \
        ideatek_finger_event_kind_t kind; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_finger_event_t,
    IDEATEK_FINGER_EVENT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_finger_event_t,
    IDEATEK_FINGER_EVENT_FIELDS);

#endif
