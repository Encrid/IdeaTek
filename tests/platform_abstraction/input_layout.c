/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <ideatek_input_gamepad.h>
#include <ideatek_input.h>
#include <ideatek_input_keyboard.h>
#include <ideatek_input_mouse.h>
#include <ideatek_input_text.h>
#include <ideatek_input_touch.h>

#define SIZE(type) printf("size %zu\nalignment %zu\n", sizeof(type), \
    alignof(type))
#define AT(type, field) printf("%s %zu\n", #field, offsetof(type, field))
#define TEXT_OF(...) BRIDGE_SCHEMA_TEXT(__VA_ARGS__)

typedef struct {
    const char *name;
    void (*show)(void);
} show_mode_t;

static void show_input_event_kind(void)
{
    SIZE(ideatek_input_event_kind_t);
}

static void show_input_event(void)
{
    SIZE(ideatek_input_event_t);
    AT(ideatek_input_event_t, order);
    AT(ideatek_input_event_t, kind);
}

static void show_modifier(void)
{
    SIZE(ideatek_modifier_t);
}

static void show_keyboard(void)
{
    SIZE(ideatek_keyboard_t);
    AT(ideatek_keyboard_t, keys);
    AT(ideatek_keyboard_t, modifiers);
}

static void show_key_event(void)
{
    SIZE(ideatek_key_event_t);
    AT(ideatek_key_event_t, character);
    AT(ideatek_key_event_t, key);
    AT(ideatek_key_event_t, code);
    AT(ideatek_key_event_t, order);
    AT(ideatek_key_event_t, modifiers);
    AT(ideatek_key_event_t, down);
    AT(ideatek_key_event_t, repeat);
}

static void show_text_request(void)
{
    SIZE(ideatek_text_request_t);
    AT(ideatek_text_request_t, screen_keyboard);
}

static void show_text_event(void)
{
    SIZE(ideatek_text_event_t);
    AT(ideatek_text_event_t, code_point);
    AT(ideatek_text_event_t, order);
}

static void show_mouse_mode(void)
{
    SIZE(ideatek_mouse_mode_t);
}

static void show_cursor(void)
{
    SIZE(ideatek_cursor_t);
}

static void show_mouse(void)
{
    SIZE(ideatek_mouse_t);
    AT(ideatek_mouse_t, x);
    AT(ideatek_mouse_t, y);
    AT(ideatek_mouse_t, buttons);
}

static void show_mouse_motion(void)
{
    SIZE(ideatek_mouse_motion_t);
    AT(ideatek_mouse_motion_t, x);
    AT(ideatek_mouse_motion_t, y);
    AT(ideatek_mouse_motion_t, rel_x);
    AT(ideatek_mouse_motion_t, rel_y);
    AT(ideatek_mouse_motion_t, buttons);
    AT(ideatek_mouse_motion_t, order);
}

static void show_mouse_button(void)
{
    SIZE(ideatek_mouse_button_t);
    AT(ideatek_mouse_button_t, x);
    AT(ideatek_mouse_button_t, y);
    AT(ideatek_mouse_button_t, order);
    AT(ideatek_mouse_button_t, button);
    AT(ideatek_mouse_button_t, down);
}

static void show_mouse_wheel(void)
{
    SIZE(ideatek_mouse_wheel_t);
    AT(ideatek_mouse_wheel_t, ticks_x);
    AT(ideatek_mouse_wheel_t, ticks_y);
    AT(ideatek_mouse_wheel_t, order);
}

static void show_mouse_request(void)
{
    SIZE(ideatek_mouse_request_t);
    AT(ideatek_mouse_request_t, mode);
    AT(ideatek_mouse_request_t, cursor);
}

static void show_finger_event_kind(void)
{
    SIZE(ideatek_finger_event_kind_t);
}

static void show_finger_event(void)
{
    SIZE(ideatek_finger_event_t);
    AT(ideatek_finger_event_t, id);
    AT(ideatek_finger_event_t, x);
    AT(ideatek_finger_event_t, y);
    AT(ideatek_finger_event_t, order);
    AT(ideatek_finger_event_t, kind);
}

static void show_gamepad_button(void)
{
    SIZE(ideatek_gamepad_button_t);
}

static void show_gamepad(void)
{
    SIZE(ideatek_gamepad_t);
    AT(ideatek_gamepad_t, name);
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
}

static void show_gamepad_event(void)
{
    SIZE(ideatek_gamepad_event_t);
    AT(ideatek_gamepad_event_t, gamepad);
    AT(ideatek_gamepad_event_t, order);
    AT(ideatek_gamepad_event_t, button);
    AT(ideatek_gamepad_event_t, down);
}

