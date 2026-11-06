/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "window_inside.h"

static const window_driver_t DRIVERS[] = {
#if defined(_WIN32)
    { "windows", IDEATEK_SURFACE_WINDOWS, 0, SDL_KMOD_CTRL,
        SDL_PROP_WINDOW_WIN32_INSTANCE_POINTER,
        SDL_PROP_WINDOW_WIN32_HWND_POINTER },
#elif defined(__APPLE__)
    { "cocoa", IDEATEK_SURFACE_METAL, SDL_WINDOW_METAL, SDL_KMOD_GUI,
        nullptr, nullptr },
#else
    { "x11", IDEATEK_SURFACE_XLIB, 0, SDL_KMOD_CTRL,
        SDL_PROP_WINDOW_X11_DISPLAY_POINTER,
        SDL_PROP_WINDOW_X11_WINDOW_NUMBER },
    { "wayland", IDEATEK_SURFACE_WAYLAND, 0, SDL_KMOD_CTRL,
        SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER,
        SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER },
#endif
    { nullptr, IDEATEK_SURFACE_NONE, 0, SDL_KMOD_CTRL, nullptr, nullptr },
};

const window_driver_t *find_driver(void)
{
    const char *name = SDL_GetCurrentVideoDriver();
    size_t i = 0;

    while (DRIVERS[i].name && (!name || strcmp(DRIVERS[i].name, name)))
        i++;
    return (&DRIVERS[i]);
}

static void check_handles(const bridge_api_t *api, ideatek_window_state_t *s)
{
    if (s->surface == IDEATEK_SURFACE_NONE || s->native_window)
        return;
    bridge_native_say(api, BRIDGE_LOG_ERR,
        "the window has no handle of its surface: %s", SDL_GetError());
    s->surface = IDEATEK_SURFACE_NONE;
    s->native_display = 0;
}

void read_handles(const bridge_api_t *api)
{
    const window_driver_t *d = window_module.driver;
    SDL_PropertiesID props = SDL_GetWindowProperties(window_module.window);
    ideatek_window_state_t *s = &window_module.state;

    s->surface = d->surface;
    s->native_display = (uint64_t)(uintptr_t)SDL_GetPointerProperty(props,
        d->display_key, nullptr);
    if (d->surface == IDEATEK_SURFACE_XLIB)
        s->native_window = (uint64_t)SDL_GetNumberProperty(props,
            d->window_key, 0);
    else
        s->native_window = (uint64_t)(uintptr_t)SDL_GetPointerProperty(props,
            d->window_key, nullptr);
    if (d->surface == IDEATEK_SURFACE_METAL) {
        window_module.view = SDL_Metal_CreateView(window_module.window);
        s->native_window = (uint64_t)(uintptr_t)SDL_Metal_GetLayer(
            window_module.view);
    }
    check_handles(api, s);
}
