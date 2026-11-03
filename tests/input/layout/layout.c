/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <ideatek_input.h>
#include <ideatek_keyboard.h>
#include <ideatek_text.h>
#include <ideatek_mouse.h>
#include <ideatek_gamepad.h>
#include <ideatek_touch.h>
#include <ideatek_pen.h>
#include <ideatek_sensor.h>

#define SIZE(type) printf("size %zu\nalignment %zu\n", sizeof(type), \
    alignof(type))
#define AT(type, field) printf("%s %zu\n", #field, offsetof(type, field))
#define TEXT_OF(...) BRIDGE_SCHEMA_TEXT(__VA_ARGS__)

typedef struct {
    const char *name;
    void (*show)(void);
} show_mode_t;

static void show_input_device(void)
{
    SIZE(ideatek_input_device_t);
    AT(ideatek_input_device_t, name);
    AT(ideatek_input_device_t, serial);
    AT(ideatek_input_device_t, vendor);
    AT(ideatek_input_device_t, product);
    AT(ideatek_input_device_t, version);
    AT(ideatek_input_device_t, guid);
    AT(ideatek_input_device_t, kind);
    AT(ideatek_input_device_t, rumble);
    AT(ideatek_input_device_t, trigger_rumble);
    AT(ideatek_input_device_t, mono_led);
    AT(ideatek_input_device_t, rgb_led);
    AT(ideatek_input_device_t, player_led);
}

static void show_device_kind(void)
{
    SIZE(ideatek_device_kind_t);
}

static void show_input_user(void)
{
    SIZE(ideatek_input_user_t);
    AT(ideatek_input_user_t, user);
}

static void show_device_state(void)
{
    SIZE(ideatek_device_state_t);
    AT(ideatek_device_state_t, player_index);
    AT(ideatek_device_state_t, percent);
    AT(ideatek_device_state_t, power);
    AT(ideatek_device_state_t, link);
}

static void show_power(void)
{
    SIZE(ideatek_power_t);
}

static void show_link(void)
{
    SIZE(ideatek_link_t);
}

static void show_input_event(void)
{
    SIZE(ideatek_input_event_t);
    AT(ideatek_input_event_t, device);
    AT(ideatek_input_event_t, window);
    AT(ideatek_input_event_t, user);
    AT(ideatek_input_event_t, old_user);
    AT(ideatek_input_event_t, order);
    AT(ideatek_input_event_t, kind);
}

static void show_input_event_kind(void)
{
    SIZE(ideatek_input_event_kind_t);
}

static void show_keyboard(void)
{
    SIZE(ideatek_keyboard_t);
    AT(ideatek_keyboard_t, window);
    AT(ideatek_keyboard_t, keys);
    AT(ideatek_keyboard_t, modifiers);
    AT(ideatek_keyboard_t, ime);
    AT(ideatek_keyboard_t, screen_keyboard);
}

static void show_modifier(void)
{
    SIZE(ideatek_modifier_t);
}

static void show_key_layout(void)
{
    SIZE(ideatek_key_layout_t);
    AT(ideatek_key_layout_t, key);
}

static void show_key_event(void)
{
    SIZE(ideatek_key_event_t);
    AT(ideatek_key_event_t, keyboard);
    AT(ideatek_key_event_t, window);
    AT(ideatek_key_event_t, key);
    AT(ideatek_key_event_t, raw);
    AT(ideatek_key_event_t, order);
    AT(ideatek_key_event_t, code);
    AT(ideatek_key_event_t, modifiers);
    AT(ideatek_key_event_t, down);
    AT(ideatek_key_event_t, repeat);
}

static void show_text_request(void)
{
    SIZE(ideatek_text_request_t);
    AT(ideatek_text_request_t, window);
    AT(ideatek_text_request_t, x);
    AT(ideatek_text_request_t, y);
    AT(ideatek_text_request_t, width);
    AT(ideatek_text_request_t, height);
    AT(ideatek_text_request_t, cursor);
    AT(ideatek_text_request_t, clears);
    AT(ideatek_text_request_t, type);
    AT(ideatek_text_request_t, capitalize);
    AT(ideatek_text_request_t, enabled);
    AT(ideatek_text_request_t, autocorrect);
    AT(ideatek_text_request_t, multiline);
    AT(ideatek_text_request_t, draws_composition);
    AT(ideatek_text_request_t, draws_candidates);
}