static void show_gamepad_request(void)
{
    SIZE(ideatek_gamepad_request_t);
    AT(ideatek_gamepad_request_t, weak);
    AT(ideatek_gamepad_request_t, strong);
    AT(ideatek_gamepad_request_t, duration_ms);
}

static void show_descriptors(void)
{
    static const char *const DESCRIPTORS[] = {
        ideatek_input_event_kind_t_desc, ideatek_input_event_t_desc,
        ideatek_modifier_t_desc, ideatek_keyboard_t_desc,
        ideatek_key_event_t_desc, ideatek_text_request_t_desc,
        ideatek_text_event_t_desc, ideatek_mouse_mode_t_desc,
        ideatek_cursor_t_desc, ideatek_mouse_t_desc,
        ideatek_mouse_motion_t_desc, ideatek_mouse_button_t_desc,
        ideatek_mouse_wheel_t_desc, ideatek_mouse_request_t_desc,
        ideatek_finger_event_kind_t_desc, ideatek_finger_event_t_desc,
        ideatek_gamepad_button_t_desc, ideatek_gamepad_t_desc,
        ideatek_gamepad_state_t_desc, ideatek_gamepad_event_t_desc,
        ideatek_gamepad_request_t_desc
    };

    for (size_t i = 0; i < sizeof(DESCRIPTORS) / sizeof(DESCRIPTORS[0]); i++)
        printf("%s\n", DESCRIPTORS[i]);
}

static void show_texts(void)
{
    static const char *const TEXTS[] = {
        TEXT_OF(IDEATEK_INPUT_EVENT_KIND_VALUES),
        TEXT_OF(IDEATEK_INPUT_EVENT_FIELDS),
        TEXT_OF(IDEATEK_MODIFIER_VALUES), TEXT_OF(IDEATEK_KEYBOARD_FIELDS),
        TEXT_OF(IDEATEK_KEY_EVENT_FIELDS),
        TEXT_OF(IDEATEK_TEXT_REQUEST_FIELDS),
        TEXT_OF(IDEATEK_TEXT_EVENT_FIELDS), TEXT_OF(IDEATEK_MOUSE_MODE_VALUES),
        TEXT_OF(IDEATEK_CURSOR_VALUES), TEXT_OF(IDEATEK_MOUSE_FIELDS),
        TEXT_OF(IDEATEK_MOUSE_MOTION_FIELDS),
        TEXT_OF(IDEATEK_MOUSE_BUTTON_FIELDS),
        TEXT_OF(IDEATEK_MOUSE_WHEEL_FIELDS),
        TEXT_OF(IDEATEK_MOUSE_REQUEST_FIELDS),
        TEXT_OF(IDEATEK_FINGER_EVENT_KIND_VALUES),
        TEXT_OF(IDEATEK_FINGER_EVENT_FIELDS),
        TEXT_OF(IDEATEK_GAMEPAD_BUTTON_VALUES),
        TEXT_OF(IDEATEK_GAMEPAD_FIELDS), TEXT_OF(IDEATEK_GAMEPAD_STATE_FIELDS),
        TEXT_OF(IDEATEK_GAMEPAD_EVENT_FIELDS),
        TEXT_OF(IDEATEK_GAMEPAD_REQUEST_FIELDS)
    };

    for (size_t i = 0; i < sizeof(TEXTS) / sizeof(TEXTS[0]); i++)
        printf("%s\n", TEXTS[i]);
}

int main(int argc, char **argv)
{
    static const show_mode_t MODES[] = {
        { "input_event_kind", show_input_event_kind },
        { "input_event", show_input_event }, { "modifier", show_modifier },
        { "keyboard", show_keyboard }, { "key_event", show_key_event },
        { "text_request", show_text_request },
        { "text_event", show_text_event }, { "mouse_mode", show_mouse_mode },
        { "cursor", show_cursor }, { "mouse", show_mouse },
        { "mouse_motion", show_mouse_motion },
        { "mouse_button", show_mouse_button },
        { "mouse_wheel", show_mouse_wheel },
        { "mouse_request", show_mouse_request },
        { "finger_event_kind", show_finger_event_kind },
        { "finger_event", show_finger_event },
        { "gamepad_button", show_gamepad_button }, { "gamepad", show_gamepad },
        { "gamepad_state", show_gamepad_state },
        { "gamepad_event", show_gamepad_event },
        { "gamepad_request", show_gamepad_request },
        { "descriptors", show_descriptors }, { "texts", show_texts }
    };

    for (size_t i = 0; argc > 1 && i < sizeof(MODES) / sizeof(MODES[0]); i++)
        if (!strcmp(argv[1], MODES[i].name)) {
            MODES[i].show();
            return (0);
        }
    printf("usage: layout <mode>\n");
    return (2);
}
