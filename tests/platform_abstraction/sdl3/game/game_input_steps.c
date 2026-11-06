/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdlib.h>
#include <string.h>

#include "game.h"

static SDL_WindowID first_window(void)
{
    int count = 0;
    SDL_Window **windows = SDL_GetWindows(&count);
    SDL_WindowID id = count ? SDL_GetWindowID(windows[0]) : 0;

    SDL_free(windows);
    return (id);
}

static void push_key(const char *args)
{
    SDL_Event e = {};
    unsigned scancode = 0;
    unsigned key = 0;
    unsigned mod = 0;
    int down = 1;
    int repeat = 0;

    sscanf(args, "%x %x %x %d %d", &scancode, &key, &mod, &down, &repeat);
    e.type = down ? SDL_EVENT_KEY_DOWN : SDL_EVENT_KEY_UP;
    e.key.windowID = first_window();
    e.key.scancode = (SDL_Scancode)scancode;
    e.key.key = key;
    e.key.mod = (SDL_Keymod)mod;
    e.key.down = down;
    e.key.repeat = repeat;
    SDL_PushEvent(&e);
}

static void push_text(char *args)
{
    SDL_Event e = {};

    e.type = SDL_EVENT_TEXT_INPUT;
    e.text.windowID = first_window();
    e.text.text = args;
    SDL_PushEvent(&e);
}

static void push_mouse(const char *verb, const char *args)
{
    SDL_Event e = {};
    float a[4] = {};
    unsigned n = 0;

    sscanf(args, "%f %f %f %f %u", &a[0], &a[1], &a[2], &a[3], &n);
    e.motion.windowID = first_window();
    if (!strcmp(verb, "motion")) {
        e.type = SDL_EVENT_MOUSE_MOTION;
        e.motion.x = a[0];
        e.motion.y = a[1];
        e.motion.xrel = a[2];
        e.motion.yrel = a[3];
        e.motion.state = n;
    } else if (!strcmp(verb, "wheel")) {
        e.type = SDL_EVENT_MOUSE_WHEEL;
        e.wheel.x = a[0];
        e.wheel.y = a[1];
        e.wheel.direction = a[2] != 0.0F ? SDL_MOUSEWHEEL_FLIPPED
            : SDL_MOUSEWHEEL_NORMAL;
    } else {
        e.type = a[1] != 0.0F ? SDL_EVENT_MOUSE_BUTTON_DOWN
            : SDL_EVENT_MOUSE_BUTTON_UP;
        e.button.button = (Uint8)a[0];
        e.button.down = a[1] != 0.0F;
        e.button.clicks = (Uint8)a[2];
        e.button.x = a[3];
        e.button.y = (float)n;
    }
    SDL_PushEvent(&e);
}

static bool SDLCALL rumble(void *data, Uint16 low, Uint16 high)
{
    (void)data;
    game.rumble_low = (float)low / 65535.0F;
    game.rumble_high = (float)high / 65535.0F;
    return (true);
}

static void add_pad(char *args)
{
    SDL_VirtualJoystickDesc desc;
    SDL_JoystickID id;

    if (game.pad_count >= 20)
        return;
    SDL_INIT_INTERFACE(&desc);
    desc.type = SDL_JOYSTICK_TYPE_GAMEPAD;
    desc.naxes = SDL_GAMEPAD_AXIS_COUNT;
    desc.nbuttons = SDL_GAMEPAD_BUTTON_COUNT;
    desc.name = args;
    desc.Rumble = rumble;
    id = SDL_AttachVirtualJoystick(&desc);
    game.pad_ids[game.pad_count] = id;
    game.pads[game.pad_count++] = SDL_OpenJoystick(id);
}

static void remove_pad(void)
{
    if (!game.pad_count)
        return;
    game.pad_count--;
    SDL_CloseJoystick(game.pads[game.pad_count]);
    SDL_DetachVirtualJoystick(game.pad_ids[game.pad_count]);
}

static void set_pad(const char *verb, const char *args)
{
    int n = 0;
    int v = 0;

    sscanf(args, "%d %d", &n, &v);
    if (!game.pad_count)
        return;
    if (!strcmp(verb, "padbutton"))
        SDL_SetJoystickVirtualButton(game.pads[0], n, v != 0);
    else
        SDL_SetJoystickVirtualAxis(game.pads[0], n, (Sint16)v);
}

static void ask_text(const bridge_api_t *api, const char *args)
{
    ideatek_text_request_t r = {};
    int keyboard = 0;

    sscanf(args, "%f %f %f %f %f %d", &r.x, &r.y, &r.width, &r.height,
        &r.cursor, &keyboard);
    r.screen_keyboard = keyboard;
    BRIDGE_NATIVE_ATTACH(api, game.input_entity, game.input.text_request, r);
}

