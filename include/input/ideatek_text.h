/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_TEXT_H
    #define BC_IDEATEK_TEXT_H

    #include <stdint.h>

    #include "bridge_abi.h"
    #include "bridge_schema.h"
    #include "ideatek_world.h"

    #define IDEATEK_TEXT_TYPE_VALUES { \
        IDEATEK_TEXT_TYPE_TEXT = 0, \
        IDEATEK_TEXT_TYPE_TEXT_NAME = 1, \
        IDEATEK_TEXT_TYPE_TEXT_EMAIL = 2, \
        IDEATEK_TEXT_TYPE_TEXT_USERNAME = 3, \
        IDEATEK_TEXT_TYPE_TEXT_PASSWORD_HIDDEN = 4, \
        IDEATEK_TEXT_TYPE_TEXT_PASSWORD_VISIBLE = 5, \
        IDEATEK_TEXT_TYPE_NUMBER = 6, \
        IDEATEK_TEXT_TYPE_NUMBER_PASSWORD_HIDDEN = 7, \
        IDEATEK_TEXT_TYPE_NUMBER_PASSWORD_VISIBLE = 8 \
    }

    #define IDEATEK_CAPITALIZE_VALUES { \
        IDEATEK_CAPITALIZE_NONE = 0, \
        IDEATEK_CAPITALIZE_SENTENCES = 1, \
        IDEATEK_CAPITALIZE_WORDS = 2, \
        IDEATEK_CAPITALIZE_LETTERS = 3 \
    }

    #define IDEATEK_TEXT_EVENT_KIND_VALUES { \
        IDEATEK_TEXT_EVENT_KIND_COMMIT = 0, \
        IDEATEK_TEXT_EVENT_KIND_COMPOSITION = 1, \
        IDEATEK_TEXT_EVENT_KIND_CANDIDATE = 2, \
        IDEATEK_TEXT_EVENT_KIND_SCREEN_KEYBOARD_SHOWN = 3, \
        IDEATEK_TEXT_EVENT_KIND_SCREEN_KEYBOARD_HIDDEN = 4 \
    }

    #define IDEATEK_TEXT_REQUEST_FIELDS { \
        ideatek_entity_t window; \
        int32_t x; \
        int32_t y; \
        uint32_t width; \
        uint32_t height; \
        int32_t cursor; \
        uint32_t clears; \
        ideatek_text_type_t type; \
        ideatek_capitalize_t capitalize; \
        bool enabled; \
        bool autocorrect; \
        bool multiline; \
        bool draws_composition; \
        bool draws_candidates; \
    }

    #define IDEATEK_TEXT_EVENT_FIELDS { \
        ideatek_entity_t keyboard; \
        ideatek_entity_t window; \
        bridge_text_t text; \
        int32_t start; \
        int32_t length; \
        uint32_t index; \
        uint32_t count; \
        int32_t selected; \
        uint32_t order; \
        ideatek_text_event_kind_t kind; \
        bool horizontal; \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_text_type_t, uint8_t, IDEATEK_TEXT_TYPE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_text_type_t, uint8_t, IDEATEK_TEXT_TYPE_VALUES);
BRIDGE_SCHEMA_ENUM_TYPE(ideatek_capitalize_t, uint8_t,
    IDEATEK_CAPITALIZE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_capitalize_t, uint8_t,
    IDEATEK_CAPITALIZE_VALUES);
BRIDGE_SCHEMA_ENUM_TYPE(ideatek_text_event_kind_t, uint8_t,
    IDEATEK_TEXT_EVENT_KIND_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_text_event_kind_t, uint8_t,
    IDEATEK_TEXT_EVENT_KIND_VALUES);
BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_text_request_t, IDEATEK_TEXT_REQUEST_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_text_request_t, IDEATEK_TEXT_REQUEST_FIELDS);
BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_text_event_t, IDEATEK_TEXT_EVENT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_text_event_t, IDEATEK_TEXT_EVENT_FIELDS);
#endif
