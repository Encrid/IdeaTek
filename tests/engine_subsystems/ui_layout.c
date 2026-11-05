/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <ideatek_ui.h>

#define SIZE(type) printf("size %zu\nalignment %zu\n", sizeof(type), \
    alignof(type))
#define AT(type, field) printf("%s %zu\n", #field, offsetof(type, field))
#define TEXT_OF(...) BRIDGE_SCHEMA_TEXT(__VA_ARGS__)

typedef struct {
    const char *name;
    void (*show)(void);
} show_mode_t;

static void show_ui_root(void)
{
    SIZE(ideatek_ui_root_t);
    AT(ideatek_ui_root_t, target);
    AT(ideatek_ui_root_t, order);
    AT(ideatek_ui_root_t, scale);
    AT(ideatek_ui_root_t, right_to_left);
}

static void show_ui_node(void)
{
    SIZE(ideatek_ui_node_t);
    AT(ideatek_ui_node_t, order);
}

static void show_ui_unit(void)
{
    SIZE(ideatek_ui_unit_t);
}

static void show_ui_length(void)
{
    SIZE(ideatek_ui_length_t);
    AT(ideatek_ui_length_t, value);
    AT(ideatek_ui_length_t, unit);
}

static void show_ui_display(void)
{
    SIZE(ideatek_ui_display_t);
}

static void show_ui_position(void)
{
    SIZE(ideatek_ui_position_t);
}

static void show_ui_direction(void)
{
    SIZE(ideatek_ui_direction_t);
}

static void show_ui_align(void)
{
    SIZE(ideatek_ui_align_t);
}

static void show_ui_overflow(void)
{
    SIZE(ideatek_ui_overflow_t);
}

static void show_ui_layout(void)
{
    SIZE(ideatek_ui_layout_t);
    AT(ideatek_ui_layout_t, left);
    AT(ideatek_ui_layout_t, right);
    AT(ideatek_ui_layout_t, top);
    AT(ideatek_ui_layout_t, bottom);
    AT(ideatek_ui_layout_t, width);
    AT(ideatek_ui_layout_t, height);
    AT(ideatek_ui_layout_t, min_width);
    AT(ideatek_ui_layout_t, min_height);
    AT(ideatek_ui_layout_t, max_width);
    AT(ideatek_ui_layout_t, max_height);
    AT(ideatek_ui_layout_t, basis);
    AT(ideatek_ui_layout_t, aspect);
    AT(ideatek_ui_layout_t, margin_left);
    AT(ideatek_ui_layout_t, margin_right);
    AT(ideatek_ui_layout_t, margin_top);
    AT(ideatek_ui_layout_t, margin_bottom);
    AT(ideatek_ui_layout_t, padding_left);
    AT(ideatek_ui_layout_t, padding_right);
    AT(ideatek_ui_layout_t, padding_top);
    AT(ideatek_ui_layout_t, padding_bottom);
    AT(ideatek_ui_layout_t, border_left);
    AT(ideatek_ui_layout_t, border_right);
    AT(ideatek_ui_layout_t, border_top);
    AT(ideatek_ui_layout_t, border_bottom);
    AT(ideatek_ui_layout_t, grow);
    AT(ideatek_ui_layout_t, shrink);
    AT(ideatek_ui_layout_t, gap_x);
    AT(ideatek_ui_layout_t, gap_y);
    AT(ideatek_ui_layout_t, display);
    AT(ideatek_ui_layout_t, position);
    AT(ideatek_ui_layout_t, direction);
    AT(ideatek_ui_layout_t, wrap);
    AT(ideatek_ui_layout_t, align_items);
    AT(ideatek_ui_layout_t, align_self);
    AT(ideatek_ui_layout_t, justify_content);
    AT(ideatek_ui_layout_t, align_content);
    AT(ideatek_ui_layout_t, overflow_x);
    AT(ideatek_ui_layout_t, overflow_y);
}

static void show_ui_scroll(void)
{
    SIZE(ideatek_ui_scroll_t);
    AT(ideatek_ui_scroll_t, x);
    AT(ideatek_ui_scroll_t, y);
}

static void show_ui_rect(void)
{
    SIZE(ideatek_ui_rect_t);
    AT(ideatek_ui_rect_t, x);
    AT(ideatek_ui_rect_t, y);
    AT(ideatek_ui_rect_t, width);
    AT(ideatek_ui_rect_t, height);
    AT(ideatek_ui_rect_t, content_width);
    AT(ideatek_ui_rect_t, content_height);
    AT(ideatek_ui_rect_t, clip_x);
    AT(ideatek_ui_rect_t, clip_y);
    AT(ideatek_ui_rect_t, clip_width);
    AT(ideatek_ui_rect_t, clip_height);
}

