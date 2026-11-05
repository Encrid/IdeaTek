/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "bridge_native.h"
#include "ideatek_animation.h"
#include "ideatek_localization.h"
#include "ideatek_ui.h"

[[gnu::section(MANIFEST_SECTION), gnu::used, gnu::retain]]
static const char WHO[] = MANIFEST;

#define STRUCT(api, name, T) BRIDGE_NATIVE_COMPONENT(api, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define ENUM(api, name, T) BRIDGE_NATIVE_DECLARE(api, BRIDGE_ENUM, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define MASK(api, name, T) BRIDGE_NATIVE_DECLARE(api, BRIDGE_BITMASK, name, \
    T, BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define TAG(api, name) BRIDGE_NATIVE_DECLARE_TAG(api, name, BRIDGE_STRIP, \
    BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)

[[maybe_unused]] static void declare_fields(const bridge_api_t *api)
{
    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    STRUCT(api, "ideatek:ideatek_vec3_t", ideatek_vec3_t);
    STRUCT(api, "ideatek:ideatek_color_t", ideatek_color_t);
    ENUM(api, "ideatek:ideatek_ui_unit_t", ideatek_ui_unit_t);
    STRUCT(api, "ideatek:ideatek_ui_length_t", ideatek_ui_length_t);
    ENUM(api, "ideatek:ideatek_ui_display_t", ideatek_ui_display_t);
    ENUM(api, "ideatek:ideatek_ui_position_t", ideatek_ui_position_t);
    ENUM(api, "ideatek:ideatek_ui_direction_t", ideatek_ui_direction_t);
    ENUM(api, "ideatek:ideatek_ui_align_t", ideatek_ui_align_t);
    ENUM(api, "ideatek:ideatek_ui_overflow_t", ideatek_ui_overflow_t);
    MASK(api, "ideatek:ideatek_flip_t", ideatek_flip_t);
    ENUM(api, "ideatek:ideatek_image_mode_t", ideatek_image_mode_t);
    ENUM(api, "ideatek:ideatek_font_format_t", ideatek_font_format_t);
    ENUM(api, "ideatek:ideatek_text_align_t", ideatek_text_align_t);
    ENUM(api, "ideatek:ideatek_text_wrap_t", ideatek_text_wrap_t);
    ENUM(api, "ideatek:ideatek_text_overflow_t", ideatek_text_overflow_t);
    MASK(api, "ideatek:ideatek_pointer_buttons_t", ideatek_pointer_buttons_t);
    MASK(api, "ideatek:ideatek_pickable_flags_t", ideatek_pickable_flags_t);
    ENUM(api, "ideatek:ideatek_interaction_t", ideatek_interaction_t);
    ENUM(api, "ideatek:ideatek_pointer_event_kind_t",
        ideatek_pointer_event_kind_t);
    MASK(api, "ideatek:ideatek_focusable_flags_t", ideatek_focusable_flags_t);
    ENUM(api, "ideatek:ideatek_focus_event_kind_t", ideatek_focus_event_kind_t);
    ENUM(api, "ideatek:ideatek_focus_cause_t", ideatek_focus_cause_t);
    ENUM(api, "ideatek:ideatek_button_mode_t", ideatek_button_mode_t);
    ENUM(api, "ideatek:ideatek_slider_axis_t", ideatek_slider_axis_t);
    MASK(api, "ideatek:ideatek_scroll_flags_t", ideatek_scroll_flags_t);
    MASK(api, "ideatek:ideatek_text_field_flags_t", ideatek_text_field_flags_t);
    MASK(api, "ideatek:ideatek_screen_flags_t", ideatek_screen_flags_t);
    ENUM(api, "ideatek:ideatek_screen_event_kind_t",
        ideatek_screen_event_kind_t);
    ENUM(api, "ideatek:ideatek_gesture_kind_t", ideatek_gesture_kind_t);
    ENUM(api, "ideatek:ideatek_gesture_phase_t", ideatek_gesture_phase_t);
    MASK(api, "ideatek:ideatek_anchor_flags_t", ideatek_anchor_flags_t);
    ENUM(api, "ideatek:ideatek_gradient_kind_t", ideatek_gradient_kind_t);
    ENUM(api, "ideatek:ideatek_ease_t", ideatek_ease_t);
    ENUM(api, "ideatek:ideatek_ease_direction_t", ideatek_ease_direction_t);
}

[[maybe_unused]] static void declare_late(const bridge_api_t *api)
{
    STRUCT(api, "ideatek:UiRoot", ideatek_ui_root_t);
    declare_fields(api);
}

