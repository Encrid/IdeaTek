/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "gpu_inside.h"

static void settle_device(const bridge_api_t *api)
{
    uint32_t at = bridge_native_find_value(api, gpu_module.request,
        GPU_REQUEST);
    const ideatek_gpu_request_t *r = (const void *)(api->shared_base + at);
    uint32_t chosen = GPU_GPUS;
    uint32_t hit;

    drop_device(api);
    gpu_module.remake = false;
    for (uint32_t i = 0; i < gpu_module.gpu_count; i++) {
        hit = gpu_module.gpus[i].entity == r->gpu.id;
        chosen = hit * i + !hit * chosen;
    }
    hit = r->gpu.id == 0;
    gpu_module.chosen = hit * gpu_module.default_gpu + !hit * chosen;
    make_device(api, r->features, r->gpu.id);
}

static void settle_window(const bridge_api_t *api)
{
    if (gpu_module.device && gpu_module.window && !gpu_module.surface
        && gpu_module.surface_kind != IDEATEK_SURFACE_NONE)
        make_surface(api);
    if (gpu_module.stale) {
        drop_swapchain();
        gpu_module.stale = false;
    }
    if (gpu_module.device && gpu_module.surface && gpu_module.swap_asked
        && gpu_module.drawable && !gpu_module.swapchain)
        make_swapchain(api);
}

bool settle(const bridge_api_t *api)
{
    if (!gpu_module.started)
        start_gpus(api);
    if (gpu_module.broken)
        return (false);
    if (gpu_module.remake)
        settle_device(api);
    gpu_module.remake |= gpu_module.lost;
    gpu_module.lost = false;
    settle_window(api);
    return (true);
}

void attach_round(const bridge_api_t *api, ideatek_swapchain_status_t status)
{
    ideatek_swapchain_t s = gpu_module.swap_value;
    ideatek_vulkan_swapchain_t v = {
        .image = (uint64_t)gpu_module.swap_images[gpu_module.image_index],
        .view = (uint64_t)gpu_module.views[gpu_module.image_index],
        .format = (int32_t)gpu_module.format,
        .color_space = (int32_t)gpu_module.color_space };

    s.frame = gpu_module.frame;
    s.slot = (uint32_t)(gpu_module.frame % gpu_module.frames);
    s.status = status;
    BRIDGE_NATIVE_ATTACH(api, gpu_module.window, SWAPCHAIN, s);
    if (status == IDEATEK_SWAPCHAIN_STATUS_READY)
        BRIDGE_NATIVE_ATTACH(api, gpu_module.window, VULKAN_SWAPCHAIN, v);
}

BRIDGE_NATIVE(swap)
{
    const bridge_list_t *list = bridge_native_get_list(api,
        gpu_module.swap_list);
    const bridge_trigger_t *t = (const void *)(api->private_base + list->at);
    uint32_t at;

    for (uint32_t i = 0; i < list->count; i++) {
        if (t[i].id != gpu_module.window)
            continue;
        at = bridge_native_find_value(api, t[i].id, SWAPCHAIN_REQUEST);
        gpu_module.swap_request = *(const ideatek_swapchain_request_t *)(
            api->shared_base + at);
        gpu_module.swap_asked = true;
        gpu_module.stale = true;
    }
    return (0);
}