static void show_flip(void)
{
    SIZE(ideatek_flip_t);
}

static void show_image_mode(void)
{
    SIZE(ideatek_image_mode_t);
}

static void show_ui_box(void)
{
    SIZE(ideatek_ui_box_t);
    AT(ideatek_ui_box_t, background);
    AT(ideatek_ui_box_t, border);
    AT(ideatek_ui_box_t, shadow);
    AT(ideatek_ui_box_t, radius);
    AT(ideatek_ui_box_t, shadow_x);
    AT(ideatek_ui_box_t, shadow_y);
    AT(ideatek_ui_box_t, shadow_blur);
    AT(ideatek_ui_box_t, shadow_spread);
}

static void show_ui_image(void)
{
    SIZE(ideatek_ui_image_t);
    AT(ideatek_ui_image_t, texture);
    AT(ideatek_ui_image_t, tint);
    AT(ideatek_ui_image_t, uv_x);
    AT(ideatek_ui_image_t, uv_y);
    AT(ideatek_ui_image_t, uv_width);
    AT(ideatek_ui_image_t, uv_height);
    AT(ideatek_ui_image_t, slice_left);
    AT(ideatek_ui_image_t, slice_top);
    AT(ideatek_ui_image_t, slice_right);
    AT(ideatek_ui_image_t, slice_bottom);
    AT(ideatek_ui_image_t, mode);
    AT(ideatek_ui_image_t, flip);
}

static void show_font_format(void)
{
    SIZE(ideatek_font_format_t);
}

static void show_font(void)
{
    SIZE(ideatek_font_t);
    AT(ideatek_font_t, fallback);
    AT(ideatek_font_t, bytes);
    AT(ideatek_font_t, revision);
    AT(ideatek_font_t, format);
}

static void show_ui_text(void)
{
    SIZE(ideatek_ui_text_t);
    AT(ideatek_ui_text_t, text);
}

static void show_text_align(void)
{
    SIZE(ideatek_text_align_t);
}

static void show_text_wrap(void)
{
    SIZE(ideatek_text_wrap_t);
}

static void show_text_overflow(void)
{
    SIZE(ideatek_text_overflow_t);
}

static void show_ui_text_look(void)
{
    SIZE(ideatek_ui_text_look_t);
    AT(ideatek_ui_text_look_t, font);
    AT(ideatek_ui_text_look_t, color);
    AT(ideatek_ui_text_look_t, outline_color);
    AT(ideatek_ui_text_look_t, shadow);
    AT(ideatek_ui_text_look_t, size);
    AT(ideatek_ui_text_look_t, line_height);
    AT(ideatek_ui_text_look_t, outline);
    AT(ideatek_ui_text_look_t, shadow_x);
    AT(ideatek_ui_text_look_t, shadow_y);
    AT(ideatek_ui_text_look_t, letter_spacing);
    AT(ideatek_ui_text_look_t, align);
    AT(ideatek_ui_text_look_t, wrap);
    AT(ideatek_ui_text_look_t, overflow);
}

static void show_ui_text_span(void)
{
    SIZE(ideatek_ui_text_span_t);
    AT(ideatek_ui_text_span_t, text);
    AT(ideatek_ui_text_span_t, font);
    AT(ideatek_ui_text_span_t, image);
    AT(ideatek_ui_text_span_t, color);
    AT(ideatek_ui_text_span_t, size);
}

static void show_pointer_buttons(void)
{
    SIZE(ideatek_pointer_buttons_t);
}

static void show_ui_pointer(void)
{
    SIZE(ideatek_ui_pointer_t);
    AT(ideatek_ui_pointer_t, target);
    AT(ideatek_ui_pointer_t, x);
    AT(ideatek_ui_pointer_t, y);
    AT(ideatek_ui_pointer_t, wheel_x);
    AT(ideatek_ui_pointer_t, wheel_y);
    AT(ideatek_ui_pointer_t, buttons);
}

static void show_ui_pointer_hit(void)
{
    SIZE(ideatek_ui_pointer_hit_t);
    AT(ideatek_ui_pointer_hit_t, node);
}

static void show_pickable_flags(void)
{
    SIZE(ideatek_pickable_flags_t);
}

static void show_ui_pickable(void)
{
    SIZE(ideatek_ui_pickable_t);
    AT(ideatek_ui_pickable_t, flags);
}

static void show_interaction(void)
{
    SIZE(ideatek_interaction_t);
}

