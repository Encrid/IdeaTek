/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_WINDOW_H
    #define BC_IDEATEK_WINDOW_H

    #include <stdint.h>

    #include "bridge_abi.h"
    #include "bridge_schema.h"
    #include "ideatek_world.h"

    #define IDEATEK_WINDOW_MODE_VALUES { \
        IDEATEK_WINDOW_MODE_WINDOWED = 0, \
        IDEATEK_WINDOW_MODE_FULLSCREEN = 1, \
        IDEATEK_WINDOW_MODE_EXCLUSIVE_FULLSCREEN = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_window_mode_t, uint8_t,
    IDEATEK_WINDOW_MODE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_window_mode_t, uint8_t,
    IDEATEK_WINDOW_MODE_VALUES);

    #define IDEATEK_DISPLAY_TARGET_VALUES { \
        IDEATEK_DISPLAY_TARGET_DEFAULT = 0, \
        IDEATEK_DISPLAY_TARGET_CHOSEN = 1 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_display_target_t, uint8_t,
    IDEATEK_DISPLAY_TARGET_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_display_target_t, uint8_t,
    IDEATEK_DISPLAY_TARGET_VALUES);

    #define IDEATEK_SURFACE_VALUES { \
        IDEATEK_SURFACE_NONE = 0, \
        IDEATEK_SURFACE_WINDOWS = 1, \
        IDEATEK_SURFACE_METAL = 2, \
        IDEATEK_SURFACE_XLIB = 3, \
        IDEATEK_SURFACE_XCB = 4, \
        IDEATEK_SURFACE_WAYLAND = 5, \
        IDEATEK_SURFACE_ANDROID = 6, \
        IDEATEK_SURFACE_WEB = 7 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_surface_t, uint8_t, IDEATEK_SURFACE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_surface_t, uint8_t, IDEATEK_SURFACE_VALUES);

    #define IDEATEK_DROP_KIND_VALUES { \
        IDEATEK_DROP_FILE = 0, \
        IDEATEK_DROP_TEXT = 1 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_drop_kind_t, uint8_t,
    IDEATEK_DROP_KIND_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_drop_kind_t, uint8_t,
    IDEATEK_DROP_KIND_VALUES);

    #define IDEATEK_WINDOW_REQUEST_FIELDS { \
        char title[128]; \
        ideatek_entity_t display; \
        ideatek_entity_t video_mode; \
        uint32_t width; \
        uint32_t height; \
        ideatek_window_mode_t mode; \
        ideatek_display_target_t target; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_window_request_t,
    IDEATEK_WINDOW_REQUEST_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_window_request_t,
    IDEATEK_WINDOW_REQUEST_FIELDS);

    #define IDEATEK_WINDOW_STATE_FIELDS { \
        uint64_t native_display; \
        uint64_t native_window; \
        ideatek_entity_t display; \
        ideatek_entity_t video_mode; \
        uint32_t width; \
        uint32_t height; \
        uint32_t pixel_width; \
        uint32_t pixel_height; \
        float display_scale; \
        float sdr_white_level; \
        float hdr_headroom; \
        float drop_x; \
        float drop_y; \
        ideatek_window_mode_t mode; \
        ideatek_surface_t surface; \
        bool focused; \
        bool minimized; \
        bool hdr; \
        bool close_asked; \
        bool drop_over; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_window_state_t,
    IDEATEK_WINDOW_STATE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_window_state_t,
    IDEATEK_WINDOW_STATE_FIELDS);

    #define IDEATEK_DISPLAY_FIELDS { \
        char name[128]; \
        ideatek_entity_t desktop_mode; \
        ideatek_entity_t current_mode; \
        uint32_t width; \
        uint32_t height; \
        float content_scale; \
        bool hdr; \
        bool is_primary; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_display_t, IDEATEK_DISPLAY_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_display_t, IDEATEK_DISPLAY_FIELDS);

    #define IDEATEK_VIDEO_MODE_FIELDS { \
        ideatek_entity_t display; \
        uint32_t width; \
        uint32_t height; \
        float pixel_density; \
        int32_t refresh_numerator; \
        int32_t refresh_denominator; \
        uint32_t order; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_video_mode_t, IDEATEK_VIDEO_MODE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_video_mode_t, IDEATEK_VIDEO_MODE_FIELDS);

    #define IDEATEK_DROP_FIELDS { \
        ideatek_entity_t window; \
        bridge_text_t data; \
        bridge_text_t source; \
        uint32_t set; \
        uint32_t index; \
        float x; \
        float y; \
        ideatek_drop_kind_t kind; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_drop_t, IDEATEK_DROP_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_drop_t, IDEATEK_DROP_FIELDS);

#endif
