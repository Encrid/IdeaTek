/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "input_inside.h"

static const SDL_SystemCursor CURSORS[INPUT_CURSORS] = {
    SDL_SYSTEM_CURSOR_DEFAULT, SDL_SYSTEM_CURSOR_DEFAULT,
    SDL_SYSTEM_CURSOR_TEXT, SDL_SYSTEM_CURSOR_CROSSHAIR,
    SDL_SYSTEM_CURSOR_POINTER, SDL_SYSTEM_CURSOR_EW_RESIZE,
    SDL_SYSTEM_CURSOR_NS_RESIZE, SDL_SYSTEM_CURSOR_NWSE_RESIZE,
    SDL_SYSTEM_CURSOR_NESW_RESIZE, SDL_SYSTEM_CURSOR_MOVE,
    SDL_SYSTEM_CURSOR_NOT_ALLOWED,
};

SDL_Window *first_window(void)
{
    int count = 0;
    SDL_Window **windows = SDL_GetWindows(&count);
    SDL_Window *window = count ? windows[0] : nullptr;

    SDL_free(windows);
    return (window);
}

static void apply_mode(const bridge_api_t *api, SDL_Window *w,
    ideatek_mouse_mode_t mode)
{
    bool done = true;

    if (mode > IDEATEK_MOUSE_MODE_CONFINED)
        mode = IDEATEK_MOUSE_MODE_NORMAL;
    done &= SDL_SetWindowRelativeMouseMode(w,
        mode == IDEATEK_MOUSE_MODE_RELATIVE);
    done &= SDL_SetWindowMouseGrab(w, mode == IDEATEK_MOUSE_MODE_CONFINED);
    if (mode == IDEATEK_MOUSE_MODE_HIDDEN)
        done &= SDL_HideCursor();
    else if (mode != IDEATEK_MOUSE_MODE_RELATIVE)
        done &= SDL_ShowCursor();
    if (!done)
        bridge_native_say(api, BRIDGE_LOG_WARN, "no mouse mode %u: %s", mode,
            SDL_GetError());
}

static SDL_Cursor *system_cursor(ideatek_cursor_t cursor)
{
    if (cursor == IDEATEK_CURSOR_DEFAULT || cursor >= INPUT_CURSORS)
        return (SDL_GetDefaultCursor());
    if (!input_module.cursors[cursor])
        input_module.cursors[cursor] = SDL_CreateSystemCursor(
            CURSORS[cursor]);
    return (input_module.cursors[cursor]);
}

static void apply_cursor(const bridge_api_t *api, ideatek_cursor_t cursor)
{
    SDL_Cursor *c = system_cursor(cursor);

    if (!c || !SDL_SetCursor(c))
        bridge_native_say(api, BRIDGE_LOG_WARN, "no cursor %u: %s", cursor,
            SDL_GetError());
}

BRIDGE_NATIVE(mouse)
{
    const bridge_list_t *list = bridge_native_get_list(api,
        input_module.mouse_list);
    const bridge_trigger_t *t = (const bridge_trigger_t *)(api->private_base
        + list->at);
    SDL_Window *w = first_window();
    const ideatek_mouse_request_t *r;

    for (uint32_t i = 0; w && i < list->count; i++) {
        if (t[i].id != input_module.entity)
            continue;
        r = (const void *)(api->shared_base + bridge_native_find_value(api,
            t[i].id, MOUSE_REQUEST));
        apply_mode(api, w, r->mode);
        apply_cursor(api, r->cursor);
    }
    return (0);
}
