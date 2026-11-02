/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_WINDOW_H
    #define BC_IDEATEK_WINDOW_H

    #include <stdint.h>

    #include "bridge_schema.h"

    #define IDEATEK_PLACE_VALUES { IDEATEK_PLACE_SYSTEM = 0, \
        IDEATEK_PLACE_CENTER = 1, IDEATEK_PLACE_AT = 2 }
    #define IDEATEK_WINDOW_MODE_VALUES { IDEATEK_WINDOW_MODE_WINDOWED = 0, \
        IDEATEK_WINDOW_MODE_MINIMIZED = 1, IDEATEK_WINDOW_MODE_MAXIMIZED = 2, \
        IDEATEK_WINDOW_MODE_FULLSCREEN = 3, \
        IDEATEK_WINDOW_MODE_EXCLUSIVE_FULLSCREEN = 4 }
    #define IDEATEK_SURFACE_VALUES { IDEATEK_SURFACE_NONE = 0, \
        IDEATEK_SURFACE_WINDOWS = 1, IDEATEK_SURFACE_METAL = 2, \
        IDEATEK_SURFACE_XLIB = 3, IDEATEK_SURFACE_XCB = 4, \
        IDEATEK_SURFACE_WAYLAND = 5, IDEATEK_SURFACE_ANDROID = 6, \
        IDEATEK_SURFACE_WEB = 7 }
    #define IDEATEK_WINDOW_REQUEST_FIELDS { char title[128]; uint32_t width; \
        uint32_t height; int32_t x; int32_t y; uint32_t screen; \
        ideatek_place_t place; ideatek_window_mode_t mode; bool resizable; \
        bool borderless; }
    #define IDEATEK_WINDOW_STATE_FIELDS { uint64_t display; uint64_t window; \
        uint32_t width; uint32_t height; float scale; int32_t x; int32_t y; \
        uint32_t screen; ideatek_window_mode_t mode; bool focused; \
        bool close_asked; ideatek_surface_t surface; }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_place_t, uint8_t, IDEATEK_PLACE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_place_t, uint8_t, IDEATEK_PLACE_VALUES);
BRIDGE_SCHEMA_ENUM_TYPE(ideatek_window_mode_t, uint8_t,
    IDEATEK_WINDOW_MODE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_window_mode_t, uint8_t,
    IDEATEK_WINDOW_MODE_VALUES);
BRIDGE_SCHEMA_ENUM_TYPE(ideatek_surface_t, uint8_t, IDEATEK_SURFACE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_surface_t, uint8_t, IDEATEK_SURFACE_VALUES);
BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_window_request_t,
    IDEATEK_WINDOW_REQUEST_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_window_request_t,
    IDEATEK_WINDOW_REQUEST_FIELDS);
BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_window_state_t,
    IDEATEK_WINDOW_STATE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_window_state_t,
    IDEATEK_WINDOW_STATE_FIELDS);

#endif
