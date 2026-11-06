/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "window_inside.h"

static int see_trigger(const bridge_api_t *api, uint64_t id, uint32_t events)
{
    if (events == BRIDGE_ON_LEAVE) {
        if (id != window_module.entity)
            return (0);
        if (window_module.window)
            SDL_HideWindow(window_module.window);
        return (BRIDGE_END);
    }
    if (!window_module.entity)
        window_module.entity = id;
    if (id == window_module.entity)
        window_module.pending = true;
    else
        bridge_native_say(api, BRIDGE_LOG_WARN,
            "the entity %llu asks for a second window, and gets none",
            (unsigned long long)id);
    return (0);
}

BRIDGE_NATIVE(request)
{
    const bridge_list_t *list = bridge_native_get_list(api,
        window_module.request_list);
    const bridge_trigger_t *t = (const bridge_trigger_t *)(api->private_base
        + list->at);
    int status = 0;

    for (uint32_t i = 0; i < list->count; i++)
        if (see_trigger(api, t[i].id, t[i].events) == BRIDGE_END)
            status = BRIDGE_END;
    return (status);
}
