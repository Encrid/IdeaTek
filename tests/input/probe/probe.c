/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "bridge_native.h"
#include "ideatek_input.h"
#include "ideatek_keyboard.h"
#include "ideatek_text.h"
#include "ideatek_mouse.h"
#include "ideatek_gamepad.h"
#include "ideatek_touch.h"
#include "ideatek_pen.h"
#include "ideatek_sensor.h"

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
    ENUM(api, "ideatek:ideatek_device_kind_t", ideatek_device_kind_t);
    ENUM(api, "ideatek:ideatek_power_t", ideatek_power_t);
    ENUM(api, "ideatek:ideatek_link_t", ideatek_link_t);
    ENUM(api, "ideatek:ideatek_input_event_kind_t", ideatek_input_event_kind_t);
    MASK(api, "ideatek:ideatek_modifier_t", ideatek_modifier_t);
    ENUM(api, "ideatek:ideatek_text_type_t", ideatek_text_type_t);
    ENUM(api, "ideatek:ideatek_capitalize_t", ideatek_capitalize_t);
    ENUM(api, "ideatek:ideatek_text_event_kind_t", ideatek_text_event_kind_t);
    ENUM(api, "ideatek:ideatek_source_t", ideatek_source_t);
    ENUM(api, "ideatek:ideatek_mouse_mode_t", ideatek_mouse_mode_t);
    ENUM(api, "ideatek:ideatek_cursor_t", ideatek_cursor_t);
    ENUM(api, "ideatek:ideatek_joystick_type_t", ideatek_joystick_type_t);
    MASK(api, "ideatek:ideatek_hat_t", ideatek_hat_t);
    ENUM(api, "ideatek:ideatek_joystick_event_kind_t",
    ideatek_joystick_event_kind_t);
    ENUM(api, "ideatek:ideatek_gamepad_type_t", ideatek_gamepad_type_t);
    ENUM(api, "ideatek:ideatek_face_label_t", ideatek_face_label_t);
    ENUM(api, "ideatek:ideatek_gamepad_button_t", ideatek_gamepad_button_t);
    ENUM(api, "ideatek:ideatek_gamepad_axis_t", ideatek_gamepad_axis_t);
    ENUM(api, "ideatek:ideatek_capsense_t", ideatek_capsense_t);
    ENUM(api, "ideatek:ideatek_gamepad_event_kind_t",
    ideatek_gamepad_event_kind_t);
    ENUM(api, "ideatek:ideatek_touch_type_t", ideatek_touch_type_t);
    ENUM(api, "ideatek:ideatek_finger_event_kind_t",
    ideatek_finger_event_kind_t);
    ENUM(api, "ideatek:ideatek_pinch_event_kind_t", ideatek_pinch_event_kind_t);
    ENUM(api, "ideatek:ideatek_pen_type_t", ideatek_pen_type_t);
    ENUM(api, "ideatek:ideatek_pen_axis_t", ideatek_pen_axis_t);
    MASK(api, "ideatek:ideatek_pen_input_t", ideatek_pen_input_t);
    ENUM(api, "ideatek:ideatek_pen_event_kind_t", ideatek_pen_event_kind_t);
    ENUM(api, "ideatek:ideatek_sensor_type_t", ideatek_sensor_type_t);
}

[[maybe_unused]] static uint32_t declare_input_device(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:InputDevice", ideatek_input_device_t));
}

[[maybe_unused]] static uint32_t declare_input_user(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:InputUser", ideatek_input_user_t));
}

[[maybe_unused]] static uint32_t declare_device_state(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:DeviceState", ideatek_device_state_t));
}

[[maybe_unused]] static uint32_t declare_input_event(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:InputEvent", ideatek_input_event_t));
}

[[maybe_unused]] static uint32_t declare_keyboard(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:Keyboard", ideatek_keyboard_t));
}

[[maybe_unused]] static uint32_t declare_key_layout(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:KeyLayout", ideatek_key_layout_t));
}

