/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "frame_inside.h"

static void follow(const bridge_trigger_t *t)
{
    bool gone = !!(t->events & BRIDGE_ON_LEAVE);
    bool mine = t->id == frame_module.window;
    bool take = !gone & (frame_module.window == 0);
    bool drop = gone & mine;

    frame_module.window = take * t->id + !take * !drop * frame_module.window;
}

BRIDGE_NATIVE(window)
{
    const bridge_list_t *list = bridge_native_get_list(api,
        frame_module.window_list);
    const bridge_trigger_t *t = (const void *)(api->private_base + list->at);

    for (uint32_t i = 0; i < list->count; i++)
        follow(&t[i]);
    return (0);
}
