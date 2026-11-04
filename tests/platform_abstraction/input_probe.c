/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "bridge_native.h"
#include "ideatek_input_gamepad.h"
#include "ideatek_input.h"
#include "ideatek_input_keyboard.h"
#include "ideatek_input_mouse.h"
#include "ideatek_input_text.h"
#include "ideatek_input_touch.h"

[[gnu::section(MANIFEST_SECTION), gnu::used, gnu::retain]]
static const char WHO[] = MANIFEST;

#define ENUM(api, name, T) BRIDGE_NATIVE_DECLARE(api, BRIDGE_ENUM, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define MASK(api, name, T) BRIDGE_NATIVE_DECLARE(api, BRIDGE_BITMASK, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define STRUCT(api, name, T) BRIDGE_NATIVE_COMPONENT(api, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)

[[maybe_unused]] static void declare_fields(const bridge_api_t *api)
{
    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    ENUM(api, "ideatek:ideatek_input_event_kind_t",
        ideatek_input_event_kind_t);
    MASK(api, "ideatek:ideatek_modifier_t", ideatek_modifier_t);
    ENUM(api, "ideatek:ideatek_mouse_mode_t", ideatek_mouse_mode_t);
    ENUM(api, "ideatek:ideatek_cursor_t", ideatek_cursor_t);
    ENUM(api, "ideatek:ideatek_finger_event_kind_t",
        ideatek_finger_event_kind_t);
    MASK(api, "ideatek:ideatek_gamepad_button_t", ideatek_gamepad_button_t);
}

[[maybe_unused]] static uint32_t declare_text_request(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:TextRequest", ideatek_text_request_t));
}

[[maybe_unused]] static uint32_t declare_mouse_request(
    const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:MouseRequest", ideatek_mouse_request_t));
}

[[maybe_unused]] static uint32_t declare_gamepad_request(
    const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:GamepadRequest", ideatek_gamepad_request_t));
}

[[maybe_unused]] static void declare_requests(const bridge_api_t *api)
{
    declare_text_request(api);
    declare_mouse_request(api);
    declare_gamepad_request(api);
}

[[maybe_unused]] static void declare_module(const bridge_api_t *api)
{
    STRUCT(api, "ideatek:InputEvent", ideatek_input_event_t);
    STRUCT(api, "ideatek:Keyboard", ideatek_keyboard_t);
    STRUCT(api, "ideatek:KeyEvent", ideatek_key_event_t);
    declare_text_request(api);
    STRUCT(api, "ideatek:TextEvent", ideatek_text_event_t);
    STRUCT(api, "ideatek:Mouse", ideatek_mouse_t);
    STRUCT(api, "ideatek:MouseMotion", ideatek_mouse_motion_t);
    STRUCT(api, "ideatek:MouseButton", ideatek_mouse_button_t);
    STRUCT(api, "ideatek:MouseWheel", ideatek_mouse_wheel_t);
    declare_mouse_request(api);
    STRUCT(api, "ideatek:FingerEvent", ideatek_finger_event_t);
    STRUCT(api, "ideatek:Gamepad", ideatek_gamepad_t);
    STRUCT(api, "ideatek:GamepadState", ideatek_gamepad_state_t);
    STRUCT(api, "ideatek:GamepadEvent", ideatek_gamepad_event_t);
    declare_gamepad_request(api);
}

[[maybe_unused]] static void ask(const bridge_api_t *api)
{
    uint64_t input = api->spawn(api);
    ideatek_text_request_t text = { .x = 1.0F, .y = 2.0F, .width = 3.0F,
        .height = 4.0F, .cursor = 1.0F, .screen_keyboard = true };
    ideatek_mouse_request_t mouse = { .mode = IDEATEK_MOUSE_MODE_CONFINED,
        .cursor = IDEATEK_CURSOR_ARROW };
    ideatek_gamepad_request_t rumble = { .weak = 0.5F, .strong = 0.5F,
        .duration_ms = 100 };

    declare_fields(api);
    BRIDGE_NATIVE_ATTACH(api, input, declare_text_request(api), text);
    BRIDGE_NATIVE_ATTACH(api, input, declare_mouse_request(api), mouse);
    BRIDGE_NATIVE_ATTACH(api, api->spawn(api), declare_gamepad_request(api),
        rumble);
}