[[maybe_unused]] static uint32_t declare_key_event(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:KeyEvent", ideatek_key_event_t));
}

[[maybe_unused]] static uint32_t declare_text_request(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:TextRequest", ideatek_text_request_t));
}

[[maybe_unused]] static uint32_t declare_text_event(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:TextEvent", ideatek_text_event_t));
}

[[maybe_unused]] static uint32_t declare_mouse(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:Mouse", ideatek_mouse_t));
}

[[maybe_unused]] static uint32_t declare_mouse_motion(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:MouseMotion", ideatek_mouse_motion_t));
}

[[maybe_unused]] static uint32_t declare_mouse_button(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:MouseButton", ideatek_mouse_button_t));
}

[[maybe_unused]] static uint32_t declare_mouse_wheel(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:MouseWheel", ideatek_mouse_wheel_t));
}

[[maybe_unused]] static uint32_t declare_mouse_request(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:MouseRequest", ideatek_mouse_request_t));
}

[[maybe_unused]] static uint32_t declare_joystick(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:Joystick", ideatek_joystick_t));
}

[[maybe_unused]] static uint32_t declare_joystick_state(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:JoystickState", ideatek_joystick_state_t));
}

[[maybe_unused]] static uint32_t declare_joystick_event(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:JoystickEvent", ideatek_joystick_event_t));
}

[[maybe_unused]] static uint32_t declare_gamepad(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:Gamepad", ideatek_gamepad_t));
}

[[maybe_unused]] static uint32_t declare_gamepad_state(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:GamepadState", ideatek_gamepad_state_t));
}

[[maybe_unused]] static uint32_t declare_gamepad_event(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:GamepadEvent", ideatek_gamepad_event_t));
}

[[maybe_unused]] static uint32_t declare_joystick_request(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:JoystickRequest", ideatek_joystick_request_t));
}

[[maybe_unused]] static uint32_t declare_touch_device(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:TouchDevice", ideatek_touch_device_t));
}

[[maybe_unused]] static uint32_t declare_touch_state(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:TouchState", ideatek_touch_state_t));
}

[[maybe_unused]] static uint32_t declare_finger_event(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:FingerEvent", ideatek_finger_event_t));
}

[[maybe_unused]] static uint32_t declare_pinch_event(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:PinchEvent", ideatek_pinch_event_t));
}

[[maybe_unused]] static uint32_t declare_pen(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:Pen", ideatek_pen_t));
}

[[maybe_unused]] static uint32_t declare_pen_state(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:PenState", ideatek_pen_state_t));
}

[[maybe_unused]] static uint32_t declare_pen_event(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:PenEvent", ideatek_pen_event_t));
}

[[maybe_unused]] static uint32_t declare_sensor(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:Sensor", ideatek_sensor_t));
}

[[maybe_unused]] static uint32_t declare_sensor_request(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:SensorRequest", ideatek_sensor_request_t));
}

[[maybe_unused]] static uint32_t declare_sensor_state(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:SensorState", ideatek_sensor_state_t));
}

[[maybe_unused]] static uint32_t declare_sensor_event(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:SensorEvent", ideatek_sensor_event_t));
}

[[maybe_unused]] static void declare_module(const bridge_api_t *api)
{
    declare_input_device(api);
    declare_input_user(api);
    declare_device_state(api);
    declare_input_event(api);
    declare_keyboard(api);
    declare_key_layout(api);
    declare_key_event(api);
    declare_text_request(api);
    declare_text_event(api);
    declare_mouse(api);
    declare_mouse_motion(api);
    declare_mouse_button(api);
    declare_mouse_wheel(api);
    declare_mouse_request(api);
    declare_joystick(api);
    declare_joystick_state(api);
    declare_joystick_event(api);
    declare_gamepad(api);
    declare_gamepad_state(api);
    declare_gamepad_event(api);
    declare_joystick_request(api);
    declare_touch_device(api);
    declare_touch_state(api);
    declare_finger_event(api);
    declare_pinch_event(api);
    declare_pen(api);
    declare_pen_state(api);
    declare_pen_event(api);
    declare_sensor(api);
    declare_sensor_request(api);
    declare_sensor_state(api);
    declare_sensor_event(api);
}