static void show_text_type(void)
{
    SIZE(ideatek_text_type_t);
}

static void show_capitalize(void)
{
    SIZE(ideatek_capitalize_t);
}

static void show_text_event(void)
{
    SIZE(ideatek_text_event_t);
    AT(ideatek_text_event_t, keyboard);
    AT(ideatek_text_event_t, window);
    AT(ideatek_text_event_t, text);
    AT(ideatek_text_event_t, start);
    AT(ideatek_text_event_t, length);
    AT(ideatek_text_event_t, index);
    AT(ideatek_text_event_t, count);
    AT(ideatek_text_event_t, selected);
    AT(ideatek_text_event_t, order);
    AT(ideatek_text_event_t, kind);
    AT(ideatek_text_event_t, horizontal);
}

static void show_text_event_kind(void)
{
    SIZE(ideatek_text_event_kind_t);
}

static void show_mouse(void)
{
    SIZE(ideatek_mouse_t);
    AT(ideatek_mouse_t, window);
    AT(ideatek_mouse_t, x);
    AT(ideatek_mouse_t, y);
    AT(ideatek_mouse_t, buttons);
    AT(ideatek_mouse_t, relative);
    AT(ideatek_mouse_t, cursor_found);
    AT(ideatek_mouse_t, warp_works);
}

static void show_mouse_motion(void)
{
    SIZE(ideatek_mouse_motion_t);
    AT(ideatek_mouse_motion_t, mouse);
    AT(ideatek_mouse_motion_t, window);
    AT(ideatek_mouse_motion_t, x);
    AT(ideatek_mouse_motion_t, y);
    AT(ideatek_mouse_motion_t, rel_x);
    AT(ideatek_mouse_motion_t, rel_y);
    AT(ideatek_mouse_motion_t, buttons);
    AT(ideatek_mouse_motion_t, order);
    AT(ideatek_mouse_motion_t, source);
}

static void show_source(void)
{
    SIZE(ideatek_source_t);
}

static void show_mouse_button(void)
{
    SIZE(ideatek_mouse_button_t);
    AT(ideatek_mouse_button_t, mouse);
    AT(ideatek_mouse_button_t, window);
    AT(ideatek_mouse_button_t, x);
    AT(ideatek_mouse_button_t, y);
    AT(ideatek_mouse_button_t, order);
    AT(ideatek_mouse_button_t, button);
    AT(ideatek_mouse_button_t, clicks);
    AT(ideatek_mouse_button_t, source);
    AT(ideatek_mouse_button_t, down);
}

static void show_mouse_wheel(void)
{
    SIZE(ideatek_mouse_wheel_t);
    AT(ideatek_mouse_wheel_t, mouse);
    AT(ideatek_mouse_wheel_t, window);
    AT(ideatek_mouse_wheel_t, x);
    AT(ideatek_mouse_wheel_t, y);
    AT(ideatek_mouse_wheel_t, ticks_x);
    AT(ideatek_mouse_wheel_t, ticks_y);
    AT(ideatek_mouse_wheel_t, mouse_x);
    AT(ideatek_mouse_wheel_t, mouse_y);
    AT(ideatek_mouse_wheel_t, order);
    AT(ideatek_mouse_wheel_t, flipped);
}

static void show_mouse_request(void)
{
    SIZE(ideatek_mouse_request_t);
    AT(ideatek_mouse_request_t, window);
    AT(ideatek_mouse_request_t, pixels);
    AT(ideatek_mouse_request_t, width);
    AT(ideatek_mouse_request_t, height);
    AT(ideatek_mouse_request_t, hot_x);
    AT(ideatek_mouse_request_t, hot_y);
    AT(ideatek_mouse_request_t, warp_x);
    AT(ideatek_mouse_request_t, warp_y);
    AT(ideatek_mouse_request_t, warps);
    AT(ideatek_mouse_request_t, mode);
    AT(ideatek_mouse_request_t, cursor);
    AT(ideatek_mouse_request_t, capture);
    AT(ideatek_mouse_request_t, custom);
}

