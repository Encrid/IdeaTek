/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "input_inside.h"

static void declare_fields(const bridge_api_t *api)
{
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:ideatek_entity_t", ideatek_entity_t,
        BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_DECLARE(api, BRIDGE_ENUM,
        "ideatek:ideatek_input_event_kind_t", ideatek_input_event_kind_t,
        BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_DECLARE(api, BRIDGE_BITMASK, "ideatek:ideatek_modifier_t",
        ideatek_modifier_t, BRIDGE_STRIP, BRIDGE_TABLE,
        BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_DECLARE(api, BRIDGE_ENUM, "ideatek:ideatek_mouse_mode_t",
        ideatek_mouse_mode_t, BRIDGE_STRIP, BRIDGE_TABLE,
        BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_DECLARE(api, BRIDGE_ENUM, "ideatek:ideatek_cursor_t",
        ideatek_cursor_t, BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE);
}

static void declare_more_fields(const bridge_api_t *api)
{
    BRIDGE_NATIVE_DECLARE(api, BRIDGE_BITMASK,
        "ideatek:ideatek_gamepad_button_t", ideatek_gamepad_button_t,
        BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_DECLARE(api, BRIDGE_ENUM,
        "ideatek:ideatek_finger_event_kind_t", ideatek_finger_event_kind_t,
        BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_DECLARE(api, BRIDGE_ENUM, "ideatek:ideatek_window_mode_t",
        ideatek_window_mode_t, BRIDGE_STRIP, BRIDGE_TABLE,
        BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_DECLARE(api, BRIDGE_ENUM, "ideatek:ideatek_surface_t",
        ideatek_surface_t, BRIDGE_STRIP, BRIDGE_TABLE,
        BRIDGE_ON_DESTROY_REMOVE);
}

static void declare_keys(const bridge_api_t *api)
{
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:InputEvent", ideatek_input_event_t,
        BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:Keyboard", ideatek_keyboard_t,
        BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:KeyEvent", ideatek_key_event_t,
        BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:TextRequest",
        ideatek_text_request_t, BRIDGE_STRIP, BRIDGE_TABLE,
        BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:TextEvent", ideatek_text_event_t,
        BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE);
}

static void declare_mouse(const bridge_api_t *api)
{
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:Mouse", ideatek_mouse_t,
        BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:MouseMotion", ideatek_mouse_motion_t,
        BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:MouseButton", ideatek_mouse_button_t,
        BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:MouseWheel", ideatek_mouse_wheel_t,
        BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE);
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:MouseRequest",
        ideatek_mouse_request_t, BRIDGE_STRIP, BRIDGE_TABLE,
        BRIDGE_ON_DESTROY_REMOVE);
}

void declare_types(const bridge_api_t *api)
{
    declare_fields(api);
    declare_more_fields(api);
    declare_keys(api);
    declare_mouse(api);
    declare_pads(api);
}