static void show_ui_interaction(void)
{
    SIZE(ideatek_ui_interaction_t);
    AT(ideatek_ui_interaction_t, pointer);
    AT(ideatek_ui_interaction_t, state);
}

static void show_pointer_event_kind(void)
{
    SIZE(ideatek_pointer_event_kind_t);
}

static void show_ui_pointer_event(void)
{
    SIZE(ideatek_ui_pointer_event_t);
    AT(ideatek_ui_pointer_event_t, node);
    AT(ideatek_ui_pointer_event_t, pointer);
    AT(ideatek_ui_pointer_event_t, other);
    AT(ideatek_ui_pointer_event_t, x);
    AT(ideatek_ui_pointer_event_t, y);
    AT(ideatek_ui_pointer_event_t, dx);
    AT(ideatek_ui_pointer_event_t, dy);
    AT(ideatek_ui_pointer_event_t, order);
    AT(ideatek_ui_pointer_event_t, kind);
    AT(ideatek_ui_pointer_event_t, button);
}

static void show_ui_focus(void)
{
    SIZE(ideatek_ui_focus_t);
    AT(ideatek_ui_focus_t, root);
    AT(ideatek_ui_focus_t, up);
    AT(ideatek_ui_focus_t, down);
    AT(ideatek_ui_focus_t, left);
    AT(ideatek_ui_focus_t, right);
    AT(ideatek_ui_focus_t, next);
    AT(ideatek_ui_focus_t, previous);
    AT(ideatek_ui_focus_t, accept);
    AT(ideatek_ui_focus_t, cancel);
}

static void show_ui_focus_state(void)
{
    SIZE(ideatek_ui_focus_state_t);
    AT(ideatek_ui_focus_state_t, node);
    AT(ideatek_ui_focus_state_t, visible);
}

static void show_focusable_flags(void)
{
    SIZE(ideatek_focusable_flags_t);
}

static void show_ui_focusable(void)
{
    SIZE(ideatek_ui_focusable_t);
    AT(ideatek_ui_focusable_t, tab);
    AT(ideatek_ui_focusable_t, flags);
}

static void show_ui_neighbors(void)
{
    SIZE(ideatek_ui_neighbors_t);
    AT(ideatek_ui_neighbors_t, up);
    AT(ideatek_ui_neighbors_t, down);
    AT(ideatek_ui_neighbors_t, left);
    AT(ideatek_ui_neighbors_t, right);
}

static void show_ui_focus_move(void)
{
    SIZE(ideatek_ui_focus_move_t);
    AT(ideatek_ui_focus_move_t, focus);
    AT(ideatek_ui_focus_move_t, node);
    AT(ideatek_ui_focus_move_t, order);
}

static void show_focus_event_kind(void)
{
    SIZE(ideatek_focus_event_kind_t);
}

static void show_focus_cause(void)
{
    SIZE(ideatek_focus_cause_t);
}

static void show_ui_focus_event(void)
{
    SIZE(ideatek_ui_focus_event_t);
    AT(ideatek_ui_focus_event_t, focus);
    AT(ideatek_ui_focus_event_t, node);
    AT(ideatek_ui_focus_event_t, order);
    AT(ideatek_ui_focus_event_t, kind);
    AT(ideatek_ui_focus_event_t, cause);
}

static void show_ui_value(void)
{
    SIZE(ideatek_ui_value_t);
    AT(ideatek_ui_value_t, value);
}

static void show_ui_value_event(void)
{
    SIZE(ideatek_ui_value_event_t);
    AT(ideatek_ui_value_event_t, node);
    AT(ideatek_ui_value_event_t, value);
    AT(ideatek_ui_value_event_t, order);
}

static void show_button_mode(void)
{
    SIZE(ideatek_button_mode_t);
}

static void show_ui_button(void)
{
    SIZE(ideatek_ui_button_t);
    AT(ideatek_ui_button_t, mode);
    AT(ideatek_ui_button_t, disabled);
}

static void show_ui_activated(void)
{
    SIZE(ideatek_ui_activated_t);
    AT(ideatek_ui_activated_t, node);
    AT(ideatek_ui_activated_t, source);
    AT(ideatek_ui_activated_t, order);
}

static void show_ui_toggle(void)
{
    SIZE(ideatek_ui_toggle_t);
    AT(ideatek_ui_toggle_t, group);
    AT(ideatek_ui_toggle_t, disabled);
}

static void show_ui_toggle_group(void)
{
    SIZE(ideatek_ui_toggle_group_t);
    AT(ideatek_ui_toggle_group_t, allow_none);
}

