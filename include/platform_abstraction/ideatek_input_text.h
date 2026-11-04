/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_INPUT_TEXT_H
    #define BC_IDEATEK_INPUT_TEXT_H

    #include <stdint.h>

    #include "bridge_abi.h"
    #include "bridge_schema.h"

    #define IDEATEK_TEXT_REQUEST_FIELDS { \
        bool screen_keyboard; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_text_request_t,
    IDEATEK_TEXT_REQUEST_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_text_request_t,
    IDEATEK_TEXT_REQUEST_FIELDS);

    #define IDEATEK_TEXT_EVENT_FIELDS { \
        uint32_t code_point; \
        uint32_t order; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_text_event_t, IDEATEK_TEXT_EVENT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_text_event_t, IDEATEK_TEXT_EVENT_FIELDS);

#endif