static void show_mouse_mode(void)
{
    SIZE(ideatek_mouse_mode_t);
}

static void show_cursor(void)
{
    SIZE(ideatek_cursor_t);
}

static void show_joystick(void)
{
    SIZE(ideatek_joystick_t);
    AT(ideatek_joystick_t, axes);
    AT(ideatek_joystick_t, buttons);
    AT(ideatek_joystick_t, hats);
    AT(ideatek_joystick_t, balls);
    AT(ideatek_joystick_t, type);
}

static void show_joystick_type(void)
{
    SIZE(ideatek_joystick_type_t);
}

static void show_joystick_state(void)
{
    SIZE(ideatek_joystick_state_t);
    AT(ideatek_joystick_state_t, buttons);
    AT(ideatek_joystick_state_t, axes);
    AT(ideatek_joystick_state_t, hats);
}

static void show_hat(void)
{
    SIZE(ideatek_hat_t);
}

static void show_joystick_event(void)
{
    SIZE(ideatek_joystick_event_t);
    AT(ideatek_joystick_event_t, joystick);
    AT(ideatek_joystick_event_t, order);
    AT(ideatek_joystick_event_t, index);
    AT(ideatek_joystick_event_t, value);
    AT(ideatek_joystick_event_t, rel_x);
    AT(ideatek_joystick_event_t, rel_y);
    AT(ideatek_joystick_event_t, hat);
    AT(ideatek_joystick_event_t, kind);
    AT(ideatek_joystick_event_t, down);
}

static void show_joystick_event_kind(void)
{
    SIZE(ideatek_joystick_event_kind_t);
}

static void show_gamepad(void)
{
    SIZE(ideatek_gamepad_t);
    AT(ideatek_gamepad_t, type);
    AT(ideatek_gamepad_t, labels);
}

static void show_gamepad_type(void)
{
    SIZE(ideatek_gamepad_type_t);
}

static void show_face_label(void)
{
    SIZE(ideatek_face_label_t);
}

static void show_gamepad_state(void)
{
    SIZE(ideatek_gamepad_state_t);
    AT(ideatek_gamepad_state_t, left_x);
    AT(ideatek_gamepad_state_t, left_y);
    AT(ideatek_gamepad_state_t, right_x);
    AT(ideatek_gamepad_state_t, right_y);
    AT(ideatek_gamepad_state_t, left_trigger);
    AT(ideatek_gamepad_state_t, right_trigger);
    AT(ideatek_gamepad_state_t, buttons);
    AT(ideatek_gamepad_state_t, pressed);
    AT(ideatek_gamepad_state_t, capsense);
}

static void show_gamepad_button(void)
{
    SIZE(ideatek_gamepad_button_t);
}

static void show_gamepad_axis(void)
{
    SIZE(ideatek_gamepad_axis_t);
}

static void show_capsense(void)
{
    SIZE(ideatek_capsense_t);
}

static void show_gamepad_event(void)
{
    SIZE(ideatek_gamepad_event_t);
    AT(ideatek_gamepad_event_t, gamepad);
    AT(ideatek_gamepad_event_t, order);
    AT(ideatek_gamepad_event_t, value);
    AT(ideatek_gamepad_event_t, index);
    AT(ideatek_gamepad_event_t, kind);
    AT(ideatek_gamepad_event_t, down);
}

static void show_gamepad_event_kind(void)
{
    SIZE(ideatek_gamepad_event_kind_t);
}

