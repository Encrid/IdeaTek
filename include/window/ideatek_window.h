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

    #define IDEATEK_PLACE_VALUES { \
        IDEATEK_PLACE_SYSTEM = 0, \
        IDEATEK_PLACE_CENTER = 1, \
        IDEATEK_PLACE_AT = 2 \
    }

    #define IDEATEK_WINDOW_MODE_VALUES { \
        IDEATEK_WINDOW_MODE_WINDOWED = 0, \
        IDEATEK_WINDOW_MODE_MINIMIZED = 1, \
        IDEATEK_WINDOW_MODE_MAXIMIZED = 2, \
        IDEATEK_WINDOW_MODE_FULLSCREEN = 3, \
        IDEATEK_WINDOW_MODE_EXCLUSIVE_FULLSCREEN = 4 \
    }

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

    #define IDEATEK_FLASH_VALUES { \
        IDEATEK_FLASH_CANCEL = 0, \
        IDEATEK_FLASH_BRIEFLY = 1, \
        IDEATEK_FLASH_UNTIL_FOCUSED = 2 \
    }

    #define IDEATEK_ORIENTATION_VALUES { \
        IDEATEK_ORIENTATION_UNKNOWN = 0, \
        IDEATEK_ORIENTATION_LANDSCAPE = 1, \
        IDEATEK_ORIENTATION_LANDSCAPE_FLIPPED = 2, \
        IDEATEK_ORIENTATION_PORTRAIT = 3, \
        IDEATEK_ORIENTATION_PORTRAIT_FLIPPED = 4 \
    }

    #define IDEATEK_PROGRESS_VALUES { \
        IDEATEK_PROGRESS_NONE = 0, \
        IDEATEK_PROGRESS_INDETERMINATE = 1, \
        IDEATEK_PROGRESS_NORMAL = 2, \
        IDEATEK_PROGRESS_PAUSED = 3, \
        IDEATEK_PROGRESS_ERROR = 4 \
    }

    #define IDEATEK_REGION_VALUES { \
        IDEATEK_REGION_NORMAL = 0, \
        IDEATEK_REGION_DRAG = 1, \
        IDEATEK_REGION_RESIZE_TOP_LEFT = 2, \
        IDEATEK_REGION_RESIZE_TOP = 3, \
        IDEATEK_REGION_RESIZE_TOP_RIGHT = 4, \
        IDEATEK_REGION_RESIZE_RIGHT = 5, \
        IDEATEK_REGION_RESIZE_BOTTOM_RIGHT = 6, \
        IDEATEK_REGION_RESIZE_BOTTOM = 7, \
        IDEATEK_REGION_RESIZE_BOTTOM_LEFT = 8, \
        IDEATEK_REGION_RESIZE_LEFT = 9 \
    }

    #define IDEATEK_DROP_KIND_VALUES { \
        IDEATEK_DROP_FILE = 0, \
        IDEATEK_DROP_TEXT = 1 \
    }

    #define IDEATEK_WINDOW_REQUEST_FIELDS { \
        char title[128]; \
        ideatek_entity_t display; \
        ideatek_entity_t video_mode; \
        int32_t x; \
        int32_t y; \
        uint32_t width; \
        uint32_t height; \
        uint32_t min_width; \
        uint32_t min_height; \
        uint32_t max_width; \
        uint32_t max_height; \
        uint32_t flashes; \
        uint32_t raises; \
        uint32_t menus; \
        int32_t menu_x; \
        int32_t menu_y; \
        float opacity; \
        float progress; \
        ideatek_place_t place; \
        ideatek_window_mode_t mode; \
        ideatek_flash_t flash; \
        ideatek_progress_t progress_state; \
        bool resizable; \
        bool borderless; \
        bool visible; \
        bool always_on_top; \
        bool transparent; \
        bool focusable; \
    }

    #define IDEATEK_WINDOW_STATE_FIELDS { \
        uint64_t native_display; \
        uint64_t native_window; \
        ideatek_entity_t display; \
        ideatek_entity_t video_mode; \
        int32_t x; \
        int32_t y; \
        uint32_t width; \
        uint32_t height; \
        uint32_t pixel_width; \
        uint32_t pixel_height; \
        int32_t border_top; \
        int32_t border_left; \
        int32_t border_bottom; \
        int32_t border_right; \
        int32_t safe_x; \
        int32_t safe_y; \
        uint32_t safe_width; \
        uint32_t safe_height; \
        float display_scale; \
        float sdr_white_level; \
        float hdr_headroom; \
        float opacity; \
        float drop_x; \
        float drop_y; \
        ideatek_window_mode_t mode; \
        ideatek_surface_t surface; \
        bool focused; \
        bool mouse_inside; \
        bool occluded; \
        bool visible; \
        bool resizable; \
        bool borderless; \
        bool always_on_top; \
        bool hdr; \
        bool close_asked; \
        bool transparent; \
        bool focusable; \
        bool progress_shown; \
        bool hit_test; \
        bool drop_over; \
    }

    #define IDEATEK_DISPLAY_FIELDS { \
        char name[128]; \
        ideatek_entity_t desktop_mode; \
        ideatek_entity_t current_mode; \
        int32_t x; \
        int32_t y; \
        uint32_t width; \
        uint32_t height; \
        int32_t usable_x; \
        int32_t usable_y; \
        uint32_t usable_width; \
        uint32_t usable_height; \
        float content_scale; \
        ideatek_orientation_t orientation; \
        ideatek_orientation_t natural_orientation; \
        bool hdr; \
    }

    #define IDEATEK_VIDEO_MODE_FIELDS { \
        ideatek_entity_t display; \
        uint32_t width; \
        uint32_t height; \
        float pixel_density; \
        int32_t refresh_numerator; \
        int32_t refresh_denominator; \
        uint32_t order; \
    }

    #define IDEATEK_WINDOW_REGION_FIELDS { \
        ideatek_entity_t window; \
        int32_t x; \
        int32_t y; \
        uint32_t width; \
        uint32_t height; \
        uint32_t order; \
        ideatek_region_t role; \
    }

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
BRIDGE_SCHEMA_ENUM_TYPE(ideatek_progress_t, uint8_t, IDEATEK_PROGRESS_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_progress_t, uint8_t, IDEATEK_PROGRESS_VALUES);
BRIDGE_SCHEMA_ENUM_TYPE(ideatek_region_t, uint8_t, IDEATEK_REGION_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_region_t, uint8_t, IDEATEK_REGION_VALUES);
BRIDGE_SCHEMA_ENUM_TYPE(ideatek_drop_kind_t, uint8_t,
    IDEATEK_DROP_KIND_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_drop_kind_t, uint8_t,
    IDEATEK_DROP_KIND_VALUES);
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
BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_window_region_t,
    IDEATEK_WINDOW_REGION_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_window_region_t,
    IDEATEK_WINDOW_REGION_FIELDS);
BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_drop_t, IDEATEK_DROP_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_drop_t, IDEATEK_DROP_FIELDS);

#endif
