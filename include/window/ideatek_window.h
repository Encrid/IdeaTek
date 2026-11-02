/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_WINDOW_H
    #define BC_IDEATEK_WINDOW_H

    #include <stdint.h>

    #include "bridge_schema.h"
    #include "ideatek_space.h"

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
    #define IDEATEK_FLASH_VALUES { IDEATEK_FLASH_CANCEL = 0, \
        IDEATEK_FLASH_BRIEFLY = 1, IDEATEK_FLASH_UNTIL_FOCUSED = 2 }
    #define IDEATEK_ORIENTATION_VALUES { IDEATEK_ORIENTATION_UNKNOWN = 0, \
        IDEATEK_ORIENTATION_LANDSCAPE = 1, \
        IDEATEK_ORIENTATION_LANDSCAPE_FLIPPED = 2, \
        IDEATEK_ORIENTATION_PORTRAIT = 3, \
        IDEATEK_ORIENTATION_PORTRAIT_FLIPPED = 4 }
    #define IDEATEK_WINDOW_REQUEST_FIELDS { char title[128]; \
        ideatek_entity_t display; ideatek_entity_t video_mode; int32_t x; \
        int32_t y; uint32_t width; uint32_t height; uint32_t min_width; \
        uint32_t min_height; uint32_t max_width; uint32_t max_height; \
        uint32_t flashes; uint32_t raises; ideatek_place_t place; \
        ideatek_window_mode_t mode; ideatek_flash_t flash; bool resizable; \
        bool borderless; bool visible; bool always_on_top; }
    #define IDEATEK_WINDOW_STATE_FIELDS { uint64_t native_display; \
        uint64_t native_window; ideatek_entity_t display; \
        ideatek_entity_t video_mode; int32_t x; int32_t y; uint32_t width; \
        uint32_t height; uint32_t pixel_width; uint32_t pixel_height; \
        int32_t border_top; int32_t border_left; int32_t border_bottom; \
        int32_t border_right; int32_t safe_x; int32_t safe_y; \
        uint32_t safe_width; uint32_t safe_height; float display_scale; \
        float sdr_white_level; float hdr_headroom; \
        ideatek_window_mode_t mode; ideatek_surface_t surface; bool focused; \
        bool mouse_inside; bool occluded; bool visible; bool resizable; \
        bool borderless; bool always_on_top; bool hdr; bool close_asked; }
    #define IDEATEK_DISPLAY_FIELDS { char name[128]; \
        ideatek_entity_t desktop_mode; ideatek_entity_t current_mode; \
        int32_t x; int32_t y; uint32_t width; uint32_t height; \
        int32_t usable_x; int32_t usable_y; uint32_t usable_width; \
        uint32_t usable_height; float content_scale; \
        ideatek_orientation_t orientation; \
        ideatek_orientation_t natural_orientation; bool hdr; }
    #define IDEATEK_VIDEO_MODE_FIELDS { ideatek_entity_t display; \
        uint32_t width; uint32_t height; float pixel_density; \
        int32_t refresh_numerator; int32_t refresh_denominator; \
        uint32_t order; }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_place_t, uint8_t, IDEATEK_PLACE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_place_t, uint8_t, IDEATEK_PLACE_VALUES);
BRIDGE_SCHEMA_ENUM_TYPE(ideatek_window_mode_t, uint8_t,
    IDEATEK_WINDOW_MODE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_window_mode_t, uint8_t,
    IDEATEK_WINDOW_MODE_VALUES);
BRIDGE_SCHEMA_ENUM_TYPE(ideatek_surface_t, uint8_t, IDEATEK_SURFACE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_surface_t, uint8_t, IDEATEK_SURFACE_VALUES);
BRIDGE_SCHEMA_ENUM_TYPE(ideatek_flash_t, uint8_t, IDEATEK_FLASH_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_flash_t, uint8_t, IDEATEK_FLASH_VALUES);
BRIDGE_SCHEMA_ENUM_TYPE(ideatek_orientation_t, uint8_t,
    IDEATEK_ORIENTATION_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_orientation_t, uint8_t,
    IDEATEK_ORIENTATION_VALUES);
BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_window_request_t,
    IDEATEK_WINDOW_REQUEST_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_window_request_t,
    IDEATEK_WINDOW_REQUEST_FIELDS);
BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_window_state_t,
    IDEATEK_WINDOW_STATE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_window_state_t,
    IDEATEK_WINDOW_STATE_FIELDS);
BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_display_t, IDEATEK_DISPLAY_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_display_t, IDEATEK_DISPLAY_FIELDS);
BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_video_mode_t, IDEATEK_VIDEO_MODE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_video_mode_t, IDEATEK_VIDEO_MODE_FIELDS);

#endif
