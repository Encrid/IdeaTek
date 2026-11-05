/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef BC_IDEATEK_UI_H
    #define BC_IDEATEK_UI_H

    #include <stdint.h>

    #include "bridge_abi.h"
    #include "bridge_schema.h"
    #include "ideatek_rendering.h"

    #define IDEATEK_UI_ROOT_FIELDS { \
        ideatek_entity_t target; \
        int32_t order; \
        float scale; \
        bool right_to_left; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_root_t, IDEATEK_UI_ROOT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_root_t, IDEATEK_UI_ROOT_FIELDS);

    #define IDEATEK_UI_NODE_FIELDS { \
        int32_t order; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_node_t, IDEATEK_UI_NODE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_node_t, IDEATEK_UI_NODE_FIELDS);

    #define IDEATEK_UI_UNIT_VALUES { \
        IDEATEK_UI_UNIT_AUTO = 0, \
        IDEATEK_UI_UNIT_PX = 1, \
        IDEATEK_UI_UNIT_PERCENT = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_ui_unit_t, uint8_t, IDEATEK_UI_UNIT_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_ui_unit_t, uint8_t, IDEATEK_UI_UNIT_VALUES);

    #define IDEATEK_UI_LENGTH_FIELDS { \
        float value; \
        ideatek_ui_unit_t unit; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_length_t, IDEATEK_UI_LENGTH_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_length_t, IDEATEK_UI_LENGTH_FIELDS);

    #define IDEATEK_UI_DISPLAY_VALUES { \
        IDEATEK_UI_DISPLAY_NONE = 0, \
        IDEATEK_UI_DISPLAY_FLEX = 1, \
        IDEATEK_UI_DISPLAY_GRID = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_ui_display_t,
    uint8_t, IDEATEK_UI_DISPLAY_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_ui_display_t,
    uint8_t, IDEATEK_UI_DISPLAY_VALUES);

    #define IDEATEK_UI_POSITION_VALUES { \
        IDEATEK_UI_POSITION_RELATIVE = 0, \
        IDEATEK_UI_POSITION_ABSOLUTE = 1 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_ui_position_t,
    uint8_t, IDEATEK_UI_POSITION_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_ui_position_t,
    uint8_t, IDEATEK_UI_POSITION_VALUES);

    #define IDEATEK_UI_DIRECTION_VALUES { \
        IDEATEK_UI_DIRECTION_ROW = 0, \
        IDEATEK_UI_DIRECTION_COLUMN = 1, \
        IDEATEK_UI_DIRECTION_ROW_REVERSE = 2, \
        IDEATEK_UI_DIRECTION_COLUMN_REVERSE = 3 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_ui_direction_t,
    uint8_t, IDEATEK_UI_DIRECTION_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_ui_direction_t,
    uint8_t, IDEATEK_UI_DIRECTION_VALUES);

    #define IDEATEK_UI_ALIGN_VALUES { \
        IDEATEK_UI_ALIGN_START = 0, \
        IDEATEK_UI_ALIGN_CENTER = 1, \
        IDEATEK_UI_ALIGN_END = 2, \
        IDEATEK_UI_ALIGN_STRETCH = 3, \
        IDEATEK_UI_ALIGN_SPACE_BETWEEN = 4, \
        IDEATEK_UI_ALIGN_SPACE_AROUND = 5, \
        IDEATEK_UI_ALIGN_SPACE_EVENLY = 6 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_ui_align_t, uint8_t, IDEATEK_UI_ALIGN_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_ui_align_t, uint8_t, IDEATEK_UI_ALIGN_VALUES);

    #define IDEATEK_UI_OVERFLOW_VALUES { \
        IDEATEK_UI_OVERFLOW_VISIBLE = 0, \
        IDEATEK_UI_OVERFLOW_CLIP = 1, \
        IDEATEK_UI_OVERFLOW_SCROLL = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_ui_overflow_t,
    uint8_t, IDEATEK_UI_OVERFLOW_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_ui_overflow_t,
    uint8_t, IDEATEK_UI_OVERFLOW_VALUES);

    #define IDEATEK_UI_LAYOUT_FIELDS { \
        ideatek_ui_length_t left; \
        ideatek_ui_length_t right; \
        ideatek_ui_length_t top; \
        ideatek_ui_length_t bottom; \
        ideatek_ui_length_t width; \
        ideatek_ui_length_t height; \
        ideatek_ui_length_t min_width; \
        ideatek_ui_length_t min_height; \
        ideatek_ui_length_t max_width; \
        ideatek_ui_length_t max_height; \
        ideatek_ui_length_t basis; \
        float aspect; \
        float margin_left; \
        float margin_right; \
        float margin_top; \
        float margin_bottom; \
        float padding_left; \
        float padding_right; \
        float padding_top; \
        float padding_bottom; \
        float border_left; \
        float border_right; \
        float border_top; \
        float border_bottom; \
        float grow; \
        float shrink; \
        float gap_x; \
        float gap_y; \
        ideatek_ui_display_t display; \
        ideatek_ui_position_t position; \
        ideatek_ui_direction_t direction; \
        bool wrap; \
        ideatek_ui_align_t align_items; \
        ideatek_ui_align_t align_self; \
        ideatek_ui_align_t justify_content; \
        ideatek_ui_align_t align_content; \
        ideatek_ui_overflow_t overflow_x; \
        ideatek_ui_overflow_t overflow_y; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_layout_t, IDEATEK_UI_LAYOUT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_layout_t, IDEATEK_UI_LAYOUT_FIELDS);

    #define IDEATEK_UI_SCROLL_FIELDS { \
        float x; \
        float y; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_scroll_t, IDEATEK_UI_SCROLL_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_scroll_t, IDEATEK_UI_SCROLL_FIELDS);

    #define IDEATEK_UI_RECT_FIELDS { \
        float x; \
        float y; \
        float width; \
        float height; \
        float content_width; \
        float content_height; \
        float clip_x; \
        float clip_y; \
        float clip_width; \
        float clip_height; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_rect_t, IDEATEK_UI_RECT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_rect_t, IDEATEK_UI_RECT_FIELDS);

    #define IDEATEK_FLIP_VALUES { \
        IDEATEK_FLIP_X = 0x01, \
        IDEATEK_FLIP_Y = 0x02 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_flip_t, uint8_t, IDEATEK_FLIP_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_flip_t, uint8_t, IDEATEK_FLIP_VALUES);

    #define IDEATEK_IMAGE_MODE_VALUES { \
        IDEATEK_IMAGE_MODE_STRETCH = 0, \
        IDEATEK_IMAGE_MODE_SLICED = 1, \
        IDEATEK_IMAGE_MODE_TILED = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_image_mode_t,
    uint8_t, IDEATEK_IMAGE_MODE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_image_mode_t,
    uint8_t, IDEATEK_IMAGE_MODE_VALUES);

    #define IDEATEK_UI_BOX_FIELDS { \
        ideatek_color_t background; \
        ideatek_color_t border; \
        ideatek_color_t shadow; \
        float radius[4]; \
        float shadow_x; \
        float shadow_y; \
        float shadow_blur; \
        float shadow_spread; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_box_t, IDEATEK_UI_BOX_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_box_t, IDEATEK_UI_BOX_FIELDS);

    #define IDEATEK_UI_IMAGE_FIELDS { \
        ideatek_entity_t texture; \
        ideatek_color_t tint; \
        float uv_x; \
        float uv_y; \
        float uv_width; \
        float uv_height; \
        float slice_left; \
        float slice_top; \
        float slice_right; \
        float slice_bottom; \
        ideatek_image_mode_t mode; \
        ideatek_flip_t flip; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_image_t, IDEATEK_UI_IMAGE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_image_t, IDEATEK_UI_IMAGE_FIELDS);

    #define IDEATEK_FONT_FORMAT_VALUES { \
        IDEATEK_FONT_FORMAT_SFNT = 0 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_font_format_t,
    uint8_t, IDEATEK_FONT_FORMAT_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_font_format_t,
    uint8_t, IDEATEK_FONT_FORMAT_VALUES);

    #define IDEATEK_FONT_FIELDS { \
        ideatek_entity_t fallback; \
        bridge_text_t bytes; \
        uint32_t revision; \
        ideatek_font_format_t format; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_font_t, IDEATEK_FONT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_font_t, IDEATEK_FONT_FIELDS);

    #define IDEATEK_UI_TEXT_FIELDS { \
        bridge_text_t text; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_text_t, IDEATEK_UI_TEXT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_text_t, IDEATEK_UI_TEXT_FIELDS);

    #define IDEATEK_TEXT_ALIGN_VALUES { \
        IDEATEK_TEXT_ALIGN_START = 0, \
        IDEATEK_TEXT_ALIGN_CENTER = 1, \
        IDEATEK_TEXT_ALIGN_END = 2, \
        IDEATEK_TEXT_ALIGN_JUSTIFY = 3 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_text_align_t,
    uint8_t, IDEATEK_TEXT_ALIGN_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_text_align_t,
    uint8_t, IDEATEK_TEXT_ALIGN_VALUES);

    #define IDEATEK_TEXT_WRAP_VALUES { \
        IDEATEK_TEXT_WRAP_WORD = 0, \
        IDEATEK_TEXT_WRAP_ANY = 1, \
        IDEATEK_TEXT_WRAP_NONE = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_text_wrap_t,
    uint8_t, IDEATEK_TEXT_WRAP_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_text_wrap_t,
    uint8_t, IDEATEK_TEXT_WRAP_VALUES);

    #define IDEATEK_TEXT_OVERFLOW_VALUES { \
        IDEATEK_TEXT_OVERFLOW_CLIP = 0, \
        IDEATEK_TEXT_OVERFLOW_ELLIPSIS = 1 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_text_overflow_t,
    uint8_t, IDEATEK_TEXT_OVERFLOW_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_text_overflow_t,
    uint8_t, IDEATEK_TEXT_OVERFLOW_VALUES);

    #define IDEATEK_UI_TEXT_LOOK_FIELDS { \
        ideatek_entity_t font; \
        ideatek_color_t color; \
        ideatek_color_t outline_color; \
        ideatek_color_t shadow; \
        float size; \
        float line_height; \
        float outline; \
        float shadow_x; \
        float shadow_y; \
        float letter_spacing; \
        ideatek_text_align_t align; \
        ideatek_text_wrap_t wrap; \
        ideatek_text_overflow_t overflow; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_text_look_t, IDEATEK_UI_TEXT_LOOK_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_text_look_t, IDEATEK_UI_TEXT_LOOK_FIELDS);

    #define IDEATEK_UI_TEXT_SPAN_FIELDS { \
        bridge_text_t text; \
        ideatek_entity_t font; \
        ideatek_entity_t image; \
        ideatek_color_t color; \
        float size; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_text_span_t, IDEATEK_UI_TEXT_SPAN_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_text_span_t, IDEATEK_UI_TEXT_SPAN_FIELDS);

    #define IDEATEK_POINTER_BUTTONS_VALUES { \
        IDEATEK_POINTER_BUTTONS_PRIMARY = 0x01, \
        IDEATEK_POINTER_BUTTONS_SECONDARY = 0x02, \
        IDEATEK_POINTER_BUTTONS_MIDDLE = 0x04 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_pointer_buttons_t,
    uint8_t, IDEATEK_POINTER_BUTTONS_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_pointer_buttons_t,
    uint8_t, IDEATEK_POINTER_BUTTONS_VALUES);

    #define IDEATEK_UI_POINTER_FIELDS { \
        ideatek_entity_t target; \
        float x; \
        float y; \
        float wheel_x; \
        float wheel_y; \
        ideatek_pointer_buttons_t buttons; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_pointer_t, IDEATEK_UI_POINTER_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_pointer_t, IDEATEK_UI_POINTER_FIELDS);

    #define IDEATEK_UI_POINTER_HIT_FIELDS { \
        ideatek_entity_t node; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_pointer_hit_t,
    IDEATEK_UI_POINTER_HIT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_pointer_hit_t,
    IDEATEK_UI_POINTER_HIT_FIELDS);

    #define IDEATEK_PICKABLE_FLAGS_VALUES { \
        IDEATEK_PICKABLE_FLAGS_BLOCK = 0x01, \
        IDEATEK_PICKABLE_FLAGS_HOVER = 0x02 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_pickable_flags_t,
    uint8_t, IDEATEK_PICKABLE_FLAGS_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_pickable_flags_t,
    uint8_t, IDEATEK_PICKABLE_FLAGS_VALUES);

    #define IDEATEK_UI_PICKABLE_FIELDS { \
        ideatek_pickable_flags_t flags; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_pickable_t, IDEATEK_UI_PICKABLE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_pickable_t, IDEATEK_UI_PICKABLE_FIELDS);

    #define IDEATEK_INTERACTION_VALUES { \
        IDEATEK_INTERACTION_NONE = 0, \
        IDEATEK_INTERACTION_HOVERED = 1, \
        IDEATEK_INTERACTION_PRESSED = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_interaction_t,
    uint8_t, IDEATEK_INTERACTION_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_interaction_t,
    uint8_t, IDEATEK_INTERACTION_VALUES);

    #define IDEATEK_UI_INTERACTION_FIELDS { \
        ideatek_entity_t pointer; \
        ideatek_interaction_t state; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_interaction_t,
    IDEATEK_UI_INTERACTION_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_interaction_t,
    IDEATEK_UI_INTERACTION_FIELDS);

    #define IDEATEK_POINTER_EVENT_KIND_VALUES { \
        IDEATEK_POINTER_EVENT_KIND_ENTER = 0, \
        IDEATEK_POINTER_EVENT_KIND_LEAVE = 1, \
        IDEATEK_POINTER_EVENT_KIND_PRESS = 2, \
        IDEATEK_POINTER_EVENT_KIND_RELEASE = 3, \
        IDEATEK_POINTER_EVENT_KIND_CLICK = 4, \
        IDEATEK_POINTER_EVENT_KIND_DRAG_START = 5, \
        IDEATEK_POINTER_EVENT_KIND_DRAG = 6, \
        IDEATEK_POINTER_EVENT_KIND_DRAG_END = 7, \
        IDEATEK_POINTER_EVENT_KIND_DROP = 8, \
        IDEATEK_POINTER_EVENT_KIND_SCROLL = 9 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_pointer_event_kind_t,
    uint8_t, IDEATEK_POINTER_EVENT_KIND_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_pointer_event_kind_t,
    uint8_t, IDEATEK_POINTER_EVENT_KIND_VALUES);

    #define IDEATEK_UI_POINTER_EVENT_FIELDS { \
        ideatek_entity_t node; \
        ideatek_entity_t pointer; \
        ideatek_entity_t other; \
        float x; \
        float y; \
        float dx; \
        float dy; \
        uint32_t order; \
        ideatek_pointer_event_kind_t kind; \
        ideatek_pointer_buttons_t button; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_pointer_event_t,
    IDEATEK_UI_POINTER_EVENT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_pointer_event_t,
    IDEATEK_UI_POINTER_EVENT_FIELDS);

    #define IDEATEK_UI_FOCUS_FIELDS { \
        ideatek_entity_t root; \
        ideatek_entity_t up; \
        ideatek_entity_t down; \
        ideatek_entity_t left; \
        ideatek_entity_t right; \
        ideatek_entity_t next; \
        ideatek_entity_t previous; \
        ideatek_entity_t accept; \
        ideatek_entity_t cancel; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_focus_t, IDEATEK_UI_FOCUS_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_focus_t, IDEATEK_UI_FOCUS_FIELDS);

    #define IDEATEK_UI_FOCUS_STATE_FIELDS { \
        ideatek_entity_t node; \
        bool visible; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_focus_state_t,
    IDEATEK_UI_FOCUS_STATE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_focus_state_t,
    IDEATEK_UI_FOCUS_STATE_FIELDS);

    #define IDEATEK_FOCUSABLE_FLAGS_VALUES { \
        IDEATEK_FOCUSABLE_FLAGS_CLICK = 0x01, \
        IDEATEK_FOCUSABLE_FLAGS_NAVIGATE = 0x02, \
        IDEATEK_FOCUSABLE_FLAGS_AUTO = 0x04 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_focusable_flags_t,
    uint8_t, IDEATEK_FOCUSABLE_FLAGS_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_focusable_flags_t,
    uint8_t, IDEATEK_FOCUSABLE_FLAGS_VALUES);

    #define IDEATEK_UI_FOCUSABLE_FIELDS { \
        int32_t tab; \
        ideatek_focusable_flags_t flags; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_focusable_t, IDEATEK_UI_FOCUSABLE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_focusable_t, IDEATEK_UI_FOCUSABLE_FIELDS);

    #define IDEATEK_UI_NEIGHBORS_FIELDS { \
        ideatek_entity_t up; \
        ideatek_entity_t down; \
        ideatek_entity_t left; \
        ideatek_entity_t right; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_neighbors_t, IDEATEK_UI_NEIGHBORS_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_neighbors_t, IDEATEK_UI_NEIGHBORS_FIELDS);

    #define IDEATEK_UI_FOCUS_MOVE_FIELDS { \
        ideatek_entity_t focus; \
        ideatek_entity_t node; \
        uint32_t order; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_focus_move_t,
    IDEATEK_UI_FOCUS_MOVE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_focus_move_t,
    IDEATEK_UI_FOCUS_MOVE_FIELDS);

    #define IDEATEK_FOCUS_EVENT_KIND_VALUES { \
        IDEATEK_FOCUS_EVENT_KIND_GAINED = 0, \
        IDEATEK_FOCUS_EVENT_KIND_LOST = 1, \
        IDEATEK_FOCUS_EVENT_KIND_ACTIVATE = 2, \
        IDEATEK_FOCUS_EVENT_KIND_CANCEL = 3 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_focus_event_kind_t,
    uint8_t, IDEATEK_FOCUS_EVENT_KIND_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_focus_event_kind_t,
    uint8_t, IDEATEK_FOCUS_EVENT_KIND_VALUES);

    #define IDEATEK_FOCUS_CAUSE_VALUES { \
        IDEATEK_FOCUS_CAUSE_NAVIGATE = 0, \
        IDEATEK_FOCUS_CAUSE_POINTER = 1, \
        IDEATEK_FOCUS_CAUSE_AUTO = 2, \
        IDEATEK_FOCUS_CAUSE_GAME = 3 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_focus_cause_t,
    uint8_t, IDEATEK_FOCUS_CAUSE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_focus_cause_t,
    uint8_t, IDEATEK_FOCUS_CAUSE_VALUES);

    #define IDEATEK_UI_FOCUS_EVENT_FIELDS { \
        ideatek_entity_t focus; \
        ideatek_entity_t node; \
        uint32_t order; \
        ideatek_focus_event_kind_t kind; \
        ideatek_focus_cause_t cause; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_focus_event_t,
    IDEATEK_UI_FOCUS_EVENT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_focus_event_t,
    IDEATEK_UI_FOCUS_EVENT_FIELDS);

    #define IDEATEK_UI_VALUE_FIELDS { \
        float value; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_value_t, IDEATEK_UI_VALUE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_value_t, IDEATEK_UI_VALUE_FIELDS);

    #define IDEATEK_UI_VALUE_EVENT_FIELDS { \
        ideatek_entity_t node; \
        float value; \
        uint32_t order; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_value_event_t,
    IDEATEK_UI_VALUE_EVENT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_value_event_t,
    IDEATEK_UI_VALUE_EVENT_FIELDS);

    #define IDEATEK_BUTTON_MODE_VALUES { \
        IDEATEK_BUTTON_MODE_RELEASE = 0, \
        IDEATEK_BUTTON_MODE_PRESS = 1 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_button_mode_t,
    uint8_t, IDEATEK_BUTTON_MODE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_button_mode_t,
    uint8_t, IDEATEK_BUTTON_MODE_VALUES);

    #define IDEATEK_UI_BUTTON_FIELDS { \
        ideatek_button_mode_t mode; \
        bool disabled; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_button_t, IDEATEK_UI_BUTTON_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_button_t, IDEATEK_UI_BUTTON_FIELDS);

    #define IDEATEK_UI_ACTIVATED_FIELDS { \
        ideatek_entity_t node; \
        ideatek_entity_t source; \
        uint32_t order; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_activated_t, IDEATEK_UI_ACTIVATED_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_activated_t, IDEATEK_UI_ACTIVATED_FIELDS);

    #define IDEATEK_UI_TOGGLE_FIELDS { \
        ideatek_entity_t group; \
        bool disabled; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_toggle_t, IDEATEK_UI_TOGGLE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_toggle_t, IDEATEK_UI_TOGGLE_FIELDS);

    #define IDEATEK_UI_TOGGLE_GROUP_FIELDS { \
        bool allow_none; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_toggle_group_t,
    IDEATEK_UI_TOGGLE_GROUP_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_toggle_group_t,
    IDEATEK_UI_TOGGLE_GROUP_FIELDS);

    #define IDEATEK_SLIDER_AXIS_VALUES { \
        IDEATEK_SLIDER_AXIS_HORIZONTAL = 0, \
        IDEATEK_SLIDER_AXIS_VERTICAL = 1 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_slider_axis_t,
    uint8_t, IDEATEK_SLIDER_AXIS_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_slider_axis_t,
    uint8_t, IDEATEK_SLIDER_AXIS_VALUES);

    #define IDEATEK_UI_SLIDER_FIELDS { \
        float min; \
        float max; \
        float step; \
        ideatek_slider_axis_t axis; \
        bool disabled; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_slider_t, IDEATEK_UI_SLIDER_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_slider_t, IDEATEK_UI_SLIDER_FIELDS);

    #define IDEATEK_SCROLL_FLAGS_VALUES { \
        IDEATEK_SCROLL_FLAGS_WHEEL = 0x01, \
        IDEATEK_SCROLL_FLAGS_DRAG = 0x02 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_scroll_flags_t,
    uint8_t, IDEATEK_SCROLL_FLAGS_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_scroll_flags_t,
    uint8_t, IDEATEK_SCROLL_FLAGS_VALUES);

    #define IDEATEK_UI_SCROLL_AREA_FIELDS { \
        ideatek_scroll_flags_t flags; \
        bool disabled; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_scroll_area_t,
    IDEATEK_UI_SCROLL_AREA_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_scroll_area_t,
    IDEATEK_UI_SCROLL_AREA_FIELDS);

    #define IDEATEK_UI_SCROLL_TO_FIELDS { \
        ideatek_entity_t node; \
        float x; \
        float y; \
        uint32_t order; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_scroll_to_t, IDEATEK_UI_SCROLL_TO_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_scroll_to_t, IDEATEK_UI_SCROLL_TO_FIELDS);

    #define IDEATEK_TEXT_FIELD_FLAGS_VALUES { \
        IDEATEK_TEXT_FIELD_FLAGS_SECRET = 0x01, \
        IDEATEK_TEXT_FIELD_FLAGS_MULTILINE = 0x02 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_text_field_flags_t,
    uint8_t, IDEATEK_TEXT_FIELD_FLAGS_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_text_field_flags_t,
    uint8_t, IDEATEK_TEXT_FIELD_FLAGS_VALUES);

    #define IDEATEK_UI_TEXT_FIELD_FIELDS { \
        uint32_t max_length; \
        ideatek_text_field_flags_t flags; \
        bool disabled; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_text_field_t,
    IDEATEK_UI_TEXT_FIELD_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_text_field_t,
    IDEATEK_UI_TEXT_FIELD_FIELDS);

    #define IDEATEK_UI_TEXT_CHANGE_FIELDS { \
        ideatek_entity_t node; \
        bridge_text_t text; \
        uint32_t order; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_text_change_t,
    IDEATEK_UI_TEXT_CHANGE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_text_change_t,
    IDEATEK_UI_TEXT_CHANGE_FIELDS);

    #define IDEATEK_UI_CARET_FIELDS { \
        uint32_t caret; \
        uint32_t anchor; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_caret_t, IDEATEK_UI_CARET_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_caret_t, IDEATEK_UI_CARET_FIELDS);

    #define IDEATEK_SCREEN_FLAGS_VALUES { \
        IDEATEK_SCREEN_FLAGS_MODAL = 0x01, \
        IDEATEK_SCREEN_FLAGS_RESTORE_FOCUS = 0x02 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_screen_flags_t,
    uint8_t, IDEATEK_SCREEN_FLAGS_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_screen_flags_t,
    uint8_t, IDEATEK_SCREEN_FLAGS_VALUES);

    #define IDEATEK_UI_SCREEN_FIELDS { \
        ideatek_entity_t focus; \
        ideatek_screen_flags_t flags; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_screen_t, IDEATEK_UI_SCREEN_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_screen_t, IDEATEK_UI_SCREEN_FIELDS);

    #define IDEATEK_UI_SCREEN_STATE_FIELDS { \
        ideatek_entity_t last; \
        bool top; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_screen_state_t,
    IDEATEK_UI_SCREEN_STATE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_screen_state_t,
    IDEATEK_UI_SCREEN_STATE_FIELDS);

    #define IDEATEK_SCREEN_EVENT_KIND_VALUES { \
        IDEATEK_SCREEN_EVENT_KIND_TOP = 0, \
        IDEATEK_SCREEN_EVENT_KIND_COVERED = 1 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_screen_event_kind_t,
    uint8_t, IDEATEK_SCREEN_EVENT_KIND_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_screen_event_kind_t,
    uint8_t, IDEATEK_SCREEN_EVENT_KIND_VALUES);

    #define IDEATEK_UI_SCREEN_EVENT_FIELDS { \
        ideatek_entity_t screen; \
        uint32_t order; \
        ideatek_screen_event_kind_t kind; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_screen_event_t,
    IDEATEK_UI_SCREEN_EVENT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_screen_event_t,
    IDEATEK_UI_SCREEN_EVENT_FIELDS);

    #define IDEATEK_GESTURE_KIND_VALUES { \
        IDEATEK_GESTURE_KIND_TAP = 0, \
        IDEATEK_GESTURE_KIND_LONG_PRESS = 1, \
        IDEATEK_GESTURE_KIND_PAN = 2, \
        IDEATEK_GESTURE_KIND_PINCH = 3 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_gesture_kind_t,
    uint8_t, IDEATEK_GESTURE_KIND_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_gesture_kind_t,
    uint8_t, IDEATEK_GESTURE_KIND_VALUES);

    #define IDEATEK_GESTURE_PHASE_VALUES { \
        IDEATEK_GESTURE_PHASE_BEGIN = 0, \
        IDEATEK_GESTURE_PHASE_CHANGE = 1, \
        IDEATEK_GESTURE_PHASE_END = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_gesture_phase_t,
    uint8_t, IDEATEK_GESTURE_PHASE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_gesture_phase_t,
    uint8_t, IDEATEK_GESTURE_PHASE_VALUES);

    #define IDEATEK_UI_GESTURE_FIELDS { \
        ideatek_entity_t node; \
        ideatek_entity_t target; \
        float x; \
        float y; \
        float dx; \
        float dy; \
        float scale; \
        float angle; \
        float vx; \
        float vy; \
        uint32_t order; \
        ideatek_gesture_kind_t kind; \
        ideatek_gesture_phase_t phase; \
        uint8_t fingers; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_gesture_t, IDEATEK_UI_GESTURE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_gesture_t, IDEATEK_UI_GESTURE_FIELDS);

    #define IDEATEK_ANCHOR_FLAGS_VALUES { \
        IDEATEK_ANCHOR_FLAGS_HIDE_BEHIND = 0x01 \
    }