static void show_joystick_request(void)
{
    SIZE(ideatek_joystick_request_t);
    AT(ideatek_joystick_request_t, duration_ms);
    AT(ideatek_joystick_request_t, trigger_duration_ms);
    AT(ideatek_joystick_request_t, rumbles);
    AT(ideatek_joystick_request_t, trigger_rumbles);
    AT(ideatek_joystick_request_t, player_index);
    AT(ideatek_joystick_request_t, low);
    AT(ideatek_joystick_request_t, high);
    AT(ideatek_joystick_request_t, trigger_left);
    AT(ideatek_joystick_request_t, trigger_right);
    AT(ideatek_joystick_request_t, red);
    AT(ideatek_joystick_request_t, green);
    AT(ideatek_joystick_request_t, blue);
}

static void show_finger(void)
{
    SIZE(ideatek_finger_t);
    AT(ideatek_finger_t, id);
    AT(ideatek_finger_t, window);
    AT(ideatek_finger_t, x);
    AT(ideatek_finger_t, y);
    AT(ideatek_finger_t, pressure);
}

static void show_touch_device(void)
{
    SIZE(ideatek_touch_device_t);
    AT(ideatek_touch_device_t, gamepad);
    AT(ideatek_touch_device_t, type);
    AT(ideatek_touch_device_t, pinch);
}

static void show_touch_type(void)
{
    SIZE(ideatek_touch_type_t);
}

static void show_touch_state(void)
{
    SIZE(ideatek_touch_state_t);
    AT(ideatek_touch_state_t, fingers);
    AT(ideatek_touch_state_t, count);
}

static void show_finger_event(void)
{
    SIZE(ideatek_finger_event_t);
    AT(ideatek_finger_event_t, finger);
    AT(ideatek_finger_event_t, touch);
    AT(ideatek_finger_event_t, window);
    AT(ideatek_finger_event_t, x);
    AT(ideatek_finger_event_t, y);
    AT(ideatek_finger_event_t, dx);
    AT(ideatek_finger_event_t, dy);
    AT(ideatek_finger_event_t, pressure);
    AT(ideatek_finger_event_t, order);
    AT(ideatek_finger_event_t, kind);
}

static void show_finger_event_kind(void)
{
    SIZE(ideatek_finger_event_kind_t);
}

static void show_pinch_event(void)
{
    SIZE(ideatek_pinch_event_t);
    AT(ideatek_pinch_event_t, window);
    AT(ideatek_pinch_event_t, scale);
    AT(ideatek_pinch_event_t, order);
    AT(ideatek_pinch_event_t, kind);
}

static void show_pinch_event_kind(void)
{
    SIZE(ideatek_pinch_event_kind_t);
}

static void show_pen(void)
{
    SIZE(ideatek_pen_t);
    AT(ideatek_pen_t, type);
}

static void show_pen_type(void)
{
    SIZE(ideatek_pen_type_t);
}

static void show_pen_state(void)
{
    SIZE(ideatek_pen_state_t);
    AT(ideatek_pen_state_t, window);
    AT(ideatek_pen_state_t, x);
    AT(ideatek_pen_state_t, y);
    AT(ideatek_pen_state_t, axes);
    AT(ideatek_pen_state_t, flags);
}

static void show_pen_axis(void)
{
    SIZE(ideatek_pen_axis_t);
}

static void show_pen_input(void)
{
    SIZE(ideatek_pen_input_t);
}

static void show_pen_event(void)
{
    SIZE(ideatek_pen_event_t);
    AT(ideatek_pen_event_t, pen);
    AT(ideatek_pen_event_t, window);
    AT(ideatek_pen_event_t, x);
    AT(ideatek_pen_event_t, y);
    AT(ideatek_pen_event_t, value);
    AT(ideatek_pen_event_t, flags);
    AT(ideatek_pen_event_t, order);
    AT(ideatek_pen_event_t, button);
    AT(ideatek_pen_event_t, axis);
    AT(ideatek_pen_event_t, kind);
    AT(ideatek_pen_event_t, eraser);
}

static void show_pen_event_kind(void)
{
    SIZE(ideatek_pen_event_kind_t);
}

static void show_sensor(void)
{
    SIZE(ideatek_sensor_t);
    AT(ideatek_sensor_t, gamepad);
    AT(ideatek_sensor_t, raw_type);
    AT(ideatek_sensor_t, rate);
    AT(ideatek_sensor_t, type);
}

