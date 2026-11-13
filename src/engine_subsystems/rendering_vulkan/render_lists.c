/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "render_inside.h"

static uint64_t get_last(const bridge_api_t *api, uint32_t list, uint64_t id)
{
    const bridge_list_t *l = bridge_native_get_list(api,
        render_module.lists[list]);
    const bridge_trigger_t *t = (const void *)(api->private_base + l->at);

    for (uint32_t i = 0; i < l->count; i++)
        id = !(t[i].events & BRIDGE_ON_LEAVE) * t[i].id;
    return (id);
}

static void change_camera(const bridge_api_t *api, uint64_t id, bool leave)
{
    uint64_t *ids = render_module.cameras;
    uint32_t *n = &render_module.camera_count;
    uint32_t k = 0;

    while (k < *n && ids[k] != id)
        k++;
    if (leave && k < *n)
        ids[k] = ids[--(*n)];
    else if (!leave && k == *n && *n < RENDER_CAMERAS)
        ids[(*n)++] = id;
    else if (!leave && k == *n)
        bridge_native_say(api, BRIDGE_LOG_WARN, "no room for cameras in round "
            "%llu: 1 are not drawn", (unsigned long long)render_module.round);
}

BRIDGE_NATIVE(device)
{
    render_module.device.entity = get_last(api, LIST_DEVICE,
        render_module.device.entity);
    return (0);
}

BRIDGE_NATIVE(window)
{
    render_module.window = get_last(api, LIST_WINDOW, render_module.window);
    return (0);
}

BRIDGE_NATIVE(cameras)
{
    const bridge_list_t *l = bridge_native_get_list(api,
        render_module.lists[LIST_CAMERAS]);
    const bridge_trigger_t *t = (const void *)(api->private_base + l->at);

    for (uint32_t i = 0; i < l->count; i++)
        change_camera(api, t[i].id, t[i].events & BRIDGE_ON_LEAVE);
    return (0);
}
