/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "audio_inside.h"

static void take_events(const bridge_api_t *api)
{
    SDL_Event e;

    while (SDL_PeepEvents(&e, 1, SDL_GETEVENT, SDL_EVENT_AUDIO_DEVICE_ADDED,
        SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED) == 1) {
        if (e.type == SDL_EVENT_AUDIO_DEVICE_ADDED)
            add_device(api, e.adevice.which, e.adevice.recording);
        if (e.type == SDL_EVENT_AUDIO_DEVICE_REMOVED)
            remove_device(api, e.adevice.which);
    }
}

BRIDGE_NATIVE(round)
{
    if (!audio_module.started)
        start_audio(api);
    if (audio_module.broken)
        return (1);
    audio_module.side = 1 - audio_module.side;
    SDL_PumpEvents();
    take_events(api);
    update_defaults(api);
    apply_stream(api, &audio_module.streams[0]);
    apply_stream(api, &audio_module.streams[1]);
    take_recorded(api);
    attach_state(api, &audio_module.streams[0]);
    attach_state(api, &audio_module.streams[1]);
    return (0);
}