[[maybe_unused]] static void declare_all(const bridge_api_t *api)
{
    declare_fields(api);
    STRUCT(api, "ideatek:UiRoot", ideatek_ui_root_t);
    STRUCT(api, "ideatek:UiNode", ideatek_ui_node_t);
    STRUCT(api, "ideatek:UiLayout", ideatek_ui_layout_t);
    STRUCT(api, "ideatek:UiScroll", ideatek_ui_scroll_t);
    STRUCT(api, "ideatek:UiRect", ideatek_ui_rect_t);
    STRUCT(api, "ideatek:UiBox", ideatek_ui_box_t);
    STRUCT(api, "ideatek:UiImage", ideatek_ui_image_t);
    STRUCT(api, "ideatek:Font", ideatek_font_t);
    STRUCT(api, "ideatek:UiText", ideatek_ui_text_t);
    STRUCT(api, "ideatek:UiTextLook", ideatek_ui_text_look_t);
    STRUCT(api, "ideatek:UiTextSpan", ideatek_ui_text_span_t);
    STRUCT(api, "ideatek:UiPointer", ideatek_ui_pointer_t);
    STRUCT(api, "ideatek:UiPointerHit", ideatek_ui_pointer_hit_t);
    STRUCT(api, "ideatek:UiPickable", ideatek_ui_pickable_t);
    STRUCT(api, "ideatek:UiInteraction", ideatek_ui_interaction_t);
    STRUCT(api, "ideatek:UiPointerEvent", ideatek_ui_pointer_event_t);
    STRUCT(api, "ideatek:UiFocus", ideatek_ui_focus_t);
    STRUCT(api, "ideatek:UiFocusState", ideatek_ui_focus_state_t);
    STRUCT(api, "ideatek:UiFocusable", ideatek_ui_focusable_t);
    STRUCT(api, "ideatek:UiNeighbors", ideatek_ui_neighbors_t);
    STRUCT(api, "ideatek:UiFocusMove", ideatek_ui_focus_move_t);
    STRUCT(api, "ideatek:UiFocusEvent", ideatek_ui_focus_event_t);
    STRUCT(api, "ideatek:UiValue", ideatek_ui_value_t);
    STRUCT(api, "ideatek:UiValueEvent", ideatek_ui_value_event_t);
    STRUCT(api, "ideatek:UiButton", ideatek_ui_button_t);
    STRUCT(api, "ideatek:UiActivated", ideatek_ui_activated_t);
    STRUCT(api, "ideatek:UiToggle", ideatek_ui_toggle_t);
    STRUCT(api, "ideatek:UiToggleGroup", ideatek_ui_toggle_group_t);
    STRUCT(api, "ideatek:UiSlider", ideatek_ui_slider_t);
    STRUCT(api, "ideatek:UiScrollArea", ideatek_ui_scroll_area_t);
    STRUCT(api, "ideatek:UiScrollTo", ideatek_ui_scroll_to_t);
    STRUCT(api, "ideatek:UiTextField", ideatek_ui_text_field_t);
    STRUCT(api, "ideatek:UiTextChange", ideatek_ui_text_change_t);
    STRUCT(api, "ideatek:UiCaret", ideatek_ui_caret_t);
    STRUCT(api, "ideatek:UiScreen", ideatek_ui_screen_t);
    STRUCT(api, "ideatek:UiScreenState", ideatek_ui_screen_state_t);
    STRUCT(api, "ideatek:UiScreenEvent", ideatek_ui_screen_event_t);
    STRUCT(api, "ideatek:UiGesture", ideatek_ui_gesture_t);
    STRUCT(api, "ideatek:UiAnchor", ideatek_ui_anchor_t);
    STRUCT(api, "ideatek:UiAnchorState", ideatek_ui_anchor_state_t);
    STRUCT(api, "ideatek:UiGrid", ideatek_ui_grid_t);
    STRUCT(api, "ideatek:UiGridItem", ideatek_ui_grid_item_t);
    STRUCT(api, "ideatek:UiTransform", ideatek_ui_transform_t);
    STRUCT(api, "ideatek:UiOpacity", ideatek_ui_opacity_t);
    STRUCT(api, "ideatek:UiGradient", ideatek_ui_gradient_t);
    STRUCT(api, "ideatek:UiAccessible", ideatek_ui_accessible_t);
    STRUCT(api, "ideatek:UiShader", ideatek_ui_shader_t);
    STRUCT(api, "ideatek:UiBlur", ideatek_ui_blur_t);
    STRUCT(api, "ideatek:UiMask", ideatek_ui_mask_t);
    STRUCT(api, "ideatek:UiStyle", ideatek_ui_style_t);
}

