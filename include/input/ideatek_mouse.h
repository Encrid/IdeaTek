/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_MOUSE_H
    #define BC_IDEATEK_MOUSE_H

    #include <stdint.h>

    #include "bridge_abi.h"
    #include "bridge_schema.h"

    #define IDEATEK_MOUSE_MODE_VALUES { \
        IDEATEK_MOUSE_MODE_NORMAL = 0, \
        IDEATEK_MOUSE_MODE_HIDDEN = 1, \
        IDEATEK_MOUSE_MODE_RELATIVE = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_mouse_mode_t, uint8_t,
    IDEATEK_MOUSE_MODE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_mouse_mode_t, uint8_t,
    IDEATEK_MOUSE_MODE_VALUES);

    #define IDEATEK_CURSOR_VALUES { \
        IDEATEK_CURSOR_DEFAULT = 0, \
        IDEATEK_CURSOR_ARROW = 1, \
        IDEATEK_CURSOR_IBEAM = 2, \
        IDEATEK_CURSOR_CROSSHAIR = 3, \
        IDEATEK_CURSOR_POINTING_HAND = 4, \
        IDEATEK_CURSOR_RESIZE_EW = 5, \
        IDEATEK_CURSOR_RESIZE_NS = 6, \
        IDEATEK_CURSOR_RESIZE_NWSE = 7, \
        IDEATEK_CURSOR_RESIZE_NESW = 8, \
        IDEATEK_CURSOR_RESIZE_ALL = 9, \
        IDEATEK_CURSOR_NOT_ALLOWED = 10 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_cursor_t, uint8_t, IDEATEK_CURSOR_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_cursor_t, uint8_t, IDEATEK_CURSOR_VALUES);

    #define IDEATEK_MOUSE_FIELDS { \
        float x; \
        float y; \
        uint32_t buttons; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_mouse_t, IDEATEK_MOUSE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_mouse_t, IDEATEK_MOUSE_FIELDS);

    #define IDEATEK_MOUSE_MOTION_FIELDS { \
        float x; \
        float y; \
        float rel_x; \
        float rel_y; \
        uint32_t buttons; \
        uint32_t order; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_mouse_motion_t,
    IDEATEK_MOUSE_MOTION_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_mouse_motion_t,
    IDEATEK_MOUSE_MOTION_FIELDS);

    #define IDEATEK_MOUSE_BUTTON_FIELDS { \
        float x; \
        float y; \
        uint32_t order; \
        uint8_t button; \
        bool down; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_mouse_button_t,
    IDEATEK_MOUSE_BUTTON_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_mouse_button_t,
    IDEATEK_MOUSE_BUTTON_FIELDS);

    #define IDEATEK_MOUSE_WHEEL_FIELDS { \
        float ticks_x; \
        float ticks_y; \
        uint32_t order; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_mouse_wheel_t, IDEATEK_MOUSE_WHEEL_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_mouse_wheel_t, IDEATEK_MOUSE_WHEEL_FIELDS);

    #define IDEATEK_MOUSE_REQUEST_FIELDS { \
        ideatek_mouse_mode_t mode; \
        ideatek_cursor_t cursor; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_mouse_request_t,
    IDEATEK_MOUSE_REQUEST_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_mouse_request_t,
    IDEATEK_MOUSE_REQUEST_FIELDS);

#endif
