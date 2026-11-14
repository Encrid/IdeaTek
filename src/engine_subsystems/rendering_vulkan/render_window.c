/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "render_inside.h"

bool fit_window(const bridge_api_t *api, const ideatek_swapchain_t **swap,
    const ideatek_vulkan_swapchain_t **vk)
{
    const ideatek_swapchain_t *s = RENDER_FIND(api, render_module.window,
        SWAPCHAIN, ideatek_swapchain_t);
    bool ready = s && s->status == IDEATEK_SWAPCHAIN_STATUS_READY;
    bool same = ready && has_size(s);

    *swap = s;
    *vk = RENDER_FIND(api, render_module.window, VULKAN_SWAPCHAIN,
        ideatek_vulkan_swapchain_t);
    ready &= *vk != nullptr;
    if (ready && fit_target(s))
        return (true);
    if (ready && !same)
        bridge_native_say(api, BRIDGE_LOG_ERR, "no image for the window: "
            "vulkan refuses it");
    skip_captures(api);
    return (false);
}

bool has_size(const ideatek_swapchain_t *swap)
{
    const render_target_t *t = &render_module.target;
    uint64_t have = (uint64_t)t->width << 32 | t->height;

    return (have == ((uint64_t)swap->width << 32 | swap->height));
}

uint32_t gather_cameras(const bridge_api_t *api, uint64_t *out)
{
    const ideatek_camera_t *c;
    int32_t orders[RENDER_CAMERAS];
    uint32_t n = 0;
    uint32_t k;

    for (uint32_t i = 0; i < render_module.camera_count; i++) {
        c = RENDER_FIND(api, render_module.cameras[i], CAMERA,
            ideatek_camera_t);
        if (!c->active || c->target.id != render_module.window)
            continue;
        for (k = n; k > 0 && orders[k - 1] > c->order; k--) {
            orders[k] = orders[k - 1];
            out[k] = out[k - 1];
        }
        orders[k] = c->order;
        out[k] = render_module.cameras[i];
        n++;
    }
    return (n);
}