[[maybe_unused]] static void role_game(const bridge_api_t *api)
{
    uint64_t id = api->spawn(api);

    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    STRUCT(api, "ideatek:ideatek_vec3_t", ideatek_vec3_t);
    STRUCT(api, "ideatek:ideatek_color_t", ideatek_color_t);
    ENUM(api, "ideatek:ideatek_ui_unit_t", ideatek_ui_unit_t);
    STRUCT(api, "ideatek:ideatek_ui_length_t", ideatek_ui_length_t);
    ENUM(api, "ideatek:ideatek_ui_display_t", ideatek_ui_display_t);
    ENUM(api, "ideatek:ideatek_ui_position_t", ideatek_ui_position_t);
    ENUM(api, "ideatek:ideatek_ui_direction_t", ideatek_ui_direction_t);
    ENUM(api, "ideatek:ideatek_ui_align_t", ideatek_ui_align_t);
    ENUM(api, "ideatek:ideatek_ui_overflow_t", ideatek_ui_overflow_t);
    MASK(api, "ideatek:ideatek_flip_t", ideatek_flip_t);
    ENUM(api, "ideatek:ideatek_image_mode_t", ideatek_image_mode_t);
    ENUM(api, "ideatek:ideatek_font_format_t", ideatek_font_format_t);
    ENUM(api, "ideatek:ideatek_text_align_t", ideatek_text_align_t);
    ENUM(api, "ideatek:ideatek_text_wrap_t", ideatek_text_wrap_t);
    ENUM(api, "ideatek:ideatek_text_overflow_t", ideatek_text_overflow_t);
    MASK(api, "ideatek:ideatek_pointer_buttons_t", ideatek_pointer_buttons_t);
    MASK(api, "ideatek:ideatek_pickable_flags_t", ideatek_pickable_flags_t);
    MASK(api, "ideatek:ideatek_focusable_flags_t", ideatek_focusable_flags_t);
    ENUM(api, "ideatek:ideatek_button_mode_t", ideatek_button_mode_t);
    ENUM(api, "ideatek:ideatek_slider_axis_t", ideatek_slider_axis_t);
    MASK(api, "ideatek:ideatek_scroll_flags_t", ideatek_scroll_flags_t);
    MASK(api, "ideatek:ideatek_text_field_flags_t", ideatek_text_field_flags_t);
    MASK(api, "ideatek:ideatek_screen_flags_t", ideatek_screen_flags_t);
    MASK(api, "ideatek:ideatek_anchor_flags_t", ideatek_anchor_flags_t);
    ENUM(api, "ideatek:ideatek_gradient_kind_t", ideatek_gradient_kind_t);
    ENUM(api, "ideatek:ideatek_ease_t", ideatek_ease_t);
    ENUM(api, "ideatek:ideatek_ease_direction_t", ideatek_ease_direction_t);
    api->attach(api, id, STRUCT(api, "ideatek:UiRoot", ideatek_ui_root_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UiNode", ideatek_ui_node_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UiLayout", ideatek_ui_layout_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UiBox", ideatek_ui_box_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UiImage", ideatek_ui_image_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:Font", ideatek_font_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UiText", ideatek_ui_text_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UiTextLook",
        ideatek_ui_text_look_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UiTextSpan",
        ideatek_ui_text_span_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UiPickable",
        ideatek_ui_pickable_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UiFocus", ideatek_ui_focus_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UiFocusable",
        ideatek_ui_focusable_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UiNeighbors",
        ideatek_ui_neighbors_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UiFocusMove",
        ideatek_ui_focus_move_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UiValue", ideatek_ui_value_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UiButton", ideatek_ui_button_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UiToggle", ideatek_ui_toggle_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UiToggleGroup",
        ideatek_ui_toggle_group_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UiSlider", ideatek_ui_slider_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UiScrollArea",
        ideatek_ui_scroll_area_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UiScrollTo",
        ideatek_ui_scroll_to_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UiTextField",
        ideatek_ui_text_field_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UiScreen", ideatek_ui_screen_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UiAnchor", ideatek_ui_anchor_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UiStyle", ideatek_ui_style_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UiScroll", ideatek_ui_scroll_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UiPointer", ideatek_ui_pointer_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UiGrid", ideatek_ui_grid_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UiGridItem",
        ideatek_ui_grid_item_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UiTransform",
        ideatek_ui_transform_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UiOpacity", ideatek_ui_opacity_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UiGradient",
        ideatek_ui_gradient_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UiAccessible",
        ideatek_ui_accessible_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UiShader", ideatek_ui_shader_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UiBlur", ideatek_ui_blur_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UiMask", ideatek_ui_mask_t),
        BRIDGE_ZEROED);
}

[[maybe_unused]] static void role_module(const bridge_api_t *api)
{
    uint64_t id = api->spawn(api);

    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    STRUCT(api, "ideatek:ideatek_vec3_t", ideatek_vec3_t);
    STRUCT(api, "ideatek:ideatek_position_t", ideatek_position_t);
    STRUCT(api, "ideatek:ideatek_color_t", ideatek_color_t);
    ENUM(api, "ideatek:ideatek_ui_unit_t", ideatek_ui_unit_t);
    STRUCT(api, "ideatek:ideatek_ui_length_t", ideatek_ui_length_t);
    ENUM(api, "ideatek:ideatek_ui_display_t", ideatek_ui_display_t);
    ENUM(api, "ideatek:ideatek_ui_position_t", ideatek_ui_position_t);
    ENUM(api, "ideatek:ideatek_ui_direction_t", ideatek_ui_direction_t);
    ENUM(api, "ideatek:ideatek_ui_align_t", ideatek_ui_align_t);
    ENUM(api, "ideatek:ideatek_ui_overflow_t", ideatek_ui_overflow_t);
    MASK(api, "ideatek:ideatek_flip_t", ideatek_flip_t);
    ENUM(api, "ideatek:ideatek_image_mode_t", ideatek_image_mode_t);
    ENUM(api, "ideatek:ideatek_font_format_t", ideatek_font_format_t);
    ENUM(api, "ideatek:ideatek_text_align_t", ideatek_text_align_t);
    ENUM(api, "ideatek:ideatek_text_wrap_t", ideatek_text_wrap_t);
    ENUM(api, "ideatek:ideatek_text_overflow_t", ideatek_text_overflow_t);
    MASK(api, "ideatek:ideatek_pointer_buttons_t", ideatek_pointer_buttons_t);
    MASK(api, "ideatek:ideatek_pickable_flags_t", ideatek_pickable_flags_t);
    ENUM(api, "ideatek:ideatek_interaction_t", ideatek_interaction_t);
    ENUM(api, "ideatek:ideatek_pointer_event_kind_t",
        ideatek_pointer_event_kind_t);
    MASK(api, "ideatek:ideatek_focusable_flags_t", ideatek_focusable_flags_t);
    ENUM(api, "ideatek:ideatek_focus_event_kind_t", ideatek_focus_event_kind_t);
    ENUM(api, "ideatek:ideatek_focus_cause_t", ideatek_focus_cause_t);
    ENUM(api, "ideatek:ideatek_button_mode_t", ideatek_button_mode_t);
    ENUM(api, "ideatek:ideatek_slider_axis_t", ideatek_slider_axis_t);
    MASK(api, "ideatek:ideatek_scroll_flags_t", ideatek_scroll_flags_t);
    MASK(api, "ideatek:ideatek_text_field_flags_t", ideatek_text_field_flags_t);
    MASK(api, "ideatek:ideatek_screen_flags_t", ideatek_screen_flags_t);
    ENUM(api, "ideatek:ideatek_screen_event_kind_t",
        ideatek_screen_event_kind_t);
    ENUM(api, "ideatek:ideatek_gesture_kind_t", ideatek_gesture_kind_t);
    ENUM(api, "ideatek:ideatek_gesture_phase_t", ideatek_gesture_phase_t);
    MASK(api, "ideatek:ideatek_anchor_flags_t", ideatek_anchor_flags_t);
    ENUM(api, "ideatek:ideatek_gradient_kind_t", ideatek_gradient_kind_t);
    ENUM(api, "ideatek:ideatek_ease_t", ideatek_ease_t);
    ENUM(api, "ideatek:ideatek_ease_direction_t", ideatek_ease_direction_t);
    STRUCT(api, "ideatek:UiRoot", ideatek_ui_root_t);
    STRUCT(api, "ideatek:UiNode", ideatek_ui_node_t);
    STRUCT(api, "ideatek:UiLayout", ideatek_ui_layout_t);
    STRUCT(api, "ideatek:UiBox", ideatek_ui_box_t);
    STRUCT(api, "ideatek:UiImage", ideatek_ui_image_t);
    STRUCT(api, "ideatek:Font", ideatek_font_t);
    STRUCT(api, "ideatek:UiText", ideatek_ui_text_t);
    STRUCT(api, "ideatek:UiTextLook", ideatek_ui_text_look_t);
    STRUCT(api, "ideatek:UiTextSpan", ideatek_ui_text_span_t);
    STRUCT(api, "ideatek:UiPickable", ideatek_ui_pickable_t);
    STRUCT(api, "ideatek:UiFocus", ideatek_ui_focus_t);
    STRUCT(api, "ideatek:UiFocusable", ideatek_ui_focusable_t);
    STRUCT(api, "ideatek:UiNeighbors", ideatek_ui_neighbors_t);
    STRUCT(api, "ideatek:UiFocusMove", ideatek_ui_focus_move_t);
    STRUCT(api, "ideatek:UiValue", ideatek_ui_value_t);
    STRUCT(api, "ideatek:UiButton", ideatek_ui_button_t);
    STRUCT(api, "ideatek:UiToggle", ideatek_ui_toggle_t);
    STRUCT(api, "ideatek:UiToggleGroup", ideatek_ui_toggle_group_t);
    STRUCT(api, "ideatek:UiSlider", ideatek_ui_slider_t);
    STRUCT(api, "ideatek:UiScrollArea", ideatek_ui_scroll_area_t);
    STRUCT(api, "ideatek:UiScrollTo", ideatek_ui_scroll_to_t);
    STRUCT(api, "ideatek:UiTextField", ideatek_ui_text_field_t);
    STRUCT(api, "ideatek:UiScreen", ideatek_ui_screen_t);
    STRUCT(api, "ideatek:UiAnchor", ideatek_ui_anchor_t);
    STRUCT(api, "ideatek:UiStyle", ideatek_ui_style_t);
    STRUCT(api, "ideatek:UiScroll", ideatek_ui_scroll_t);
    STRUCT(api, "ideatek:UiPointer", ideatek_ui_pointer_t);
    STRUCT(api, "ideatek:UiGrid", ideatek_ui_grid_t);
    STRUCT(api, "ideatek:UiGridItem", ideatek_ui_grid_item_t);
    STRUCT(api, "ideatek:UiTransform", ideatek_ui_transform_t);
    STRUCT(api, "ideatek:UiOpacity", ideatek_ui_opacity_t);
    STRUCT(api, "ideatek:UiGradient", ideatek_ui_gradient_t);
    STRUCT(api, "ideatek:UiAccessible", ideatek_ui_accessible_t);
    STRUCT(api, "ideatek:UiShader", ideatek_ui_shader_t);
    STRUCT(api, "ideatek:UiBlur", ideatek_ui_blur_t);
    STRUCT(api, "ideatek:UiMask", ideatek_ui_mask_t);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:WorldTransform",
        ideatek_world_transform_t, BRIDGE_STRIP, BRIDGE_TABLE,
        BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:AnimationCurves",
        ideatek_animation_curves_t, BRIDGE_STRIP, BRIDGE_TABLE,
        BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:LocalizedText",
        ideatek_localized_text_t, BRIDGE_STRIP, BRIDGE_TABLE,
        BRIDGE_ON_DESTROY_REMOVE);
    api->attach(api, id, STRUCT(api, "ideatek:UiRect", ideatek_ui_rect_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UiScroll", ideatek_ui_scroll_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UiPointer", ideatek_ui_pointer_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UiPointerHit",
        ideatek_ui_pointer_hit_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UiInteraction",
        ideatek_ui_interaction_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UiPointerEvent",
        ideatek_ui_pointer_event_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UiFocusState",
        ideatek_ui_focus_state_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UiFocusEvent",
        ideatek_ui_focus_event_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UiValueEvent",
        ideatek_ui_value_event_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UiActivated",
        ideatek_ui_activated_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UiTextChange",
        ideatek_ui_text_change_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UiCaret", ideatek_ui_caret_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UiScreenState",
        ideatek_ui_screen_state_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UiScreenEvent",
        ideatek_ui_screen_event_t), BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UiGesture", ideatek_ui_gesture_t),
        BRIDGE_ZEROED);
    api->attach(api, id, STRUCT(api, "ideatek:UiAnchorState",
        ideatek_ui_anchor_state_t), BRIDGE_ZEROED);
}

BRIDGE_NATIVE(init)
{
#if defined(ROLE_LATE)
    declare_late(api);
#elif defined(ROLE_GAME)
    role_game(api);
#elif defined(ROLE_MODULE)
    role_module(api);
#else
    declare_all(api);
#endif
    return (0);
}

BRIDGE_NATIVE(shutdown)
{
    return (0);
}

BRIDGE_NATIVE(reload)
{
    return (1);
}