static void show_sensor_type(void)
{
    SIZE(ideatek_sensor_type_t);
}

static void show_sensor_request(void)
{
    SIZE(ideatek_sensor_request_t);
    AT(ideatek_sensor_request_t, enabled);
}

static void show_sensor_state(void)
{
    SIZE(ideatek_sensor_state_t);
    AT(ideatek_sensor_state_t, time);
    AT(ideatek_sensor_state_t, data);
}

static void show_sensor_event(void)
{
    SIZE(ideatek_sensor_event_t);
    AT(ideatek_sensor_event_t, time);
    AT(ideatek_sensor_event_t, sensor);
    AT(ideatek_sensor_event_t, data);
    AT(ideatek_sensor_event_t, order);
}

static void show_descriptors(void)
{
    static const char *const DESCRIPTORS[] = {
        ideatek_device_kind_t_desc,
        ideatek_power_t_desc,
        ideatek_link_t_desc,
        ideatek_input_event_kind_t_desc,
        ideatek_modifier_t_desc,
        ideatek_text_type_t_desc,
        ideatek_capitalize_t_desc,
        ideatek_text_event_kind_t_desc,
        ideatek_source_t_desc,
        ideatek_mouse_mode_t_desc,
        ideatek_cursor_t_desc,
        ideatek_joystick_type_t_desc,
        ideatek_hat_t_desc,
        ideatek_joystick_event_kind_t_desc,
        ideatek_gamepad_type_t_desc,
        ideatek_face_label_t_desc,
        ideatek_gamepad_button_t_desc,
        ideatek_gamepad_axis_t_desc,
        ideatek_capsense_t_desc,
        ideatek_gamepad_event_kind_t_desc,
        ideatek_touch_type_t_desc,
        ideatek_finger_event_kind_t_desc,
        ideatek_pinch_event_kind_t_desc,
        ideatek_pen_type_t_desc,
        ideatek_pen_axis_t_desc,
        ideatek_pen_input_t_desc,
        ideatek_pen_event_kind_t_desc,
        ideatek_sensor_type_t_desc,
        ideatek_input_device_t_desc,
        ideatek_input_user_t_desc,
        ideatek_device_state_t_desc,
        ideatek_input_event_t_desc,
        ideatek_keyboard_t_desc,
        ideatek_key_layout_t_desc,
        ideatek_key_event_t_desc,
        ideatek_text_request_t_desc,
        ideatek_text_event_t_desc,
        ideatek_mouse_t_desc,
        ideatek_mouse_motion_t_desc,
        ideatek_mouse_button_t_desc,
        ideatek_mouse_wheel_t_desc,
        ideatek_mouse_request_t_desc,
        ideatek_joystick_t_desc,
        ideatek_joystick_state_t_desc,
        ideatek_joystick_event_t_desc,
        ideatek_gamepad_t_desc,
        ideatek_gamepad_state_t_desc,
        ideatek_gamepad_event_t_desc,
        ideatek_joystick_request_t_desc,
        ideatek_finger_t_desc,
        ideatek_touch_device_t_desc,
        ideatek_touch_state_t_desc,
        ideatek_finger_event_t_desc,
        ideatek_pinch_event_t_desc,
        ideatek_pen_t_desc,
        ideatek_pen_state_t_desc,
        ideatek_pen_event_t_desc,
        ideatek_sensor_t_desc,
        ideatek_sensor_request_t_desc,
        ideatek_sensor_state_t_desc,
        ideatek_sensor_event_t_desc,
    };

    for (size_t i = 0; i < sizeof(DESCRIPTORS) / sizeof(DESCRIPTORS[0]); i++)
        printf("%s\n", DESCRIPTORS[i]);
}