static void ask_mouse(const bridge_api_t *api, const char *args)
{
    unsigned mode = 0;
    unsigned cursor = 0;
    ideatek_mouse_request_t r = {};

    sscanf(args, "%u %u", &mode, &cursor);
    r.mode = (ideatek_mouse_mode_t)mode;
    r.cursor = (ideatek_cursor_t)cursor;
    BRIDGE_NATIVE_ATTACH(api, game.input_entity, game.input.mouse_request, r);
}

static void ask_rumble(const bridge_api_t *api, const char *args, bool pad)
{
    ideatek_gamepad_request_t r = {};

    sscanf(args, "%f %f %u", &r.weak, &r.strong, &r.duration_ms);
    BRIDGE_NATIVE_ATTACH(api, pad ? game.pad_entity : game.input_entity,
        game.input.gamepad_request, r);
}

static void say_rumble(const bridge_api_t *api)
{
    bridge_native_say(api, BRIDGE_LOG_INFO, "r%u rumble low %.2f high %.2f",
        game.round, game.rumble_low, game.rumble_high);
}

static void set_hint(const char *name, const char *args)
{
    SDL_SetHintWithPriority(name, args, SDL_HINT_OVERRIDE);
}

static void say_sdl(const bridge_api_t *api, const char *verb,
    const char *args)
{
    SDL_Window *w = SDL_GetWindowFromID(first_window());
    SDL_GUID guid = SDL_StringToGUID(args);
    char *mapping = SDL_GetGamepadMappingForGUID(guid);

    if (!strcmp(verb, "textactive"))
        bridge_native_say(api, BRIDGE_LOG_INFO, "r%u text active %d",
            game.round, w && SDL_TextInputActive(w));
    if (!strcmp(verb, "mousestate"))
        bridge_native_say(api, BRIDGE_LOG_INFO, "r%u mouse relative %d grab "
            "%d shown %d default %d", game.round,
            w && SDL_GetWindowRelativeMouseMode(w), w && SDL_GetWindowMouseGrab(w),
            SDL_CursorVisible(), SDL_GetCursor() == SDL_GetDefaultCursor());
    if (!strcmp(verb, "hint"))
        bridge_native_say(api, BRIDGE_LOG_INFO, "r%u hint %s [%s]", game.round,
            args, SDL_GetHint(args) ? SDL_GetHint(args) : "");
    if (!strcmp(verb, "hasmapping"))
        bridge_native_say(api, BRIDGE_LOG_INFO, "r%u mapping %d", game.round,
            mapping != nullptr);
    SDL_free(mapping);
}

static void ask_text_other(const bridge_api_t *api)
{
    ideatek_text_request_t r = { .width = 10.0F, .height = 10.0F };

    BRIDGE_NATIVE_ATTACH(api, api->spawn(api), game.input.text_request, r);
}

bool game_input_step(const bridge_api_t *api, const char *verb, char *args)
{
    bool done = true;

    if (!strcmp(verb, "keyevent"))
        push_key(args);
    else if (!strcmp(verb, "text"))
        push_text(args);
    else if (!strcmp(verb, "motion") || !strcmp(verb, "wheel")
        || !strcmp(verb, "button"))
        push_mouse(verb, args);
    else if (!strcmp(verb, "addpad"))
        add_pad(args);
    else if (!strcmp(verb, "removepad"))
        remove_pad();
    else if (!strcmp(verb, "padbutton") || !strcmp(verb, "padaxis"))
        set_pad(verb, args);
    else if (!strcmp(verb, "textreq"))
        ask_text(api, args);
    else if (!strcmp(verb, "textoff"))
        api->detach(api, game.input_entity, game.input.text_request);
    else if (!strcmp(verb, "mousereq"))
        ask_mouse(api, args);
    else if (!strcmp(verb, "rumble") || !strcmp(verb, "rumbleoff"))
        ask_rumble(api, args, !strcmp(verb, "rumble"));
    else if (!strcmp(verb, "rumbled"))
        say_rumble(api);
    else if (!strcmp(verb, "focus"))
        set_hint("IDEATEK_TEST_FOCUS", args);
    else if (!strcmp(verb, "touch"))
        set_hint("IDEATEK_TEST_TOUCH", args);
    else if (!strcmp(verb, "density"))
        set_hint("IDEATEK_TEST_DENSITY", args);
    else if (!strcmp(verb, "textother"))
        ask_text_other(api);
    else if (!strcmp(verb, "textactive") || !strcmp(verb, "mousestate")
        || !strcmp(verb, "hint") || !strcmp(verb, "hasmapping"))
        say_sdl(api, verb, args);
    else
        done = false;
    return (done);
}
