/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_hints.h>
#include <SDL3/SDL_stdinc.h>

#include "sdl3_start.h"

typedef struct {
    uint32_t first;
    uint32_t last;
} sdl3_start_range_t;

static const sdl3_start_range_t OFF[] = {
    { SDL_EVENT_TERMINATING, SDL_EVENT_SYSTEM_THEME_CHANGED },
    { SDL_EVENT_PINCH_BEGIN, SDL_EVENT_PINCH_END },
    { SDL_EVENT_AUDIO_DEVICE_ADDED, SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED },
    { SDL_EVENT_SENSOR_UPDATE, SDL_EVENT_SENSOR_UPDATE },
    { SDL_EVENT_PEN_PROXIMITY_IN, SDL_EVENT_PEN_AXIS },
    { SDL_EVENT_CAMERA_DEVICE_ADDED, SDL_EVENT_CAMERA_DEVICE_DENIED },
    { SDL_EVENT_RENDER_TARGETS_RESET, SDL_EVENT_RENDER_DEVICE_LOST },
    { SDL_EVENT_PRIVATE0, SDL_EVENT_PRIVATE3 },
    { SDL_EVENT_USER, SDL_EVENT_LAST },
};

static void disable_events(void)
{
    for (size_t i = 0; i < SDL_arraysize(OFF); i++)
        for (uint32_t type = OFF[i].first; type <= OFF[i].last; type++)
            SDL_SetEventEnabled(type, false);
}

static bool start_events(void)
{
    bool started = false;

    SDL_SetHintWithPriority(SDL_HINT_NO_SIGNAL_HANDLERS, "1",
        SDL_HINT_OVERRIDE);
    started = SDL_InitSubSystem(SDL_INIT_EVENTS);
    SDL_ResetHint(SDL_HINT_NO_SIGNAL_HANDLERS);
    return (started);
}

bool sdl3_start_subsystem(SDL_InitFlags flags, char *reason, size_t size)
{
    bool started = false;

    if (SDL_WasInit(0) == 0)
        disable_events();
    if (!start_events()) {
        SDL_strlcpy(reason, SDL_GetError(), size);
        return (false);
    }
    if (flags == SDL_INIT_EVENTS)
        return (true);
    started = SDL_InitSubSystem(flags);
    if (!started)
        SDL_strlcpy(reason, SDL_GetError(), size);
    SDL_QuitSubSystem(SDL_INIT_EVENTS);
    return (started);
}

void sdl3_stop_subsystem(SDL_InitFlags flags)
{
    SDL_QuitSubSystem(flags);
    if (SDL_WasInit(0) == 0)
        SDL_Quit();
}
