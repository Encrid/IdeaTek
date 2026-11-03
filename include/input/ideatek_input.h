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

    #define IDEATEK_INPUT_EVENT_KIND_VALUES { \
        IDEATEK_INPUT_EVENT_KIND_FOCUS_GAINED = 0, \
        IDEATEK_INPUT_EVENT_KIND_FOCUS_LOST = 1 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_input_event_kind_t, uint8_t,
    IDEATEK_INPUT_EVENT_KIND_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_input_event_kind_t, uint8_t,
    IDEATEK_INPUT_EVENT_KIND_VALUES);

    #define IDEATEK_INPUT_EVENT_FIELDS { \
        uint32_t order; \
        ideatek_input_event_kind_t kind; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_input_event_t, IDEATEK_INPUT_EVENT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_input_event_t, IDEATEK_INPUT_EVENT_FIELDS);

#endif