static void show_texts(void)
{
    static const char *const TEXTS[] = {
        TEXT_OF(IDEATEK_DEVICE_KIND_VALUES),
        TEXT_OF(IDEATEK_POWER_VALUES),
        TEXT_OF(IDEATEK_LINK_VALUES),
        TEXT_OF(IDEATEK_INPUT_EVENT_KIND_VALUES),
        TEXT_OF(IDEATEK_MODIFIER_VALUES),
        TEXT_OF(IDEATEK_TEXT_TYPE_VALUES),
        TEXT_OF(IDEATEK_CAPITALIZE_VALUES),
        TEXT_OF(IDEATEK_TEXT_EVENT_KIND_VALUES),
        TEXT_OF(IDEATEK_SOURCE_VALUES),
        TEXT_OF(IDEATEK_MOUSE_MODE_VALUES),
        TEXT_OF(IDEATEK_CURSOR_VALUES),
        TEXT_OF(IDEATEK_JOYSTICK_TYPE_VALUES),
        TEXT_OF(IDEATEK_HAT_VALUES),
        TEXT_OF(IDEATEK_JOYSTICK_EVENT_KIND_VALUES),
        TEXT_OF(IDEATEK_GAMEPAD_TYPE_VALUES),
        TEXT_OF(IDEATEK_FACE_LABEL_VALUES),
        TEXT_OF(IDEATEK_GAMEPAD_BUTTON_VALUES),
        TEXT_OF(IDEATEK_GAMEPAD_AXIS_VALUES),
        TEXT_OF(IDEATEK_CAPSENSE_VALUES),
        TEXT_OF(IDEATEK_GAMEPAD_EVENT_KIND_VALUES),
        TEXT_OF(IDEATEK_TOUCH_TYPE_VALUES),
        TEXT_OF(IDEATEK_FINGER_EVENT_KIND_VALUES),
        TEXT_OF(IDEATEK_PINCH_EVENT_KIND_VALUES),
        TEXT_OF(IDEATEK_PEN_TYPE_VALUES),
        TEXT_OF(IDEATEK_PEN_AXIS_VALUES),
        TEXT_OF(IDEATEK_PEN_INPUT_VALUES),
        TEXT_OF(IDEATEK_PEN_EVENT_KIND_VALUES),
        TEXT_OF(IDEATEK_SENSOR_TYPE_VALUES),
        TEXT_OF(IDEATEK_INPUT_DEVICE_FIELDS),
        TEXT_OF(IDEATEK_INPUT_USER_FIELDS),
        TEXT_OF(IDEATEK_DEVICE_STATE_FIELDS),
        TEXT_OF(IDEATEK_INPUT_EVENT_FIELDS),
        TEXT_OF(IDEATEK_KEYBOARD_FIELDS),
        TEXT_OF(IDEATEK_KEY_LAYOUT_FIELDS),
        TEXT_OF(IDEATEK_KEY_EVENT_FIELDS),
        TEXT_OF(IDEATEK_TEXT_REQUEST_FIELDS),
        TEXT_OF(IDEATEK_TEXT_EVENT_FIELDS),
        TEXT_OF(IDEATEK_MOUSE_FIELDS),
        TEXT_OF(IDEATEK_MOUSE_MOTION_FIELDS),
        TEXT_OF(IDEATEK_MOUSE_BUTTON_FIELDS),
        TEXT_OF(IDEATEK_MOUSE_WHEEL_FIELDS),
        TEXT_OF(IDEATEK_MOUSE_REQUEST_FIELDS),
        TEXT_OF(IDEATEK_JOYSTICK_FIELDS),
        TEXT_OF(IDEATEK_JOYSTICK_STATE_FIELDS),
        TEXT_OF(IDEATEK_JOYSTICK_EVENT_FIELDS),
        TEXT_OF(IDEATEK_GAMEPAD_FIELDS),
        TEXT_OF(IDEATEK_GAMEPAD_STATE_FIELDS),
        TEXT_OF(IDEATEK_GAMEPAD_EVENT_FIELDS),
        TEXT_OF(IDEATEK_JOYSTICK_REQUEST_FIELDS),
        TEXT_OF(IDEATEK_FINGER_FIELDS),
        TEXT_OF(IDEATEK_TOUCH_DEVICE_FIELDS),
        TEXT_OF(IDEATEK_TOUCH_STATE_FIELDS),
        TEXT_OF(IDEATEK_FINGER_EVENT_FIELDS),
        TEXT_OF(IDEATEK_PINCH_EVENT_FIELDS),
        TEXT_OF(IDEATEK_PEN_FIELDS),
        TEXT_OF(IDEATEK_PEN_STATE_FIELDS),
        TEXT_OF(IDEATEK_PEN_EVENT_FIELDS),
        TEXT_OF(IDEATEK_SENSOR_FIELDS),
        TEXT_OF(IDEATEK_SENSOR_REQUEST_FIELDS),
        TEXT_OF(IDEATEK_SENSOR_STATE_FIELDS),
        TEXT_OF(IDEATEK_SENSOR_EVENT_FIELDS),
    };

    for (size_t i = 0; i < sizeof(TEXTS) / sizeof(TEXTS[0]); i++)
        printf("%s\n", TEXTS[i]);
}

