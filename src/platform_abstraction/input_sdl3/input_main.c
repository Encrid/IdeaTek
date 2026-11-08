/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "input_inside.h"

[[gnu::section(MANIFEST_SECTION), gnu::used, gnu::retain]]
static const char WHO[] = MANIFEST;

input_module_t input_module;

static void add_runs(const bridge_api_t *api)
{
    const uint32_t changes[] = { INPUT_EVENT, KEYBOARD, KEY_EVENT, TEXT_EVENT,
        MOUSE, MOUSE_MOTION, MOUSE_BUTTON, MOUSE_WHEEL, GAMEPAD, GAMEPAD_STATE,
        GAMEPAD_EVENT, FINGER_EVENT };
    bridge_term_t term[1] = {{ WINDOW_STATE, BRIDGE_READ, BRIDGE_AND, 0 }};
    uint32_t both = BRIDGE_ON_ENTER | BRIDGE_ON_ATTACH;

    BRIDGE_NATIVE_ADD_SYSTEM(api, "events", "first", nullptr, 0, changes, 12,
        nullptr, 0);
    input_module.window_list = BRIDGE_NATIVE_OBSERVER(api, "window", both,
        term, 1, nullptr, 0, nullptr, 0);
    term[0].slot = TEXT_REQUEST;
    input_module.text_list = BRIDGE_NATIVE_OBSERVER(api, "text",
        both | BRIDGE_ON_LEAVE, term, 1, nullptr, 0, nullptr, 0);
    term[0].slot = MOUSE_REQUEST;
    input_module.mouse_list = BRIDGE_NATIVE_OBSERVER(api, "mouse", both, term,
        1, nullptr, 0, nullptr, 0);
    term[0].slot = GAMEPAD_REQUEST;
    input_module.pad_list = BRIDGE_NATIVE_OBSERVER(api, "rumble", both, term,
        1, nullptr, 0, nullptr, 0);
}

static void enable_events(void)
{
    static const uint32_t ranges[][2] = {
        { SDL_EVENT_KEY_DOWN, SDL_EVENT_SCREEN_KEYBOARD_HIDDEN },
        { SDL_EVENT_MOUSE_MOTION, SDL_EVENT_MOUSE_REMOVED },
        { SDL_EVENT_JOYSTICK_AXIS_MOTION, SDL_EVENT_JOYSTICK_UPDATE_COMPLETE },
        { SDL_EVENT_GAMEPAD_AXIS_MOTION,
            SDL_EVENT_GAMEPAD_STEAM_HANDLE_UPDATED },
        { SDL_EVENT_FINGER_DOWN, SDL_EVENT_FINGER_CANCELED },
    };

    for (size_t i = 0; i < SDL_arraysize(ranges); i++)
        for (uint32_t type = ranges[i][0]; type <= ranges[i][1]; type++)
            SDL_SetEventEnabled(type, true);
}

BRIDGE_NATIVE(init)
{
    char reason[INPUT_REASON] = "";
    ideatek_keyboard_t keyboard = {};
    ideatek_mouse_t mouse = {};

    if (!sdl3_start_subsystem(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD, reason,
        sizeof(reason))) {
        bridge_native_say(api, BRIDGE_LOG_ERR, "no input: %s", reason);
        return (1);
    }
    enable_events();
    declare_types(api);
    add_runs(api);
    input_module.entity = api->spawn(api);
    BRIDGE_NATIVE_ATTACH(api, input_module.entity, KEYBOARD, keyboard);
    BRIDGE_NATIVE_ATTACH(api, input_module.entity, MOUSE, mouse);
    return (0);
}

BRIDGE_NATIVE(shutdown)
{
    if (input_module.text_on)
        SDL_StopTextInput(first_window());
    for (uint32_t i = 0; i < input_module.pad_count; i++)
        SDL_CloseGamepad(input_module.pads[i].gamepad);
    for (uint32_t i = 0; i < INPUT_CURSORS; i++)
        if (input_module.cursors[i])
            SDL_DestroyCursor(input_module.cursors[i]);
    sdl3_stop_subsystem(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD);
    return (0);
}

BRIDGE_NATIVE(reload)
{
    return (0);
}