[[maybe_unused]] static void ask(const bridge_api_t *api)
{
    ideatek_text_request_t text = { .width = 640, .height = 32, .enabled = true,
        .type = IDEATEK_TEXT_TYPE_NUMBER };
    uint64_t id = api->spawn(api);

    declare_fields(api);
    BRIDGE_NATIVE_ATTACH(api, id, declare_text_request(api), text);
    api->attach(api, id, declare_mouse_request(api), BRIDGE_ZEROED);
    api->attach(api, id, declare_joystick_request(api), BRIDGE_ZEROED);
    api->attach(api, id, declare_sensor_request(api), BRIDGE_ZEROED);
}

[[maybe_unused]] static void report(const bridge_api_t *api)
{
    static const char NAME[] = "probe keyboard";
    uint64_t id = api->spawn(api);
    uint32_t at = api->alloc(api, sizeof(NAME) - 1);
    ideatek_input_device_t device = { .name = { .at = at, .len = sizeof(NAME) - 1 },
        .kind = IDEATEK_DEVICE_KIND_KEYBOARD };

    memcpy(api->shared_base + at, NAME, sizeof(NAME) - 1);
    declare_fields(api);
    declare_text_request(api);
    declare_mouse_request(api);
    declare_joystick_request(api);
    declare_sensor_request(api);
    BRIDGE_NATIVE_ATTACH(api, id, declare_input_device(api), device);
    api->attach(api, id, declare_input_user(api), BRIDGE_ZEROED);
    api->attach(api, id, declare_device_state(api), BRIDGE_ZEROED);
    api->attach(api, id, declare_input_event(api), BRIDGE_ZEROED);
    api->attach(api, id, declare_keyboard(api), BRIDGE_ZEROED);
    api->attach(api, id, declare_key_layout(api), BRIDGE_ZEROED);
    api->attach(api, id, declare_key_event(api), BRIDGE_ZEROED);
    api->attach(api, id, declare_text_event(api), BRIDGE_ZEROED);
    api->attach(api, id, declare_mouse(api), BRIDGE_ZEROED);
    api->attach(api, id, declare_mouse_motion(api), BRIDGE_ZEROED);
    api->attach(api, id, declare_mouse_button(api), BRIDGE_ZEROED);
    api->attach(api, id, declare_mouse_wheel(api), BRIDGE_ZEROED);
    api->attach(api, id, declare_joystick(api), BRIDGE_ZEROED);
    api->attach(api, id, declare_joystick_state(api), BRIDGE_ZEROED);
    api->attach(api, id, declare_joystick_event(api), BRIDGE_ZEROED);
    api->attach(api, id, declare_gamepad(api), BRIDGE_ZEROED);
    api->attach(api, id, declare_gamepad_state(api), BRIDGE_ZEROED);
    api->attach(api, id, declare_gamepad_event(api), BRIDGE_ZEROED);
    api->attach(api, id, declare_touch_device(api), BRIDGE_ZEROED);
    api->attach(api, id, declare_touch_state(api), BRIDGE_ZEROED);
    api->attach(api, id, declare_finger_event(api), BRIDGE_ZEROED);
    api->attach(api, id, declare_pinch_event(api), BRIDGE_ZEROED);
    api->attach(api, id, declare_pen(api), BRIDGE_ZEROED);
    api->attach(api, id, declare_pen_state(api), BRIDGE_ZEROED);
    api->attach(api, id, declare_pen_event(api), BRIDGE_ZEROED);
    api->attach(api, id, declare_sensor(api), BRIDGE_ZEROED);
    api->attach(api, id, declare_sensor_state(api), BRIDGE_ZEROED);
    api->attach(api, id, declare_sensor_event(api), BRIDGE_ZEROED);
}

BRIDGE_NATIVE(init)
{
#if defined(ROLE_LATE)
    declare_input_device(api);
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
