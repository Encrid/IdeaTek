/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_SYSTEM_H
    #define BC_IDEATEK_SYSTEM_H

    #include <stdint.h>

    #include "bridge_abi.h"
    #include "bridge_schema.h"

    #define IDEATEK_SYSTEM_STATE_FIELDS { \
        bridge_text_t locales; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_system_state_t,
    IDEATEK_SYSTEM_STATE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_system_state_t,
    IDEATEK_SYSTEM_STATE_FIELDS);

    #define IDEATEK_URL_REQUEST_FIELDS { \
        bridge_text_t url; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_url_request_t, IDEATEK_URL_REQUEST_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_url_request_t, IDEATEK_URL_REQUEST_FIELDS);

    #define IDEATEK_MESSAGE_LEVEL_VALUES { \
        IDEATEK_MESSAGE_LEVEL_ERROR = 0, \
        IDEATEK_MESSAGE_LEVEL_WARNING = 1, \
        IDEATEK_MESSAGE_LEVEL_INFORMATION = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_message_level_t, uint8_t,
    IDEATEK_MESSAGE_LEVEL_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_message_level_t, uint8_t,
    IDEATEK_MESSAGE_LEVEL_VALUES);

    #define IDEATEK_MESSAGE_REQUEST_FIELDS { \
        bridge_text_t title; \
        bridge_text_t message; \
        ideatek_message_level_t level; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_message_request_t,
    IDEATEK_MESSAGE_REQUEST_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_message_request_t,
    IDEATEK_MESSAGE_REQUEST_FIELDS);

#endif
