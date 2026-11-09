/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "audio_inside.h"

[[gnu::section(MANIFEST_SECTION), gnu::used, gnu::retain]]
static const char WHO[] = MANIFEST;

audio_module_t audio_module;

static void enable_events(void)
{
    for (uint32_t type = SDL_EVENT_AUDIO_DEVICE_ADDED;
        type <= SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED; type++)
        SDL_SetEventEnabled(type, true);
}

BRIDGE_NATIVE(init)
{
    char reason[AUDIO_REASON] = "";

    if (!sdl3_start_subsystem(SDL_INIT_AUDIO, reason, sizeof(reason))) {
        bridge_native_say(api, BRIDGE_LOG_ERR, "no audio: %s", reason);
        return (1);
    }
    declare_types(api);
    add_runs(api);
    enable_events();
    return (0);
}

BRIDGE_NATIVE(shutdown)
{
    close_stream(&audio_module.streams[0]);
    close_stream(&audio_module.streams[1]);
    sdl3_stop_subsystem(SDL_INIT_AUDIO);
    return (0);
}

BRIDGE_NATIVE(reload)
{
    return (0);
}
