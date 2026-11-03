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
    #include "ideatek_world.h"

    #define IDEATEK_SOURCE_VALUES { IDEATEK_SOURCE_MOUSE = 0, \
        IDEATEK_SOURCE_TOUCH = 1, IDEATEK_SOURCE_PEN = 2 }
    #define IDEATEK_MOUSE_MODE_VALUES { IDEATEK_MOUSE_MODE_NORMAL = 0, \
        IDEATEK_MOUSE_MODE_HIDDEN = 1, IDEATEK_MOUSE_MODE_CONFINED = 2, \
        IDEATEK_MOUSE_MODE_RELATIVE = 3 }
    #define IDEATEK_CURSOR_VALUES { IDEATEK_CURSOR_DEFAULT = 0, \
        IDEATEK_CURSOR_TEXT = 1, IDEATEK_CURSOR_WAIT = 2, \
        IDEATEK_CURSOR_CROSSHAIR = 3, IDEATEK_CURSOR_PROGRESS = 4, \
        IDEATEK_CURSOR_NWSE_RESIZE = 5, IDEATEK_CURSOR_NESW_RESIZE = 6, \
        IDEATEK_CURSOR_EW_RESIZE = 7, IDEATEK_CURSOR_NS_RESIZE = 8, \
        IDEATEK_CURSOR_MOVE = 9, IDEATEK_CURSOR_NOT_ALLOWED = 10, \
        IDEATEK_CURSOR_POINTER = 11, IDEATEK_CURSOR_NW_RESIZE = 12, \
        IDEATEK_CURSOR_N_RESIZE = 13, IDEATEK_CURSOR_NE_RESIZE = 14, \
        IDEATEK_CURSOR_E_RESIZE = 15, IDEATEK_CURSOR_SE_RESIZE = 16, \
        IDEATEK_CURSOR_S_RESIZE = 17, IDEATEK_CURSOR_SW_RESIZE = 18, \
        IDEATEK_CURSOR_W_RESIZE = 19, IDEATEK_CURSOR_CONTEXT_MENU = 20, \
        IDEATEK_CURSOR_HELP = 21, IDEATEK_CURSOR_CELL = 22, \
        IDEATEK_CURSOR_VERTICAL_TEXT = 23, IDEATEK_CURSOR_ALIAS = 24, \
        IDEATEK_CURSOR_COPY = 25, IDEATEK_CURSOR_NO_DROP = 26, \
        IDEATEK_CURSOR_GRAB = 27, IDEATEK_CURSOR_GRABBING = 28, \
        IDEATEK_CURSOR_COL_RESIZE = 29, IDEATEK_CURSOR_ROW_RESIZE = 30, \
        IDEATEK_CURSOR_ALL_SCROLL = 31, IDEATEK_CURSOR_ZOOM_IN = 32, \
        IDEATEK_CURSOR_ZOOM_OUT = 33 }
    #define IDEATEK_MOUSE_FIELDS { ideatek_entity_t window; float x; float y; \
        uint32_t buttons; bool relative; bool cursor_found; bool warp_works; }
    #define IDEATEK_MOUSE_MOTION_FIELDS { ideatek_entity_t mouse; \
        ideatek_entity_t window; float x; float y; float rel_x; float rel_y; \
        uint32_t buttons; uint32_t order; ideatek_source_t source; }
    #define IDEATEK_MOUSE_BUTTON_FIELDS { ideatek_entity_t mouse; \
        ideatek_entity_t window; float x; float y; uint32_t order; \
        uint8_t button; uint8_t clicks; ideatek_source_t source; bool down; }
    #define IDEATEK_MOUSE_WHEEL_FIELDS { ideatek_entity_t mouse; \
        ideatek_entity_t window; float x; float y; int32_t ticks_x; \
        int32_t ticks_y; float mouse_x; float mouse_y; uint32_t order; \
        bool flipped; }
    #define IDEATEK_MOUSE_REQUEST_FIELDS { ideatek_entity_t window; \
        bridge_text_t pixels; uint32_t width; uint32_t height; int32_t hot_x; \
        int32_t hot_y; float warp_x; float warp_y; uint32_t warps; \
        ideatek_mouse_mode_t mode; ideatek_cursor_t cursor; bool capture; \
        bool custom; }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_source_t, uint8_t, IDEATEK_SOURCE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_source_t, uint8_t, IDEATEK_SOURCE_VALUES);
BRIDGE_SCHEMA_ENUM_TYPE(ideatek_mouse_mode_t, uint8_t,
    IDEATEK_MOUSE_MODE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_mouse_mode_t, uint8_t,
    IDEATEK_MOUSE_MODE_VALUES);
BRIDGE_SCHEMA_ENUM_TYPE(ideatek_cursor_t, uint8_t, IDEATEK_CURSOR_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_cursor_t, uint8_t, IDEATEK_CURSOR_VALUES);
BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_mouse_t, IDEATEK_MOUSE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_mouse_t, IDEATEK_MOUSE_FIELDS);
BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_mouse_motion_t, IDEATEK_MOUSE_MOTION_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_mouse_motion_t, IDEATEK_MOUSE_MOTION_FIELDS);
BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_mouse_button_t, IDEATEK_MOUSE_BUTTON_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_mouse_button_t, IDEATEK_MOUSE_BUTTON_FIELDS);
BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_mouse_wheel_t, IDEATEK_MOUSE_WHEEL_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_mouse_wheel_t, IDEATEK_MOUSE_WHEEL_FIELDS);
BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_mouse_request_t,
    IDEATEK_MOUSE_REQUEST_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_mouse_request_t,
    IDEATEK_MOUSE_REQUEST_FIELDS);
#endif
