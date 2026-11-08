/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "window_inside.h"

[[gnu::section(MANIFEST_SECTION), gnu::used, gnu::retain]]
static const char WHO[] = MANIFEST;

window_module_t window_module;

static void add_runs(const bridge_api_t *api)
{
    const uint32_t changes[] = { STATE, DISPLAY, VIDEO_MODE, DROP, PASTE };
    bridge_term_t request[1] = {{ REQUEST, BRIDGE_READ, BRIDGE_AND, 0 }};
    bridge_term_t clipboard[1] = {{ CLIPBOARD, BRIDGE_READ, BRIDGE_AND, 0 }};

    BRIDGE_NATIVE_ADD_SYSTEM(api, "events", "first", nullptr, 0, changes, 5,
        nullptr, 0);
    window_module.request_list = BRIDGE_NATIVE_OBSERVER(api, "request",
        BRIDGE_ON_ENTER | BRIDGE_ON_ATTACH | BRIDGE_ON_LEAVE, request, 1,
        nullptr, 0, nullptr, 0);
    window_module.clipboard_list = BRIDGE_NATIVE_OBSERVER(api, "clipboard",
        BRIDGE_ON_ENTER | BRIDGE_ON_ATTACH, clipboard, 1, nullptr, 0, nullptr,
        0);
}

BRIDGE_NATIVE(init)
{
    char reason[WINDOW_REASON] = "";

    if (!sdl3_start_subsystem(SDL_INIT_VIDEO, reason, sizeof(reason))) {
        bridge_native_say(api, BRIDGE_LOG_ERR, "no video: %s", reason);
        return (1);
    }
    window_module.driver = find_driver();
    window_module.displays_dirty = true;
    enable_events();
    declare_types(api);
    add_runs(api);
    return (0);
}

void close_window(void)
{
#if defined(__APPLE__)
    if (window_module.view)
        SDL_Metal_DestroyView(window_module.view);
#endif
    if (window_module.window)
        SDL_DestroyWindow(window_module.window);
    window_module.view = nullptr;
    window_module.window = nullptr;
}

BRIDGE_NATIVE(shutdown)
{
    close_window();
    sdl3_stop_subsystem(SDL_INIT_VIDEO);
    return (0);
}

BRIDGE_NATIVE(reload)
{
    return (0);
}