static void show_slider_axis(void)
{
    SIZE(ideatek_slider_axis_t);
}

static void show_ui_slider(void)
{
    SIZE(ideatek_ui_slider_t);
    AT(ideatek_ui_slider_t, min);
    AT(ideatek_ui_slider_t, max);
    AT(ideatek_ui_slider_t, step);
    AT(ideatek_ui_slider_t, axis);
    AT(ideatek_ui_slider_t, disabled);
}

static void show_scroll_flags(void)
{
    SIZE(ideatek_scroll_flags_t);
}

static void show_ui_scroll_area(void)
{
    SIZE(ideatek_ui_scroll_area_t);
    AT(ideatek_ui_scroll_area_t, flags);
    AT(ideatek_ui_scroll_area_t, disabled);
}

static void show_ui_scroll_to(void)
{
    SIZE(ideatek_ui_scroll_to_t);
    AT(ideatek_ui_scroll_to_t, node);
    AT(ideatek_ui_scroll_to_t, x);
    AT(ideatek_ui_scroll_to_t, y);
    AT(ideatek_ui_scroll_to_t, order);
}

static void show_text_field_flags(void)
{
    SIZE(ideatek_text_field_flags_t);
}

static void show_ui_text_field(void)
{
    SIZE(ideatek_ui_text_field_t);
    AT(ideatek_ui_text_field_t, max_length);
    AT(ideatek_ui_text_field_t, flags);
    AT(ideatek_ui_text_field_t, disabled);
}

static void show_ui_text_change(void)
{
    SIZE(ideatek_ui_text_change_t);
    AT(ideatek_ui_text_change_t, node);
    AT(ideatek_ui_text_change_t, text);
    AT(ideatek_ui_text_change_t, order);
}

static void show_ui_caret(void)
{
    SIZE(ideatek_ui_caret_t);
    AT(ideatek_ui_caret_t, caret);
    AT(ideatek_ui_caret_t, anchor);
}

static void show_screen_flags(void)
{
    SIZE(ideatek_screen_flags_t);
}

static void show_ui_screen(void)
{
    SIZE(ideatek_ui_screen_t);
    AT(ideatek_ui_screen_t, focus);
    AT(ideatek_ui_screen_t, flags);
}

static void show_ui_screen_state(void)
{
    SIZE(ideatek_ui_screen_state_t);
    AT(ideatek_ui_screen_state_t, last);
    AT(ideatek_ui_screen_state_t, top);
}

static void show_screen_event_kind(void)
{
    SIZE(ideatek_screen_event_kind_t);
}

static void show_ui_screen_event(void)
{
    SIZE(ideatek_ui_screen_event_t);
    AT(ideatek_ui_screen_event_t, screen);
    AT(ideatek_ui_screen_event_t, order);
    AT(ideatek_ui_screen_event_t, kind);
}

static void show_gesture_kind(void)
{
    SIZE(ideatek_gesture_kind_t);
}

static void show_gesture_phase(void)
{
    SIZE(ideatek_gesture_phase_t);
}

static void show_ui_gesture(void)
{
    SIZE(ideatek_ui_gesture_t);
    AT(ideatek_ui_gesture_t, node);
    AT(ideatek_ui_gesture_t, target);
    AT(ideatek_ui_gesture_t, x);
    AT(ideatek_ui_gesture_t, y);
    AT(ideatek_ui_gesture_t, dx);
    AT(ideatek_ui_gesture_t, dy);
    AT(ideatek_ui_gesture_t, scale);
    AT(ideatek_ui_gesture_t, angle);
    AT(ideatek_ui_gesture_t, vx);
    AT(ideatek_ui_gesture_t, vy);
    AT(ideatek_ui_gesture_t, order);
    AT(ideatek_ui_gesture_t, kind);
    AT(ideatek_ui_gesture_t, phase);
    AT(ideatek_ui_gesture_t, fingers);
}

static void show_anchor_flags(void)
{
    SIZE(ideatek_anchor_flags_t);
}

static void show_ui_anchor(void)
{
    SIZE(ideatek_ui_anchor_t);
    AT(ideatek_ui_anchor_t, entity);
    AT(ideatek_ui_anchor_t, camera);
    AT(ideatek_ui_anchor_t, offset);
    AT(ideatek_ui_anchor_t, pivot_x);
    AT(ideatek_ui_anchor_t, pivot_y);
    AT(ideatek_ui_anchor_t, flags);
}

static void show_ui_anchor_state(void)
{
    SIZE(ideatek_ui_anchor_state_t);
    AT(ideatek_ui_anchor_state_t, distance);
    AT(ideatek_ui_anchor_state_t, visible);
}

