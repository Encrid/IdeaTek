/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_LOCALIZATION_H
    #define BC_IDEATEK_LOCALIZATION_H

    #include <stdint.h>

    #include "bridge_abi.h"
    #include "bridge_schema.h"
    #include "ideatek_world.h"

    #define IDEATEK_LOCALE_FIELDS { \
        bridge_text_t code; \
        bridge_text_t fallback; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_locale_t, IDEATEK_LOCALE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_locale_t, IDEATEK_LOCALE_FIELDS);

    #define IDEATEK_TEXT_SYNTAX_VALUES { \
        IDEATEK_TEXT_SYNTAX_ICU = 0, \
        IDEATEK_TEXT_SYNTAX_FLUENT = 1 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_text_syntax_t,
    uint8_t, IDEATEK_TEXT_SYNTAX_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_text_syntax_t,
    uint8_t, IDEATEK_TEXT_SYNTAX_VALUES);

    #define IDEATEK_STRING_TABLE_FIELDS { \
        bridge_text_t bytes; \
        bridge_text_t locale; \
        uint64_t domain; \
        uint32_t count; \
        uint32_t revision; \
        ideatek_text_syntax_t syntax; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_string_table_t, IDEATEK_STRING_TABLE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_string_table_t, IDEATEK_STRING_TABLE_FIELDS);

    #define IDEATEK_TEXT_ARG_VALUES { \
        IDEATEK_TEXT_ARG_INT = 0, \
        IDEATEK_TEXT_ARG_FLOAT = 1, \
        IDEATEK_TEXT_ARG_TEXT = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_text_arg_t, uint8_t, IDEATEK_TEXT_ARG_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_text_arg_t, uint8_t, IDEATEK_TEXT_ARG_VALUES);

    #define IDEATEK_TEXT_KEY_FIELDS { \
        bridge_text_t args; \
        uint64_t key; \
        uint64_t source_hash; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_text_key_t, IDEATEK_TEXT_KEY_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_text_key_t, IDEATEK_TEXT_KEY_FIELDS);

    #define IDEATEK_LOCALIZED_TEXT_FIELDS { \
        bridge_text_t text; \
        bridge_text_t locale; \
        bool missing; \
        bool stale; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_localized_text_t,
    IDEATEK_LOCALIZED_TEXT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_localized_text_t,
    IDEATEK_LOCALIZED_TEXT_FIELDS);

#endif
