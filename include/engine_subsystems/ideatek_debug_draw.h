/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_DEBUG_DRAW_H
    #define BC_IDEATEK_DEBUG_DRAW_H

    #include <stdint.h>

    #include "bridge_abi.h"
    #include "bridge_schema.h"

    #define IDEATEK_DEBUG_KIND_VALUES { \
        IDEATEK_DEBUG_KIND_LINE = 0, \
        IDEATEK_DEBUG_KIND_POINT = 1, \
        IDEATEK_DEBUG_KIND_BOX = 2, \
        IDEATEK_DEBUG_KIND_SPHERE = 3, \
        IDEATEK_DEBUG_KIND_CAPSULE = 4, \
        IDEATEK_DEBUG_KIND_ARROW = 5, \
        IDEATEK_DEBUG_KIND_AXES = 6, \
        IDEATEK_DEBUG_KIND_TEXT = 7 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_debug_kind_t,
    uint8_t, IDEATEK_DEBUG_KIND_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_debug_kind_t,
    uint8_t, IDEATEK_DEBUG_KIND_VALUES);

    #define IDEATEK_DEBUG_FLAGS_VALUES { \
        IDEATEK_DEBUG_FLAGS_ON_TOP = 0x01, \
        IDEATEK_DEBUG_FLAGS_SCREEN = 0x02 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_debug_flags_t,
    uint8_t, IDEATEK_DEBUG_FLAGS_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_debug_flags_t,
    uint8_t, IDEATEK_DEBUG_FLAGS_VALUES);

    #define IDEATEK_DEBUG_DRAW_FIELDS { \
        uint64_t groups; \
        bridge_text_t data; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_debug_draw_t, IDEATEK_DEBUG_DRAW_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_debug_draw_t, IDEATEK_DEBUG_DRAW_FIELDS);

    #define IDEATEK_DEBUG_VIEW_FIELDS { \
        uint64_t groups; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_debug_view_t, IDEATEK_DEBUG_VIEW_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_debug_view_t, IDEATEK_DEBUG_VIEW_FIELDS);

#endif