static void show_ui_grid(void)
{
    SIZE(ideatek_ui_grid_t);
    AT(ideatek_ui_grid_t, columns);
    AT(ideatek_ui_grid_t, rows);
}

static void show_ui_grid_item(void)
{
    SIZE(ideatek_ui_grid_item_t);
    AT(ideatek_ui_grid_item_t, column);
    AT(ideatek_ui_grid_item_t, row);
    AT(ideatek_ui_grid_item_t, column_span);
    AT(ideatek_ui_grid_item_t, row_span);
}

static void show_ui_transform(void)
{
    SIZE(ideatek_ui_transform_t);
    AT(ideatek_ui_transform_t, x);
    AT(ideatek_ui_transform_t, y);
    AT(ideatek_ui_transform_t, scale_x);
    AT(ideatek_ui_transform_t, scale_y);
    AT(ideatek_ui_transform_t, rotation);
    AT(ideatek_ui_transform_t, rotation_x);
    AT(ideatek_ui_transform_t, rotation_y);
    AT(ideatek_ui_transform_t, perspective);
    AT(ideatek_ui_transform_t, pivot_x);
    AT(ideatek_ui_transform_t, pivot_y);
}

static void show_ui_opacity(void)
{
    SIZE(ideatek_ui_opacity_t);
    AT(ideatek_ui_opacity_t, value);
}

static void show_gradient_kind(void)
{
    SIZE(ideatek_gradient_kind_t);
}

static void show_ui_gradient(void)
{
    SIZE(ideatek_ui_gradient_t);
    AT(ideatek_ui_gradient_t, stops);
    AT(ideatek_ui_gradient_t, count);
    AT(ideatek_ui_gradient_t, angle);
    AT(ideatek_ui_gradient_t, kind);
}

static void show_ui_accessible(void)
{
    SIZE(ideatek_ui_accessible_t);
    AT(ideatek_ui_accessible_t, label);
    AT(ideatek_ui_accessible_t, description);
}

static void show_ui_shader(void)
{
    SIZE(ideatek_ui_shader_t);
    AT(ideatek_ui_shader_t, name);
    AT(ideatek_ui_shader_t, params);
}

static void show_ui_blur(void)
{
    SIZE(ideatek_ui_blur_t);
    AT(ideatek_ui_blur_t, radius);
    AT(ideatek_ui_blur_t, backdrop);
}

static void show_ui_mask(void)
{
    SIZE(ideatek_ui_mask_t);
    AT(ideatek_ui_mask_t, texture);
}

static void show_ease(void)
{
    SIZE(ideatek_ease_t);
}

static void show_ease_direction(void)
{
    SIZE(ideatek_ease_direction_t);
}

static void show_ui_style(void)
{
    SIZE(ideatek_ui_style_t);
    AT(ideatek_ui_style_t, normal);
    AT(ideatek_ui_style_t, hovered);
    AT(ideatek_ui_style_t, pressed);
    AT(ideatek_ui_style_t, disabled);
    AT(ideatek_ui_style_t, focus);
    AT(ideatek_ui_style_t, transition);
    AT(ideatek_ui_style_t, ease);
    AT(ideatek_ui_style_t, ease_direction);
}