[[maybe_unused]] static void report(const bridge_api_t *api)
{
    uint64_t input = api->spawn(api);
    uint64_t gamepad = api->spawn(api);
    ideatek_input_event_t focus = { .order = 0,
        .kind = IDEATEK_INPUT_EVENT_KIND_FOCUS_GAINED };
    ideatek_keyboard_t keyboard = { .modifiers = IDEATEK_MODIFIER_NUM };
    ideatek_key_event_t key = { .character = 'a', .key = 0x00070004,
        .code = 0x00070004, .order = 1, .down = true };
    ideatek_text_event_t text = { .code_point = 'a', .order = 2 };
    ideatek_mouse_t mouse = { .x = 1.0F, .y = 2.0F };
    ideatek_mouse_motion_t motion = { .x = 1.0F, .y = 2.0F, .rel_x = 1.0F,
        .rel_y = 2.0F, .order = 3 };
    ideatek_mouse_button_t button = { .x = 1.0F, .y = 2.0F, .order = 4,
        .button = 1, .down = true, .clicks = 2 };
    ideatek_mouse_wheel_t wheel = { .ticks_y = 1.0F, .order = 5 };
    ideatek_finger_event_t finger = { .id = 1, .x = 1.0F, .y = 2.0F,
        .order = 6, .kind = IDEATEK_FINGER_EVENT_KIND_BEGAN };
    ideatek_gamepad_t pad = { .name = "probe" };
    ideatek_gamepad_state_t state = { .left_trigger = 1.0F,
        .buttons = IDEATEK_GAMEPAD_BUTTON_SOUTH };
    ideatek_gamepad_event_t press = { .gamepad = { gamepad }, .order = 7,
        .button = IDEATEK_GAMEPAD_BUTTON_SOUTH, .down = true };

    declare_fields(api);
    declare_requests(api);
    BRIDGE_NATIVE_ATTACH(api, input, STRUCT(api, "ideatek:InputEvent",
        ideatek_input_event_t), focus);
    BRIDGE_NATIVE_ATTACH(api, input, STRUCT(api, "ideatek:Keyboard",
        ideatek_keyboard_t), keyboard);
    BRIDGE_NATIVE_ATTACH(api, input, STRUCT(api, "ideatek:KeyEvent",
        ideatek_key_event_t), key);
    BRIDGE_NATIVE_ATTACH(api, input, STRUCT(api, "ideatek:TextEvent",
        ideatek_text_event_t), text);
    BRIDGE_NATIVE_ATTACH(api, input, STRUCT(api, "ideatek:Mouse",
        ideatek_mouse_t), mouse);
    BRIDGE_NATIVE_ATTACH(api, input, STRUCT(api, "ideatek:MouseMotion",
        ideatek_mouse_motion_t), motion);
    BRIDGE_NATIVE_ATTACH(api, input, STRUCT(api, "ideatek:MouseButton",
        ideatek_mouse_button_t), button);
    BRIDGE_NATIVE_ATTACH(api, input, STRUCT(api, "ideatek:MouseWheel",
        ideatek_mouse_wheel_t), wheel);
    BRIDGE_NATIVE_ATTACH(api, input, STRUCT(api, "ideatek:FingerEvent",
        ideatek_finger_event_t), finger);
    BRIDGE_NATIVE_ATTACH(api, gamepad, STRUCT(api, "ideatek:Gamepad",
        ideatek_gamepad_t), pad);
    BRIDGE_NATIVE_ATTACH(api, gamepad, STRUCT(api, "ideatek:GamepadState",
        ideatek_gamepad_state_t), state);
    BRIDGE_NATIVE_ATTACH(api, gamepad, STRUCT(api, "ideatek:GamepadEvent",
        ideatek_gamepad_event_t), press);
}

BRIDGE_NATIVE(init)
{
#if defined(ROLE_LATE)
    declare_mouse_request(api);
    declare_fields(api);
#elif defined(ROLE_GAME)
    ask(api);
#elif defined(ROLE_INPUT)
    report(api);
#else
    declare_fields(api);
    declare_module(api);
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
