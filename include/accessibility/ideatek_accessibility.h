/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_ACCESSIBILITY_H
    #define BC_IDEATEK_ACCESSIBILITY_H

    #include <stdint.h>

    #include "bridge_abi.h"
    #include "bridge_schema.h"

    #define IDEATEK_THEME_VALUES { \
        IDEATEK_THEME_UNKNOWN = 0, \
        IDEATEK_THEME_LIGHT = 1, \
        IDEATEK_THEME_DARK = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_theme_t, uint8_t, IDEATEK_THEME_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_theme_t, uint8_t, IDEATEK_THEME_VALUES);

    #define IDEATEK_ACCESSIBILITY_STATE_FIELDS { \
        ideatek_theme_t theme; \
        int8_t increase_contrast; \
        int8_t reduce_animation; \
        int8_t reduce_transparency; \
        int8_t screen_reader; \
        bool speech; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_accessibility_state_t,
    IDEATEK_ACCESSIBILITY_STATE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_accessibility_state_t,
    IDEATEK_ACCESSIBILITY_STATE_FIELDS);

    #define IDEATEK_SPEECH_REQUEST_FIELDS { \
        bridge_text_t text; \
        bool interrupt; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_speech_request_t,
    IDEATEK_SPEECH_REQUEST_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_speech_request_t,
    IDEATEK_SPEECH_REQUEST_FIELDS);

#endif
