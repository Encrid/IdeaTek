/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "window_inside.h"

static void check_handles(const bridge_api_t *api, ideatek_window_state_t *s)
{
    if (s->surface == IDEATEK_SURFACE_NONE || s->native_window)
        return;
    bridge_native_say(api, BRIDGE_LOG_ERR,
        "the window has no handle of its surface: %s", SDL_GetError());
    s->surface = IDEATEK_SURFACE_NONE;
    s->native_display = 0;
}

#if defined(__APPLE__)
static uint64_t read_window([[maybe_unused]] SDL_PropertiesID props,
    const window_driver_t *d)
{
    if (d->surface != IDEATEK_SURFACE_METAL)
        return (0);
    window_module.view = SDL_Metal_CreateView(window_module.window);
    return ((uint64_t)(uintptr_t)SDL_Metal_GetLayer(window_module.view));
}

#elif defined(_WIN32)
static uint64_t read_window(SDL_PropertiesID props, const window_driver_t *d)
{
    return ((uint64_t)(uintptr_t)SDL_GetPointerProperty(props, d->window_key,
        nullptr));
}

#else
static uint64_t read_window(SDL_PropertiesID props, const window_driver_t *d)
{
    if (d->surface == IDEATEK_SURFACE_XLIB)
        return ((uint64_t)SDL_GetNumberProperty(props, d->window_key, 0));
    return ((uint64_t)(uintptr_t)SDL_GetPointerProperty(props, d->window_key,
        nullptr));
}

#endif

void read_handles(const bridge_api_t *api)
{
    const window_driver_t *d = window_module.driver;
    SDL_PropertiesID props = SDL_GetWindowProperties(window_module.window);
    ideatek_window_state_t *s = &window_module.state;

    s->surface = d->surface;
    s->native_display = (uint64_t)(uintptr_t)SDL_GetPointerProperty(props,
        d->display_key, nullptr);
    s->native_window = read_window(props, d);
    check_handles(api, s);
}
