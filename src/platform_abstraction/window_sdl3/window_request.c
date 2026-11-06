/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "window_inside.h"

window_display_t *get_target(const ideatek_window_request_t *request)
{
    window_display_t *display = nullptr;

    if (request->target == IDEATEK_DISPLAY_TARGET_CHOSEN)
        display = find_display(0, request->display.id);
    if (!display)
        display = find_display(SDL_GetPrimaryDisplay(), 0);
    return (display);
}

static bool is_new_mode(const ideatek_window_request_t *a,
    const ideatek_window_request_t *b)
{
    return (a->display.id != b->display.id
        || a->video_mode.id != b->video_mode.id || a->width != b->width
        || a->height != b->height || a->mode != b->mode
        || a->target != b->target);
}

static void read_request(const bridge_api_t *api, uint32_t at,
    ideatek_window_request_t *r)
{
    memcpy(r, api->shared_base + at, sizeof(*r));
    if (cut_text(r->title, r->title, strnlen(r->title, sizeof(r->title))))
        bridge_native_say(api, BRIDGE_LOG_WARN, "the title is longer than "
            "127 bytes, and the window shows its first %zu bytes",
            strlen(r->title));
}

void apply_request(const bridge_api_t *api)
{
    uint32_t at = bridge_native_find_value(api, window_module.entity,
        REQUEST);
    ideatek_window_request_t r;
    bool was_open = window_module.window;

    window_module.pending = false;
    read_request(api, at, &r);
    if (!was_open && !open_window(api, &r))
        return;
    if (was_open && strcmp(r.title, window_module.applied.title))
        SDL_SetWindowTitle(window_module.window, r.title);
    if (was_open && (is_new_mode(&r, &window_module.applied)
        || window_module.moved))
        set_mode(api, &r, get_target(&r));
    window_module.applied = r;
    window_module.moved = false;
    window_module.dirty = true;
}