BRIDGE_SCHEMA_BITMASK_TYPE(ideatek_anchor_flags_t,
    uint8_t, IDEATEK_ANCHOR_FLAGS_VALUES);
BRIDGE_SCHEMA_BITMASK_DESC(ideatek_anchor_flags_t,
    uint8_t, IDEATEK_ANCHOR_FLAGS_VALUES);

    #define IDEATEK_UI_ANCHOR_FIELDS { \
        ideatek_entity_t entity; \
        ideatek_entity_t camera; \
        ideatek_vec3_t offset; \
        float pivot_x; \
        float pivot_y; \
        ideatek_anchor_flags_t flags; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_anchor_t, IDEATEK_UI_ANCHOR_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_anchor_t, IDEATEK_UI_ANCHOR_FIELDS);

    #define IDEATEK_UI_ANCHOR_STATE_FIELDS { \
        float distance; \
        bool visible; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_anchor_state_t,
    IDEATEK_UI_ANCHOR_STATE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_anchor_state_t,
    IDEATEK_UI_ANCHOR_STATE_FIELDS);

    #define IDEATEK_UI_GRID_FIELDS { \
        uint32_t columns; \
        uint32_t rows; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_grid_t, IDEATEK_UI_GRID_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_grid_t, IDEATEK_UI_GRID_FIELDS);

    #define IDEATEK_UI_GRID_ITEM_FIELDS { \
        uint32_t column; \
        uint32_t row; \
        uint32_t column_span; \
        uint32_t row_span; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_grid_item_t, IDEATEK_UI_GRID_ITEM_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_grid_item_t, IDEATEK_UI_GRID_ITEM_FIELDS);

    #define IDEATEK_UI_TRANSFORM_FIELDS { \
        float x; \
        float y; \
        float scale_x; \
        float scale_y; \
        float rotation; \
        float rotation_x; \
        float rotation_y; \
        float perspective; \
        float pivot_x; \
        float pivot_y; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_transform_t, IDEATEK_UI_TRANSFORM_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_transform_t, IDEATEK_UI_TRANSFORM_FIELDS);

    #define IDEATEK_UI_OPACITY_FIELDS { \
        float value; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_opacity_t, IDEATEK_UI_OPACITY_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_opacity_t, IDEATEK_UI_OPACITY_FIELDS);

    #define IDEATEK_GRADIENT_KIND_VALUES { \
        IDEATEK_GRADIENT_KIND_LINEAR = 0, \
        IDEATEK_GRADIENT_KIND_RADIAL = 1, \
        IDEATEK_GRADIENT_KIND_CONIC = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_gradient_kind_t,
    uint8_t, IDEATEK_GRADIENT_KIND_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_gradient_kind_t,
    uint8_t, IDEATEK_GRADIENT_KIND_VALUES);

    #define IDEATEK_UI_GRADIENT_FIELDS { \
        bridge_text_t stops; \
        uint32_t count; \
        float angle; \
        ideatek_gradient_kind_t kind; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_gradient_t, IDEATEK_UI_GRADIENT_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_gradient_t, IDEATEK_UI_GRADIENT_FIELDS);

    #define IDEATEK_UI_ACCESSIBLE_FIELDS { \
        bridge_text_t label; \
        bridge_text_t description; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_accessible_t,
    IDEATEK_UI_ACCESSIBLE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_accessible_t,
    IDEATEK_UI_ACCESSIBLE_FIELDS);

    #define IDEATEK_UI_SHADER_FIELDS { \
        bridge_text_t name; \
        bridge_text_t params; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_shader_t, IDEATEK_UI_SHADER_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_shader_t, IDEATEK_UI_SHADER_FIELDS);

    #define IDEATEK_UI_BLUR_FIELDS { \
        float radius; \
        bool backdrop; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_blur_t, IDEATEK_UI_BLUR_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_blur_t, IDEATEK_UI_BLUR_FIELDS);

    #define IDEATEK_UI_MASK_FIELDS { \
        ideatek_entity_t texture; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_mask_t, IDEATEK_UI_MASK_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_mask_t, IDEATEK_UI_MASK_FIELDS);

    #define IDEATEK_EASE_VALUES { \
        IDEATEK_EASE_LINEAR = 0, \
        IDEATEK_EASE_QUADRATIC = 1, \
        IDEATEK_EASE_CUBIC = 2, \
        IDEATEK_EASE_QUARTIC = 3, \
        IDEATEK_EASE_QUINTIC = 4, \
        IDEATEK_EASE_SINE = 5, \
        IDEATEK_EASE_EXPONENTIAL = 6, \
        IDEATEK_EASE_CIRCULAR = 7, \
        IDEATEK_EASE_BACK = 8, \
        IDEATEK_EASE_ELASTIC = 9, \
        IDEATEK_EASE_BOUNCE = 10 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_ease_t, uint8_t, IDEATEK_EASE_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_ease_t, uint8_t, IDEATEK_EASE_VALUES);

    #define IDEATEK_EASE_DIRECTION_VALUES { \
        IDEATEK_EASE_DIRECTION_IN = 0, \
        IDEATEK_EASE_DIRECTION_OUT = 1, \
        IDEATEK_EASE_DIRECTION_IN_OUT = 2 \
    }

BRIDGE_SCHEMA_ENUM_TYPE(ideatek_ease_direction_t,
    uint8_t, IDEATEK_EASE_DIRECTION_VALUES);
BRIDGE_SCHEMA_ENUM_DESC(ideatek_ease_direction_t,
    uint8_t, IDEATEK_EASE_DIRECTION_VALUES);

    #define IDEATEK_UI_STYLE_FIELDS { \
        ideatek_entity_t normal; \
        ideatek_entity_t hovered; \
        ideatek_entity_t pressed; \
        ideatek_entity_t disabled; \
        ideatek_entity_t focus; \
        float transition; \
        ideatek_ease_t ease; \
        ideatek_ease_direction_t ease_direction; \
    }

BRIDGE_SCHEMA_STRUCT_TYPE(ideatek_ui_style_t, IDEATEK_UI_STYLE_FIELDS);
BRIDGE_SCHEMA_STRUCT_DESC(ideatek_ui_style_t, IDEATEK_UI_STYLE_FIELDS);

#endif