static void show_descriptors(void)
{
    const char *descriptors[] = {
        ideatek_ui_root_t_desc, ideatek_ui_node_t_desc, ideatek_ui_unit_t_desc,
        ideatek_ui_length_t_desc, ideatek_ui_display_t_desc,
        ideatek_ui_position_t_desc, ideatek_ui_direction_t_desc,
        ideatek_ui_align_t_desc, ideatek_ui_overflow_t_desc,
        ideatek_ui_layout_t_desc, ideatek_ui_scroll_t_desc,
        ideatek_ui_rect_t_desc, ideatek_flip_t_desc, ideatek_image_mode_t_desc,
        ideatek_ui_box_t_desc, ideatek_ui_image_t_desc,
        ideatek_font_format_t_desc, ideatek_font_t_desc, ideatek_ui_text_t_desc,
        ideatek_text_align_t_desc, ideatek_text_wrap_t_desc,
        ideatek_text_overflow_t_desc, ideatek_ui_text_look_t_desc,
        ideatek_ui_text_span_t_desc, ideatek_pointer_buttons_t_desc,
        ideatek_ui_pointer_t_desc, ideatek_ui_pointer_hit_t_desc,
        ideatek_pickable_flags_t_desc, ideatek_ui_pickable_t_desc,
        ideatek_interaction_t_desc, ideatek_ui_interaction_t_desc,
        ideatek_pointer_event_kind_t_desc, ideatek_ui_pointer_event_t_desc,
        ideatek_ui_focus_t_desc, ideatek_ui_focus_state_t_desc,
        ideatek_focusable_flags_t_desc, ideatek_ui_focusable_t_desc,
        ideatek_ui_neighbors_t_desc, ideatek_ui_focus_move_t_desc,
        ideatek_focus_event_kind_t_desc, ideatek_focus_cause_t_desc,
        ideatek_ui_focus_event_t_desc, ideatek_ui_value_t_desc,
        ideatek_ui_value_event_t_desc, ideatek_button_mode_t_desc,
        ideatek_ui_button_t_desc, ideatek_ui_activated_t_desc,
        ideatek_ui_toggle_t_desc, ideatek_ui_toggle_group_t_desc,
        ideatek_slider_axis_t_desc, ideatek_ui_slider_t_desc,
        ideatek_scroll_flags_t_desc, ideatek_ui_scroll_area_t_desc,
        ideatek_ui_scroll_to_t_desc, ideatek_text_field_flags_t_desc,
        ideatek_ui_text_field_t_desc, ideatek_ui_text_change_t_desc,
        ideatek_ui_caret_t_desc, ideatek_screen_flags_t_desc,
        ideatek_ui_screen_t_desc, ideatek_ui_screen_state_t_desc,
        ideatek_screen_event_kind_t_desc, ideatek_ui_screen_event_t_desc,
        ideatek_gesture_kind_t_desc, ideatek_gesture_phase_t_desc,
        ideatek_ui_gesture_t_desc, ideatek_anchor_flags_t_desc,
        ideatek_ui_anchor_t_desc, ideatek_ui_anchor_state_t_desc,
        ideatek_ui_grid_t_desc, ideatek_ui_grid_item_t_desc,
        ideatek_ui_transform_t_desc, ideatek_ui_opacity_t_desc,
        ideatek_gradient_kind_t_desc, ideatek_ui_gradient_t_desc,
        ideatek_ui_accessible_t_desc, ideatek_ui_shader_t_desc,
        ideatek_ui_blur_t_desc, ideatek_ui_mask_t_desc, ideatek_ease_t_desc,
        ideatek_ease_direction_t_desc, ideatek_ui_style_t_desc };

    for (size_t i = 0; i < sizeof(descriptors) / sizeof(descriptors[0]); i++)
        printf("%s\n", descriptors[i]);
}

