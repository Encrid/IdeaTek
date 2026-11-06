/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "window_inside.h"

static void wait_for_events(const bridge_api_t *api)
{
    uint32_t at = 0;
    int64_t left = 0;
    double wait = 0.0;

    if (window_module.entity)
        at = bridge_native_find_value(api, window_module.entity,
            EVENT_WAIT);
    if (at && window_module.taken) {
        memcpy(&wait, api->shared_base + at, sizeof(wait));
        left = (int64_t)window_module.taken + (int64_t)(wait * 1e9)
            - (int64_t)SDL_GetTicksNS();
    }
    if (left > 0)
        SDL_WaitEventTimeout(nullptr, (Sint32)((left + 999999) / 1000000));
    else
        SDL_PumpEvents();
}

static bool take_events(const bridge_api_t *api)
{
    bool asked = SDL_HasEvent(SDL_EVENT_QUIT)
        || SDL_HasEvent(SDL_EVENT_WINDOW_CLOSE_REQUESTED);

    count_pastes();
    if (SDL_HasEvents(SDL_EVENT_WINDOW_FIRST, SDL_EVENT_WINDOW_LAST))
        window_module.dirty = true;
    if (SDL_HasEvents(SDL_EVENT_DISPLAY_FIRST, SDL_EVENT_DISPLAY_LAST))
        window_module.displays_dirty = true;
    SDL_FlushEvents(SDL_EVENT_QUIT, SDL_EVENT_QUIT);
    SDL_FlushEvents(SDL_EVENT_DISPLAY_FIRST, SDL_EVENT_DISPLAY_LAST);
    SDL_FlushEvents(SDL_EVENT_WINDOW_FIRST, SDL_EVENT_WINDOW_LAST);
    SDL_FlushEvents(SDL_EVENT_CLIPBOARD_UPDATE, SDL_EVENT_CLIPBOARD_UPDATE);
    take_drops(api);
    return (asked);
}

static int answer_close(const bridge_api_t *api, bool asked)
{
    const window_settings_t *settings = bridge_native_get_single(api,
        SETTINGS);

    if (asked && settings->close_on_request) {
        if (window_module.window)
            SDL_HideWindow(window_module.window);
        return (BRIDGE_END);
    }
    if (asked != window_module.close_asked) {
        window_module.close_asked = asked;
        window_module.dirty = true;
    }
    return (0);
}

BRIDGE_NATIVE(events)
{
    bool asked;
    int status;

    if (!window_module.drops.at && !start_items(api))
        return (1);
    wait_for_events(api);
    turn_items(api);
    asked = take_events(api);
    window_module.taken = SDL_GetTicksNS();
    if (window_module.displays_dirty)
        sync_displays(api);
    if (window_module.pending || window_module.moved)
        apply_request(api);
    spawn_pastes(api);
    status = answer_close(api, asked);
    attach_state(api);
    return (status);
}

void enable_events(void)
{
    static const uint32_t ranges[][2] = {
        { SDL_EVENT_QUIT, SDL_EVENT_QUIT },
        { SDL_EVENT_DISPLAY_FIRST, SDL_EVENT_DISPLAY_LAST },
        { SDL_EVENT_WINDOW_FIRST, SDL_EVENT_WINDOW_LAST },
        { SDL_EVENT_CLIPBOARD_UPDATE, SDL_EVENT_CLIPBOARD_UPDATE },
        { SDL_EVENT_DROP_FILE, SDL_EVENT_DROP_POSITION },
    };

    for (size_t i = 0; i < SDL_arraysize(ranges); i++)
        for (uint32_t type = ranges[i][0]; type <= ranges[i][1]; type++)
            SDL_SetEventEnabled(type, true);
}
