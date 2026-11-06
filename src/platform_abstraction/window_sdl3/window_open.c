/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdlib.h>
#include <string.h>

#include "window_inside.h"

static void set_icon(const bridge_api_t *api)
{
    const window_settings_t *settings = bridge_native_get_single(api,
        SETTINGS);
    char *path = copy_arena_text(api, settings->icon);
    SDL_Surface *surface = nullptr;

    if (path && strcmp(path, "none")) {
        surface = SDL_LoadPNG(path);
        if (!surface || !SDL_SetWindowIcon(window_module.window, surface))
            bridge_native_say(api, BRIDGE_LOG_WARN, "no icon %s: %s", path,
                SDL_GetError());
        SDL_DestroySurface(surface);
    }
    free(path);
}

static const SDL_DisplayMode *get_exclusive(const bridge_api_t *api,
    const ideatek_window_request_t *request, const window_display_t *display)
{
    const SDL_DisplayMode *exclusive = nullptr;

    if (display && display->listed && !request->video_mode.id)
        exclusive = SDL_GetCurrentDisplayMode(display->id);
    if (display && request->video_mode.id)
        exclusive = find_mode(display, request->video_mode.id);
    if (!exclusive)
        bridge_native_say(api, BRIDGE_LOG_WARN, "the video mode %llu is not "
            "an exclusive mode of the display, so the window fills the "
            "display", (unsigned long long)request->video_mode.id);
    return (exclusive);
}

static void set_fullscreen(const bridge_api_t *api,
    const ideatek_window_request_t *request, const window_display_t *display)
{
    SDL_Window *w = window_module.window;
    const SDL_DisplayMode *exclusive = nullptr;

    if (request->mode == IDEATEK_WINDOW_MODE_EXCLUSIVE_FULLSCREEN)
        exclusive = get_exclusive(api, request, display);
    if (!SDL_SetWindowFullscreenMode(w, exclusive)) {
        bridge_native_say(api, BRIDGE_LOG_WARN, "the system refuses the video "
            "mode %llu: %s, so the window fills the display",
            (unsigned long long)request->video_mode.id, SDL_GetError());
        SDL_SetWindowFullscreenMode(w, nullptr);
    }
    SDL_SetWindowFullscreen(w, true);
}

void set_mode(const bridge_api_t *api, const ideatek_window_request_t *request,
    const window_display_t *display)
{
    SDL_Window *w = window_module.window;
    int place = display ? (int)SDL_WINDOWPOS_CENTERED_DISPLAY(display->id)
        : (int)SDL_WINDOWPOS_CENTERED;

    SDL_SetWindowPosition(w, place, place);
    if (request->mode != IDEATEK_WINDOW_MODE_WINDOWED) {
        set_fullscreen(api, request, display);
        return;
    }
    SDL_SetWindowFullscreen(w, false);
    SDL_SetWindowSize(w, (int)request->width, (int)request->height);
    SDL_SetWindowPosition(w, place, place);
}

bool open_window(const bridge_api_t *api,
    const ideatek_window_request_t *request)
{
    SDL_WindowFlags flags = SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE
        | SDL_WINDOW_HIGH_PIXEL_DENSITY | window_module.driver->flags;

    window_module.window = SDL_CreateWindow(request->title,
        (int)request->width, (int)request->height, flags);
    if (!window_module.window) {
        bridge_native_say(api, BRIDGE_LOG_ERR, "no window: %s", SDL_GetError());
        return (false);
    }
    set_icon(api);
    read_handles(api);
    set_mode(api, request, get_target(request));
    SDL_ShowWindow(window_module.window);
    return (true);
}