static void show_texts(void)
{
    const char *texts[] = {
        TEXT_OF(IDEATEK_UI_ROOT_FIELDS), TEXT_OF(IDEATEK_UI_NODE_FIELDS),
        TEXT_OF(IDEATEK_UI_UNIT_VALUES), TEXT_OF(IDEATEK_UI_LENGTH_FIELDS),
        TEXT_OF(IDEATEK_UI_DISPLAY_VALUES), TEXT_OF(IDEATEK_UI_POSITION_VALUES),
        TEXT_OF(IDEATEK_UI_DIRECTION_VALUES), TEXT_OF(IDEATEK_UI_ALIGN_VALUES),
        TEXT_OF(IDEATEK_UI_OVERFLOW_VALUES), TEXT_OF(IDEATEK_UI_LAYOUT_FIELDS),
        TEXT_OF(IDEATEK_UI_SCROLL_FIELDS), TEXT_OF(IDEATEK_UI_RECT_FIELDS),
        TEXT_OF(IDEATEK_FLIP_VALUES), TEXT_OF(IDEATEK_IMAGE_MODE_VALUES),
        TEXT_OF(IDEATEK_UI_BOX_FIELDS), TEXT_OF(IDEATEK_UI_IMAGE_FIELDS),
        TEXT_OF(IDEATEK_FONT_FORMAT_VALUES), TEXT_OF(IDEATEK_FONT_FIELDS),
        TEXT_OF(IDEATEK_UI_TEXT_FIELDS), TEXT_OF(IDEATEK_TEXT_ALIGN_VALUES),
        TEXT_OF(IDEATEK_TEXT_WRAP_VALUES),
        TEXT_OF(IDEATEK_TEXT_OVERFLOW_VALUES),
        TEXT_OF(IDEATEK_UI_TEXT_LOOK_FIELDS),
        TEXT_OF(IDEATEK_UI_TEXT_SPAN_FIELDS),
        TEXT_OF(IDEATEK_POINTER_BUTTONS_VALUES),
        TEXT_OF(IDEATEK_UI_POINTER_FIELDS),
        TEXT_OF(IDEATEK_UI_POINTER_HIT_FIELDS),
        TEXT_OF(IDEATEK_PICKABLE_FLAGS_VALUES),
        TEXT_OF(IDEATEK_UI_PICKABLE_FIELDS),
        TEXT_OF(IDEATEK_INTERACTION_VALUES),
        TEXT_OF(IDEATEK_UI_INTERACTION_FIELDS),
        TEXT_OF(IDEATEK_POINTER_EVENT_KIND_VALUES),
        TEXT_OF(IDEATEK_UI_POINTER_EVENT_FIELDS),
        TEXT_OF(IDEATEK_UI_FOCUS_FIELDS),
        TEXT_OF(IDEATEK_UI_FOCUS_STATE_FIELDS),
        TEXT_OF(IDEATEK_FOCUSABLE_FLAGS_VALUES),
        TEXT_OF(IDEATEK_UI_FOCUSABLE_FIELDS),
        TEXT_OF(IDEATEK_UI_NEIGHBORS_FIELDS),
        TEXT_OF(IDEATEK_UI_FOCUS_MOVE_FIELDS),
        TEXT_OF(IDEATEK_FOCUS_EVENT_KIND_VALUES),
        TEXT_OF(IDEATEK_FOCUS_CAUSE_VALUES),
        TEXT_OF(IDEATEK_UI_FOCUS_EVENT_FIELDS),
        TEXT_OF(IDEATEK_UI_VALUE_FIELDS),
        TEXT_OF(IDEATEK_UI_VALUE_EVENT_FIELDS),
        TEXT_OF(IDEATEK_BUTTON_MODE_VALUES), TEXT_OF(IDEATEK_UI_BUTTON_FIELDS),
        TEXT_OF(IDEATEK_UI_ACTIVATED_FIELDS), TEXT_OF(IDEATEK_UI_TOGGLE_FIELDS),
        TEXT_OF(IDEATEK_UI_TOGGLE_GROUP_FIELDS),
        TEXT_OF(IDEATEK_SLIDER_AXIS_VALUES), TEXT_OF(IDEATEK_UI_SLIDER_FIELDS),
        TEXT_OF(IDEATEK_SCROLL_FLAGS_VALUES),
        TEXT_OF(IDEATEK_UI_SCROLL_AREA_FIELDS),
        TEXT_OF(IDEATEK_UI_SCROLL_TO_FIELDS),
        TEXT_OF(IDEATEK_TEXT_FIELD_FLAGS_VALUES),
        TEXT_OF(IDEATEK_UI_TEXT_FIELD_FIELDS),
        TEXT_OF(IDEATEK_UI_TEXT_CHANGE_FIELDS),
        TEXT_OF(IDEATEK_UI_CARET_FIELDS), TEXT_OF(IDEATEK_SCREEN_FLAGS_VALUES),
        TEXT_OF(IDEATEK_UI_SCREEN_FIELDS),
        TEXT_OF(IDEATEK_UI_SCREEN_STATE_FIELDS),
        TEXT_OF(IDEATEK_SCREEN_EVENT_KIND_VALUES),
        TEXT_OF(IDEATEK_UI_SCREEN_EVENT_FIELDS),
        TEXT_OF(IDEATEK_GESTURE_KIND_VALUES),
        TEXT_OF(IDEATEK_GESTURE_PHASE_VALUES),
        TEXT_OF(IDEATEK_UI_GESTURE_FIELDS),
        TEXT_OF(IDEATEK_ANCHOR_FLAGS_VALUES), TEXT_OF(IDEATEK_UI_ANCHOR_FIELDS),
        TEXT_OF(IDEATEK_UI_ANCHOR_STATE_FIELDS),
        TEXT_OF(IDEATEK_UI_GRID_FIELDS), TEXT_OF(IDEATEK_UI_GRID_ITEM_FIELDS),
        TEXT_OF(IDEATEK_UI_TRANSFORM_FIELDS),
        TEXT_OF(IDEATEK_UI_OPACITY_FIELDS),
        TEXT_OF(IDEATEK_GRADIENT_KIND_VALUES),
        TEXT_OF(IDEATEK_UI_GRADIENT_FIELDS),
        TEXT_OF(IDEATEK_UI_ACCESSIBLE_FIELDS),
        TEXT_OF(IDEATEK_UI_SHADER_FIELDS), TEXT_OF(IDEATEK_UI_BLUR_FIELDS),
        TEXT_OF(IDEATEK_UI_MASK_FIELDS), TEXT_OF(IDEATEK_EASE_VALUES),
        TEXT_OF(IDEATEK_EASE_DIRECTION_VALUES), TEXT_OF(IDEATEK_UI_STYLE_FIELDS) };

    for (size_t i = 0; i < sizeof(texts) / sizeof(texts[0]); i++)
        printf("%s\n", texts[i]);
}

