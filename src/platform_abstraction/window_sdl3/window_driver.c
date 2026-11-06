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

    while (DRIVERS[i].name && strcmp(DRIVERS[i].name, name))
        i++;
    return (&DRIVERS[i]);
}