int main(int argc, char **argv)
{
    static const show_mode_t MODES[] = {
        { "input_device", show_input_device },
        { "device_kind", show_device_kind },
        { "input_user", show_input_user },
        { "device_state", show_device_state },
        { "power", show_power },
        { "link", show_link },
        { "input_event", show_input_event },
        { "input_event_kind", show_input_event_kind },
        { "keyboard", show_keyboard },
        { "modifier", show_modifier },
        { "key_layout", show_key_layout },
        { "key_event", show_key_event },
        { "text_request", show_text_request },
        { "text_type", show_text_type },
        { "capitalize", show_capitalize },
        { "text_event", show_text_event },
        { "text_event_kind", show_text_event_kind },
        { "mouse", show_mouse },
        { "mouse_motion", show_mouse_motion },
        { "source", show_source },
        { "mouse_button", show_mouse_button },
        { "mouse_wheel", show_mouse_wheel },
        { "mouse_request", show_mouse_request },
        { "mouse_mode", show_mouse_mode },
        { "cursor", show_cursor },
        { "joystick", show_joystick },
        { "joystick_type", show_joystick_type },
        { "joystick_state", show_joystick_state },
        { "hat", show_hat },
        { "joystick_event", show_joystick_event },
        { "joystick_event_kind", show_joystick_event_kind },
        { "gamepad", show_gamepad },
        { "gamepad_type", show_gamepad_type },
        { "face_label", show_face_label },
        { "gamepad_state", show_gamepad_state },
        { "gamepad_button", show_gamepad_button },
        { "gamepad_axis", show_gamepad_axis },
        { "capsense", show_capsense },
        { "gamepad_event", show_gamepad_event },
        { "gamepad_event_kind", show_gamepad_event_kind },
        { "joystick_request", show_joystick_request },
        { "finger", show_finger },
        { "touch_device", show_touch_device },
        { "touch_type", show_touch_type },
        { "touch_state", show_touch_state },
        { "finger_event", show_finger_event },
        { "finger_event_kind", show_finger_event_kind },
        { "pinch_event", show_pinch_event },
        { "pinch_event_kind", show_pinch_event_kind },
        { "pen", show_pen },
        { "pen_type", show_pen_type },
        { "pen_state", show_pen_state },
        { "pen_axis", show_pen_axis },
        { "pen_input", show_pen_input },
        { "pen_event", show_pen_event },
        { "pen_event_kind", show_pen_event_kind },
        { "sensor", show_sensor },
        { "sensor_type", show_sensor_type },
        { "sensor_request", show_sensor_request },
        { "sensor_state", show_sensor_state },
        { "sensor_event", show_sensor_event },
        { "descriptors", show_descriptors },
        { "texts", show_texts },
    };

    for (size_t i = 0; argc > 1 && i < sizeof(MODES) / sizeof(MODES[0]); i++)
        if (!strcmp(argv[1], MODES[i].name)) {
            MODES[i].show();
            return (0);
        }
    printf("usage: layout <mode>\n");
    return (2);
}