int main(int argc, char **argv)
{
    static const show_mode_t MODES[] = {
        { "ui_root", show_ui_root }, { "ui_node", show_ui_node },
        { "ui_unit", show_ui_unit }, { "ui_length", show_ui_length },
        { "ui_display", show_ui_display }, { "ui_position", show_ui_position },
        { "ui_direction", show_ui_direction }, { "ui_align", show_ui_align },
        { "ui_overflow", show_ui_overflow }, { "ui_layout", show_ui_layout },
        { "ui_scroll", show_ui_scroll }, { "ui_rect", show_ui_rect },
        { "flip", show_flip }, { "image_mode", show_image_mode },
        { "ui_box", show_ui_box }, { "ui_image", show_ui_image },
        { "font_format", show_font_format }, { "font", show_font },
        { "ui_text", show_ui_text }, { "text_align", show_text_align },
        { "text_wrap", show_text_wrap },
        { "text_overflow", show_text_overflow },
        { "ui_text_look", show_ui_text_look },
        { "ui_text_span", show_ui_text_span },
        { "pointer_buttons", show_pointer_buttons },
        { "ui_pointer", show_ui_pointer },
        { "ui_pointer_hit", show_ui_pointer_hit },
        { "pickable_flags", show_pickable_flags },
        { "ui_pickable", show_ui_pickable },
        { "interaction", show_interaction },
        { "ui_interaction", show_ui_interaction },
        { "pointer_event_kind", show_pointer_event_kind },
        { "ui_pointer_event", show_ui_pointer_event },
        { "ui_focus", show_ui_focus },
        { "ui_focus_state", show_ui_focus_state },
        { "focusable_flags", show_focusable_flags },
        { "ui_focusable", show_ui_focusable },
        { "ui_neighbors", show_ui_neighbors },
        { "ui_focus_move", show_ui_focus_move },
        { "focus_event_kind", show_focus_event_kind },
        { "focus_cause", show_focus_cause },
        { "ui_focus_event", show_ui_focus_event },
        { "ui_value", show_ui_value },
        { "ui_value_event", show_ui_value_event },
        { "button_mode", show_button_mode }, { "ui_button", show_ui_button },
        { "ui_activated", show_ui_activated }, { "ui_toggle", show_ui_toggle },
        { "ui_toggle_group", show_ui_toggle_group },
        { "slider_axis", show_slider_axis }, { "ui_slider", show_ui_slider },
        { "scroll_flags", show_scroll_flags },
        { "ui_scroll_area", show_ui_scroll_area },
        { "ui_scroll_to", show_ui_scroll_to },
        { "text_field_flags", show_text_field_flags },
        { "ui_text_field", show_ui_text_field },
        { "ui_text_change", show_ui_text_change },
        { "ui_caret", show_ui_caret }, { "screen_flags", show_screen_flags },
        { "ui_screen", show_ui_screen },
        { "ui_screen_state", show_ui_screen_state },
        { "screen_event_kind", show_screen_event_kind },
        { "ui_screen_event", show_ui_screen_event },
        { "gesture_kind", show_gesture_kind },
        { "gesture_phase", show_gesture_phase },
        { "ui_gesture", show_ui_gesture },
        { "anchor_flags", show_anchor_flags }, { "ui_anchor", show_ui_anchor },
        { "ui_anchor_state", show_ui_anchor_state },
        { "ui_grid", show_ui_grid }, { "ui_grid_item", show_ui_grid_item },
        { "ui_transform", show_ui_transform },
        { "ui_opacity", show_ui_opacity },
        { "gradient_kind", show_gradient_kind },
        { "ui_gradient", show_ui_gradient },
        { "ui_accessible", show_ui_accessible },
        { "ui_shader", show_ui_shader }, { "ui_blur", show_ui_blur },
        { "ui_mask", show_ui_mask }, { "ease", show_ease },
        { "ease_direction", show_ease_direction },
        { "ui_style", show_ui_style }, { "descriptors", show_descriptors },
        { "texts", show_texts }
    };

    for (size_t i = 0; argc > 1 && i < sizeof(MODES) / sizeof(MODES[0]); i++)
        if (!strcmp(argv[1], MODES[i].name)) {
            MODES[i].show();
            return (0);
        }
    printf("usage: layout <mode>\n");
    return (2);
}
