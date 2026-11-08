/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "gpu_inside.h"

static void take_state(const bridge_api_t *api, uint64_t id, uint32_t at)
{
    const ideatek_window_state_t *w = (const void *)(api->shared_base + at);
    bool resized = (w->pixel_width != gpu_module.pixel_width)
        | (w->pixel_height != gpu_module.pixel_height);

    if (w->native_window != gpu_module.native_window)
        drop_surface(api);
    gpu_module.window = id;
    gpu_module.surface_kind = w->surface;
    gpu_module.native_display = w->native_display;
    gpu_module.native_window = w->native_window;
    gpu_module.drawable = !w->minimized & (w->pixel_width > 0)
        & (w->pixel_height > 0);
    gpu_module.window_hdr = w->hdr;
    gpu_module.stale |= resized;
    gpu_module.pixel_width = w->pixel_width;
    gpu_module.pixel_height = w->pixel_height;
}

static void follow(const bridge_api_t *api, const bridge_trigger_t *t)
{
    bool gone = !!(t->events & BRIDGE_ON_LEAVE);
    bool mine = t->id == gpu_module.window;
    bool second = !gone & (gpu_module.window != 0) & !mine;

    if (gone & mine) {
        drop_surface(api);
        gpu_module.window = 0;
    }
    if (second)
        bridge_native_say(api, BRIDGE_LOG_WARN,
            "a second window %llu gets no swapchain",
            (unsigned long long)t->id);
    if (!gone & !second)
        take_state(api, t->id, bridge_native_find_value(api, t->id,
            WINDOW_STATE));
}

BRIDGE_NATIVE(window)
{
    const bridge_list_t *list = bridge_native_get_list(api,
        gpu_module.window_list);
    const bridge_trigger_t *t = (const void *)(api->private_base + list->at);

    for (uint32_t i = 0; i < list->count; i++)
        follow(api, &t[i]);
    return (0);
}
