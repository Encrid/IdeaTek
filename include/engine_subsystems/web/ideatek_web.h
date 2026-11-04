/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_WEB_H
    #define BC_IDEATEK_WEB_H

    #include <stdint.h>

    #include "bridge_abi.h"
    #include "bridge_schema.h"

    #define IDEATEK_WEB_METHOD_VALUES { \
        IDEATEK_WEB_METHOD_GET = 0, \
        IDEATEK_WEB_METHOD_HEAD = 1, \
        IDEATEK_WEB_METHOD_POST = 2, \
        IDEATEK_WEB_METHOD_PUT = 3, \
        IDEATEK_WEB_METHOD_DELETE = 4, \
        IDEATEK_WEB_METHOD_PATCH = 5 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_web_method_t,
    uint8_t, IDEATEK_WEB_METHOD_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_web_method_t,
    uint8_t, IDEATEK_WEB_METHOD_VALUES);

    #define IDEATEK_WEB_REQUEST_FIELDS { \
        bridge_text_t url; \
        bridge_text_t headers; \
        bridge_text_t body; \
        float timeout; \
        uint32_t max_redirects; \
        ideatek_web_method_t method; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_web_request_t, IDEATEK_WEB_REQUEST_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_web_request_t, IDEATEK_WEB_REQUEST_FIELDS);

    #define IDEATEK_WEB_STATE_VALUES { \
        IDEATEK_WEB_STATE_PENDING = 0, \
        IDEATEK_WEB_STATE_DONE = 1, \
        IDEATEK_WEB_STATE_FAILED = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_web_state_t,
    uint8_t, IDEATEK_WEB_STATE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_web_state_t,
    uint8_t, IDEATEK_WEB_STATE_VALUES);

    #define IDEATEK_WEB_ERROR_VALUES { \
        IDEATEK_WEB_ERROR_NONE = 0, \
        IDEATEK_WEB_ERROR_RESOLVE = 1, \
        IDEATEK_WEB_ERROR_CONNECT = 2, \
        IDEATEK_WEB_ERROR_TLS = 3, \
        IDEATEK_WEB_ERROR_TIMEOUT = 4, \
        IDEATEK_WEB_ERROR_TOO_BIG = 5 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_web_error_t,
    uint8_t, IDEATEK_WEB_ERROR_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_web_error_t,
    uint8_t, IDEATEK_WEB_ERROR_VALUES);

    #define IDEATEK_WEB_RESPONSE_FIELDS { \
        bridge_text_t headers; \
        bridge_text_t body; \
        uint16_t status; \
        ideatek_web_state_t state; \
        ideatek_web_error_t error; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_web_response_t, IDEATEK_WEB_RESPONSE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_web_response_t, IDEATEK_WEB_RESPONSE_FIELDS);

#endif
